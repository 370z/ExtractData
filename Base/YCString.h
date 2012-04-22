
#pragma		once

#define		YCSTRINGT_BUFFERSIZE_ALIGNMENT				64	// �o�b�t�@�̃A���C�����g
#define		YCSTRINGT_OFFSET_BUFFERSIZE					16
#define		YCSTRINGT_OFFSET_LENGTH						12

//----------------------------------------------------------------------------------------
//-- ��{������N���X --------------------------------------------------------------------
//----------------------------------------------------------------------------------------

class	YCBaseString
{
public:

	int										GetBaseTypeLength( const char* pszSrc ) const;
	int										GetBaseTypeLength( const char* pszSrc, int nLength ) const;
	int										GetBaseTypeLength( const wchar_t* pwszSrc ) const;
	int										GetBaseTypeLength( const wchar_t* pwszSrc, int nLength ) const;

	int										ConvertToBaseType( wchar_t* pwszDst, int nDstLength, const char* pszSrc, int nSrcLength = -1 ) const;
	int										ConvertToBaseType( char* pszDst, int nDstLength, const wchar_t* pwszSrc, int nSrcLength = -1 ) const;
};

//----------------------------------------------------------------------------------------
//-- �e���v���[�g������N���X ------------------------------------------------------------
//----------------------------------------------------------------------------------------

// �ϒ������ɓn�����߁A���z�֐�������Ă͂����Ȃ�
// ���z�֐������Ɓu__vfptr�v�����o�ϐ��̑��݂ɂ��ϒ������ɓn���Ȃ��Ȃ�

template<class TYPE>
class	YCStringT : protected YCBaseString
{
private:

	TYPE*									m_ptString;


public:

	// �R���X�g���N�^

											YCStringT();
											YCStringT( const char* pszSrc );
											YCStringT( const char* pszSrc, int nCount );
											YCStringT( const char& rfcSrc );
											YCStringT( const wchar_t* pwszSrc );
											YCStringT( const wchar_t* pwszSrc, int nCount );
											YCStringT( const wchar_t& rfwcSrc );
											YCStringT( const YCStringT<TYPE>& rfclsSrc );
											YCStringT( const YCStringT<TYPE>& rfclsSrc, int nCount );

	// �f�X�g���N�^

											~YCStringT();	// ���z�֐��ɂ����__vfptr�̑��݂ɂ��ϒ������ɓn���Ȃ��Ȃ�

	// ���

	BOOL									LoadString( UINT uID );

	void									Copy( const char* pszSrc );
	void									Copy( const char* pszSrc, int nLength );
	void									Copy( const char& rfcSrc );
	void									Copy( const wchar_t* pwszSrc );
	void									Copy( const wchar_t* pwszSrc, int nLength );
	void									Copy( const wchar_t& rfwcSrc );

	// �ǉ�

	void									Append( const char* pszAppend );
	void									Append( const char* pszAppend, int nLength );
	void									Append( const char& rfcAppend );
	void									Append( const wchar_t* pwszAppend );
	void									Append( const wchar_t* pwszAppend, int nLength );
	void									Append( const wchar_t& rfwcAppend );

	// �����t���f�[�^�̐ݒ�

	void									Format( const TYPE* pszFormat, ... );
	void									AppendFormat( const TYPE* pszFormat, ... );

	// �}��

	int										Insert( int nIndex, const TYPE* pszInsert );
	int										Insert( int nIndex, const TYPE& rfcInsert );

	// �폜

	void									Empty();

	int										Delete( int nIndex, int nCount = 1 );

	int										Remove( const TYPE* pszRemove );
	int										Remove( const TYPE& rfcRemove );

	YCStringT<TYPE>&						Trim();
	YCStringT<TYPE>&						Trim( const TYPE* pszTrim );
	YCStringT<TYPE>&						Trim( const TYPE& rfcTrim );

	YCStringT<TYPE>&						TrimLeft();
	YCStringT<TYPE>&						TrimLeft( const TYPE* pszTrim );
	YCStringT<TYPE>&						TrimLeft( const TYPE& rfcTrim );

	YCStringT<TYPE>&						TrimRight();
	YCStringT<TYPE>&						TrimRight( const TYPE* pszTrim );
	YCStringT<TYPE>&						TrimRight( const TYPE& rfcTrim );

	// �u��

	int										Replace( const TYPE* pszOld, const TYPE* pszNew );
	int										Replace( const TYPE& rfcOld, const TYPE& rfcNew );

	// �������啶���ϊ�

	YCStringT<TYPE>&						MakeLower();
	YCStringT<TYPE>&						MakeUpper();

	// ��r

	int										Compare( const char* pszSrc ) const;
	int										Compare( const char& rfcSrc ) const;
	int										Compare( const wchar_t* pwszSrc ) const;
	int										Compare( const wchar_t& pwwcSrc ) const;

	int										CompareNoCase( const char* pszSrc ) const;
	int										CompareNoCase( const char& rfcSrc ) const;
	int										CompareNoCase( const wchar_t* pwszSrc ) const;
	int										CompareNoCase( const wchar_t& rfwcSrc ) const;

	// ����

	int										Find( const TYPE* pszSub, int nStart = 0 ) const;
	int										Find( const TYPE& rfcSub, int nStart = 0 ) const;

	int										ReverseFind( const TYPE* pszSub ) const;
	int										ReverseFind( const TYPE& rfcSub ) const;

	// ���o

	YCStringT<TYPE>							Left( int nCount ) const;
	YCStringT<TYPE>							Right( int nCount ) const;
	YCStringT<TYPE>							Mid( int nFirst ) const;
	YCStringT<TYPE>							Mid( int nFirst, int nCount ) const;

	// ������o�b�t�@�̎擾

	TYPE*									GetBuffer( int nBufferSize );

	// ������o�b�t�@�̃|�C���^�̎擾

	inline	const TYPE*						GetString() const;

	// ������̒����̎擾

	inline	int								GetLength() const;

	// �����񂪋󂩂ǂ������ׂ�

	bool									IsEmpty() const;

	// �p�X�֘A

	void									RenameExtension( const TYPE* ptSrc );

	YCStringT<TYPE>							GetFileName() const;
	int										GetFileNameIndex() const;
	YCStringT<TYPE>							GetFileTitle() const;
	YCStringT<TYPE>							GetFileExt() const;
	int										GetFileExtIndex() const;
	YCStringT<TYPE>							GetDirPath() const;

	// ������Z�q

	YCStringT<TYPE>&						operator=( const char* pszSrc );
	YCStringT<TYPE>&						operator=( const char& rfcSrc );
	YCStringT<TYPE>&						operator=( const wchar_t* pwszSrc );
	YCStringT<TYPE>&						operator=( const wchar_t& rfwcSrc );
	YCStringT<TYPE>&						operator=( const YCStringT<TYPE>& rfclsSrc );

	// +���Z�q

	YCStringT<TYPE>							operator+( const char* pszAppend ) const;
	YCStringT<TYPE>							operator+( const char& rfcAppend ) const;
	YCStringT<TYPE>							operator+( const wchar_t* pwszAppend ) const;
	YCStringT<TYPE>							operator+( const wchar_t& rfwcAppend ) const;
	YCStringT<TYPE>							operator+( const YCStringT<TYPE>& rfclsAppend ) const;

	// +���Z�q(�O���֐�)

	template<class TYPE>
	friend	YCStringT<TYPE>					operator+( const char* pszSrc, const YCStringT<TYPE>& rfclsAppend );

	template<class TYPE>
	friend	YCStringT<TYPE>					operator+( const char& rfcSrc, const YCStringT<TYPE>& rfclsAppend );

	template<class TYPE>
	friend	YCStringT<TYPE>					operator+( const wchar_t* pwszSrc, const YCStringT<TYPE>& rfclsAppend );

	template<class TYPE>
	friend	YCStringT<TYPE>					operator+( const wchar_t& rfwcSrc, const YCStringT<TYPE>& rfclsAppend );

	// +=���Z�q

	YCStringT<TYPE>&						operator+=( const char* pszAppend );
	YCStringT<TYPE>&						operator+=( const char& rfcAppend );
	YCStringT<TYPE>&						operator+=( const wchar_t* pwszAppend );
	YCStringT<TYPE>&						operator+=( const wchar_t& rfwcAppend );
	YCStringT<TYPE>&						operator+=( const YCStringT<TYPE>& rfclsAppend );

	// ==���Z�q

	bool									operator==( const char* pszSrc ) const;
	bool									operator==( const char& rfcSrc ) const;
	bool									operator==( const wchar_t* pwszSrc ) const;
	bool									operator==( const wchar_t& rfwcSrc ) const;
	bool									operator==( const YCStringT<TYPE>& rfclsSrc ) const;

	// ==���Z�q(�O���֐�)

	template<class TYPE>
	friend	bool							operator==( const char* pszSrc, const YCStringT<TYPE>& rfclsSrc );

	template<class TYPE>
	friend	bool							operator==( const char& rfcSrc, const YCStringT<TYPE>& rfclsSrc );

	template<class TYPE>
	friend	bool							operator==( const wchar_t* pwszSrc, const YCStringT<TYPE>& rfclsSrc );

	template<class TYPE>
	friend	bool							operator==( const wchar_t& rfwcSrc, const YCStringT<TYPE>& rfclsSrc );

	// !=���Z�q

	bool									operator!=( const char* pszSrc ) const;
	bool									operator!=( const char& rfcSrc ) const;
	bool									operator!=( const wchar_t* pwszSrc ) const;
	bool									operator!=( const wchar_t& rfwcSrc ) const;
	bool									operator!=( const YCStringT<TYPE>& rfclsSrc ) const;

	// !=���Z�q(�O���֐�)

	template<class TYPE>
	friend	bool							operator!=( const char* pszSrc, const YCStringT<TYPE>& rfclsSrc );

	template<class TYPE>
	friend	bool							operator!=( const char& rfcSrc, const YCStringT<TYPE>& rfclsSrc );

	template<class TYPE>
	friend	bool							operator!=( const wchar_t* pwszSrc, const YCStringT<TYPE>& rfclsSrc );

	template<class TYPE>
	friend	bool							operator!=( const wchar_t& rfwcSrc, const YCStringT<TYPE>& rfclsSrc );

