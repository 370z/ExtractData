
#include	"stdafx.h"
#include	"Braban.h"

//////////////////////////////////////////////////////////////////////////////////////////
//	�����\������

BOOL	CBraban::OnCheckDecrypt(
	CArcFile*			pclArc							// �A�[�J�C�u
	)
{
	return	CheckTpm( "C42B141D42FCBCDA29850FA9E9FE3FF2" );
}

//////////////////////////////////////////////////////////////////////////////////////////
//	���������̏�����

DWORD	CBraban::OnInitDecrypt(
	CArcFile*			pclArc							// �A�[�J�C�u
	)
{
	return	((pclArc->GetOpenFileInfo()->key ^ 0xFF) & 0xFF) - 1;
}

//////////////////////////////////////////////////////////////////////////////////////////
//	��������

DWORD	CBraban::OnDecrypt(
	BYTE*				pbtTarget,						// �����Ώۃf�[�^
	DWORD				dwTargetSize,					// �����T�C�Y
	DWORD				dwOffset,						// �����Ώۃf�[�^�̈ʒu
	DWORD				dwDecryptKey					// �����L�[
	)
{
	BYTE				btDecryptkey = (BYTE) dwDecryptKey;

	for( DWORD i = 0 ; i < dwTargetSize ; i++ )
	{
		pbtTarget[i] ^= btDecryptkey;
	}

	return	dwTargetSize;
}
