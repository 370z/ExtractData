
#include	"stdafx.h"
#include	"../ExtractBase.h"
#include	"../Image.h"
#include	"Will.h"

//////////////////////////////////////////////////////////////////////////////////////////
//	�}�E���g

BOOL	CWill::Mount(
	CArcFile*			pclArc							// �}�E���g
	)
{
	if( lstrcmpi( pclArc->GetArcExten(), _T(".arc") ) != 0 )
	{
		return	FALSE;
	}

	BOOL				bMatch = FALSE;

	static const char*	apszHeader[] =
	{
		"OGG", "WSC", "ANM", "MSK", "WIP", "TBL", "SCR"
	};

	for( DWORD i = 0 ; i < _countof( apszHeader ) ; i++ )
	{
		if( memcmp( &pclArc->GetHed()[4], apszHeader[i], 4 ) == 0 )
		{
			bMatch = TRUE;
			break;
		}
	}

	if( !bMatch )
	{
		return	FALSE;
	}

	// �t�@�C���̎�ސ��擾

	DWORD				dwFileFormats;

	pclArc->Read( &dwFileFormats, 4 );

	// �t�@�C���t�H�[�}�b�g�C���f�b�N�X�擾

	DWORD				dwFormatIndexSize = 12 * dwFileFormats;

	YCMemory<BYTE>		clmbtFormatIndex( dwFormatIndexSize );
	DWORD				dwFormatIndexPtr = 0;

	pclArc->Read( &clmbtFormatIndex[0], dwFormatIndexSize );

	// �C���f�b�N�X�T�C�Y�擾

	DWORD				dwIndexSize = 0;

	for( DWORD i = 0 ; i < dwFileFormats ; i++ )
	{
		dwIndexSize += *(DWORD*) &clmbtFormatIndex[12 * i + 4] * 17;
	}

	// �C���f�b�N�X�擾

	YCMemory<BYTE>		clmbtIndex( dwIndexSize );
	DWORD				dwIndexPtr = 0;

	pclArc->Read( &clmbtIndex[0], dwIndexSize );

	// �t�@�C�����̎擾

	std::vector<SFileInfo>	vcFileInfo;
	std::vector<SFileInfo>	vcMaskFileInfo;
	std::vector<SFileInfo>	vcNotMaskFileInfo;

	for( DWORD i = 0 ; i < dwFileFormats ; i++ )
	{
		// �t�@�C���̎�ނ���g���q�擾

		TCHAR				szFileExt[8];

		memcpy( szFileExt, &clmbtFormatIndex[dwFormatIndexPtr], 4 );
		szFileExt[4] = '\0';

		::CharLower( szFileExt );

		// �t�@�C�����̎擾

		DWORD				dwFiles = *(DWORD*) &clmbtFormatIndex[dwFormatIndexPtr + 4];

		for( DWORD j = 0 ; j < dwFiles; j++ )
		{
			// �t�@�C�����擾

			char				szFileTitle[16];

			memcpy( szFileTitle, &clmbtIndex[dwIndexPtr], 9 );
			szFileTitle[9] = '\0';

			TCHAR				szFileName[32];

			_stprintf( szFileName, _T("%s.%s"), szFileTitle, szFileExt );

			// �t�@�C����񃊃X�g�ɒǉ�

			SFileInfo			stFileInfo;

			stFileInfo.name = szFileName;
			stFileInfo.sizeCmp = *(DWORD*) &clmbtIndex[dwIndexPtr + 9];
			stFileInfo.sizeOrg = stFileInfo.sizeCmp;
			stFileInfo.start = *(DWORD*) &clmbtIndex[dwIndexPtr + 13];
			stFileInfo.end = stFileInfo.start + stFileInfo.sizeCmp;

			if( lstrcmp( szFileExt, _T("msk") ) == 0 )
			{
				// �}�X�N�摜

				vcMaskFileInfo.push_back( stFileInfo );
			}
			else
			{
				vcFileInfo.push_back( stFileInfo );
			}

			dwIndexPtr += 17;
		}

		dwFormatIndexPtr += 12;
	}

	// �t�@�C�����Ń\�[�g

	std::sort( vcFileInfo.begin(), vcFileInfo.end(), CArcFile::CompareForFileInfo );

	// �}�X�N�摜�����������t�@�C�����̎擾

	for( size_t i = 0 ; i < vcMaskFileInfo.size() ; i++ )
	{
		SFileInfo*			pstsiMask = &vcMaskFileInfo[i];

		// ��������t�@�C�����̎擾

		TCHAR				szTargetName[_MAX_FNAME];

		lstrcpy( szTargetName, pstsiMask->name );

		PathRenameExtension( szTargetName, _T(".wip") );

		// ��������t�@�C�����̎擾

		SFileInfo*			pstsiTarget = NULL;

		pstsiTarget = pclArc->SearchForFileInfo( vcFileInfo, szTargetName );

		if( pstsiTarget != NULL )
		{
			// �}�X�N�摜�ł���ƒf��

			pstsiTarget->starts.push_back( pstsiMask->start );
			pstsiTarget->sizesCmp.push_back( pstsiMask->sizeCmp );
			pstsiTarget->sizesOrg.push_back( pstsiMask->sizeOrg );

			// �i���󋵍X�V

			pclArc->GetProg()->UpdatePercent( pstsiMask->sizeCmp );
		}
		else
		{
			// �}�X�N�摜�ł͂Ȃ�

			vcNotMaskFileInfo.push_back( *pstsiMask );
		}
	}

	// ���X�g�r���[�ɒǉ�

	for( size_t i = 0 ; i < vcFileInfo.size() ; i++ )
	{
		pclArc->AddFileInfo( vcFileInfo[i] );
	}

	for( size_t i = 0 ; i < vcNotMaskFileInfo.size() ; i++ )
	{
		pclArc->AddFileInfo( vcNotMaskFileInfo[i] );
	}

	return	TRUE;
}

