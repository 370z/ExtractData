#include "stdafx.h"
#include "../ExtractBase.h"
#include "Izumo.h"

// IZUMO��*.dat�̃t�@�C�������擾����֐�
BOOL CIzumo::Mount(CArcFile* pclArc)
{
	if ((pclArc->GetArcExten() != _T(".dat")) || (memcmp(pclArc->GetHed(), "PAK0", 4) != 0))
		return FALSE;

	// �C���f�b�N�X�T�C�Y�擾(�ŏ��̃t�@�C���̊J�n�A�h���X)
	DWORD index_size;
	pclArc->Seek(4, FILE_BEGIN);
	pclArc->Read(&index_size, 4);

	// �X�L�b�v����o�C�g���擾

	DWORD				dwDummys;
	DWORD				dwSkip;

	pclArc->Read( &dwDummys, 4 );
	dwSkip = dwDummys << 3;

	// �t�@�C�����擾
	DWORD ctFile;
	pclArc->Read(&ctFile, 4);

	// �s���ȃf�[�^���X�L�b�v
	pclArc->Seek(dwSkip, FILE_CURRENT);

	// �X�L�b�v����������
	index_size -= 16 + dwSkip;

	// �C���f�b�N�X�擾
	YCMemory<BYTE> index(index_size);
	LPBYTE pIndex = &index[0];
	pclArc->Read(pIndex, index_size);

	// �t�@�C�����C���f�b�N�X�擾
	LPBYTE pFileNameIndex = pIndex + (ctFile << 4);

	// �s�v�ȃt�@�C�����̏���

	for( DWORD i = 1 ; i < dwDummys ; i++ )
	{
		pFileNameIndex += pFileNameIndex[0] + 1;
	}

	for (int i = 0; i < (int)ctFile; i++) {
		// �t�@�C�����擾
		TCHAR szFileName[256];
		BYTE len = *pFileNameIndex++;
		memcpy(szFileName, pFileNameIndex, len);
		szFileName[len] = _T('\0');

		// ���X�g�r���[�ɒǉ�
		SFileInfo infFile;
		infFile.name = szFileName;
		infFile.sizeOrg = *(LPDWORD)&pIndex[4];
		infFile.sizeCmp = infFile.sizeOrg;
		infFile.start = *(LPDWORD)&pIndex[0];
		infFile.end = infFile.start + infFile.sizeCmp;
		pclArc->AddFileInfo(infFile);

		pIndex += 16;
		pFileNameIndex += len;
	}

	return TRUE;
}