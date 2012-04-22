#include "stdafx.h"
#include "../ExtractBase.h"
#include "../Image.h"
#include "InnocentGrey.h"

BOOL CInnocentGrey::Mount(CArcFile* pclArc)
{
	if ((pclArc->GetArcExten() != _T(".dat")) || (memcmp(pclArc->GetHed(), "PACKDAT.", 8) != 0))
		return FALSE;

	// �t�@�C�����擾
	DWORD ctFile;
	pclArc->Seek(8, FILE_BEGIN);
	pclArc->Read(&ctFile, 4);
	// �t�@�C��������C���f�b�N�X�T�C�Y�擾
	DWORD index_size = ctFile * 48;
	// �C���f�b�N�X�擾
	YCMemory<BYTE> index(index_size);
	LPBYTE pIndex = &index[0];
	pclArc->Seek(4, FILE_CURRENT);
	pclArc->Read(pIndex, index_size);

	for (int i = 0; i < (int)ctFile; i++) {
		// �t�@�C�����擾
		TCHAR szFileName[32];
		memcpy(szFileName, pIndex, 32);

		// ���X�g�r���[�ɒǉ�
		SFileInfo infFile;
		infFile.name = szFileName;
		infFile.start = *(LPDWORD)&pIndex[32];
		infFile.sizeOrg = *(LPDWORD)&pIndex[40];
		infFile.sizeCmp = *(LPDWORD)&pIndex[44];
		infFile.end = infFile.start + infFile.sizeCmp;
		infFile.title = _T("InnocentGrey");
		pclArc->AddFileInfo(infFile);

		pIndex += 48;
	}

	return TRUE;
}

BOOL CInnocentGrey::Decode(CArcFile* pclArc)
{
	SFileInfo* pInfFile = pclArc->GetOpenFileInfo();

	if ((pInfFile->title != _T("InnocentGrey")) || (pclArc->GetArcExten() != _T(".dat")) || (pInfFile->format != _T("S")))
		return FALSE;

	if (pInfFile->format == _T("BMP")) {
		// �ǂݍ���
		YCMemory<BYTE> buf(pInfFile->sizeOrg);
		pclArc->Read(&buf[0], pInfFile->sizeOrg);
		// ����
		for (int i = 0; i < (int)pInfFile->sizeOrg; i++)
			buf[i] ^= 0xFF;
		// �o��
		CImage image;
		image.Init( pclArc, &buf[0] );
		image.Write( pInfFile->sizeOrg );
	}
	else {
		// �o�b�t�@�m��
		DWORD BufSize = pclArc->GetBufSize();
		YCMemory<BYTE> buf(BufSize);

		// �o�̓t�@�C������
		pclArc->OpenFile();

		for (DWORD WriteSize = 0; WriteSize != pInfFile->sizeOrg; WriteSize += BufSize) {
			// �o�b�t�@�T�C�Y����
			pclArc->SetBufSize(&BufSize, WriteSize);

			// �������ďo��
			pclArc->Read(&buf[0], BufSize);
			for (int i = 0; i < (int)BufSize; i++)
				buf[i] ^= 0xFF;
			pclArc->WriteFile(&buf[0], BufSize);
		}
	}

	return TRUE;
}