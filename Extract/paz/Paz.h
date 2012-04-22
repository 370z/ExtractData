
#pragma	once

#include	"../../ExtractBase.h"

class	CPaz : public CExtractBase
{
public:

	// �L�[���

	struct	SKeyInfo
	{
		YCString			clsType;
		BYTE				abtKey[64];
	};


	virtual	BOOL							Mount( CArcFile* pclArc );
	virtual	BOOL							Decode( CArcFile* pclArc );


protected:

	// 72 + 4096�o�C�g�̃e�[�u���̏�����

	virtual	void							InitTable();

	// ���[�r�[�e�[�u���̏�����

	virtual	DWORD							InitMovieTable( void* pvTable );

	// �x�[�X�A�[�J�C�u�t�@�C�����̎擾

	void									GetBaseArcName( LPTSTR pszDst, LPCTSTR pszArcName );

	// 32�o�C�g�̃L�[���Z�b�g����(���Ƀt�@�C���ɂ���ăL�[�͈قȂ�)

	virtual	void							InitMountKey( CArcFile* pclArc ) = 0;
	virtual	void							InitDecodeKey( CArcFile* pclArc ) = 0;
	DWORD									SetKey( CArcFile* pclArc, const SKeyInfo* pstKeyInfo );

	// �e�[�u���𕜍�����

	virtual	void							DecodeTable1();
	virtual	void							DecodeTable2();

	// �ŗL�Í��𕜍�����

	virtual	void							Decrypt( void* pvTarget, DWORD dwSize ) {}
	virtual	void							Decrypt2( void* pvTarget, DWORD dwSize ) {}

	// �f�[�^�𕜍�����

	virtual	void							DecodeData( void* pvTarget, DWORD dwSize );

	// ���[�r�[�f�[�^�𕜍�����

	virtual	void							DecodeMovieData( void* pvTarget, DWORD dwSize );

	// DWORD�l�𕜍�����

	virtual	DWORD							DecodeValueByTable( DWORD dwValue, void* pvTable );
	virtual	void							DecodeValue( DWORD* pdwValue1, DWORD* pdwValue2, void* pvTable );

	// 

	virtual	DWORD*							GetTable();
	virtual	BYTE*							GetMovieTable();

	virtual	BYTE*							GetKey();
	virtual	DWORD							GetMovieBufSize( CArcFile* pclArc );

private:

	DWORD				m_adwTable[1042];
	BYTE				m_abtMovieTable[256];
	BYTE				m_abtKey[32];
};
