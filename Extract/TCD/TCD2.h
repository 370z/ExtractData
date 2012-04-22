
#pragma		once

//----------------------------------------------------------------------------------------
//-- TCD2�N���X --------------------------------------------------------------------------
//----------------------------------------------------------------------------------------

#include	"TCDBase.h"

class	CTCD2 : public CTCDBase
{
public:

	struct	STCD2IndexInfo
	{
		DWORD				dwFileSize;					// �t�@�C���T�C�Y
		DWORD				dwFileCount;				// �t�@�C����
		DWORD				dwDirCount;					// �t�H���_��
		DWORD				dwIndexOffset;				// �C���f�b�N�X�ւ̐�΃I�t�Z�b�g�l
		DWORD				dwDirNameLength;			// �t�H���_���̒���
		DWORD				dwFileNameLength;			// �t�@�C�����̒���
		DWORD				dwFileCountEx;				// �t�@�C����(�g����)
		DWORD				dwFileNameLengthEx;			// �t�@�C�����̒���(�g����)
	};

	struct	STCD2DirInfo
	{
		DWORD				dwFileCount;				// �t�@�C����
		DWORD				dwFileNameOffset;			// �t�@�C�����ւ̃I�t�Z�b�g�l
		DWORD				dwFileOffset;				// �t�@�C���ւ̃I�t�Z�b�g�l
		DWORD				dwReserve;					// ���U�[�u
	};

	virtual	BOOL							Mount( CArcFile* pclArc );


protected:

	virtual	BOOL							DecompRLE2( void* pvDst, DWORD dwDstSize, const void* pvSrc, DWORD dwSrcSize );
};
