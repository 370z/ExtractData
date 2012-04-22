
#include	"stdafx.h"
#include	"../ExtractBase.h"
#include	"../Image.h"
#include	"../Tga.h"
#include	"../Arc/LZSS.h"
#include	"CircusPak.h"

//////////////////////////////////////////////////////////////////////////////////////////
//	�}�E���g

BOOL	CCircusPak::Mount(
	CArcFile*			pclArc							// �A�[�J�C�u
	)
{
	if( MountPakForKujiraCons( pclArc) )
	{
		return	TRUE;
	}

	if( MountPakForKujira( pclArc ) )
	{
		return	TRUE;
	}

	if( MountPakForACDC( pclArc ) )
	{
		return	TRUE;
	}

	if( MountPakForDCGS( pclArc ) )
	{
		return	TRUE;
	}

	return	FALSE;
}

//////////////////////////////////////////////////////////////////////////////////////////
//	�ŏI����������-Conservative-����pak�̃}�E���g

BOOL	CCircusPak::MountPakForKujiraCons(
	CArcFile*			pclArc							// �A�[�J�C�u
	)
{
	if( pclArc->GetArcExten() != _T(".pak") )
	{
		return	FALSE;
	}

	if( memcmp( pclArc->GetHed(), "File Pack 1.0y", 14 ) != 0 )
	{
		return	FALSE;
	}

	// �t�@�C�����̎擾

	DWORD				dwFiles;

	pclArc->SeekHed( 16 );
	pclArc->Read( &dwFiles, 4 );

	// ����

	dwFiles ^= 0xAAAAAAAA;

	// �C���f�b�N�X�擾

	YCMemory<SPakFileInfoType1>	clmIndex( dwFiles );

	pclArc->SeekCur( 4 );
	pclArc->Read( &clmIndex[0], (sizeof(SPakFileInfoType1) * dwFiles) );

	// �t�@�C�����̎擾

	for( DWORD i = 0 ; i < dwFiles ; i++ )
	{
		// �t�@�C�����擾

		char				szFileName[25];

		for( DWORD j = 0 ; j < 24 ; j++ )
		{
			szFileName[j] = clmIndex[i].szFileName[j] ^ 0xAA;
		}

		szFileName[24] = '\0';

		// ���X�g�r���[�ɒǉ�

		SFileInfo			stFileInfo;

		stFileInfo.name = szFileName;
		stFileInfo.start = clmIndex[i].dwOffset ^ 0xAAAAAAAA;
		stFileInfo.sizeCmp = clmIndex[i].dwCompFileSize ^ 0xAAAAAAAA;
		stFileInfo.sizeOrg = stFileInfo.sizeCmp;
		stFileInfo.end = stFileInfo.start + stFileInfo.sizeCmp;

		pclArc->AddFileInfo( stFileInfo );
	}

	return	TRUE;
}

//////////////////////////////////////////////////////////////////////////////////////////
//	�ŏI�������������pak�̃}�E���g

BOOL	CCircusPak::MountPakForKujira(
	CArcFile*			pclArc							// �A�[�J�C�u
	)
{
	if( pclArc->GetArcExten() != _T(".pak") )
	{
		return	FALSE;
	}

	if( memcmp( pclArc->GetHed(), "File Pack 1.00", 14 ) != 0 )
	{
		return	FALSE;
	}

	// �t�@�C�����̎擾

	DWORD				dwFiles;

	pclArc->SeekHed( 16 );
	pclArc->Read( &dwFiles, 4 );

	// ����

	dwFiles ^= 0xAAAAAAAA;

	// �C���f�b�N�X�擾

	YCMemory<SPakFileInfoType2>	clmIndex( dwFiles );

	pclArc->SeekCur( 4 );
	pclArc->Read( &clmIndex[0], (sizeof(SPakFileInfoType2) * dwFiles) );

	// �t�@�C�����̎擾

	for( DWORD i = 0 ; i < dwFiles ; i++ )
	{
		// �t�@�C�����擾

		char				szFileName[33];

		for( DWORD j = 0 ; j < 32 ; j++ )
		{
			szFileName[j] = clmIndex[i].szFileName[j] ^ 0xAA;
		}

		szFileName[32] = '\0';

		// ���X�g�r���[�ɒǉ�

		SFileInfo			stFileInfo;

		stFileInfo.name = szFileName;
		stFileInfo.start = clmIndex[i].dwOffset ^ 0xAAAAAAAA;
		stFileInfo.sizeCmp = clmIndex[i].dwCompFileSize ^ 0xAAAAAAAA;
		stFileInfo.sizeOrg = stFileInfo.sizeCmp;
		stFileInfo.end = stFileInfo.start + stFileInfo.sizeCmp;

		pclArc->AddFileInfo( stFileInfo );
	}

	return	TRUE;
}

