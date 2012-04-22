
#include	"stdafx.h"
#include	"FlyingShine.h"

//////////////////////////////////////////////////////////////////////////////////////////
//	�����\������

BOOL	CFlyingShine::OnCheckDecrypt(
	CArcFile*			pclArc							// �A�[�J�C�u
	)
{
	return	CheckTpm( "2FAA3AF83558C93EC2C44F06DD727ED5" );
}

//////////////////////////////////////////////////////////////////////////////////////////
//	���������̏�����

DWORD	CFlyingShine::OnInitDecrypt(
	CArcFile*			pclArc							// �A�[�J�C�u
	)
{
	return	pclArc->GetOpenFileInfo()->key & 0xFFFF;
}

//////////////////////////////////////////////////////////////////////////////////////////
//	��������

DWORD	CFlyingShine::OnDecrypt(
	BYTE*				pbtTarget,						// �����Ώۃf�[�^
	DWORD				dwTargetSize,					// �����T�C�Y
	DWORD				dwOffset,						// �����Ώۃf�[�^�̈ʒu
	DWORD				dwDecryptKey					// �����L�[
	)
{
	BYTE				btDecryptkey1 = (BYTE) ((dwDecryptKey >> 8) & 0xFF);
	BYTE				btDecryptkey2 = (BYTE) ((dwDecryptKey & 0xFF) % 8);

	for( DWORD i = 0 ; i < dwTargetSize ; i++ )
	{
		pbtTarget[i] ^= btDecryptkey1;

		// �E���[�e�[�g

		pbtTarget[i] = (pbtTarget[i] >> btDecryptkey2) | (pbtTarget[i] << (8 - btDecryptkey2));
	}

	return	dwTargetSize;
}
