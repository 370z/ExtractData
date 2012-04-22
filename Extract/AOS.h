
#pragma		once

#include	"../ExtractBase.h"

class	CAOS : public CExtractBase
{
public:

	struct	SABMHeader
	{
		BITMAPFILEHEADER	stBMPFileHeader;			// �r�b�g�}�b�v�t�@�C���w�b�_
		BITMAPINFOHEADER	stBMPInfoHeader;			// �r�b�g�}�b�v�C���t�H�w�b�_
		WORD				wUnknown[2];				// �s��
		DWORD				dwFrames;					// �t���[����

	};

	virtual	BOOL							Mount( CArcFile* pclArc );
	virtual	BOOL							Decode( CArcFile* pclArc );


protected:

	BOOL									DecodeABM( CArcFile* pclArc );
	BOOL									DecodeMask( CArcFile* pclArc );
	BOOL									DecodeScript( CArcFile* pclArc );

	BOOL									DecompABM( BYTE* pbtDst, DWORD dwDstSize, const BYTE* pbtSrc, DWORD dwSrcSize );
	BOOL									DecompScript( BYTE* pbtDst, DWORD dwDstSize, const BYTE* pbtSrc, DWORD dwSrcSize );

	DWORD									CreateHuffmanTable( DWORD* pdwTableOfBit0, DWORD* pdwTableOfBit1, const BYTE* pbtSrc, DWORD* pdwSrcPtr, DWORD* pdwTablePtr, DWORD* pdwCurrentSrc, DWORD* pdwBitShift );
	BOOL									DecompHuffman( BYTE* pbtDst, DWORD dwDstSize, const DWORD* pdwTableOfBit0, const DWORD* pdwTableOfBit1, const BYTE* pbtSrc, DWORD dwRoot, DWORD dwCurrentSrc, DWORD dwBitShift );
};