//////////////////////////////////////////////////////////////////////////////////////////
//	ACDC����pak�̃}�E���g

BOOL	CCircusPak::MountPakForACDC(
	CArcFile*			pclArc							// �A�[�J�C�u
	)
{
	if( pclArc->GetArcExten() != _T(".pak") )
	{
		return	FALSE;
	}

	if( memcmp( pclArc->GetHed(), "ACDC_RA", 7 ) != 0 )
	{
		return	FALSE;
	}

	// �t�@�C�����̎擾

	DWORD				dwFiles;

	pclArc->SeekHed( 8 );
	pclArc->Read( &dwFiles, 4 );

	// ����

	dwFiles ^= 0xA6A6A6A6;

	// �C���f�b�N�X�擾

	YCMemory<SPakFileInfoType2>	clmIndex( dwFiles );

	pclArc->SeekCur( 4 );
	pclArc->Read( &clmIndex[0], (sizeof(SPakFileInfoType2) * dwFiles) );

	// �t�@�C�����̎擾

	for( DWORD i = 0 ; i < dwFiles ; i++ )
	{
		// �t�@�C�����擾

		char				szFileName[33];

		for( DWORD j = 0 ; j < 32 ; j++ )
		{
			szFileName[j] = clmIndex[i].szFileName[j] ^ 0xA6;
		}

		szFileName[32] = '\0';

		// ���X�g�r���[�ɒǉ�

		SFileInfo			stFileInfo;

		stFileInfo.name = szFileName;
		stFileInfo.start = clmIndex[i].dwOffset ^ 0xA6A6A6A6;
		stFileInfo.sizeCmp = clmIndex[i].dwCompFileSize ^ 0xA6A6A6A6;
		stFileInfo.sizeOrg = stFileInfo.sizeCmp;
		stFileInfo.end = stFileInfo.start + stFileInfo.sizeCmp;

		pclArc->AddFileInfo( stFileInfo );
	}

	return	TRUE;
}

//////////////////////////////////////////////////////////////////////////////////////////
//	DCGS����pak�̃}�E���g

BOOL	CCircusPak::MountPakForDCGS(
	CArcFile*			pclArc							// �A�[�J�C�u
	)
{
	if( pclArc->GetArcExten() != _T(".pak") )
	{
		return	FALSE;
	}

	if( memcmp( pclArc->GetHed(), "PACK", 4 ) != 0 )
	{
		return	FALSE;
	}

	pclArc->SeekHed( 4 );

	// �t�@�C�����̎擾

	DWORD				dwFiles;

	pclArc->Read( &dwFiles, 4 );

	// �t���O�̎擾

	DWORD				dwFlags;

	pclArc->Read( &dwFlags, 4 );

	// �C���f�b�N�X�T�C�Y�̎擾

	DWORD				dwIndexSize = (sizeof(SPakFileInfoType3) * dwFiles);

	// �C���f�b�N�X�̎擾

	YCMemory<SPakFileInfoType3>	clmIndex( dwFiles );

	pclArc->Read( &clmIndex[0], dwIndexSize );

	// �C���f�b�N�X�̕���

	BYTE*				pbtIndex = (BYTE*) &clmIndex[0];

	for( DWORD i = 0 ; i < dwIndexSize ; i++ )
	{
		pbtIndex[i] = (pbtIndex[i] << 4) | (pbtIndex[i] >> 4);
	}

	// �t�@�C�����̎擾

	for( DWORD i = 0 ; i < dwFiles ; i++ )
	{
		// ���X�g�r���[�ɒǉ�

		SFileInfo			stFileInfo;

		stFileInfo.name.Copy( clmIndex[i].szFileName, sizeof(clmIndex[i].szFileName) );
		stFileInfo.start = clmIndex[i].dwOffset;
		stFileInfo.sizeCmp = clmIndex[i].dwCompFileSize;
		stFileInfo.sizeOrg = clmIndex[i].dwFileSize;

		if( stFileInfo.sizeCmp == 0 )
		{
			stFileInfo.sizeCmp = stFileInfo.sizeOrg;
		}

		stFileInfo.end = stFileInfo.start + stFileInfo.sizeCmp;

		pclArc->AddFileInfo( stFileInfo );
	}

	return	TRUE;
}

