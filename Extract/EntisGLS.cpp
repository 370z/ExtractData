
#include	"stdafx.h"
#include	"../ExtractBase.h"
#include	"EntisGLS.h"

//////////////////////////////////////////////////////////////////////////////////////////
//	�}�E���g

BOOL	CEntisGLS::Mount(
	CArcFile*			pclArc							// �A�[�J�C�u
	)
{
	if( pclArc->GetArcExten() != _T(".noa") )
	{
		return	FALSE;
	}

	if( memcmp( pclArc->GetHed(), "Entis\x1A\0\0", 8 ) != 0 )
	{
		return	FALSE;
	}

//	if( memcmp( pclArc->GetHed(), ""


// 64�o�C�g�ǂݍ��݂Ńw�b�_�`�F�b�N
// 16�o�C�g�ǂݍ��݂�DirEntry
//  4�o�C�g�ǂݍ��݂ŃG���g���[���擾

//  8�o�C�g�ǂݍ��݂Łc�I���W�i���t�@�C���T�C�Y�擾
//  4�o�C�g�ǂݍ��݂Łc�s��
//  4�o�C�g�ǂݍ��݂Łc�t�@�C���^�C�v�擾(10 00 00 80���t�@�C���A10 00 00 00���t�H���_)
//  8�o�C�g�ǂݍ��݂Łc�s��
//  8�o�C�g�ǂݍ��݂Łc�s��
//  4�o�C�g�ǂݍ��݂Łc�s��
//  4�o�C�g�ǂݍ��݂Ńt�@�C�������擾
//  �t�@�C���������̓ǂݍ��݂Ńt�@�C�����擾

//  8�o�C�g�ǂݍ��݂Łc�I���W�i���t�@�C���T�C�Y�擾
//  4�o�C�g�ǂݍ��݂Łc�s��
//  4�o�C�g�ǂݍ��݂Łc�t�@�C���^�C�v�擾
//  8�o�C�g�ǂݍ��݂Łc�s��
//  8�o�C�g�ǂݍ��݂Łc�s��
//  4�o�C�g�ǂݍ��݂Łc�s��
//  4�o�C�g�ǂݍ��݂Ńt�@�C�������擾
//  �t�@�C���������̓ǂݍ��݂Ńt�@�C�����擾
















	return	FALSE;
}

//////////////////////////////////////////////////////////////////////////////////////////
//	�f�R�[�h

BOOL	CEntisGLS::Decode(
	CArcFile*			pclArc							// �A�[�J�C�u
	)
{




	return	FALSE;
}
