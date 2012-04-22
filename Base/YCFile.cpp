
#include	"stdafx.h"
#include	"YCFile.h"

//////////////////////////////////////////////////////////////////////////////////////////
//	�R���X�g���N�^

YCFile::YCFile()
{
	m_hFile = INVALID_HANDLE_VALUE;
}

//////////////////////////////////////////////////////////////////////////////////////////
//	�f�X�g���N�^

YCFile::~YCFile()
{
	Close();
}

//////////////////////////////////////////////////////////////////////////////////////////
//	���[�h���w�肵�ĊJ��

BOOL	YCFile::Open(
	LPCTSTR				pszPathToFile,					// �t�@�C���p�X
	UINT				uOpenFlags						// ���[�h
	)
{
	Close();

	if( lstrlen( pszPathToFile ) > MAX_PATH )
	{
		// �p�X����������

		return	FALSE;
	}

	// �A�N�Z�X���@

	DWORD				dwAccess;
	DWORD				dwCreateDisposition;

	if( uOpenFlags & modeRead )
	{
		dwAccess = GENERIC_READ;
		dwCreateDisposition = OPEN_EXISTING;
	}
	else if( uOpenFlags & modeReadWrite )
	{
		dwAccess = (GENERIC_READ | GENERIC_WRITE);
		dwCreateDisposition = OPEN_EXISTING;
	}
	else if( uOpenFlags & modeWrite )
	{
		dwAccess = GENERIC_WRITE;
		dwCreateDisposition = CREATE_NEW;
	}
	else
	{
		dwAccess = 0;
		dwCreateDisposition = OPEN_EXISTING;
	}

	// ���L���[�h

	DWORD				dwShare;

	if( uOpenFlags & shareDenyNone )
	{
		dwShare = (FILE_SHARE_READ | FILE_SHARE_WRITE);
	}
	else if( uOpenFlags & shareDenyRead )
	{
		dwShare = FILE_SHARE_WRITE;
	}
	else if( uOpenFlags & shareDenyWrite )
	{
		dwShare = FILE_SHARE_READ;
	}
	else
	{
		dwShare = 0;
	}

	// �t�@�C���̑�������уt���O

	DWORD				dwFlagsAndAttributes = FILE_ATTRIBUTE_NORMAL;

	if( uOpenFlags & osNoBuffer )
	{
		dwFlagsAndAttributes |= FILE_FLAG_NO_BUFFERING;
	}
	if( uOpenFlags & osWriteThrough )
	{
		dwFlagsAndAttributes |= FILE_FLAG_WRITE_THROUGH;
	}
	if( uOpenFlags & osRandomAccess )
	{
		dwFlagsAndAttributes |= FILE_FLAG_RANDOM_ACCESS;
	}
	if( uOpenFlags & osSequentialScan )
	{
		dwFlagsAndAttributes |= FILE_FLAG_SEQUENTIAL_SCAN;
	}

	// �t�@�C�������݂���Ƃ��A�܂��͑��݂��Ȃ��Ƃ��̂��ꂼ��̓���

	if( uOpenFlags & modeCreate )
	{
		if( uOpenFlags & modeNoTruncate )
		{
			dwCreateDisposition = OPEN_ALWAYS;
		}
		else
		{
			dwCreateDisposition = CREATE_ALWAYS;
		}
	}

	// �t�@�C�����J��

	m_hFile = ::CreateFile( pszPathToFile, dwAccess, dwShare, NULL, dwCreateDisposition, dwFlagsAndAttributes, NULL );

	// �t�@�C���p�X��ێ�

	m_clsPathToFile = pszPathToFile;
	m_clsFileName = m_clsPathToFile.GetFileName();
	m_clsFileExt = m_clsPathToFile.GetFileExt();

	return	(m_hFile != INVALID_HANDLE_VALUE);
}

//////////////////////////////////////////////////////////////////////////////////////////
//	�t�@�C�������

void	YCFile::Close()
{
	if( m_hFile != INVALID_HANDLE_VALUE )
	{
		::CloseHandle( m_hFile );
		m_hFile = INVALID_HANDLE_VALUE;
	}
}

