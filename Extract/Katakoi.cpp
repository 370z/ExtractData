#include "stdafx.h"
#include "../ExtractBase.h"
#include "../Image.h"
#include "../Wav.h"
#include "../Ogg.h"
#include "Katakoi.h"

BOOL CKatakoi::Mount(CArcFile* pclArc)
{
	if (MountIar(pclArc) == TRUE)
		return TRUE;
	if (MountWar(pclArc) == TRUE)
		return TRUE;

	return FALSE;
}

BOOL CKatakoi::MountIar(CArcFile* pclArc)
{
	if (pclArc->GetArcExten() != _T(".iar"))
		return FALSE;

	if (memcmp(pclArc->GetHed(), "iar ", 4) != 0)
		return FALSE;

	// Version check
	DWORD dwVersion = *(LPDWORD)&pclArc->GetHed()[4];
	DWORD dwFileEntrySize = 0;

	if (dwVersion == 2) {
		dwFileEntrySize = 4;
	}
	else if (dwVersion == 3) {
		dwFileEntrySize = 8;
	}
	else {
		return FALSE;
	}

	pclArc->SeekHed(0x1C);

	// Get number of files
	DWORD dwFiles;
	pclArc->Read(&dwFiles, 4);

	// Get index size
	DWORD dwIndexSize = dwFiles * dwFileEntrySize;

	// Get index
	YCMemory<BYTE> clmbtIndex(dwIndexSize);
	pclArc->Read(&clmbtIndex[0], dwIndexSize);

	// Get index filename
	YCMemory<BYTE> clmbtSec;
	DWORD dwNameIndex;

	BOOL bSec = GetNameIndex(pclArc, clmbtSec, dwNameIndex);

	// File information retrieval

	TCHAR szFileName[_MAX_FNAME];
	TCHAR szWork[_MAX_FNAME];

	if (!bSec) {
		// Failed to get the filename index

		lstrcpy(szWork, pclArc->GetArcName());
		PathRemoveExtension(szWork);
	}

	for (DWORD i = 0; i < dwFiles; i++) {
		if (!bSec) {
			// Create a sequential filename
			_stprintf(szFileName, _T("%s_%06u"), szWork, i);
		}
		else {
			// Get the name of the file from the filename index
			lstrcpy(szFileName, (LPCTSTR)&clmbtSec[dwNameIndex]);

			dwNameIndex += strlen((char*)&clmbtSec[dwNameIndex]) + 1;	// Filename
			dwNameIndex += strlen((char*)&clmbtSec[dwNameIndex]) + 1;	// File type
			dwNameIndex += strlen((char*)&clmbtSec[dwNameIndex]) + 1;	// Archive type
			dwNameIndex += 4 + *(LPDWORD)&clmbtSec[dwNameIndex];		// Archive name length + Archive name + File number
		}

		SFileInfo stfiWork;

		stfiWork.name = szFileName;
		stfiWork.start = *(LPDWORD)&clmbtIndex[i * dwFileEntrySize];
		stfiWork.end = ((i + 1) < dwFiles) ? *(LPDWORD)&clmbtIndex[(i+1) * dwFileEntrySize] : pclArc->GetArcSize();
		stfiWork.sizeCmp = stfiWork.end - stfiWork.start;
		stfiWork.sizeOrg = stfiWork.sizeCmp;

		pclArc->AddFileInfo(stfiWork);
	}

	return TRUE;
}

