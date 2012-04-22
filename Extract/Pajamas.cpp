
#include	"stdafx.h"
#include	"../ExtractBase.h"
#include	"../Image.h"
#include	"Pajamas.h"

//////////////////////////////////////////////////////////////////////////////////////////
//	�}�E���g

BOOL	CPajamas::Mount(
	CArcFile*			pclArc							// �}�E���g
	)
{
	if( MountDat1( pclArc ) )
	{
		return	TRUE;
	}

	if( MountDat2( pclArc ) )
	{
		return	TRUE;
	}

	return	FALSE;
}

//////////////////////////////////////////////////////////////////////////////////////////
//	GAMEDAT PACK�̃}�E���g

BOOL	CPajamas::MountDat1(
	CArcFile*			pclArc							// �A�[�J�C�u
	)
{
	if( (pclArc->GetArcExten() != _T(".dat")) && (pclArc->GetArcExten() != _T(".pak")) )
	{
		return	FALSE;
	}

	if( memcmp( pclArc->GetHed(), "GAMEDAT PACK", 12 ) != 0 )
	{
		return	FALSE;
	}

	// �t�@�C�����擾

	DWORD				dwFiles;

	pclArc->SeekHed( 12 );
	pclArc->Read( &dwFiles, 4 );

	// �t�@�C��������C���f�b�N�X�T�C�Y�擾

	DWORD				dwIndexSize = dwFiles * 24;

	// �C���f�b�N�X�擾

	YCMemory<BYTE>		clmbtIndex( dwIndexSize );
	DWORD				dwIndexPtr = 0;

	pclArc->Read( &clmbtIndex[0], dwIndexSize );

	// �t�@�C�����̃C���f�b�N�X�擾

	DWORD				dwFileNameIndexSize = dwFiles << 4;

	BYTE*				pbtFileInfoIndex = &clmbtIndex[dwFileNameIndexSize];

	// �I�t�Z�b�g�擾(�J�n�A�h���X��0����n�܂��Ă��邽�ߕ␳�p�̃I�t�Z�b�g���K�v)

	DWORD				dwOffset = 16 + dwIndexSize;

	for( DWORD i = 0 ; i < dwFiles ; i++ )
	{
		// �t�@�C�����擾

		TCHAR				szFileName[16];

		memcpy( szFileName, &clmbtIndex[dwIndexPtr], 16 );

		// ���X�g�r���[�ɒǉ�

		SFileInfo			stFileInfo;

		stFileInfo.name = szFileName;
		stFileInfo.start = *(DWORD*) &pbtFileInfoIndex[0] + dwOffset;
		stFileInfo.sizeCmp = *(DWORD*) &pbtFileInfoIndex[4];
		stFileInfo.sizeOrg = stFileInfo.sizeCmp;
		stFileInfo.end = stFileInfo.start + stFileInfo.sizeCmp;

		pclArc->AddFileInfo( stFileInfo );

		dwIndexPtr += 16;
		pbtFileInfoIndex += 8;
	}

	return	TRUE;
}

//////////////////////////////////////////////////////////////////////////////////////////
//	GAMEDAT PAC2�̃}�E���g

BOOL	CPajamas::MountDat2(
	CArcFile*			pclArc							// �A�[�J�C�u
	)
{
	if( pclArc->GetArcExten() != _T(".dat") )
	{
		return	FALSE;
	}

	if( memcmp( pclArc->GetHed(), "GAMEDAT PAC2", 12 ) != 0 )
	{
		return	FALSE;
	}

	// �t�@�C�����擾

	DWORD				dwFiles;

	pclArc->SeekHed( 12 );
	pclArc->Read( &dwFiles, 4 );

	// �t�@�C��������C���f�b�N�X�T�C�Y�擾

	DWORD				dwIndexSize = dwFiles * 40;

	// �C���f�b�N�X�擾

	YCMemory<BYTE>		clmbtIndex( dwIndexSize );
	DWORD				dwIndexPtr = 0;

	pclArc->Read( &clmbtIndex[0], dwIndexSize );

	// �t�@�C�����̃C���f�b�N�X�擾

	DWORD				dwFileNameIndexSize = dwFiles << 5;

	BYTE*				pbtFileInfoIndex = &clmbtIndex[dwFileNameIndexSize];

	// �I�t�Z�b�g�擾(�J�n�A�h���X��0����n�܂��Ă��邽�ߕ␳�p�̃I�t�Z�b�g���K�v)

	DWORD				dwOffset = 16 + dwIndexSize;

	for( DWORD i = 0 ; i < dwFiles ; i++ )
	{
		// �t�@�C�����擾

		TCHAR				szFileName[32];

		memcpy( szFileName, &clmbtIndex[dwIndexPtr], 32 );

		// ���X�g�r���[�ɒǉ�

		SFileInfo			stFileInfo;

		stFileInfo.name = szFileName;
		stFileInfo.start = *(DWORD*) &pbtFileInfoIndex[0] + dwOffset;
		stFileInfo.sizeCmp = *(DWORD*) &pbtFileInfoIndex[4];
		stFileInfo.sizeOrg = stFileInfo.sizeCmp;
		stFileInfo.end = stFileInfo.start + stFileInfo.sizeCmp;

		pclArc->AddFileInfo( stFileInfo );

		dwIndexPtr += 32;
		pbtFileInfoIndex += 8;
	}

	return	TRUE;
}

