
#include	"stdafx.h"
#include	"YCFileFindSx.h"

//////////////////////////////////////////////////////////////////////////////////////////
//	�t�@�C������

BOOL	YCFileFindSx::FindFile(
	std::vector<YCString>&	rfvcPathToDstFile,			// �i�[��
	LPCTSTR					pszPathToTargetFolder,		// �����Ώۃf�B���N�g��
	LPCTSTR					pszTargetFileName,			// �Y���t�@�C����(���C���h�J�[�h��)
	BOOL					bSearchSubDirectory			// �T�u�f�B���N�g���̌����w��(TRUE�F��������AFALSE�F�������Ȃ�)
	)
{
	BOOL				bReturn = FALSE;

	// �t�@�C���T��

	YCFileFind			clffTarget;

	if( clffTarget.FindFirstFile( pszPathToTargetFolder, pszTargetFileName ) )
	{
		do
		{
			if( clffTarget.IsDots() )
			{
				// �}�[�J�[

				continue;
			}

			if( clffTarget.IsDirectory() )
			{
				// �f�B���N�g��

				continue;
			}

			// ���������t�@�C�������X�g�ɉ�����

			rfvcPathToDstFile.push_back( clffTarget.GetFilePath() );

		}
		while( clffTarget.FindNextFile() );

		bReturn = TRUE;
	}

	clffTarget.Close();

	if( !bSearchSubDirectory )
	{
		// �T�u�f�B���N�g�����������Ȃ�

		return	bReturn;
	}

	// �f�B���N�g���T��

	if( clffTarget.FindFirstFile( pszPathToTargetFolder, _T("*.*") ) )
	{
		do
		{
			if( clffTarget.IsDots() )
			{
				// �}�[�J�[

				continue;
			}

			if( !clffTarget.IsDirectory() )
			{
				// �f�B���N�g���ł͂Ȃ�

				continue;
			}

			// �ċA�Ăяo��

			FindFile( rfvcPathToDstFile, clffTarget.GetFilePath(), pszTargetFileName, bSearchSubDirectory );
		}
		while( clffTarget.FindNextFile() );

		bReturn = TRUE;
	}

	clffTarget.Close();

	return	bReturn;
}
