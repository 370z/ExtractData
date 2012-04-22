
#include	"stdafx.h"
#include	"YCIni.h"

//////////////////////////////////////////////////////////////////////////////////////////
//	�R���X�g���N�^

YCIni::YCIni(
	LPCTSTR				pszPathToIni					// INI�t�@�C���p�X
	)
{
	// ���s�t�@�C���p�X�̎擾

	TCHAR				szPathToExecuteFolder[MAX_PATH];

	::GetModuleFileName( NULL, szPathToExecuteFolder, _countof( szPathToExecuteFolder ) );
	::PathRemoveFileSpec( szPathToExecuteFolder );

	// INI�t�@�C���p�X�̎擾

	m_clsPathToIni.Format( _T("%s\\%s"), szPathToExecuteFolder, pszPathToIni );

//	m_clsPathToIni = pszPathToIni;
}

//////////////////////////////////////////////////////////////////////////////////////////
//	�Z�N�V��������ݒ�

void	YCIni::SetSection(
	LPCTSTR				pszSection						// �Z�N�V������
	)
{
	m_clsSection = pszSection;
}

//////////////////////////////////////////////////////////////////////////////////////////
//	�Z�N�V��������ݒ�

void	YCIni::SetSection(
	UINT				uID								// �X�g�����OID
	)
{
	TCHAR				szSection[256];

	::LoadString( ::GetModuleHandle( NULL ), uID, szSection, _countof( szSection ) );

	SetSection( szSection );
}

//////////////////////////////////////////////////////////////////////////////////////////
//	�L�[����ݒ�

void	YCIni::SetKey(
	LPCTSTR				pszKey							// �L�[��
	)
{
	m_clsKey = pszKey;
}

//////////////////////////////////////////////////////////////////////////////////////////
//	��������擾

void	YCIni::ReadStr(
	LPTSTR				pszDst,							// ������̊i�[��
	DWORD				dwDstSize,						// �i�[�o�b�t�@�T�C�Y
	LPCTSTR				pszDefault						// �f�t�H���g�l
	)
{
	::GetPrivateProfileString( m_clsSection, m_clsKey, pszDefault, pszDst, dwDstSize, m_clsPathToIni );
}

//////////////////////////////////////////////////////////////////////////////////////////
//	��������擾

void	YCIni::ReadStr(
	YCString&			rfclsDst,						// ������̊i�[��
	const YCString&		rfclsDefault					// �f�t�H���g�l
	)
{
	TCHAR				szDst[1024];

	ReadStr( szDst, _countof( szDst ), rfclsDefault );

	rfclsDst = szDst;
}

//////////////////////////////////////////////////////////////////////////////////////////
//	�������ݒ�

void	YCIni::WriteStr(
	LPCTSTR				pszStr							// �ݒ�l
	)
{
	::WritePrivateProfileString( m_clsSection, m_clsKey, pszStr, m_clsPathToIni );
}

//////////////////////////////////////////////////////////////////////////////////////////
//	�Z�N�V�������폜

BOOL	YCIni::DeleteSection(
	LPCTSTR				pszSection						// �Z�N�V������
	)
{
	if( pszSection == NULL )
	{
		pszSection = m_clsSection;
	}

	return	::WritePrivateProfileString( pszSection, NULL, NULL, m_clsPathToIni );
}
