
#include	"stdafx.h"
#include	"../../ExtractBase.h"
#include	"../../Arc/Zlib.h"
#include	"../../Image.h"
#include	"../../Ogg.h"
#include	"../../FindFile.h"
#include	"Tlg.h"
#include	"Krkr.h"

//////////////////////////////////////////////////////////////////////////////////////////
//	�}�E���g

BOOL	CKrkr::Mount(
	CArcFile*			pclArc							// �A�[�J�C�u
	)
{
	DWORD				dwOffset;

	if( memcmp( pclArc->GetHed(), "XP3\r\n \n\x1A\x8B\x67\x01", 11 ) == 0 )
	{
		// XP3

		dwOffset = 0;
	}
	else if( memcmp( pclArc->GetHed(), "MZ", 2 ) == 0 )
	{
		// EXE�^

		if( !FindXP3FromExecuteFile( pclArc, &dwOffset ) )
		{
			return	FALSE;
		}
	}
	else
	{
		return	FALSE;
	}

	m_pclArc = pclArc;

	// tpm��MD5�l�̐ݒ�

	SetMD5ForTpm( pclArc );

	// �����\���`�F�b�N

	if( !OnCheckDecrypt( pclArc ) )
	{
		// ���̃A�[�J�C�u�͕����ł��Ȃ�

		return	FALSE;
	}

	// �C���f�b�N�X�̈ʒu�擾

	INT64				n64IndexPos;

	pclArc->SeekHed( 11 + dwOffset );
	pclArc->Read( &n64IndexPos, 8 );

	pclArc->SeekCur( n64IndexPos - 19 );

	BYTE				abtWork[256];

	pclArc->Read( abtWork, sizeof( abtWork ) );

	switch( abtWork[0] )
	{
	case	0x80:

		n64IndexPos = *(INT64*) &abtWork[9];
		break;
	}

	// �C���f�b�N�X�w�b�_�ǂݍ���

	BYTE				btCmpIndex;

	pclArc->SeekHed( n64IndexPos + dwOffset );
	pclArc->Read( &btCmpIndex, 1 );

	UINT64				u64CompIndexSize;
	UINT64				u64IndexSize;

	if( btCmpIndex )
	{
		// �C���f�b�N�X�����k����Ă���

		pclArc->Read( &u64CompIndexSize, 8 );
	}

	pclArc->Read( &u64IndexSize, 8 );

	// �o�b�t�@�m��

	YCMemory<BYTE>		clmbtIndex( u64IndexSize );
	DWORD				dwIndexPtr = 0;

	// �C���f�b�N�X�w�b�_�����k����Ă���ꍇ�A�𓀂���

	if( btCmpIndex )
	{
		// �C���f�b�N�X�����k����Ă���

		CZlib				clZlib;

		// �o�b�t�@�m��

		YCMemory<BYTE>		clmbtCompIndex( u64CompIndexSize );

		// zlib��

		pclArc->Read( &clmbtCompIndex[0], u64CompIndexSize );

		clZlib.Decompress( &clmbtIndex[0], u64IndexSize, &clmbtCompIndex[0], u64CompIndexSize );
	}
	else
	{
		// �����k�C���f�b�N�X

		pclArc->Read( &clmbtIndex[0], u64IndexSize );
	}

	// �C���f�b�N�X����t�@�C�����擾

	for( UINT64 i = 0 ; i < u64IndexSize ; )
	{
		// "File" �`�����N

		FileChunk			stFileChunk;

		memcpy( stFileChunk.name, &clmbtIndex[i], 4 );

		stFileChunk.size = *(UINT64*) &clmbtIndex[i + 4];

		if( memcmp(stFileChunk.name, "File", 4 ) != 0 )
		{
			break;
		}

		i += 12;

		// "info" �`�����N

		InfoChunk			stInfoChunk;

		memcpy( stInfoChunk.name, &clmbtIndex[i], 4 );

		stInfoChunk.size = *(UINT64*) &clmbtIndex[i + 4];
		stInfoChunk.protect = *(DWORD*) &clmbtIndex[i + 12];
		stInfoChunk.orgSize = *(UINT64*) &clmbtIndex[i + 16];
		stInfoChunk.arcSize = *(UINT64*) &clmbtIndex[i + 24];
		stInfoChunk.nameLen = *(WORD*) &clmbtIndex[i + 32];
		stInfoChunk.filename = (wchar_t*) &clmbtIndex[i + 34];

		if( memcmp( stInfoChunk.name, "info", 4 ) != 0 )
		{
			break;
		}

		i += 12 + stInfoChunk.size;

		// "segm" �`�����N

		SegmChunk			stSegmChunk;

		memcpy( stSegmChunk.name, &clmbtIndex[i], 4 );

		stSegmChunk.size = *(UINT64*) &clmbtIndex[i + 4];

		if( memcmp( stSegmChunk.name, "segm", 4 ) != 0 )
		{
			break;
		}

		i += 12;

		SFileInfo			stFileInfo;

		UINT64				u64SegmCount = (stSegmChunk.size / 28);

		for( UINT64 j = 0 ; j < u64SegmCount ; j++ )
		{
			stSegmChunk.comp = *(DWORD*) &clmbtIndex[i];
			stSegmChunk.start = *(UINT64*) &clmbtIndex[i + 4] + dwOffset;
			stSegmChunk.orgSize = *(UINT64*) &clmbtIndex[i + 12];
			stSegmChunk.arcSize = *(UINT64*) &clmbtIndex[i + 20];

			stFileInfo.bCmps.push_back( stSegmChunk.comp );
			stFileInfo.starts.push_back( stSegmChunk.start );
			stFileInfo.sizesOrg.push_back( stSegmChunk.orgSize );
			stFileInfo.sizesCmp.push_back( stSegmChunk.arcSize );

			i += 28;
		}

		// ���Ƀ`�����N���Ȃ����`�F�b�N

		UINT64				u64Remainder = stFileChunk.size - 12 - stInfoChunk.size - 12 - stSegmChunk.size;

		if( u64Remainder > 0 )
		{
			// "adlr" �`�����N

			if( memcmp( &clmbtIndex[i], "adlr", 4 ) == 0 )
			{
				AdlrChunk			stAdlrChunk;

				memcpy( stAdlrChunk.name, &clmbtIndex[i], 4 );

				stAdlrChunk.size = *(UINT64*) &clmbtIndex[i + 4];
				stAdlrChunk.key = *(DWORD*) &clmbtIndex[i + 12];

				stFileInfo.key = stAdlrChunk.key;
			}

			i += u64Remainder;
		}

		// ���X�g�r���[�\���\�̍\���̂Ɋi�[

		stFileInfo.name.Copy( stInfoChunk.filename, stInfoChunk.nameLen );
		stFileInfo.sizeOrg = stInfoChunk.orgSize;
		stFileInfo.sizeCmp = stInfoChunk.arcSize;
		stFileInfo.start = stFileInfo.starts[0];
		stFileInfo.end = stFileInfo.start + stFileInfo.sizeCmp;

		if( stSegmChunk.comp )
		{
			stFileInfo.format = _T("zlib");
		}

		pclArc->AddFileInfo( stFileInfo );
	}

	return	TRUE;
}