//////////////////////////////////////////////////////////////////////////////////////////
//	�f�R�[�h

BOOL	CCircusPak::Decode(
	CArcFile*			pclArc							// �A�[�J�C�u
	)
{
	if( DecodePakForKujiraCons( pclArc ) )
	{
		return	TRUE;
	}

	if( DecodePakForKujira( pclArc ) )
	{
		return	TRUE;
	}

	if( DecodePakForACDC( pclArc ) )
	{
		return	TRUE;
	}

	if( DecodePakForDCGS( pclArc ) )
	{
		return	TRUE;
	}

	return	FALSE;
}

//////////////////////////////////////////////////////////////////////////////////////////
//	�ŏI����������-Conservative-����pak�̃f�R�[�h

BOOL	CCircusPak::DecodePakForKujiraCons(
	CArcFile*			pclArc							// �A�[�J�C�u
	)
{
	SFileInfo*			pstFileInfo = pclArc->GetOpenFileInfo();

	if( pclArc->GetArcExten() != _T(".pak") )
	{
		return	FALSE;
	}

	if( memcmp( pclArc->GetHed(), "File Pack 1.0y", 14 ) != 0 )
	{
		return	FALSE;
	}

	BYTE				abtKey[2];

	abtKey[0] = 0xFF;
	abtKey[1] = (pstFileInfo->format == _T("CS")) ? 1 : 0;

	if( pstFileInfo->format == _T("BMP") )
	{
		// BMP

		DWORD				dwSrcSize = pstFileInfo->sizeCmp;

		YCMemory<BYTE>		clmbtSrc( dwSrcSize );

		pclArc->Read( &clmbtSrc[0], dwSrcSize );

		// ����

		Decrypt1( &clmbtSrc[0], dwSrcSize, abtKey );

		// �o��

		DecodeBMP( pclArc, &clmbtSrc[0], dwSrcSize );
	}
	else if( pstFileInfo->format == _T("CPS") )
	{
		// CPS

		DWORD				dwSrcSize = pstFileInfo->sizeCmp;

		YCMemory<BYTE>		clmbtSrc( dwSrcSize );

		pclArc->Read( &clmbtSrc[0], dwSrcSize );

		// ����

		Decrypt1( &clmbtSrc[0], dwSrcSize, abtKey );

		*(DWORD*) &clmbtSrc[4] ^= 0x80701084;

		// �f�R�[�h

		DecodeCps( pclArc, &clmbtSrc[0], dwSrcSize );
	}
	else
	{
		// ���̑�

		DecodeEtc( pclArc, Decrypt2, abtKey );
	}

	return	TRUE;
}

//////////////////////////////////////////////////////////////////////////////////////////
//	�ŏI�������������pak�̃f�R�[�h

BOOL	CCircusPak::DecodePakForKujira(
	CArcFile*			pclArc							// �A�[�J�C�u
	)
{
	SFileInfo*			pstFileInfo = pclArc->GetOpenFileInfo();

	if( pclArc->GetArcExten() != _T(".pak") )
	{
		return	FALSE;
	}

	if( memcmp( pclArc->GetHed(), "File Pack 1.00", 14 ) != 0 )
	{
		return	FALSE;
	}

	BYTE				abtKey[2];

	abtKey[0] = 0xFF;
	abtKey[1] = (pstFileInfo->format == _T("CS")) ? 1 : 0;

	if( pstFileInfo->format == _T("BMP") )
	{
		// �ǂݍ���

		DWORD				dwSrcSize = pstFileInfo->sizeCmp;

		YCMemory<BYTE>		clmbtSrc( dwSrcSize );

		pclArc->Read( &clmbtSrc[0], dwSrcSize );

		// ����

		Decrypt1( &clmbtSrc[0], dwSrcSize, abtKey );

		// �o��

		DecodeBMP( pclArc, &clmbtSrc[0], dwSrcSize );
	}
	else if( pstFileInfo->format == _T("CPS") )
	{
		// CPS

		DWORD				dwSrcSize = pstFileInfo->sizeCmp;

		YCMemory<BYTE>		clmbtSrc( dwSrcSize );

		pclArc->Read( &clmbtSrc[0], dwSrcSize );

		// ����

		Decrypt1( &clmbtSrc[0], dwSrcSize, abtKey );

		*(DWORD*) &clmbtSrc[4] ^= 0x80701084;

		// �f�R�[�h

		DecodeCps( pclArc, &clmbtSrc[0], dwSrcSize );
	}
	else
	{
		// ���̑�

		DecodeEtc( pclArc, Decrypt2, abtKey );
	}

	return	TRUE;
}

