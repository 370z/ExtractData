
#include	"stdafx.h"
#include	"../ExtractBase.h"
#include	"../Image.h"
#include	"../Tga.h"
#include	"Himauri.h"

//////////////////////////////////////////////////////////////////////////////////////////
//	�}�E���g

BOOL	CHimauri::Mount(
	CArcFile*			pclArc							// �A�[�J�C�u
	)
{
	if( pclArc->GetArcExten() != _T(".hxp") )
	{
		return	FALSE;
	}

	if( MountHim4( pclArc) )
	{
		return	TRUE;
	}

	if( MountHim5( pclArc ) )
	{
		return	TRUE;
	}

	return	FALSE;
}

//////////////////////////////////////////////////////////////////////////////////////////
//	Him4�̃}�E���g

BOOL	CHimauri::MountHim4(
	CArcFile*			pclArc							// �A�[�J�C�u
	)
{
	if( memcmp( pclArc->GetHed(), "Him4", 4 ) != 0 )
	{
		return	FALSE;
	}

	// �t�@�C�����擾

	DWORD				dwFiles;

	pclArc->SeekHed( 4 );
	pclArc->Read( &dwFiles, 4 );

	// �t�@�C��������C���f�b�N�X�T�C�Y�擾

	DWORD				dwIndexSize = dwFiles * 4;

	// �C���f�b�N�X�擾

	YCMemory<BYTE>		clmbtIndex( dwIndexSize );
	DWORD				dwIndexPtr = 0;

	pclArc->Read( &clmbtIndex[0], dwIndexSize );

	TCHAR				szArcName[_MAX_FNAME];

	lstrcpy( szArcName, pclArc->GetArcName() );
	PathRemoveExtension( szArcName );

	for( DWORD i = 0 ; i < dwFiles ; i++ )
	{
		TCHAR				szFileName[_MAX_FNAME];

		_stprintf( szFileName, _T("%s_%06d"), szArcName, i );

		// ���X�g�r���[�ɒǉ�

		SFileInfo			stFileInfo;

		stFileInfo.name = szFileName;
		stFileInfo.start = *(LPDWORD) &clmbtIndex[dwIndexPtr + 0];
		stFileInfo.end = ((i + 1) < dwFiles) ? *(DWORD*) &clmbtIndex[dwIndexPtr + 4] : pclArc->GetArcSize();
		stFileInfo.sizeCmp = stFileInfo.end - stFileInfo.start;
		stFileInfo.sizeOrg = stFileInfo.sizeCmp;

		if( stFileInfo.sizeCmp != 10 )
		{
			// �_�~�[�t�@�C���ł͂Ȃ�

			pclArc->AddFileInfo( stFileInfo );
		}

		dwIndexPtr += 4;
	}

	return	TRUE;
}

//////////////////////////////////////////////////////////////////////////////////////////
//	Him5�̃}�E���g