	// <���Z�q

	bool									operator<( const char* pszSrc ) const;
	bool									operator<( const wchar_t* pwszSrc ) const;
	bool									operator<( const YCStringT<TYPE>& rfclsSrc ) const;

	// <���Z�q(�O���֐�)

	template<class TYPE>
	friend	bool							operator<( const char* pszSrc, const YCStringT<TYPE>& rfclsSrc );

	template<class TYPE>
	friend	bool							operator<( const wchar_t* pwszSrc, const YCStringT<TYPE>& rfclsSrc );

	// <=���Z�q

	bool									operator<=( const char* pszSrc ) const;
	bool									operator<=( const wchar_t* pwszSrc ) const;
	bool									operator<=( const YCStringT<TYPE>& rfclsSrc ) const;

	// <=���Z�q(�O���֐�)

	template<class TYPE>
	friend	bool							operator<=( const char* pszSrc, const YCStringT<TYPE>& rfclsSrc );

	template<class TYPE>
	friend	bool							operator<=( const wchar_t* pwszSrc, const YCStringT<TYPE>& rfclsSrc );

	// >���Z�q

	bool									operator>( const char* pszSrc ) const;
	bool									operator>( const wchar_t* pwszSrc ) const;
	bool									operator>( const YCStringT<TYPE>& rfclsSrc ) const;

	// >���Z�q(�O���֐�)

	template<class TYPE>
	friend	bool							operator>( const char* pszSrc, const YCStringT<TYPE>& rfclsSrc );

	template<class TYPE>
	friend	bool							operator>( const wchar_t* pwszSrc, const YCStringT<TYPE>& rfclsSrc );

	// >=���Z�q

	bool									operator>=( const char* pszSrc ) const;
	bool									operator>=( const wchar_t* pwszSrc ) const;
	bool									operator>=( const YCStringT<TYPE>& rfclsSrc ) const;

	// >=���Z�q(�O���֐�)

	template<class TYPE>
	friend	bool							operator>=( const char* pszSrc, const YCStringT<TYPE>& rfclsSrc );

	template<class TYPE>
	friend	bool							operator>=( const wchar_t* pwszSrc, const YCStringT<TYPE>& rfclsSrc );

	// []���Z�q

	inline	TYPE&							operator[]( int nPos );
	inline	const TYPE&						operator[]( int nPos ) const;

	// const TYPE*���Z�q

	inline									operator const TYPE*() const;


protected:

	void									Alloc( int nBufferSize );
	void									Free();

	void									ExtendBuffer( int nBufferSize );

	void									SetBufferSize( int nBufferSize );
	int										GetBufferSize();

	void									SetLength( int nLength );
};

typedef	YCStringT<TCHAR>								YCString;
typedef	YCStringT<char>									YCStringA;
typedef	YCStringT<wchar_t>								YCStringW;

//////////////////////////////////////////////////////////////////////////////////////////
//	�R���X�g���N�^

template<class TYPE> YCStringT<TYPE>::YCStringT()
{
	m_ptString = NULL;

	Empty();
}

//////////////////////////////////////////////////////////////////////////////////////////
//	�R���X�g���N�^

template<class TYPE> YCStringT<TYPE>::YCStringT(
	const char*			pszSrc							// �����l
	)
{
	m_ptString = NULL;

	Copy( pszSrc );
}

//////////////////////////////////////////////////////////////////////////////////////////
//	�R���X�g���N�^

template<class TYPE> YCStringT<TYPE>::YCStringT(
	const char*			pszSrc,							// �����l
	int					nCount							// �R�s�[���钷��
	)
{
	m_ptString = NULL;

	Copy( pszSrc, nCount );
}

//////////////////////////////////////////////////////////////////////////////////////////
//	�R���X�g���N�^

template<class TYPE> YCStringT<TYPE>::YCStringT(
	const char&			rfcSrc							// �����l
	)
{
	m_ptString = NULL;

	Copy( rfcSrc );
}

//////////////////////////////////////////////////////////////////////////////////////////
//	�R���X�g���N�^

template<class TYPE> YCStringT<TYPE>::YCStringT(
	const wchar_t*		pwszSrc							// �����l
	)
{
	m_ptString = NULL;

	Copy( pwszSrc );
}

//////////////////////////////////////////////////////////////////////////////////////////
//	�R���X�g���N�^

template<class TYPE> YCStringT<TYPE>::YCStringT(
	const wchar_t*		pwszSrc,						// �����l
	int					nCount							// �R�s�[���钷��
	)
{
	m_ptString = NULL;

	Copy( pwszSrc, nCount );
}

//////////////////////////////////////////////////////////////////////////////////////////
//	�R���X�g���N�^

template<class TYPE> YCStringT<TYPE>::YCStringT(
	const wchar_t&		rfwcSrc							// �����l
	)
{
	m_ptString = NULL;

	Copy( rfwcSrc );
}

//////////////////////////////////////////////////////////////////////////////////////////
//	�R�s�[�R���X�g���N�^

template<class TYPE> YCStringT<TYPE>::YCStringT(
	const YCStringT<TYPE>&	rfclsSrc					// �R�s�[��������N���X
	)
{
	m_ptString = NULL;

	Copy( rfclsSrc );
}

//////////////////////////////////////////////////////////////////////////////////////////
//	�f�X�g���N�^

template<class TYPE> YCStringT<TYPE>::~YCStringT()
{
	Free();
}

//////////////////////////////////////////////////////////////////////////////////////////
//	�����񃊃\�[�X��ǂݍ���

template<class TYPE> BOOL YCStringT<TYPE>::LoadString(
	UINT				uID								// ���\�[�XID
	)
{
	return	::LoadString( ::GetModuleHandle( NULL ), uID, GetBuffer( 1024 ), 1024 );
}

//////////////////////////////////////////////////////////////////////////////////////////
//	������̑��

template<class TYPE> void YCStringT<TYPE>::Copy(
	const char*			pszSrc							// �R�s�[��������
	)
{
	TYPE*				ptString;
	int					nSrcLength;

	switch( sizeof(TYPE) )
	{
	case	1:
		// char�^

		nSrcLength = lstrlenA( pszSrc );

		ptString = GetBuffer( (nSrcLength + 1) );

		lstrcpyA( (char*) ptString, pszSrc );

		break;

	case	2:
		// wchar_t�^

		nSrcLength = GetBaseTypeLength( pszSrc );

		ptString = GetBuffer( (nSrcLength + 1) );

		ConvertToBaseType( (wchar_t*) ptString, GetBufferSize(), pszSrc );

		break;
	}

	SetLength( nSrcLength );
}

//////////////////////////////////////////////////////////////////////////////////////////
//	������̑��

template<class TYPE> void YCStringT<TYPE>::Copy(
	const char*			pszSrc,							// �R�s�[��������
	int					nCount							// �R�s�[���钷��
	)
{
	TYPE*				ptString;
	int					nSrcLength;

	switch( sizeof(TYPE) )
	{
	case	1:
		// char�^

		nSrcLength = nCount;

		ptString = GetBuffer( (nSrcLength + 1) );

		lstrcpynA( (char*) ptString, pszSrc, (nCount + 1) );

		break;

	case	2:
		// wchar_t�^

		nSrcLength = GetBaseTypeLength( pszSrc, nCount );

		ptString = GetBuffer( (nSrcLength + 1) );

		ConvertToBaseType( (wchar_t*) ptString, GetBufferSize(), pszSrc, nCount );

		break;
	}

	SetLength( nSrcLength );
}

//////////////////////////////////////////////////////////////////////////////////////////
//	�����̑��

template<class TYPE> void YCStringT<TYPE>::Copy(
	const char&			rfcSrc							// �R�s�[������
	)
{
	char				szSrc[2];

	szSrc[0] = rfcSrc;
	szSrc[1] = '\0';

	Copy( szSrc );
}

//////////////////////////////////////////////////////////////////////////////////////////
//	������̑��

template<class TYPE> void YCStringT<TYPE>::Copy(
	const wchar_t*		pwszSrc							// �R�s�[��������
	)
{
	TYPE*				ptString;
	int					nSrcLength;

	switch( sizeof(TYPE) )
	{
	case	1:
		// char�^

		nSrcLength = GetBaseTypeLength( pwszSrc );

		ptString = GetBuffer( (nSrcLength + 1) );

		ConvertToBaseType( (char*) ptString, GetBufferSize(), pwszSrc );

		break;

	case	2:
		// wchar_t�^

		nSrcLength = lstrlenW( pwszSrc );

		ptString = GetBuffer( (nSrcLength + 1) );

		lstrcpyW( (wchar_t*) ptString, pwszSrc );

		break;
	}

	SetLength( nSrcLength );
}

//////////////////////////////////////////////////////////////////////////////////////////
//	������̑��

template<class TYPE> void YCStringT<TYPE>::Copy(
	const wchar_t*		pwszSrc,						// �R�s�[��������
	int					nCount							// �R�s�[���钷��
	)
{
	TYPE*				ptString;
	int					nSrcLength;

	switch( sizeof(TYPE) )
	{
	case	1:
		// char�^

		nSrcLength = GetBaseTypeLength( pwszSrc, nCount );

		ptString = GetBuffer( (nSrcLength + 1) );

		ConvertToBaseType( (char*) ptString, GetBufferSize(), pwszSrc, nCount );

		break;

	case	2:
		// wchar_t�^

		nSrcLength = nCount;

		ptString = GetBuffer( (nSrcLength + 1) );

		lstrcpynW( (wchar_t*) ptString, (wchar_t*) pwszSrc, (nCount + 1) );

		break;
	}

	SetLength( nSrcLength );
}

//////////////////////////////////////////////////////////////////////////////////////////
//	�����̑��

template<class TYPE> void YCStringT<TYPE>::Copy(
	const wchar_t&		rfwcSrc							// �R�s�[������
	)
{
	wchar_t				wszSrc[2];

	wszSrc[0] = rfwcSrc;
	wszSrc[1] = L'\0';

	Copy( wszSrc );
}

//////////////////////////////////////////////////////////////////////////////////////////
//	������̒ǉ�

