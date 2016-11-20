#include "StdAfx.h"
#include "../Image.h"
#include "../Image/Tga.h"
#include "../Arc/LZSS.h"
#include "CircusPak.h"

/// Mounting
///
/// @param pclArc Archive
///
bool CCircusPak::Mount(CArcFile* pclArc)
{
	if (MountPakForKujiraCons(pclArc))
		return true;

	if (MountPakForKujira(pclArc))
		return true;

	if (MountPakForACDC(pclArc))
		return true;

	if (MountPakForDCGS(pclArc))
		return true;

	return false;
}

/// 最終試験くじら-Conservative-向けpakのマウント
///
/// @param pclArc Archive
///
bool CCircusPak::MountPakForKujiraCons(CArcFile* pclArc)
{
	if (pclArc->GetArcExten() != _T(".pak"))
		return false;

	if (memcmp(pclArc->GetHed(), "File Pack 1.0y", 14) != 0)
		return false;

	// Get file count
	DWORD dwFiles;
	pclArc->SeekHed(16);
	pclArc->Read(&dwFiles, 4);

	// Decryption
	dwFiles ^= 0xAAAAAAAA;

	// Get index
	YCMemory<SPakFileInfoType1> clmIndex(dwFiles);
	pclArc->SeekCur(4);
	pclArc->Read(&clmIndex[0], (sizeof(SPakFileInfoType1) * dwFiles));

	// Get file information
	for (DWORD i = 0; i < dwFiles; i++)
	{
		// Get file name
		char szFileName[25];
		for (DWORD j = 0; j < 24; j++)
		{
			szFileName[j] = clmIndex[i].szFileName[j] ^ 0xAA;
		}
		szFileName[24] = '\0';

		// Add to listview
		SFileInfo stFileInfo;
		stFileInfo.name = szFileName;
		stFileInfo.start = clmIndex[i].dwOffset ^ 0xAAAAAAAA;
		stFileInfo.sizeCmp = clmIndex[i].dwCompFileSize ^ 0xAAAAAAAA;
		stFileInfo.sizeOrg = stFileInfo.sizeCmp;
		stFileInfo.end = stFileInfo.start + stFileInfo.sizeCmp;

		pclArc->AddFileInfo(stFileInfo);
	}

	return true;
}

/// 最終試験くじら向けpakのマウント
///
/// @param pclArc Archive
///
bool CCircusPak::MountPakForKujira(CArcFile* pclArc)
{
	if (pclArc->GetArcExten() != _T(".pak"))
		return false;

	if (memcmp(pclArc->GetHed(), "File Pack 1.00", 14) != 0)
		return false;

	// Get file count
	DWORD dwFiles;
	pclArc->SeekHed(16);
	pclArc->Read(&dwFiles, 4);

	// Decryption
	dwFiles ^= 0xAAAAAAAA;

	// Get index
	YCMemory<SPakFileInfoType2> clmIndex(dwFiles);
	pclArc->SeekCur(4);
	pclArc->Read(&clmIndex[0], (sizeof(SPakFileInfoType2) * dwFiles));

	// Get file information
	for (DWORD i = 0; i < dwFiles; i++)
	{
		// Get file name
		char szFileName[33];
		for (DWORD j = 0; j < 32; j++)
		{
			szFileName[j] = clmIndex[i].szFileName[j] ^ 0xAA;
		}
		szFileName[32] = '\0';

		// Add to listview
		SFileInfo stFileInfo;
		stFileInfo.name = szFileName;
		stFileInfo.start = clmIndex[i].dwOffset ^ 0xAAAAAAAA;
		stFileInfo.sizeCmp = clmIndex[i].dwCompFileSize ^ 0xAAAAAAAA;
		stFileInfo.sizeOrg = stFileInfo.sizeCmp;
		stFileInfo.end = stFileInfo.start + stFileInfo.sizeCmp;

		pclArc->AddFileInfo(stFileInfo);
	}

	return true;
}

