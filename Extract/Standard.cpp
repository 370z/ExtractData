
#include	"stdafx.h"
#include	"../ExtractBase.h"
#include	"../Arc/LZSS.h"
#include	"../Arc/Zlib.h"
#include	"Ahx.h"
#include	"../Image.h"
#include	"../Ogg.h"
#include	"Standard.h"

//////////////////////////////////////////////////////////////////////////////////////////
//	�}�E���g

BOOL	CStandard::Mount(
	CArcFile*			pclArc							// �A�[�J�C�u
	)
{
	// �t�@�C�����i�[

	SFileInfo			stFileInfo;

	stFileInfo.name = pclArc->GetArcName();
	stFileInfo.sizeOrg = pclArc->GetArcSize();
	stFileInfo.sizeCmp = stFileInfo.sizeOrg;
	stFileInfo.start = 0;
	stFileInfo.end = stFileInfo.sizeOrg;

	pclArc->AddFileInfo( stFileInfo );

	return	TRUE;
}

//////////////////////////////////////////////////////////////////////////////////////////
//	�f�R�[�h

BOOL	CStandard::Decode(
	CArcFile*			pclArc							// �A�[�J�C�u
	)
{
	if( DecodeLZSS( pclArc ) )
	{
		return	TRUE;
	}

	if( DecodeZlib( pclArc ) )
	{
		return	TRUE;
	}

	if( DecodeAhx( pclArc ) )
	{
		return	TRUE;
	}

	if( DecodeImage( pclArc ) )
	{
		return	TRUE;
	}

	if( DecodeOgg( pclArc ) )
	{
		return	TRUE;
	}

	return	Extract( pclArc );
}

//////////////////////////////////////////////////////////////////////////////////////////
//	LZSS�̃f�R�[�h

BOOL	CStandard::DecodeLZSS(
	CArcFile*			pclArc							// �A�[�J�C�u
	)
{
	CLZSS				clLZSS;

	return	clLZSS.Decode( pclArc );
}

//////////////////////////////////////////////////////////////////////////////////////////
//	zlib�̃f�R�[�h

BOOL	CStandard::DecodeZlib(
	CArcFile*			pclArc							// �A�[�J�C�u
	)
{
	CZlib				clZlib;

	return	clZlib.Decode( pclArc );
}

//////////////////////////////////////////////////////////////////////////////////////////
//	AHX�̃f�R�[�h

BOOL	CStandard::DecodeAhx(
	CArcFile*			pclArc							// �A�[�J�C�u
	)
{
	CAhx				clAHX;

	return	clAHX.Decode( pclArc );
}

//////////////////////////////////////////////////////////////////////////////////////////
//	�摜�̃f�R�[�h

BOOL	CStandard::DecodeImage(
	CArcFile*			pclArc							// �A�[�J�C�u
	)
{
	CImage				clImage;

	return	clImage.Decode( pclArc );
}

//////////////////////////////////////////////////////////////////////////////////////////
//	Ogg Vorbis�̃f�R�[�h

BOOL	CStandard::DecodeOgg(
	CArcFile*			pclArc							// �A�[�J�C�u
	)
{
	COgg				clOgg;

	return	clOgg.Decode( pclArc );
}

//////////////////////////////////////////////////////////////////////////////////////////
//	���o

BOOL	CStandard::Extract(
	CArcFile*			pclArc							// �A�[�J�C�u
	)
{
	pclArc->OpenFile();
	pclArc->ReadWrite();
	pclArc->CloseFile();

	return	TRUE;
}