template<class TYPE> void YCStringT<TYPE>::Append(
	const char*			pszAppend						// �ǉ�������
	)
{
	int					nStringLength = GetLength();
	int					nNewLength = nStringLength;

	switch( sizeof(TYPE) )
	{
	case	1:
		// char�^

		nNewLength += lstrlenA( pszAppend );

		// �o�b�t�@�̊g��

		ExtendBuffer( (nNewLength + 1) );

		// ������̒ǉ�

		lstrcpyA( (char*) &m_ptString[nStringLength], pszAppend );

		break;

	case	2:
		// wchar_t�^

		nNewLength += GetBaseTypeLength( pszAppend );

		// �o�b�t�@�̊g��

		ExtendBuffer( (nNewLength + 1) );

		// ������̒ǉ�

		ConvertToBaseType( (wchar_t*) &m_ptString[nStringLength], (GetBufferSize() - nStringLength), pszAppend );

		break;
	}

	SetLength( nNewLength );
}

//////////////////////////////////////////////////////////////////////////////////////////
//	������̒ǉ�

template<class TYPE> void YCStringT<TYPE>::Append(
	const char*			pszAppend,						// �ǉ�������
	int					nCount							// �ǉ����镶����
	)
{
	int					nStringLength = GetLength();
	int					nNewLength = nStringLength;

	switch( sizeof(TYPE) )
	{
	case	1:
		// char�^

		nNewLength += nCount;

		// �o�b�t�@�̊g��

		ExtendBuffer( (nNewLength + 1) );

		// ������̒ǉ�

		lstrcpynA( (char*) &m_ptString[nStringLength], pszAppend, (nCount + 1) );

		break;

	case	2:
		// wchar_t�^

		nNewLength += GetBaseTypeLength( pszAppend, nCount );

		// �o�b�t�@�̊g��

		ExtendBuffer( (nNewLength + 1) );

		// ������̒ǉ�

		ConvertToBaseType( (wchar_t*) &m_ptString[nStringLength], (GetBufferSize() - nStringLength), pszAppend, nCount );

		break;
	}

	SetLength( nNewLength );
}

//////////////////////////////////////////////////////////////////////////////////////////
//	�����̒ǉ�

template<class TYPE> void YCStringT<TYPE>::Append(
	const char&			rfcAppend						// �ǉ�����
	)
{
	char				szAppend[2];

	szAppend[0] = rfcAppend;
	szAppend[1] = '\0';

	Append( szAppend );
}

//////////////////////////////////////////////////////////////////////////////////////////
//	������̒ǉ�

template<class TYPE> void YCStringT<TYPE>::Append(
	const wchar_t*		pwszAppend						// �ǉ�������
	)
{
	int					nStringLength = GetLength();
	int					nNewLength = nStringLength;

	switch( sizeof(TYPE) )
	{
	case	1:
		// char�^

		nNewLength += GetBaseTypeLength( pwszAppend );

		// �o�b�t�@�̊g��

		ExtendBuffer( (nNewLength + 1) );

		// ������̒ǉ�

		ConvertToBaseType( (char*) &m_ptString[nStringLength], (GetBufferSize() - nStringLength), pwszAppend );

		break;

	case	2:
		// wchar_t�^

		nNewLength += lstrlenW( pwszAppend );

		// �o�b�t�@�̊g��

		ExtendBuffer( (nNewLength + 1) );

		// ������̒ǉ�

		lstrcpyW( (wchar_t*) &m_ptString[nStringLength], pwszAppend );

		break;
	}

	SetLength( nNewLength );
}

//////////////////////////////////////////////////////////////////////////////////////////
//	������̒ǉ�

template<class TYPE> void YCStringT<TYPE>::Append(
	const wchar_t*		pwszAppend,						// �ǉ�������
	int					nCount							// �ǉ����镶����
	)
{
	int					nStringLength = GetLength();
	int					nNewLength = nStringLength;

	switch( sizeof(TYPE) )
	{
	case	1:
		// char�^

		nNewLength += GetBaseTypeLength( pwszAppend, nCount );

		// �o�b�t�@�̊g��

		ExtendBuffer( (nNewLength + 1) );

		// ������̒ǉ�

		ConvertToBaseType( (char*) &m_ptString[nStringLength], (GetBufferSize() - nStringLength), pwszAppend, nCount );

		break;

	case	2:
		// wchar_t�^

		nNewLength += nCount;

		// �o�b�t�@�̊g��

		ExtendBuffer( (nNewLength + 1) );

		// ������̒ǉ�

		lstrcpynW( (wchar_t*) &m_ptString[nStringLength], pwszAppend, (nCount + 1) );

		break;
	}

	SetLength( nNewLength );
}

//////////////////////////////////////////////////////////////////////////////////////////
//	�����̒ǉ�

template<class TYPE> void YCStringT<TYPE>::Append(
	const wchar_t&		rfwcAppend						// �ǉ�����
	)
{
	wchar_t				wszAppend[2];

	wszAppend[0] = rfwcAppend;
	wszAppend[1] = L'\0';

	Append( wszAppend );
}

//////////////////////////////////////////////////////////////////////////////////////////
//	�����t���f�[�^�̐ݒ�

template<class TYPE> void YCStringT<TYPE>::Format(
	const TYPE*			pszFormat,						// �����t���f�[�^
	...													// �ϒ�����
	)
{
	va_list				vaArgPtr;

	va_start( vaArgPtr, pszFormat );

	YCMemory<TYPE>		clmBuffer( 1024 );

	while( 1 )
	{
		va_list				vaWork = vaArgPtr;

		if( _vsntprintf( &clmBuffer[0], (clmBuffer.size() - 1), pszFormat, vaWork ) == - 1 )
		{
			clmBuffer.resize( clmBuffer.size() * 2 );
		}
		else
		{
			break;
		}
	}

	va_end( vaArgPtr );

	Copy( &clmBuffer[0] );
}

//////////////////////////////////////////////////////////////////////////////////////////
//	�����t���f�[�^�̐ݒ�

template<class TYPE> void YCStringT<TYPE>::AppendFormat(
	const TYPE*			pszFormat,						// �����t���f�[�^
	...													// �ϒ�����
	)
{
	va_list				vaArgPtr;

	va_start( vaArgPtr, pszFormat );

	YCMemory<TYPE>		clmBuffer( 1024 );

	while( 1 )
	{
		va_list				vaWork = vaArgPtr;

		if( _vsntprintf( &clmBuffer[0], (clmBuffer.size() - 1), pszFormat, vaWork ) == - 1 )
		{
			clmBuffer.resize( clmBuffer.size() * 2 );
		}
		else
		{
			break;
		}
	}

	va_end( vaArgPtr );

	Append( &clmBuffer[0] );
}

//////////////////////////////////////////////////////////////////////////////////////////
//	�}��
//
//	return	�}����̕�����̒���

template<class TYPE> int YCStringT<TYPE>::Insert(
	int					nIndex,							// �}���ʒu
	const TYPE*			pszInsert						// �}��������
	)
{
	if( nIndex < 0 )
	{
		// �擪�ɑ}��

		nIndex = 0;
	}
	else if( nIndex >= GetLength() )
	{
		// �A��

		Append( pszInsert );

		return	GetLength();
	}

	// �}������

	int					nStringLength = GetLength();
	int					nNewLength = nStringLength;
	int					nInsertLength;
	int					nShiftLength;

	switch( sizeof(TYPE) )
	{
	case	1:
		// char�^

		nInsertLength = lstrlenA( (const char*) pszInsert );
		nShiftLength = lstrlenA( (const char*) &m_ptString[nIndex] );
		break;

	case	2:
		// wchar_t�^

		nInsertLength = lstrlenW( (const wchar_t*) pszInsert );
		nShiftLength = lstrlenW( (const wchar_t*) &m_ptString[nIndex] );
		break;
	}

	// �o�b�t�@�̊g��

	nNewLength += nInsertLength;

	ExtendBuffer( (nNewLength + 1) );

	// �������}������󂫗̈�����

	memmove( &m_ptString[nIndex + nInsertLength], &m_ptString[nIndex], (sizeof(TYPE) * (nShiftLength + 1)) );

	// ������̑}��

	memcpy( &m_ptString[nIndex], pszInsert, (sizeof(TYPE) * nInsertLength) );

	SetLength( nNewLength );

	return	GetLength();
}

//////////////////////////////////////////////////////////////////////////////////////////
//	�����̑}��
//
//	return	�}����̕�����̒���

template<class TYPE> int YCStringT<TYPE>::Insert(
	int					nIndex,							// �}���ʒu
	const TYPE&			rfcInsert						// �}������
	)
{
	TYPE				szInsert[2];

	szInsert[0] = rfcInsert;
	szInsert[1] = 0;

	return	Insert( nIndex, szInsert );
}

//////////////////////////////////////////////////////////////////////////////////////////
//	���������ɂ���

template<class TYPE> void YCStringT<TYPE>::Empty()
{
	switch( sizeof(TYPE) )
	{
	case	1:
		// char�^

		Copy( "" );
		break;

	case	2:
		// wchar_t�^

		Copy( L"" );
		break;
	}
}

//////////////////////////////////////////////////////////////////////////////////////////
//	������̍폜
//
//	�폜��̕�����̒���

template<class TYPE> int YCStringT<TYPE>::Delete(
	int					nIndex,							// �폜�ʒu
	int					nCount							// �폜���镶����
	)
{
	int					nStringLength = GetLength();
	int					nNewLength = nStringLength;

	if( nIndex < 0 )
	{
		// �擪����폜

		nIndex = 0;
	}
	else if( nIndex >= nStringLength )
	{
		// �폜���镶����Ȃ�

		return	GetLength();
	}

	if( nCount <= 0 )
	{
		// �폜���镶����Ȃ�

		return	GetLength();
	}

	if( (nIndex + nCount) >= nStringLength )
	{
		// �폜�ʒu�ȍ~�̕�������폜

		m_ptString[nIndex] = 0;

		SetLength( nIndex );

		return	GetLength();
	}

	// �폜����

	int					nShiftLength = (GetLength() - (nIndex + nCount));

	// ������̍폜
	// ����������ɋl�߂ď㏑������

	memmove( &m_ptString[nIndex], &m_ptString[nIndex + nCount], (sizeof(TYPE) * (nShiftLength + 1)) );

	nNewLength -= nCount;

	SetLength( nNewLength );

	return	GetLength();
}

