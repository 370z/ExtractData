
#include	"stdafx.h"
#include	"Fate.h"

//////////////////////////////////////////////////////////////////////////////////////////
//	�����\������

BOOL	CFate::OnCheckDecrypt(
	CArcFile*			pclArc							// �A�[�J�C�u
	)
{
	static const char*	apszMD5[] =
	{
		"4BE5F676CE80429B3245DA9E721FE23B",
		"7570E3AFA14F221268DCC48CAEE1277F",
	};

	for( DWORD i = 0 ; i < _countof( apszMD5 ) ; i++ )
	{
		if( CheckTpm( apszMD5[i] ) )
		{
			return	TRUE;
		}
	}

	return	FALSE;
}

//////////////////////////////////////////////////////////////////////////////////////////
//	���������̏�����

DWORD	CFate::OnInitDecrypt(
	CArcFile*			pclArc							// �A�[�J�C�u
	)
{
	return	0x36;
}

//////////////////////////////////////////////////////////////////////////////////////////
//	��������

DWORD	CFate::OnDecrypt(
	BYTE*				pbtTarget,						// �����Ώۃf�[�^
	DWORD				dwTargetSize,					// �����T�C�Y
	DWORD				dwOffset,						// �����Ώۃf�[�^�̈ʒu
	DWORD				dwDecryptKey					// �����L�[
	)
{
	for( DWORD i = 0 ; i < dwTargetSize ; i++ )
	{
		pbtTarget[i] ^= dwDecryptKey;

		if( (dwOffset + i) == 0x13 )
		{
			pbtTarget[i] ^= 0x01;
		}

		if( (dwOffset + i) == 0x2EA29 )
		{
			pbtTarget[i] ^= 0x03;
		}
	}

	return	dwTargetSize;
}