BOOL CKatakoi::MountWar(CArcFile* pclArc)
{
	if (pclArc->GetArcExten() != _T(".war"))
		return FALSE;

	if (memcmp(pclArc->GetHed(), "war ", 4) != 0)
		return FALSE;

	// Version check
	DWORD dwVersion = *(LPDWORD)&pclArc->GetHed()[4];
	DWORD dwFileEntrySize = 0;

	if (dwVersion == 8) {
		dwFileEntrySize = 24;
	}
	else {
		return FALSE;
	}

	pclArc->SeekHed(0x08);

	// Get the number of files
	DWORD dwFiles;
	pclArc->Read(&dwFiles, 4);

	// Get index size
	DWORD dwIndexSize = dwFiles * dwFileEntrySize;

	// Get index
	YCMemory<BYTE> clmbtIndex(dwIndexSize);
	pclArc->SeekCur(0x04);
	pclArc->Read(&clmbtIndex[0], dwIndexSize);

	// Get the filename index
	YCMemory<BYTE> clmbtSec;
	DWORD dwNameIndex;

	BOOL bSec = GetNameIndex(pclArc, clmbtSec, dwNameIndex);

	// �t�@�C�����C���f�b�N�X���擾�ł������ǂ������A�[�J�C�u���Ƃɐݒ�(�f�R�[�h�̍����������Ɏg�p)
	pclArc->SetFlag(bSec);

	// Getting file info

	TCHAR szFileName[_MAX_FNAME];
	TCHAR szWork[_MAX_FNAME];

	if (!bSec) {
		// Failed to get the filename index

		lstrcpy(szWork, pclArc->GetArcName());
		PathRemoveExtension(szWork);
	}

	for (DWORD i = 0; i < dwFiles; i++) {
		if (!bSec) {
			// Create a sequential filename
			_stprintf(szFileName, _T("%s_%06u"), szWork, i);
		}
		else {
			// Get filename from the filename index
			lstrcpy(szFileName, (LPCTSTR)&clmbtSec[dwNameIndex]);

			dwNameIndex += strlen((char*)&clmbtSec[dwNameIndex]) + 1;	// File name
			dwNameIndex += strlen((char*)&clmbtSec[dwNameIndex]) + 1;	// File type
			dwNameIndex += strlen((char*)&clmbtSec[dwNameIndex]) + 1;	// Archive type
			dwNameIndex += 4 + *(LPDWORD)&clmbtSec[dwNameIndex];		// Archive name length + Archive name + File number
		}

		SFileInfo stfiWork;

		stfiWork.name = szFileName;
		stfiWork.start = *(LPDWORD)&clmbtIndex[i * dwFileEntrySize];
		stfiWork.sizeCmp = *(LPDWORD)&clmbtIndex[i * dwFileEntrySize + 4];
		stfiWork.sizeOrg = stfiWork.sizeCmp;
		stfiWork.end = stfiWork.start + stfiWork.sizeCmp;
		pclArc->AddFileInfo(stfiWork);
	}

	return TRUE;
}

