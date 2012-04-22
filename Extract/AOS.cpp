
#include	"stdafx.h"
#include	"../Image.h"
#include	"AOS.h"

//////////////////////////////////////////////////////////////////////////////////////////
//	�}�E���g

BOOL	CAOS::Mount(
	CArcFile*			pclArc							// �A�[�J�C�u
	)
{
	if( pclArc->GetArcExten() != _T(".aos") )
	{
		return	FALSE;
	}

	// �s����4�o�C�g

	pclArc->SeekHed( 4 );

	// �I�t�Z�b�g�̎擾

	DWORD				dwOffset;

	pclArc->Read( &dwOffset, 4 );

	// �C���f�b�N�X�T�C�Y�̎擾

	DWORD				dwIndexSize;

	pclArc->Read( &dwIndexSize, 4 );

	// �A�[�J�C�u�t�@�C�����̎擾

	char				szArchiveName[261];

	pclArc->Read( szArchiveName, 261 );

	if( pclArc->GetArcName() != szArchiveName )
	{
		// �A�[�J�C�u�t�@�C�������قȂ�

		pclArc->SeekHed();
		return	FALSE;
	}

	// �C���f�b�N�X�̎擾

	YCMemory<BYTE>		clmIndex( dwIndexSize );

	pclArc->Read( &clmIndex[0], dwIndexSize );

	// �t�@�C�����̎擾

	for( DWORD i = 0 ; i < dwIndexSize ; i += 40 )
	{
		SFileInfo			stFileInfo;

		stFileInfo.name.Copy( (char*) &clmIndex[i], 32 );
		stFileInfo.start = *(DWORD*) &clmIndex[i + 32] + dwOffset;
		stFileInfo.sizeCmp = *(DWORD*) &clmIndex[i + 36];
		stFileInfo.sizeOrg = stFileInfo.sizeCmp;
		stFileInfo.end = stFileInfo.start + stFileInfo.sizeCmp;

		pclArc->AddFileInfo( stFileInfo );
	}

	return	TRUE;
}

//////////////////////////////////////////////////////////////////////////////////////////
//	�f�R�[�h

BOOL	CAOS::Decode(
	CArcFile*			pclArc							// �A�[�J�C�u
	)
{
	if( pclArc->GetArcExten() != _T(".aos") )
	{
		return	FALSE;
	}

	BOOL				bReturn = FALSE;
	SFileInfo*			pstFileInfo = pclArc->GetOpenFileInfo();

	if( pstFileInfo->format == _T("ABM") )
	{
		// ABM

		bReturn = DecodeABM( pclArc );
	}
	else if( pstFileInfo->format == _T("MSK") )
	{
		// �}�X�N�摜

		bReturn = DecodeMask( pclArc );
	}
	else if( pstFileInfo->format == _T("SCR") )
	{
		// �X�N���v�g

		bReturn = DecodeScript( pclArc );
	}

	return	bReturn;
}

//////////////////////////////////////////////////////////////////////////////////////////
//	ABM�̃f�R�[�h

