#include "stdafx.h"
#include "../ExtractBase.h"
#include "Ykc.h"

BOOL CYkc::Mount(CArcFile* pclArc)
{
	if (memcmp(pclArc->GetHed(), "YKC001", 6) != 0)
		return FALSE;

	// �C���f�b�N�X�ւ̃I�t�Z�b�g�ƃC���f�b�N�X�̃T�C�Y�̎擾

	DWORD				dwIndexOffset;
	DWORD				dwIndexSize;

	pclArc->SeekHed(0x10);
	pclArc->Read(&dwIndexOffset, 4);
	pclArc->Read(&dwIndexSize, 4);

	// �C���f�b�N�X�̎擾

	YCMemory<BYTE>		clmbtIndex(dwIndexSize);

	pclArc->SeekHed(dwIndexOffset);
	pclArc->Read(&clmbtIndex[0], dwIndexSize);

	// �t�@�C�����C���f�b�N�X�ւ̃I�t�Z�b�g�ƃt�@�C�����C���f�b�N�X�̃T�C�Y�̎擾

	DWORD				dwFileNameIndexOffset;
	DWORD				dwFileNameIndexSize;

	dwFileNameIndexOffset = *(LPDWORD)&clmbtIndex[0];
	dwFileNameIndexSize = dwIndexOffset - dwFileNameIndexOffset;

	// �t�@�C�����C���f�b�N�X�̎擾

	YCMemory<BYTE> clmbtFileNameIndex(dwFileNameIndexSize);

	pclArc->SeekHed(dwFileNameIndexOffset);
	pclArc->Read(&clmbtFileNameIndex[0], dwFileNameIndexSize);

	// �t�@�C�����̎擾

	for (DWORD i = 0, j = 0; i < dwIndexSize; i += 20) {
		// �t�@�C�����̒����̎擾

		DWORD				dwFileNameLen;

		dwFileNameLen = *(LPDWORD)&clmbtIndex[i + 4];

		// �t�@�C�����̎擾

		TCHAR				szFileName[_MAX_FNAME];

		lstrcpy(szFileName, (LPCTSTR)&clmbtFileNameIndex[j]);
		j += dwFileNameLen;

		// �t�@�C�����̎擾

		SFileInfo			stfiWork;

		stfiWork.name = szFileName;
		stfiWork.start = *(LPDWORD)&clmbtIndex[i + 8];
		stfiWork.sizeCmp = *(LPDWORD)&clmbtIndex[i + 12];
		stfiWork.sizeOrg = stfiWork.sizeCmp;
		stfiWork.end = stfiWork.start + stfiWork.sizeCmp;

		pclArc->AddFileInfo(stfiWork);
	}

	return TRUE;
}

BOOL CYkc::Decode(CArcFile* pclArc)
{
	if (memcmp(pclArc->GetHed(), "YKC001", 6) != 0)
		return FALSE;

	if (DecodeYKS(pclArc) == TRUE)
		return TRUE;

	if (DecodeYKG(pclArc) == TRUE)
		return TRUE;

	return FALSE;
}

BOOL CYkc::DecodeYKS(CArcFile* pclArc)
{
	SFileInfo*			pstfiWork = pclArc->GetOpenFileInfo();

	if (pstfiWork->format != _T("YKS"))
		return FALSE;

	// YKS�t�@�C���̓ǂݍ���

	YCMemory<BYTE>		clmbtSrc(pstfiWork->sizeCmp);

	pclArc->Read(&clmbtSrc[0], pstfiWork->sizeCmp);

	if (memcmp(&clmbtSrc[0], "YKS001", 6) == 0) {
		// YKS001

		// �e�L�X�g�����ւ̃I�t�Z�b�g�̎擾

		DWORD				dwTextOffset;

		dwTextOffset = *(LPDWORD)&clmbtSrc[0x20];

		// �e�L�X�g�����̕���

		for (DWORD i = dwTextOffset; i < pstfiWork->sizeCmp; i++) {
			clmbtSrc[i] ^= 0xAA;
		}

		// �o��

		pclArc->OpenScriptFile();
		pclArc->WriteFile(&clmbtSrc[0], pstfiWork->sizeCmp);
	}
	else {
		// ���̑�

		pclArc->OpenFile();
		pclArc->WriteFile(&clmbtSrc[0], pstfiWork->sizeCmp);
	}

	return TRUE;
}

BOOL CYkc::DecodeYKG(CArcFile* pclArc)
{
	SFileInfo*			pstfiWork = pclArc->GetOpenFileInfo();

	if (pstfiWork->format != _T("YKG"))
		return FALSE;

	// YKG�t�@�C���̓ǂݍ���

	YCMemory<BYTE>		clmbtSrc(pstfiWork->sizeCmp);

	pclArc->Read(&clmbtSrc[0], pstfiWork->sizeCmp);

	if (memcmp(&clmbtSrc[0], "YKG000", 6) == 0) {
		// YKG000

		// PNG�w�b�_�̏C��

		memcpy(&clmbtSrc[0x41], "PNG", 3);

		// �o��

		pclArc->OpenFile( _T(".png") );
		pclArc->WriteFile( &clmbtSrc[0x40], pstfiWork->sizeCmp - 0x40 );
	}
	else {
		// ���̑�

		pclArc->OpenFile();
		pclArc->WriteFile(&clmbtSrc[0], pstfiWork->sizeCmp);
	}

	return TRUE;
}