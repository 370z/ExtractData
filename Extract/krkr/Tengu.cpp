
#include	"stdafx.h"
#include	"Tengu.h"

//////////////////////////////////////////////////////////////////////////////////////////
//	�����\������

BOOL	CTengu::OnCheckDecrypt(
	CArcFile*			pclArc							// �A�[�J�C�u
	)
{
	return	CheckTpm( "CE093BB86595E62ADBCB1280CA6583EF" );
}

//////////////////////////////////////////////////////////////////////////////////////////
//	���������̏�����

DWORD	CTengu::OnInitDecrypt(
	CArcFile*			pclArc							// �A�[�J�C�u
	)
{
	// �����L�[

	return	0x08;
}

//////////////////////////////////////////////////////////////////////////////////////////
//	��������

DWORD	CTengu::OnDecrypt(
	BYTE*				pbtTarget,						// �����Ώۃf�[�^
	DWORD				dwTargetSize,					// �����T�C�Y
	DWORD				dwOffset,						// �����Ώۃf�[�^�̈ʒu
	DWORD				dwDecryptKey					// �����L�[
	)
{
	BYTE				btDecryptKey = (BYTE) dwDecryptKey;

	// ����

	for( DWORD i = 0 ; i < dwTargetSize ; i++ )
	{
		pbtTarget[i] ^= btDecryptKey;
	}

	if( memcmp( pbtTarget, "\xFE\xFE\x01\xFF\xFE", 5 ) != 0 )
	{
		return	dwTargetSize;
	}

	// �e���|�����ɃR�s�[

	YCMemory<BYTE>		clmbtTemporary( dwTargetSize );

	memcpy( &clmbtTemporary[0], pbtTarget, dwTargetSize );

	ZeroMemory( pbtTarget, dwTargetSize );

	// ����

	for( DWORD i = 5, j = 0 ; i < dwTargetSize ; i += 2, j += 2 )
	{
		WORD				wWork1 = *(WORD*) &clmbtTemporary[i];
		WORD				wWork2 = wWork1;

		wWork1 = (wWork1 & 0x5555) << 1;
		wWork2 = (wWork2 & 0xAAAA) >> 1;

		*(WORD*) &pbtTarget[j] = (wWork1 | wWork2);
	}

	// �}���`�o�C�g�����ɕϊ�

	YCStringA			clsWork = (wchar_t*) pbtTarget;

	strcpy( (char*) pbtTarget, clsWork );

	return	(DWORD) clsWork.GetLength();
}
