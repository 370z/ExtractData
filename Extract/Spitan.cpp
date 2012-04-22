#include "stdafx.h"
#include "../ExtractBase.h"
#include "Spitan.h"

BOOL CSpitan::Mount(CArcFile* pclArc)
{
	if (MountSound(pclArc) == TRUE)
		return TRUE;
	if (MountGraphic1(pclArc) == TRUE)
		return TRUE;
	if (MountGraphic2(pclArc) == TRUE)
		return TRUE;
	if (MountGraphic3(pclArc) == TRUE)
		return TRUE;
	return FALSE;
}

// �X�s�����bm0000, bv00**, k000*�̃t�@�C�������擾����֐�
BOOL CSpitan::MountSound(CArcFile* pclArc)
{
	if ((pclArc->GetArcName().Left(6) != _T("bm0000")) && (pclArc->GetArcName().Left(4) != _T("bv00")) && (pclArc->GetArcName().Left(4) != _T("k000")))
		return FALSE;

	pclArc->Seek(4, FILE_BEGIN);

	for (int i = 1; ; i++) {
		BYTE header[16];
		pclArc->Read(header, sizeof(header));

		if (memcmp(header, "OggS", 4) == 0)
			break;
		if (memcmp(header, "\0\0\0\0", 4) == 0)
			break;

		// �t�@�C�����擾
		TCHAR szFileName[_MAX_FNAME];
		_stprintf(szFileName, _T("%s_%06d.ogg"), pclArc->GetArcName(), i);

		// ���X�g�r���[�ɒǉ�
		SFileInfo infFile;
		infFile.name = szFileName;
		infFile.sizeCmp = *(LPDWORD)&header[0];
		infFile.sizeOrg = infFile.sizeCmp;
		infFile.start = *(LPDWORD)&header[4];
		infFile.end = infFile.start + infFile.sizeCmp;
		pclArc->AddFileInfo(infFile);
	}

	return TRUE;
}

// �X�s�����*.aif�̃t�@�C�������擾����֐�
BOOL CSpitan::MountGraphic1(CArcFile* pclArc)
{
	if (pclArc->GetArcExten() != _T(".aif"))
		return FALSE;
	if (memcmp(pclArc->GetHed(), "\x10\0\0\0", 4) != 0)
		return FALSE;

	for (int i = 1; ; i++) {
		BYTE header[32];
		pclArc->Read(header, sizeof(header));

		if (memcmp(header, "�PNG", 4) == 0)
			break;

		// �t�@�C�����擾
		TCHAR szFileName[_MAX_FNAME];
		_stprintf(szFileName, _T("%s_%06d.png"), pclArc->GetArcName(), i);

		// ���X�g�r���[�ɒǉ�
		SFileInfo infFile;
		infFile.name = szFileName;
		infFile.start = *(LPDWORD)&header[4];
		infFile.sizeCmp = *(LPDWORD)&header[8];
		infFile.sizeOrg = infFile.sizeCmp;
		infFile.end = infFile.start + infFile.sizeCmp;
		pclArc->AddFileInfo(infFile);
	}

	return TRUE;
}

