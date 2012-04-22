
#include	"stdafx.h"
#include	"YCMultiFile.h"

//////////////////////////////////////////////////////////////////////////////////////////
//	�R���X�g���N�^

YCMultiFile::YCMultiFile()
{
	m_dwCurrentFileID = 0;
}

//////////////////////////////////////////////////////////////////////////////////////////
//	�f�X�g���N�^

YCMultiFile::~YCMultiFile()
{
	Close();
}

//////////////////////////////////////////////////////////////////////////////////////////
//	���[�h���w�肵�ĊJ��

BOOL	YCMultiFile::Open(
	LPCTSTR				pszPathToFile,					// �t�@�C���p�X
	UINT				uOpenFlags						// ���[�h
	)
{
	Close();

	return	Add( pszPathToFile, uOpenFlags );
}

//////////////////////////////////////////////////////////////////////////////////////////
//	���[�h���w�肵�ĊJ��

BOOL	YCMultiFile::Add(
	LPCTSTR				pszPathToFile,					// �t�@�C���p�X
	UINT				uOpenFlags						// ���[�h
	)
{
	BOOL				bReturn = FALSE;
	YCFile*				pclFile = new YCFile();

	if( pclFile->Open( pszPathToFile, uOpenFlags ) )
	{
		// �I�[�v������

		m_vtpclFile.push_back( pclFile );

		bReturn = TRUE;
	}
	else
	{
		// �I�[�v�����s

		delete	pclFile;
	}

	return	bReturn;
}

//////////////////////////////////////////////////////////////////////////////////////////
//	�t�@�C�������

void	YCMultiFile::Close()
{
	for( size_t siCnt = 0 ; siCnt < m_vtpclFile.size() ; siCnt++ )
	{
		m_vtpclFile[siCnt]->Close();

		delete	m_vtpclFile[siCnt];
	}

	m_vtpclFile.clear();

	m_dwCurrentFileID = 0;
}

//////////////////////////////////////////////////////////////////////////////////////////
//	�t�@�C����ǂݍ���

DWORD	YCMultiFile::Read(
	void*				pvBuffer,						// �o�b�t�@
	DWORD				dwReadSize						// �ǂݍ��ރT�C�Y
	)
{
	DWORD				dwResult;
	BYTE*				pbtBuffer = (BYTE*) pvBuffer;
	DWORD				dwBufferPtr = 0;

	while( 1 )
	{
		dwResult = m_vtpclFile[m_dwCurrentFileID]->Read( &pbtBuffer[dwBufferPtr], dwReadSize );

		dwBufferPtr += dwResult;

		if( dwBufferPtr >= dwReadSize )
		{
			// �ǂݍ��ݏI��

			break;
		}

		// ���̃t�@�C�����J��

		SetNextFile();
	}

	return	dwBufferPtr;
}

//////////////////////////////////////////////////////////////////////////////////////////
//	�t�@�C���ɏ�������

DWORD	YCMultiFile::Write(
	const void*			pvBuffer,						// �o�b�t�@
	DWORD				dwWriteSize						// �������ރT�C�Y
	)
{
	return	m_vtpclFile[m_dwCurrentFileID]->Write( pvBuffer, dwWriteSize );
}

//////////////////////////////////////////////////////////////////////////////////////////
//	�t�@�C���|�C���^���ړ�����

