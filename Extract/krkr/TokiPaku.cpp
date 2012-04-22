
#include	"stdafx.h"
#include	"TokiPaku.h"

//////////////////////////////////////////////////////////////////////////////////////////
//	�����\������

BOOL	CTokiPaku::OnCheckDecrypt(
	CArcFile*			pclArc							// �A�[�J�C�u
	)
{
//	return	CheckTpm( "510BE09DF50DB143E90D3837D416FD0F" );
	return	CheckTpm( "A9D18BCE341E20D25DB4DBFAAE7FBF5B" );
}

//////////////////////////////////////////////////////////////////////////////////////////
//	���������̏�����

DWORD	CTokiPaku::OnInitDecrypt(
	CArcFile*			pclArc							// �A�[�J�C�u
	)
{
	SFileInfo*			pstFileInfo = pclArc->GetOpenFileInfo();
	LPCTSTR				pszFileExt = PathFindExtension( pstFileInfo->name );

	if( lstrcmp( pszFileExt, _T(".dll") ) == 0 )
	{
		// �������Ȃ��t�@�C��

		SetDecryptRequirement( FALSE );
		return	0;
	}

	// ��������T�C�Y

	if( (lstrcmp( pszFileExt, _T(".ks") ) != 0) && (lstrcmp( pszFileExt, _T(".tjs") ) != 0) && (lstrcmp( pszFileExt, _T(".asd") ) != 0) )
	{
		SetDecryptSize( 256 );
	}

	// �����L�[

	DWORD				dwDecryptKey = 0xFFFFFFFF;

	BYTE*				pbtKey = &(BYTE&) dwDecryptKey;

	for( int i = 0 ; i < lstrlen( pszFileExt ) ; i++ )
	{
		pbtKey[i] = ~pszFileExt[i];
	}

	return	dwDecryptKey;
}

//////////////////////////////////////////////////////////////////////////////////////////
//	��������

DWORD	CTokiPaku::OnDecrypt(
	BYTE*				pbtTarget,						// �����Ώۃf�[�^
	DWORD				dwTargetSize,					// �����T�C�Y
	DWORD				dwOffset,						// �����Ώۃf�[�^�̈ʒu
	DWORD				dwDecryptKey					// �����L�[
	)
{
	// ����

	for( DWORD i = 0 ; i < dwTargetSize ; i += 4 )
	{
		*(DWORD*) &pbtTarget[i] ^= dwDecryptKey;
	}

	return	dwTargetSize;
}