/// pak mounting for ACDC
///
/// @param pclArc Archive
///
bool CCircusPak::MountPakForACDC(CArcFile* pclArc)
{
	if (pclArc->GetArcExten() != _T(".pak"))
		return false;

	if (memcmp(pclArc->GetHed(), "ACDC_RA", 7) != 0)
		return false;

	// Get file count
	DWORD dwFiles;
	pclArc->SeekHed(8);
	pclArc->Read(&dwFiles, 4);

	// Decryption
	dwFiles ^= 0xA6A6A6A6;

	// Get index
	YCMemory<SPakFileInfoType2>	clmIndex(dwFiles);
	pclArc->SeekCur(4);
	pclArc->Read(&clmIndex[0], (sizeof(SPakFileInfoType2) * dwFiles));

	// Get file information
	for (DWORD i = 0; i < dwFiles; i++)
	{
		// Get filename
		char szFileName[33];
		for (DWORD j = 0; j < 32; j++)
		{
			szFileName[j] = clmIndex[i].szFileName[j] ^ 0xA6;
		}
		szFileName[32] = '\0';

		// Add to listview
		SFileInfo stFileInfo;
		stFileInfo.name = szFileName;
		stFileInfo.start = clmIndex[i].dwOffset ^ 0xA6A6A6A6;
		stFileInfo.sizeCmp = clmIndex[i].dwCompFileSize ^ 0xA6A6A6A6;
		stFileInfo.sizeOrg = stFileInfo.sizeCmp;
		stFileInfo.end = stFileInfo.start + stFileInfo.sizeCmp;

		pclArc->AddFileInfo(stFileInfo);
	}

	return true;
}

/// pak mounting for DCGS
///
/// @param pclArc Archive
///
bool CCircusPak::MountPakForDCGS(CArcFile* pclArc)
{
	if (pclArc->GetArcExten() != _T(".pak"))
		return false;

	if (memcmp(pclArc->GetHed(), "PACK", 4) != 0)
		return false;

	pclArc->SeekHed(4);

	// Get file count
	DWORD dwFiles;
	pclArc->Read(&dwFiles, 4);

	// Get flags
	DWORD dwFlags;
	pclArc->Read(&dwFlags, 4);

	// Get index size
	DWORD dwIndexSize = (sizeof(SPakFileInfoType3) * dwFiles);

	// Get index
	YCMemory<SPakFileInfoType3> clmIndex(dwFiles);
	pclArc->Read(&clmIndex[0], dwIndexSize);

	// Decode index
	BYTE* pbtIndex = (BYTE*)&clmIndex[0];
	for (DWORD i = 0; i < dwIndexSize; i++)
	{
		pbtIndex[i] = (pbtIndex[i] << 4) | (pbtIndex[i] >> 4);
	}

	// Get file information
	for (DWORD i = 0; i < dwFiles; i++)
	{
		// Add to listview
		SFileInfo stFileInfo;
		stFileInfo.name.Copy(clmIndex[i].szFileName, sizeof(clmIndex[i].szFileName));
		stFileInfo.start = clmIndex[i].dwOffset;
		stFileInfo.sizeCmp = clmIndex[i].dwCompFileSize;
		stFileInfo.sizeOrg = clmIndex[i].dwFileSize;

		if (stFileInfo.sizeCmp == 0)
		{
			stFileInfo.sizeCmp = stFileInfo.sizeOrg;
		}

		stFileInfo.end = stFileInfo.start + stFileInfo.sizeCmp;

		pclArc->AddFileInfo(stFileInfo);
	}

	return true;
}

/// Decoding
///
/// @param pclArc Archive
///
bool CCircusPak::Decode(CArcFile* pclArc)
{
	if (DecodePakForKujiraCons(pclArc))
		return true;

	if (DecodePakForKujira(pclArc))
		return true;

	if (DecodePakForACDC(pclArc))
		return true;

	if (DecodePakForDCGS(pclArc))
		return true;

	return false;
}

/// 最終試験くじら-Conservative-向けpakのデコード
///
/// @param pclArc Archive
///
bool CCircusPak::DecodePakForKujiraCons(CArcFile* pclArc)
{
	const SFileInfo* file_info = pclArc->GetOpenFileInfo();

	if (pclArc->GetArcExten() != _T(".pak"))
		return false;

	if (memcmp(pclArc->GetHed(), "File Pack 1.0y", 14) != 0)
		return false;

	BYTE abtKey[2];
	abtKey[0] = 0xFF;
	abtKey[1] = (file_info->format == _T("CS")) ? 1 : 0;

	if (file_info->format == _T("BMP"))
	{
		// BMP
		DWORD          dwSrcSize = file_info->sizeCmp;
		YCMemory<BYTE> clmbtSrc(dwSrcSize);
		pclArc->Read(&clmbtSrc[0], dwSrcSize);

		// Decode
		Decrypt1(&clmbtSrc[0], dwSrcSize, abtKey);

		// Output
		DecodeBMP(pclArc, &clmbtSrc[0], dwSrcSize);
	}
	else if (file_info->format == _T("CPS"))
	{
		// CPS
		DWORD          dwSrcSize = file_info->sizeCmp;
		YCMemory<BYTE> clmbtSrc(dwSrcSize);
		pclArc->Read(&clmbtSrc[0], dwSrcSize);

		// Decryption
		Decrypt1(&clmbtSrc[0], dwSrcSize, abtKey);
		*(DWORD*)&clmbtSrc[4] ^= 0x80701084;

		// Decoding
		DecodeCps(pclArc, &clmbtSrc[0], dwSrcSize);
	}
	else
	{
		// Other
		DecodeEtc(pclArc, Decrypt2, abtKey);
	}

	return true;
}

