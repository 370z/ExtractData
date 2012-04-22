
#include	"stdafx.h"
#include	"YCLibrary.h"

//////////////////////////////////////////////////////////////////////////////////////////
//	�R���X�g���N�^

YCLibrary::YCLibrary()
{
	m_hModule = NULL;
}

//////////////////////////////////////////////////////////////////////////////////////////
//	�f�X�g���N�^

YCLibrary::~YCLibrary()
{
	Free();
}

//////////////////////////////////////////////////////////////////////////////////////////
//	�w�肳�ꂽ���W���[�������[�h

BOOL	YCLibrary::Load(
	LPCTSTR				pszPathToFile					// ���[�h���郂�W���[����
	)
{
	m_hModule = ::LoadLibrary( pszPathToFile );

	return	(m_hModule != NULL);
}

//////////////////////////////////////////////////////////////////////////////////////////
//	���[�h�������W���[�������

void	YCLibrary::Free()
{
	if( m_hModule != NULL )
	{
		::FreeLibrary( m_hModule );
		m_hModule = NULL;
	}
}

//////////////////////////////////////////////////////////////////////////////////////////
//	�֐��̃A�h���X���擾

FARPROC	YCLibrary::GetProcAddress(
	LPCTSTR				pszProcName						// �֐���
	)
{
	if( m_hModule == NULL )
	{
		return	NULL;
	}

	YCStringA			clsProcName = pszProcName;	// GetProcAddressW�����݂��Ȃ��̂Ŏd���Ȃ�

	return	::GetProcAddress( m_hModule, clsProcName );
}