BOOL CKatakoi::GetNameIndex(CArcFile* pclArc, YCMemory<BYTE>& clmbtSec, DWORD& dwNameIndex)
{
	// �t�@�C�����C���f�b�N�X�t�@�C�����J��

	TCHAR szPathToSec[MAX_PATH];

	if (GetPathToSec(szPathToSec, pclArc->GetArcPath()) == FALSE) {
		// sec5�t�@�C����������Ȃ�����

//		MessageBox(pclArc->GetProg()->GetHandle(), _T("sec5�t�@�C����������܂���B\n�C���X�g�[���t�H���_����sec5�t�@�C�������݂��Ă��Ȃ��\��������܂��B"), _T("�G���["), MB_OK);
		return FALSE;
	}

	CFile clfSec;

	if (!clfSec.OpenForRead(szPathToSec)) {
		// sec5�t�@�C���̃I�[�v���Ɏ��s

		return FALSE;
	}

	DWORD dwSecSize = clfSec.GetFileSize();

	// �ǂݍ���
	clmbtSec.resize(dwSecSize);
	clfSec.Read(&clmbtSec[0], dwSecSize);

	if (memcmp(&clmbtSec[0], "SEC5", 4) != 0) {
		// sec5�t�@�C�����������Ȃ�

		TCHAR szError[MAX_PATH * 2];

		_stprintf(szError, _T("%s ������������܂���B"), szPathToSec);
//		MessageBox(pclArc->GetProg()->GetHandle(), szError, _T("�G���["), MB_OK);

		return FALSE;
	}

	// RESR��T��

	for (dwNameIndex = 8; dwNameIndex < dwSecSize; ) {
		if (memcmp(&clmbtSec[dwNameIndex], "RESR", 4) == 0) {
			// "RESR"����������

			DWORD dwNameIndexSize = *(LPDWORD)&clmbtSec[dwNameIndex + 4];
			DWORD dwNameIndexFiles = *(LPDWORD)&clmbtSec[dwNameIndex + 8];

			dwNameIndex += 12;

			// �A�[�J�C�u���ƈ�v����C���f�b�N�X��T��

			for (DWORD i = 0; i < dwNameIndexFiles; i++) {
				DWORD dwWork = 0;
				dwWork += strlen((char*)&clmbtSec[dwNameIndex + dwWork]) + 1;	// �t�@�C����
				dwWork += strlen((char*)&clmbtSec[dwNameIndex + dwWork]) + 1;	// �t�@�C���^�C�v
				dwWork += strlen((char*)&clmbtSec[dwNameIndex + dwWork]) + 1;	// �A�[�J�C�u�^�C�v

				DWORD dwLength = *(LPDWORD)&clmbtSec[dwNameIndex + dwWork];		// �A�[�J�C�u�� + �t�@�C���ԍ���
				dwWork += 4;

				for (DWORD i = (dwNameIndex + dwWork); ; i++) {
					if (clmbtSec[i] == '\0') {
						// �A�[�J�C�u���ƈ�v����C���f�b�N�X�ł͂Ȃ�

						break;
					}
					if (lstrcmp((LPCTSTR)&clmbtSec[i], pclArc->GetArcName()) == 0) {
						// �A�[�J�C�u���ƈ�v����C���f�b�N�X����������

						// ���������̗L����

						if (lstrcmp(PathFindFileName(szPathToSec), _T("toa.sec5")) == 0) {
							// �Ǔނ����ɂ��肢
							pclArc->SetFlag(TRUE);
						}
						else if (lstrcmp(PathFindFileName(szPathToSec), _T("katakoi.sec5")) == 0) {
							// �З����̌�
							pclArc->SetFlag(TRUE);
						}

						return TRUE;
					}
				}

				dwNameIndex += dwWork + dwLength;
			}

			break;
		}

		dwNameIndex += 8 + *(LPDWORD)&clmbtSec[dwNameIndex + 4];
	}

	// �A�[�J�C�u���ƈ�v����C���f�b�N�X��������Ȃ�����

//	MessageBox(pclArc->GetProg()->GetHandle(), _T("�t�@�C�����̎擾�Ɏ��s���܂����B\n�A�[�J�C�u�t�@�C�������ύX����Ă���\��������܂��B"), _T("�G���["), MB_OK);

	return FALSE;
}

BOOL CKatakoi::GetPathToSec(LPTSTR pszPathToSec, const YCString& strPathToArc)
{
	TCHAR szWork[MAX_PATH];

	lstrcpy(szWork, strPathToArc);
	PathRemoveFileSpec(szWork);
	PathAppend(szWork, _T("*.sec5"));

	// �A�[�J�C�u�t�H���_������sec5�t�@�C����T��

	HANDLE			hFind;
	WIN32_FIND_DATA	stwfdWork;

	hFind = FindFirstFile(szWork, &stwfdWork);

	if (hFind == INVALID_HANDLE_VALUE) {
		// �C���X�g�[���t�H���_��(�Ǝv����)����sec5�t�@�C����T��

		PathRemoveFileSpec(szWork);
		PathRemoveFileSpec(szWork);
		PathAppend(szWork, _T("*.sec5"));

		hFind = FindFirstFile(szWork, &stwfdWork);

		if (hFind == INVALID_HANDLE_VALUE) {
			// sec5�t�@�C����������Ȃ�����

			return FALSE;
		}
	}

	FindClose(hFind);

	lstrcpy(pszPathToSec, szWork);
	PathRemoveFileSpec(pszPathToSec);
	PathAppend(pszPathToSec, stwfdWork.cFileName);

	return TRUE;
}

BOOL CKatakoi::Decode(CArcFile* pclArc)
{
	if (DecodeIar(pclArc) == TRUE)
		return TRUE;
	if (DecodeWar(pclArc) == TRUE)
		return TRUE;

	return FALSE;
}