//////////////////////////////////////////////////////////////////////////////////////////
//	������̍폜
//
//	�����񂩂�폜������

template<class TYPE> int YCStringT<TYPE>::Remove(
	const TYPE*			pszRemove						// �폜�Ώە�����
	)
{
	int					nIndex = 0;
	int					nCount = 0;
	int					nRemoveLength = 0;

	switch( sizeof(TYPE) )
	{
	case	1:
		// char�^

		nRemoveLength = lstrlenA( pszRemove );
		break;

	case	2:
		// wchar_t�^

		nRemoveLength = lstrlenW( pszRemove );
		break;
	}

	if( nRemoveLength <= 0 )
	{
		// �폜������Ȃ�

		return	nCount;
	}

	while( 1 )
	{
		nIndex = Find( pszRemove, nIndex );

		if( nIndex >= 0 )
		{
			// �폜�Ώە����񂪑��݂���

			Delete( nIndex, nRemoveLength );

			nCount++;
		}
		else
		{
			// �폜�Ώە����񂪑��݂��Ȃ�

			break;
		}
	}

	return	nCount;
}

//////////////////////////////////////////////////////////////////////////////////////////
//	�����̍폜
//
//	�����񂩂�폜������

template<class TYPE> int YCStringT<TYPE>::Remove(
	const TYPE&			rfcRemove						// �폜�Ώە���
	)
{
	int					nIndex = 0;
	int					nCount = 0;
	int					nRemoveLength = 1;

	while( 1 )
	{
		nIndex = Find( rfcRemove, nIndex );

		if( nIndex >= 0 )
		{
			// �폜�Ώە����񂪑��݂���

			Delete( nIndex, nRemoveLength );

			nCount++;
		}
		else
		{
			// �폜�Ώە����񂪑��݂��Ȃ�

			break;
		}
	}

	return	nCount;
}

//////////////////////////////////////////////////////////////////////////////////////////
//	�擪�Ɩ����ɂ���󔒕����̍폜

template<class TYPE> YCStringT<TYPE>& YCStringT<TYPE>::Trim()
{
	TrimLeft();
	TrimRight();

	return	*this;
}

//////////////////////////////////////////////////////////////////////////////////////////
//	�擪�Ɩ����ɂ���󔒕����̍폜

template<class TYPE> YCStringT<TYPE>&  YCStringT<TYPE>::Trim(
	const TYPE*			pszTrim							// �폜�Ώە�����
	)
{

}

//////////////////////////////////////////////////////////////////////////////////////////
//	�擪�Ɩ����ɂ���󔒕����̍폜

template<class TYPE> YCStringT<TYPE>&  YCStringT<TYPE>::Trim(
	const TYPE&			rfcTrim							// �폜�Ώە���
	)
{

}

//////////////////////////////////////////////////////////////////////////////////////////
//	�擪�ɂ���󔒕����̍폜

template<class TYPE> YCStringT<TYPE>& YCStringT<TYPE>::TrimLeft()
{
	BOOL				bResult;
	int					nCount;

	switch( sizeof(TYPE) )
	{
	case	1:
		// char�^

		bResult = TRUE;

		for( nCount = 0 ; nCount < GetLength() ; )
		{
			if( ::IsDBCSLeadByte( m_ptString[nCount] ) )
			{
				// �S�p

				if( strncmp( (char*) &m_ptString[nCount], "�@", 2 ) == 0 )
				{
					// �g���~���O�Ώە���
				}
				else
				{
					// ���̑�

					bResult = FALSE;
				}

				nCount += 2;
			}
			else
			{
				// ���p

				switch( m_ptString[nCount] )
				{
				case	' ':
				case	'\t':
				case	'\r':
				case	'\n':
					// �g���~���O�Ώە���

					break;

				default:
					// ���̑�

					bResult = FALSE;
				}

				nCount += 1;
			}
		}

		break;

	case	2:
		// wchar_t�^

		bResult = TRUE;

		for( nCount = 0 ; nCount < GetLength() ; nCount += 2 )
		{
			switch( m_ptString[nCount] )
			{
			case	L' ':
			case	L'\t':
			case	L'\r':
			case	L'\n':
			case	L'�@':
				// �g���~���O�Ώە���

				break;

			default:
				// ���̑�

				bResult = FALSE;
			}
		}

		break;
	}

	Delete( 0, nCount );

	return	*this;
}

//////////////////////////////////////////////////////////////////////////////////////////
//	�擪�ɂ���󔒕����̍폜

template<class TYPE> YCStringT<TYPE>& YCStringT<TYPE>::TrimLeft( const TYPE* pszTrim )
{

}

//////////////////////////////////////////////////////////////////////////////////////////
//	�擪�ɂ���󔒕����̍폜

template<class TYPE> YCStringT<TYPE>& YCStringT<TYPE>::TrimLeft( const TYPE& rfcTrim )
{

}

//////////////////////////////////////////////////////////////////////////////////////////
//	�����ɂ���󔒕����̍폜

template<class TYPE> YCStringT<TYPE>& YCStringT<TYPE>::TrimRight()
{
	BOOL				bResult;
	int					nIndex;
	int					nTrimIndex;
	int					nCount;

	switch( sizeof(TYPE) )
	{
	case	1:
		// char�^

		bResult = TRUE;
		nIndex = 0;
		nTrimIndex = 0;

		for( nCount = 0 ; nCount < 2 ; nCount++ )
		{
			if( ::IsDBCSLeadByte( m_ptString[nIndex] ) )
			{
				// �S�p
			}
			else
			{
				// ���p

				switch( m_ptString[nIndex] )
				{

				}
			}
		}


		break;

	case	2:
		// wchar_t�^

		break;
	}

}

//////////////////////////////////////////////////////////////////////////////////////////
//	�����ɂ���󔒕����̍폜

template<class TYPE> YCStringT<TYPE>& YCStringT<TYPE>::TrimRight( const TYPE* pszTrim )
{

}

//////////////////////////////////////////////////////////////////////////////////////////
//	�����ɂ���󔒕����̍폜

template<class TYPE> YCStringT<TYPE>& YCStringT<TYPE>::TrimRight( const TYPE& rfcTrim )
{

}

//////////////////////////////////////////////////////////////////////////////////////////
//	�u��
//
//	return	�u��������ꂽ��

template<class TYPE> int YCStringT<TYPE>::Replace(
	const TYPE*			pszOld,							// �u���Ώە���
	const TYPE*			pszNew							// �u����̕���
	)
{
	int					nIndex = 0;
	int					nCount = 0;
	int					nStringLength = GetLength();
	int					nNewStringLength = nStringLength;
	int					nOldLength;
	int					nNewLength;

	switch( sizeof(TYPE) )
	{
	case	1:
		// char�^

		nOldLength = lstrlenA( (const char*) pszOld );
		nNewLength = lstrlenA( (const char*) pszNew );
		break;

	case		2:
		// wchar_t�^

		nOldLength = lstrlenW( (const wchar_t*) pszOld );
		nNewLength = lstrlenW( (const wchar_t*) pszNew );
		break;
	}

	while( 1 )
	{
		nIndex = Find( pszOld, nIndex );

		if( nIndex >= 0 )
		{
			// �u���Ώە����񂪑��݂���

			if( nNewLength == nOldLength )
			{
				// �u�����������̒����͕ς��Ȃ�

				memcpy( &m_ptString[nIndex], pszNew, (sizeof(TYPE) * nNewLength) );
			}
			else
			{
				// �u����͕����񂪑����^��������

				nNewStringLength += (nNewLength - nOldLength);

				ExtendBuffer( (nNewStringLength + 1) );

				memmove( &m_ptString[nIndex + nNewLength], &m_ptString[nIndex + nOldLength], (sizeof(TYPE) * ((nStringLength + 1) - (nIndex + nOldLength))) );

				memcpy( &m_ptString[nIndex], pszNew, (sizeof(TYPE) * nNewLength) );

				nStringLength = nNewStringLength;
			}

			nCount++;
			nIndex += nNewLength;
		}
		else
		{
			// �u���Ώە����񂪑��݂��Ȃ�

			break;
		}
	}

	SetLength( nStringLength );

	return	nCount;
}

//////////////////////////////////////////////////////////////////////////////////////////
//	�u��
//
//	return	�u��������ꂽ��

template<class TYPE> int YCStringT<TYPE>::Replace(
	const TYPE&			rfcOld,							// �u���Ώە���
	const TYPE&			rfcNew							// �u����̕���
	)
{
	int					nIndex = 0;
	int					nCount = 0;
	int					nNewIndex = GetLength();

	while( 1 )
	{
		nIndex = Find( rfcOld, nIndex );

		if( nIndex >= 0 )
		{
			// �u���Ώە��������݂���

			m_ptString[nIndex] = rfcNew;

			nCount++;
		}
		else
		{
			// �u���Ώە��������݂��Ȃ�

			break;
		}
	}

	return	nCount;
}

//////////////////////////////////////////////////////////////////////////////////////////
//	�������ϊ�

template<class TYPE> YCStringT<TYPE>& YCStringT<TYPE>::MakeLower()
{
	::CharLower( m_ptString );

	return	*this;
}

//////////////////////////////////////////////////////////////////////////////////////////
//	�啶���ϊ�

template<class TYPE> YCStringT<TYPE>& YCStringT<TYPE>::MakeUpper()
{
	::CharUpper( m_ptString );

	return	*this;
}

//////////////////////////////////////////////////////////////////////////////////////////
//	��r
//
//	return	0		��v����
//			���̒l	pszSrc��菬����
//			���̒l	pszSrc���傫��

template<class TYPE> int YCStringT<TYPE>::Compare(
	const char*			pszSrc							// ��r������
	) const
{
	int					nReturn;
	TYPE*				ptWork;
	int					nWork;

	switch( sizeof(TYPE) )
	{
	case	1:
		// char�^

		nReturn = lstrcmpA( (const char*) m_ptString, pszSrc );
		break;

	case	2:
		// wchar_t�^

		nWork = GetBaseTypeLength( pszSrc );

		ptWork = new TYPE[nWork];

		ConvertToBaseType( (wchar_t*) ptWork, nWork, pszSrc );

		nReturn = lstrcmpW( (const wchar_t*) m_ptString, (const wchar_t*) ptWork );

		delete[]	ptWork;

		break;
	}

	return	nReturn;
}