//////////////////////////////////////////////////////////////////////////////////////////
//	ACDC����pak�̃f�R�[�h

BOOL	CCircusPak::DecodePakForACDC(
	CArcFile*			pclArc							// �A�[�J�C�u
	)
{
	SFileInfo*			pstFileInfo = pclArc->GetOpenFileInfo();

	if( pclArc->GetArcExten() != _T(".pak") )
	{
		return	FALSE;
	}

	if( memcmp( pclArc->GetHed(), "ACDC_RA", 7 ) != 0 )
	{
		return	FALSE;
	}

	BYTE				abtKey[2];

	abtKey[0] = 0x68;
	abtKey[1] = (pstFileInfo->format == _T("CS")) ? 1 : 0;

	if( pstFileInfo->format == _T("BMP") )
	{
		// BMP

		DWORD				dwSrcSize = pstFileInfo->sizeCmp;

		YCMemory<BYTE>		clmbtSrc( dwSrcSize );

		pclArc->Read( &clmbtSrc[0], dwSrcSize );

		// ����

		Decrypt1( &clmbtSrc[0], dwSrcSize, abtKey );

		// �o��

		DecodeBMP( pclArc, &clmbtSrc[0], dwSrcSize );
	}
	else if( pstFileInfo->format == _T("TGA") )
	{
		// TGA

		DWORD				dwSrcSize = pstFileInfo->sizeCmp;

		YCMemory<BYTE>		clmbtSrc( dwSrcSize );

		pclArc->Read( &clmbtSrc[0], dwSrcSize );

		// ����

		Decrypt1( &clmbtSrc[0], dwSrcSize, abtKey );

		// �o��

		CTga				clTGA;

		clTGA.Decode( pclArc, &clmbtSrc[0], dwSrcSize );
	}
	else if( pstFileInfo->format == _T("CPS") )
	{
		// CPS

		DWORD				dwSrcSize = pstFileInfo->sizeCmp;

		YCMemory<BYTE>		clmbtSrc( dwSrcSize );

		pclArc->Read( &clmbtSrc[0], dwSrcSize );

		// ����

		Decrypt1( &clmbtSrc[0], dwSrcSize, abtKey );

		*(DWORD*) &clmbtSrc[4] ^= 0x0A415FCF;

		if( dwSrcSize >= 0x308 )
		{
			*(DWORD*) &clmbtSrc[4] ^= clmbtSrc[dwSrcSize - 1];

			DWORD				dwWork;
			DWORD				dwWork2;

			// �f�[�^�̓���ւ�

			dwWork = 8 + (dwSrcSize - 8) - 0xFF;
			dwWork2 = 8 + ((dwSrcSize - 8) >> 9) + 0xFF;

			std::swap( clmbtSrc[dwWork], clmbtSrc[dwWork2] );

			// �f�[�^�̓���ւ�

			dwWork = 8 + (dwSrcSize - 8) - (0xFF << 1);
			dwWork2 = 8 + (((dwSrcSize - 8) >> 9) << 1) + 0xFF;

			std::swap( clmbtSrc[dwWork], clmbtSrc[dwWork2] );
		}

		// �f�R�[�h

		DecodeCps( pclArc, &clmbtSrc[0], dwSrcSize );
	}
	else
	{
		// ���̑�

		DecodeEtc( pclArc, Decrypt2, abtKey );
	}

	return	TRUE;
}

//////////////////////////////////////////////////////////////////////////////////////////
//	DCGS����pak�̃f�R�[�h

