#include "stdafx.h"
#include "../ExtractBase.h"
#include "MeltyBlood.h"

// MELTY BLOOD��data**.p�̃t�@�C�������擾����֐�
BOOL CMeltyBlood::Mount(CArcFile* pclArc)
{
	if ((pclArc->GetHed()[0] != 0x00) && (pclArc->GetHed()[0] != 0x01))
		return FALSE;

	if ((pclArc->GetArcName().Left(5) != _T("data0")) || (pclArc->GetArcExten() != _T(".p")))
		return FALSE;

	DWORD deckey = 0xe3df59ac;

	// �t�@�C�����擾
	DWORD ctFile;
	pclArc->Seek(4, FILE_BEGIN);
	pclArc->Read(&ctFile, 4);
	ctFile ^= deckey;

	// �t�@�C��������C���f�b�N�X�T�C�Y�擾
	DWORD index_size = ctFile * 68;

	// �C���f�b�N�X�擾
	YCMemory<BYTE> index(index_size);
	LPBYTE pIndex = &index[0];
	pclArc->Read(pIndex, index_size);

	for (int i = 0; i < (int)ctFile; i++) {
		// �t�@�C�����擾
		TCHAR szFileName[60];
		memcpy(szFileName, pIndex, 60);
		// �t�@�C��������
		for (int j = 0; j < 59; j++)
			szFileName[j] ^= i * j * 3 + 61;

		// ���X�g�r���[�ɒǉ�
		SFileInfo infFile;
		infFile.name = szFileName;
		infFile.sizeOrg = *(LPDWORD)&pIndex[64] ^ deckey;
		infFile.sizeCmp = infFile.sizeOrg;
		infFile.start = *(LPDWORD)&pIndex[60];
		infFile.end = infFile.start + infFile.sizeOrg;
		infFile.title = _T("MeltyBlood");
		pclArc->AddFileInfo(infFile);

		pIndex += 68;
	}

	return TRUE;
}

// ���o����֐�
BOOL CMeltyBlood::Decode(CArcFile* pclArc)
{
	SFileInfo* pInfFile = pclArc->GetOpenFileInfo();

	if (pInfFile->format != _T("MeltyBlood"))
		return FALSE;

	// �o�̓t�@�C������
	pclArc->OpenFile();

	// 0x00�`0x2173�܂ł̃f�[�^�𕜍����A�o��
	Decrypt(pclArc);

	// 0x2173��������̂܂܏o��
	if (pInfFile->sizeOrg > 0x2173)
		pclArc->ReadWrite(pInfFile->sizeOrg - 0x2173);

	return TRUE;
}

// �f�[�^�𕜍�����֐�
void CMeltyBlood::Decrypt(CArcFile* pclArc)
{
	SFileInfo* pInfFile = pclArc->GetOpenFileInfo();

	// �o�b�t�@�m��
	int lim = (pInfFile->sizeOrg < 0x2173) ? pInfFile->sizeOrg : 0x2173;
	YCMemory<BYTE> buf(lim);

	pclArc->Read(&buf[0], lim);
	// ����
	int keylen = (int)pInfFile->name.GetLength();
	for (int i = 0; i < lim; i++)
		buf[i] ^= pInfFile->name[i % keylen] + i + 3;
	pclArc->WriteFile(&buf[0], lim);
}