//////////////////////////////////////////////////////////////////////////////////////////
//	�f�R�[�h

BOOL	CKrkr::Decode(
	CArcFile*			pclArc							// �A�[�J�C�u
	)
{
	SFileInfo*			pstFileInfo = pclArc->GetOpenFileInfo();

	if( (pclArc->GetArcExten() != _T(".xp3")) && (pclArc->GetArcExten() != _T(".exe")) )
	{
		return	FALSE;
	}

	YCString			clsFileExt = PathFindExtension( pstFileInfo->name );

	clsFileExt.MakeLower();

	InitDecrypt( pclArc );

//char s[256];
//_stprintf(s, "%08X", pInfFile->key);
//MessageBox(NULL, s, "", 0);

	// �o�b�t�@�m��

	DWORD				dwBufferSize = pclArc->GetBufSize();

	YCMemory<BYTE>		clmbtBuffer;

	// �������Ɍ������邩�ǂ���

	BOOL				bComposeMemory = FALSE;

	if( (clsFileExt == _T(".tlg")) ||
		((clsFileExt == _T(".ogg")) && pclArc->GetOpt()->bFixOgg) ||
		(clsFileExt == _T(".bmp")) )
	{
		// TLG, OGG(CRC�C���L��), BMP

		clmbtBuffer.resize( pstFileInfo->sizeOrg + 3 );
		bComposeMemory = TRUE;
	}
	else if( (m_dwDecryptKey == 0) &&
		pclArc->GetOpt()->bEasyDecrypt && (
		(clsFileExt == _T(".tjs")) ||
		(clsFileExt == _T(".ks")) ||
		(clsFileExt == _T(".asd")) ||
		(clsFileExt == _T(".txt"))) )
	{
		// tjs, ks, asd, txt

		clmbtBuffer.resize( pstFileInfo->sizeOrg + 3 );
		bComposeMemory = TRUE;
	}
	else
	{
		// ���̑�

		clmbtBuffer.resize( dwBufferSize + 3 );
	}

	// �o�̓t�@�C������

	if( !bComposeMemory )
	{
		pclArc->OpenFile();
	}

	DWORD				dwBufferPtr = 0;
	DWORD				dwBufferSizeBase = dwBufferSize;
	DWORD				dwWroteSize = 0;

	for( size_t i = 0 ; i < pstFileInfo->starts.size() ; i++ )
	{
		dwBufferSize = dwBufferSizeBase;

		pclArc->SeekHed( pstFileInfo->starts[i] );

		if( pstFileInfo->bCmps[i] )
		{
			// ���k�f�[�^

			CZlib				clZlib;

			// �o�b�t�@�m��

			DWORD				dwSrcSize = pstFileInfo->sizesCmp[i];

			YCMemory<BYTE>		clmbtSrc( dwSrcSize );

			DWORD				dwDstSize = pstFileInfo->sizesOrg[i];

			YCMemory<BYTE>		clmbtDst( dwDstSize + 3 );

			// zlib��

			pclArc->Read( &clmbtSrc[0], dwSrcSize );

			clZlib.Decompress( &clmbtDst[0], dwDstSize, &clmbtSrc[0], dwSrcSize );

			DWORD				dwDataSize = Decrypt( &clmbtDst[0], dwDstSize, dwWroteSize );

			if( bComposeMemory )
			{
				// �o�b�t�@�Ɍ���

				memcpy( &clmbtBuffer[dwBufferPtr], &clmbtDst[0], dwDataSize );

				dwBufferPtr += dwDataSize;
			}
			else
			{
				// �o��

				pclArc->WriteFile( &clmbtDst[0], dwDataSize, dwDstSize );
			}

			dwWroteSize += dwDstSize;
		}
		else
		{
			// �����k�f�[�^

			if( bComposeMemory )
			{
				// �o�b�t�@�Ɍ���

				DWORD				dwDstSize = pstFileInfo->sizesOrg[i];

				pclArc->Read( &clmbtBuffer[dwBufferPtr], dwDstSize );

				DWORD				dwDataSize = Decrypt( &clmbtBuffer[dwBufferPtr], dwDstSize, dwWroteSize );

				dwBufferPtr += dwDataSize;
				dwWroteSize += dwDstSize;
			}
			else
			{
				DWORD				dwDstSize = pstFileInfo->sizesOrg[i];

				for( DWORD dwWroteSizes = 0 ; dwWroteSizes != dwDstSize ; dwWroteSizes += dwBufferSize )
				{
					// �o�b�t�@�T�C�Y����

					pclArc->SetBufSize( &dwBufferSize, dwWroteSizes, dwDstSize );

					pclArc->Read( &clmbtBuffer[0], dwBufferSize );

					DWORD				dwDataSize = Decrypt( &clmbtBuffer[0], dwBufferSize, dwWroteSize );

					pclArc->WriteFile( &clmbtBuffer[0], dwDataSize );

					dwWroteSize += dwBufferSize;
				}
			}
		}
	}

	if( clsFileExt == _T(".tlg") )
	{
		// tlg��bmp�ɕϊ�

		CTlg				clTLG;

		clTLG.Decode( pclArc, &clmbtBuffer[0] );
	}
	else if( clsFileExt == _T(".ogg") && pclArc->GetOpt()->bFixOgg )
	{
		// ogg��CRC���C��

		COgg				clOGG;

		clOGG.Decode( pclArc, &clmbtBuffer[0] );
	}
	else if( clsFileExt == _T(".bmp") )
	{
		// bmp�o��(png�ϊ�)

		CImage				clImage;

		clImage.Init( pclArc, &clmbtBuffer[0] );
		clImage.Write( pstFileInfo->sizeOrg );
	}
	else if( m_dwDecryptKey == 0 &&
		pclArc->GetOpt()->bEasyDecrypt && (
		(clsFileExt == _T(".tjs")) ||
		(clsFileExt == _T(".ks")) ||
		(clsFileExt == _T(".asd")) ||
		(clsFileExt == _T(".txt"))) )
	{
		// �e�L�X�g�t�@�C��

		DWORD				dwDstSize = pstFileInfo->sizeOrg;

		SetDecryptRequirement( TRUE );

		m_dwDecryptKey = pclArc->InitDecryptForText( &clmbtBuffer[0], dwDstSize );

		DWORD				dwDataSize = Decrypt( &clmbtBuffer[0], dwDstSize, 0 );

		pclArc->OpenFile();
		pclArc->WriteFile( &clmbtBuffer[0], dwDataSize, dwDstSize );
	}

	return	TRUE;
}

