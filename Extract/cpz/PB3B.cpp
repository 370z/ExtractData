
#include	"stdafx.h"
#include	"../../Image.h"
#include	"JBP1.h"
#include	"PB3B.h"

//////////////////////////////////////////////////////////////////////////////////////////
//	�f�R�[�h
//
//	���l	pvSrc�͐擪���������������

BOOL	CPB3B::Decode(
	CArcFile*			pclArc,							// �A�[�J�C�u
	void*				pvSrc,							// ���k�f�[�^
	DWORD				dwSrcSize,						// ���k�f�[�^�T�C�Y
	PB3B_DECRYPT		pfnDecrypt						// �����֐�
	)
{
	BYTE*				pbtSrc = (BYTE*) pvSrc;

	// ����

	Decrypt( pbtSrc, dwSrcSize );

	// �摜���̎擾

	WORD				wType = *(WORD*) &pbtSrc[28];
	long				lWidth = *(WORD*) &pbtSrc[30];
	long				lHeight = *(WORD*) &pbtSrc[32];
	WORD				wBpp = *(WORD*) &pbtSrc[34];

	switch( wType )
	{
		case	1:
			Decode1( pclArc, pbtSrc, dwSrcSize, lWidth, lHeight, wBpp );
			break;

		case	2:
		case	3:
			Decode3( pclArc, pbtSrc, dwSrcSize, lWidth, lHeight, wBpp );
			break;

		case	4:
			Decode4( pclArc, pbtSrc, dwSrcSize, lWidth, lHeight, wBpp );
			break;

		case	5:
			Decode5( pclArc, pbtSrc, dwSrcSize, lWidth, lHeight, wBpp );
			break;

		case	6:
			Decode6( pclArc, pbtSrc, dwSrcSize, lWidth, lHeight, wBpp, pfnDecrypt );
			break;

		default:
			// �s��

			pclArc->OpenFile();
			pclArc->WriteFile( pbtSrc, dwSrcSize );
	}

	return	TRUE;
}

//////////////////////////////////////////////////////////////////////////////////////////
//	����

void	CPB3B::Decrypt(
	BYTE*				pbtTarget,						// �����Ώۃf�[�^
	DWORD				dwSize							// �����T�C�Y
	)
{
	for( int i = 8 ; i < 52 ; i += 2 )
	{
		*(WORD*) &pbtTarget[i] ^= *(WORD*) &pbtTarget[dwSize - 3];
	}

	for( int i = 8, j = 0 ; i < 52 ; i++, j++ )
	{
		pbtTarget[i] -= pbtTarget[dwSize - 47 + j];
	}
}

//////////////////////////////////////////////////////////////////////////////////////////
//	�f�R�[�h1

BOOL	CPB3B::Decode1(
	CArcFile*			pclArc,							// �A�[�J�C�u
	const BYTE*			pbtSrc,							// ���k�f�[�^
	DWORD				dwSrcSize,						// ���k�f�[�^�T�C�Y
	long				lWidth,							// ����
	long				lHeight,						// �c��
	WORD				wBpp							// �r�b�g��
	)
{
	// �o�͗p�o�b�t�@�m��

	DWORD				dwDstSize = (lWidth * lHeight * (wBpp >> 3));

	YCMemory<BYTE>		clmbtDst( dwDstSize );

	// ��

	Decomp1( &clmbtDst[0], dwDstSize, pbtSrc, dwSrcSize, lWidth, lHeight, wBpp );

	// �o��

	CImage				clImage;

	clImage.Init( pclArc, lWidth, lHeight, wBpp );
	clImage.WriteReverse( &clmbtDst[0], dwDstSize );

	return	TRUE;
}

//////////////////////////////////////////////////////////////////////////////////////////
//	�f�R�[�h3

