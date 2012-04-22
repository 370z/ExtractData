
#include	"stdafx.h"
#include	"../ExtractBase.h"
#include	"../Arc/LZSS.h"
#include	"../Image.h"
#include	"LostChild.h"

//////////////////////////////////////////////////////////////////////////////////////////
//	�}�E���g

BOOL	CLostChild::Mount(
	CArcFile*			pclArc							// �A�[�J�C�u
	)
{
	if( memcmp( pclArc->GetHed(), "EPK ", 4 ) != 0 )
	{
		return	FALSE;
	}

	pclArc->SeekHed( 4 );

	// �C���f�b�N�X�T�C�Y�擾

	DWORD				dwIndexSize;

	pclArc->Read( &dwIndexSize, 4 );

	dwIndexSize -= 32;

	pclArc->SeekCur( 16 );

	// �t�@�C�����擾

	DWORD				dwFiles;

	pclArc->Read( &dwFiles, 4 );

	pclArc->SeekCur( 4 );

	// �C���f�b�N�X�擾

	YCMemory<BYTE>		clmIndex( dwIndexSize );

	pclArc->Read( &clmIndex[0], dwIndexSize );

	// �t�@�C�����C���f�b�N�X

	BYTE*				pbtFileNameIndex = &clmIndex[*(DWORD*) &clmIndex[8]] - 32;

	// �����A�[�J�C�u�t�@�C��

	if( pclArc->GetArcName() == _T("data.epk") )
	{
		YCString			clsPathToArc = pclArc->GetArcPath();

		for( int i = 1 ; i <= 3 ; i++ )
		{
			YCString			clsArcExt;

			clsArcExt.Format( _T(".e%02d"), i );

			clsPathToArc.RenameExtension( clsArcExt );

			pclArc->Open( clsPathToArc );

			pclArc->GetProg()->ReplaceAllFileSize( pclArc->GetArcSize() );
		}

		pclArc->SetFirstArc();
	}

	// �t�@�C�����̎擾

	DWORD				dwIndexPtr = 0;
	DWORD				dwFileNameIndexPtr = 0;

	for( DWORD i = 0 ; i < dwFiles ; i++ )
	{
		// �t�@�C�����擾

		char				szFileName[_MAX_FNAME];
		DWORD				dwLength = *(DWORD*) &pbtFileNameIndex[dwFileNameIndexPtr];

		for( DWORD j = 0 ; j < dwLength ; j++ )
		{
			szFileName[j] = pbtFileNameIndex[dwFileNameIndexPtr + 4 + j] ^ 0xFF;
		}

		szFileName[dwLength] = pbtFileNameIndex[dwFileNameIndexPtr + 4 + dwLength];

		dwFileNameIndexPtr += 4 + dwLength + 1;

		// ���X�g�ɒǉ�

		SFileInfo			stFileInfo;

		stFileInfo.name = szFileName;
		stFileInfo.start = *(UINT64*) &clmIndex[dwIndexPtr + 16];
		stFileInfo.sizeCmp = *(DWORD*) &clmIndex[dwIndexPtr + 24];
		stFileInfo.sizeOrg = stFileInfo.sizeCmp;
		stFileInfo.title = _T("LOST CHILD");

		dwIndexPtr += 40;

		if( stFileInfo.start > 3900000000 )
		{
			stFileInfo.start -= 3900000000;

			if( pclArc->GetArcsID() == 2 )
			{
				pclArc->SetNextArc();
			}
		}
		else if( stFileInfo.start > 2600000000 )
		{
			stFileInfo.start -= 2600000000;

			if( pclArc->GetArcsID() == 1 )
			{
				pclArc->SetNextArc();
			}
		}
		else if( stFileInfo.start > 1300000000 )
		{
			stFileInfo.start -= 1300000000;

			if( pclArc->GetArcsID() == 0 )
			{
				pclArc->SetNextArc();
			}
		}

		// �t�@�C���T�C�Y�𑫂����Ƃ�1.2GB�𒴂�����1.2GB�����Ē���

		stFileInfo.end = stFileInfo.start + stFileInfo.sizeCmp;

		if( stFileInfo.end > 1300000000 )
		{
			stFileInfo.end -= 1300000000;
		}

		pclArc->AddFileInfo( stFileInfo );
	}

	pclArc->SetFirstArc();

	return	TRUE;
}

