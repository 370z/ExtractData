#pragma once

class CFateFD : public CExtractBase {
protected:
	// �f�R�[�h���
	struct DecodeInfo {
		BYTE key;
		DWORD size;
	};

public:
	BOOL Mount(CArcFile* pclArc);
	BOOL Decode(CArcFile* pclArc);
};