BOOL	CPB3B::Decode3(
	CArcFile*			pclArc,							// �A�[�J�C�u
	const BYTE*			pbtSrc,							// ���k�f�[�^
	DWORD				dwSrcSize,						// ���k�f�[�^�T�C�Y
	long				lWidth,							// ����
	long				lHeight,						// �c��
	WORD				wBpp							// �r�b�g��
	)
{
	// �o�͗p�o�b�t�@�m��

	DWORD				dwDstSize = (lWidth * lHeight * (wBpp >> 3));

	YCMemory<BYTE>		clmbtDst( dwDstSize );

	// ��

	Decomp3( &clmbtDst[0], dwDstSize, pbtSrc, dwSrcSize, lWidth, lHeight, wBpp );

	// �o��

	CImage				clImage;

	clImage.Init( pclArc, lWidth, lHeight, wBpp );
	clImage.WriteReverse( &clmbtDst[0], dwDstSize );

	return	TRUE;
}

//////////////////////////////////////////////////////////////////////////////////////////
//	�f�R�[�h4

BOOL	CPB3B::Decode4(
	CArcFile*			pclArc,							// �A�[�J�C�u
	const BYTE*			pbtSrc,							// ���k�f�[�^
	DWORD				dwSrcSize,						// ���k�f�[�^�T�C�Y
	long				lWidth,							// ����
	long				lHeight,						// �c��
	WORD				wBpp							// �r�b�g��
	)
{
	pclArc->OpenFile();
	pclArc->WriteFile( pbtSrc, dwSrcSize );

	return	TRUE;
}

//////////////////////////////////////////////////////////////////////////////////////////
//	�f�R�[�h5

BOOL	CPB3B::Decode5(
	CArcFile*			pclArc,							// �A�[�J�C�u
	const BYTE*			pbtSrc,							// ���k�f�[�^
	DWORD				dwSrcSize,						// ���k�f�[�^�T�C�Y
	long				lWidth,							// ����
	long				lHeight,						// �c��
	WORD				wBpp							// �r�b�g��
	)
{
	// �x�[�X�摜�p�o�b�t�@�m��

	DWORD				dwBaseSize = (lWidth * lHeight * (wBpp >> 3));

	YCMemory<BYTE>		clmbtBase( dwBaseSize );

	// �x�[�X�摜�̉�

	Decomp5( &clmbtBase[0], dwBaseSize, pbtSrc, dwSrcSize, lWidth, lHeight, wBpp );

	// �x�[�X�摜�̏o��

	CImage				clImage;

	clImage.Init( pclArc, lWidth, lHeight, wBpp );
	clImage.WriteReverse( &clmbtBase[0], dwBaseSize );
	clImage.Close();

	return	TRUE;
}

//////////////////////////////////////////////////////////////////////////////////////////
//	�f�R�[�h6

