
#include	"stdafx.h"
#include	"Natsupochi.h"

//////////////////////////////////////////////////////////////////////////////////////////
//	�����\������

BOOL	CNatsupochi::OnCheckDecrypt(
	CArcFile*			pclArc							// �A�[�J�C�u
	)
{
	return	CheckTpm( "B1CCCE8E4048B563B316D8BAEDF7E2B1" );
}

//////////////////////////////////////////////////////////////////////////////////////////
//	���������̏�����

DWORD	CNatsupochi::OnInitDecrypt(
	CArcFile*			pclArc							// �A�[�J�C�u
	)
{
	return	(pclArc->GetOpenFileInfo()->key >> 3);
}

//////////////////////////////////////////////////////////////////////////////////////////
//	��������

DWORD	CNatsupochi::OnDecrypt(
	BYTE*				pbtTarget,						// �����Ώۃf�[�^
	DWORD				dwTargetSize,					// �����T�C�Y
	DWORD				dwOffset,						// �����Ώۃf�[�^�̈ʒu
	DWORD				dwDecryptKey					// �����L�[
	)
{
	BYTE				btDecryptKey = (BYTE) (dwDecryptKey & 0xFF);

	for( DWORD i = 0 ; i < dwTargetSize ; i++ )
	{
		pbtTarget[i] ^= btDecryptKey;
	}

	return	dwTargetSize;
}
