#include "stdafx.h"
#include "../ExtractBase.h"
#include "FateFD.h"

// Fate/hollow ataraxia��video.xp3�t�@�C�������擾����֐�
BOOL CFateFD::Mount(CArcFile* pclArc)
{return FALSE;
	if ((pclArc->GetArcExten() != _T(".xp3")) || (memcmp(pclArc->GetHed(), "XP3", 3) != 0) || (pclArc->GetHed()[0x0C] != 0x93))
		return FALSE;

	SFileInfo file[2];
	file[0].name = _T("FateFDOP1.mpg");
	file[0].start = 0x3A2;
	file[0].end = 0x551FBA6;
	file[1].name = _T("FateFDOP2.mpg");
	file[1].start = 0x551FBA6;
	file[1].end = 0x90693AA;

	// ���X�g�r���[�ɒǉ�
	for (int i = 0; i < 2; i++) {
		SFileInfo infFile;
		infFile.name = file[i].name;
		infFile.start = file[i].start;
		infFile.end = file[i].end;
		infFile.sizeOrg = infFile.end - infFile.start;
		infFile.sizeCmp = infFile.sizeOrg;
		pclArc->AddFileInfo(infFile);
	}

	return TRUE;
}

BOOL CFateFD::Decode(CArcFile* pclArc)
{
	SFileInfo* pInfFile = pclArc->GetOpenFileInfo();

	if (pInfFile->name.substr(0, 8) != _T("FateFDOP"))
		return FALSE;

	// �o�b�t�@�m��
	DWORD BufSize = pclArc->GetBufSize();
	YCMemory<BYTE> buf(BufSize);

	// �o�̓t�@�C������
	pclArc->OpenFile();

	DecodeInfo decode[6];
	// 1�ڂ̃t�@�C�����f�R�[�h���邽�߂̏���
	if (pInfFile->start == 0x3A2) {
		decode[0].key = 0xCE;
		decode[0].size = 0xB6B - pInfFile->start;
		decode[1].key = 0x94;
		decode[1].size = 0x7036 - 0xB6B;
		decode[2].key = 0xB3;
		decode[2].size = 0x7037 - 0x7036;
		decode[3].key = 0x94;
		decode[3].size = 0xAF27 - 0x7037;
		decode[4].key = 0xF8;
		decode[4].size = 0xAF28 - 0xAF27;
		decode[5].key = 0x94;
		decode[5].size = 0x551FBA6 - 0xAF28;
	}
	// 2�ڂ̃t�@�C�����f�R�[�h���邽�߂̏���
	else {
		decode[0].key = 0x5A;
		decode[0].size = 0x552036D - pInfFile->start;
		decode[1].key = 0x21;
		decode[1].size = 0x552254A - 0x552036D;
		decode[2].key = 0xFE;
		decode[2].size = 0x552254B - 0x552254A;
		decode[3].key = 0x21;
		decode[3].size = 0x552CDF6 - 0x552254B;
		decode[4].key = 0xDB;
		decode[4].size = 0x552CDF7 - 0x552CDF6;
		decode[5].key = 0x21;
		decode[5].size = 0x90693AA - 0x552CDF7;
	}

	DWORD BufSize_base = BufSize;
	for (int i = 0; i < 6; i++) {
		BufSize = BufSize_base;
		for (DWORD DecodeSize = 0; DecodeSize != decode[i].size; DecodeSize += BufSize) {
			// �o�b�t�@�T�C�Y����
			if (DecodeSize + BufSize > decode[i].size)
				BufSize = decode[i].size - DecodeSize;

			// �ǂݍ���
			pclArc->Read(&buf[0], BufSize);
			// ������
			for (int j = 0; j < (int)BufSize; j++)
				buf[j] ^= decode[i].key;
			// �������������̂���������
			pclArc->WriteFile(&buf[0], BufSize);
		}
	}

	return TRUE;
}