
#pragma		once

#include	"../../ExtractBase.h"
#include	"../../MD5.h"

class	CKrkr : public CExtractBase
{
public:

	// �t�@�C���`�����N

	struct	FileChunk
	{
		BYTE			name[4];
		QWORD			size;
	};

	// �C���t�H�`�����N

	struct	InfoChunk
	{
		BYTE			name[4];
		QWORD			size;
		DWORD			protect;	// (1 << 31) : protected
		QWORD			orgSize;
		QWORD			arcSize;
		WORD			nameLen;
		wchar_t*		filename;	// length : nameLen, Unicode
	};

	// �Z�O�����g�`�����N

	struct	SegmChunk
	{
		BYTE			name[4];
		QWORD			size;
		DWORD			comp;		// 1 : compressed
		QWORD			start;		// �A�[�J�C�u�擪����f�[�^�܂ł̃I�t�Z�b�g
		QWORD			orgSize;	// original size
		QWORD			arcSize;	// archived size
	};

	// adlr�`�����N

	struct	AdlrChunk
	{
		BYTE			name[4];
		QWORD			size;
		DWORD			key;		// �t�@�C���ŗL�L�[
	};

	virtual	BOOL		Mount( CArcFile* pclArc );
	virtual	BOOL		Decode( CArcFile* pclArc );
	virtual	BOOL		Extract( CArcFile* pclArc );


protected:

	// �����\���ǂ����̊m�F

	virtual BOOL		OnCheckDecrypt( CArcFile* pclArc );

	// tpm�̃`�F�b�N

	BOOL				CheckTpm( const char* pszMD5 );

	// �����L�[�̃Z�b�g

	void				InitDecrypt( CArcFile* pclArc );
	virtual DWORD		OnInitDecrypt( CArcFile* pclArc );

	// �������s��

	DWORD				Decrypt( BYTE* pvTarget, DWORD dwTargetSize, DWORD dwOffset );
	virtual DWORD		OnDecrypt( BYTE* pvTarget, DWORD dwTargetSize, DWORD dwOffset, DWORD dwDecryptKey );

	// �����v���̐ݒ�

	void									SetDecryptRequirement( BOOL bDecrypt );

	// �����T�C�Y�̐ݒ�

	void									SetDecryptSize( DWORD dwDecryptSize );

	// EXE������XP3��T��

	BOOL				FindXP3FromExecuteFile( CArcFile* pclArc, DWORD* pdwOffset );


private:

	BOOL									m_bDecrypt;			// �����v��
	DWORD									m_dwDecryptKey;
	DWORD									m_dwDecryptSize;	// �����T�C�Y
	YCString								m_clsTpmPath;
	CArcFile*								m_pclArc;


private:

	void				SetMD5ForTpm( CArcFile* pclArc );
};