BOOL CKatakoi::DecodeIar(CArcFile* pclArc)
{
	if (pclArc->GetArcExten() != _T(".iar"))
		return FALSE;

	if (memcmp(pclArc->GetHed(), "iar ", 4) != 0)
		return FALSE;

	SFileInfo* pstfiWork = pclArc->GetOpenFileInfo();

	// �ǂݍ���
	YCMemory<BYTE> clmbtSrc(pstfiWork->sizeCmp);
	pclArc->Read(&clmbtSrc[0], pstfiWork->sizeCmp);

	// �o�̓o�b�t�@
	DWORD dwDstSize = *(LPDWORD)&clmbtSrc[8];
	YCMemory<BYTE> clmbtDst(dwDstSize * 2);

	// ��
	DecompImage(&clmbtDst[0], dwDstSize, &clmbtSrc[64], *(LPDWORD)&clmbtSrc[16]);

	long lWidth = *(LPLONG)&clmbtSrc[32];
	long lHeight = *(LPLONG)&clmbtSrc[36];
	WORD wBpp;

	switch (*(LPBYTE)&clmbtSrc[0]) {
	case 0x02:
		wBpp = 8;
		break;
	case 0x1C:
		wBpp = 24;
		break;
	case 0x3C:
		wBpp = 32;
		break;
	default:
		return FALSE;
	}

	BOOL bDiff = (*(LPBYTE)&clmbtSrc[1] == 8);

	if (bDiff) {
		// �����t�@�C��
		DecodeCompose(pclArc, &clmbtDst[0], dwDstSize, lWidth, lHeight, wBpp);
	}
	else {
		CImage image;
		image.Init(pclArc, lWidth, lHeight, wBpp);
		image.WriteReverse(&clmbtDst[0], dwDstSize);
	}

	return TRUE;
}

BOOL CKatakoi::DecodeWar(CArcFile* pclArc)
{
	if (pclArc->GetArcExten() != _T(".war"))
		return FALSE;

	if (memcmp(pclArc->GetHed(), "war ", 4) != 0)
		return FALSE;

	SFileInfo* pstfiWork = pclArc->GetOpenFileInfo();

	// �ǂݍ���
	YCMemory<BYTE> clmbtSrc(pstfiWork->sizeCmp);
	pclArc->Read(&clmbtSrc[0], pstfiWork->sizeCmp);

	if (memcmp(&clmbtSrc[0], "OggS", 4) == 0) {
		// Ogg Vorbis

		COgg ogg;
		ogg.Decode(pclArc, &clmbtSrc[0]);
	}
	else {
		// WAV(�ƒf��)

		DWORD	dwDataSize = *(LPDWORD)&clmbtSrc[4];
		DWORD	dwFreq = *(LPDWORD)&clmbtSrc[12];
		WORD	wChannels = *(LPWORD)&clmbtSrc[10];
		WORD	wBits = *(LPWORD)&clmbtSrc[22];

		CWav wav;
		wav.Init(pclArc, dwDataSize, dwFreq, wChannels, wBits);
		wav.Write(&clmbtSrc[24]);
	}

	return TRUE;
}

void CKatakoi::GetBit(LPBYTE& pbySrc, DWORD& dwFlags)
{
	dwFlags >>= 1;

	if (dwFlags <= 0xFFFF) {
		// 0xFFFF�ȉ��ɂȂ���

		dwFlags = *(LPWORD)&pbySrc[0] | 0xFFFF0000;
		pbySrc += 2;
	}
}