//////////////////////////////////////////////////////////////////////////////////////////
//	�f�R�[�h

BOOL	CLostChild::Decode(
	CArcFile*			pclArc							// �A�[�J�C�u
	)
{
	SFileInfo*			pstFileInfo = pclArc->GetOpenFileInfo();

	if( pstFileInfo->title != _T("LOST CHILD") )
	{
		return	FALSE;
	}

	if( DecodeESUR( pclArc ) )
	{
		return	TRUE;
	}

	if( DecodeLAD( pclArc ) )
	{
		return	TRUE;
	}

	return	Extract( pclArc );
}

//////////////////////////////////////////////////////////////////////////////////////////
//	ESUR�̃f�R�[�h

BOOL	CLostChild::DecodeESUR(
	CArcFile*			pclArc							// �A�[�J�C�u
	)
{
	SFileInfo*			pstFileInfo = pclArc->GetOpenFileInfo();

	if( pstFileInfo->format != _T("SUR") )
	{
		return	FALSE;
	}

	DWORD				dwReadSize;

	// �ǂݍ���

	DWORD				dwSrcSize = pstFileInfo->sizeCmp;

	YCMemory<BYTE>		clmSrc( dwSrcSize );

	dwReadSize = pclArc->Read( &clmSrc[0], dwSrcSize );

	if( dwReadSize < dwSrcSize )
	{
		pclArc->SetNextArc();
		pclArc->SeekHed();

		pclArc->Read( &clmSrc[dwReadSize], (dwSrcSize - dwReadSize) );
	}

	// �w�b�_���̎擾

	long				lWidth = *(long*) &clmSrc[8];
	long				lHeight = *(long*) &clmSrc[12];
	DWORD				dwDstSize = *(DWORD*) &clmSrc[4] - 32;
	WORD				wBpp = 32;

	// LZSS�𓀗p�o�b�t�@�̊m��

	YCMemory<BYTE>		clmDst( dwDstSize );

	// LZSS��

	DecompLZSS( &clmDst[0], dwDstSize, &clmSrc[32], (dwSrcSize - 32), 4096, 4078, 3 );

	// �o��

	CImage				clImage;

	clImage.Init( pclArc, lWidth, lHeight, wBpp );
	clImage.WriteReverse( &clmDst[0], dwDstSize );
	clImage.Close();

	return	TRUE;
}

//////////////////////////////////////////////////////////////////////////////////////////
//	LAD�̃f�R�[�h

BOOL	CLostChild::DecodeLAD(
	CArcFile*			pclArc							// �A�[�J�C�u
	)
{
	SFileInfo*			pstFileInfo = pclArc->GetOpenFileInfo();

	if( pstFileInfo->format != _T("LAD") )
	{
		return	FALSE;
	}

	DWORD				dwReadSize;

	// �ǂݍ���

	DWORD				dwSrcSize = pstFileInfo->sizeCmp;

	YCMemory<BYTE>		clmSrc( dwSrcSize );

	dwReadSize = pclArc->Read( &clmSrc[0], dwSrcSize );

	if( dwReadSize < dwSrcSize )
	{
		pclArc->SetNextArc();
		pclArc->SeekHed();

		pclArc->Read( &clmSrc[dwReadSize], (dwSrcSize - dwReadSize) );
	}

	// �w�b�_���̎擾

	long				lWidth = *(long*) &clmSrc[8];
	long				lHeight = *(long*) &clmSrc[12];
	DWORD				dwDstSize = *(DWORD*) &clmSrc[28];
	WORD				wBpp = 8;

	// LZSS�𓀗p�o�b�t�@�̊m��

	YCMemory<BYTE>		clmDst( dwDstSize );

	// LZSS��

	DecompLZSS( &clmDst[0], dwDstSize, &clmSrc[32], (dwSrcSize - 32), 4096, 4078, 3 );

	pclArc->OpenFile();
	pclArc->WriteFile( &clmDst[0], dwDstSize, dwSrcSize );
	pclArc->CloseFile();

	// �o��

//	CImage				clImage;

//	clImage.Init( pclArc, lWidth, lHeight, wBpp );
//	clImage.WriteReverse( &clmDst[0], dwDstSize );
//	clImage.Close();

	return	TRUE;
}