//////////////////////////////////////////////////////////////////////////////////////////
//	���o

BOOL	CKrkr::Extract(
	CArcFile*			pclArc							// �A�[�J�C�u
	)
{
	SFileInfo*			pstFileInfo = pclArc->GetOpenFileInfo();

	DWORD				dwBufferSize = pclArc->GetBufSize();
	DWORD				dwBufferSizeBase = dwBufferSize;

	YCMemory<BYTE>		clmbtBuffer( dwBufferSize );

	pclArc->OpenFile();

	for( size_t i = 0 ; i < pstFileInfo->starts.size() ; i++ )
	{
		dwBufferSize = dwBufferSizeBase;

		pclArc->SeekHed( pstFileInfo->starts[i] );

		DWORD				dwDstSize = pstFileInfo->sizesOrg[i];

		for( DWORD dwWroteSizes = 0 ; dwWroteSizes != dwDstSize ; dwWroteSizes += dwBufferSize )
		{
			// �o�b�t�@�T�C�Y����

			pclArc->SetBufSize( &dwBufferSize, dwWroteSizes, dwDstSize );

			pclArc->Read( &clmbtBuffer[0], dwBufferSize );
			pclArc->WriteFile( &clmbtBuffer[0], dwBufferSize );
		}
	}

	pclArc->CloseFile();

	return	TRUE;
}

