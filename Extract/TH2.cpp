
#include	"stdafx.h"
#include	"../ExtractBase.h"
#include	"../Arc/LZSS.h"
#include	"../Image.h"
#include	"../Tga.h"
#include	"TH2.h"

//////////////////////////////////////////////////////////////////////////////////////////
//	�}�E���g

BOOL	CTH2::Mount(
	CArcFile*			pclArc							// �A�[�J�C�u
	)
{
	if( MountKCAP( pclArc ) )
	{
		return	TRUE;
	}

	if( MountLAC( pclArc ) )
	{
		return	TRUE;
	}

	if( MountDpl( pclArc ) )
	{
		return	TRUE;
	}

	if( MountWMV( pclArc ) )
	{
		return	TRUE;
	}

	return	FALSE;
}

//////////////////////////////////////////////////////////////////////////////////////////
//	KCAP�̃}�E���g

BOOL	CTH2::MountKCAP(
	CArcFile*			pclArc							// �A�[�J�C�u
	)
{
	if( pclArc->GetArcExten().CompareNoCase( _T(".pak") ) != 0 )
	{
		return	FALSE;
	}

	if( memcmp( pclArc->GetHed(), "KCAP", 4 ) != 0 )
	{
		return	FALSE;
	}

	// �t�@�C�����擾

	DWORD				dwFiles;

	pclArc->SeekHed( 4 );
	pclArc->Read( &dwFiles, 4 );

	// �t�@�C��������C���f�b�N�X�T�C�Y�擾

	DWORD				dwIndexSize = (dwFiles * 36);

	// �C���f�b�N�X�擾

	YCMemory<BYTE>		clmIndex( dwIndexSize );

	pclArc->Read( &clmIndex[0], dwIndexSize );

	DWORD				dwIndexPtr = 0;

	for( DWORD i = 0 ; i < dwFiles ; i++ )
	{
		DWORD				dwType = *(DWORD*) &clmIndex[dwIndexPtr + 0];

		// ���݃t�@�C���X�L�b�v

		if( dwType == 0xCCCCCCCC )
		{
			dwIndexPtr += 36;
			continue;
		}

		// �t�@�C�����擾

		char				szFileName[25];

		memcpy( szFileName, &clmIndex[dwIndexPtr + 4], 24 );

		szFileName[24] = '\0';

		// ���X�g�r���[�ɒǉ�

		SFileInfo			stFileInfo;

		stFileInfo.name = szFileName;
		stFileInfo.start = *(DWORD*) &clmIndex[dwIndexPtr + 28];
		stFileInfo.sizeCmp = *(DWORD*) &clmIndex[dwIndexPtr + 32];
		stFileInfo.sizeOrg = stFileInfo.sizeCmp;
		stFileInfo.end = stFileInfo.start + stFileInfo.sizeCmp;
		stFileInfo.title = _T("TH2");

		if( dwType == 1 )
		{
			stFileInfo.format = _T("LZ");
		}

		pclArc->AddFileInfo( stFileInfo );

		dwIndexPtr += 36;
	}

	return	TRUE;
}

//////////////////////////////////////////////////////////////////////////////////////////
//	LAC�̃}�E���g

BOOL	CTH2::MountLAC(
	CArcFile*			pclArc							// �A�[�J�C�u
	)
{
	if( pclArc->GetArcExten().CompareNoCase( _T(".pak") ) != 0 )
	{
		return	FALSE;
	}

	if( memcmp( pclArc->GetHed(), "LAC", 3 ) != 0 )
	{
		return	FALSE;
	}

	// �t�@�C�����擾

	DWORD				dwFiles;

	pclArc->SeekHed( 4 );
	pclArc->Read( &dwFiles, 4 );

	// �t�@�C��������C���f�b�N�X�T�C�Y�擾

	DWORD				dwIndexSize = (dwFiles * 40);

	// �C���f�b�N�X�擾

	YCMemory<BYTE>		clmIndex( dwIndexSize );

	pclArc->Read( &clmIndex[0], dwIndexSize );

	DWORD				dwIndexPtr = 0;

	for( DWORD i = 0 ; i < dwFiles ; i++ )
	{
		// �t�@�C�����擾

		char				szFileName[33];

		memcpy( szFileName, &clmIndex[dwIndexPtr + 0], 32 );

		szFileName[32] = '\0';

		for( int j = 0 ; j < lstrlen( szFileName ) ; j++ )
		{
			szFileName[j] ^= 0xFF;
		}

		// ���X�g�r���[�ɒǉ�

		SFileInfo			stFileInfo;

		stFileInfo.name = szFileName;
		stFileInfo.sizeCmp = *(DWORD*) &clmIndex[dwIndexPtr + 32];
		stFileInfo.sizeOrg = stFileInfo.sizeCmp;
		stFileInfo.start = *(DWORD*) &clmIndex[dwIndexPtr + 36];
		stFileInfo.end = stFileInfo.start + stFileInfo.sizeCmp;

		pclArc->AddFileInfo( stFileInfo );

		dwIndexPtr += 40;
	}

	return	TRUE;
}

//////////////////////////////////////////////////////////////////////////////////////////
//	

