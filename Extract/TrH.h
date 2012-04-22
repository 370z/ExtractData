#pragma once

class CTrH : public CExtractBase {
protected:
	// PX�t�H�[�}�b�g
	struct PXHed {
		BYTE	pxID[4];		// "fPX "
		DWORD	FileSize;		// �t�@�C���T�C�Y
		BYTE	CtrkID[4];		// "cTRK"
		DWORD	DataSize;		// �g�`�f�[�^�̃o�C�g��
		DWORD	dummy1;			// �s��
		DWORD	dummy2;			// �s��
		DWORD	dummy3;			// �s��
		DWORD	freq;			// �T���v�����O���[�g
		WORD	dummy4;			// �s��
		WORD	channels;		// �`�����l����
		DWORD	ChunkByte;		// �`�����N�̃o�C�g��
		WORD	bits;			// �T���v��������̃r�b�g��(bit/sample)
		WORD	FormatID;		// �t�H�[�}�b�gID
	};

public:
	BOOL Mount(CArcFile* pclArc);
	BOOL Decode(CArcFile* pclArc);
};