BOOL	CCircusPak::DecodePakForDCGS(
	CArcFile*			pclArc							// �A�[�J�C�u
	)
{
	SFileInfo*			pstFileInfo = pclArc->GetOpenFileInfo();

	if( pclArc->GetArcExten() != _T(".pak") )
	{
		return	FALSE;
	}

	if( memcmp( pclArc->GetHed(), "PACK", 4 ) != 0 )
	{
		return	FALSE;
	}

	// �ǂݍ���

	DWORD				dwSrcSize = pstFileInfo->sizeCmp;

	YCMemory<BYTE>		clmSrc( dwSrcSize );

	pclArc->Read( &clmSrc[0], dwSrcSize );

	// ��

	BYTE*				pbtDst = &clmSrc[0];
	DWORD				dwDstSize = pstFileInfo->sizeOrg;
	YCMemory<BYTE>		clmDst;

	if( dwSrcSize != dwDstSize )
	{
		// ���k����Ă���

		clmDst.resize( dwDstSize );

		// ��

		CLZSS				clLZSS;

		clLZSS.Decomp( &clmDst[0], dwDstSize, &clmSrc[0], dwSrcSize, 4096, 4078, 3 );

		pbtDst = &clmDst[0];
	}

	// �o��

	if( pstFileInfo->format == _T("OP2") )
	{
		// �摜

		long				lWidth = *(long*) &pbtDst[4];
		long				lHeight = *(long*) &pbtDst[8];
		WORD				wBpp = *(WORD*) &pbtDst[12];
		DWORD				dwDataOffset = *(DWORD*) &pbtDst[20];
		DWORD				dwPalletSize = (dwDataOffset - 32);
		DWORD				dwDIBSize = *(DWORD*) &pbtDst[24];

		dwDstSize = *(DWORD*) &pbtDst[28];

		YCMemory<BYTE>		clmDIB( dwDIBSize );

		// ��

		CLZSS				clLZSS;

		clLZSS.Decomp( &clmDIB[0], dwDIBSize, &pbtDst[dwDataOffset], dwDstSize, 4096, 4078, 3 );

		// �o��

		CImage				clImage;

		clImage.Init( pclArc, lWidth, lHeight, wBpp, &pbtDst[32], dwPalletSize );
		clImage.WriteReverse( &clmDIB[0], dwDIBSize );
		clImage.Close();
	}
	else
	{
		// ���̑�

		pclArc->OpenFile();
		pclArc->WriteFile( pbtDst, dwDstSize );
		pclArc->CloseFile();
	}

	return	TRUE;
}

//////////////////////////////////////////////////////////////////////////////////////////
//	BMP�̃f�R�[�h

BOOL	CCircusPak::DecodeBMP(
	CArcFile*			pclArc,							// �A�[�J�C�u
	const void*			pvSrc,							// BMP�f�[�^(�������ς݂ł��邱��)
	DWORD				dwSrcSize						// BMP�f�[�^�T�C�Y
	)
{
	SFileInfo*			pstFileInfo = pclArc->GetOpenFileInfo();

	if( pstFileInfo->format != _T("BMP") )
	{
		return	FALSE;
	}

	BYTE*				pbtSrc = (BYTE*) pvSrc;

	BITMAPFILEHEADER*	pstbmfhSrc = (BITMAPFILEHEADER*) &pbtSrc[0];
	BITMAPINFOHEADER*	pstbmihSrc = (BITMAPINFOHEADER*) &pbtSrc[14];

	dwSrcSize = (pstbmihSrc->biBitCount == 8) ? (dwSrcSize - 54 - 1024) : (dwSrcSize - 54);

	CImage				clImage;

	clImage.Init( pclArc, pbtSrc );
	clImage.Write( dwSrcSize );

	return	TRUE;
}

//////////////////////////////////////////////////////////////////////////////////////////
//	CPS�̃f�R�[�h

