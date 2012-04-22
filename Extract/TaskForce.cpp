
#include	"stdafx.h"
#include	"../Arc/LZSS.h"
#include	"../Image.h"
#include	"../Tga.h"
#include	"TaskForce.h"

//////////////////////////////////////////////////////////////////////////////////////////
//	�}�E���g

BOOL	CTaskForce::Mount(
	CArcFile*			pclArc							// �A�[�J�C�u
	)
{
	if( MountDat( pclArc ) )
	{
		return	TRUE;
	}

	if( MountTlz( pclArc ) )
	{
		return	TRUE;
	}

	if( MountBma( pclArc ) )
	{
		return	TRUE;
	}

	return	FALSE;
}

//////////////////////////////////////////////////////////////////////////////////////////
//	dat�̃}�E���g

BOOL	CTaskForce::MountDat(
	CArcFile*			pclArc							// �A�[�J�C�u
	)
{
	if( pclArc->GetArcExten() != _T(".dat") )
	{
		return	FALSE;
	}

	if( memcmp( pclArc->GetHed(), "tskforce", 8 ) != 0 )
	{
		return	FALSE;
	}

	pclArc->SeekHed( 8 );

	// �t�@�C�����̎擾

	DWORD				dwFiles;

	pclArc->Read( &dwFiles, 4 );

	// �C���f�b�N�X�̎擾

	YCMemory<SFileEntry>	clmIndex( dwFiles );

	pclArc->Read( &clmIndex[0], (sizeof(SFileEntry) * dwFiles) );

	// �t�@�C�����̎擾

	DWORD				dwIndexPtr = 0;

	for( DWORD i = 0 ; i < dwFiles ; i++ )
	{
		SFileInfo			stFileInfo;

		stFileInfo.name = clmIndex[i].szFileName;
		stFileInfo.sizeCmp = clmIndex[i].dwCompressedSize;
		stFileInfo.sizeOrg = clmIndex[i].dwOriginalSize;
		stFileInfo.start = clmIndex[i].dwOffset;
		stFileInfo.end = stFileInfo.start + stFileInfo.sizeCmp;

		if( stFileInfo.sizeCmp != stFileInfo.sizeOrg )
		{
			stFileInfo.format = _T("LZ");
		}

		pclArc->AddFileInfo( stFileInfo );

		dwIndexPtr += sizeof(SFileEntry);
	}

	return	TRUE;
}

//////////////////////////////////////////////////////////////////////////////////////////
//	tlz�̃}�E���g

BOOL	CTaskForce::MountTlz(
	CArcFile*			pclArc							// �A�[�J�C�u
	)
{
	if( (pclArc->GetArcExten() != _T(".tsk")) && (pclArc->GetArcExten() != _T(".tfz")) )
	{
		return	FALSE;
	}

	if( memcmp( pclArc->GetHed(), "tlz", 3 ) != 0 )
	{
		return	FALSE;
	}

	return	pclArc->Mount();
}

//////////////////////////////////////////////////////////////////////////////////////////
//	bma�̃}�E���g

BOOL	CTaskForce::MountBma(
	CArcFile*			pclArc							// �A�[�J�C�u
	)
{
	if( pclArc->GetArcExten() != _T(".tsz") )
	{
		return	FALSE;
	}

	if( memcmp( pclArc->GetHed(), "bma", 3 ) != 0 )
	{
		return	FALSE;
	}

	return	pclArc->Mount();
}

//////////////////////////////////////////////////////////////////////////////////////////
//	�f�R�[�h

BOOL	CTaskForce::Decode(
	CArcFile*			pclArc							// �A�[�J�C�u
	)
{
	if( DecodeTlz( pclArc ) )
	{
		return	TRUE;
	}

	if( DecodeBma( pclArc ) )
	{
		return	TRUE;
	}

	if( DecodeTGA( pclArc ) )
	{
		return	TRUE;
	}

	return	FALSE;
}

//////////////////////////////////////////////////////////////////////////////////////////
//	tlz�̃f�R�[�h

