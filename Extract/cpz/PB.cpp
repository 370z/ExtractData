
#include	"stdafx.h"
#include	"../../ExtractBase.h"
#include	"PB.h"

//////////////////////////////////////////////////////////////////////////////////////////
//	LZSS�̉�

BOOL	CPB::DecompLZSS(
	void*				pvDst,							// �i�[��
	DWORD				dwDstSize,						// �i�[��T�C�Y
	const void*			pvFlags,						// �t���O�f�[�^
	DWORD				dwFlagsSize,					// �t���O�f�[�^�T�C�Y
	const void*			pvSrc,							// ���k�f�[�^
	DWORD				dwSrcSize						// ���k�f�[�^�T�C�Y
	)
{
	// �����̏�����

	DWORD				dwDicSize = 2048;

	YCMemory<BYTE>		clmbtDic( dwDicSize );

	ZeroMemory( &clmbtDic[0], dwDicSize );

	DWORD				dwDicPtr = 2014;

	// ��

	const BYTE*			pbtSrc = (const BYTE*) pvSrc;
	const BYTE*			pbtFlags = (const BYTE*) pvFlags;
	BYTE*				pbtDst = (BYTE*) pvDst;
	DWORD				dwSrcPtr = 0;
	DWORD				dwFlagsPtr = 0;
	DWORD				dwDstPtr = 0;
	BYTE				btCode = 0x80;

	while( (dwSrcPtr < dwSrcSize) && (dwDstPtr < dwDstSize) && (dwFlagsPtr < dwFlagsSize) )
	{
		if( btCode == 0 )
		{
			dwFlagsPtr++;
			btCode = 0x80;
		}

		if( pbtFlags[dwFlagsPtr] & btCode )
		{
			// ���k����Ă���

			WORD				wWork = *(WORD*) &pbtSrc[dwSrcPtr];

			DWORD				dwBack = (wWork >> 5);
			DWORD				dwLength = (wWork & 0x1F) + 3;

			// �o�b�t�@�𒴂��Ȃ��悤�ɒ���

			if( (dwDstPtr + dwLength) > dwDstSize )
			{
				dwLength = (dwDstSize - dwDstPtr);
			}

			// �����Q��

			for( DWORD i = 0 ; i < dwLength ; i++ )
			{
				pbtDst[dwDstPtr + i] = clmbtDic[dwDicPtr++] = clmbtDic[dwBack++];

				dwDicPtr &= (dwDicSize - 1);
				dwBack &= (dwDicSize - 1);
			}

			dwSrcPtr += 2;
			dwDstPtr += dwLength;
		}
		else
		{
			// �����k

			pbtDst[dwDstPtr++] = clmbtDic[dwDicPtr++] = pbtSrc[dwSrcPtr++];

			dwDicPtr &= (dwDicSize - 1);
		}

		btCode >>= 1;
	}

	return	TRUE;
}