BOOL	CCircusPak::DecodeCps(
	CArcFile*			pclArc,							// �A�[�J�C�u
	const void*			pvSrc,							// CPS�f�[�^(�������ς݂ł��邱��)
	DWORD				dwSrcSize						// CPS�f�[�^�T�C�Y
	)
{
	SFileInfo*			pstFileInfo = pclArc->GetOpenFileInfo();

	if( pstFileInfo->format != _T("CPS") )
	{
		return	FALSE;
	}

	const BYTE*			pbtSrc = (const BYTE*) pvSrc;

	// �o�̓o�b�t�@�m��

	DWORD				dwDstSize = *(DWORD*) &pbtSrc[4];

	YCMemory<BYTE>		clmbtDst( dwDstSize );

	// ��

	if( memcmp( pbtSrc, "CCC0", 4 ) == 0 )
	{
		// CCC0

		DecompCCC0( &clmbtDst[0], dwDstSize, pbtSrc, dwSrcSize );

		long				lWidth = *(WORD*) &clmbtDst[12];
		long				lHeight = *(WORD*) &clmbtDst[14];
		WORD				wBpp = clmbtDst[16];

		// �o��

		CImage				clImage;

		clImage.Init( pclArc, lWidth, lHeight, wBpp );
		clImage.Write( &clmbtDst[18], (dwDstSize - 18) );
	}
	else if( memcmp( pbtSrc, "CCM0", 4 ) == 0 )
	{
		// CCM0

		DecompCCM0( &clmbtDst[0], dwDstSize, pbtSrc, dwSrcSize );

		// �o��

		BITMAPFILEHEADER*	pstBMPFileHeader = (BITMAPFILEHEADER*) &clmbtDst[0];
		BITMAPINFOHEADER*	pstBMPInfoHeader = (BITMAPINFOHEADER*) &clmbtDst[14];

		dwDstSize = (pstBMPInfoHeader->biBitCount == 8) ? (dwDstSize - 54 - 1024) : (dwDstSize - 54);

		CImage				clImage;

		clImage.Init( pclArc, &clmbtDst[0] );
		clImage.Write( dwDstSize );
	}
	else
	{
		// �s��

		pclArc->OpenFile();
		pclArc->WriteFile( pbtSrc, dwSrcSize );
	}

	return	TRUE;
}

//////////////////////////////////////////////////////////////////////////////////////////
//	���̑��̃f�[�^�̃f�R�[�h

BOOL	CCircusPak::DecodeEtc(
	CArcFile*			pclArc,							// �A�[�J�C�u
	FDecrypt			pfnDecryptFunc,					// �����֐��ւ̃|�C���^
	const void*			pvKey							// �����L�[
	)
{
	SFileInfo*			pstFileInfo = pclArc->GetOpenFileInfo();
	DWORD				dwBufSize = pclArc->GetBufSize();

	YCMemory<BYTE>		clmbtBuf( dwBufSize );

	// �o�̓t�@�C������

	if( pstFileInfo->format == _T("CS") )
	{
		pclArc->OpenScriptFile();
	}
	else
	{
		pclArc->OpenFile();
	}

	for( DWORD dwWriteSize = 0 ; dwWriteSize < pstFileInfo->sizeOrg ; dwWriteSize += dwBufSize )
	{
		// �o�b�t�@�T�C�Y����

		pclArc->SetBufSize( &dwBufSize, dwWriteSize );

		// ���������f�[�^���o��

		pclArc->Read( &clmbtBuf[0], dwBufSize );

		pfnDecryptFunc( &clmbtBuf[0], dwBufSize, pvKey );

		pclArc->WriteFile( &clmbtBuf[0], dwBufSize );
	}

	return	TRUE;
}

//////////////////////////////////////////////////////////////////////////////////////////
//	CCC0�̉�

BOOL	CCircusPak::DecompCCC0(
	void*				pvDst,							// �i�[��
	DWORD				dwDstSize,						// �i�[��̃T�C�Y
	const void*			pvSrc,							// CCC0�f�[�^
	DWORD				dwSrcSize						// CCC0�f�[�^�T�C�Y
	)
{
	const BYTE*			pbtSrc = (const BYTE*) pvSrc;
	BYTE*				pbtDst = (BYTE*) pvDst;

	DWORD				dwSrcPtrOfBit = 0x2C * 8;
	DWORD				dwDstPtr = 0;

	// ��

	for( dwDstPtr = 0 ; dwDstPtr < dwDstSize ; dwDstPtr += 2 )
	{
		DWORD			dwIndex;

		for( dwIndex = 0 ; dwIndex < 0x0F ; dwIndex++ )
		{
			if( GetBit( pbtSrc, &dwSrcPtrOfBit, 1 ) == 1 )
			{
				break;
			}
		}

		if( dwIndex == 0x0F )
		{
			if( GetBit( pbtSrc, &dwSrcPtrOfBit, 1 ) == 0 )
			{
				dwIndex++;
			}
		}

		if( dwIndex != 0 )
		{
			*(WORD*) &pbtDst[dwDstPtr] = ((WORD*) (pbtSrc + 8))[dwIndex - 1];
		}
		else
		{
			*(WORD*) &pbtDst[dwDstPtr] = GetBit( pbtSrc, &dwSrcPtrOfBit, 16 );
		}
	}

	// �T�C�Y����̏ꍇ1�]��

	if( dwDstSize & 1 )
	{
		pbtDst[dwDstPtr] = pbtSrc[0x28];
	}

	return	TRUE;
}