BOOL	CAOS::DecodeABM(
	CArcFile*			pclArc							// �A�[�J�C�u
	)
{
	SFileInfo*			pstFileInfo = pclArc->GetOpenFileInfo();

	// �f�[�^�̓ǂݍ���

	DWORD				dwSrcSize = pstFileInfo->sizeCmp;

	YCMemory<BYTE>		clmSrc( dwSrcSize );

	pclArc->Read( &clmSrc[0], dwSrcSize );

	// �r�b�g�}�b�v�w�b�_�̎擾

	BITMAPFILEHEADER*	pstbfhSrc = (BITMAPFILEHEADER*) &clmSrc[0];
	BITMAPINFOHEADER*	pstbihSrc = (BITMAPINFOHEADER*) &clmSrc[14];

	CImage				clImage;
	YCMemory<BYTE>		clmDst;
	YCString			clsLastName;
	DWORD				dwDstSize;
	DWORD				dwFrames;
	DWORD				dwOffsetToData;
	DWORD				dwSrcPtr = 0;
	DWORD				dwDstPtr = 0;

	switch( pstbihSrc->biBitCount )
	{
	case	1:
		// �}���`�t���[��

		dwFrames = *(DWORD*) &clmSrc[58];
		dwOffsetToData = *(DWORD*) &clmSrc[66];

		dwDstSize = (pstbihSrc->biWidth * pstbihSrc->biHeight * 4);

		clmDst.resize( dwDstSize );

		if( dwFrames >= 2 )
		{
			// �����t�@�C��

			clsLastName.Format( _T("_000") );
		}

		// ��

		dwSrcPtr = dwOffsetToData;

		for( DWORD i = 0 ; i < dwDstSize ; i += 4 )
		{
			clmDst[i + 0] = clmSrc[dwSrcPtr++];
			clmDst[i + 1] = clmSrc[dwSrcPtr++];
			clmDst[i + 2] = clmSrc[dwSrcPtr++];
			clmDst[i + 3] = 0xFF;
		}

		// �o��

		clImage.Init( pclArc, pstbihSrc->biWidth, pstbihSrc->biHeight, 32, NULL, 0, clsLastName );
		clImage.WriteReverse( &clmDst[0], dwDstSize );
		clImage.Close();

		// 

		for( DWORD i = 1 ; i < dwFrames ; i++ )
		{
			DWORD				dwOffsetToFrame = *(DWORD*) &clmSrc[70 + (i - 1) * 4];

			clsLastName.Format( _T("_%03d"), i );

			// ��

			ZeroMemory( &clmDst[0], dwDstSize );

			DecompABM( &clmDst[0], dwDstSize, &clmSrc[dwOffsetToFrame], (dwSrcSize - dwOffsetToFrame) );

			// �o��

			clImage.Init( pclArc, pstbihSrc->biWidth, pstbihSrc->biHeight, 32, NULL, 0, clsLastName );
			clImage.WriteReverse( &clmDst[0], dwDstSize, FALSE );
			clImage.Close();
		}

		break;

	case	32:
		// 32bit

		dwDstSize = (pstbihSrc->biWidth * pstbihSrc->biHeight * 4);

		clmDst.resize( dwDstSize );

		// ��

		DecompABM( &clmDst[0], dwDstSize, &clmSrc[54], (dwSrcSize - 54) );

		// �o��

		clImage.Init( pclArc, pstbihSrc->biWidth, pstbihSrc->biHeight, pstbihSrc->biBitCount );
		clImage.WriteReverse( &clmDst[0], dwDstSize );
		clImage.Close();

		break;

	default:
		// ���̑�

		pclArc->OpenFile();
		pclArc->WriteFile( &clmSrc[0], dwSrcSize );
		pclArc->CloseFile();
	}

	return	TRUE;
}

//////////////////////////////////////////////////////////////////////////////////////////
//	�}�X�N�摜�̃f�R�[�h

BOOL	CAOS::DecodeMask(
	CArcFile*			pclArc							// �A�[�J�C�u
	)
{
	SFileInfo*			pstFileInfo = pclArc->GetOpenFileInfo();

	// �f�[�^�̓ǂݍ���

	DWORD				dwSrcSize = pstFileInfo->sizeCmp;

	YCMemory<BYTE>		clmSrc( dwSrcSize );

	pclArc->Read( &clmSrc[0], dwSrcSize );

	// �o��

	CImage				clImage;

	clImage.Init( pclArc, &clmSrc[0] );
	clImage.Write( dwSrcSize );
	clImage.Close();

	return	TRUE;
}

//////////////////////////////////////////////////////////////////////////////////////////
//	�X�N���v�g�̃f�R�[�h

BOOL	CAOS::DecodeScript(
	CArcFile*			pclArc							// �A�[�J�C�u
	)
{
	SFileInfo*			pstFileInfo = pclArc->GetOpenFileInfo();

	// ���k�f�[�^�̓ǂݍ���

	DWORD				dwSrcSize = pstFileInfo->sizeCmp;

	YCMemory<BYTE>		clmSrc( dwSrcSize );

	pclArc->Read( &clmSrc[0], dwSrcSize );

	// �𓀗p�o�b�t�@�̊m��

	DWORD				dwDstSize = *(DWORD*) &clmSrc[0];

	YCMemory<BYTE>		clmDst( dwDstSize );

	// ��

	DecompScript( &clmDst[0], dwDstSize, &clmSrc[4], (dwSrcSize - 4) );

	// �o��

	pclArc->OpenScriptFile();
	pclArc->WriteFile( &clmDst[0], dwDstSize, dwSrcSize );
	pclArc->CloseFile();

	return	TRUE;
}