//////////////////////////////////////////////////////////////////////////////////////////
//	�A�[�J�C�u�t�H���_���ɂ���tpm��MD5�l��ݒ�

void	CKrkr::SetMD5ForTpm(
	CArcFile*			pclArc							// �A�[�J�C�u
	)
{
	if( pclArc->CheckMD5OfSet() )
	{
		// ���łɐݒ�ς�

		return;
	}

	// �A�[�J�C�u�̃f�B���N�g���p�X�̎擾

	TCHAR				szBasePathToTpm[MAX_PATH];

	lstrcpy( szBasePathToTpm, pclArc->GetArcPath() );
	PathRemoveFileSpec( szBasePathToTpm );

	// tpm�̃t�@�C���p�X�̎擾

	CFindFile				clFindFile;

	std::vector<YCString>&	vtsPathToTpm = clFindFile.DoFind( szBasePathToTpm, _T("*.tpm") );

	// tpm��MD5�l�̐ݒ�

	CMD5				clmd5Tpm;

	for( size_t i = 0 ; i < vtsPathToTpm.size() ; i++)
	{
		pclArc->SetMD5( clmd5Tpm.Calculate( vtsPathToTpm[i] ) );
	}

	pclArc->SetMD5OfFlag( TRUE );
}

//////////////////////////////////////////////////////////////////////////////////////////
//	�����\������

BOOL	CKrkr::OnCheckDecrypt(
	CArcFile*			pclArc							// �A�[�J�C�u
	)
{
	return	TRUE;
}

//////////////////////////////////////////////////////////////////////////////////////////
//	�A�[�J�C�u�t�H���_���ɂ���tpm��MD5�ƈ�v���邩�̊m�F

BOOL	CKrkr::CheckTpm(
	const char*			pszMD5							// MD5
	)
{
	// ��r

	for( size_t i = 0 ; i < m_pclArc->GetMD5().size() ; i++ )
	{
		if( memcmp( pszMD5, m_pclArc->GetMD5()[i].szABCD, 32 ) == 0 )
		{
			// ��v����

			return	TRUE;
		}
	}

	return	FALSE;
}

//////////////////////////////////////////////////////////////////////////////////////////
//	���������̏�����

void	CKrkr::InitDecrypt(
	CArcFile*			pclArc							// �A�[�J�C�u
	)
{
	m_pclArc = pclArc;

	// �����v���̗L����

	SetDecryptRequirement( TRUE );

	// �����T�C�Y�̐ݒ�

	SetDecryptSize( 0 );

	// �I�[�o�[���C�h���ꂽ�����������֐����Ă�

	m_dwDecryptKey = OnInitDecrypt( pclArc );
}

//////////////////////////////////////////////////////////////////////////////////////////
//	�f�t�H���g�ŊȈՕ������g�p

