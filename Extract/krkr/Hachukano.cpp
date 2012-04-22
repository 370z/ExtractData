
#include	"stdafx.h"
#include	"Hachukano.h"

//////////////////////////////////////////////////////////////////////////////////////////
//	�����\������

BOOL	CHachukano::OnCheckDecrypt(
	CArcFile*			pclArc							// �A�[�J�C�u
	)
{
	return	pclArc->CheckExe( _T("hachukano.exe") );
}

//////////////////////////////////////////////////////////////////////////////////////////
//	���������̏�����

DWORD	CHachukano::OnInitDecrypt(
	CArcFile*			pclArc							// �A�[�J�C�u
	)
{
	SFileInfo*			pstFileInfo = pclArc->GetOpenFileInfo();
	LPCTSTR				pszFileExt = PathFindExtension( pstFileInfo->name );

	if( (lstrcmp( pszFileExt, _T(".dll") ) == 0) || (pstFileInfo->name == _T("startup.tjs")) )
	{
		// �������Ȃ��t�@�C��

		SetDecryptRequirement( FALSE );
		return	0;
	}

	// ��������T�C�Y

	if( (lstrcmp( pszFileExt, _T(".ks") ) != 0) && (lstrcmp( pszFileExt, _T(".tjs") ) != 0) && (lstrcmp( pszFileExt, _T(".asd") ) != 0) )
	{
		SetDecryptSize( 512 );
	}

	// �����L�[

	m_dwChangeDecryptKey = 0;

	return	(pstFileInfo->key ^ 0x03020100);
}

//////////////////////////////////////////////////////////////////////////////////////////
//	��������

DWORD	CHachukano::OnDecrypt(
	BYTE*				pbtTarget,						// �����Ώۃf�[�^
	DWORD				dwTargetSize,					// �����T�C�Y
	DWORD				dwOffset,						// �����Ώۃf�[�^�̈ʒu
	DWORD				dwDecryptKey					// �����L�[
	)
{
	// ����

	for( DWORD i = 0 ; i < dwTargetSize ; i += 4 )
	{
		if( (i & 255) == 0 )
		{
			m_dwChangeDecryptKey = 0;
		}
		else
		{
			m_dwChangeDecryptKey += 0x04040404;
		}

		*(DWORD*) &pbtTarget[i] ^= dwDecryptKey ^ m_dwChangeDecryptKey;
	}

	return	dwTargetSize;
}