//////////////////////////////////////////////////////////////////////////////////////////
//	LZSS�̉�

BOOL	CLostChild::DecompLZSS(
	void*				pvDst,							// �i�[��
	DWORD				dwDstSize,						// �i�[��T�C�Y
	const void*			pvSrc,							// ���k�f�[�^
	DWORD				dwSrcSize,						// ���k�f�[�^�T�C�Y
	DWORD				dwDicSize,						// �����T�C�Y
	DWORD				dwDicPtr,						// ���������Q�ƈʒu
	DWORD				dwLengthOffset					// ��v���̃I�t�Z�b�g
	)
{
	BYTE*				pbtDst = (BYTE*) pvDst;
	const BYTE*			pbtSrc = (const BYTE*) pvSrc;

	// �����o�b�t�@�̊m��

	YCMemory<BYTE>		clmbtDic( dwDicSize );

	ZeroMemory( &clmbtDic[0], dwDicSize );

	// ��

	DWORD				dwSrcPtr = 0;
	DWORD				dwDstPtr = 0;
	BYTE				btFlags;
	DWORD				dwBitCount = 0;

	while( (dwSrcPtr < dwSrcSize) && (dwDstPtr < dwDstSize) )
	{
		if( dwBitCount == 0 )
		{
			// 8bit�ǂݐ؂���

			btFlags = pbtSrc[dwSrcPtr++];
			dwBitCount = 8;
		}

		if( btFlags & 1 )
		{
			// �����k�f�[�^

			pbtDst[dwDstPtr] = clmbtDic[dwDicPtr] = pbtSrc[dwSrcPtr];

			dwDstPtr++;
			dwSrcPtr++;
			dwDicPtr++;

			dwDicPtr &= (dwDicSize - 1);
		}
		else
		{
			// ���k�f�[�^

			BYTE				btLow = pbtSrc[dwSrcPtr++];
			BYTE				btHigh = pbtSrc[dwSrcPtr++];

			DWORD				dwBack = ((btLow << 4) | (btHigh >> 4));
			DWORD				dwLength = ((btHigh & 0x0F) + dwLengthOffset);

			if( (dwDstPtr + dwLength) > dwDstSize )
			{
				// �o�̓o�b�t�@�𒴂��Ă��܂�

				dwLength = (dwDstSize - dwDstPtr);
			}

			for( DWORD j = 0 ; j < dwLength ; j++ )
			{
				pbtDst[dwDstPtr] = clmbtDic[dwDicPtr] = clmbtDic[dwBack];

				dwDstPtr++;
				dwDicPtr++;
				dwBack++;

				dwDicPtr &= (dwDicSize - 1);
				dwBack &= (dwDicSize - 1);
			}
		}

		btFlags >>= 1;
		dwBitCount--;
	}

	return	TRUE;
}

//////////////////////////////////////////////////////////////////////////////////////////
//	���o

BOOL	CLostChild::Extract(
	CArcFile*			pclArc							// �A�[�J�C�u
	)
{
	SFileInfo*			pstFileInfo = pclArc->GetOpenFileInfo();

	if( pstFileInfo->title != _T("LOST CHILD") )
	{
		return	FALSE;
	}

	// �o�b�t�@�m��

	DWORD				dwBufferSize = pclArc->GetBufSize();

	YCMemory<BYTE>		clmBuffer( dwBufferSize );

	// �o�̓t�@�C������

	pclArc->OpenFile();

	for( DWORD WriteSize = 0 ; WriteSize != pstFileInfo->sizeCmp ; WriteSize += dwBufferSize )
	{
		// �o�b�t�@�T�C�Y����

		pclArc->SetBufSize( &dwBufferSize, WriteSize );

		// �ǂݏ���

		DWORD				dwReadSize;

		dwReadSize = pclArc->Read( &clmBuffer[0], dwBufferSize );
		pclArc->WriteFile( &clmBuffer[0], dwReadSize );

		if( dwReadSize < dwBufferSize )
		{
			pclArc->SetNextArc();

			pclArc->SeekHed();
			pclArc->Read( &clmBuffer[0], (dwBufferSize - dwReadSize) );
			pclArc->WriteFile( &clmBuffer[0], (dwBufferSize - dwReadSize) );
		}
	}

	pclArc->CloseFile();

	return	TRUE;
}