/// 最終試験くじら向けpakのデコード
///
/// @param pclArc Archive
///
bool CCircusPak::DecodePakForKujira(CArcFile* pclArc)
{
	const SFileInfo* file_info = pclArc->GetOpenFileInfo();

	if (pclArc->GetArcExten() != _T(".pak"))
		return false;

	if (memcmp(pclArc->GetHed(), "File Pack 1.00", 14) != 0)
		return false;

	BYTE abtKey[2];
	abtKey[0] = 0xFF;
	abtKey[1] = (file_info->format == _T("CS")) ? 1 : 0;

	if (file_info->format == _T("BMP"))
	{
		// Read
		DWORD          dwSrcSize = file_info->sizeCmp;
		YCMemory<BYTE> clmbtSrc(dwSrcSize);
		pclArc->Read(&clmbtSrc[0], dwSrcSize);

		// Decryption
		Decrypt1(&clmbtSrc[0], dwSrcSize, abtKey);

		// Output
		DecodeBMP(pclArc, &clmbtSrc[0], dwSrcSize);
	}
	else if (file_info->format == _T("CPS"))
	{
		// CPS
		DWORD          dwSrcSize = file_info->sizeCmp;
		YCMemory<BYTE> clmbtSrc(dwSrcSize);
		pclArc->Read(&clmbtSrc[0], dwSrcSize);

		// Decryption
		Decrypt1(&clmbtSrc[0], dwSrcSize, abtKey);
		*(DWORD*)&clmbtSrc[4] ^= 0x80701084;

		// Decoding
		DecodeCps(pclArc, &clmbtSrc[0], dwSrcSize);
	}
	else
	{
		// Other
		DecodeEtc(pclArc, Decrypt2, abtKey);
	}

	return true;
}

/// pak decoding for ACDC
///
/// @param pclArc Archive
///
bool CCircusPak::DecodePakForACDC(CArcFile* pclArc)
{
	const SFileInfo* file_info = pclArc->GetOpenFileInfo();

	if (pclArc->GetArcExten() != _T(".pak"))
		return false;

	if (memcmp(pclArc->GetHed(), "ACDC_RA", 7) != 0)
		return false;

	BYTE abtKey[2];
	abtKey[0] = 0x68;
	abtKey[1] = (file_info->format == _T("CS")) ? 1 : 0;

	if (file_info->format == _T("BMP"))
	{
		// BMP
		DWORD          dwSrcSize = file_info->sizeCmp;
		YCMemory<BYTE> clmbtSrc(dwSrcSize);
		pclArc->Read(&clmbtSrc[0], dwSrcSize);

		// Decryption
		Decrypt1(&clmbtSrc[0], dwSrcSize, abtKey);

		// Output
		DecodeBMP(pclArc, &clmbtSrc[0], dwSrcSize);
	}
	else if (file_info->format == _T("TGA"))
	{
		// TGA
		DWORD          dwSrcSize = file_info->sizeCmp;
		YCMemory<BYTE> clmbtSrc(dwSrcSize);
		pclArc->Read(&clmbtSrc[0], dwSrcSize);

		// Decryption
		Decrypt1(&clmbtSrc[0], dwSrcSize, abtKey);

		// Output
		CTga clTGA;
		clTGA.Decode(pclArc, &clmbtSrc[0], dwSrcSize);
	}
	else if (file_info->format == _T("CPS"))
	{
		// CPS
		DWORD          dwSrcSize = file_info->sizeCmp;
		YCMemory<BYTE> clmbtSrc(dwSrcSize);
		pclArc->Read(&clmbtSrc[0], dwSrcSize);

		// Decryption
		Decrypt1(&clmbtSrc[0], dwSrcSize, abtKey);
		*(DWORD*)&clmbtSrc[4] ^= 0x0A415FCF;

		if (dwSrcSize >= 0x308)
		{
			*(DWORD*)&clmbtSrc[4] ^= clmbtSrc[dwSrcSize - 1];

			DWORD dwWork;
			DWORD dwWork2;

			// Replace data
			dwWork = 8 + (dwSrcSize - 8) - 0xFF;
			dwWork2 = 8 + ((dwSrcSize - 8) >> 9) + 0xFF;

			std::swap(clmbtSrc[dwWork], clmbtSrc[dwWork2]);

			// Replace data
			dwWork = 8 + (dwSrcSize - 8) - (0xFF << 1);
			dwWork2 = 8 + (((dwSrcSize - 8) >> 9) << 1) + 0xFF;

			std::swap(clmbtSrc[dwWork], clmbtSrc[dwWork2]);
		}

		// Decoding
		DecodeCps(pclArc, &clmbtSrc[0], dwSrcSize);
	}
	else
	{
		// Other
		DecodeEtc(pclArc, Decrypt2, abtKey);
	}

	return true;
}