BOOL	CHimauri::MountHim5(
	CArcFile*			pclArc							// �A�[�J�C�u
	)
{
	if( memcmp( pclArc->GetHed(), "Him5", 4 ) != 0 )
	{
		return	FALSE;
	}

	// �t�@�C�����擾

	DWORD				dwFiles;

	pclArc->SeekHed( 4 );
	pclArc->Read( &dwFiles, 4 );

	// �t�@�C��������C���f�b�N�X�T�C�Y�擾

	DWORD				dwIndexSize = dwFiles * 8;

	// �C���f�b�N�X�擾

	YCMemory<BYTE>		clmbtIndex( dwIndexSize );
	DWORD				dwIndexPtr = 0;

	pclArc->Read( &clmbtIndex[0], dwIndexSize );

	// �C���f�b�N�X����C���f�b�N�X2�̃T�C�Y�擾

	DWORD				dwIndexSize2 = *(DWORD*) &clmbtIndex[dwFiles * 8 - 4] - *(DWORD*) &clmbtIndex[4] + *(DWORD*) &clmbtIndex[dwFiles * 8 - 8];

	// �C���f�b�N�X2�擾

	YCMemory<BYTE>		clmbtIndex2( dwIndexSize2 );
	DWORD				dwIndexPtr2 = 0;

	pclArc->Read( &clmbtIndex2[0], dwIndexSize2 );

	// �_�~�[����

	for( DWORD i = dwFiles ; i > 0 ; i-- )
	{
		if( *(DWORD*) &clmbtIndex[dwIndexPtr + 0] == 0 )
		{
			dwFiles--;
		}

		dwIndexPtr += 8;
	}

	// ���������̗L�����̎擾
	// �Ă߂�p�̏���

	BOOL				bDiffCompose = (pclArc->GetArcName() == _T("natsucha.hxp"));

	// �t�@�C�����̎擾

	std::vector<SFileInfo>	vcFileInfo;
	std::vector<SFileInfo>	vcDiffFileInfo;

	for( DWORD i = 0 ; i < dwFiles ; i++ )
	{
		while( 1 )
		{
			// 1�Z�O�����g�̒����擾

			DWORD				dwSegLength = clmbtIndex2[dwIndexPtr2 + 0];

			// �C���f�b�N�X2����t�@�C�����擾

			DWORD				dwFileNameLength = (dwSegLength - 5);

			char				szFileName[_MAX_FNAME];

			memcpy( szFileName, &clmbtIndex2[dwIndexPtr2 + 5], dwFileNameLength );

			SFileInfo			stFileInfo;

			stFileInfo.name = szFileName;
			stFileInfo.start = pclArc->ConvEndian( *(DWORD*) &clmbtIndex2[dwIndexPtr2 + 1] );

			// �I���A�h���X�擾

			if( clmbtIndex2[dwIndexPtr2 + dwSegLength] == 0 )
			{
				if( (i + 1) == dwFiles )
				{
					stFileInfo.end = pclArc->GetArcSize();
				}
				else
				{
					stFileInfo.end = pclArc->ConvEndian( *(DWORD*) &clmbtIndex2[dwIndexPtr2 + dwSegLength + 2] );
				}
			}
			else
			{
				stFileInfo.end = pclArc->ConvEndian( *(DWORD*) &clmbtIndex2[dwIndexPtr2 + dwSegLength + 1] );
			}

			stFileInfo.sizeCmp = stFileInfo.end - stFileInfo.start;
			stFileInfo.sizeOrg = stFileInfo.sizeCmp;

			if( stFileInfo.sizeCmp != 10 )
			{
				// �_�~�[�t�@�C���ł͂Ȃ�

				if( bDiffCompose )
				{
					// �����������L��

					char*				pszDiffMark = strrchr( szFileName, _T('_') );

					if( (pszDiffMark != NULL) && (strlen( pszDiffMark ) >= 2) && isdigit( pszDiffMark[1] ) )
					{
						// �����t�@�C��

						int					nPos = stFileInfo.name.ReverseFind( _T('_') );

						stFileInfo.name.Delete( (nPos + 1), stFileInfo.name.GetLength() );

						vcDiffFileInfo.push_back( stFileInfo );
					}
					else
					{
						vcFileInfo.push_back( stFileInfo );
					}
				}
				else
				{
					pclArc->AddFileInfo( stFileInfo );
				}
			}

			// 1�Z�O�����g�̍Ōオ0�Ȃ�1�t�@�C���I���Ɣ��f

			if( clmbtIndex2[dwIndexPtr2 + dwSegLength] == 0 )
			{
				dwIndexPtr2 += dwSegLength + 1;
				break;
			}

			dwIndexPtr2 += dwSegLength;
		}
	}

	if( bDiffCompose )
	{
		// �����������L��

		for( size_t i = 0 ; i < vcFileInfo.size() ; i++ )
		{
			int					nPos = vcFileInfo[i].name.ReverseFind( _T('_') );

			if( nPos >= 0 )
			{
				YCString			clsBaseFileName = vcFileInfo[i].name.Left( (nPos + 1) );

				for( size_t j = 0 ; j < vcDiffFileInfo.size() ; j++ )
				{
					if( clsBaseFileName == vcDiffFileInfo[j].name )
					{
						// �����摜�ł���

						vcFileInfo[i].starts.push_back( vcDiffFileInfo[j].start );
						vcFileInfo[i].sizesCmp.push_back( vcDiffFileInfo[j].sizeCmp );
						vcFileInfo[i].sizesOrg.push_back( vcDiffFileInfo[j].sizeOrg );
					}
				}
			}

			pclArc->AddFileInfo( vcFileInfo[i] );
		}
	}

	return	TRUE;
}

