#include "stdafx.h"
#include "AngelType.h"

//////////////////////////////////////////////////////////////////////////////////////////
// Mount
//
// Parameters:
//   - pclArc - Archive

BOOL CAngelType::Mount(CArcFile* pclArc)
{
	if (!pclArc->CheckExe(_T("ANGELTYPE.exe")))
	{
		return FALSE;
	}

	return CPaz::Mount(pclArc);
}

//////////////////////////////////////////////////////////////////////////////////////////
// Decode
//
// Parameters:
//   - pclArc - Archive

BOOL CAngelType::Decode(CArcFile* pclArc)
{
	if (!pclArc->CheckExe(_T("ANGELTYPE.exe")))
	{
		return FALSE;
	}

	return CPaz::Decode(pclArc);
}

//////////////////////////////////////////////////////////////////////////////////////////
// Initialize mount key
//
// Parameters:
//   - pclArc - Archive

void CAngelType::InitMountKey(CArcFile* pclArc)
{
	static const SKeyInfo astKeyInfo[] =
	{
		{ _T("scr"),    "\xFA\x07\xB4\x39\xF8\x01\x8D\x8A\xEC\xF9\x86\xE6\x41\x0B\x30\xF2\x01\x31\x73\x7A\x32\x0E\x25\xB2\x12\x5D\xCA\xFB\xCD\xB6\x3C\xDB" },
		{ _T("bg"),     "\x8B\x71\x06\xF7\x84\x65\x26\xCC\xE3\x43\xFF\xD8\x31\x4A\x33\x0F\x12\x1E\x29\x3E\x0A\x74\x06\x32\x0D\x5F\xC7\x57\x9E\xF3\x51\x99" },
		{ _T("st"),     "\x3A\x0A\x26\x36\xCB\xE5\x89\x5B\xEB\x63\xD1\x2E\xB3\x98\xA0\x1C\xD4\x84\xCE\x24\xBA\x9D\xBD\xDC\x84\xFB\xD5\xEE\x79\xDF\xF4\xD9" },
		{ _T("sys"),    "\xDC\x58\x83\xA7\xB8\xBE\xFD\x63\x89\xCF\x9A\x07\x66\x25\xEE\xB0\x7A\xCB\x31\xBC\x8F\xA5\xF1\xBA\x5A\x85\xF2\xC3\x59\x88\xFC\x17" },
		{ _T("bgm"),    "\x04\xA0\x4F\xBC\xF7\xED\x8B\xCB\x00\xBD\xBB\x47\xA2\xDE\x55\x25\xF8\x16\xE1\x59\x94\x6B\x09\x97\x32\x13\xDE\x95\xFA\xBC\x00\x8B" },
		{ _T("voice"),  "\x05\xE7\x77\xA5\xF4\x30\xF8\x3E\x53\x22\x53\x8D\x86\x70\xCD\x95\x01\x4B\x2E\x0B\x93\x8A\x2E\xFA\x71\x7C\x19\xE8\xD7\x07\x59\x2E" },
		{ _T("se"),     "\xF3\xF4\xAD\x54\xD9\x04\xCE\x24\x46\xB3\x41\x3C\xEB\x49\x0D\xDA\x09\x10\x27\xA4\x19\x62\x47\x2F\x39\x53\xE2\xFC\x2B\xB9\x1D\xBB" },
		{ _T("mov"),    "\xA1\x4B\x5E\x7B\x93\xA7\x53\xA9\x5C\xE6\x25\x76\x6D\x96\x8B\x8D\x43\xCB\x9C\xB6\x72\x10\xFB\x3D\x6E\xF0\x39\xD4\xF2\xDD\x24\xAA" }
	};

	SetKey(pclArc, astKeyInfo);
}

//////////////////////////////////////////////////////////////////////////////////////////
// Initialize decode key
//
// Parameters:
//   - pclArc - Archive

void CAngelType::InitDecodeKey(CArcFile* pclArc)
{
	static const SKeyInfo astKeyInfo[] =
	{
		{ _T("scr"),    "\x27\x1C\x36\xA7\x22\x3E\x6D\xCC\x7F\x3C\xBE\x50\x7A\xF0\x00\x91\x4F\xA1\x6A\xBB\x03\xAB\xA4\x9A\xE7\xF4\x71\x9B\x9E\xE4\xB3\xB7" },
		{ _T("bg"),     "\x51\xB4\x92\x0A\xF1\xC0\x78\xDB\x26\xAA\xAC\xC5\xC1\x08\x4A\xD8\x6B\x60\x55\x5F\x49\x28\xA5\x14\x65\x9C\x74\x7F\x8F\x1E\x41\x42" },
		{ _T("st"),     "\x75\xD8\xD6\x12\xC2\x7A\x27\x19\xEC\xB6\xC9\x87\x8C\x52\xCE\x0D\xE6\x7D\xB3\x06\x70\x3A\x32\x08\x1F\x8D\xA9\xA7\x1C\x3E\xE7\x1F" },
		{ _T("sys"),    "\xCE\xDB\x08\xAC\xC5\xD7\x37\x4D\x7B\x65\x0C\xB6\x7D\xE7\x3A\x29\xDA\xAB\x6A\x00\x8C\xAD\xE6\x9F\x9D\x2A\xE6\x95\xC8\xBF\x33\xA7" },
		{ _T("bgm"),    "\x54\xD1\xEC\x89\xED\x03\x28\x00\x3C\x79\x29\x32\xF4\xA4\xFA\xE3\x20\x5E\x1E\x5C\x6D\x0D\x21\xBF\x2B\x97\xC0\x86\xD3\xDF\x72\xF0" },
		{ _T("voice"),  "\xC1\x8E\x0A\x18\xEB\xEA\x34\x78\x59\x77\x95\x99\x14\x25\x3C\xB2\x53\xC9\x36\xEA\xA4\xA5\xFF\x10\xD1\xB7\x8D\x7B\x3F\x97\xB2\xD3" },
		{ _T("se"),     "\x8F\xA4\xA5\x89\x30\x36\x5C\xBC\xBA\xA1\x88\x4C\xBD\xC4\xEE\xCD\xCB\xE0\xD6\x39\x83\x81\x5F\xFB\x59\x2D\x62\x34\xCE\xA4\xC1\xE6" },
		{ _T("mov"),    "\xF7\x68\xC4\xCA\xEE\x54\x5B\x96\x0B\xFC\xF5\x6A\x91\x9D\xBD\x2D\xA1\x56\xE6\x99\x1C\x6B\xDD\xA5\x4C\x5D\x15\x2F\x01\x81\x2C\x81" }
	};

	SetKey(pclArc, astKeyInfo);
}