/// pak decoding for DCGS
///
/// @param pclArc Archive
///
bool CCircusPak::DecodePakForDCGS(CArcFile* pclArc)
{
	const SFileInfo* file_info = pclArc->GetOpenFileInfo();

	if (pclArc->GetArcExten() != _T(".pak"))
		return false;

	if (memcmp(pclArc->GetHed(), "PACK", 4) != 0)
		return false;

	// Read
	DWORD          dwSrcSize = file_info->sizeCmp;
	YCMemory<BYTE> clmSrc(dwSrcSize);
	pclArc->Read(&clmSrc[0], dwSrcSize);

	// Decompression
	BYTE*          pbtDst = &clmSrc[0];
	DWORD          dwDstSize = file_info->sizeOrg;
	YCMemory<BYTE> clmDst;

	if (dwSrcSize != dwDstSize)
	{
		// Is compressed
		clmDst.resize(dwDstSize);

		// Decompression
		CLZSS clLZSS;
		clLZSS.Decomp(&clmDst[0], dwDstSize, &clmSrc[0], dwSrcSize, 4096, 4078, 3);
		pbtDst = &clmDst[0];
	}

	// Output
	if (file_info->format == _T("OP2"))
	{
		// Image
		long  lWidth = *(long*)&pbtDst[4];
		long  lHeight = *(long*)&pbtDst[8];
		WORD  wBpp = *(WORD*)&pbtDst[12];
		DWORD dwDataOffset = *(DWORD*)&pbtDst[20];
		DWORD dwPalletSize = (dwDataOffset - 32);
		DWORD dwDIBSize = *(DWORD*)&pbtDst[24];

		dwDstSize = *(DWORD*)&pbtDst[28];

		YCMemory<BYTE> clmDIB(dwDIBSize);

		// Decompression
		CLZSS clLZSS;
		clLZSS.Decomp(&clmDIB[0], dwDIBSize, &pbtDst[dwDataOffset], dwDstSize, 4096, 4078, 3);

		// Output
		CImage clImage;
		clImage.Init(pclArc, lWidth, lHeight, wBpp, &pbtDst[32], dwPalletSize);
		clImage.WriteReverse(&clmDIB[0], dwDIBSize);
		clImage.Close();
	}
	else
	{
		// Other
		pclArc->OpenFile();
		pclArc->WriteFile(pbtDst, dwDstSize);
		pclArc->CloseFile();
	}

	return true;
}

/// BMP Decoding
///
/// @param pclArc    Archive
/// @param pvSrc     BMP data (to be decoded)
/// @param dwSrcSize BMP data size
///
bool CCircusPak::DecodeBMP(CArcFile* pclArc, const void* pvSrc, DWORD dwSrcSize)
{
	const SFileInfo* file_info = pclArc->GetOpenFileInfo();
	if (file_info->format != _T("BMP"))
		return false;

	BYTE*             pbtSrc = (BYTE*)pvSrc;
	BITMAPFILEHEADER* pstbmfhSrc = (BITMAPFILEHEADER*)&pbtSrc[0];
	BITMAPINFOHEADER* pstbmihSrc = (BITMAPINFOHEADER*)&pbtSrc[14];

	dwSrcSize = (pstbmihSrc->biBitCount == 8) ? (dwSrcSize - 54 - 1024) : (dwSrcSize - 54);

	CImage clImage;
	clImage.Init(pclArc, pbtSrc);
	clImage.Write(dwSrcSize);

	return true;
}

