#include "stdafx.h"
#include "../ExtractBase.h"
#include "../Wav.h"
#include "TrH.h"

BOOL CTrH::Mount(CArcFile* pclArc)
{
	if ((pclArc->GetArcExten() != _T(".px")) || (memcmp(pclArc->GetHed(), "fPX ", 4) != 0))
		return FALSE;
	return pclArc->Mount();
}

// WAV�ɕϊ����Ē��o����֐�
BOOL CTrH::Decode(CArcFile* pclArc)
{
	SFileInfo* pInfFile = pclArc->GetOpenFileInfo();

	if ((pInfFile->format != _T("PX")) || (memcmp(pclArc->GetHed(), "fPX ", 4) != 0))
		return FALSE;

	// px�w�b�_�ǂݍ���
	PXHed pxHed;
	pclArc->Read(&pxHed, sizeof(PXHed));

	// �o��
	CWav wav;
	wav.Init(pclArc, pInfFile->sizeOrg - 44, pxHed.freq, pxHed.channels, pxHed.bits);
	wav.Write();

	return TRUE;
}