DWORD	CKrkr::OnInitDecrypt(
	CArcFile*			pclArc							// �A�[�J�C�u
	)
{
	DWORD				dwDecryptKey = pclArc->InitDecrypt();

	if( dwDecryptKey == 0 )
	{
		// �Í�������Ă��Ȃ�

		SetDecryptRequirement( FALSE );
	}

	return	dwDecryptKey;
}

//////////////////////////////////////////////////////////////////////////////////////////
//	��������

DWORD	CKrkr::Decrypt(
	BYTE*				pbtTarget,						// �����Ώۃf�[�^
	DWORD				dwTargetSize,					// �����T�C�Y
	DWORD				dwOffset						// �����Ώۃf�[�^�̈ʒu
	)
{
	// �������Ȃ��t�@�C��

	if( !m_bDecrypt )
	{
		// �����v���Ȃ�

		return	dwTargetSize;
	}

	DWORD				dwDecryptSize = m_dwDecryptSize;

	if( dwDecryptSize == 0 )
	{
		// �����T�C�Y���ݒ肳��Ă��Ȃ�

		return	OnDecrypt( pbtTarget, dwTargetSize, dwOffset, m_dwDecryptKey );
	}
	else
	{
		// �����T�C�Y���ݒ肳��Ă���

		if( dwOffset >= dwDecryptSize )
		{
			// ����ȏ㕜�����Ȃ�

			return	dwTargetSize;
		}

		if( dwDecryptSize > dwTargetSize )
		{
			// ���߂�ꂽ�����T�C�Y���f�[�^�T�C�Y���傫��

			dwDecryptSize = dwTargetSize;
		}

		OnDecrypt( pbtTarget, dwDecryptSize, dwOffset, m_dwDecryptKey );

		return	dwTargetSize;
	}
}

//////////////////////////////////////////////////////////////////////////////////////////
//	�f�t�H���g�ŊȈՕ������g�p
//
//	���l	dwDecryptKey��OnInitDecrypt�̖߂�l

DWORD	CKrkr::OnDecrypt(
	BYTE*				pbtTarget,						// �����Ώۃf�[�^
	DWORD				dwTargetSize,					// �����T�C�Y
	DWORD				dwOffset,						// �����Ώۃf�[�^�̈ʒu
	DWORD				dwDecryptKey					// �����L�[
	)
{
	m_pclArc->Decrypt( pbtTarget, dwTargetSize );

	return	dwTargetSize;
}

//////////////////////////////////////////////////////////////////////////////////////////
//	�����v���̐ݒ�

void	CKrkr::SetDecryptRequirement(
	BOOL				bDecrypt						// �����v��
	)
{
	m_bDecrypt = bDecrypt;
}

//////////////////////////////////////////////////////////////////////////////////////////
//	�����T�C�Y�̐ݒ�

void	CKrkr::SetDecryptSize(
	DWORD				dwDecryptSize					// �����T�C�Y
	)
{
	m_dwDecryptSize = dwDecryptSize;
}

//////////////////////////////////////////////////////////////////////////////////////////
//	���s�t�@�C��������XP3�A�[�J�C�u�ւ̃I�t�Z�b�g���擾

BOOL	CKrkr::FindXP3FromExecuteFile(
	CArcFile*			pclArc,							// �A�[�J�C�u
	DWORD*				pdwOffset						// �i�[��
	)
{
	if( pclArc->GetArcSize() <= 0x200000 )
	{
		// �g���g���̎��s�t�@�C���ł͂Ȃ�

		return	FALSE;
	}

	*pdwOffset = 16;

	pclArc->SeekHed( 16 );

	BYTE				abtBuffer[4096];
	DWORD				dwReadSize;

	do
	{
		dwReadSize = pclArc->Read( abtBuffer, sizeof(abtBuffer) );

		for( DWORD i = 0, j = 0 ; i < (dwReadSize / 16) ; i++, j += 16 )
		{
			if( memcmp( &abtBuffer[j], "XP3\r\n \n\x1A\x8B\x67\x01", 11) == 0 )
			{
				// XP3�A�[�J�C�u����

				*pdwOffset += j;
				return	TRUE;
			}
		}

		*pdwOffset += dwReadSize;

		if( *pdwOffset >= 0x500000 )
		{
			// �����ł��؂�
			
			break;
		}

		// �L�����Z���m�F

		if( pclArc->GetProg()->OnCancel() )
		{
			throw	-1;
		}
	}
	while( dwReadSize == sizeof(abtBuffer) );

	// XP3�A�[�J�C�u�Ȃ�

	*pdwOffset = 0;

	pclArc->SeekHed();

	return	FALSE;
}