BOOL	CPB3B::Decode6(
	CArcFile*			pclArc,							// �A�[�J�C�u
	const BYTE*			pbtSrc,							// ���k�f�[�^
	DWORD				dwSrcSize,						// ���k�f�[�^�T�C�Y
	long				lWidth,							// ����
	long				lHeight,						// �c��
	WORD				wBpp,							// �r�b�g��
	PB3B_DECRYPT		pfnDecrypt						// �����֐�
	)
{
	// �o�͗p�o�b�t�@�m��

	DWORD				dwDstSize = (lWidth * lHeight * (wBpp >> 3));

	YCMemory<BYTE>		clmbtDst( dwDstSize );

	// �x�[�X�摜�̃t�@�C�����擾

	BYTE				abtBaseFileName[64];

	memcpy( abtBaseFileName, pbtSrc + 52, 32 );

	// �x�[�X�摜�̃t�@�C��������

	const static DWORD	adwCrypt[] =
	{
		0x9CF375A6, 0xA37869C5, 0x794FA53E, 0xC73AFE59
	};

	for( int i = 0 ; i < 2 ; i++ )
	{
		for( int j = 0 ; j < 4 ; j++ )
		{
			((DWORD*) abtBaseFileName)[i * 4 + j] ^= adwCrypt[j];
		}
	}

	abtBaseFileName[32] = '\0';

	YCString			clsBaseFileName = (char*) abtBaseFileName;

	clsBaseFileName += _T(".pb3");

	// �v���~�e�B�u�����N�̃x�[�X�摜�̃t�@�C�������ꕔ�ςɂȂ��Ă���̂ŏC��

	if( clsBaseFileName == _T("ast-si0a_100.pb3") )
	{
		clsBaseFileName = _T("st-si0a_100a.pb3");
	}

	// �x�[�X�摜�̊i�[

	const SFileInfo*		pstBaseFileInfo = pclArc->GetFileInfo( clsBaseFileName, TRUE );

	if( pstBaseFileInfo != NULL )
	{
		// �x�[�X�摜�����݂���

		DWORD				dwBaseSrcSize = pstBaseFileInfo->sizeCmp;

		YCMemory<BYTE>		clmbtBaseSrc( dwBaseSrcSize );

		// �x�[�X�摜�̓ǂݍ���

		pclArc->SeekHed( pstBaseFileInfo->start );
		pclArc->Read( &clmbtBaseSrc[0], dwBaseSrcSize );

		// �x�[�X�摜�̕���

		pfnDecrypt( &clmbtBaseSrc[0], dwBaseSrcSize, pclArc, *pstBaseFileInfo );
		Decrypt( &clmbtBaseSrc[0], dwBaseSrcSize );

		// �x�[�X�摜�̉�

		Decomp5( &clmbtDst[0], dwDstSize, &clmbtBaseSrc[0], dwBaseSrcSize, lWidth, lHeight, wBpp );
	}
	else
	{
		// �����摜�̂�

		ZeroMemory( &clmbtDst[0], dwDstSize );
	}

	// �����摜�̉�

	Decomp6( &clmbtDst[0], dwDstSize, pbtSrc, dwSrcSize, lWidth, lHeight, wBpp );

	// �o��

	CImage				clImage;

	clImage.Init( pclArc, lWidth, lHeight, wBpp );
	clImage.WriteReverse( &clmbtDst[0], dwDstSize );

	return	TRUE;
}

//////////////////////////////////////////////////////////////////////////////////////////
//	��1

