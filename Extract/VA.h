#pragma once

class CVA : public CExtractBase {
private:
	BOOL MountNwa(CArcFile* pclArc);
	BOOL MountNwk(CArcFile* pclArc);
	BOOL MountOvk(CArcFile* pclArc);
	BOOL DecodeNwa(CArcFile* pclArc);

	inline int getbits(LPBYTE& data, int& shift, int bits);

protected:
	// NWA�t�H�[�}�b�g
	struct NWAHed {
		WORD	channels;		// �`�����l����
		WORD	bits;			// �T���v��������̃r�b�g��(bit/sample)
		DWORD	freq;			// �T���v�����O���[�g
		DWORD	CmpLevel;		// ���k���x��
		DWORD	dummy1;			// �s��
		DWORD	blocks;			// �u���b�N��
		DWORD	DataSize;		// �W�J��̔g�`�f�[�^�̃o�C�g��
		DWORD	CompFileSize;	// ���k���̃t�@�C���T�C�Y
		DWORD	samples;		// �W�J��̃f�[�^��
		DWORD	BlockSize;		// �f�[�^1�u���b�N��W�J�������̃T���v���P�ʂ̃f�[�^��
		DWORD	RestSize;		// �ŏI�u���b�N��W�J�������̃T���v���P�ʂ̃f�[�^��
		DWORD	dummy2;			// �s��
	};

	// NWK�C���f�b�N�X�w�b�_
	struct IndexNwk {
		DWORD	size;
		DWORD	start;
		DWORD	dummy;
	};

public:
	BOOL Mount(CArcFile* pclArc);
	BOOL Decode(CArcFile* pclArc);
};