BOOL CKatakoi::DecompImage(LPBYTE pbyDst, DWORD dwDstSize, LPBYTE pbySrc, DWORD dwSrcSize)
{
	DWORD dwFlags = 0;		// �t���O�͕K�����������邱��
	DWORD dwBack;
	DWORD dwLength;
	DWORD dwWork;
	LPBYTE pbyDstBegin = pbyDst;
	LPBYTE pbySrcEnd = pbySrc + dwSrcSize;
	LPBYTE pbyDstEnd = pbyDst + dwDstSize;

	while ((pbySrc < pbySrcEnd) && (pbyDst < pbyDstEnd)) {
		GetBit(pbySrc, dwFlags);

		if (dwFlags & 1) {
			// �����k�f�[�^

			*pbyDst++ = *pbySrc++;
		}
		else {
			// ���k�f�[�^

			GetBit(pbySrc, dwFlags);

			if (dwFlags & 1) {
				// ���k�p�^�[��1(���k�f�[�^��3�o�C�g�ȏ�)

				// �߂�o�C�g�������߂�

				GetBit(pbySrc, dwFlags);

				// 1�o�C�g�v���X

				dwWork = 1;
				dwBack = dwFlags & 1;

				GetBit(pbySrc, dwFlags);

				if ((dwFlags & 1) == 0) {
					// 0x201�o�C�g�v���X

					GetBit(pbySrc, dwFlags);

					dwWork = 0x201;

					if ((dwFlags & 1) == 0) {
						// 0x401�o�C�g�v���X

						GetBit(pbySrc, dwFlags);

						dwWork = 0x401;
						dwBack = (dwBack << 1) | (dwFlags & 1);

						GetBit(pbySrc, dwFlags);

						if ((dwFlags & 1) == 0) {
							// 0x801�o�C�g�v���X

							GetBit(pbySrc, dwFlags);

							dwWork = 0x801;
							dwBack = (dwBack << 1) | (dwFlags & 1);

							GetBit(pbySrc, dwFlags);

							if ((dwFlags & 1) == 0) {
								// 0x1001�o�C�g�v���X

								GetBit(pbySrc, dwFlags);

								dwWork = 0x1001;
								dwBack = (dwBack << 1) | (dwFlags & 1);
							}
						}
					}
				}

				dwBack = ((dwBack << 8) | *pbySrc++) + dwWork;

				// ���k�o�C�g�������߂�

				GetBit(pbySrc, dwFlags);

				if (dwFlags & 1) {
					// 3�o�C�g�̈��k�f�[�^

					dwLength = 3;
				}
				else {
					GetBit(pbySrc, dwFlags);

					if (dwFlags & 1) {
						// 4�o�C�g�̈��k�f�[�^

						dwLength = 4;
					}
					else {
						GetBit(pbySrc, dwFlags);

						if (dwFlags & 1) {
							// 5�o�C�g�̈��k�f�[�^

							dwLength = 5;
						}
						else {
							GetBit(pbySrc, dwFlags);

							if (dwFlags & 1) {
								// 6�o�C�g�̈��k�f�[�^

								dwLength = 6;
							}
							else {
								GetBit(pbySrc, dwFlags);

								if (dwFlags & 1) {
									// 7�`8�o�C�g�̈��k�f�[�^

									GetBit(pbySrc, dwFlags);

									dwLength = (dwFlags & 1);
									dwLength += 7;
								}
								else {
									GetBit(pbySrc, dwFlags);

									if (dwFlags & 1) {
										// 17�o�C�g�ȏ�̈��k�f�[�^

										dwLength = *pbySrc++ + 0x11;
									}
									else {
										// 9�`16�o�C�g�̈��k�f�[�^

										GetBit(pbySrc, dwFlags);
										dwLength = (dwFlags & 1) << 2;

										GetBit(pbySrc, dwFlags);
										dwLength |= (dwFlags & 1) << 1;

										GetBit(pbySrc, dwFlags);
										dwLength |= (dwFlags & 1);

										dwLength += 9;
									}
								}
							}
						}
					}
				}
			}
			else {
				// ���k�p�^�[��2(���k�f�[�^��2�o�C�g)

				dwLength = 2;

				// �߂�o�C�g�������߂�

				GetBit(pbySrc, dwFlags);

				if (dwFlags & 1) {
					GetBit(pbySrc, dwFlags);
					dwBack = (dwFlags & 1) << 0x0A;

					GetBit(pbySrc, dwFlags);
					dwBack |= (dwFlags & 1) << 0x09;

					GetBit(pbySrc, dwFlags);
					dwBack |= (dwFlags & 1) << 0x08;

					dwBack |= *pbySrc++;
					dwBack += 0x100;
				}
				else {
					dwBack = *pbySrc++ + 1;

					if (dwBack == 0x100) {
						// �I��

						break;
					}
				}
			}

			// ���k�f�[�^�̓W�J

			if (dwBack > (pbyDst - pbyDstBegin)) {
				return FALSE;
			}

			LPBYTE pbyWorkOfDst = pbyDst - dwBack;

			for (DWORD k = 0; (k < dwLength) && (pbyDst < pbyDstEnd) && (pbyWorkOfDst < pbyDstEnd); k++) {
				*pbyDst++ = *pbyWorkOfDst++;
			}
		}
	}

	return TRUE;
}

