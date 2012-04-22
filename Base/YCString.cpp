
#include	"stdafx.h"
#include	"YCString.h"

//////////////////////////////////////////////////////////////////////////////////////////
//	�����R�[�h�ϊ���̒����̎擾
//
//	return	���C�h�����ɕϊ������Ƃ��̒���
//
//	���l	�}���`�o�C�g���� -> ���C�h����

int		YCBaseString::GetBaseTypeLength(
	const char*			pszSrc							// �}���`�o�C�g������
	) const
{
	return	(GetBaseTypeLength( pszSrc, -1 ) - 1);
}

//////////////////////////////////////////////////////////////////////////////////////////
//	�����R�[�h�ϊ���̒����̎擾
//
//	return	���C�h�����ɕϊ������Ƃ��̒���
//
//	���l	�}���`�o�C�g���� -> ���C�h����

int		YCBaseString::GetBaseTypeLength(
	const char*			pszSrc,							// �}���`�o�C�g������
	int					nLength							// ����
	) const
{
	return	::MultiByteToWideChar( CP_ACP, 0, pszSrc, nLength, NULL, 0 );
}

//////////////////////////////////////////////////////////////////////////////////////////
//	�����R�[�h�ϊ���̒����̎擾
//
//	return	�}���`�o�C�g�����ɕϊ������Ƃ��̒���
//
//	���l	���C�h���� -> �}���`�o�C�g����

int		YCBaseString::GetBaseTypeLength(
	const wchar_t*		pwszSrc							// ���C�h������
	) const
{
	return	(GetBaseTypeLength( pwszSrc, -1 ) - 1);
}

//////////////////////////////////////////////////////////////////////////////////////////
//	�����R�[�h�ϊ���̒����̎擾
//
//	return	�}���`�o�C�g�����ɕϊ������Ƃ��̒���
//
//	���l	���C�h���� -> �}���`�o�C�g����

int		YCBaseString::GetBaseTypeLength(
	const wchar_t*		pwszSrc,						// ���C�h������
	int					nLength							// ����
	) const
{
	return	::WideCharToMultiByte( CP_ACP, 0, pwszSrc, nLength, NULL, 0, NULL, NULL );
}

//////////////////////////////////////////////////////////////////////////////////////////
//	�����R�[�h�̕ϊ�
//
//	return	���C�h�����ɕϊ������Ƃ��̒���
//
//	���l	�}���`�o�C�g���� -> ���C�h����

int		YCBaseString::ConvertToBaseType(
	wchar_t*			pwszDst,						// ���C�h������̊i�[��
	int					nDstLength,						// �i�[��T�C�Y
	const char*			pszSrc,							// �}���`�o�C�g������
	int					nSrcLength						// �}���`�o�C�g������̒���
	) const
{
	int					nLength;

	nLength = ::MultiByteToWideChar( CP_ACP, 0, pszSrc, nSrcLength, pwszDst, nDstLength );

	if( nSrcLength == -1 )
	{
		nLength -= 1;
	}
	else
	{
		pwszDst[nLength] = L'\0';
	}

	return	nLength;
}

//////////////////////////////////////////////////////////////////////////////////////////
//	�����R�[�h�̕ϊ�
//
//	return	�}���`�o�C�g�����ɕϊ������Ƃ��̒���
//
//	���l	���C�h���� -> �}���`�o�C�g����

int		YCBaseString::ConvertToBaseType(
	char*				pszDst,							// �}���`�o�C�g������̊i�[��
	int					nDstLength,						// �i�[��T�C�Y
	const wchar_t*		pwszSrc,						// ���C�h������
	int					nSrcLength						// ���C�h������̒���
	) const
{
	int					nLength;

	nLength = ::WideCharToMultiByte( CP_ACP, 0, pwszSrc, nSrcLength, pszDst, nDstLength, NULL, NULL );

	if( nSrcLength == -1 )
	{
		nLength -= 1;
	}
	else
	{
		pszDst[nLength] = '\0';
	}

	return	nLength;
}
