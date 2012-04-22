#pragma once

class CNavel : public CExtractBase {
protected:
	// WPD�t�H�[�}�b�g
	struct FormatWPD {
		BYTE	ID[4];		// " DPW"
		DWORD	type;		// �s��
		DWORD	dummy2;		// �s��
		DWORD	ChunkByte;	// �`�����N�̃o�C�g��
		DWORD	freq;		// �T���v�����O���[�g
		DWORD	DataSize;	// �f�[�^�T�C�Y
		WORD	FormatID;	// �t�H�[�}�b�gID
		WORD	channels;	// �`�����l����
		DWORD	freq2;		// �T���v�����O���[�g(���̂�2����)
		DWORD	bps;		// �f�[�^���x(Byte/sec)
		WORD	BlockSize;	// �u���b�N�T�C�Y(Byte/sample * �`�����l����)
		WORD	bits;		// �T���v��������̃r�b�g��(bit/sample)
		DWORD	dummy3;		// �s��
	};

public:
	BOOL Mount(CArcFile* pclArc);
	BOOL MountPac(CArcFile* pclArc);
	BOOL MountWpd(CArcFile* pclArc);
	BOOL Decode(CArcFile* pclArc);
};