//////////////////////////////////////////////////////////////////////////////////////////
//	��r
//
//	return	0		��v����
//			���̒l	rfcSrc��菬����
//			���̒l	rfcSrc���傫��

template<class TYPE> int YCStringT<TYPE>::Compare(
	const char&			rfcSrc							// ��r����
	) const
{
	char				szSrc[2];

	szSrc[0] = rfcSrc;
	szSrc[1] = '\0';

	return	Compare( szSrc );
}

//////////////////////////////////////////////////////////////////////////////////////////
//	��r
//
//	return	0		��v����
//			���̒l	pszSrc��菬����
//			���̒l	pszSrc���傫��

template<class TYPE> int YCStringT<TYPE>::Compare(
	const wchar_t*		pwszSrc							// ��r������
	) const
{
	int					nReturn;
	TYPE*				ptWork;
	int					nWork;

	switch( sizeof(TYPE) )
	{
	case	1:
		// char�^

		nWork = GetBaseTypeLength( pwszSrc );

		ptWork = new TYPE[nWork];

		ConvertToBaseType( (char*) ptWork, nWork, pwszSrc );

		nReturn = lstrcmpA( (const char*) m_ptString, (const char*) ptWork );

		delete[]	ptWork;

		break;

	case	2:
		// wchar_t�^

		nReturn = lstrcmpW( (const wchar_t*) m_ptString, pwszSrc );
		break;
	}

	return	nReturn;
}

//////////////////////////////////////////////////////////////////////////////////////////
//	��r
//
//	return	0		��v����
//			���̒l	rfwcSrc��菬����
//			���̒l	rfwcSrc���傫��

template<class TYPE> int YCStringT<TYPE>::Compare(
	const wchar_t&		rfwcSrc							// ��r����
	) const
{
	wchar_t				wszSrc[2];

	wszSrc[0] = pwszSrc;
	wszSrc[1] = L'\0';

	return	Compare( wszSrc );
}

//////////////////////////////////////////////////////////////////////////////////////////
//	��r
//
//	return	0		��v����
//			���̒l	pszSrc��菬����
//			���̒l	pszSrc���傫��
//
//	���l	�啶���E�������̔�r�Ȃ�

template<class TYPE> int YCStringT<TYPE>::CompareNoCase(
	const char*			pszSrc							// ��r������
	) const
{
	int					nReturn;
	TYPE*				ptWork;
	int					nWork;

	switch( sizeof(TYPE) )
	{
	case	1:
		// char�^

		nReturn = lstrcmpiA( (const char*) m_ptString, pszSrc );
		break;

	case	2:
		// wchar_t�^

		nWork = GetBaseTypeLength( pszSrc );

		ptWork = new TYPE[nWork];

		ConvertToBaseType( (wchar_t*) ptWork, nWork, pszSrc );

		nReturn = lstrcmpiW( (const wchar_t*) m_ptString, (const wchar_t*) ptWork );

		delete[]	ptWork;

		break;
	}

	return	nReturn;
}

//////////////////////////////////////////////////////////////////////////////////////////
//	��r
//
//	return	0		��v����
//			���̒l	rfcSrc��菬����
//			���̒l	rfcSrc���傫��
//
//	���l	�啶���E�������̔�r�Ȃ�

template<class TYPE> int YCStringT<TYPE>::CompareNoCase(
	const char&			rfcSrc							// ��r����
	) const
{
	char				szSrc[2];

	szSrc[0] = rfcSrc;
	szSrc[1] = '\0';

	return	CompareNoCase( szSrc );
}

//////////////////////////////////////////////////////////////////////////////////////////
//	��r
//
//	return	0		��v����
//			���̒l	pszSrc��菬����
//			���̒l	pszSrc���傫��
//
//	���l	�啶���E�������̔�r�Ȃ�

template<class TYPE> int YCStringT<TYPE>::CompareNoCase(
	const wchar_t*		pwszSrc							// ��r������
	) const
{
	int					nReturn;
	TYPE*				ptWork;
	int					nWork;

	switch( sizeof(TYPE) )
	{
	case	1:
		// char�^

		nWork = GetBaseTypeLength( pwszSrc );

		ptWork = new TYPE[nWork];

		ConvertToBaseType( (char*) ptWork, nWork, pwszSrc );

		nReturn = lstrcmpiA( (const char*) m_ptString, (const char*) ptWork );

		delete[]	ptWork;

		break;

	case	2:
		// wchar_t�^

		nReturn = lstrcmpiW( (const wchar_t*) m_ptString, pwszSrc );
		break;
	}

	return	nReturn;
}

//////////////////////////////////////////////////////////////////////////////////////////
//	��r
//
//	return	0		��v����
//			���̒l	rfwcSrc��菬����
//			���̒l	rfwcSrc���傫��
//
//	���l	�啶���E�������̔�r�Ȃ�

template<class TYPE> int YCStringT<TYPE>::CompareNoCase(
	const wchar_t&		rfwcSrc							// ��r����
	) const
{
	wchar_t				wszSrc[2];

	wszSrc[0] = rfwcSrc;
	wszSrc[1] = L'\0';

	return	CompareNoCase( wszSrc );
}

//////////////////////////////////////////////////////////////////////////////////////////
//	�O������
//
//	return	��v�����ʒu
//
//	���l	��v���镶���񂪑��݂��Ȃ��ꍇ�� -1 ��Ԃ�

template<class TYPE> int YCStringT<TYPE>::Find(
	const TYPE*			pszSub,							// �����Ώە�����
	int					nStart							// �����J�n�ʒu
	) const
{
	INT_PTR				npResult = -1;

	if( nStart < 0 )
	{
		// ������̐擪���猟��

		nStart = 0;
	}
	else if( nStart >= GetLength() )
	{
		// ������̍Ōォ�猟��

		return	(int) npResult;
	}

	switch( sizeof(TYPE) )
	{
	case	1:
		// char�^

		const BYTE*			pbtFound;

		pbtFound = _mbsstr( (const BYTE*) &m_ptString[nStart], (const BYTE*) pszSub );

		if( pbtFound != NULL )
		{
			// �Ώە����񂪑��݂���

			npResult = (pbtFound - (BYTE*) m_ptString);
		}

		break;

	case	2:
		// wchar_t�^

		const wchar_t*		pwszFound;

		pwszFound = wcsstr( (const wchar_t*) &m_ptString[nStart], (const wchar_t*) pszSub );

		if( pwszFound != NULL )
		{
			//  �Ώە����񂪑��݂���

			npResult = (pwszFound - (wchar_t*) m_ptString);
		}

		break;
	}

	return	(int) npResult;
}

//////////////////////////////////////////////////////////////////////////////////////////
//	�O������

template<class TYPE> int YCStringT<TYPE>::Find(
	const TYPE&			rfcSub,							// �����Ώە���
	int					nStart							// �����J�n�ʒu
	) const
{
	INT_PTR				npResult = -1;

	if( nStart <  0 )
	{
		// ������̐擪���猟��

		nStart = 0;
	}
	else if( nStart >= GetLength() )
	{
		// ������̍Ōォ�猟��

		return	(int) npResult;
	}

	switch( sizeof(TYPE) )
	{
	case	1:
		// char�^

		const BYTE*			pbtFound;

		pbtFound = _mbschr( (const BYTE*) &m_ptString[nStart], rfcSub );

		if( pbtFound != NULL )
		{
			// �Ώە����񂪑��݂���

			npResult = (pbtFound - (BYTE*) m_ptString);
		}

		break;

	case	2:
		// wchar_t�^

		const wchar_t*		pwszFound;

		pwszFound = wcschr( (const wchar_t*) &m_ptString[nStart], rfcSub );

		if( pwszFound != NULL )
		{
			// �Ώە����񂪑��݂���

			npResult = (pwszFound - (wchar_t*) m_ptString);
		}

		break;
	}

	return	(int) npResult;
}

//////////////////////////////////////////////////////////////////////////////////////////
//	�������
//
//	return	���������ʒu
//
//	���l	�}���`�o�C�g�̏ꍇ�A�������x�͒x�����߁A�o���邾���g�p���Ȃ����Ƃ��]�܂���

template<class TYPE> int YCStringT<TYPE>::ReverseFind(
	const TYPE*			pszSub							// �����Ώە�����
	) const
{
	INT_PTR				npResult = -1;

	// �����Ώە�����̒����̎擾

	int					nSubLength = 0;

	switch( sizeof(TYPE) )
	{
	case	1:
		// char�^

		nSubLength = lstrlenA( (const char*) pszSub );
		break;

	case	2:
		// wchar_t�^

		nSubLength = lstrlenW( (const wchar_t*) pszSub );
		break;
	}

	if( nSubLength <= 0 )
	{
		// �����Ώە�����Ȃ�

		return	(int) npResult;
	}

	// ������̍Ōォ�猟��

	int					nStart = (GetLength() - nSubLength);
	int					nIndex;

	switch( sizeof(TYPE) )
	{
	case	1:
		// char�^

		char*				pszWork;

		pszWork = (char*) &m_ptString[nStart];

		while( 1 )
		{
			if( lstrcmpA( pszWork, (const char*) pszSub ) == 0 )
			{
				// ��v����

				npResult = (pszWork - (char*) m_ptString);
				break;
			}

			if( pszWork == (char*) m_ptString )
			{
				break;
			}

			pszWork = ::CharPrevA( (char*) m_ptString, pszWork );
		}

		break;

	case	2:
		// wchar_t�^

		for( nIndex = nStart ; nIndex >= 0 ; nIndex-- )
		{
			if( lstrcmpW( (wchar_t*) &m_ptString[nIndex], (const wchar_t*) pszSub ) == 0 )
			{
				// ��v����

				npResult = nIndex;
				break;
			}
		}

		break;
	}

	return	(int) npResult;
}

//////////////////////////////////////////////////////////////////////////////////////////
//	�������

template<class TYPE> int YCStringT<TYPE>::ReverseFind(
	const TYPE&			rfcSub							// �����Ώە���
	) const
{
	INT_PTR				npResult = -1;

	switch( sizeof(TYPE) )
	{
	case	1:
		// char�^

		const BYTE*			pbtFound;

		pbtFound = _mbsrchr( (const BYTE*) m_ptString, rfcSub );

		if( pbtFound != NULL )
		{
			// �Ώە����񂪑��݂���

			npResult = (pbtFound - (BYTE*) m_ptString);
		}

		break;

	case	2:
		// wchar_t�^

		const wchar_t*		pwszFound;

		pwszFound = wcsrchr( (wchar_t*) m_ptString, rfcSub );

		if( pwszFound != NULL )
		{
			// �Ώە����񂪑��݂���

			npResult = (pwszFound - (wchar_t*) m_ptString);
		}

		break;
	}

	return	(int) npResult;
}

//////////////////////////////////////////////////////////////////////////////////////////
//	���̕����𒊏o
//
//	return	���o����������

template<class TYPE> YCStringT<TYPE> YCStringT<TYPE>::Left(
	int					nCount							// ���o���镶����
	) const
{
	YCStringT<TYPE>		clsResult = *this;

	clsResult.Delete( nCount, clsResult.GetLength() );

	return	clsResult;
}

//////////////////////////////////////////////////////////////////////////////////////////
//	�E�̕����𒊏o
//
//	return	���o����������

template<class TYPE> YCStringT<TYPE> YCStringT<TYPE>::Right( int nCount ) const
{

}

//////////////////////////////////////////////////////////////////////////////////////////
//	�����̕����𒊏o
//
//	return	���o����������

template<class TYPE> YCStringT<TYPE> YCStringT<TYPE>::Mid( int nFirst ) const
{

}

//////////////////////////////////////////////////////////////////////////////////////////
//	�����̕����𒊏o
//
//	return	���o����������

template<class TYPE> YCStringT<TYPE> YCStringT<TYPE>::Mid( int nFirst, int nCount ) const
{

}








//////////////////////////////////////////////////////////////////////////////////////////
//	�o�b�t�@�̎擾

template<class TYPE> TYPE* YCStringT<TYPE>::GetBuffer(
	int					nBufferSize						// �o�b�t�@�T�C�Y
	)
{
	if( nBufferSize > GetBufferSize() )
	{
		// ���݂̃o�b�t�@�T�C�Y���傫��

		Free();

		// �������̊m��

		Alloc( nBufferSize );
	}

	return	m_ptString;
}

//////////////////////////////////////////////////////////////////////////////////////////
//	�����񒷂̎擾

template<class TYPE> int YCStringT<TYPE>::GetLength() const
{
	if( m_ptString == NULL )
	{
		return	0;
	}

	return	*(int*) (m_ptString - (YCSTRINGT_OFFSET_LENGTH / sizeof(TYPE)));
}

//////////////////////////////////////////////////////////////////////////////////////////
//	������̎擾

template<class TYPE> const TYPE* YCStringT<TYPE>::GetString() const
{
	return	m_ptString;
}

//////////////////////////////////////////////////////////////////////////////////////////
//	�����񂪋󂩂ǂ������ׂ�

template<class TYPE> bool YCStringT<TYPE>::IsEmpty() const
{
	bool				bReturn = false;

	switch( sizeof(TYPE) )
	{
	case	1:
		// char�^

		bReturn = (*this == "");
		break;

	case	2:
		// wchar_t�^

		bReturn = (*this == L"");
		break;
	}

	return	bReturn;
}

//////////////////////////////////////////////////////////////////////////////////////////
//	�g���q�̃��l�[��

template<class TYPE> void YCStringT<TYPE>::RenameExtension(
	const TYPE*			ptRenameExt						// ���l�[����̊g���q
	)
{
	int					nFileNameIndex = GetFileNameIndex();
	int					nFileExtIndex = GetFileExtIndex();

	if( nFileExtIndex >= nFileNameIndex )
	{
		// �g���q�𔭌�

		int					nNewStringLength = GetLength();

		switch( sizeof(TYPE) )
		{
		case	1:
			// char�^

			nNewStringLength += lstrlenA( (const char*) ptRenameExt ) - lstrlenA( (const char*) &m_ptString[nFileExtIndex] );

			ExtendBuffer( (nNewStringLength + 1) );

			lstrcpyA( (char*) &m_ptString[nFileExtIndex], (const char*) ptRenameExt );
			break;

		case	2:
			// wchar_t�^

			nNewStringLength += lstrlenA( (const char*) ptRenameExt ) - lstrlenA( (const char*) &m_ptString[nFileExtIndex] );

			ExtendBuffer( (nNewStringLength + 1) );

			lstrcpyW( (wchar_t*) &m_ptString[nFileExtIndex], (const wchar_t*) ptRenameExt );
			break;
		}

		SetLength( nNewStringLength );
	}
}

//////////////////////////////////////////////////////////////////////////////////////////
//	�t�@�C�����̎擾

template<class TYPE> YCStringT<TYPE> YCStringT<TYPE>::GetFileName() const
{
	int					nIndex = GetFileNameIndex();

	return	&m_ptString[nIndex];
}

//////////////////////////////////////////////////////////////////////////////////////////
//	�t�@�C�����ւ̃C���f�b�N�X�̎擾

template<class TYPE> int YCStringT<TYPE>::GetFileNameIndex() const
{
	int					nFound = -1;
	int					nIndex = 0;

	switch( sizeof(TYPE) )
	{
	case	1:
		// char�^

		nFound = ReverseFind( '\\' );
		break;

	case	2:
		// wchar_t�^

		nFound = ReverseFind( L'\\' );
		break;
	}

	if( nFound >= 0 )
	{
		// �t�@�C�����𔭌�

		nIndex = (nFound + 1);
	}
	else
	{
		// ������S�̂��t�@�C�����ƒf��

		nIndex = 0;
	}

	return	nIndex;
}

//////////////////////////////////////////////////////////////////////////////////////////
//	�t�@�C���^�C�g���̎擾

template<class TYPE> YCStringT<TYPE> YCStringT<TYPE>::GetFileTitle() const
{
	YCStringT<TYPE>		clsFileTitle = GetFileName();
	TYPE				szWork[1];

	switch( sizeof(TYPE) )
	{
	case	1:
		// char�^

		szWork[0] = '\0';
		break;

	case	2:
		// wchar_t�^

		szWork[0] = L'\0';
		break;
	}

	clsFileTitle.RenameExtension( szWork );

	return	clsFileTitle;
}

//////////////////////////////////////////////////////////////////////////////////////////
//	�g���q�̎擾

template<class TYPE> YCStringT<TYPE> YCStringT<TYPE>::GetFileExt() const
{
	int					nIndex = GetFileExtIndex();

	return	&m_ptString[nIndex];
}

//////////////////////////////////////////////////////////////////////////////////////////
//	�g���q�ւ̃C���f�b�N�X�̎擾

template<class TYPE> int YCStringT<TYPE>::GetFileExtIndex() const
{
	int					nFound = -1;
	int					nIndex = 0;

	switch( sizeof(TYPE) )
	{
	case	1:
		// char�^

		nFound = ReverseFind( '.' );
		break;

	case	2:
		// wchar_t�^

		nFound = ReverseFind( L'.' );
		break;
	}

	if( nFound >= 0 )
	{
		// �g���q�𔭌�

		nIndex = nFound;
	}
	else
	{
		// �g���q���Ȃ�

		nIndex = GetLength();
	}

	return	nIndex;
}

//////////////////////////////////////////////////////////////////////////////////////////
//	�t�H���_�p�X�̎擾

template<class TYPE> YCStringT<TYPE> YCStringT<TYPE>::GetDirPath() const
{
	int					nTargetIndex = 0;
	int					nFileNameIndex = GetFileNameIndex();
	int					nDriveIndex = -1;

	if( nFileNameIndex >= 1 )
	{
		// �t�H���_���𔭌�

		nTargetIndex = (nFileNameIndex - 1);
	}

	switch( sizeof(TYPE) )
	{
	case	1:
		// char�^

		nDriveIndex = ReverseFind( ':' );
		break;

	case	2:
		// wchar_t�^

		nDriveIndex = ReverseFind( L':' );
		break;
	}

	if( nDriveIndex >= 0 )
	{
		// �h���C�u�����܂܂�Ă���

		if( nTargetIndex == (nDriveIndex + 1) )
		{
			// ':'�̒����'\\'�����݂���

			nTargetIndex = nFileNameIndex;
		}
		else
		{
			// ':'���O�͍폜���Ȃ�

			nTargetIndex = (nTargetIndex < nDriveIndex) ? (nDriveIndex + 1) : nTargetIndex;
		}
	}

	return	Left( nTargetIndex );
}

//////////////////////////////////////////////////////////////////////////////////////////
//	������Z�q

template<class TYPE> YCStringT<TYPE>& YCStringT<TYPE>::operator=(
	const char*			pszSrc							// ���������
	)
{
	Copy( pszSrc );

	return	*this;
}

//////////////////////////////////////////////////////////////////////////////////////////
//	������Z�q

template<class TYPE> YCStringT<TYPE>& YCStringT<TYPE>::operator=(
	const char&			rfcSrc							// �������
	)
{
	Copy( rfcSrc );

	return	*this;
}

//////////////////////////////////////////////////////////////////////////////////////////
//	������Z�q

template<class TYPE> YCStringT<TYPE>& YCStringT<TYPE>::operator=(
	const wchar_t*		pwszSrc							// ���������
	)
{
	Copy( pwszSrc );

	return	*this;
}

//////////////////////////////////////////////////////////////////////////////////////////
//	������Z�q

template<class TYPE> YCStringT<TYPE>& YCStringT<TYPE>::operator=(
	const wchar_t&		rfwcSrc							// �������
	)
{
	Copy( rfwcSrc );

	return	*this;
}

//////////////////////////////////////////////////////////////////////////////////////////
//	������Z�q

template<class TYPE> YCStringT<TYPE>& YCStringT<TYPE>::operator=(
	const YCStringT<TYPE>&	rfclsSrc					// ���������
	)
{
	Copy( rfclsSrc );

	return	*this;
}

