
#pragma		once

//----------------------------------------------------------------------------------------
//-- TCD3�N���X --------------------------------------------------------------------------
//----------------------------------------------------------------------------------------

#include	"TCDBase.h"

class	CTCD3 : public CTCDBase
{
public:

	struct	STCD3IndexInfo
	{
		DWORD				dwFileSize;					// �t�@�C���T�C�Y
		DWORD				dwIndexOffset;				// �C���f�b�N�X�ւ̐�΃I�t�Z�b�g�l
		DWORD				dwDirCount;					// �t�H���_��
		DWORD				dwDirNameLength;			// �t�H���_���̒���(4�o�C�g�P�ʁB��FSYSTEM == 8�o�C�g)
		DWORD				dwFileCount;				// �t�@�C����
		DWORD				dwFileNameLength;			// �t�@�C�����̒���
		DWORD				dwFileCountEx;				// �t�@�C����(�g����)
		DWORD				dwFileNameLengthEx;			// �t�@�C�����̒���(�g����)
	};

	struct	STCD3DirInfo
	{
		DWORD				dwFileCount;				// �t�@�C����
		DWORD				dwFileNameOffset;			// �t�@�C�����ւ̃I�t�Z�b�g�l
		DWORD				dwFileOffset;				// �t�@�C���ւ̃I�t�Z�b�g�l
		DWORD				dwReserve;					// ���U�[�u
	};

	BOOL									Mount( CArcFile* pclArc );


protected:

	virtual	BOOL							DecompRLE2( void* pvDst, DWORD dwDstSize, const void* pvSrc, DWORD dwSrcSize );
};