//////////////////////////////////////////////////////////////////////////////////////////
//	CCM0�̉�

BOOL	CCircusPak::DecompCCM0(
	void*				pvDst,							// �i�[��
	DWORD				dwDstSize,						// �i�[��̃T�C�Y
	const void*			pvSrc,							// CCM0�f�[�^
	DWORD				dwSrcSize						// CCM0�f�[�^�T�C�Y
	)
{
	const BYTE*			pbtSrc = (const BYTE*) pvSrc;
	BYTE*				pbtDst = (BYTE*) pvDst;

	DWORD				dwSrcPtrOfBit;
	DWORD				dwDstPtr = 0;

	// ��

	if( dwDstSize < 0x80 )
	{
		// �����k

		memcpy( pbtDst, &pbtSrc[8], dwDstSize );
	}
	else
	{
		for( DWORD i = 0 ; i < 0x80 ; i++ )
		{
			pbtDst[dwDstPtr++] = pbtSrc[8 + i] + 0xF0;
		}

		dwSrcPtrOfBit = 0x88 * 8;

		while( (dwDstPtr < dwDstSize) )
		{
			DWORD				dwFlag = GetBit( pbtSrc, &dwSrcPtrOfBit, 1 );

			if( dwFlag & 1 )
			{
				// ���k

				DWORD				dwBack = GetBit( pbtSrc, &dwSrcPtrOfBit, 7 ) + 1;
				DWORD				dwLength = GetBit( pbtSrc, &dwSrcPtrOfBit, 4 ) + 2;

				for( DWORD i = 0 ; i < dwLength ; i++ )
				{
					pbtDst[dwDstPtr + i] = pbtDst[dwDstPtr - dwBack + i];
				}

				dwDstPtr += dwLength;
			}
			else
			{
				// �����k

				pbtDst[dwDstPtr++] = GetBit( pbtSrc, &dwSrcPtrOfBit, 8 );
			}
		}
	}

	return	TRUE;
}

//////////////////////////////////////////////////////////////////////////////////////////
//	�r�b�g��̎擾

DWORD	CCircusPak::GetBit(
	const void*			pvSrc,							// ���̓f�[�^
	DWORD*				pdwSrcPtrOfBit,					// ���̓f�[�^�̈ʒu(�ǂݍ��񂾃r�b�g�������Z�����)
	DWORD				dwReadBitLength					// �ǂݍ��ރr�b�g��
	)
{
	const BYTE*			pbtSrc = (const BYTE*) pvSrc;
	DWORD				dwData = 0;

	for( DWORD i = 0 ; i < dwReadBitLength ; i++ )
	{
		dwData <<= 1;
		dwData |= (pbtSrc[*pdwSrcPtrOfBit >> 3] >> (*pdwSrcPtrOfBit & 7)) & 1;
		(*pdwSrcPtrOfBit)++;
	}

	return	dwData;
}

//////////////////////////////////////////////////////////////////////////////////////////
//	����1

BOOL	CCircusPak::Decrypt1(
	void*				pvTarget,						// �����Ώۃf�[�^
	DWORD				dwTargetSize,					// �����T�C�Y
	const void*			pvKey							// �����l
	)
{
	BYTE*				pbtTarget = (BYTE*) pvTarget;
	const BYTE*			pbtKey = (const BYTE*) pvKey;

	for( DWORD i = 0 ; i < dwTargetSize ; i++ )
	{
		pbtTarget[i] ^= pbtKey[0];
	}

	return	TRUE;
}

//////////////////////////////////////////////////////////////////////////////////////////
//	����2

BOOL	CCircusPak::Decrypt2(
	void*				pvTarget,						// �����Ώۃf�[�^
	DWORD				dwTargetSize,					// �����T�C�Y
	const void*			pvKey							// �����l
	)
{
	BYTE*				pbtTarget = (BYTE*) pvTarget;
	const BYTE*			pbtKey = (const BYTE*) pvKey;

	for( DWORD i = 0 ; i < dwTargetSize ; i++ )
	{
		pbtTarget[i] ^= pbtKey[0];
		pbtTarget[i] -= pbtKey[1];
	}

	return	TRUE;
}
