
#include	"stdafx.h"
#include	"Specialite.h"

//////////////////////////////////////////////////////////////////////////////////////////
//	�����\������

BOOL	CSpecialite::OnCheckDecrypt(
	CArcFile*			pclArc							// �A�[�J�C�u
	)
{
	return	CheckTpm( "F78FF15C0BD396080BCDF431AED59211" );
}

//////////////////////////////////////////////////////////////////////////////////////////
//	���������̏�����

DWORD	CSpecialite::OnInitDecrypt(
	CArcFile*			pclArc							// �A�[�J�C�u
	)
{
	return	0xAF;
}

//////////////////////////////////////////////////////////////////////////////////////////
//	��������

DWORD	CSpecialite::OnDecrypt(
	BYTE*				pbtTarget,						// �����Ώۃf�[�^
	DWORD				dwTargetSize,					// �����T�C�Y
	DWORD				dwOffset,						// �����Ώۃf�[�^�̈ʒu
	DWORD				dwDecryptKey					// �����L�[
	)
{
	BYTE				btDecryptKey = (BYTE) dwDecryptKey;

	for( DWORD i = 0 ; i < dwTargetSize ; i++ )
	{
		pbtTarget[i] ^= btDecryptKey;
		pbtTarget[i] += 1;
	}

	return	dwTargetSize;
}