UINT64	YCMultiFile::Seek(
	INT64				n64Offset,						// �ړ�����o�C�g��
	DWORD				dwSeekMode						// �ړ����[�h
	)
{
	UINT64				u64Position = 0;
	UINT64				u64CurrentPosition;
	UINT64				u64FileSize;

	switch( dwSeekMode )
	{
	case	YCFile::begin:
		// �擪����

		for( size_t siCnt = 0 ; siCnt < m_vtpclFile.size() ; siCnt++ )
		{
			u64FileSize = m_vtpclFile[siCnt]->GetLength();

			if( n64Offset >= u64FileSize )
			{
				n64Offset -= u64FileSize;
				u64Position += u64FileSize;
			}
			else
			{
				SetFile( siCnt );
				break;
			}
		}

		u64Position += m_vtpclFile[m_dwCurrentFileID]->Seek( n64Offset, dwSeekMode );

		break;

	case	YCFile::current:
		// ���݈ʒu����

		if( n64Offset > 0 )
		{
			// �v���X�̕����Ɉړ�

			for( size_t siCnt = m_dwCurrentFileID ; siCnt < m_vtpclFile.size() ; siCnt++ )
			{
				if( siCnt == m_dwCurrentFileID )
				{
					u64CurrentPosition = m_vtpclFile[siCnt]->GetPosition();
					u64FileSize = m_vtpclFile[siCnt]->GetLength() - u64CurrentPosition;
				}
				else
				{
					u64FileSize = m_vtpclFile[siCnt]->GetLength();
				}

				if( n64Offset >= u64FileSize )
				{
					n64Offset -= u64FileSize;
					u64Position += u64FileSize;
				}
				else
				{
					SetFile( siCnt );
					break;
				}

				dwSeekMode = YCFile::begin;
			}
		}
		else if( n64Offset < 0 )
		{
			// �}�C�i�X�̕����Ɉړ�

			n64Offset = -n64Offset;

			for( int nCnt = m_nCurrentFileID ; nCnt >= 0 ; nCnt-- )
			{
				if( nCnt == m_nCurrentFileID )
				{
					u64FileSize = m_vtpclFile[nCnt]->GetPosition();
				}
				else
				{
					u64FileSize = m_vtpclFile[nCnt]->GetLength();
				}

				if( n64Offset >= u64FileSize )
				{
					n64Offset -= u64FileSize;
					u64Position += u64FileSize;
				}
				else
				{
					SetFile( nCnt );
					break;
				}

				dwSeekMode = YCFile::end;
			}

			n64Offset = -n64Offset;
		}
		else
		{
			// �ړ����Ȃ�


		}

		u64Position += m_vtpclFile[m_dwCurrentFileID]->Seek( n64Offset, dwSeekMode );

		break;

	case	YCFile::end:
		// �I�[����





		break;
	}

	return	u64Position;
}

//////////////////////////////////////////////////////////////////////////////////////////
//	�t�@�C���|�C���^��擪����ړ�����

UINT64	YCMultiFile::SeekHed(
	INT64				n64Offset						// �ړ�����o�C�g��
	)
{
	return	Seek( n64Offset, YCFile::begin );
}

//////////////////////////////////////////////////////////////////////////////////////////
//	�t�@�C���|�C���^���I�[����ړ�����

UINT64	YCMultiFile::SeekEnd(
	INT64				n64Offset						// �ړ�����o�C�g��
	)
{
	return	Seek( -n64Offset, YCFile::end );
}

//////////////////////////////////////////////////////////////////////////////////////////
//	�t�@�C���|�C���^�����݈ʒu����ړ�����

UINT64	YCMultiFile::SeekCur(
	INT64				n64Offset						// �ړ�����o�C�g��
	)
{
	return	Seek( n64Offset, YCFile::current );
}

//////////////////////////////////////////////////////////////////////////////////////////
//	���݂̃t�@�C���|�C���^���擾

UINT64	YCMultiFile::GetPosition()
{
	return	SeekCur( 0 );
}

//////////////////////////////////////////////////////////////////////////////////////////
//	�t�@�C���̒������擾

UINT64	YCMultiFile::GetLength()
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

YCString	YCMultiFile::GetFilePath()
{
	return	m_vtpclFile[m_dwCurrentFileID]->GetFilePath();
}

//////////////////////////////////////////////////////////////////////////////////////////
//	�t�@�C�������擾

YCString	YCMultiFile::GetFileName()
{
	return	m_vtpclFile[m_dwCurrentFileID]->GetFileName();
}

//////////////////////////////////////////////////////////////////////////////////////////
//	�g���q���擾

YCString	YCMultiFile::GetFileExt()
{
	return	m_vtpclFile[m_dwCurrentFileID]->GetFileExt();
}

//////////////////////////////////////////////////////////////////////////////////////////
//	

void	YCMultiFile::SetFile(
	DWORD				dwFileID						// �t�@�C��ID
	)
{
	m_dwCurrentFileID = dwFileID;
}

//////////////////////////////////////////////////////////////////////////////////////////
//	

void	YCMultiFile::SetFirstFile()
{
	SetFile( 0 );
}

//////////////////////////////////////////////////////////////////////////////////////////
//	

void	YCMultiFile::SetNextFile()
{
	if( m_dwCurrentFileID >= GetFileCount() )
	{
		// �����O��Ɉ���

		SetFile( 0 );
	}
	else
	{
		SetFile( GetCurrentFileID() + 1 );
	}
}

//////////////////////////////////////////////////////////////////////////////////////////
//	

DWORD	YCMultiFile::GetCurrentFileID()
{
	return	m_dwCurrentFileID;
}

//////////////////////////////////////////////////////////////////////////////////////////
//	

size_t	YCMultiFile::GetFileCount()
{
	return	m_vtpclFile.size();
}