// �X�s�����f0*�̃t�@�C�������擾����֐�
BOOL CSpitan::MountGraphic2(CArcFile* pclArc)
{
	BOOL flag = FALSE;
	for (int i = 0; i < 5; i++) {
		TCHAR szArcName[_MAX_FNAME];
		_stprintf(szArcName, _T("f%02d"), i);
		if (lstrcmp(pclArc->GetArcName(), szArcName) == 0)
			flag = TRUE;
	}
	if (flag == FALSE)
		return FALSE;

	// .field�t�@�C���̃t�@�C�����擾
	std::vector<FileInfo> FileInfoList;
	while (1) {
		BYTE header[8];
		pclArc->Read(header, sizeof(header));

		if (memcmp(header, ".field", 6) == 0)
			break;
		if (memcmp(header, "\0\0\0\0\0\0", 6) == 0)
			break;

		FileInfo file;
		file.size = *(LPDWORD)&header[0];
		file.start = *(LPDWORD)&header[4];
		FileInfoList.push_back(file);
	}

	for (int i = 0, ctFile = 1; i < (int)FileInfoList.size(); i++) {
		// PNG�t�@�C���̑��΃A�h���X�ƃt�@�C���T�C�Y�擾
		FileInfo file[5];

		pclArc->Seek(FileInfoList[i].start + 24, FILE_BEGIN);
		for (int j = 0; j < 2; j++) {
			pclArc->Read(&file[j].start, 4);
			pclArc->Read(&file[j].size, 4);
		}

		pclArc->Read(&file[2].start, 4);
		// 3�ڂ�PNG�t�@�C���̃t�@�C���T�C�Y�́A3�ڂ�PNG�w�b�_�̒��O�ɏ����Ă��邽�߁A�����܂Ŕ��
		QWORD tmp_pos = pclArc->GetArcPointer();
		pclArc->Seek(FileInfoList[i].start + file[2].start, FILE_BEGIN);

		int count = 0;
		while (1) {
			// ����ReadFile�������𐔂��邱�Ƃɂ���āA3�ڂ�PNG�w�b�_�̑��΃A�h���X�����߂邱�Ƃ��ł���
			count++;

			DWORD filesize;
			pclArc->Read(&filesize, 4);
			// PNG�w�b�_��������A����.field�t�@�C���̒���PNG�t�@�C������4��(�ʏ��5��)
			if (filesize == 0x474E5089) {
				file[2].size = 0;
				break;
			}
			// 30�o�C�g�ȏゾ������t�@�C���T�C�Y�Ɣ���
			else if (filesize >= 30) {
				file[2].size = filesize;
				file[2].start += 4 * count;
				break;
			}
		}

		pclArc->Seek(tmp_pos, FILE_BEGIN);
		for (int j = 3; j < 5; j++) {
			pclArc->Read(&file[j].start, 4);
			pclArc->Read(&file[j].size, 4);
		}

		for (int j = 0; j < 5; j++) {
			// �t�@�C���T�C�Y��0��������X�L�b�v
			if (file[j].size == 0)
				continue;

			// �t�@�C�����擾
			TCHAR szFileName[_MAX_FNAME];
			_stprintf(szFileName, _T("%s_%06d.png"), pclArc->GetArcName(), ctFile++);

			// ���X�g�r���[�ɒǉ�
			SFileInfo infFile;
			infFile.name = szFileName;
			infFile.sizeCmp = file[j].size;
			infFile.sizeOrg = infFile.sizeCmp;
			infFile.start = FileInfoList[i].start + file[j].start;
			infFile.end = infFile.start + infFile.sizeCmp;
			pclArc->AddFileInfo(infFile);

			pclArc->Seek(infFile.sizeCmp, FILE_CURRENT);
		}
	}

	return TRUE;
}

// �X�s�����b0*, c0*, IGNR****�̃t�@�C�������擾����֐�
BOOL CSpitan::MountGraphic3(CArcFile* pclArc)
{
	BOOL flag = FALSE;
	if (pclArc->GetArcName().Left(4) == _T("IGNR"))
		flag = TRUE;
	for (int i = 0; i < 5; i++) {
		TCHAR szArcName[_MAX_FNAME];
		_stprintf(szArcName, _T("b%02d"), i);
		if( pclArc->GetArcName() == szArcName )
			flag = TRUE;
	}
	for (int i = 0; i < 2; i++) {
		TCHAR szArcName[_MAX_FNAME];
		_stprintf(szArcName, _T("c%02d"), i);
		if( pclArc->GetArcName() == szArcName )
			flag = TRUE;
	}
	if (flag == FALSE)
		return FALSE;

	std::vector<FileInfo> FileInfoList;

	// NORI�t�@�C���̃t�@�C�����擾
	while (1) {
		BYTE header[8];
		pclArc->Read(header, sizeof(header));

		if (memcmp(header, "NORI", 4) == 0)
			break;
		if (memcmp(header, "\0\0\0\0", 4) == 0)
			break;

		FileInfo file;
		file.size = *(LPDWORD)&header[0];
		file.start = *(LPDWORD)&header[4];
		FileInfoList.push_back(file);
	}

	for (int i = 0, ctFile = 1; i < (int)FileInfoList.size(); i++) {
		// NORI�t�@�C���̒��ɂ���PNG�t�@�C�����擾
		DWORD ctPng;
		pclArc->Seek(FileInfoList[i].start + 0x4C, FILE_BEGIN);
		pclArc->Read(&ctPng, 4);

		pclArc->Seek(0x40 - 0x1C, FILE_CURRENT);

		for (int j = 0; j < (int)ctPng; j++) {
			pclArc->Seek(0x1C, FILE_CURRENT);

			// 1�ڂ�2�ڂ̃t�@�C���T�C�Y�擾
			DWORD filesize[2];
			for (int k = 0; k < 2; k++)
				pclArc->Read(&filesize[k], 4);

			for (int k = 0; k < 2; k++) {
				// �t�@�C���T�C�Y��0��������X�L�b�v
				if (filesize[k] == 0)
					continue;

				// �t�@�C�����擾
				TCHAR szFileName[_MAX_FNAME];
				_stprintf(szFileName, _T("%s_%06d.png"), pclArc->GetArcName(), ctFile++);

				// ���X�g�r���[�ɒǉ�
				SFileInfo infFile;
				infFile.name = szFileName;
				infFile.sizeCmp = filesize[k];
				infFile.sizeOrg = infFile.sizeCmp;
				infFile.start = pclArc->GetArcPointer();
				infFile.end = infFile.start + infFile.sizeCmp;
				pclArc->AddFileInfo(infFile);

				pclArc->Seek(infFile.sizeCmp, FILE_CURRENT);
			}
		}
	}

	return TRUE;
}