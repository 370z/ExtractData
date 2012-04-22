
#pragma	once

//#define	YCMemory	std::valarray

//----------------------------------------------------------------------------------------
//-- �������Ǘ��N���X --------------------------------------------------------------------
//----------------------------------------------------------------------------------------

// std::vector��std::valarray���o�b�t�@�Ƃ��ė��p���Ă��镔����u��������

template<class TYPE>
class	YCMemory
{
public:

											YCMemory();
											YCMemory( DWORD dwCount );
											YCMemory( DWORD dwCount, const TYPE& rftInitValue );
											YCMemory( const YCMemory<TYPE>& rfclmSrc );
											~YCMemory();

	void									resize( DWORD dwCount );
	void									resize( DWORD dwCount, const TYPE& rftInitValue );
	void									clear();

	inline	DWORD							size() const;
	TYPE									at( DWORD dwPos ) const;

	inline	TYPE&							operator[]( DWORD dwPos );
	inline	const TYPE&						operator[]( DWORD dwPos ) const;
	inline	YCMemory<TYPE>&					operator=( const YCMemory<TYPE>& rfclmSrc );


protected:

	void									alloc( DWORD dwCount );
	inline	void							fill( const TYPE& rftValue );

private:

	TYPE*									m_ptMemory;
	DWORD									m_dwMemoryCount;
};

//////////////////////////////////////////////////////////////////////////////////////////
//	�R���X�g���N�^

template<class TYPE> YCMemory<TYPE>::YCMemory()
{
	m_ptMemory = NULL;
	m_dwMemoryCount = 0;
}

//////////////////////////////////////////////////////////////////////////////////////////
//	�R���X�g���N�^

template<class TYPE> YCMemory<TYPE>::YCMemory(
	DWORD				dwCount							// �v�f��
	)
{
	alloc( dwCount );
}

//////////////////////////////////////////////////////////////////////////////////////////
//	�R���X�g���N�^

template<class TYPE> YCMemory<TYPE>::YCMemory(
	DWORD				dwCount,						// �v�f��
	const TYPE&			rftInitValue					// �����l
	)
{
	alloc( dwCount );

	fill( rftInitValue );
}

//////////////////////////////////////////////////////////////////////////////////////////
//	�R�s�[�R���X�g���N�^

template<class TYPE> YCMemory<TYPE>::YCMemory(
	const YCMemory<TYPE>&	rfclmSrc					// �R�s�[���������N���X
	)
{
	alloc( rfclmSrc.size() );

	memcpy( m_ptMemory, &rfclmSrc[0], sizeof(TYPE) * rfclmSrc.size() );
}

//////////////////////////////////////////////////////////////////////////////////////////
//	�f�X�g���N�^

template<class TYPE> YCMemory<TYPE>::~YCMemory()
{
	clear();
}

//////////////////////////////////////////////////////////////////////////////////////////
//	�������T�C�Y�̕ύX

template<class TYPE> void YCMemory<TYPE>::resize(
	DWORD				dwCount							// �v�f��
	)
{
	clear();

	alloc( dwCount );
}

//////////////////////////////////////////////////////////////////////////////////////////
//	�������T�C�Y�̕ύX

template<class TYPE> void YCMemory<TYPE>::resize(
	DWORD				dwCount,						// �v�f��
	const TYPE&			rftInitValue					// �����l
	)
{
	clear();

	alloc( dwCount );

	fill( rftInitValue );
}

//////////////////////////////////////////////////////////////////////////////////////////
//	�������̉��

template<class TYPE> void YCMemory<TYPE>::clear()
{
	if( m_ptMemory != NULL )
	{
		delete[]	m_ptMemory;
		m_ptMemory = NULL;
		m_dwMemoryCount = 0;
	}
}

//////////////////////////////////////////////////////////////////////////////////////////
//	�v�f���̎擾

template<class TYPE> DWORD YCMemory<TYPE>::size() const
{
	return	m_dwMemoryCount;
}

//////////////////////////////////////////////////////////////////////////////////////////
//	�v�f�̎Q��
//
//	���l	�͈͊O�̎Q�Ƃɑ΂��āustd::out_of_range�v��O�𓊂���

template<class TYPE> TYPE YCMemory<TYPE>::at(
	DWORD				dwPos							// �Q�ƈʒu
	) const
{
	if( dwPos >= m_dwMemorySize )
	{
		// �͈͊O�̎Q��

		throw	std::out_of_range;
	}

	return	m_ptMemory[dwPos];
}

//////////////////////////////////////////////////////////////////////////////////////////
//	�v�f�ɃA�N�Z�X
//
//	���l	�͈͊O�̎Q�Ƃɑ΂��ĉ������Ȃ����ߍ��������A������댯��������̂Œ���

template<class TYPE> TYPE& YCMemory<TYPE>::operator[](
	DWORD				dwPos							// �Q�ƈʒu
	)
{
	return	m_ptMemory[dwPos];
}

//////////////////////////////////////////////////////////////////////////////////////////
//	�v�f�̎Q��
//
//	���l	�͈͊O�̎Q�Ƃɑ΂��ĉ������Ȃ����ߍ��������A������댯��������̂Œ���

template<class TYPE> const TYPE& YCMemory<TYPE>::operator[](
	DWORD				dwPos							// �Q�ƈʒu
	) const
{
	return	m_ptMemory[dwPos];
}

//////////////////////////////////////////////////////////////////////////////////////////
//	�R�s�[

template<class TYPE> YCMemory<TYPE>& YCMemory<TYPE>::operator=(
	const YCMemory<TYPE>&	rfclmSrc					// �R�s�[���������N���X
	)
{
	resize( rfclmSrc.size() );

	memcpy( m_ptMemory, &rfclmSrc[0], rfclmSrc.size() );

	return	*this;
}

//////////////////////////////////////////////////////////////////////////////////////////
//	�������̊m��

template<class TYPE> void YCMemory<TYPE>::alloc(
	DWORD				dwCount							// �v�f��
	)
{
	if( dwCount == 0 )
	{
		// 0�o�C�g�̃������m�ۂ̑Ώ�

		dwCount = 1;
	}

	m_ptMemory = new TYPE[dwCount];
	m_dwMemoryCount = dwCount;
}

//////////////////////////////////////////////////////////////////////////////////////////
//	�������𖄂߂�

template<class TYPE> void YCMemory<TYPE>::fill(
	const TYPE&			rftValue						// �l
	)
{
	for( DWORD i = 0 ; i < size() ; i++ )
	{
		m_ptMemory[i] = rftValue;
	}
}