BOOL	CTH2::MountDpl(
	CArcFile*			pclArc							// �A�[�J�C�u
	)
{
	if( pclArc->GetArcExten() != _T(".a") )
	{
		return	FALSE;
	}

	if( memcmp( pclArc->GetHed(), "\x1E\xAF", 2 ) != 0 )
	{
		return	FALSE;
	}

	// �t�@�C�����擾

	WORD				wFiles;

	pclArc->SeekHed( 2 );
	pclArc->Read( &wFiles, 2 );

	// �t�@�C��������C���f�b�N�X�T�C�Y�擾

	DWORD				dwIndexSize = (wFiles << 5);

	// �C���f�b�N�X�擾

	YCMemory<BYTE>		clmIndex( dwIndexSize );

	pclArc->Read( &clmIndex[0], dwIndexSize );

	DWORD				dwOffset = (dwIndexSize + 4);
	DWORD				dwIndexPtr = 0;

	for( WORD i = 0 ; i < wFiles ; i++ )
	{
		// �t�@�C�����擾

		char				szFileName[25];

		memcpy( szFileName, &clmIndex[dwIndexPtr + 0], 24 );

		szFileName[24] = '\0';

		// ���X�g�r���[�ɒǉ�

		SFileInfo			stFileInfo;

		stFileInfo.name = szFileName;
		stFileInfo.sizeCmp = *(DWORD*) &clmIndex[dwIndexPtr + 24];
		stFileInfo.sizeOrg = stFileInfo.sizeCmp;
		stFileInfo.start = *(DWORD*) &clmIndex[dwIndexPtr + 28] + dwOffset;
		stFileInfo.end = stFileInfo.start + stFileInfo.sizeCmp;

		pclArc->AddFileInfo( stFileInfo );

		dwIndexPtr += 32;
	}

	return	TRUE;
}

//////////////////////////////////////////////////////////////////////////////////////////
//	WMV�̃}�E���g

BOOL	CTH2::MountWMV(
	CArcFile*			pclArc							// �A�[�J�C�u
	)
{
	if( pclArc->GetArcExten() != _T(".wmv") )
	{
		return	FALSE;
	}

	if( memcmp( pclArc->GetHed(), "\x00\x00\x00\x00\x00\x00\x00\x00\xA6\xD9\x00\xAA\x00\x62\xCE\x6C", 16 ) != 0 )
	{
		return	FALSE;
	}

	return	pclArc->Mount();
}

//////////////////////////////////////////////////////////////////////////////////////////
//	�f�R�[�h

BOOL	CTH2::Decode(
	CArcFile*			pclArc							// �A�[�J�C�u
	)
{
	if( DecodeWMV( pclArc ) )
	{
		return	TRUE;
	}

	if( DecodeEtc( pclArc ) )
	{
		return	TRUE;
	}

	return	FALSE;
}

//////////////////////////////////////////////////////////////////////////////////////////
//	WMV�̃f�R�[�h

BOOL	CTH2::DecodeWMV(
	CArcFile*			pclArc							// �A�[�J�C�u
	)
{
	SFileInfo*			pstFileInfo = pclArc->GetOpenFileInfo();

	if( pclArc->GetArcExten() != _T(".wmv") )
	{
		return	FALSE;
	}

	if( memcmp( pclArc->GetHed(), "\x00\x00\x00\x00\x00\x00\x00\x00\xA6\xD9\x00\xAA\x00\x62\xCE\x6C", 16 ) != 0 )
	{
		return	FALSE;
	}

	// �o�b�t�@�̎擾

	DWORD				dwBufferSize = pclArc->GetBufSize();

	YCMemory<BYTE>		clmBuffer( dwBufferSize );

	// �o��

	pclArc->OpenFile();

	pclArc->SeekCur( 8 );
	pclArc->WriteFile( "\x30\x26\xB2\x75\x8E\x66\xCF\x11", 8 );

	pclArc->ReadWrite( (pstFileInfo->sizeCmp - 8) );

	pclArc->CloseFile();

	return	TRUE;
}

//////////////////////////////////////////////////////////////////////////////////////////
//	���̑��t�@�C���̃f�R�[�h

BOOL	CTH2::DecodeEtc(
	CArcFile*			pclArc							// �A�[�J�C�u
	)
{
	SFileInfo*			pstFileInfo = pclArc->GetOpenFileInfo();

	if( pstFileInfo->title != _T("TH2") )
	{
		return	FALSE;
	}

	if( (pstFileInfo->format != _T("LZ")) && (pstFileInfo->format != _T("TGA")) )
	{
		return	FALSE;
	}

	YCMemory<BYTE>		clmDst;
	DWORD				dwDstSize;

	// LZ�W�J

	if( pstFileInfo->format == _T("LZ") )
	{
		// ���̓T�C�Y�擾

		DWORD				dwSrcSize = (pstFileInfo->sizeCmp - 8);

		// �o�̓T�C�Y�擾

		pclArc->SeekCur( 4 );
		pclArc->Read( &dwDstSize, 4 );

		// �o�b�t�@�m��

		YCMemory<BYTE>		clmSrc( dwSrcSize );

		clmDst.resize( dwDstSize );

		// �ǂݍ���

		pclArc->Read( &clmSrc[0], dwSrcSize );

		// LZSS��

		CLZSS				clLZSS;

		clLZSS.Decomp( &clmDst[0], dwDstSize, &clmSrc[0], dwSrcSize, 4096, 4078, 3 );
	}
	else
	{
		// �����k

		dwDstSize = pstFileInfo->sizeOrg;

		clmDst.resize( dwDstSize );

		pclArc->Read( &clmDst[0], dwDstSize );
	}

	YCString			clsFileExt = pstFileInfo->name.GetFileExt().MakeLower();

	if( clsFileExt == _T(".tga") )
	{
		// TGA

		CTga				clTGA;

		clTGA.Decode( pclArc, &clmDst[0], dwDstSize );
	}
	else if( clsFileExt == _T(".bmp") )
	{
		// BMP

		CImage				clImage;

		clImage.Init( pclArc, &clmDst[0] );
		clImage.Write( dwDstSize );
	}
	else
	{
		// ���̑�

		pclArc->OpenFile();
		pclArc->WriteFile( &clmDst[0], dwDstSize );
	}

	return	TRUE;
}
