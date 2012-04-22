
#include	"stdafx.h"
#include	"YCFileFind.h"

//////////////////////////////////////////////////////////////////////////////////////////
//	�R���X�g���N�^

YCFileFind::YCFileFind()
{
	m_hFind = INVALID_HANDLE_VALUE;
}

//////////////////////////////////////////////////////////////////////////////////////////
//	�f�X�g���N�^

YCFileFind::~YCFileFind()
{
	Close();
}

//////////////////////////////////////////////////////////////////////////////////////////
//	�t�@�C���������J�n

BOOL	YCFileFind::FindFirstFile(
	LPCTSTR				pszPathToFile					// �t�@�C���p�X
	)
{
	// �f�B���N�g���p�X�̕ۑ�

	TCHAR				szPathToFolder[_MAX_PATH];

	lstrcpy( szPathToFolder, pszPathToFile );
	PathRemoveFileSpec( szPathToFolder );

	m_clsPathToFolder = szPathToFolder;

	// �t�@�C�������̊J�n

	m_hFind = ::FindFirstFile( pszPathToFile, &m_stwfdFindData );

	return	(m_hFind != INVALID_HANDLE_VALUE);
}

//////////////////////////////////////////////////////////////////////////////////////////
//	�t�@�C���������J�n

BOOL	YCFileFind::FindFirstFile(
	LPCTSTR				pszPathToFolder,				// �f�B���N�g���p�X
	LPCTSTR				pszFileName						// �t�@�C����(���C���h�J�[�h��)
	)
{
	// �f�B���N�g���p�X�̕ۑ�

	TCHAR				szPathToFolder[_MAX_PATH];

	lstrcpy( szPathToFolder, pszPathToFolder );
	PathRemoveBackslash( szPathToFolder );	

	m_clsPathToFolder = szPathToFolder;

	// �t�@�C���p�X�̍쐬

	TCHAR				szPathToFile[_MAX_PATH];

	lstrcpy( szPathToFile, szPathToFolder );
	PathAppend( szPathToFile, pszFileName );

	// �t�@�C�������̊J�n

	m_hFind = ::FindFirstFile( szPathToFile, &m_stwfdFindData );

	return	(m_hFind != INVALID_HANDLE_VALUE);
}

//////////////////////////////////////////////////////////////////////////////////////////
//	�t�@�C�������𑱍s

BOOL	YCFileFind::FindNextFile()
{
	return	::FindNextFile( m_hFind, &m_stwfdFindData );
}

//////////////////////////////////////////////////////////////////////////////////////////
//	�t�@�C���������I��

void	YCFileFind::Close()
{
	if( m_hFind != INVALID_HANDLE_VALUE )
	{
		::FindClose( m_hFind );
		m_hFind = INVALID_HANDLE_VALUE;
	}
}

//////////////////////////////////////////////////////////////////////////////////////////
//	���������t�@�C�������擾

YCString	YCFileFind::GetFileName()
{
	return	m_stwfdFindData.cFileName;
}

//////////////////////////////////////////////////////////////////////////////////////////
//	���������t�@�C���p�X���擾

YCString	YCFileFind::GetFilePath()
{
	TCHAR				szPathToFile[_MAX_PATH];

	_stprintf( szPathToFile, _T("%s\\%s"), m_clsPathToFolder, m_stwfdFindData.cFileName );

	return	szPathToFile;
}

//////////////////////////////////////////////////////////////////////////////////////////
//	���������t�@�C���^�C�g�����擾

YCString	YCFileFind::GetFileTitle()
{
	TCHAR				szFileTitle[_MAX_FNAME];

	lstrcpy( szFileTitle, m_stwfdFindData.cFileName );
	PathRemoveExtension( szFileTitle );

	return	szFileTitle;
}

//////////////////////////////////////////////////////////////////////////////////////////
//	���������t�@�C�����f�B���N�g�����ǂ������m�F

BOOL	YCFileFind::IsDirectory()
{
	return	(m_stwfdFindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY);
}

//////////////////////////////////////////////////////////////////////////////////////////
//	���݂̃f�B���N�g���Ƃ��̐e�f�B���N�g���̃}�[�J�[�𒲂ׂ�

BOOL	YCFileFind::IsDots()
{
	return	((lstrcmp( m_stwfdFindData.cFileName, _T(".") ) == 0) || (lstrcmp( m_stwfdFindData.cFileName, _T("..") ) == 0));
}
