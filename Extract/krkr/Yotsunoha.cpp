
#include	"stdafx.h"
#include	"Yotsunoha.h"

//////////////////////////////////////////////////////////////////////////////////////////
//	�����\������

BOOL	CYotsunoha::OnCheckDecrypt(
	CArcFile*			pclArc							// �A�[�J�C�u
	)
{
	return	CheckTpm( "AAF0A99EAF4018CB1AA5E0D9065C2239" );
}

//////////////////////////////////////////////////////////////////////////////////////////
//	���������̏�����

DWORD	CYotsunoha::OnInitDecrypt(
	CArcFile*			pclArc							// �A�[�J�C�u
	)
{
	SFileInfo*			pstFileInfo = pclArc->GetOpenFileInfo();

	// �����L�[

	return	(((pstFileInfo->key >> 8) & 0xFF) ^ (pstFileInfo->key & 0xFF));
}

//////////////////////////////////////////////////////////////////////////////////////////
//	��������

DWORD	CYotsunoha::OnDecrypt(
	BYTE*				pbtTarget,						// �����Ώۃf�[�^
	DWORD				dwTargetSize,					// �����T�C�Y
	DWORD				dwOffset,						// �����Ώۃf�[�^�̈ʒu
	DWORD				dwDecryptKey					// �����L�[
	)
{
	// ����

	BYTE				btDecryptKey = (BYTE) dwDecryptKey;

	for( DWORD i = 0 ; i < dwTargetSize ; i++ )
	{
		pbtTarget[i] ^= btDecryptKey;
	}

	return	dwTargetSize;
}
