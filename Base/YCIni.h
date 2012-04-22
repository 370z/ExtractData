
#pragma	once

//----------------------------------------------------------------------------------------
//-- INI�N���X ---------------------------------------------------------------------------
//----------------------------------------------------------------------------------------

class	YCIni
{
public:

											YCIni( LPCTSTR pszIniName );

	void									SetSection( UINT uID );
	void									SetSection( LPCTSTR pszSection );

	void									SetKey( LPCTSTR pszKey );

	template<typename TYPE> TYPE			ReadDec( const TYPE& rftDefault );
	template<typename TYPE> void			ReadDec( TYPE* ptDst, const TYPE& rftDefault );
	template<typename TYPE> void			ReadDec( TYPE* ptDst );

	template<typename TYPE> TYPE			ReadHex( LPCTSTR pszDef );
	template<typename TYPE> void			ReadHex( TYPE* ptDst, LPCTSTR pszDefault );

	void									ReadStr( LPTSTR pszDst, DWORD dwDstSize, LPCTSTR pszDefault );
	void									ReadStr( YCString& rfclsDst, const YCString& rfclsDefault );

	template<typename TYPE> void			WriteDec( const TYPE& rftDec );

	template<typename TYPE> void			WriteHex( const TYPE& rftHex, DWORD dwNumber = 1 );

	void									WriteStr( LPCTSTR pszStr );

	BOOL									DeleteSection( LPCTSTR pszSection = NULL );


private:

	YCString								m_clsPathToIni;
	YCString								m_clsSection;
	YCString								m_clsKey;
};

//////////////////////////////////////////////////////////////////////////////////////////
//	���l���擾

template<typename TYPE> TYPE	YCIni::ReadDec(
	const TYPE&			rftDefault						// �f�t�H���g�l
	)
{
	return	::GetPrivateProfileInt( m_clsSection, m_clsKey, rftDefault, m_clsPathToIni );
}

//////////////////////////////////////////////////////////////////////////////////////////
//	���l���擾

template<typename TYPE> void	YCIni::ReadDec(
	TYPE*				ptDst,							// ���l�̊i�[��
	const TYPE&			rftDefault						// �f�t�H���g�l
	)
{
	*ptDst = ReadDec( rftDefault );
}

//////////////////////////////////////////////////////////////////////////////////////////
//	���l���擾

template<typename TYPE> void	YCIni::ReadDec(
	TYPE*				ptDst							// ���l�̊i�[��(��Ƀf�t�H���g�l���i�[����Ă��邱��)
	)
{
	*ptDst = ReadDec( *ptDst );
}

//////////////////////////////////////////////////////////////////////////////////////////
//	16�i�����擾

template<typename TYPE> TYPE	YCIni::ReadHex(
	LPCTSTR				pszDefault						// �f�t�H���g�l
	)
{
	TCHAR				szWork[256];

	::GetPrivateProfileString( m_clsSection, m_clsKey, pszDefault, szWork, sizeof( szWork ), m_clsPathToIni );

	return	strtoul( szWork, NULL, 16 );
}

//////////////////////////////////////////////////////////////////////////////////////////
//	16�i�����擾

template<typename TYPE> void	YCIni::ReadHex(
	TYPE*				ptDst,							// 16�i���̊i�[��
	LPCTSTR				pszDefault						// �f�t�H���g�l
	)
{
	*ptDst = ReadHex( pszDefault );
}

//////////////////////////////////////////////////////////////////////////////////////////
//	���l��ݒ�

template<typename TYPE> void	YCIni::WriteDec(
	const TYPE&			rftDec							// �ݒ�l
	)
{
	TCHAR				szWork[256];

	_stprintf( szWork, _T("%d"), rftDec );

	WriteStr( szWork );
}

//////////////////////////////////////////////////////////////////////////////////////////
//	16�i����ݒ�

template<typename TYPE> void	YCIni::WriteHex(
	const TYPE&			rftHex,							// �ݒ�l
	DWORD				dwNumber						// ����
	)
{
	TCHAR				szWork[256];

	_stprintf( szWork, _T("%0*X"), dwNumber, rftHex );

	WriteStr( szWork );
}