/// CPS Decoding
///
/// @param pclArc    Archive
/// @param pvSrc     CPS data (to be decoded)
/// @param dwSrcSize CPS data size
///
bool CCircusPak::DecodeCps(CArcFile* pclArc, const void* pvSrc, DWORD dwSrcSize)
{
	const SFileInfo* file_info = pclArc->GetOpenFileInfo();

	if (file_info->format != _T("CPS"))
		return false;

	const BYTE* pbtSrc = (const BYTE*)pvSrc;

	// Ensure output buffer exists
	DWORD          dwDstSize = *(DWORD*)&pbtSrc[4];
	YCMemory<BYTE> clmbtDst(dwDstSize);

	// Decompress
	if (memcmp(pbtSrc, "CCC0", 4) == 0)
	{
		// CCC0
		DecompCCC0(&clmbtDst[0], dwDstSize, pbtSrc, dwSrcSize);

		long lWidth = *(WORD*)&clmbtDst[12];
		long lHeight = *(WORD*)&clmbtDst[14];
		WORD wBpp = clmbtDst[16];

		// Output
		CImage clImage;
		clImage.Init(pclArc, lWidth, lHeight, wBpp);
		clImage.Write(&clmbtDst[18], (dwDstSize - 18));
	}
	else if (memcmp(pbtSrc, "CCM0", 4) == 0)
	{
		// CCM0
		DecompCCM0(&clmbtDst[0], dwDstSize, pbtSrc, dwSrcSize);

		// Output
		BITMAPFILEHEADER* pstBMPFileHeader = (BITMAPFILEHEADER*)&clmbtDst[0];
		BITMAPINFOHEADER* pstBMPInfoHeader = (BITMAPINFOHEADER*)&clmbtDst[14];
		dwDstSize = (pstBMPInfoHeader->biBitCount == 8) ? (dwDstSize - 54 - 1024) : (dwDstSize - 54);

		CImage clImage;
		clImage.Init(pclArc, &clmbtDst[0]);
		clImage.Write(dwDstSize);
	}
	else
	{
		// Unknown
		pclArc->OpenFile();
		pclArc->WriteFile(pbtSrc, dwSrcSize);
	}

	return true;
}

/// Decoding 'Other' data
///
/// @param archive             Archive
/// @param decryption_function Pointer to the decryption function
/// @param key                 Decryption key
///
bool CCircusPak::DecodeEtc(CArcFile* archive, FDecrypt decryption_function, const void* key)
{
	const SFileInfo* file_info = archive->GetOpenFileInfo();

	size_t buffer_size = archive->GetBufSize();
	std::vector<u8> buffer(buffer_size);

	// Generate output files
	if (file_info->format == _T("CS"))
	{
		archive->OpenScriptFile();
	}
	else
	{
		archive->OpenFile();
	}

	for (size_t write_size = 0; write_size < file_info->sizeOrg; write_size += buffer_size)
	{
		// Buffer size adjustment
		archive->SetBufSize(&buffer_size, write_size);

		// Output the decoded data
		archive->Read(buffer.data(), buffer_size);
		decryption_function(buffer.data(), buffer_size, key);
		archive->WriteFile(buffer.data(), buffer_size);
	}

	return true;
}

/// CCC0 Extraction
///
/// @param pvDst     Destination
/// @param dwDstSize Destination size
/// @param pvSrc     CCC0 data
/// @param dwSrcSize CCC0 data size
///
bool CCircusPak::DecompCCC0(void* pvDst, DWORD dwDstSize, const void* pvSrc, DWORD dwSrcSize)
{
	const BYTE* pbtSrc = (const BYTE*)pvSrc;
	BYTE*       pbtDst = (BYTE*)pvDst;

	DWORD dwSrcPtrOfBit = 0x2C * 8;
	DWORD dwDstPtr = 0;

	// Decompression
	for (dwDstPtr = 0; dwDstPtr < dwDstSize; dwDstPtr += 2)
	{
		DWORD dwIndex;

		for (dwIndex = 0; dwIndex < 0x0F; dwIndex++)
		{
			if (GetBit(pbtSrc, &dwSrcPtrOfBit, 1) == 1)
			{
				break;
			}
		}

		if (dwIndex == 0x0F)
		{
			if (GetBit(pbtSrc, &dwSrcPtrOfBit, 1) == 0)
			{
				dwIndex++;
			}
		}

		if (dwIndex != 0)
		{
			*(WORD*)&pbtDst[dwDstPtr] = ((WORD*)(pbtSrc + 8))[dwIndex - 1];
		}
		else
		{
			*(WORD*)&pbtDst[dwDstPtr] = GetBit(pbtSrc, &dwSrcPtrOfBit, 16);
		}
	}

	// サイズが奇数の場合1余る

	if (dwDstSize & 1)
	{
		pbtDst[dwDstPtr] = pbtSrc[0x28];
	}

	return true;
}