//////////////////////////////////////////////////////////////////////////////////////////
//	�t�@�C����ǂݍ���

DWORD	YCFile::Read(
	void*				pvBuffer,						// �o�b�t�@
	DWORD				dwReadSize						// �ǂݍ��ރT�C�Y
	)
{
	DWORD				dwResult;

	if( !::ReadFile( m_hFile, pvBuffer, dwReadSize, &dwResult, NULL ) )
	{
		dwResult = 0;
	}

	return	dwResult;
}

//////////////////////////////////////////////////////////////////////////////////////////
//	�t�@�C���ɏ�������

DWORD	YCFile::Write(
	const void*			pvBuffer,						// �o�b�t�@
	DWORD				dwWriteSize						// �������ރT�C�Y
	)
{
	DWORD				dwResult;

	if( !::WriteFile( m_hFile, pvBuffer, dwWriteSize, &dwResult, NULL ) )
	{
		dwResult = 0;
	}

	return	dwResult;
}

//////////////////////////////////////////////////////////////////////////////////////////
//	�t�@�C���|�C���^���ړ�����

UINT64	YCFile::Seek(
	INT64				n64Offset,						// �ړ�����o�C�g��
	DWORD				dwSeekMode						// �ړ����[�h
	)
{
	switch( dwSeekMode )
	{
	case	begin:
		dwSeekMode = FILE_BEGIN;
		break;

	case	current:
		dwSeekMode = FILE_CURRENT;
		break;

	case	end:
		dwSeekMode = FILE_END;
		break;

	default:
		dwSeekMode = FILE_BEGIN;
	}

	LARGE_INTEGER		stliWork;

	stliWork.QuadPart = n64Offset;
	stliWork.LowPart = ::SetFilePointer( m_hFile, stliWork.LowPart, &stliWork.HighPart, dwSeekMode );

	if( (stliWork.LowPart == INVALID_SET_FILE_POINTER) && (GetLastError() != NO_ERROR) )
	{
		// �ړ����s

		stliWork.QuadPart = -1;
	}

	return	(UINT64) (stliWork.QuadPart);
}

//////////////////////////////////////////////////////////////////////////////////////////
//	�t�@�C���|�C���^��擪����ړ�����

UINT64	YCFile::SeekHed(
	INT64				n64Offset						// �ړ�����o�C�g��
	)
{
	return	Seek( n64Offset, begin );
}

//////////////////////////////////////////////////////////////////////////////////////////
//	�t�@�C���|�C���^���I�[����ړ�����

UINT64	YCFile::SeekEnd(
	INT64				n64Offset						// �ړ�����o�C�g��
	)
{
	return	Seek( -n64Offset, end );
}

//////////////////////////////////////////////////////////////////////////////////////////
//	�t�@�C���|�C���^�����݈ʒu����ړ�����

UINT64	YCFile::SeekCur(
	INT64				n64Offset						// �ړ�����o�C�g��
	)
{
	return	Seek( n64Offset, current );
}

//////////////////////////////////////////////////////////////////////////////////////////
//	���݂̃t�@�C���|�C���^���擾

UINT64	YCFile::GetPosition()
{
	return	SeekCur( 0 );
}

//////////////////////////////////////////////////////////////////////////////////////////
//	�t�@�C���̒������擾

UINT64	YCFile::GetLength()
{
	UINT64				u64CurrentOffset;
	UINT64				u64EndOffset;

	u64CurrentOffset = GetPosition();

	u64EndOffset = SeekEnd();

	SeekHed( u64CurrentOffset );

	return	u64EndOffset;
}

//////////////////////////////////////////////////////////////////////////////////////////
//	�t�@�C���p�X���擾

YCString	YCFile::GetFilePath()
{
	return	m_clsPathToFile;
}

//////////////////////////////////////////////////////////////////////////////////////////
//	�t�@�C�������擾

YCString	YCFile::GetFileName()
{
	return	m_clsFileName;
}

//////////////////////////////////////////////////////////////////////////////////////////
//	�g���q���擾

YCString	YCFile::GetFileExt()
{
	return	m_clsFileExt;
}