BOOL CKatakoi::DecodeCompose(CArcFile* pclArc, LPBYTE pbyDiff, DWORD dwDiffSize, long lWidthForDiff, long lHeightForDiff, WORD wBppForDiff)
{
	SFileInfo*			pstfiDiff = pclArc->GetOpenFileInfo();
	DWORD				dwDiffNum = pclArc->GetOpenFileInfoNum();

	const SFileInfo*		pstfiBase;
	BOOL				bExistsForBase = FALSE;
	TCHAR				szFileNameForBase[MAX_PATH];

	lstrcpy(szFileNameForBase, pstfiDiff->name);

	LPTSTR				pszFileNumberFordiff1 = &szFileNameForBase[lstrlen(szFileNameForBase) - 1];
	LPTSTR				pszFileNumberFordiff2 = &szFileNameForBase[lstrlen(szFileNameForBase) - 2];

	// �A�Ԃ𐔒l�ɕϊ�

	long				lFileNumberForDiff1 = _tcstol(pszFileNumberFordiff1, NULL, 10);
	long				lFileNumberForDiff2 = _tcstol(pszFileNumberFordiff2, NULL, 10);

	if (pclArc->GetFlag()) {
		// �x�[�X�t�@�C���̌���(�����t�@�C�����O������)

		long		lFileNumberForBase = lFileNumberForDiff1;
		long		lCount = lFileNumberForDiff1;

		while (!bExistsForBase) {
			lFileNumberForBase--;
			lCount--;

			if (lCount < 0) {
				// �����I��

				break;
			}

			_stprintf(pszFileNumberFordiff1, _T("%d"), lFileNumberForBase);

			pstfiBase = pclArc->GetFileInfo(szFileNameForBase);

			if (pstfiBase == NULL) {
				// ���ԃt�@�C��

				continue;
			}

			BYTE		byWork;

			pclArc->SeekHed(pstfiBase->start + 1);
			pclArc->Read(&byWork, 1);

			if (byWork == 0) {
				// �x�[�X�t�@�C������������

				bExistsForBase = TRUE;
			}
		}

		// �x�[�X�t�@�C���̌���(�����t�@�C�����������)

		lFileNumberForBase = lFileNumberForDiff1;
		lCount = lFileNumberForDiff1;

		while (!bExistsForBase) {
			lFileNumberForBase++;
			lCount++;

			if (lCount >= 10) {
				// �����I��

				break;
			}

			_stprintf(pszFileNumberFordiff1, _T("%d"), lFileNumberForBase);

			pstfiBase = pclArc->GetFileInfo(szFileNameForBase);

			if (pstfiBase == NULL) {
				// ���ԃt�@�C��

				continue;
			}

			BYTE		byWork;

			pclArc->SeekHed(pstfiBase->start + 1);
			pclArc->Read(&byWork, 1);

			if (byWork == 0) {
				// �x�[�X�t�@�C������������

				bExistsForBase = TRUE;
			}
		}

		// �x�[�X�t�@�C���̌���(2���ڂ�1�߂��Č���)

		lFileNumberForBase = (lFileNumberForDiff2 / 10) * 10;
		lCount = 10;

		while (!bExistsForBase) {
			lFileNumberForBase--;
			lCount--;

			if (lCount < 0) {
				// �����I��

				break;
			}

			_stprintf(pszFileNumberFordiff2, _T("%02d"), lFileNumberForBase);

			pstfiBase = pclArc->GetFileInfo(szFileNameForBase);

			if (pstfiBase == NULL) {
				// ���ԃt�@�C��

				continue;
			}

			BYTE		byWork;

			pclArc->SeekHed(pstfiBase->start + 1);
			pclArc->Read(&byWork, 1);

			if (byWork == 0) {
				// �x�[�X�t�@�C������������

				bExistsForBase = TRUE;
			}
		}
	}

	if (bExistsForBase) {
		// �x�[�X�t�@�C�������݂���

		YCMemory<BYTE>		clmbtSrcForBase(pstfiBase->sizeCmp);

		pclArc->SeekHed(pstfiBase->start);
		pclArc->Read(&clmbtSrcForBase[0], pstfiBase->sizeCmp);

		long		lWidthForBase = *(LPLONG)&clmbtSrcForBase[32];
		long		lHeightForBase = *(LPLONG)&clmbtSrcForBase[36];

		if ((lWidthForBase >= lWidthForDiff) && (lHeightForBase >= lHeightForDiff)) {
			// �x�[�X�̂ق����傫��

			DWORD				dwDstSizeForBase = *(LPDWORD)&clmbtSrcForBase[8];
			YCMemory<BYTE>		clmbtDstForBase(dwDstSizeForBase);

			// �x�[�X�t�@�C���̉�

			DecompImage(&clmbtDstForBase[0], dwDstSizeForBase, &clmbtSrcForBase[64], *(LPDWORD)&clmbtSrcForBase[16]);

			// �x�[�X�t�@�C���ƍ����t�@�C��������

			Compose(&clmbtDstForBase[0], dwDstSizeForBase, pbyDiff, dwDiffSize, lWidthForBase, lWidthForDiff, wBppForDiff);

			// �o��

			CImage				cliWork;
			long				lWidth = *(LPLONG)&clmbtSrcForBase[32];
			long				lHeight = *(LPLONG)&clmbtSrcForBase[36];
			WORD				wBpp = wBppForDiff;

			cliWork.Init(pclArc, lWidth, lHeight, wBpp);
			cliWork.WriteReverse(&clmbtDstForBase[0], dwDstSizeForBase);

			return TRUE;
		}
		else if ((lWidthForDiff >= lWidthForBase) && (lHeightForDiff >= lHeightForBase)) {
			// �����̕����傫��

			DWORD				dwDstSizeForBase = *(LPDWORD)&clmbtSrcForBase[8];
			YCMemory<BYTE>		clmbtDstForBase(dwDstSizeForBase);

			// �x�[�X�t�@�C���̉�

			DecompImage(&clmbtDstForBase[0], dwDstSizeForBase, &clmbtSrcForBase[64], *(LPDWORD)&clmbtSrcForBase[16]);

			// �����̃T�C�Y�Ń������m��

			DWORD				dwDstSize = lWidthForDiff * lHeightForDiff * (wBppForDiff >> 3);
			YCMemory<BYTE>		clmbtDst(dwDstSize);

			memset(&clmbtDst[0], 0, dwDstSize);

			// �x�[�X�t�@�C�����E���ɍ��킹�Ĕz�u

			long				lX = lWidthForDiff - lWidthForBase;
			long				lY = lHeightForDiff - lHeightForBase;
			LPBYTE				pbyDstForBase = &clmbtDstForBase[0];
			LPBYTE				pbyDst = &clmbtDst[0];

			long				lGapForX = lX * (wBppForDiff >> 3);
			long				lLineForBase = lWidthForBase * (wBppForDiff >> 3);
			long				lLineForDiff = lWidthForDiff * (wBppForDiff >> 3);

			// �c�ʒu�����ɍ��킹��
			pbyDst += lY * lLineForDiff;

			for (long y = lY; y < lHeightForDiff; y++) {
				// ���ʒu���E�ɍ��킹��
				pbyDst += lGapForX;

				memcpy(pbyDst, pbyDstForBase, lLineForBase);

				pbyDst += lLineForBase;
				pbyDstForBase += lLineForBase;
			}

			// �x�[�X�t�@�C���ƍ����t�@�C��������

			Compose(&clmbtDst[0], dwDstSize, pbyDiff, dwDiffSize, lWidthForDiff, lWidthForDiff, wBppForDiff);

			// �o��

			CImage				cliWork;
			long				lWidth = lWidthForDiff;
			long				lHeight = lHeightForDiff;
			WORD				wBpp = wBppForDiff;

			cliWork.Init(pclArc, lWidth, lHeight, wBpp);
			cliWork.WriteReverse(&clmbtDst[0], dwDstSize);

			return TRUE;
		}
	}

	// �x�[�X�t�@�C�������݂��Ȃ�

	long				lWidth = lWidthForDiff;
	long				lHeight = lHeightForDiff;
	WORD				wBpp = wBppForDiff;

	// �^�����̃f�[�^��p��

	DWORD				dwDstSize = ((lWidth * (wBpp >> 3) + 3) & 0xFFFFFFFC) * lHeight;
	YCMemory<BYTE>		clmbtDst(dwDstSize);

	memset(&clmbtDst[0], 0, dwDstSize);

	// �^�����̃f�[�^�ƍ���

	Compose(&clmbtDst[0], dwDstSize, pbyDiff, dwDiffSize, lWidth, lWidth, wBpp);

	CImage				cliWork;

	cliWork.Init(pclArc, lWidth, lHeight, wBpp);
	cliWork.WriteReverse(&clmbtDst[0], dwDstSize);

	// �G���[���b�Z�[�W�̏o��
/*
	if (pclArc->GetFlag()) {
		TCHAR				szError[1024];

		_stprintf(szError, _T("%s\n\n���摜��������Ȃ��������߁A�����������s�킸�ɏo�͂��܂����B"), pstfiDiff->name);

		MessageBox(pclArc->GetProg()->GetHandle(), szError, _T("��"), MB_OK);
	}
*/
	return FALSE;
}

