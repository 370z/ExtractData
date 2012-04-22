
#include	"stdafx.h"
#include	"YCUtil.h"
/*
//////////////////////////////////////////////////////////////////////////////////////////
//	�t�@�C���t�H�[�}�b�g��ݒ�

YCString	YCUtil::SetFileFormat(
	const YCString&		rfcstFileName					// �t�@�C����
	)
{
	// �g���q���擾

	LPCTSTR				pszFileExt = PathFindExtension( rfcstFileName );

	if( pszFileExt == NULL )
	{
		// �g���q�����݂��Ȃ�

		return	_T("");
	}

	if( lstrlen( pszFileExt ) < 2 )
	{
		// �g���q��"."�����Ȃ�

		return	_T("");
	}

	// �g���q��啶���ɕϊ�

	TCHAR				szFileFormat[256];

	lstrcpy( szFileFormat, &pszFileExt[1] );
	CharUpper( szFileFormat );

	return	szFileFormat;
}

//////////////////////////////////////////////////////////////////////////////////////////
//	���l���w�肵�������ƂɃJ���}�ŋ�؂�

YCString	YCUtil::SetCommaFormat(
	DWORD				dwSize,							// ���l
	DWORD				dwDigit							// ��؂錅
	)
{
	YCString			cstSize;

	cstSize.Format( _T("%d"), dwSize );

	int					nLength = cstSize.length();

	int					comma_num = (nLength - 1) / dwDigit;
	int					comma_pos = nLength % dwDigit;

	if( comma_pos == 0 )
	{
		comma_pos = dwDigit;
	}
	if( comma_num == 0 )
	{
		comma_pos = 0;
	}

	for( int i = 0 ; i < comma_num ; i++ )
	{
		cstSize.insert( comma_pos + dwDigit * i + i, _T(",") );
	}

	return	cstSize;
}
*/
//////////////////////////////////////////////////////////////////////////////////////////
//	�쐬�������ŉ��w�̃f�B���N�g���܂ō쐬

void	YCUtil::CreateDirectory(
	LPCTSTR				pszPathToFile,					// �t�@�C���p�X
	BOOL				bFileName						// �t�@�C���p�X�Ƀt�@�C�������܂܂�Ă��邩(TRUE�F�܂܂�Ă���AFALSE�F�܂܂�Ă��Ȃ�)
	)
{
	std::vector<YCString>	vcDirPathList;
	LPCTSTR					pszFilePathBase = pszPathToFile;

	while( (pszPathToFile = PathFindNextComponent( pszPathToFile )) != NULL )
	{
		YCString				clsDirPath( pszFilePathBase, pszPathToFile - pszFilePathBase - 1 ); // �O�̂���-1���Ė�����'\'��t���Ȃ��悤�ɂ���

		vcDirPathList.push_back( clsDirPath );
	}

	// ���[�g���珇�Ƀf�B���N�g�����쐬

	size_t					uMax = vcDirPathList.size();

	if( bFileName )
	{
		// �t�@�C�����̃f�B���N�g�����쐬���Ȃ��悤��-1����

		uMax--;
	}

	for( size_t uCnt = 0 ; uCnt < uMax ; uCnt++ )
	{
		::CreateDirectory( vcDirPathList[uCnt], NULL );
	}
}

//////////////////////////////////////////////////////////////////////////////////////////
//	�X���b�V�����o�b�N�X���b�V���ɒu��

void	YCUtil::ReplaceSlashToBackslash(
	LPSTR				pszFileName						// �t�@�C����
	)
{
	while( *pszFileName != '\0' )
	{
		if( !::IsDBCSLeadByte( *pszFileName ) )
		{
			// 1�o�C�g����

			// �X���b�V�����o�b�N�X���b�V���ɒu��

			if( *pszFileName == '/' )
			{
				*pszFileName = '\\';
			}
		}

		// ���̕�����

		pszFileName = ::CharNextA( pszFileName );
	}
}

//////////////////////////////////////////////////////////////////////////////////////////
//	�X���b�V�����o�b�N�X���b�V���ɒu��

void	YCUtil::ReplaceSlashToBackslash(
	LPWSTR				pwszFileName					// �t�@�C����
	)
{
	for( ; *pwszFileName != L'\0' ; pwszFileName++ )
	{
		if( *pwszFileName == L'/' )
		{
			// �X���b�V��

			*pwszFileName = L'\\';
		}
	}
}

//////////////////////////////////////////////////////////////////////////////////////////
//	�G���f�B�A���ϊ�(4byte)

DWORD	YCUtil::ConvEndian(
	DWORD				dwSrc
	)
{
	_asm
	{
		mov				eax, dwSrc
		bswap			eax
		mov				dwSrc, eax
	}

	return	dwSrc;
}

//////////////////////////////////////////////////////////////////////////////////////////
//	�G���f�B�A���ϊ�(4byte)

void	YCUtil::ConvEndian(
	LPDWORD				pdwDst
	)
{
	*pdwDst = ConvEndian( *pdwDst );
}

//////////////////////////////////////////////////////////////////////////////////////////
//	�G���f�B�A���ϊ�(2byte)

WORD	YCUtil::ConvEndian(
	WORD				wSrc
	)
{
	_asm
	{
		mov				ax, wSrc
		rol				ax, 8
		mov				wSrc, ax
	}

	return	wSrc;
}

//////////////////////////////////////////////////////////////////////////////////////////
//	�G���f�B�A���ϊ�(2byte)

void	YCUtil::ConvEndian(
	LPWORD				pwDst
	)
{
	*pwDst = ConvEndian( *pwDst );
}

//////////////////////////////////////////////////////////////////////////////////////////
//	�������̓��e���r
//
//	���l	�W���֐�memcmp()�̃��C���h�J�[�h�Ή���

BOOL	YCUtil::CompareMemory(
	const void*			pvData1,						// ��r�f�[�^1
	const void*			pvData2,						// ��r�f�[�^2
	DWORD				dwSize							// ��r�T�C�Y
	)
{
	BYTE*				pbtData1 = (BYTE*)pvData1;
	BYTE*				pbtData2 = (BYTE*)pvData2;

	for( DWORD i = 0 ; i < dwSize ; i++ )
	{
		if( (pbtData1[i] != pbtData2[i]) && (pbtData2[i] != '*') )
		{
			// �s��v

			return	FALSE;
		}
	}

	// ��v

	return	TRUE;
}