BOOL	CPB3B::Decomp1(
	BYTE*				pbtDst,							// �i�[��
	DWORD				dwDstSize,						// �i�[��T�C�Y
	const BYTE*			pbtSrc,							// ���k�f�[�^
	DWORD				dwSrcSize,						// ���k�f�[�^�T�C�Y
	long				lWidth,							// ����
	long				lHeight,						// �c��
	WORD				wBpp							// �r�b�g��
	)
{
	DWORD				dwOffsetOfFlags = *(DWORD*) &pbtSrc[44];
	DWORD				dwOffsetOfCompData = *(DWORD*) &pbtSrc[48];
	long				lWidthOfBlock = *(DWORD*) &pbtSrc[24];
	long				lHeightOfBlock = *(DWORD*) &pbtSrc[24];
	WORD				wByteCount = (wBpp >> 3);
	long				lLine = (lWidth * wByteCount);

	// LZSS�𓀗p�o�b�t�@�̊m��

	DWORD				dwTempSize = (lWidth * lHeight);

	YCMemory<BYTE>		clmbtTemp( dwTempSize );

	// ��

	for( WORD wColor = 0 ; wColor < wByteCount ; wColor++ )
	{
		DWORD				dwWork;

		// 

		const BYTE*			pbtWorkOfSrc = &pbtSrc[dwOffsetOfFlags];

		dwWork = (wByteCount * 4);

		for( WORD i = 0 ; i < wColor ; i++ )
		{
			dwWork += ((DWORD*) pbtWorkOfSrc)[i];
		}

		pbtWorkOfSrc += dwWork;

		// LZSS�𓀌�̃T�C�Y���擾

		DWORD				dwLZSSDecodeSize = *(DWORD*) &pbtWorkOfSrc[8];

		// �t���O�f�[�^�ւ̃|�C���^�̎擾

		const BYTE*			pbtFlags = pbtWorkOfSrc + *(DWORD*) &pbtWorkOfSrc[0] + *(DWORD*) &pbtWorkOfSrc[4] + 12;

		// ���k�f�[�^�ւ̃|�C���^�̎擾

		const BYTE*			pbtCompData = &pbtSrc[dwOffsetOfCompData];

		dwWork = (wByteCount * 4);

		for( WORD i = 0 ; i < wColor ; i++ )
		{
			dwWork += ((DWORD*) pbtCompData)[i];
		}

		pbtCompData += dwWork;

		// �o�b�t�@�s���ɑΏ�
		// dwLZSSDecodeSize�Ŗ���o�b�t�@�m�ۂ���ΑΏ�����K�v�Ȃ����ǁA�������m�ہ^�j���͍ŏ����ɗ}������

		if( dwTempSize < dwLZSSDecodeSize )
		{
			clmbtTemp.resize( dwLZSSDecodeSize );
		}

		// LZSS��

		DWORD				dwFlagsSize = 0xFFFFFFFF;	// �T�C�Y�s��
		DWORD				dwCompDataSize = 0xFFFFFFFF;	// �T�C�Y�s��

		DecompLZSS( &clmbtTemp[0], dwLZSSDecodeSize, pbtFlags, dwFlagsSize, pbtCompData, dwCompDataSize );

		// �u���b�N���k�̉�

		BYTE*				pbtWorkOfDst = &pbtDst[wColor];
		long				lBlockCountOfWidth = ((lWidth + (lWidthOfBlock - 1)) / lWidthOfBlock);
		long				lBlockCountOfHeight = ((lHeight + (lHeightOfBlock - 1)) / lHeightOfBlock);
		BYTE				btCode = 0x80;
		DWORD				dwTempPtr = 0;
		DWORD				dwFlagsPtr = 0;
		DWORD				dwCompDataPtr = 0;

		pbtFlags = pbtWorkOfSrc + 12;
		pbtCompData = pbtWorkOfSrc + *(DWORD*) &pbtWorkOfSrc[0] + 12;

		for( long lY = 0, lBlockY = 0 ; lBlockY < lBlockCountOfHeight ; lY += lHeightOfBlock, lBlockY++ )
		{
			BYTE*				pbtWorkOfDst2 = pbtWorkOfDst;
			long				lHeightOfBlockMax = ((lY + lHeightOfBlock) > lHeight) ? (lHeight - lY) : lHeightOfBlock;

			for( long lX = 0, lBlockX = 0 ; lBlockX < lBlockCountOfWidth ; lX += lWidthOfBlock, lBlockX++ )
			{
				if( btCode == 0 )
				{
					// 8�u���b�N��������

					dwFlagsPtr++;
					btCode = 0x80;
				}

				// 1�u���b�N�̏���

				BYTE*				pbtWorkOfDst3 = pbtWorkOfDst2;
				long				lWidthOfBlockMax = ((lX + lWidthOfBlock) > lWidth) ? (lWidth - lX) : lWidthOfBlock;

				if( pbtFlags[dwFlagsPtr] & btCode )
				{
					// ���k����Ă���

					for( long i = 0 ; i < lHeightOfBlockMax ; i++ )
					{
						for( long j = 0 ; j < lWidthOfBlockMax ; j++ )
						{
							pbtWorkOfDst3[j * wByteCount] = pbtCompData[dwCompDataPtr];
						}

						pbtWorkOfDst3 += lLine;
					}

					dwCompDataPtr++;
				}
				else
				{
					// ���k����Ă��Ȃ�

					for( long i = 0 ; i < lHeightOfBlockMax ; i++ )
					{
						for( long j = 0 ; j < lWidthOfBlockMax ; j++ )
						{
							pbtWorkOfDst3[j * wByteCount] = clmbtTemp[dwTempPtr++];
						}

						pbtWorkOfDst3 += lLine;
					}
				}

				// �ׂ̃u���b�N���w��

				pbtWorkOfDst2 += (lWidthOfBlock * wByteCount);
				btCode >>= 1;
			}

			pbtWorkOfDst += (lLine * lHeightOfBlock);
		}
	}

	return	TRUE;
}