//////////////////////////////////////////////////////////////////////////////////////////
//	ABM�̉�

BOOL	CAOS::DecompABM(
	BYTE*				pbtDst,							// �i�[��
	DWORD				dwDstSize,						// �i�[��T�C�Y
	const BYTE*			pbtSrc,							// ���k�f�[�^
	DWORD				dwSrcSize						// ���k�f�[�^�T�C�Y
	)
{
	DWORD				dwSrcPtr = 0;
	DWORD				dwDstPtr = 0;

	BYTE				btCurrentSrc;
	DWORD				dwAlphaCount = 0;

	while( dwDstPtr < dwDstSize )
	{
		DWORD				dwLength;

		btCurrentSrc = pbtSrc[dwSrcPtr++];

		switch( btCurrentSrc )
		{
		case	0:
			// 0x00�ł���

			dwLength = pbtSrc[dwSrcPtr++];

			for( DWORD i = 0 ; i < dwLength ; i++ )
			{
				pbtDst[dwDstPtr++] = 0;

				dwAlphaCount++;

				if( dwAlphaCount == 3 )
				{
					pbtDst[dwDstPtr++] = 0;

					dwAlphaCount = 0;
				}
			}

			break;

		case	255:
			// 0xFF�ł���

			dwLength = pbtSrc[dwSrcPtr++];

			for( DWORD i = 0 ; i < dwLength ; i++ )
			{
				pbtDst[dwDstPtr++] = pbtSrc[dwSrcPtr++];

				dwAlphaCount++;

				if( dwAlphaCount == 3 )
				{
					pbtDst[dwDstPtr++] = 0xFF;

					dwAlphaCount = 0;
				}
			}

			break;

		default:
			// ���̑�

			pbtDst[dwDstPtr++] = pbtSrc[dwSrcPtr++];

			dwAlphaCount++;

			if( dwAlphaCount == 3 )
			{
				pbtDst[dwDstPtr++] = btCurrentSrc;

				dwAlphaCount = 0;
			}

		}
	}

	return	TRUE;
}

//////////////////////////////////////////////////////////////////////////////////////////
//	�X�N���v�g�̉�

BOOL	CAOS::DecompScript(
	BYTE*				pbtDst,							// �i�[��
	DWORD				dwDstSize,						// �i�[��T�C�Y
	const BYTE*			pbtSrc,							// ���k�f�[�^
	DWORD				dwSrcSize						// ���k�f�[�^�T�C�Y
	)
{
	// �e�[�u���̍\�z

	DWORD				adwTableOfBit0[511];
	DWORD				adwTableOfBit1[511];
	DWORD				dwSrcPtr = 0;
	DWORD				dwTablePtr = 256;
	DWORD				dwCurrentSrc = 0;
	DWORD				dwBitShift = 0;

	ZeroMemory( adwTableOfBit0, sizeof(adwTableOfBit0) );
	ZeroMemory( adwTableOfBit1, sizeof(adwTableOfBit1) );

	dwTablePtr = CreateHuffmanTable( adwTableOfBit0, adwTableOfBit1, pbtSrc, &dwSrcPtr, &dwTablePtr, &dwCurrentSrc, &dwBitShift );

	// ��

	DecompHuffman( pbtDst, dwDstSize, adwTableOfBit0, adwTableOfBit1, &pbtSrc[dwSrcPtr], dwTablePtr, dwCurrentSrc, dwBitShift );

	return	TRUE;
}

//////////////////////////////////////////////////////////////////////////////////////////
//	�e�[�u���̍\�z

