#include "stdafx.h"
#include "../ExtractBase.h"
#include "Baldr.h"

BOOL	CBaldr::Mount(
	CArcFile*			pclArc							// �A�[�J�C�u
	)
{
	if( pclArc->GetArcExten() != _T(".pac") )
	{
		return	FALSE;
	}

	if( memcmp( pclArc->GetHed(), "PAC", 3 ) != 0 )
	{
		return	FALSE;
	}

	if( memcmp( &pclArc->GetHed()[72], "\0\0\0\0", 4 ) != 0 )
	{
		return	FALSE;
	}

	// �t�@�C�����擾

	DWORD				dwFiles;

	pclArc->Seek( 4, FILE_BEGIN );
	pclArc->Read( &dwFiles, 4 );

	// �t���O�擾

	DWORD				dwFlags;

	pclArc->Read( &dwFlags, 4 );

	// ���k�t�H�[�}�b�g�̎擾

	YCString				clsFormat;

	switch( dwFlags )
	{
	case	0:
		// �����k

		break;

	case	1:
		// LZSS

		clsFormat = _T("LZ");
		break;

	case	2:
		// �s��

		break;

	case	3:
		// ZLIB

		clsFormat = _T("zlib");
		break;
	}

	// �C���f�b�N�X�擾

	YCMemory<SPACFileInfo>	clmpacfiIndex( dwFiles );

	pclArc->Read( &clmpacfiIndex[0], (sizeof(SPACFileInfo) * dwFiles) );

	// �t�@�C�����擾

	for( DWORD i = 0 ; i < dwFiles ; i++ )
	{
		// �t�@�C�����擾

		char				szFileName[65];

		memcpy( szFileName, clmpacfiIndex[i].szFileName, 64 );
		szFileName[64] = '\0';

		if( strlen( szFileName ) <= 4 )
		{
			pclArc->SeekHed();
			return	FALSE;
		}

		// ���X�g�r���[�ɒǉ�

		SFileInfo			stFileInfo;

		stFileInfo.name = szFileName;
		stFileInfo.sizeOrg = clmpacfiIndex[i].dwFileSize;
		stFileInfo.sizeCmp = clmpacfiIndex[i].dwCompFileSize;
		stFileInfo.start = clmpacfiIndex[i].dwOffset;
		stFileInfo.end = stFileInfo.start + stFileInfo.sizeCmp;
		stFileInfo.format = clsFormat;

//		if( (stFileInfo.sizeOrg != infFile.sizeCmp) && (lstrcmp(PathFindExtension(infFile.name), _T(".wav")) != 0))
//			infFile.format = _T("LZ");

		pclArc->AddFileInfo( stFileInfo );
	}

	return	TRUE;
}