//////////////////////////////////////////////////////////////////////////////////////////
//	+���Z�q

template<class TYPE> YCStringT<TYPE> YCStringT<TYPE>::operator+(
	const char*			pszAppend						// �q���镶����
	) const
{
	YCStringT<TYPE>		clsResult = *this;

	clsResult.Append( pszAppend );

	return	clsResult;
}

//////////////////////////////////////////////////////////////////////////////////////////
//	+���Z�q

template<class TYPE> YCStringT<TYPE> YCStringT<TYPE>::operator+(
	const char&			rfcAppend						// �q���镶��
	) const
{
	YCStringT<TYPE>		clsResult = *this;

	clsResult.Append( rfcAppend );

	return	clsResult;
}

//////////////////////////////////////////////////////////////////////////////////////////
//	+���Z�q

template<class TYPE> YCStringT<TYPE> YCStringT<TYPE>::operator+(
	const wchar_t*		pwszAppend						// �q���镶����
	) const
{
	YCStringT<TYPE>		clsResult = *this;

	clsResult.Append( pwszAppend );

	return	clsResult;
}

//////////////////////////////////////////////////////////////////////////////////////////
//	+���Z�q

template<class TYPE> YCStringT<TYPE> YCStringT<TYPE>::operator+(
	const wchar_t&		rfwcAppend						// �q���镶��
	) const
{
	YCStringT<TYPE>		clsResult = *this;

	clsResult.Append( rfwcAppend );

	return	clsResult;
}

//////////////////////////////////////////////////////////////////////////////////////////
//	+���Z�q

template<class TYPE> YCStringT<TYPE> YCStringT<TYPE>::operator+(
	const YCStringT<TYPE>&	rfclsAppend					// �q���镶����
	) const
{
	YCStringT<TYPE>		clsResult = *this;

	clsResult.Append( rfclsAppend );

	return	clsResult;
}

//////////////////////////////////////////////////////////////////////////////////////////
//	+���Z�q(�O���֐�)

template<class TYPE> YCStringT<TYPE> operator+(
	const char*				pszSrc,						// �q�����镶����
	const YCStringT<TYPE>&	rfclsAppend					// �q���镶����
	)
{
	YCStringT<TYPE>		clsResult = pszSrc;

	clsResult.Append( rfclsAppend );

	return	clsResult;
}

//////////////////////////////////////////////////////////////////////////////////////////
//	+���Z�q(�O���֐�)

template<class TYPE> YCStringT<TYPE> operator+(
	const char&				rfcSrc,						// �q�����镶��
	const YCStringT<TYPE>&	rfclsAppend					// �q���镶����
	)
{
	YCStringT<TYPE>		clsResult = rfcSrc;

	clsResult.Append( rfclsAppend );

	return	clsResult;
}

//////////////////////////////////////////////////////////////////////////////////////////
//	+���Z�q(�O���֐�)

template<class TYPE> YCStringT<TYPE> operator+(
	const wchar_t*			pwszSrc,					// �q�����镶����
	const YCStringT<TYPE>&	rfclsAppend					// �q���镶����
	)
{
	YCStringT<TYPE>		clsResult = pwszSrc;

	clsResult.Append( rfclsAppend );

	return	clsResult;
}

//////////////////////////////////////////////////////////////////////////////////////////
//	+���Z�q(�O���֐�)

template<class TYPE> YCStringT<TYPE> operator+(
	const wchar_t&			rfwcSrc,					// �q�����镶��
	const YCStringT<TYPE>&	rfclsAppend					// �q���镶����
	)
{
	YCStringT<TYPE>		clsResult = rfwcSrc;

	clsResult.Append( rfclsAppend );

	return	clsResult;
}

//////////////////////////////////////////////////////////////////////////////////////////
//	+=���Z�q

template<class TYPE> YCStringT<TYPE>& YCStringT<TYPE>::operator+=(
	const char*			pszAppend						// �q���镶����
	)
{
	Append( pszAppend );

	return	*this;
}

//////////////////////////////////////////////////////////////////////////////////////////
//	+=���Z�q

template<class TYPE> YCStringT<TYPE>& YCStringT<TYPE>::operator+=(
	const char&			rfcAppend						// �q���镶��
	)
{
	Append( rfcAppend );

	return	*this;
}

//////////////////////////////////////////////////////////////////////////////////////////
//	+=���Z�q

template<class TYPE> YCStringT<TYPE>& YCStringT<TYPE>::operator+=(
	const wchar_t*		pwszAppend						// �q���镶����
	)
{
	Append( pwszAppend );

	return	*this;
}

//////////////////////////////////////////////////////////////////////////////////////////
//	+=���Z�q

template<class TYPE> YCStringT<TYPE>& YCStringT<TYPE>::operator+=(
	const wchar_t&		rfwcAppend						// �q���镶��
	)
{
	Append( rfwcAppend );

	return	*this;
}

//////////////////////////////////////////////////////////////////////////////////////////
//	+=���Z�q

template<class TYPE> YCStringT<TYPE>& YCStringT<TYPE>::operator+=(
	const YCStringT<TYPE>&	rfclsAppend					// �q���镶����
	)
{
	Append( rfclsAppend );

	return	*this;
}

//////////////////////////////////////////////////////////////////////////////////////////
//	==���Z�q

template<class TYPE> bool YCStringT<TYPE>::operator==(
	const char*			pszSrc							// ��r������
	) const
{
	return	(Compare( pszSrc ) == 0);
}

//////////////////////////////////////////////////////////////////////////////////////////
//	==���Z�q

template<class TYPE> bool YCStringT<TYPE>::operator==(
	const char&			rfcSrc							// ��r����
	) const
{
	return	(Compare( rfcSrc ) == 0);
}

//////////////////////////////////////////////////////////////////////////////////////////
//	==���Z�q

template<class TYPE> bool YCStringT<TYPE>::operator==(
	const wchar_t*			pwszSrc						// ��r������
	) const
{
	return	(Compare( pwszSrc ) == 0);
}

//////////////////////////////////////////////////////////////////////////////////////////
//	==���Z�q

template<class TYPE> bool YCStringT<TYPE>::operator==(
	const wchar_t&		rfwcSrc							// ��r����
	) const
{
	return	(Compare( rfwcSrc ) == 0);
}

//////////////////////////////////////////////////////////////////////////////////////////
//	==���Z�q

template<class TYPE> bool YCStringT<TYPE>::operator==(
	const YCStringT<TYPE>&	rfclsSrc					// ��r������
	) const
{
	return	(Compare( rfclsSrc ) == 0);
}

//////////////////////////////////////////////////////////////////////////////////////////
//	!=���Z�q

template<class TYPE> bool YCStringT<TYPE>::operator!=(
	const char*			pszSrc							// ��r������
	) const
{
	return	(Compare( pszSrc ) != 0);
}

//////////////////////////////////////////////////////////////////////////////////////////
//	!=���Z�q

template<class TYPE> bool YCStringT<TYPE>::operator!=(
	const char&			rfcSrc							// ��r����
	) const
{
	return	(Compare( rfcSrc ) != 0);
}

//////////////////////////////////////////////////////////////////////////////////////////
//	!=���Z�q

template<class TYPE> bool YCStringT<TYPE>::operator!=(
	const wchar_t*			pwszSrc						// ��r������
	) const
{
	return	(Compare( pwszSrc ) != 0);
}

//////////////////////////////////////////////////////////////////////////////////////////
//	!=���Z�q

template<class TYPE> bool YCStringT<TYPE>::operator!=(
	const wchar_t&		rfwcSrc							// ��r����
	) const
{
	return	(Compare( rfwcSrc ) != 0);
}

//////////////////////////////////////////////////////////////////////////////////////////
//	!=���Z�q

template<class TYPE> bool YCStringT<TYPE>::operator!=(
	const YCStringT<TYPE>&	rfclsSrc					// ��r������
	) const
{
	return	(Compare( rfclsSrc ) != 0);
}

//////////////////////////////////////////////////////////////////////////////////////////
//	<���Z�q
//
//	return	TRUE	pszSrc����
//			FALSE	pszSrc�ȏ�

template<class TYPE> bool YCStringT<TYPE>::operator<(
	const char*			pszSrc							// ��r������
	) const
{
	return	(Compare( pszSrc ) < 0);
}

//////////////////////////////////////////////////////////////////////////////////////////
//	<���Z�q
//
//	return	TRUE	pwszSrc����
//			FALSE	pwszSrc�ȏ�

template<class TYPE> bool YCStringT<TYPE>::operator<(
	const wchar_t*		pwszSrc							// ��r������
	) const
{
	return	(Compare( pwszSrc ) < 0);
}

//////////////////////////////////////////////////////////////////////////////////////////
//	<���Z�q
//
//	return	TRUE	rfclsSrc����
//			FALSE	rfclsSrc�ȏ�

template<class TYPE> bool YCStringT<TYPE>::operator<(
	const YCStringT<TYPE>&	rfclsSrc					// ��r������
	) const
{
	return	(Compare( rfclsSrc ) < 0);
}

//////////////////////////////////////////////////////////////////////////////////////////
//	<���Z�q(�O���֐�)
//
//	return	TRUE	rfclsSrc����
//			FALSE	rfclsSrc�ȏ�

template<class TYPE> bool operator<(
	const char*				pszSrc,						// ��r������
	const YCStringT<TYPE>&	rfclsSrc					// ��r������
	)
{
	return	(rfclsSrc.Compare( pszSrc ) >= 0);
}

//////////////////////////////////////////////////////////////////////////////////////////
//	<���Z�q(�O���֐�)
//
//	return	TRUE	rfclsSrc����
//			FALSE	rfclsSrc�ȏ�

template<class TYPE> bool operator<(
	const wchar_t*			pwszSrc,					// ��r������
	const YCStringT<TYPE>&	rfclsSrc					// ��r������
	)
{
	return	(rfclsSrc.Compare( pwszSrc ) >= 0);
}

//////////////////////////////////////////////////////////////////////////////////////////
//	<=���Z�q
//
//	return	TRUE	pszSrc�ȉ�
//			FALSE	pszSrc��

template<class TYPE> bool YCStringT<TYPE>::operator<=(
	const char*			pszSrc							// ��r������
	) const
{
	return	(Compare( pszSrc ) <= 0);
}

