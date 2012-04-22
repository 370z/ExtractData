
#include	"stdafx.h"
#include	"../../Image.h"
#include	"../../Ogg.h"
#include	"TCD3.h"

//////////////////////////////////////////////////////////////////////////////////////////
//	�}�E���g

BOOL	CTCD3::Mount(
	CArcFile*			pclArc							// �A�[�J�C�u
	)
{
	if( pclArc->GetArcExten() != _T(".TCD") )
	{
		return	FALSE;
	}

	if( memcmp( pclArc->GetHed(), "TCD3", 4 ) != 0 )
	{
		return	FALSE;
	}

	pclArc->SeekHed( 4 );

	// �t�@�C�����̎擾

	DWORD				dwFiles;

	pclArc->Read( &dwFiles, 4 );

	// �C���f�b�N�X���̎擾

	STCD3IndexInfo		astTCD3IndexInfo[5];

	pclArc->Read( astTCD3IndexInfo, (sizeof(STCD3IndexInfo) * 5) );

	// �L�[�e�[�u���̍쐬

	static BYTE			abtKey[5] = {
		0xB7, 0x39, 0x24, 0x8D, 0x8D
	};

	// �g���q�e�[�u���̍쐬

	static YCString		aclsFileExt[5] =
	{
		_T(".tct"), _T(".tsf"), _T(".spd"), _T(".ogg"), _T(".wav")
	};

	// �C���f�b�N�X�̓ǂݍ���

	for( DWORD dwFileType = 0 ; dwFileType < 5 ; dwFileType++ )
	{
		if( astTCD3IndexInfo[dwFileType].dwFileSize == 0 )
		{
			// �C���f�b�N�X�����݂��Ȃ�

			continue;
		}

		// �C���f�b�N�X�ֈړ�

		pclArc->SeekHed( astTCD3IndexInfo[dwFileType].dwIndexOffset );

		// �t�H���_��

		DWORD				dwAllDirNameLength = (astTCD3IndexInfo[dwFileType].dwDirNameLength * astTCD3IndexInfo[dwFileType].dwDirCount);

		YCMemory<BYTE>		clmbtAllDirName( dwAllDirNameLength );

		pclArc->Read( &clmbtAllDirName[0], dwAllDirNameLength );

		// �t�H���_���̕���

		for( DWORD i = 0 ; i < dwAllDirNameLength ; i++ )
		{
			clmbtAllDirName[i] -= abtKey[dwFileType];
		}

		// �t�H���_���̎擾

		YCMemory<STCD3DirInfo>	clmstTCD3DirInfo( astTCD3IndexInfo[dwFileType].dwDirCount );

		pclArc->Read( &clmstTCD3DirInfo[0], (sizeof(STCD3DirInfo) * astTCD3IndexInfo[dwFileType].dwDirCount) );

		// �t�@�C����

		DWORD				dwAllFileNameLength = (astTCD3IndexInfo[dwFileType].dwFileNameLength * astTCD3IndexInfo[dwFileType].dwFileCount);

		YCMemory<BYTE>		clmbtAllFileName( dwAllFileNameLength );

		pclArc->Read( &clmbtAllFileName[0], dwAllFileNameLength );

		// �t�@�C�����̕���

		for( DWORD i = 0 ; i < dwAllFileNameLength ; i++ )
		{
			clmbtAllFileName[i] -= abtKey[dwFileType];
		}

		// �t�@�C���ւ̃I�t�Z�b�g

		DWORD				dwAllFileOffsetLength = (astTCD3IndexInfo[dwFileType].dwFileCount + 1);

		YCMemory<DWORD>		clmdwAllFileOffset( dwAllFileOffsetLength );

		pclArc->Read( &clmdwAllFileOffset[0], (sizeof(DWORD) * dwAllFileOffsetLength) );

		// ���̊i�[

		for( DWORD dwDir = 0 ; dwDir < astTCD3IndexInfo[dwFileType].dwDirCount ; dwDir++ )
		{
			// �t�H���_���̎擾

			TCHAR				szDirName[_MAX_DIR];

			memcpy( szDirName, &clmbtAllDirName[astTCD3IndexInfo[dwFileType].dwDirNameLength * dwDir], astTCD3IndexInfo[dwFileType].dwDirNameLength );

			for( DWORD dwFile = 0 ; dwFile < clmstTCD3DirInfo[dwDir].dwFileCount ; dwFile++ )
			{
				// �t�@�C�����̎擾

				TCHAR				szFileName[_MAX_FNAME];

				memcpy( szFileName, &clmbtAllFileName[clmstTCD3DirInfo[dwDir].dwFileNameOffset + astTCD3IndexInfo[dwFileType].dwFileNameLength * dwFile], astTCD3IndexInfo[dwFileType].dwFileNameLength );
				szFileName[astTCD3IndexInfo[dwFileType].dwFileNameLength] = _T('\0');

				// �t�H���_�� + �t�@�C���� + �g���q

				TCHAR				szFullName[_MAX_PATH];

				_stprintf( szFullName, _T("%s\\%s%s"), szDirName, szFileName, aclsFileExt[dwFileType] );

				// ���̊i�[

				SFileInfo			stFileInfo;

				stFileInfo.name = szFullName;
				stFileInfo.start = clmdwAllFileOffset[clmstTCD3DirInfo[dwDir].dwFileOffset + dwFile + 0];
				stFileInfo.end = clmdwAllFileOffset[clmstTCD3DirInfo[dwDir].dwFileOffset + dwFile + 1];
				stFileInfo.sizeCmp = stFileInfo.end - stFileInfo.start;
				stFileInfo.sizeOrg = stFileInfo.sizeCmp;

				pclArc->AddFileInfo( stFileInfo );
			}
		}
	}

	return	TRUE;
}