//////////////////////////////////////////////////////////////////////////////////////////
//	��3

BOOL	CPB3B::Decomp3(
	BYTE*				pbtDst,							// �i�[��
	DWORD				dwDstSize,						// �i�[��T�C�Y
	const BYTE*			pbtSrc,							// ���k�f�[�^
	DWORD				dwSrcSize,						// ���k�f�[�^�T�C�Y
	long				lWidth,							// ����
	long				lHeight,						// �c��
	WORD				wBpp							// �r�b�g��
	)
{
	// �A���t�@�l�̎擾

	const BYTE*			pbtAlpha = pbtSrc + *(DWORD*) &pbtSrc[44];
	DWORD				dwAlphaSize = *(DWORD*) &pbtSrc[48];

	// ��

	CJBP1				clJBP1;

	clJBP1.Decomp( pbtDst, &pbtSrc[52], wBpp, pbtAlpha, dwAlphaSize );

	return	TRUE;
}

//////////////////////////////////////////////////////////////////////////////////////////
//	��4

BOOL	CPB3B::Decomp4(
	BYTE*				pbtDst,							// �i�[��
	DWORD				dwDstSize,						// �i�[��T�C�Y
	const BYTE*			pbtSrc,							// ���k�f�[�^
	DWORD				dwSrcSize,						// ���k�f�[�^�T�C�Y
	long				lWidth,							// ����
	long				lHeight,						// �c��
	WORD				wBpp							// �r�b�g��
	)
{
	return	TRUE;
}

//////////////////////////////////////////////////////////////////////////////////////////
//	��5

BOOL	CPB3B::Decomp5(
	BYTE*				pbtDst,							// �i�[��
	DWORD				dwDstSize,						// �i�[��T�C�Y
	const BYTE*			pbtSrc,							// ���k�f�[�^
	DWORD				dwSrcSize,						// ���k�f�[�^�T�C�Y
	long				lWidth,							// ����
	long				lHeight,						// �c��
	WORD				wBpp							// �r�b�g��
	)
{
	// LZSS�𓀗p�o�b�t�@

	DWORD				dwTempSize = (lWidth * lHeight);

	YCMemory<BYTE>		aclmbtTemp[4];

	// �x�[�X�摜�̉�

	for( int i = 0 ; i < 4 ; i++ )
	{
		// LZSS�𓀗p�o�b�t�@�̊m��

		aclmbtTemp[i].resize( dwTempSize );

		// �t���O�f�[�^�ւ̃|�C���^�̎擾

		const BYTE*			pbtFlags = pbtSrc + 84 + *(DWORD*) &pbtSrc[i * 8 + 52];

		// ���k�f�[�^�ւ̃|�C���^�̎擾

		const BYTE*			pbtCompData = pbtSrc + 84 + *(DWORD*) &pbtSrc[i * 8 + 56];

		// LZSS��

		DWORD				dwFlagsSize = 0xFFFFFFFF;	// �T�C�Y�s��
		DWORD				dwCompDataSize = 0xFFFFFFFF;	// �T�C�Y�s��

		DecompLZSS( &aclmbtTemp[i][0], dwTempSize, pbtFlags, dwFlagsSize, pbtCompData, dwCompDataSize );
	}

	// �i�[

	pbtDst[0] = aclmbtTemp[0][0];
	pbtDst[1] = aclmbtTemp[1][0];
	pbtDst[2] = aclmbtTemp[2][0];
	pbtDst[3] = aclmbtTemp[3][0];

	for( DWORD i = 1 ; i < dwTempSize ; i++ )
	{
		pbtDst[i * 4 + 0] = aclmbtTemp[0][i] + pbtDst[i * 4 - 4];
		pbtDst[i * 4 + 1] = aclmbtTemp[1][i] + pbtDst[i * 4 - 3];
		pbtDst[i * 4 + 2] = aclmbtTemp[2][i] + pbtDst[i * 4 - 2];
		pbtDst[i * 4 + 3] = aclmbtTemp[3][i] + pbtDst[i * 4 - 1];
	}

	return	TRUE;
}