//////////////////////////////////////////////////////////////////////////////////////////
//	�f�R�[�h

BOOL	CPajamas::Decode(
	CArcFile*			pclArc							// �A�[�J�C�u
	)
{
	if( DecodeEPA( pclArc ) )
	{
		return	TRUE;
	}

	return	FALSE;
}

//////////////////////////////////////////////////////////////////////////////////////////
//	EPA�̃f�R�[�h

BOOL	CPajamas::DecodeEPA(
	CArcFile*			pclArc							// �A�[�J�C�u
	)
{
	SFileInfo*			pstFileInfo = pclArc->GetOpenFileInfo();

	if( pstFileInfo->format != _T("EPA") )
	{
		return	FALSE;
	}

	// �w�b�_�ǂݍ���

	BYTE				abtHeader[16];

	pclArc->Read( abtHeader, sizeof(abtHeader) );

	// �����t���O�A�F���A���A�����擾

	BYTE				btDiffFlag = abtHeader[3];
	WORD				wBpp = abtHeader[4];
	long				lWidth = *(long*) &abtHeader[8];
	long				lHeight = *(long*) &abtHeader[12];

	switch( wBpp )
	{
		case	0:
			wBpp = 8;
			break;

		case	1:
			wBpp = 24;
			break;

		case	2:
			wBpp = 32;
			break;

		case	4:
			wBpp = 8;
			break;
	}

	switch( btDiffFlag )
	{
	case	1:
		// �ʏ�

		break;

	case	2:
		// ����

		pclArc->SeekCur( 40 );
		break;

	default:
		// �s��

		pclArc->SeekHed( pstFileInfo->start );
		return	FALSE;
	}

	// �p���b�g�̓ǂݍ���

	BYTE				abtPallet[768];

	if( wBpp == 8 )
	{
		pclArc->Read( abtPallet, sizeof(abtPallet) );
	}

	// EPA�f�[�^�ǂݍ���

	DWORD				dwSrcSize = pstFileInfo->sizeCmp - 16;

	if( wBpp == 8 )
	{
		dwSrcSize -= sizeof(abtPallet);
	}

	YCMemory<BYTE>		clmbtSrc( dwSrcSize );

	pclArc->Read( &clmbtSrc[0], dwSrcSize );

	// BMP�f�[�^���i�[����̈�m��

	DWORD				dwDstSize = lWidth * lHeight * (wBpp >> 3);

	YCMemory<BYTE>		clmbtDst( dwDstSize );

	ZeroMemory( &clmbtDst[0], dwDstSize );

	// EPA����

	DecompEPA( &clmbtDst[0], dwDstSize, &clmbtSrc[0], dwSrcSize, lWidth );

	// �o��

	CImage				clImage;

	clImage.Init( pclArc, lWidth, lHeight, wBpp, abtPallet, sizeof(abtPallet) );
	clImage.WriteCompoBGRAReverse( &clmbtDst[0], dwDstSize );

	return	TRUE;
}

//////////////////////////////////////////////////////////////////////////////////////////
//	EPA�̉�

BOOL	CPajamas::DecompEPA(
	void*				pvDst,							// �i�[��
	DWORD				dwDstSize,						// �i�[��T�C�Y
	const void*			pvSrc,							// ���k�f�[�^
	DWORD				dwSrcSize,						// ���k�f�[�^�T�C�Y
	long				lWidth							// ����
	)
{
	const BYTE*			pbtSrc = (const BYTE*) pvSrc;
	BYTE*				pbtDst = (BYTE*) pvDst;

	DWORD				dwSrcPtr = 0;
	DWORD				dwDstPtr = 0;

	// �I�t�Z�b�g�e�[�u��

	const DWORD			adwOffsets[16] =
	{
		0, 1, lWidth, lWidth + 1,
		2, lWidth - 1, lWidth << 1, 3,
		(lWidth << 1) + 2, lWidth + 2, (lWidth << 1) + 1, (lWidth << 1) - 1,
		(lWidth << 1) - 2, lWidth - 2, lWidth * 3, 4
	};

	// ��

	while( (dwSrcPtr < dwSrcSize) && (dwDstPtr < dwDstSize) )
	{
		BYTE				btCode = pbtSrc[dwSrcPtr++];
		DWORD				dwLength = btCode & 0x07;

		if( btCode & 0xF0 )
		{
			if( btCode & 0x08 )
			{
				dwLength = (dwLength << 8) + pbtSrc[dwSrcPtr++];
			}

			if( dwLength != 0 )
			{
				btCode >>= 4;

				DWORD				dwBack = dwDstPtr - adwOffsets[btCode];

				if( (dwDstPtr + dwLength) > dwDstSize )
				{
					// �o�̓o�b�t�@�𒴂��Ă��܂�

					dwLength = (dwDstSize - dwDstPtr);
				}

				for( DWORD i = 0 ; i < dwLength ; i++ )
				{
					pbtDst[dwDstPtr + i] = pbtDst[dwBack + i];
				}

				dwDstPtr += dwLength;
			}
		}
		else if( btCode != 0 )
		{
			memcpy( &pbtDst[dwDstPtr], &pbtSrc[dwSrcPtr], btCode );

			dwSrcPtr += btCode;
			dwDstPtr += btCode;
		}
	}

	return	TRUE;
}
