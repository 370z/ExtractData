#include "stdafx.h"
#include "../ExtractBase.h"
#include "../Mzx.h"
#include "Ahx.h"
#include "../Wav.h"
#include "Clannad.h"

// CLANNAD��VOICE.MRG�̃t�@�C�������擾����֐�
BOOL CClannad::Mount(CArcFile* pclArc)
{
	if ((pclArc->GetArcExten() != _T(".MRG")) || (memcmp(pclArc->GetHed(), "LV", 2) != 0) || (memcmp(pclArc->GetHed() + 7, "MZX0", 4) != 0))
		return FALSE;

	// VOICE.HED���J��
	TCHAR szHedFilePath[MAX_PATH];
	lstrcpy(szHedFilePath, pclArc->GetArcPath());
	PathRenameExtension(szHedFilePath, _T(".HED"));
	CFile HedFile;
	if (HedFile.Open(szHedFilePath, FILE_READ) == INVALID_HANDLE_VALUE)
		return FALSE;

	// �C���f�b�N�X�T�C�Y�擾
	DWORD index_size = HedFile.GetFileSize() - 16;
	// �C���f�b�N�X�擾
	YCMemory<BYTE> index(index_size);
	LPBYTE pIndex = &index[0];
	HedFile.Read(pIndex, index_size);
	// �C���f�b�N�X�T�C�Y����t�@�C�����擾
	DWORD ctFile = index_size >> 2;

	// VOICE2.MRG��������20000����n�߂�
	DWORD count = (pclArc->GetArcName() == _T("VOICE.MRG")) ? 0 : 20000;

	for (int i = 0; i < (int)ctFile; i++) {
		WORD pos = *(LPWORD)&pIndex[0];
		WORD argSize = *(LPWORD)&pIndex[2];

		int section = count / 1000;
		TCHAR szFileName[_MAX_FNAME];
		_stprintf(szFileName, _T("KOE\\%04d\\Z%04d%05d.wav"), section, section, count++);

		// ���X�g�r���[�ɒǉ�
		SFileInfo infFile;
		infFile.name = szFileName;
		infFile.sizeCmp = (argSize & 0xfff) * 0x800;
		infFile.sizeOrg = infFile.sizeCmp * 6; // �K���B32�o�C�g�𓀂��Ȃ��ƓW�J��̃t�@�C���T�C�Y���擾�ł��Ȃ�
		infFile.start = pos * 0x800 + (argSize & 0xf000) * 0x8000;
		infFile.end = infFile.start + infFile.sizeCmp;
		infFile.format = _T("AHX");
		infFile.title = _T("CLANNAD");
		pclArc->AddFileInfo(infFile);

		pIndex += 4;
	}

	return TRUE;
}

// ���o����֐�
BOOL CClannad::Decode(CArcFile* pclArc)
{
	SFileInfo* pInfFile = pclArc->GetOpenFileInfo();

	if ((pInfFile->title != _T("CLANNAD")) || (pInfFile->format != _T("AHX")))
		return FALSE;

	// �o�b�t�@�m��
	YCMemory<BYTE> mzx_buf(pInfFile->sizeCmp);

	// �ǂݍ���
	pclArc->Read(&mzx_buf[0], pInfFile->sizeCmp);

	// MZX��
	DWORD ahx_buf_len = *(LPDWORD)&mzx_buf[11];
	YCMemory<BYTE> ahx_buf(ahx_buf_len + 1024);
	CMzx mzx;
	mzx.Decompress(&ahx_buf[0], ahx_buf_len, &mzx_buf[7]);

	// AHX��WAV�ɕϊ����ďo��
	CAhx ahx;
	ahx.Decode(pclArc, &ahx_buf[0], ahx_buf_len);

/*
	DWORD wav_buf_len = pclArc->ConvEndian(*(LPDWORD)&ahx_buf[12]) * 2;
	YCMemory<BYTE> wav_buf(wav_buf_len + 1152 * 2); // margen = layer-2 frame size
	CAhx ahx;
	wav_buf_len = ahx.Decompress(&wav_buf[0], &ahx_buf[0], ahx_buf_len);

	// �o��
	pclArc->OpenFile();
	CWav wav;
	wav.WriteHed(pclArc, wav_buf_len, pclArc->ConvEndian(*(LPDWORD)&ahx_buf[8]), 1, 16);
	pclArc->WriteFile(&wav_buf[0], wav_buf_len);
*/
	return TRUE;
}