//////////////////////////////////////////////////////////////////////////////////////////
//	<=���Z�q
//
//	return	TRUE	pwszSrc�ȉ�
//			FALSE	pwszSrc��

template<class TYPE> bool YCStringT<TYPE>::operator<=(
	const wchar_t*		pwszSrc							// ��r������
	) const
{
	return	(Compare( pwszSrc ) <= 0);
}

//////////////////////////////////////////////////////////////////////////////////////////
//	<=���Z�q
//
//	return	TRUE	rfclsSrc�ȉ�
//			FALSE	rfclsSrc��

template<class TYPE> bool YCStringT<TYPE>::operator<=(
	const YCStringT<TYPE>&	rfclsSrc					// ��r������
	) const
{
	return	(Compare( rfclsSrc ) <= 0);
}

//////////////////////////////////////////////////////////////////////////////////////////
//	<=���Z�q(�O���֐�)
//
//	return	TRUE	rfclsSrc�ȉ�
//			FALSE	rfclsSrc��

template<class TYPE> bool operator<=(
	const char*				pszSrc,						// ��r������
	const YCStringT<TYPE>&	rfclsSrc					// ��r������
	)
{
	return	(rfclsSrc.Compare( pszSrc ) > 0);
}

//////////////////////////////////////////////////////////////////////////////////////////
//	<=���Z�q(�O���֐�)
//
//	return	TRUE	rfclsSrc�ȉ�
//			FALSE	rfclsSrc��

template<class TYPE> bool operator<=(
	const wchar_t*			pwszSrc,					// ��r������
	const YCStringT<TYPE>&	rfclsSrc					// ��r������
	)
{
	return	(rfclsSrc.Compare( pwszSrc ) > 0);
}

//////////////////////////////////////////////////////////////////////////////////////////
//	>���Z�q
//
//	return	TRUE	pszSrc��
//			FALSE	pszSrc�ȉ�

template<class TYPE> bool YCStringT<TYPE>::operator>(
	const char*			pszSrc							// ��r������
	) const
{
	return	(Compare( pszSrc ) > 0);
}

//////////////////////////////////////////////////////////////////////////////////////////
//	>���Z�q
//
//	return	TRUE	pwszSrc��
//			FALSE	pwszSrc�ȉ�

template<class TYPE> bool YCStringT<TYPE>::operator>(
	const wchar_t*		pwszSrc							// ��r������
	) const
{
	return	(Compare( pwszSrc ) > 0);
}

//////////////////////////////////////////////////////////////////////////////////////////
//	>���Z�q
//
//	return	TRUE	rfclsSrc��
//			FALSE	rfclsSrc�ȉ�

template<class TYPE> bool YCStringT<TYPE>::operator>(
	const YCStringT<TYPE>&	rfclsSrc					// ��r������
	) const
{
	return	(Compare( rfclsSrc ) > 0);
}

//////////////////////////////////////////////////////////////////////////////////////////
//	>���Z�q(�O���֐�)
//
//	return	TRUE	rfclsSrc��
//			FALSE	rfclsSrc�ȉ�

template<class TYPE> bool operator>(
	const char*				pszSrc,						// ��r������
	const YCStringT<TYPE>&	rfclsSrc					// ��r������
	)
{
	return	(rfclsSrc.Compare( pszSrc ) <= 0);
}

//////////////////////////////////////////////////////////////////////////////////////////
//	>���Z�q(�O���֐�)
//
//	return	TRUE	rfclsSrc��
//			FALSE	rfclsSrc�ȉ�

template<class TYPE> bool operator>(
	const wchar_t*			pwszSrc,					// ��r������
	const YCStringT<TYPE>&	rfclsSrc					// ��r������
	)
{
	return	(rfclsSrc.Compare( pszSrc ) <= 0);
}

//////////////////////////////////////////////////////////////////////////////////////////
//	>=���Z�q
//
//	return	TRUE	pszSrc�ȉ�
//			FALSE	pszSrc��

template<class TYPE> bool YCStringT<TYPE>::operator>=(
	const char*			pszSrc							// ��r������
	) const
{
	return	(Compare( pszSrc ) >= 0);
}

//////////////////////////////////////////////////////////////////////////////////////////
//	>=���Z�q
//
//	return	TRUE	pwszSrc�ȉ�
//			FALSE	pwszSrc��

template<class TYPE> bool YCStringT<TYPE>::operator>=(
	const wchar_t*		pwszSrc							// ��r������
	) const
{
	return	(Compare( pwszSrc ) >= 0);
}

//////////////////////////////////////////////////////////////////////////////////////////
//	>=���Z�q
//
//	return	TRUE	rfclsSrc�ȉ�
//			FALSE	rfclsSrc��

template<class TYPE> bool YCStringT<TYPE>::operator>=( 
	const YCStringT<TYPE>&	rfclsSrc					// ��r������
	) const
{
	return	(Compare( rfclsSrc ) >= 0);
}

//////////////////////////////////////////////////////////////////////////////////////////
//	>=���Z�q(�O���֐�)
//
//	return	TRUE	rfclsSrc�ȉ�
//			FALSE	rfclsSrc��

template<class TYPE> bool operator>=(
	const char*				pszSrc,						// ��r������
	const YCStringT<TYPE>&	rfclsSrc					// ��r������
	)
{
	return	(rfclsSrc.Compare( pszSrc ) < 0);
}

//////////////////////////////////////////////////////////////////////////////////////////
//	>=���Z�q(�O���֐�)
//
//	return	TRUE	rfclsSrc�ȉ�
//			FALSE	rfclsSrc��

template<class TYPE> bool operator>=(
	const wchar_t*			pwszSrc,					// ��r������
	const YCStringT<TYPE>&	rfclsSrc					// ��r������
	)
{
	return	(rfclsSrc.Compare( pwszSrc ) < 0);
}

//////////////////////////////////////////////////////////////////////////////////////////
//	�v�f�ɃA�N�Z�X
//
//	���l	�͈͊O�̎Q�Ƃɑ΂��ĉ������Ȃ����ߍ��������A������댯��������̂Œ���

template<class TYPE> TYPE& YCStringT<TYPE>::operator[](
	int					nPos							// �Q�ƈʒu
	)
{
	return	m_ptString[nPos];
}

//////////////////////////////////////////////////////////////////////////////////////////
//	�v�f�̎Q��
//
//	���l	�͈͊O�̎Q�Ƃɑ΂��ĉ������Ȃ����ߍ��������A������댯��������̂Œ���

template<class TYPE> const TYPE& YCStringT<TYPE>::operator[](
	int					nPos							// �Q�ƈʒu
	) const
{
	return	m_ptString[nPos];
}

//////////////////////////////////////////////////////////////////////////////////////////
//	�������Ԃ�

template<class TYPE> YCStringT<TYPE>::operator const TYPE*() const
{
	return	GetString();
}

//////////////////////////////////////////////////////////////////////////////////////////
//	�������̊m��

template<class TYPE> void YCStringT<TYPE>::Alloc(
	int					nBufferSize						// �o�b�t�@�T�C�Y
	)
{
	// �A���C�����g

	int					nPaddingSize = (YCSTRINGT_BUFFERSIZE_ALIGNMENT - (nBufferSize % YCSTRINGT_BUFFERSIZE_ALIGNMENT));

	if( nPaddingSize < YCSTRINGT_BUFFERSIZE_ALIGNMENT )
	{
		nBufferSize += nPaddingSize;
	}

	// �o�b�t�@�̊m��

	int					nDataSize = (16 / sizeof(TYPE));

	m_ptString = new TYPE[nDataSize + nBufferSize];

	m_ptString += nDataSize;

	// �o�b�t�@�T�C�Y�̊i�[

	SetBufferSize( nBufferSize );
}

//////////////////////////////////////////////////////////////////////////////////////////
//	�������̉��

template<class TYPE> void YCStringT<TYPE>::Free()
{
	if( m_ptString != NULL )
	{
		m_ptString -= (16 / sizeof(TYPE));

		delete[]	m_ptString;

		m_ptString = NULL;
	}
}

//////////////////////////////////////////////////////////////////////////////////////////
//	�o�b�t�@�̊g��

template<class TYPE> void YCStringT<TYPE>::ExtendBuffer(
	int					nBufferSize						// �o�b�t�@�T�C�Y
	)
{
	if( nBufferSize < GetBufferSize() )
	{
		// �o�b�t�@���g������K�v�Ȃ�

		return;
	}

	TYPE*				ptString = m_ptString;

	switch( sizeof(TYPE) )
	{
	case	1:
		// char�^

		Alloc( nBufferSize );

		lstrcpyA( (char*) m_ptString, (const char*) ptString );

		break;

	case	2:
		// wchar_t�^

		Alloc( nBufferSize );

		lstrcpyW( (wchar_t*) m_ptString, (const wchar_t*) ptString );

		break;
	}

	// �O�̃o�b�t�@�̉��

	delete[]	(ptString - (16 / sizeof(TYPE)));
}

//////////////////////////////////////////////////////////////////////////////////////////
//	�o�b�t�@�T�C�Y�̐ݒ�

template<class TYPE> void YCStringT<TYPE>::SetBufferSize(
	int					nBufferSize						// �o�b�t�@�T�C�Y
	)
{
	*(int*) (m_ptString - (YCSTRINGT_OFFSET_BUFFERSIZE / sizeof(TYPE))) = nBufferSize;
}

//////////////////////////////////////////////////////////////////////////////////////////
//	�o�b�t�@�T�C�Y�̎擾	
//
//	return	�o�b�t�@�T�C�Y

template<class TYPE> int YCStringT<TYPE>::GetBufferSize()
{
	if( m_ptString == NULL )
	{
		return	0;
	}

	return	*(int*) (m_ptString - (YCSTRINGT_OFFSET_BUFFERSIZE / sizeof(TYPE)));
}

//////////////////////////////////////////////////////////////////////////////////////////
//	������̒����̐ݒ�

template<class TYPE> void YCStringT<TYPE>::SetLength(
	int					nLength							// ������̒���
	)
{
	*(int*) (m_ptString - (YCSTRINGT_OFFSET_LENGTH / sizeof(TYPE))) = nLength;
}