//////////////////////////////////////////////////////////////////////////////////////////
//	��6
//
//	���l	pbtDst�Ƀx�[�X�摜���i�[����Ă��邱�Ƃ�O��ɂ��Ă���

BOOL	CPB3B::Decomp6(
	BYTE*				pbtDst,							// �i�[��
	DWORD				dwDstSize,						// �i�[��T�C�Y
	const BYTE*			pbtSrc,							// ���k�f�[�^
	DWORD				dwSrcSize,						// ���k�f�[�^�T�C�Y
	long				lWidth,							// ����
	long				lHeight,						// �c��
	WORD				wBpp							// �r�b�g��
	)
{
	// LZSS�𓀗p�o�b�t�@�̊m��

	DWORD				dwLZSSDecodeSize = *(DWORD*) &pbtSrc[24];

	YCMemory<BYTE>		clmbtTemp( dwLZSSDecodeSize );

	// LZSS��

	const BYTE*			pbtFlags = pbtSrc + 84;
	const BYTE*			pbtCompData = pbtSrc + 84 + *(DWORD*) &pbtSrc[44];
	DWORD				dwFlagsSize = 0xFFFFFFFF;	// �s��
	DWORD				dwCompDataSize = 0xFFFFFFFF;	// �s��

	DecompLZSS( &clmbtTemp[0], dwLZSSDecodeSize, pbtFlags, dwFlagsSize, pbtCompData, dwCompDataSize );

	// ��

	WORD				wByteCount = (wBpp >> 3);
	long				lLine = (lWidth * 4);
	long				lWidthOfBlock = 8;
	long				lHeightOfBlock = 8;
	long				lBlockCountOfWidth = ((lWidth + (lWidthOfBlock - 1)) / lWidthOfBlock);
	long				lBlockCountOfHeight = ((lHeight + (lHeightOfBlock - 1)) / lHeightOfBlock);
	DWORD				dwFlagsPtr = 0;
	DWORD				dwCompDataPtr = 0;
	BYTE				btCode = 0x80;

	pbtFlags = &clmbtTemp[8];
	pbtCompData = &clmbtTemp[*(DWORD*) &clmbtTemp[0] + 8];

	BYTE*				pbtWorkOfDst = pbtDst;

	for( long lY = 0, lBlockY = 0 ; lBlockY < lBlockCountOfHeight ; lY += lHeightOfBlock, lBlockY++ )
	{
		BYTE*				pbtWorkOfDst2 = pbtWorkOfDst;
		long				lBlockOfHeightMax = ((lY + lHeightOfBlock) > lHeight) ? (lHeight - lY) : lHeightOfBlock;

		for( long lX = 0, lBlockX = 0 ; lBlockX < lBlockCountOfWidth ; lX += lWidthOfBlock, lBlockX++ )
		{
			if( btCode == 0 )
			{
				// 8�u���b�N��������

				btCode = 0x80;
				dwFlagsPtr++;
			}

			// 1�u���b�N�̏���

			BYTE*				pbtWorkOfDst3 = pbtWorkOfDst2;
			long				lBlockOfWidthMax = ((lX + lWidthOfBlock) > lWidth) ? (lWidth - lX) : lWidthOfBlock;

			if( (pbtFlags[dwFlagsPtr] & btCode) == 0 )
			{
				// ����

				for( long i = 0 ; i < lBlockOfHeightMax ; i++ )
				{
					for( long j = 0 ; j < lBlockOfWidthMax ; j++ )
					{
						memcpy( &pbtWorkOfDst3[j * 4], &pbtCompData[dwCompDataPtr + j * 4], 4 );
					}

					dwCompDataPtr += (lBlockOfWidthMax * 4);
					pbtWorkOfDst3 += lLine;
				}
			}

			// �ׂ̃u���b�N���w��

			pbtWorkOfDst2 += (lWidthOfBlock * 4);
			btCode >>= 1;
		}

		pbtWorkOfDst += (lLine * lHeightOfBlock);
	}

	return	TRUE;
}