//////////////////////////////////////////////////////////////////////////////////////////
//	�f�R�[�h

BOOL	CHimauri::Decode(
	CArcFile*			pclArc							// �A�[�J�C�u
	)
{
	if( pclArc->GetArcExten() != _T(".hxp") )
	{
		return	FALSE;
	}

	if( memcmp( pclArc->GetHed(), "Him", 3 ) != 0 )
	{
		return	FALSE;
	}

	// ���̓T�C�Y�擾

	DWORD				dwSrcSize;

	pclArc->Read( &dwSrcSize, 4 );

	// �o�̓T�C�Y�擾

	DWORD				dwDstSize;

	pclArc->Read( &dwDstSize, 4 );

	// �o�b�t�@�m��

	YCMemory<BYTE>		clmbtSrc( dwSrcSize );
	YCMemory<BYTE>		clmbtDst( dwDstSize );

	if( dwSrcSize == 0 )
	{
		// �����k�t�@�C��

		pclArc->Read( &clmbtDst[0], dwDstSize );
	}
	else
	{
		// ���k�t�@�C��

		pclArc->Read( &clmbtSrc[0], dwSrcSize );
		Decomp( &clmbtDst[0], dwDstSize, &clmbtSrc[0], dwSrcSize );
	}

	YCString				sFileExt;

	if( memcmp( &clmbtDst[0], "OggS", 4 ) == 0 )
	{
		// Ogg Vorbis

		sFileExt = _T(".ogg");
	}
	else if( memcmp( &clmbtDst[0], "RIFF", 4 ) == 0 )
	{
		// WAVE

		sFileExt = _T(".wav");
	}
	else if( memcmp( &clmbtDst[0], "Himauri", 7 ) == 0 )
	{
		// hst

		sFileExt = _T(".txt");
	}
	else if( memcmp( &clmbtDst[0], "BM", 2 ) == 0 )
	{
		// BMP

		CImage				clImage;

		clImage.Init( pclArc, &clmbtDst[0] );
		clImage.Write( dwDstSize );

		return	TRUE;
	}
	else if( (clmbtDst[0] == 0) && (clmbtDst[1] <= 1) && (clmbtDst[2] > 0) && (memcmp( &clmbtDst[3], "\x00\x00\x00\x00\x00", 5 ) == 0) )
	{
		// TGA

		SFileInfo*			pstFileInfo = pclArc->GetOpenFileInfo();

		if( pstFileInfo->starts.empty() )
		{
			// ���������݂��Ȃ�

			CTga				clTga;

			clTga.Decode( pclArc, &clmbtDst[0], dwDstSize );
		}
		else
		{
			// ���������݂���

			// �x�[�X�摜��TGA�w�b�_�̎擾

			CTga::STGAHeader*	psttgahBase = (CTga::STGAHeader*) &clmbtDst[0];

			// TGA��

			DWORD				dwDstSize2 = ((psttgahBase->wWidth * (psttgahBase->btDepth >> 3) + 3) & 0xFFFFFFFC) * psttgahBase->wHeight;

			YCMemory<BYTE>		clmbtDst2( dwDstSize2 );

			CTga				clTga;

			clTga.Decomp( &clmbtDst2[0], dwDstSize2, &clmbtDst[0], dwDstSize );

			// �����������ďo��

			for( size_t i = 0 ; i < pstFileInfo->starts.size() ; i++ )
			{
				pclArc->SeekHed( pstFileInfo->starts[i] );

				// �����摜�̓��̓T�C�Y�擾

				DWORD				dwSrcSizeForDiff;

				pclArc->Read( &dwSrcSizeForDiff, 4 );

				// �����摜�̏o�̓T�C�Y�擾

				DWORD				dwDstSizeForDiff;

				pclArc->Read( &dwDstSizeForDiff, 4 );

				// �����摜�̃o�b�t�@�m��

				YCMemory<BYTE>		clmbtSrcForDiff( dwSrcSizeForDiff );
				YCMemory<BYTE>		clmbtDstForDiff( dwDstSizeForDiff );

				// �����摜�̎擾

				if( dwSrcSizeForDiff == 0 )
				{
					// �����k�t�@�C��

					pclArc->Read( &clmbtDstForDiff[0], dwDstSizeForDiff );
				}
				else
				{
					// ���k�t�@�C��

					pclArc->Read( &clmbtSrcForDiff[0], dwSrcSizeForDiff );
					Decomp( &clmbtDstForDiff[0], dwDstSizeForDiff, &clmbtSrcForDiff[0], dwSrcSizeForDiff );
				}

				// �����摜��TGA�w�b�_�̎擾

				CTga::STGAHeader*	psttgahDiff = (CTga::STGAHeader*) &clmbtDstForDiff[0];

				// TGA��

				DWORD				dwDstSizeForDiff2 = ((psttgahDiff->wWidth * (psttgahDiff->btDepth >> 3) + 3) & 0xFFFFFFFC) * psttgahDiff->wHeight;

				YCMemory<BYTE>		clmbtDstForDiff2( dwDstSizeForDiff );

				clTga.Decomp( &clmbtDstForDiff2[0], dwDstSizeForDiff, &clmbtDstForDiff[0], dwDstSizeForDiff );

				// ��������

				DWORD				dwDstSizeForCompose = dwDstSize2;

				YCMemory<BYTE>		clmbtDstForCompose( dwDstSizeForCompose );

				Compose( &clmbtDstForCompose[0], dwDstSizeForCompose, &clmbtDst2[0], dwDstSize2, &clmbtDstForDiff2[0], dwDstSizeForDiff2 );

				// �t�@�C�����̖����ύX

				TCHAR				szLastName[_MAX_FNAME];

				_stprintf( szLastName, _T("_%03d.bmp"), i );

				// �v���O���X�o�[�i���v��

				BOOL				bProgress = (i == 0);

				// �o��

				CImage				clImage;

				clImage.Init( pclArc, psttgahBase->wWidth, psttgahBase->wHeight, psttgahBase->btDepth, NULL, 0, szLastName );
				clImage.Write( &clmbtDstForCompose[0], dwDstSizeForCompose, bProgress );
				clImage.Close();
			}
		}

		return	TRUE;
	}
	else if( memcmp( &clmbtDst[0], "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00", 16 ) == 0 )
	{
		// ����Ƀ}�X�N�摜�ƒf��

		CImage				clImage;

		clImage.Init( pclArc, 800, 600, 8 );
		clImage.WriteReverse( &clmbtDst[0], dwDstSize );

		return	TRUE;
	}
	else
	{
		// ���̑�

		sFileExt = _T(".txt");
	}

	pclArc->OpenFile( sFileExt );
	pclArc->WriteFile( &clmbtDst[0], dwDstSize, pclArc->GetOpenFileInfo()->sizeCmp );

	return	TRUE;
}

