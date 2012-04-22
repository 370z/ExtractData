
#include	"stdafx.h"
#include	"YCStdioFile.h"

//////////////////////////////////////////////////////////////////////////////////////////
//	�R���X�g���N�^

YCStdioFile::YCStdioFile()
{
	m_pStream = NULL;
}

//////////////////////////////////////////////////////////////////////////////////////////
//	�f�X�g���N�^

YCStdioFile::~YCStdioFile()
{
	Close();
}

//////////////////////////////////////////////////////////////////////////////////////////
//	���[�h���w�肵�ĊJ��

BOOL	YCStdioFile::Open(
	LPCTSTR				pszPathToFile,					// �t�@�C���p�X
	UINT				uOpenFlags						// ���[�h
	)
{
	Close();

	if( lstrlen( pszPathToFile ) >= MAX_PATH )
	{
		// �p�X����������

		return	FALSE;
	}

	// �A�N�Z�X���@

	YCString			clsMode;

	if( uOpenFlags & typeBinary )
	{
		// �o�C�i��

		if( uOpenFlags & modeRead )
		{
			clsMode = _T("rb");
		}

		if( uOpenFlags & modeCreate )
		{
			if( uOpenFlags & modeNoTruncate )
			{
				if( uOpenFlags & modeReadWrite )
				{
					clsMode = _T("rb+");
				}
				else if( uOpenFlags & modeWrite )
				{
					clsMode = _T("ab");
				}
			}
			else
			{
				if( uOpenFlags & modeReadWrite )
				{
					clsMode = _T("wb+");
				}
				else if( uOpenFlags & modeWrite )
				{
					clsMode = _T("wb");
				}
			}
		}
		else
		{
			if( uOpenFlags & modeReadWrite )
			{
				clsMode = _T("ab+");
			}
			else if( uOpenFlags & modeWrite )
			{
				clsMode = _T("ab");
			}
		}
	}
	else
	{
		// �e�L�X�g

		if( uOpenFlags & modeRead )
		{
			clsMode = _T("r");
		}

		if( uOpenFlags & modeCreate )
		{
			if( uOpenFlags & modeNoTruncate )
			{
				if( uOpenFlags & modeReadWrite )
				{
					clsMode = _T("r+");
				}
				else if( uOpenFlags & modeWrite )
				{
					clsMode = _T("a");
				}
			}
			else
			{
				if( uOpenFlags & modeReadWrite )
				{
					clsMode = _T("w+");
				}
				else if( uOpenFlags & modeWrite )
				{
					clsMode = _T("w");
				}
			}
		}
		else
		{
			if( uOpenFlags & modeReadWrite )
			{
				clsMode = _T("a+");
			}
			else if( uOpenFlags & modeWrite )
			{
				clsMode = _T("a");
			}
		}
	}

	// �t�@�C�����J��

	m_pStream = _tfopen( pszPathToFile, clsMode );

	m_clsPathToFile = pszPathToFile;
	m_clsFileName = m_clsPathToFile.GetFileName();
	m_clsFileExt = m_clsPathToFile.GetFileExt();

	return	(m_pStream != NULL);
}

//////////////////////////////////////////////////////////////////////////////////////////
//	�t�@�C�������

void	YCStdioFile::Close()
{
	if( m_pStream != NULL )
	{
		fclose( m_pStream );
		m_pStream = NULL;
	}
}

//////////////////////////////////////////////////////////////////////////////////////////
//	�t�@�C����ǂݍ���

DWORD	YCStdioFile::Read(
	void*				pvBuffer,						// �o�b�t�@
	DWORD				dwReadSize						// �ǂݍ��ރT�C�Y
	)
{
	DWORD				dwResult;

	dwResult = fread( pvBuffer, 1, dwReadSize, m_pStream );

	return	dwResult;
}

//////////////////////////////////////////////////////////////////////////////////////////
//	�t�@�C���ɏ�������

DWORD	YCStdioFile::Write(
	const void*			pvBuffer,						// �o�b�t�@
	DWORD				dwWriteSize						// �������ރT�C�Y
	)
{
	DWORD				dwResult;

	dwResult = fwrite( pvBuffer, 1, dwWriteSize, m_pStream );

	return	dwResult;
}

//////////////////////////////////////////////////////////////////////////////////////////
//	�t�@�C����1�s�ǂݍ���

LPTSTR	YCStdioFile::ReadString(
	LPTSTR				pszBuffer,						// �o�b�t�@
	DWORD				dwBufferSize					// �o�b�t�@�T�C�Y
	)
{
	return	_fgetts( pszBuffer, dwBufferSize, m_pStream );
}

//////////////////////////////////////////////////////////////////////////////////////////
//	�t�@�C����1�s�ǂݍ���
//
//	���l	�I�[�̉��s����菜��

BOOL	YCStdioFile::ReadString(
	YCString&			rfclsBuffer						// �o�b�t�@
	)
{
	BOOL				bReturn = FALSE;
	TCHAR				szBuffer[1024];

	rfclsBuffer = _T("");

	while( 1 )
	{
		if( ReadString( szBuffer, _countof( szBuffer ) ) == NULL )
		{
			// �Ō�܂œǂݍ���

			break;
		}

		rfclsBuffer += szBuffer;

		bReturn = TRUE;

		if( szBuffer[lstrlen( szBuffer ) - 1] == _T('\n') )
		{
			// ���s�����܂œǂݍ���

			break;
		}
	}

	return	bReturn;
}

//////////////////////////////////////////////////////////////////////////////////////////
//	�t�@�C����1�s��������

void	YCStdioFile::WriteString(
	LPCTSTR				pszBuffer						// �o�b�t�@
	)
{
	_fputts( pszBuffer, m_pStream );
}

//////////////////////////////////////////////////////////////////////////////////////////
//	�t�@�C���|�C���^���ړ�����

UINT64	YCStdioFile::Seek(
	INT64				n64Offset,						// �ړ�����o�C�g��
	DWORD				dwSeekMode						// �ړ����[�h
	)
{
	switch( dwSeekMode )
	{
	case	begin:
		dwSeekMode = SEEK_SET;
		break;

	case	current:
		dwSeekMode = SEEK_CUR;
		break;

	case	end:
		dwSeekMode = SEEK_END;
		break;

	default:
		dwSeekMode = SEEK_SET;
	}

	if( _fseeki64( m_pStream, n64Offset, dwSeekMode ) )
	{
		return	(UINT64) _ftelli64( m_pStream );
	}

	return	0;
}
