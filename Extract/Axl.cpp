#include "stdafx.h"
#include "../ExtractBase.h"
#include "../Arc/LZSS.h"
#include "../Image.h"
#include "Axl.h"

BOOL CAxl::Mount(CArcFile* pclArc)
{
	if (pclArc->GetArcExten() != _T(".arc"))
		return FALSE;
	if (*(LPDWORD)&pclArc->GetHed()[4] != 0 && *(LPDWORD)&pclArc->GetHed()[4] != 1)
		return FALSE;

	// �t�@�C�����擾
	DWORD ctFile = *(LPDWORD)&pclArc->GetHed()[0];

	// �t�@�C���^�C�v�擾
	DWORD type = *(LPDWORD)&pclArc->GetHed()[4];

	// �t�@�C��������C���f�b�N�X�T�C�Y�擾
	DWORD index_size = ctFile * 44;

	// �A�[�J�C�u�t�@�C���T�C�Y���C���f�b�N�X�T�C�Y��菬���������疳�֌W�̃A�[�J�C�u�t�@�C���Ɣ���
	if (pclArc->GetArcSize() < index_size)
		return FALSE;

	pclArc->Seek(8, FILE_BEGIN);

	// �C���f�b�N�X�擾
	YCMemory<BYTE> index(index_size);
	LPBYTE pIndex = &index[0];
	pclArc->Read(pIndex, index_size);

	// �C���f�b�N�X����
	if (DecryptIndex(pIndex, index_size, pclArc->GetArcSize()) == FALSE) {
		// �����ł��Ȃ������疳�֌W�̃A�[�J�C�u�t�@�C���Ɣ���
		pclArc->SeekHed();
		return FALSE;
	}

	for (int i = 0; i < (int)ctFile; i++) {
		// �t�@�C�����擾
		TCHAR szFileName[33];
		memcpy(szFileName, pIndex, 32);
		szFileName[32] = _T('\0');

		// ���X�g�r���[�ɒǉ�
		SFileInfo infFile;
		infFile.name = szFileName;
		infFile.sizeOrg = *(LPDWORD)&pIndex[32];
		infFile.sizeCmp = *(LPDWORD)&pIndex[36];
		infFile.start = *(LPDWORD)&pIndex[40];
		infFile.end = infFile.start + infFile.sizeCmp;
		if (type == 1) infFile.format = _T("LZ");
		infFile.title = _T("AXL");
		pclArc->AddFileInfo(infFile);

		pIndex += 44;
	}

	return TRUE;
}

BOOL CAxl::Decode(CArcFile* pclArc)
{
	SFileInfo* pInfFile = pclArc->GetOpenFileInfo();

	if (pInfFile->title != _T("AXL"))
		return FALSE;

	// �o�b�t�@�m��
	YCMemory<BYTE> buf(pInfFile->sizeOrg);

	if (pInfFile->format == _T("LZ")) {
		// LZ���k�t�@�C��

		// �ǂݍ���
		YCMemory<BYTE> z_buf(pInfFile->sizeCmp);
		pclArc->Read(&z_buf[0], pInfFile->sizeCmp);

		// LZSS��

		CLZSS				clLZSS;

		clLZSS.Decomp( &buf[0], pInfFile->sizeOrg, &z_buf[0], pInfFile->sizeCmp, 4096, 4078, 3 );
	}
	else {
		// �����k�t�@�C��

		// �ǂݍ���
		pclArc->Read(&buf[0], pInfFile->sizeCmp);
	}

	if (lstrcmpi(PathFindExtension(pInfFile->name), _T(".bmp")) == 0) {
		// BMP�t�@�C��

		LPBITMAPFILEHEADER fHed = (LPBITMAPFILEHEADER)&buf[0];
		LPBITMAPINFOHEADER iHed = (LPBITMAPINFOHEADER)&buf[14];

		if (fHed->bfSize != pInfFile->sizeOrg) {
			// 32bit��BMP

			// 32bit��BMP���\�z����o�b�t�@
			YCMemory<BYTE> buf2(pInfFile->sizeOrg - 54);

			// ���l���w��
			LPBYTE pbufA = &buf[fHed->bfSize];

			// BMP�w�b�_���΂�
			LPBYTE pbuf = &buf[54];

			LPBYTE pbuf2 = &buf2[0];

			DWORD color_size = iHed->biWidth * iHed->biHeight;

			for (int i = 0; i < (int)color_size; i++) {
				// ���l����
				for (int j = 0; j < 3; j++)
					*pbuf2++ = *pbuf++;
				*pbuf2++ = *pbufA++;
			}

			CImage image;
			image.Init(pclArc, iHed->biWidth, iHed->biHeight, 32);
			image.Write(&buf2[0], pInfFile->sizeOrg - 54);
		}
		else {
			// 24bit�ȉ���BMP

			CImage image;
			image.Init(pclArc, &buf[0]);
			image.Write(pInfFile->sizeOrg);
		}
	}
	else if (pInfFile->format == _T("LZ")) {
		// BMP�ȊO��LZ���k�t�@�C��

		pclArc->OpenFile();
		pclArc->WriteFile(&buf[0], pInfFile->sizeOrg);
	}
	else {
		// ���̑��̃t�@�C��

		pclArc->OpenFile();
		pclArc->WriteFile(&buf[0], pInfFile->sizeCmp);
	}

	return TRUE;
}

void CAxl::InitMountKey(LPVOID deckey)
{
	m_len = strlen((char*)deckey);
	memcpy(m_deckey, deckey, m_len);
}