/// CCM0 Extraction
///
/// @param pvDst     Destination
/// @param dwDstSize Destination size
/// @param pvSrc     CCM0 data
/// @param dwSrcSize CCM0 data size
bool CCircusPak::DecompCCM0(void* pvDst, DWORD dwDstSize, const void* pvSrc, DWORD dwSrcSize)
{
	const BYTE* pbtSrc = (const BYTE*)pvSrc;
	BYTE*       pbtDst = (BYTE*)pvDst;

	DWORD dwSrcPtrOfBit;
	DWORD dwDstPtr = 0;

	// Decompression
	if (dwDstSize < 0x80)
	{
		// Uncompressed
		memcpy(pbtDst, &pbtSrc[8], dwDstSize);
	}
	else
	{
		for (DWORD i = 0; i < 0x80; i++)
		{
			pbtDst[dwDstPtr++] = pbtSrc[8 + i] + 0xF0;
		}

		dwSrcPtrOfBit = 0x88 * 8;

		while (dwDstPtr < dwDstSize)
		{
			DWORD dwFlag = GetBit(pbtSrc, &dwSrcPtrOfBit, 1);

			// Compressed
			if (dwFlag & 1)
			{
				DWORD dwBack = GetBit(pbtSrc, &dwSrcPtrOfBit, 7) + 1;
				DWORD dwLength = GetBit(pbtSrc, &dwSrcPtrOfBit, 4) + 2;

				for (DWORD i = 0; i < dwLength; i++)
				{
					pbtDst[dwDstPtr + i] = pbtDst[dwDstPtr - dwBack + i];
				}

				dwDstPtr += dwLength;
			}
			else // Uncompressed
			{
				pbtDst[dwDstPtr++] = GetBit(pbtSrc, &dwSrcPtrOfBit, 8);
			}
		}
	}

	return true;
}

/// Get bit sequence
///
/// @param pvSrc           Input data
/// @param pdwSrcPtrOfBit  Position of innput data (number of bits read is added)
/// @param dwReadBitLength Number of bits to read
///
DWORD CCircusPak::GetBit(const void* pvSrc, DWORD* pdwSrcPtrOfBit, DWORD dwReadBitLength)
{
	const BYTE* pbtSrc = (const BYTE*)pvSrc;
	DWORD       dwData = 0;

	for (DWORD i = 0; i < dwReadBitLength; i++)
	{
		dwData <<= 1;
		dwData |= (pbtSrc[*pdwSrcPtrOfBit >> 3] >> (*pdwSrcPtrOfBit & 7)) & 1;
		(*pdwSrcPtrOfBit)++;
	}

	return dwData;
}

/// Decryption 1
///
/// @param pvTarget     Data to be decrypted
/// @param dwTargetSize Decryption size
/// @param pvKey        Decryption key
///
void CCircusPak::Decrypt1(void* pvTarget, DWORD dwTargetSize, const void* pvKey)
{
	BYTE*       pbtTarget = (BYTE*)pvTarget;
	const BYTE* pbtKey = (const BYTE*)pvKey;

	for (DWORD i = 0; i < dwTargetSize; i++)
	{
		pbtTarget[i] ^= pbtKey[0];
	}
}

/// Decryption 2
///
/// @param pvTarget     Data to be decrypted
/// @param dwTargetSize Decryption size
/// @param pvKey        Decryption key
///
void CCircusPak::Decrypt2(void* pvTarget, DWORD dwTargetSize, const void* pvKey)
{
	BYTE*       pbtTarget = (BYTE*) pvTarget;
	const BYTE* pbtKey = (const BYTE*) pvKey;

	for (DWORD i = 0; i < dwTargetSize; i++)
	{
		pbtTarget[i] ^= pbtKey[0];
		pbtTarget[i] -= pbtKey[1];
	}
}