//////////////////////////////////////////////////////////////////////////////////////////
//	RLE�̉�(�^�C�v2)

BOOL	CTCD3::DecompRLE2(
	void*				pvDst,							// �i�[��
	DWORD				dwDstSize,						// �i�[�T�C�Y
	const void*			pvSrc,							// ���̓f�[�^
	DWORD				dwSrcSize						// ���̓f�[�^�T�C�Y
	)
{
	const BYTE*			pbtSrc = (const BYTE*) pvSrc;
	BYTE*				pbtDst = (BYTE*) pvDst;

	DWORD				dwOffset = *(DWORD*) &pbtSrc[0];
	DWORD				dwPixelCount = *(DWORD*) &pbtSrc[4];

	DWORD				dwSrcHeaderPtr = 8;
	DWORD				dwSrcDataPtr = dwOffset;
	DWORD				dwDstPtr = 0;

	while( (dwSrcHeaderPtr < dwOffset) && (dwSrcDataPtr < dwSrcSize) && (dwDstPtr < dwDstSize) )
	{
		WORD				wWork = *(WORD*)&pbtSrc[dwSrcHeaderPtr];

		dwSrcHeaderPtr += 2;

		WORD				wType = wWork >> 14;
		WORD				wLength = wWork & 0x3FFF;

		if( (dwDstPtr + (wLength * 4)) > dwDstSize )
		{
			MessageBox( NULL, _T("RLE2�̉𓀂ɕK�v�ȏo�̓o�b�t�@������܂���B���������܂��B"), _T("�G���["), 0 );
		}

		switch( wType )
		{
		case	0:
			// 0�Ŗ��߂�

			for( DWORD i = 0 ; i < wLength ; i++ )
			{
				for( DWORD j = 0 ; j < 4 ; j++ )
				{
					pbtDst[dwDstPtr++] = 0x00;
				}
			}

			break;

		case	1:
			// �A���t�@�l0xFF

			if( (dwSrcDataPtr + (wLength * 3)) > dwSrcSize )
			{
				MessageBox( NULL, _T("RLE2�̉𓀂ɕK�v�ȓ��̓o�b�t�@������܂���B���������܂��B"), _T("�G���["), 0 );
			}

			for( DWORD i = 0 ; i < wLength ; i++ )
			{
				for( DWORD j = 0 ; j < 3 ; j++ )
				{
					pbtDst[dwDstPtr++] = pbtSrc[dwSrcDataPtr++];
				}

				pbtDst[dwDstPtr++] = 0xFF;
			}

			break;

		default:
			// �A���t�@�l���w�b�_����擾

			if( (dwSrcDataPtr + (wLength * 3)) > dwSrcSize )
			{
				MessageBox( NULL, _T("RLE2�̉𓀂ɕK�v�ȓ��̓o�b�t�@������܂���B���������܂��B"), _T("�G���["), 0 );
			}

			if( (dwSrcHeaderPtr + wLength) > dwOffset )
			{
				MessageBox( NULL, _T("RLE2�̉𓀂ɕK�v�ȓ��̓o�b�t�@������܂���B���������܂��B"), _T("�G���["), 0 );
			}

			for( DWORD i = 0 ; i < wLength ; i++ )
			{
				for( DWORD j = 0 ; j < 3 ; j++ )
				{
					pbtDst[dwDstPtr++] = pbtSrc[dwSrcDataPtr++];
				}

				pbtDst[dwDstPtr++] = pbtSrc[dwSrcHeaderPtr++];
			}

			break;
		}
	}

	return	TRUE;
}
