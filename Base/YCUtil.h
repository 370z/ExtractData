
#pragma	once

//----------------------------------------------------------------------------------------
//-- ���[�e�B���e�B�֐� ------------------------------------------------------------------
//----------------------------------------------------------------------------------------

namespace	YCUtil
{
	// ������֌W

//	YCString								SetFileFormat( const YCString& rfclsFileName );
//	YCString								SetCommaFormat( DWORD dwSize, DWORD dwDigit = 3 );

	// �p�X�֌W

	void									CreateDirectory( LPCTSTR pszPathToFile, BOOL bFileName );
	void									ReplaceSlashToBackslash( LPSTR pszFileName );
	void									ReplaceSlashToBackslash( LPWSTR pwszFileName );

	// �G���f�B�A���ϊ�

	DWORD									ConvEndian( DWORD dwSrc );
	void									ConvEndian( LPDWORD pdwDst );

	WORD									ConvEndian( WORD wSrc );
	void									ConvEndian( LPWORD pwDst );

	// �������֌W

	inline BOOL								CompareMemory( const void* pvData1, const void* pvData2, DWORD dwSize );
};