//////////////////////////////////////////////////////////////////////////////////////////
//	���摜�ƍ����摜����������
//
//	�\�ꊦ�������쐬�E���J���Ă���iar�̃\�[�X�R�[�h���Q�l�ɂ��č쐬���܂����B

BOOL CKatakoi::Compose(LPBYTE pbyDst, DWORD dwDstSize, LPBYTE pbySrc, DWORD dwSrcSize, long lWidthForDst, long lWidthForSrc, WORD wBpp)
{
	WORD wColors = wBpp >> 3;
	DWORD dwLine = lWidthForSrc * wColors;
	DWORD dwHeight = *(LPDWORD)&pbySrc[8];

	DWORD dwGapForX = 0;

	if (lWidthForDst > lWidthForSrc) {
		dwGapForX = (lWidthForDst - lWidthForSrc) * wColors;
	}

	DWORD dwSrc = 12;
	DWORD dwDst = *(LPDWORD)&pbySrc[4] * (dwGapForX + dwLine);

	while ((dwHeight--) && (dwSrc < dwSrcSize)) {
		for (DWORD x = 0; x < dwGapForX; x++) {
			pbyDst[dwDst++] = 0;
		}

		DWORD dwCount = *(LPWORD)&pbySrc[dwSrc];
		dwSrc += 2;

		DWORD dwOffset = 0;

		while (dwCount--) {
			dwOffset += *(LPWORD)&pbySrc[dwSrc] * wColors;
			dwSrc += 2;

			DWORD dwLength = *(LPWORD)&pbySrc[dwSrc] * wColors;
			dwSrc += 2;

			while (dwLength--) {
				pbyDst[dwDst + dwOffset++] = pbySrc[dwSrc++];

				if ((dwDst + dwOffset) >= dwDstSize) {
					return TRUE;
				}
				if (dwSrc >= dwSrcSize) {
					return TRUE;
				}
			}
		}

		dwDst += dwLine;
	}

	return TRUE;
}