DWORD	CAOS::CreateHuffmanTable(
	DWORD*				pdwTableOfBit0,					// bit0�p�e�[�u��
	DWORD*				pdwTableOfBit1,					// bit1�p�e�[�u��
	const BYTE*			pbtSrc,							// ���k�f�[�^
	DWORD*				pdwSrcPtr,						// ���k�f�[�^�Q�ƈʒu
	DWORD*				pdwTablePtr,					// �e�[�u���Q�ƈʒu
	DWORD*				pdwCurrentSrc,					// ���݂̃f�[�^
	DWORD*				pdwBitShift						// �r�b�g�V�t�g
	)
{
	DWORD				dwReturn = 0;
	DWORD				dwTablePtr;

	if( *pdwBitShift == 0 )
	{
		// 8bit�ǂݐ؂���

		*pdwCurrentSrc = pbtSrc[(*pdwSrcPtr)++];
		*pdwBitShift = 8;
	}

	(*pdwBitShift) -= 1;

	if( (*pdwCurrentSrc >> *pdwBitShift) & 1 )
	{
		// �r�b�g��1

		dwTablePtr = *pdwTablePtr;

		(*pdwTablePtr) += 1;

		if( dwTablePtr < 511 )
		{
			pdwTableOfBit0[dwTablePtr] = CreateHuffmanTable( pdwTableOfBit0, pdwTableOfBit1, pbtSrc, pdwSrcPtr, pdwTablePtr, pdwCurrentSrc, pdwBitShift );
			pdwTableOfBit1[dwTablePtr] = CreateHuffmanTable( pdwTableOfBit0, pdwTableOfBit1, pbtSrc, pdwSrcPtr, pdwTablePtr, pdwCurrentSrc, pdwBitShift );

			dwReturn = dwTablePtr;
		}
	}
	else
	{
		// �r�b�g��0

		DWORD				dwBitShiftTemp = 8;
		DWORD				dwResult = 0;

		while( dwBitShiftTemp > *pdwBitShift )
		{
			DWORD				dwWork = ((1 << *pdwBitShift) - 1) & *pdwCurrentSrc;

			dwBitShiftTemp -= *pdwBitShift;

			*pdwCurrentSrc = pbtSrc[(*pdwSrcPtr)++];

			dwResult |= (dwWork << dwBitShiftTemp);

			*pdwBitShift = 8;
		}

		(*pdwBitShift) -= dwBitShiftTemp;

		DWORD				dwMask = (1 << dwBitShiftTemp) - 1;

		dwReturn = ((*pdwCurrentSrc >> *pdwBitShift) & dwMask) | dwResult;
	}

	return	dwReturn;
}

//////////////////////////////////////////////////////////////////////////////////////////
//	�n�t�}���̉�

BOOL	CAOS::DecompHuffman(
	BYTE*				pbtDst,							// �o�͐�
	DWORD				dwDstSize,						// �o�͐�T�C�Y
	const DWORD*		pdwTableOfBit0,					// bit0�p�e�[�u��
	const DWORD*		pdwTableOfBit1,					// bit1�p�e�[�u��
	const BYTE*			pbtSrc,							// ���k�f�[�^
	DWORD				dwRoot,							// �e�[�u���Q�ƈʒu
	DWORD				dwCurrentSrc,					// ���݂̃f�[�^
	DWORD				dwBitShift						// �r�b�g�V�t�g
	)
{
	if( dwDstSize <= 0 )
	{
		return	FALSE;
	}

	DWORD				dwSrcPtr = 0;
	DWORD				dwDstPtr = 0;

	while( dwDstPtr < dwDstSize )
	{
		DWORD				dwTablePtr = dwRoot;

		while( dwTablePtr >= 256 )
		{
			if( dwBitShift == 0 )
			{
				// 8bit�ǂݐ؂���

				dwCurrentSrc = pbtSrc[dwSrcPtr++];
				dwBitShift = 8;
			}

			dwBitShift -= 1;

			if( (dwCurrentSrc >> dwBitShift) & 1 )
			{
				// bit��1

				dwTablePtr = pdwTableOfBit1[dwTablePtr];
			}
			else
			{
				// bit��0

				dwTablePtr = pdwTableOfBit0[dwTablePtr];
			}
		}

		pbtDst[dwDstPtr++] = (BYTE) dwTablePtr;
	}

	return	TRUE;
}