//////////////////////////////////////////////////////////////////////////////////////////
//	�f�R�[�h

BOOL	CWill::Decode(
	CArcFile*			pclArc							// �A�[�J�C�u
	)
{
	SFileInfo*			pstFileInfo = pclArc->GetOpenFileInfo();

	if( (pstFileInfo->format != _T("WIP")) && (pstFileInfo->format != _T("MSK")) )
	{
		return	FALSE;
	}

	// �f�[�^�̓ǂݍ���

	DWORD				dwSrcSize = pstFileInfo->sizeCmp;

	YCMemory<BYTE>		clmbtSrc( dwSrcSize );
	DWORD				dwSrcPtr = 0;

	pclArc->Read( &clmbtSrc[0], dwSrcSize );

	// �t�@�C�����A�F���A�擾

	WORD				wFiles = *(WORD*) &clmbtSrc[4];
	WORD				wBpp = *(WORD*) &clmbtSrc[6];

	dwSrcPtr += 8;

	// ���A�����A�擾

	std::vector<long>	vclWidth;
	std::vector<long>	vclHeight;
	std::vector<DWORD>	vcdwSrcSize;

	for( WORD i = 0 ; i < wFiles ; i++ )
	{
		vclWidth.push_back( *(long*) &clmbtSrc[dwSrcPtr + 0] );
		vclHeight.push_back( *(long*) &clmbtSrc[dwSrcPtr + 4] );
		vcdwSrcSize.push_back( *(DWORD*) &clmbtSrc[dwSrcPtr + 20] );

		dwSrcPtr += 24;
	}

	// �}�X�N�摜�̗L��

	BOOL				bExistsMask = !pstFileInfo->starts.empty();

	// �}�X�N�摜�̎擾

	DWORD				dwSrcSizeForMask;
	YCMemory<BYTE>		clmbtSrcForMask;
	DWORD				dwSrcPtrForMask;
	WORD				wFilesForMask;
	WORD				wBppForMask;
	std::vector<long>	vclWidthForMask;
	std::vector<long>	vclHeightForMask;
	std::vector<DWORD>	vcdwSrcSizeForMask;

	if( bExistsMask )
	{
		// �}�X�N�摜�����݂���

		dwSrcSizeForMask = pstFileInfo->sizesCmp[0];
		dwSrcPtrForMask = 0;

		clmbtSrcForMask.resize( dwSrcSizeForMask );

		// �}�X�N�摜�̓ǂݍ���

		pclArc->SeekHed( pstFileInfo->starts[0] );
		pclArc->Read( &clmbtSrcForMask[0], dwSrcSizeForMask );

		// �t�@�C�����A�F���A�擾

		wFilesForMask = *(WORD*) &clmbtSrcForMask[4];
		wBppForMask = *(WORD*) &clmbtSrcForMask[6];

		dwSrcPtrForMask += 8;

		// ���A�����A�擾

		for( WORD i = 0 ; i < wFilesForMask ; i++ )
		{
			vclWidthForMask.push_back( *(long*) &clmbtSrcForMask[dwSrcPtrForMask + 0] );
			vclHeightForMask.push_back( *(long*) &clmbtSrcForMask[dwSrcPtrForMask + 4] );
			vcdwSrcSizeForMask.push_back( *(DWORD*) &clmbtSrcForMask[dwSrcPtrForMask + 20] );

			dwSrcPtrForMask += 24;
		}

		// �t�@�C�������������m�F

		bExistsMask = (wFiles == wFilesForMask);
	}

	// �o��

	for( WORD i = 0 ; i < wFiles ; i++ )
	{
		// �}�X�N�摜�̕t���ŕύX����邽�ߍĎ擾

		WORD				wBpp = *(WORD*) &clmbtSrc[6];

		// �o�͗p�o�b�t�@�m��

		DWORD				dwDstSize = vclWidth[i] * vclHeight[i] * (wBpp >> 3);

		YCMemory<BYTE>		clmbtDst( dwDstSize );

		ZeroMemory( &clmbtDst[0], dwDstSize );

		BYTE*				pbtDst = &clmbtDst[0];

		// �p���b�g�̎擾

		BYTE*				pbtPallet = NULL;

		if( wBpp == 8 )
		{
			pbtPallet = &clmbtSrc[dwSrcPtr];

			dwSrcPtr += 1024;
		}

		// LZSS��

		DecompLZSS( &clmbtDst[0], dwDstSize, &clmbtSrc[dwSrcPtr], vcdwSrcSize[i] );

		dwSrcPtr += vcdwSrcSize[i];

		// �}�X�N�摜��t������32bit��

		DWORD				dwDstSizeFor32bit;

		YCMemory<BYTE>		clmbtDstFor32bit;

		if( bExistsMask )
		{
			// �}�X�N�摜�����݂���

			DWORD				dwDstSizeForMask = vclWidthForMask[i] * vclHeightForMask[i] * (wBppForMask >> 3);

			YCMemory<BYTE>		clmbtDstForMask( dwDstSizeForMask );

			ZeroMemory( &clmbtDstForMask[0], dwDstSizeForMask );

			// �p���b�g�̎擾

			BYTE*				pbtPalletForMask = NULL;

			if( wBppForMask == 8 )
			{
				pbtPalletForMask = &clmbtSrcForMask[dwSrcPtrForMask];

				dwSrcPtrForMask += 1024;
			}

			// LZSS��

			DecompLZSS( &clmbtDstForMask[0], dwDstSizeForMask, &clmbtSrcForMask[dwSrcPtrForMask], vcdwSrcSizeForMask[i] );

			dwSrcPtrForMask += vcdwSrcSizeForMask[i];

			// �}�X�N�摜�̕t��

			dwDstSizeFor32bit = vclWidth[i] * vclHeight[i] * 4;

			clmbtDstFor32bit.resize( dwDstSizeFor32bit );

			if( AppendMask( &clmbtDstFor32bit[0], dwDstSizeFor32bit, &clmbtDst[0], dwDstSize, &clmbtDstForMask[0], dwDstSizeForMask ) )
			{
				// �}�X�N�摜�̕t���ɐ���

				wBpp = 32;
				dwDstSize = dwDstSizeFor32bit;
				pbtDst = &clmbtDstFor32bit[0];
			}
		}

		// �t�@�C�����̎擾

		TCHAR				szFileExt[256];

		if( wFiles == 1 )
		{
			// �t�@�C����1��

			lstrcpy( szFileExt, _T("") );
		}
		else
		{
			// �t�@�C����2�ȏ�

			_stprintf( szFileExt, _T("_%03d.bmp"), i );
		}

		// �v���O���X�o�[�i���v��

		BOOL				bProgress = TRUE;

		if( i >= 1 )
		{
			bProgress = FALSE;
		}

		// �o��

		CImage				clImage;

		clImage.Init( pclArc, vclWidth[i], vclHeight[i], wBpp, pbtPallet, 1024, szFileExt );
		clImage.WriteCompoBGRAReverse( pbtDst, dwDstSize, bProgress );
		clImage.Close();
	}

	return	TRUE;
}

