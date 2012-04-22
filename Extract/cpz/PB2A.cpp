
#include	"stdafx.h"
#include	"../../Image.h"
#include	"JBP1.h"
#include	"PB2A.h"

//////////////////////////////////////////////////////////////////////////////////////////
//	�f�R�[�h
//
//	���l	pvSrc�͐擪���������������

BOOL	CPB2A::Decode(
	CArcFile*			pclArc,							// �A�[�J�C�u
	void*				pvSrc,							// ���̓f�[�^
	DWORD				dwSrcSize						// ���̓f�[�^�T�C�Y
	)
{
	BYTE*				pbtSrc = (BYTE*) pvSrc;

	// ����

	Decrypt( pbtSrc, dwSrcSize );

	// �摜���̎擾

	WORD				wType = *(WORD*) &pbtSrc[16];
	long				lWidth = *(WORD*) &pbtSrc[18];
	long				lHeight = *(WORD*) &pbtSrc[20];
	WORD				wBpp = *(WORD*) &pbtSrc[22];

	// ��

	switch( wType )
	{
		case	1:
			Decode1( pclArc, pbtSrc, dwSrcSize, lWidth, lHeight, wBpp );
			break;

		case	2:
			Decode2( pclArc, pbtSrc, dwSrcSize, lWidth, lHeight, wBpp );
			break;

		case	3:
		case	4:
			Decode4( pclArc, pbtSrc, dwSrcSize, lWidth, lHeight, wBpp );
			break;

		case	5:
			Decode5( pclArc, pbtSrc, dwSrcSize, lWidth, lHeight, wBpp );
			break;

		case	6:
			Decode6( pclArc, pbtSrc, dwSrcSize, lWidth, lHeight, wBpp );
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

void	CPB2A::Decrypt(
	BYTE*				pbtTarget,						// �����Ώۃf�[�^
	DWORD				dwSize							// �����T�C�Y
	)
{
	for( int i = 8 ; i < 32 ; i += 2 )
	{
		*(WORD*) &pbtTarget[i] ^= *(WORD*) &pbtTarget[dwSize - 3];
	}

	for( int i = 8, j = 0 ; i < 32 ; i++, j++ )
	{
		pbtTarget[i] -= pbtTarget[dwSize - 27 + j];
	}
}

//////////////////////////////////////////////////////////////////////////////////////////
//	�f�R�[�h1

BOOL	CPB2A::Decode1(
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
//	�f�R�[�h1

BOOL	CPB2A::Decode2(
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

	Decomp2( &clmbtDst[0], dwDstSize, pbtSrc, dwSrcSize, lWidth, lHeight, wBpp );

	// �o��

	CImage				clImage;

	clImage.Init( pclArc, lWidth, lHeight, wBpp );
	clImage.WriteReverse( &clmbtDst[0], dwDstSize );

	return	TRUE;
}

//////////////////////////////////////////////////////////////////////////////////////////
//	�f�R�[�h4

BOOL	CPB2A::Decode4(
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

	Decomp4( &clmbtDst[0], dwDstSize, pbtSrc, dwSrcSize, lWidth, lHeight, wBpp );

	// �o��

	CImage				clImage;

	clImage.Init( pclArc, lWidth, lHeight, wBpp );
	clImage.WriteReverse( &clmbtDst[0], dwDstSize );

	return	TRUE;
}

//////////////////////////////////////////////////////////////////////////////////////////
//	�f�R�[�h5

BOOL	CPB2A::Decode5(
	CArcFile*			pclArc,							// �A�[�J�C�u
	const BYTE*			pbtSrc,							// ���k�f�[�^
	DWORD				dwSrcSize,						// ���k�f�[�^�T�C�Y
	long				lWidth,							// ����
	long				lHeight,						// �c��
	WORD				wBpp							// �r�b�g��
	)
{
	// �x�[�X�摜�p�o�b�t�@�m��

	DWORD				dwBaseSize = (lWidth * lHeight * 4);

	YCMemory<BYTE>		clmbtBase( dwBaseSize );

	// �x�[�X�摜�̉�

	Decomp5( &clmbtBase[0], dwBaseSize, pbtSrc, dwSrcSize, lWidth, lHeight, wBpp, NULL, 0 );

	// �x�[�X�摜�̏o��

	CImage				clImage;

	clImage.Init( pclArc, lWidth, lHeight, 32 );
	clImage.WriteReverse( &clmbtBase[0], dwBaseSize );
	clImage.Close();

	// �摜���̎擾

	DWORD				dwImageFiles = *(DWORD*) &pbtSrc[8];

	if( dwImageFiles == 1 )
	{
		// �x�[�X�摜�������݂��Ȃ�

		return	TRUE;
	}

	// �����摜�p�o�b�t�@�m��

	DWORD				dwDstSize = dwBaseSize;

	YCMemory<BYTE>		clmbtDst( dwDstSize );

	// �����摜�̏o��

	for( DWORD dwFrameNumber = 1 ; dwFrameNumber < dwImageFiles ; dwFrameNumber++ )
	{
		// �����摜�̉�

		Decomp5( &clmbtDst[0], dwDstSize, pbtSrc, dwSrcSize, lWidth, lHeight, wBpp, &clmbtBase[0], dwFrameNumber );

		// �����摜�̏o��

		TCHAR				szDiffName[256];

		_stprintf( szDiffName, _T("_%02d"), (dwFrameNumber - 1) );

		clImage.Init( pclArc, lWidth, lHeight, 32, NULL, 0, szDiffName );
		clImage.WriteReverse( &clmbtDst[0], dwDstSize, FALSE );
		clImage.Close();
	}

	return	TRUE;
}

//////////////////////////////////////////////////////////////////////////////////////////
//	�f�R�[�h6

BOOL	CPB2A::Decode6(
	CArcFile*			pclArc,							// �A�[�J�C�u
	const BYTE*			pbtSrc,							// ���k�f�[�^
	DWORD				dwSrcSize,						// ���k�f�[�^�T�C�Y
	long				lWidth,							// ����
	long				lHeight,						// �c��
	WORD				wBpp							// �r�b�g��
	)
{
	// �x�[�X�摜�p�o�b�t�@�m��

	DWORD				dwBaseSize = (lWidth * lHeight * 4);

	YCMemory<BYTE>		clmbtBase( dwBaseSize );

	// �x�[�X�摜�̉�

	Decomp6( &clmbtBase[0], dwBaseSize, pbtSrc, dwSrcSize, lWidth, lHeight, 32, NULL, 0 );

	// �x�[�X�摜�̏o��

	CImage				clImage;

	clImage.Init( pclArc, lWidth, lHeight, 32 );
	clImage.WriteReverse( &clmbtBase[0], dwBaseSize );
	clImage.Close();

	// �摜���̎擾

	DWORD				dwImageFiles = *(DWORD*) &pbtSrc[8];

	if( dwImageFiles == 1 )
	{
		// �x�[�X�摜�������݂��Ȃ�

		return	TRUE;
	}

	// �����摜�p�o�b�t�@�m��

	DWORD				dwDstSize = dwBaseSize;

	YCMemory<BYTE>		clmbtDst( dwDstSize );

	// �����摜�̏o��

	for( DWORD dwFrameNumber = 1 ; dwFrameNumber < dwImageFiles ; dwFrameNumber++ )
	{
		// �����摜�̉�

		Decomp6( &clmbtDst[0], dwDstSize, pbtSrc, dwSrcSize, lWidth, lHeight, 32, &clmbtBase[0], dwFrameNumber );

		// �����摜�̏o��

		TCHAR				szDiffName[256];

		_stprintf( szDiffName, _T("_%02d"), (dwFrameNumber - 1) );

		clImage.Init( pclArc, lWidth, lHeight, 32, NULL, 0, szDiffName );
		clImage.WriteReverse( &clmbtDst[0], dwDstSize, FALSE );
		clImage.Close();
	}

	return	TRUE;
}

//////////////////////////////////////////////////////////////////////////////////////////
//	��1

BOOL	CPB2A::Decomp1(
	BYTE*				pbtDst,							// �i�[��
	DWORD				dwDstSize,						// �i�[��T�C�Y
	const BYTE*			pbtSrc,							// ���k�f�[�^
	DWORD				dwSrcSize,						// ���k�f�[�^�T�C�Y
	long				lWidth,							// ����
	long				lHeight,						// �c��
	WORD				wBpp							// �r�b�g��
	)
{
	DWORD				dwOffsetOfFlags = *(DWORD*) &pbtSrc[24];
	DWORD				dwOffsetOfCompData = *(DWORD*) &pbtSrc[28];
	long				lWidthOfBlock = 8;
	long				lHeightOfBlock = 8;
	WORD				wByteCount = (wBpp >> 3);
	long				lLine = (lWidth * wByteCount);

	// LZSS�𓀗p�o�b�t�@�̊m��

	DWORD				dwTempSize = (lWidth * lHeight * wByteCount);
	DWORD				dwTempPtr = 0;

	YCMemory<BYTE>		clmbtTemp( dwTempSize );

	// LZSS��

	const BYTE*			pbtFlags = &pbtSrc[dwOffsetOfFlags];
	const BYTE*			pbtCompData = &pbtSrc[dwOffsetOfCompData];
	DWORD				dwFlagsSize = (dwOffsetOfCompData - dwOffsetOfFlags);
	DWORD				dwCompDataSize = (dwSrcSize - dwOffsetOfCompData);

	DecompLZSS( &clmbtTemp[0], dwTempSize, pbtFlags, dwFlagsSize, pbtCompData, dwCompDataSize );

	// �F�����̓���ւ�

	long				lBlockCountOfWidth = ((lWidth + (lWidthOfBlock - 1)) / lWidthOfBlock);
	long				lBlockCountOfHeight = ((lHeight + (lHeightOfBlock - 1)) / lHeightOfBlock);

	for( WORD wColor = 0 ; wColor < wByteCount ; wColor++ )
	{
		BYTE*				pbtWorkOfDst = &pbtDst[wColor];

		for( long lY = 0, lBlockY = 0 ; lBlockY < lBlockCountOfHeight ; lY += lHeightOfBlock, lBlockY++ )
		{
			BYTE*				pbtWorkOfDst2 = pbtWorkOfDst;
			long				lHeightOfBlockMax = ((lY + lHeightOfBlock) > lHeight) ? (lHeight - lY) : lHeightOfBlock;

			// 1�񕪂̃u���b�N������

			for( long lX = 0, lBlockX = 0 ; lBlockX < lBlockCountOfWidth ; lX += lWidthOfBlock, lBlockX++ )
			{
				BYTE*				pbtWorkOfDst3 = pbtWorkOfDst2;
				long				lWidthOfBlockMax = ((lX + lWidthOfBlock) > lWidth) ? (lWidth - lX) : lWidthOfBlock;

				// 1�u���b�N�̏���

				for( long i = 0 ; i < lHeightOfBlockMax ; i++ )
				{
					for( long j = 0 ; j < lWidthOfBlockMax ; j++ )
					{
						pbtWorkOfDst3[j * wByteCount] = clmbtTemp[dwTempPtr++];
					}

					pbtWorkOfDst3 += lLine;
				}

				// �ׂ̃u���b�N���w��

				pbtWorkOfDst2 += (lWidthOfBlock * wByteCount);
			}

			// ���̃u���b�N���w��

			pbtWorkOfDst += (lLine * lHeightOfBlock);
		}
	}

	return	TRUE;
}


//////////////////////////////////////////////////////////////////////////////////////////
//	��2

BOOL	CPB2A::Decomp2(
	BYTE*				pbtDst,							// �i�[��
	DWORD				dwDstSize,						// �i�[��T�C�Y
	const BYTE*			pbtSrc,							// ���k�f�[�^
	DWORD				dwSrcSize,						// ���k�f�[�^�T�C�Y
	long				lWidth,							// ����
	long				lHeight,						// �c��
	WORD				wBpp							// �r�b�g��
	)
{
	DWORD				dwOffsetOfFlags = *(DWORD*) &pbtSrc[24];
	DWORD				dwOffsetOfCompData = *(DWORD*) &pbtSrc[28];
	long				lWidthOfBlock = 8;
	long				lHeightOfBlock = 8;
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

		long				lBlockCountOfWidth = ((lWidth + (lWidthOfBlock - 1)) / lWidthOfBlock);
		long				lBlockCountOfHeight = ((lHeight + (lHeightOfBlock - 1)) / lHeightOfBlock);
		BYTE				btCode = 0x80;
		DWORD				dwTempPtr = 0;
		DWORD				dwFlagsPtr = 0;
		DWORD				dwCompDataPtr = 0;

		pbtFlags = pbtWorkOfSrc + 12;
		pbtCompData = pbtWorkOfSrc + *(DWORD*) &pbtWorkOfSrc[0] + 12;

		BYTE*				pbtWorkOfDst = &pbtDst[wColor];

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
//	��4

BOOL	CPB2A::Decomp4(
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

	const BYTE*			pbtAlpha = pbtSrc + *(DWORD*) &pbtSrc[24];
	DWORD				dwAlphaSize = *(DWORD*) &pbtSrc[28];

	// ��

	CJBP1				clJBP1;

	clJBP1.Decomp( pbtDst, &pbtSrc[32], wBpp, pbtAlpha, dwAlphaSize );

	return	TRUE;
}

//////////////////////////////////////////////////////////////////////////////////////////
//	��5
//
//	���l	pbtBase��NULL���w�肳�ꂽ�ꍇ�͍����������s��Ȃ�

BOOL	CPB2A::Decomp5(
	BYTE*				pbtDst,							// �i�[��
	DWORD				dwDstSize,						// �i�[��T�C�Y
	const BYTE*			pbtSrc,							// ���k�f�[�^
	DWORD				dwSrcSize,						// ���k�f�[�^�T�C�Y
	long				lWidth,							// ����
	long				lHeight,						// �c��
	WORD				wBpp,							// �r�b�g��
	const BYTE*			pbtBase,						// �x�[�X�摜
	DWORD				dwFrameNumber					// �t���[���ԍ�
	)
{
	// ��

	if( dwFrameNumber == 0 )
	{
		// �x�[�X�摜

		// LZSS�𓀗p�o�b�t�@�̊m��

		DWORD				dwTempSize = (lWidth * lHeight);

		YCMemory<BYTE>		aclmbtTemp[4];

		// LZSS��

		for( WORD i = 0 ; i < 4 ; i++ )
		{
			// LZSS�𓀗p�o�b�t�@�̊m��

			aclmbtTemp[i].resize( dwTempSize );

			// LZSS��

			const BYTE*			pbtFlags = pbtSrc + 64 + *(DWORD*) &pbtSrc[i * 8 + 32];
			const BYTE*			pbtCompData = pbtSrc + 64 + *(DWORD*) &pbtSrc[i * 8 + 36];
			DWORD				dwFlagsSize = 0xFFFFFFFF;	// �s��
			DWORD				dwCompDataSize = 0xFFFFFFFF;	// �s��

			DecompLZSS( &aclmbtTemp[i][0], dwTempSize, pbtFlags, dwFlagsSize, pbtCompData, dwCompDataSize );
		}

		for( DWORD i = 0 ; i < dwTempSize ; i++ )
		{
			pbtDst[i * 4 + 0] = aclmbtTemp[0][i];
			pbtDst[i * 4 + 1] = aclmbtTemp[1][i];
			pbtDst[i * 4 + 2] = aclmbtTemp[2][i];
			pbtDst[i * 4 + 3] = aclmbtTemp[3][i];
		}

		// ����

		for( DWORD i = 0 ; i < dwTempSize ; i++ )
		{
			BYTE				btData1 = pbtDst[i * 4 + 2] ^ pbtDst[i * 4 + 3];
			BYTE				btData2 = pbtDst[i * 4 + 1] ^ btData1;

			pbtDst[i * 4 + 2] = btData1;
			pbtDst[i * 4 + 1] = btData2;
			pbtDst[i * 4 + 0] ^= btData2;
		}
	}
	else
	{
		// �����摜

		const BYTE*			pbtWorkOfSrc = pbtSrc + *(DWORD*) &pbtSrc[12];

		for( DWORD i = 1 ; i < dwFrameNumber ; i++ )
		{
			pbtWorkOfSrc += *(DWORD*) &pbtWorkOfSrc[0];
		}

		// LZSS�𓀗p�o�b�t�@�̊m��

		DWORD				dwLZSSDecodeSize = *(DWORD*) &pbtWorkOfSrc[12];

		YCMemory<BYTE>		clmbtTemp( dwLZSSDecodeSize );

		// LZSS��

		const BYTE*			pbtFlags = pbtWorkOfSrc + 16;
		const BYTE*			pbtCompData = pbtWorkOfSrc + 16 + *(DWORD*) &pbtWorkOfSrc[4];
		DWORD				dwFlagsSize = 0xFFFFFFFF;	// �s��
		DWORD				dwCompDataSize = 0xFFFFFFFF;	// �s��

		DecompLZSS( &clmbtTemp[0], dwLZSSDecodeSize, pbtFlags, dwFlagsSize, pbtCompData, dwCompDataSize );

		// �x�[�X�摜���R�s�[

		if( pbtBase != NULL )
		{
			memcpy( pbtDst, pbtBase, dwDstSize );
		}
		else
		{
			memset( pbtDst, 0xFF, dwDstSize );
		}

		// ��

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
			// 1�񕪂̃u���b�N������

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

					for( int i = 0 ; i < lBlockOfHeightMax ; i++ )
					{
						for( int j = 0 ; j < lBlockOfWidthMax ; j++ )
						{
							memcpy( &pbtWorkOfDst3[j * 4], &pbtCompData[dwCompDataPtr + j * 3], 3 );

							pbtWorkOfDst3[j * 4 + 3] = 0xFF;
						}

						dwCompDataPtr += 3;
						pbtWorkOfDst3 += lLine;
					}
				}

				// �ׂ̃u���b�N���w��

				pbtWorkOfDst2 += (lWidthOfBlock * 4);
				btCode >>= 1;
			}

			pbtWorkOfDst += (lLine * lHeightOfBlock);
		}
	}

	return	TRUE;
}

//////////////////////////////////////////////////////////////////////////////////////////
//	��6
//
//	���l	pbtBase��NULL���w�肳�ꂽ�ꍇ�͍����������s��Ȃ�

BOOL	CPB2A::Decomp6(
	BYTE*				pbtDst,							// �i�[��
	DWORD				dwDstSize,						// �i�[��T�C�Y
	const BYTE*			pbtSrc,							// ���k�f�[�^
	DWORD				dwSrcSize,						// ���k�f�[�^�T�C�Y
	long				lWidth,							// ����
	long				lHeight,						// �c��
	WORD				wBpp,							// �r�b�g��
	const BYTE*			pbtBase,						// �x�[�X�摜
	DWORD				dwFrameNumber					// �t���[���ԍ�
	)
{
	// ��

	if( dwFrameNumber == 0 )
	{
		// �x�[�X�摜

		const BYTE*			pbtBaseOfSrc = pbtSrc + 36 + ((strlen( (char*) pbtSrc + 36 ) + 4) & 0xFFFFFFFC);

		// LZSS�𓀗p�o�b�t�@�̊m��

		DWORD				dwTempSize = (lWidth * lHeight);

		YCMemory<BYTE>		aclmbtTemp[4];

		// LZSS��

		for( WORD i = 0 ; i < 4 ; i++ )
		{
			// LZSS�𓀗p�o�b�t�@�̊m��

			aclmbtTemp[i].resize( dwTempSize );

			// LZSS��

			const BYTE*			pbtFlags = pbtBaseOfSrc + 32 + *(DWORD*) &pbtBaseOfSrc[i * 8];
			const BYTE*			pbtCompData = pbtBaseOfSrc + 32 + *(DWORD*) &pbtBaseOfSrc[i * 8 + 4];
			DWORD				dwFlagsSize = 0xFFFFFFFF;	// �s��
			DWORD				dwCompDataSize = 0xFFFFFFFF;	// �s��

			DecompLZSS( &aclmbtTemp[i][0], dwTempSize, pbtFlags, dwFlagsSize, pbtCompData, dwCompDataSize );
		}

		for( DWORD i = 0 ; i < dwTempSize ; i++ )
		{
			pbtDst[i * 4 + 0] = aclmbtTemp[0][i];
			pbtDst[i * 4 + 1] = aclmbtTemp[1][i];
			pbtDst[i * 4 + 2] = aclmbtTemp[2][i];
			pbtDst[i * 4 + 3] = aclmbtTemp[3][i];
		}

		// ����

		for( DWORD i = 0 ; i < dwTempSize ; i++ )
		{
			BYTE				btData1 = pbtDst[i * 4 + 2] ^ pbtDst[i * 4 + 3];
			BYTE				btData2 = pbtDst[i * 4 + 1] ^ btData1;

			pbtDst[i * 4 + 2] = btData1;
			pbtDst[i * 4 + 1] = btData2;
			pbtDst[i * 4 + 0] ^= btData2;
		}
	}
	else
	{
		// �����摜

		const BYTE*			pbtSrcOfDiff = pbtSrc + *(DWORD*) &pbtSrc[32] + 32;

		for( DWORD i = 1 ; i < dwFrameNumber ; i++ )
		{
			pbtSrcOfDiff += *(DWORD*) &pbtSrcOfDiff[0];
		}

		// �����摜�t�@�C�����̎擾

		char				szDiffName[256];

		size_t				uNameLen = ((strlen( (char*) pbtSrcOfDiff + 4) + 4 ) & 0xFFFFFFFC) + 4;

		strcpy( szDiffName, (char*) pbtSrcOfDiff + 4 );

		// LZSS�𓀗p�o�b�t�@�̊m��

		DWORD				dwLZSSDecodeSize = *(DWORD*) &pbtSrcOfDiff[uNameLen + 8];

		YCMemory<BYTE>		clmbtTemp( dwLZSSDecodeSize );

		// LZSS��

		const BYTE*			pbtFlags = pbtSrcOfDiff + 16 + uNameLen;
		const BYTE*			pbtCompData = pbtSrcOfDiff + 16 + uNameLen + *(DWORD*) &pbtSrcOfDiff[uNameLen];
		DWORD				dwFlagsSize = 0xFFFFFFFF;	// �s��
		DWORD				dwCompDataSize = 0xFFFFFFFF;	// �s��

		DecompLZSS( &clmbtTemp[0], dwLZSSDecodeSize, pbtFlags, dwFlagsSize, pbtCompData, dwCompDataSize );

		// �x�[�X�摜���R�s�[

		if( pbtBase != NULL )
		{
			memcpy( pbtDst, pbtBase, dwDstSize );
		}
		else
		{
			memset( pbtDst, 0xFF, dwDstSize );
		}

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

			// 1�񕪂̃u���b�N������

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
	}

	return	TRUE;
}