//////////////////////////////////////////////////////////////////////////////////////////
//	��

void	CHimauri::Decomp(
	BYTE*				pbtDst,							// �i�[��
	DWORD				dwDstSize,						// �i�[��T�C�Y
	const BYTE*			pbtSrc,							// ���k�f�[�^
	DWORD				dwSrcSize						// ���k�f�[�^�T�C�Y
	)
{
	DWORD				dwSrcPtr = 0;
	DWORD				dwDstPtr = 0;
	DWORD				dwCode = 0;
	DWORD				dwBack = 0;

	while( (dwSrcPtr < dwSrcSize) && (dwDstPtr < dwDstSize) )
	{
		if( dwCode == 0 )
		{
			dwCode = pbtSrc[dwSrcPtr++];

			if( dwCode < 0x20 )
			{
				dwBack = 0;

				if( dwCode < 0x1D )
				{
					dwCode++;
				}
				else if( dwCode == 0x1D )
				{
					dwCode = pbtSrc[dwSrcPtr++] + 0x1E;
				}
				else if( dwCode == 0x1E )
				{
					dwCode = pbtSrc[dwSrcPtr++];
					dwCode <<= 8;
					dwCode |= pbtSrc[dwSrcPtr++];
					dwCode += 0x11E;
				}
				else if (dwCode == 0x1F)
				{
					dwCode = pbtSrc[dwSrcPtr++];
					dwCode <<= 8;
					dwCode |= pbtSrc[dwSrcPtr++];
					dwCode <<= 8;
					dwCode |= pbtSrc[dwSrcPtr++];
					dwCode <<= 8;
					dwCode |= pbtSrc[dwSrcPtr++];
				}
			}
			else
			{
				if( dwCode >= 0x80 )
				{
					dwBack = ((dwCode & 0x1F) << 8) | pbtSrc[dwSrcPtr++];
					dwCode = (dwCode >> 5) & 3;
				}
				else
				{
					DWORD				dwCode2 = (dwCode & 0x60);

					if( dwCode2 == 0x20 )
					{
						dwBack = (dwCode >> 2) & 7;
						dwCode &= 3;
					}
					else
					{
						dwCode &= 0x1F;

						if( dwCode2 == 0x40 )
						{
							dwBack = pbtSrc[dwSrcPtr++];
							dwCode += 4;
						}
						else
						{
							dwBack = (dwCode << 8) | pbtSrc[dwSrcPtr++];
							dwCode = pbtSrc[dwSrcPtr++];

							if( dwCode == 0xFE )
							{
								dwCode = pbtSrc[dwSrcPtr++];
								dwCode <<= 8;
								dwCode |= pbtSrc[dwSrcPtr++];
								dwCode += 0x102;
							}
							else if( dwCode == 0xFF )
							{
								dwCode = pbtSrc[dwSrcPtr++];
								dwCode <<= 8;
								dwCode |= pbtSrc[dwSrcPtr++];
								dwCode <<= 8;
								dwCode |= pbtSrc[dwSrcPtr++];
								dwCode <<= 8;
								dwCode |= pbtSrc[dwSrcPtr++];
							}
							else
							{
								dwCode += 4;
							}
						}
					}
				}

				dwBack++;
				dwCode += 3;
			}
		}

		// ����o�͂��钷���擾

		DWORD				dwLength = dwCode;

		if( (dwDstPtr + dwLength) > dwDstSize )
		{
			dwLength = (dwDstSize - dwDstPtr);
		}

		dwCode -= dwLength;

		// �o��

		if( dwBack > 0 )
		{
			// �O�ɏo�͂����f�[�^���o��

			for( DWORD i = 0 ; i < dwLength ; i++ )
			{
				pbtDst[dwDstPtr + i] = pbtDst[dwDstPtr - dwBack + i];
			}

			dwDstPtr += dwLength;
		}
		else
		{
			// ���̓f�[�^���o��

			memcpy( &pbtDst[dwDstPtr], &pbtSrc[dwSrcPtr], dwLength );

			dwSrcPtr += dwLength;
			dwDstPtr += dwLength;
		}
	}
}

//////////////////////////////////////////////////////////////////////////////////////////
//	��������

BOOL	CHimauri::Compose(
	BYTE*				pbtDst,							// �i�[��
	DWORD				dwDstSize,						// �i�[��T�C�Y
	const BYTE*			pbtBase,						// �x�[�X�摜
	DWORD				dwBaseSize,						// �x�[�X�摜�T�C�Y
	const BYTE*			pbtDiff,						// �����摜
	DWORD				dwDiffSize						// �����摜�T�C�Y
	)
{
	// �x�[�X�摜�ƍ����摜������

	memcpy( pbtDst, pbtBase, dwDstSize );

	for( DWORD i = 0 ; i < dwDstSize ; i += 4 )
	{
		// 32bit -> 24bit

		if( pbtDiff[i + 3] > 0 )
		{
			for( int j = 0 ; j < 3 ; j++ )
			{
				pbtDst[i + j] = (pbtDiff[i + j] - pbtBase[i + j]) * pbtDiff[i + 3] / 255 + pbtBase[i + j];
			}

			pbtDst[i + 3] = 0xFF;
		}
	}

	return	TRUE;
}