BOOL CAxl::CreateKey(LPBYTE key, LPINT key_len, LPBYTE pIndex, DWORD index_size)
{
	for (int i = 0; i < (int)index_size; i += 44) {
		// �C���f�b�N�X����t�@�C�����̕������R�s�[
		BYTE fname[32];
		memcpy(fname, pIndex, 32);

		LPBYTE key_end = &fname[31];

		LPBYTE pkey1 = key_end;
		LPBYTE pkey2 = key_end - 1;

		*key_len = 1;

		while (pkey2 > fname) {
			while (*pkey1 != *pkey2) {
				// �����f�[�^���o��܂őO�Ɉړ�
				pkey2--;

				// key�̒����𐔂��Ă���
				(*key_len)++;

				// �t�@�C�����̐擪�܂Ŗ߂����烋�[�v�𔲂���
				if (pkey2 == fname)
					break;
			}

			// �t�@�C�����̐擪�܂Ŗ߂����烋�[�v�𔲂���
			if (pkey2 == fname)
				break;

			int key_len_cpy = *key_len;

			// �O�̃f�[�^�Ƃ̔�r���s��
			while (*pkey1 == *pkey2) {
				pkey1--;
				pkey2--;
				key_len_cpy--;

				// ���S��v������L�[����
				if (key_len_cpy == 0) {
					// �ŏ��ɃR�s�[����L�[�̒���
					int key_len_hed = (i + 32) % *key_len;
					LPBYTE pkey3 = key_end + 1 - key_len_hed;

					for (int j = 0; j < key_len_hed; j++)
						key[j] = *pkey3++;

					// �����̃L�[
					pkey3 = key_end + 1 - *key_len;

					for (int j = key_len_hed; j < *key_len; j++)
						key[j] = *pkey3++;

					return TRUE;
				}

				// �t�@�C�����̐擪�܂Ŗ߂����烋�[�v�𔲂���
				if (pkey2 == fname)
					break;
			}

			// �t�@�C�����̐擪�܂Ŗ߂����烋�[�v�𔲂���
			if (pkey2 == fname)
				break;
		}

		pIndex += 44;
	}

	return FALSE;
}

BOOL CAxl::DecryptIndex(LPBYTE pIndex, DWORD index_size, QWORD arcSize)
{
	// �C���f�b�N�X�̃t�@�C������������L�[����
	BYTE key[32];
	int key_len;
	if (CreateKey(key, &key_len, pIndex, index_size) == FALSE)
		return FALSE;

	// �C���f�b�N�X���R�s�[
	YCMemory<BYTE> pIndex_copy(index_size);
	memcpy(&pIndex_copy[0], pIndex, index_size);

	// �R�s�[�����C���f�b�N�X�𕜍�
	for (int i = 0, j = 0; i < (int)index_size; i++) {
		pIndex_copy[i] -= key[j++];
		if (j == key_len) j = 0;
	}

	// �ŏ��̃t�@�C���̎n�_�ƃC���f�b�N�X�̏I�_����v���邩�`�F�b�N
	if (*(LPDWORD)&pIndex_copy[40] != 8 + index_size)
		return FALSE;

	// �ŏI�t�@�C���̏I�_�ƃA�[�J�C�u�t�@�C���̏I�_����v���邩�`�F�b�N
	if (*(LPDWORD)&pIndex_copy[index_size-4] + *(LPDWORD)&pIndex_copy[index_size-8] != arcSize)
		return FALSE;

	// �`�F�b�N�̌��ʂ����Ȃ���Ε��������C���f�b�N�X���R�s�[
	memcpy(pIndex, &pIndex_copy[0], index_size);

	return TRUE;

/*
	static char* key[] = {
		"SUMMER",		// �`���[�g���A���T�}�[
		"HIDAMARI",		// �Ђ��܂�
		"KIMIKOE",		// �L�~�̐�����������
		"CFTRIAL",		// CROSS FIRE�̌���v1
		"TEST",			// CROSS FIRE, CROSS FIRE�̌���v2
		"SAKURA-R",		// �����烊���N�[�[�V����
		"KOICHU",		// ��������I �`���ɗ����邩���������`
		"LAVENDURA",	// �t���̃����F���f����
		"HAMA",			// �^�� ������, ����̂���A�݂��̂���
		"OBA",			// �f��̐Q��
		"KANSEN"		// ���� �`���~�̘A���`
	};

	YCMemory<BYTE> pIndex_copy(index_size);

	for (int i = 0; i < _countof(key); i++) {
		// �C���f�b�N�X���R�s�[
		memcpy(&pIndex_copy[0], pIndex, index_size);
		// key�̒��������߂Ă���
		int key_len = strlen((char*)key[i]);
		// ����
		for (int j = 0, k = 0; j < (int)index_size; j++) {
			pIndex_copy[j] += (BYTE)key[i][k++];
			if (k == key_len)
				k = 0;
		}
		// ���t�@�C���̎n�_�ƃC���f�b�N�X�̏I�_����v���邩�`�F�b�N
		// ��v���Ȃ�������ʂ�key�ŕ�������
		if (*(LPDWORD)&pIndex_copy[40] != 8 + index_size)
			continue;
		// �ŏI�t�@�C���̏I�_�ƃA�[�J�C�u�t�@�C���̏I�_����v���邩�`�F�b�N
		// �������v������Ή����Ă���Ɣ���
		if (*(LPDWORD)&pIndex_copy[index_size-4] + *(LPDWORD)&pIndex_copy[index_size-8] == arcSize) {
			memcpy(pIndex, &pIndex_copy[0], index_size);
			return TRUE;
		}
	}

	return FALSE;
*/


	//LPBYTE deckey = m_deckey;
	//DWORD deckey_len = m_len;

	//for (int i = 0, j = 0; i < (int)index_size; i++) {
	//	pIndex[i] += deckey[j++];
	//	if (j == deckey_len) j = 0;
	//}
}