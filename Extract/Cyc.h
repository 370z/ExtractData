#pragma once

class CCyc : public CExtractBase {
public:
	// �}�E���g
	BOOL Mount(CArcFile* pclArc);
	// gtb���Q�Ƃ��Agpk���}�E���g(dwq���A�[�J�C�u����Ă���)
	BOOL MountGpk(CArcFile* pclArc);
	// vtb���Q�Ƃ��Avpk���}�E���g(wgq���A�[�J�C�u����Ă���)
	BOOL MountVpk(CArcFile* pclArc);
	// dwq���}�E���g(BMP: PACKTYPE=0, PACKBMP+MASK: PACKTYPE=3A, JPEG: PACKTYPE=5, JPEG+MASK: PACKTYPE=7A)
	BOOL MountDwq(CArcFile* pclArc);
	// wgq���}�E���g(OGG: PACKTYPE=6)
	BOOL MountWgq(CArcFile* pclArc);
	// vaw���}�E���g(WAV: PACKTYPE=0, RIFF OGG: PACKTYPE=2)
	BOOL MountVaw(CArcFile* pclArc);
	// xtx���}�E���g(�e�L�X�g�f�[�^)
	BOOL MountXtx(CArcFile* pclArc);
	// fxf���}�E���g(xtx���r�b�g���]�����f�[�^)
	BOOL MountFxf(CArcFile* pclArc);

	// �f�R�[�h
	BOOL Decode(CArcFile* pclArc);
	// dwq���f�R�[�h
	BOOL DecodeDwq(CArcFile* pclArc);
	// wgq���f�R�[�h
	BOOL DecodeWgq(CArcFile* pclArc);
	// vaw���f�R�[�h
	BOOL DecodeVaw(CArcFile* pclArc);
	// xtx���f�R�[�h
	BOOL DecodeXtx(CArcFile* pclArc);
	// fxf���f�R�[�h
	BOOL DecodeFxf(CArcFile* pclArc);

	// RLE��dst�ɉ�
	void DecompRLE(LPBYTE dst, LPBYTE src, LONG width, LONG height);
	// RGB��BGR�ɕ��בւ���
	void RGBtoBGR(LPBYTE buf, LONG widht, LONG height);
};