
#include	"stdafx.h"
#include	"SisMiko.h"

//////////////////////////////////////////////////////////////////////////////////////////
//	�����\������

BOOL	CSisMiko::OnCheckDecrypt(
	CArcFile*			pclArc							// �A�[�J�C�u
	)
{
	return	pclArc->CheckExe( _T("SisuMiko.exe") );
}

//////////////////////////////////////////////////////////////////////////////////////////
//	���������̏�����

DWORD	CSisMiko::OnInitDecrypt(
	CArcFile*			pclArc							// �A�[�J�C�u
	)
{
	SFileInfo*			pstFileInfo = pclArc->GetOpenFileInfo();
	LPCTSTR				pszFileExt = PathFindExtension( pstFileInfo->name );
	DWORD				dwKey = pclArc->GetOpenFileInfo()->key;

	if( (lstrcmp( pszFileExt, _T(".dll") ) == 0) || (pstFileInfo->name == _T("startup.tjs")) )
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

	return	~((dwKey << 16) | (dwKey >> 16));
}

//////////////////////////////////////////////////////////////////////////////////////////
//	��������

DWORD	CSisMiko::OnDecrypt(
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