BOOL	CTaskForce::DecodeTlz(
	CArcFile*			pclArc							// �A�[�J�C�u
	)
{
	SFileInfo*			pstFileInfo = pclArc->GetOpenFileInfo();

	if( (pstFileInfo->name.GetFileExt() != _T(".tsk")) && (pstFileInfo->name.GetFileExt() != _T(".tfz")) )
	{
		return	FALSE;
	}

	// �w�b�_�̓ǂݍ���

	BYTE				abtHeader[24];

	pclArc->Read( abtHeader, sizeof(abtHeader) );

	if( memcmp( abtHeader, "tlz", 3 ) != 0 )
	{
		pclArc->SeekHed( pstFileInfo->start );
		return	FALSE;
	}

	// �t�@�C�����̎擾

	DWORD				dwDstSize = *(DWORD*) &abtHeader[16];
	DWORD				dwSrcSize = *(DWORD*) &abtHeader[20];

	// ���k�f�[�^�̓ǂݍ���

	YCMemory<BYTE>		clmSrc( dwSrcSize );

	pclArc->Read( &clmSrc[0], dwSrcSize );

	// �𓀌�̃o�b�t�@�̊m��

	YCMemory<BYTE>		clmDst( dwDstSize );

	// LZSS�̉�

	CLZSS				clLZSS;

	clLZSS.Decomp( &clmDst[0], dwDstSize, &clmSrc[0], dwSrcSize, 4096, 4078, 3 );

	// �o��

	pclArc->OpenFile();
	pclArc->WriteFile( &clmDst[0], dwDstSize, dwSrcSize );
	pclArc->CloseFile();

	return	TRUE;
}

//////////////////////////////////////////////////////////////////////////////////////////
//	bma�̃f�R�[�h

BOOL	CTaskForce::DecodeBma(
	CArcFile*			pclArc							// �A�[�J�C�u
	)
{
	SFileInfo*			pstFileInfo = pclArc->GetOpenFileInfo();

	if( pstFileInfo->name.GetFileExt() != _T(".tsz") )
	{
		return	FALSE;
	}

	// �w�b�_�̓ǂݍ���

	BYTE				abtHeader[24];

	pclArc->Read( abtHeader, sizeof(abtHeader) );

	if( memcmp( abtHeader, "bma", 3 ) != 0 )
	{
		pclArc->SeekHed( pstFileInfo->start );
		return	FALSE;
	}

	// �t�@�C�����̎擾

	long				lWidth = *(long*) &abtHeader[4];
	long				lHeight = *(long*) &abtHeader[8];
	DWORD				dwDstSize = *(DWORD*) &abtHeader[16];
	DWORD				dwSrcSize = *(DWORD*) &abtHeader[20];

	// ���k�f�[�^�̓ǂݍ���

	YCMemory<BYTE>		clmSrc( dwSrcSize );

	pclArc->Read( &clmSrc[0], dwSrcSize );

	// �𓀌�̃o�b�t�@�̊m��

	YCMemory<BYTE>		clmDst( dwDstSize );

	// LZSS�̉�

	CLZSS				clLZSS;

	clLZSS.Decomp( &clmDst[0], dwDstSize, &clmSrc[0], dwSrcSize, 4096, 4078, 3 );

	// �o��

	CImage				clImage;

	clImage.Init( pclArc, lWidth, lHeight, 32 );
	clImage.WriteReverse( &clmDst[0], dwDstSize );
	clImage.Close();

	return	TRUE;
}

//////////////////////////////////////////////////////////////////////////////////////////
//	TGA�̃f�R�[�h

BOOL	CTaskForce::DecodeTGA(
	CArcFile*			pclArc							// �A�[�J�C�u
	)
{
	SFileInfo*			pstFileInfo = pclArc->GetOpenFileInfo();

	if( pstFileInfo->name.GetFileExt() != _T(".tga") )
	{
		return	FALSE;
	}

	// �f�[�^�̓ǂݍ���

	DWORD				dwSrcSize = pstFileInfo->sizeCmp;

	YCMemory<BYTE>		clmSrc( dwSrcSize );

	pclArc->Read( &clmSrc[0], dwSrcSize );

	if( pstFileInfo->format == _T("LZ") )
	{
		// ���k����Ă���

		DWORD				dwDstSize = pstFileInfo->sizeOrg;

		YCMemory<BYTE>		clmDst( dwDstSize );

		// LZSS�̉�

		CLZSS				clLZSS;

		clLZSS.Decomp( &clmDst[0], dwDstSize, &clmSrc[0], dwSrcSize, 4096, 4078, 3 );

		// �o��

		CTga				clTGA;

		clTGA.Decode( pclArc, &clmDst[0], dwDstSize );
	}
	else
	{
		// �����k

		CTga				clTGA;

		clTGA.Decode( pclArc, &clmSrc[0], dwSrcSize );
	}

	return	TRUE;
}
