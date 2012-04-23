#include	"stdafx.h"
#include	"../ExtractBase.h"
#include	"EntisGLS.h"

//////////////////////////////////////////////////////////////////////////////////////////
//	Mount

BOOL	CEntisGLS::Mount(
	CArcFile*			pclArc							// Archive
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


//  Read 64 bytes - Header check
//  Read 16 bytes - DirEntry
//  Read 4 bytes - Obtain number of entries

//  Read 8 bytes - Get original file size
//  Read 4 bytes�cUnknown
//  Read 4 bytes�cGets file type(10 00 00 80 is a file�A10 00 00 00 is a folder)
//  Read 8 bytes�cUnknown
//  Read 8 bytes�cUnknown
//  Read 4 bytes�cUnknown
//  Read 4 bytes long and get file name
//  Reading of the filename length segment gets the file name

//  Read 8 bytes�cGets original file size
//  Read 4 bytes�cUnknown
//  Read 4 bytes�cGets filetype
//  Read 8 bytes�cUnknown
//  Read 8 bytes�cUnknown
//  Read 4 bytes�cUnknown
//  Read 4 bytes - Gets file name
//  Gets the file name by reading its segment

	return	FALSE;
}

//////////////////////////////////////////////////////////////////////////////////////////
//	Decode

BOOL	CEntisGLS::Decode(
	CArcFile*			pclArc							// Archive
	)
{




	return	FALSE;
}