//////////////////////////////////////////////////////////////////////////////////////////
//	LZSS�̉�

void	CWill::DecompLZSS(
	void*				pvDst,							// �i�[��
	DWORD				dwDstSize,						// �i�[��T�C�Y
	const void*			pvSrc,							// ���k�f�[�^
	DWORD				dwSrcSize						// ���k�f�[�^�T�C�Y
	)
{
	const BYTE*			pbtSrc = (const BYTE*) pvSrc;
	BYTE*				pbtDst = (BYTE*) pvDst;

	DWORD				dwSrcPtr = 0;
	DWORD				dwDstPtr = 0;

	// �X���C�h����

	DWORD				dwDicSize = 4096;

	YCMemory<BYTE>		clmbtDic( dwDicSize );

	ZeroMemory( &clmbtDic[0], dwDicSize );

	DWORD				dwDicPtr = 1;

	while( (dwSrcPtr < dwSrcSize) && (dwDstPtr < dwDstSize) )
	{
		BYTE				btFlags = pbtSrc[dwSrcPtr++];

		for( DWORD i = 0 ; (i < 8) && (dwSrcPtr < dwSrcSize) && (dwDstPtr < dwDstSize) ; i++ )
		{
			if( btFlags & 1 )
			{
				// �����k�f�[�^

				pbtDst[dwDstPtr++] = clmbtDic[dwDicPtr++] = pbtSrc[dwSrcPtr++];

				dwDicPtr &= (dwDicSize - 1);
			}
			else
			{
				// ���k�f�[�^

				BYTE				btLow = pbtSrc[dwSrcPtr++];
				BYTE				btHigh = pbtSrc[dwSrcPtr++];

				DWORD				dwBack = ((btLow << 8) | btHigh) >> 4;

				if( dwBack == 0 )
				{
					// �𓀊���

					return;
				}

				// ��������ǂݍ��ޒ����擾

				DWORD				dwLength = (btHigh & 0x0F) + 2;

				if( (dwDstPtr + dwLength) > dwDstSize )
				{
					// �o�̓o�b�t�@�𒴂��Ă��܂�

					dwLength = (dwDstSize - dwDstPtr);
				}

				// �����̃f�[�^�����

				for( DWORD j = 0 ; j < dwLength ; j++ )
				{
					pbtDst[dwDstPtr++] = clmbtDic[dwDicPtr++] = clmbtDic[dwBack++];

					dwDicPtr &= (dwDicSize - 1);
					dwBack &= (dwDicSize - 1);
				}
			}

			btFlags >>= 1;
		}
	}
}

//////////////////////////////////////////////////////////////////////////////////////////
//	�}�X�N�摜��t������32bit������

BOOL	CWill::AppendMask(
	BYTE*				pbtDst,							// �i�[��
	DWORD				dwDstSize,						// �i�[��T�C�Y
	const BYTE*			pbtSrc,							// 24bit�f�[�^
	DWORD				dwSrcSize,						// 24bit�f�[�^�T�C�Y
	const BYTE*			pbtMask,						// 8bit�f�[�^(�}�X�N)
	DWORD				dwMaskSize						// 8bit�f�[�^�T�C�Y
	)
{
	// ����

	memcpy( pbtDst, pbtSrc, dwSrcSize );

	memcpy( &pbtDst[dwSrcSize], pbtMask, dwMaskSize );

	return	TRUE;
}
