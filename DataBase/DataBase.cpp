#include "stdafx.h"
#include "../Common.h"
#include "../ExtractData.h"
#include "../Ctrl/ListView.h"
#include "../Ctrl/Button.h"
#include "../Ctrl/Font.h"
#include "../ArcFile.h"
#include "DataBase.h"

CDataBase::CDataBase()
{
	// �f�[�^�x�[�X�t�@�C�����t���p�X�Ŏ擾
	TCHAR modulePath[MAX_PATH];
	GetModuleFileName(NULL, modulePath, MAX_PATH);
	PathRemoveFileSpec(modulePath);
	wsprintf(m_DBFilePath, _T("%s\\database.dat"), modulePath);
}

CDataBase::~CDataBase()
{

}

// �f�[�^�x�[�X����t�@�C���p�X�ƃt�@�C�������擾����֐�
BOOL CDataBase::ReadFileInfo()
{/*
	CFile DBFile;
	if (DBFile.Open(m_DBFilePath, FILE_READ) == INVALID_HANDLE_VALUE)
		return FALSE;

	tstring buf;
	while (DBFile.ReadLine(buf) == TRUE) {
		if ((buf >= _T("A:")) && (buf <= _T("Z:"))) {
			FILEINFODB infFile = {0, buf, 0};

			// �t�@�C���ʒu�擾
			size_t offset = buf.length();
			file.start = DataBaseFile.tellg();
			file.start -= offset + 2;

			// �t�@�C�����e�擾
			while (getline(DataBaseFile, buf)) {
				if (buf == _T(""))
					break;
				file.count++;
			}

			DBFileInfoList->push_back(file);
		}
	}
*/
	return 0;
}

// �I���A�C�e�����폜����֐�
int CDataBase::DelFileInfo()
{
	//HWND DataBaseList = m_DataBaseList;
	//std::vector<DBFI> *pDBFileInfoList = &m_DBFileInfoList;

	//// �����I�����Ă��Ȃ���΍폜���Ȃ�
	//if (ListView_GetSelectedCount(DataBaseList) <= 0)
	//	return -1;

	//// UNDO�擾
	//GetUndo();

	//// �I������Ă��Ȃ��A�C�e�����擾
	//std::vector<DBFI> NoSelectedItemList;
	//int FocusedItem = -1, FirstSelectedItem = -1;
	//int nItemCount = pDBFileInfoList->size();
	//for (int i = 0; i < nItemCount; i++) {
	//	UINT uiState = ListView_GetItemState(DataBaseList, i, LVIS_SELECTED | LVIS_FOCUSED);
	//	// �t�H�[�J�X��Ԃ̃A�C�e���ԍ��擾
	//	if ((FocusedItem == -1) && (uiState & LVIS_FOCUSED))
	//		FocusedItem = i;
	//	if ((FirstSelectedItem == -1) && (uiState & LVIS_SELECTED))
	//		FirstSelectedItem = i;
	//	else if (!(uiState & LVIS_SELECTED))
	//		NoSelectedItemList.push_back((*pDBFileInfoList)[i]);
	//}

	//// DBFileInfoList�ɑI������Ă��Ȃ��A�C�e�����i�[���邱�ƂŁA�I�����ꂽ�A�C�e�����폜���邱�Ƃ��ł���(erase���ƒx��)
	//pDBFileInfoList->clear();
	//*pDBFileInfoList = NoSelectedItemList;
	//NoSelectedItemList.clear();

	//SendMessage(DataBaseList, WM_SETREDRAW, FALSE, 0);

	//// ���X�g�r���[�X�V
	//ListView listview(m_hWnd);
	//listview.Update(DataBaseList, *pDBFileInfoList, FirstSelectedItem);

	//nItemCount = pDBFileInfoList->size();
	//// �A�����č폜�o����悤�ɃA�C�e����I����Ԃɂ���
	//if (FocusedItem < nItemCount) { // ���̂������ʂ����Ȃ��ƃR���p�C���ł��Ȃ�
	//	ListView_SetItemState(DataBaseList, FocusedItem, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
	//}
	//// �����ڐ��𒴂��Ă������ԉ��̃A�C�e���Ƀt�H�[�J�X
	//else
	//	ListView_SetItemState(DataBaseList, nItemCount - 1, LVIS_FOCUSED, LVIS_FOCUSED);

	//SendMessage(DataBaseList, WM_SETREDRAW, TRUE, 0);

	//// REDO�擾
	//GetRedo();

	return 0;
}

// �ҏW��̓��e�ɏ]���ăf�[�^�x�[�X��ۑ�
BOOL CDataBase::SaveFileInfo()
{
	//CFile DBFile;
	//if (DBFile.Open(m_DBFilePath, FILE_READ) == INVALID_HANDLE_VALUE)
	//	return FALSE

	//std::vector<DBFI>& pEntDB = m_entDB;

	//// �f�[�^�x�[�X�t�@�C���̗e�ʎ擾
	//QWORD FileSize = DBFile.GetFileSize();

	//// �t�@�C���T�C�Y��0��������ۑ�����K�v���Ȃ��̂ŏI��
	//if (FileSize == 0)
	//	return -1;

	//try {
	//	// �e�ʕ��̃����������蓖�Ă�
	//	char *str = new char[FileSize + 1];
	//	ZeroMemory(str, FileSize + 1);
	//	char *pstr = str;

	//	// str�ɕۑ�����f�[�^�x�[�X��ǂݍ���
	//	size_t FileCount = pDBFileInfoList->size();
	//	for (size_t i = 0; i < FileCount; i++) {
	//		rDataBaseFile.seekg((*pDBFileInfoList)[i].start);

	//		std::string buf;
	//		while (getline(rDataBaseFile, buf)) {
	//			buf += "\n";
	//			size_t len = buf.length();
	//			for (size_t j = 0; j < len; j++)
	//				*pstr++ = buf[j];
	//			if (buf == "\n")
	//				break;
	//		}
	//	}

	//	// database.txt�ɏ�������
	//	std::ofstream wDataBaseFile(m_DataBaseName);
	//	wDataBaseFile << str;

	//	delete []str;
	//}
	//catch (std::bad_alloc) {
	//	TCHAR DataBaseName_tmp[MAX_PATH];
	//	lstrcpy(DataBaseName_tmp, m_DataBaseName);
	//	PathRenameExtension(DataBaseName_tmp, _T(".tmp"));
	//	CopyFile(m_DataBaseName, DataBaseName_tmp, FALSE);

	//	std::ifstream DataBaseFile_tmp(DataBaseName_tmp);
	//	std::ofstream DataBaseFile(m_DataBaseName);

	//	// database.txt�ɏ�������
	//	size_t FileCount = pDBFileInfoList->size();
	//	for (DWORD i = 0; i < FileCount; i++) {
	//		DataBaseFile_tmp.seekg((*pDBFileInfoList)[i].start);

	//		std::string buf;
	//		while (getline(DataBaseFile_tmp, buf)) {
	//			DataBaseFile << buf << '\n';
	//			if (buf == "")
	//				break;
	//		}
	//	}
	//}

	return 0;
}

// �I�������A�C�e���ԍ������X�g�Ɋi�[����֐�
int CDataBase::GetSelItem(std::vector<int> *SelectedItemList)
{
	//HWND DataBaseList = m_DataBaseList;

	//if (ListView_GetSelectedCount(DataBaseList) <= 0)
	//	return -1;

	//int nItem = -1;
	//while (1) {
	//	nItem = ListView_GetNextItem(DataBaseList, nItem, LVNI_ALL | LVNI_SELECTED);
	//	if (nItem == -1)
	//		break;
	//	SelectedItemList->push_back(nItem);
	//}

	return 0;
}

// UndoRedoList���N���A����֐�
void CDataBase::ClearUndoRedo()
{
	//std::vector<UndoRedo> *pUndoRedoList = &m_UndoRedoList;
	//if (!pUndoRedoList->empty()) {
	//	size_t size = pUndoRedoList->size();
	//	for (int i = 0; i < (int)size; i++) {
	//		(*pUndoRedoList)[i].DBFileInfoList.clear();
	//		(*pUndoRedoList)[i].SelectedItemList.clear();
	//	}
	//	pUndoRedoList->clear();
	//}
}

// UNDO���擾����֐�
int CDataBase::GetUndo()
{
	//// �O���UndoRedoList����
	//ClearUndoRedo();

	//HWND DataBaseList = m_DataBaseList;
	//std::vector<UndoRedo> *pUndoRedoList = &m_UndoRedoList;

	//// �V����Undo�쐬
	//UndoRedo newUndo;
	//newUndo.DBFileInfoList = m_DBFileInfoList;
	//GetSelItem(&newUndo.SelectedItemList);
	//newUndo.FocusedItem = ListView_GetNextItem(DataBaseList, -1, LVNI_ALL | LVNI_FOCUSED);
	//// UndoRedoList�ɒǉ�
	//pUndoRedoList->push_back(newUndo);

	return 0;
}

// REDO���擾����֐�
int CDataBase::GetRedo()
{
	//HWND DataBaseList = m_DataBaseList;
	//std::vector<UndoRedo> *pUndoRedoList = &m_UndoRedoList;

	//// �V����Redo�쐬
	//UndoRedo newRedo;
	//newRedo.DBFileInfoList = m_DBFileInfoList;
	//GetSelItem(&newRedo.SelectedItemList);
	//newRedo.FocusedItem = ListView_GetNextItem(DataBaseList, -1, LVNI_ALL | LVNI_FOCUSED);
	//// UndoRedoList�ɒǉ�
	//pUndoRedoList->push_back(newRedo);

	//m_Redo_flag = false;

	return 0;
}

// ���������s���֐�
int CDataBase::SetUndo()
{
	//HWND DataBaseList = m_DataBaseList;
	//std::vector<DBFI> *pDBFileInfoList = &m_DBFileInfoList;
	//std::vector<UndoRedo> *pUndoRedoList = &m_UndoRedoList;
	//bool Redo_flag = m_Redo_flag;

	//// UndoRedoList���󂾂����牽�����Ȃ�
	//if (pUndoRedoList->empty())
	//	return -1;

	//// ���X�g�A�C�e������
	//SendMessage(DataBaseList, WM_SETREDRAW, FALSE, 0);
	//*pDBFileInfoList = (*pUndoRedoList)[Redo_flag].DBFileInfoList;
	//ListView listview(m_hWnd);
	//listview.Update(DataBaseList, *pDBFileInfoList, ListView_GetTopIndex(DataBaseList));

	//// �I����ԕ���
	//size_t size = (*pUndoRedoList)[Redo_flag].SelectedItemList.size();
	//for (int i = 0; i < (int)size; i++)
	//	ListView_SetItemState(DataBaseList, (*pUndoRedoList)[Redo_flag].SelectedItemList[i], LVIS_SELECTED, LVIS_SELECTED);

	//// �t�H�[�J�X����
	//ListView_SetItemState(DataBaseList, (*pUndoRedoList)[Redo_flag].FocusedItem, LVIS_FOCUSED, LVIS_FOCUSED);
	//SendMessage(DataBaseList, WM_SETREDRAW, TRUE, 0);

	//Redo_flag ^= 1;
	//m_Redo_flag = Redo_flag;

	return 0;
}

void CDataBase::DoModal(HWND hWnd)
{
	HINSTANCE hInst = (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE);
	DialogBoxParam(hInst, _T("DATABASE"), hWnd, (DLGPROC)WndStaticProc, (LPARAM)this);
}

// �f�[�^�x�[�X�ҏW�_�C�A���O
LRESULT CDataBase::WndProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp)
{
	switch (msg) {
		case WM_INITDIALOG:
		{
			// �f�[�^�x�[�X���X�g�쐬
			m_listview.Create(hWnd, *m_pOption);
			// �e��{�^���쐬
			m_DelButton.Create(hWnd, _T("�폜"), ID_DB_BUTTON1);
			m_UndoButton.Create(hWnd, _T("���ɖ߂�"), ID_DB_BUTTON2);
			m_AllSelButton.Create(hWnd, _T("���ׂđI��"), ID_DB_BUTTON3);
			m_EndButton.Create(hWnd, _T("�ҏW�I��"), ID_DB_BUTTON4);
			// �e�E�B���h�E�̒����ɕ\��������W�����߂�
			RECT rc;
			GetWindowRect(GetParent(hWnd), (LPRECT)&rc);
			int width = 600;
			int height = 400;
			int left = rc.left + ((rc.right - rc.left) - width) / 2;
			int top = rc.top + ((rc.bottom - rc.top) - height) / 2;
			// �E�B���h�E�ʒu�E�T�C�Y�ݒ�
			//SaveLoadIni slini(hWnd);
			//slini.LoadWindowPlacement(idsDataBaseWindow, left, top, width, height);
			//slini.LoadListView(idsDataBaseList, &sort_param, 1);

			// �f�[�^�x�[�X�t�@�C���ǂݍ���
			ReadFileInfo();
			// �\�[�g���ĕ\��
			m_listview.Sort();

			SetFocus(hWnd);
			return FALSE;
		}

		case WM_COMMAND:
		{
			switch (LOWORD(wp)) {
				case ID_DB_BUTTON1: // �폜
				//case IDM_DEL:
					DelFileInfo();
					m_listview.SetFocus();
					return FALSE;

				case ID_DB_BUTTON2: // ������
				//case IDM_UNDO:
					SetUndo();
					m_listview.SetFocus();
					return FALSE;

				case ID_DB_BUTTON3: // ���ׂđI��
				//case IDM_SELECTALL:
					m_listview.SetAllItemSelect();
					m_listview.SetFocus();
					return FALSE;

				case ID_DB_BUTTON4: // �ҏW�I��
					SaveFileInfo();
				case IDCANCEL: // �L�����Z��
					m_EndButton.SetFocus(); // ������̂�h��

					//SaveLoadIni slini(hWnd);
					//slini.SaveWindowPlacement(idsDataBaseWindow);
					//slini.SaveListView(idsDataBaseList, m_DataBaseList, sort_param);

					EndDialog(hWnd, IDOK);
					return TRUE;
			}
			return FALSE;
		}

		case WM_PAINT:
		{
			PAINTSTRUCT ps;
			BeginPaint(hWnd, &ps);
			m_listview.SetFocus();
			EndPaint(hWnd, &ps);
			return FALSE;
		}

		case WM_NOTIFY:
		{
			if (wp == ID_LISTVIEW2) { //ID�`�F�b�N
				LPNMLISTVIEW pNM = (LPNMLISTVIEW)lp;
				switch (pNM->hdr.code) {
					case LVN_COLUMNCLICK:
						m_listview.Sort(pNM->iSubItem);
						return FALSE;

					case LVN_GETDISPINFO:
					{
						NMLVDISPINFO* pDispInfo = (NMLVDISPINFO*)lp;
						if (pDispInfo->item.mask & LVIF_TEXT)
							m_listview.Show(pDispInfo);
						return FALSE;
					}
				}
			}
			return FALSE;
		}

		case WM_SIZE:
			m_listview.SetWindowPos(20, 20, LOWORD(lp) - 130, HIWORD(lp) - 40);
			m_DelButton.SetWindowPos(LOWORD(lp) - 95, 70, 80, 20);
			m_UndoButton.SetWindowPos(LOWORD(lp) - 95, 110, 80, 20);
			m_AllSelButton.SetWindowPos(LOWORD(lp) - 95, 150, 80, 20);
			m_EndButton.SetWindowPos(LOWORD(lp) - 95, 190, 80, 20);
			return FALSE;
	}

	return FALSE;
}

// 1�x�ǂݍ��񂾂��Ƃ̂���t�@�C�����ǂ������f�[�^�x�[�X�t�@�C������`�F�b�N����֐�
BOOL CDataBase::Check(CArcFile* pArc)
{
	//OPTION* pOption = pArc->GetOpt();

	//// �f�[�^�x�[�X�@�\���g��Ȃ��ꍇ��FALSE��Ԃ��A�t�@�C����������擾����
	//if (pOption->bUseDB == FALSE)
	//	return FALSE;

	//CFileMap DBFile;
	//if (DBFile.Open(m_DBFilePath) == NULL)
	//	return FALSE;

	//// �w�b�_�`�F�b�N
	//if (memcmp(DBFile.GetFilePointer(), "EDDB", 4) != 0)
	//	return FALSE;

	//// �A�[�J�C�u���擾
	//DBFile.Seek(4, FILE_CURRENT);
	//LPDWORD pCtArc = (LPDWORD)DBFile.GetFilePointer();

	//// �C���f�b�N�X�T�C�Y�擾
	//DBFile.Seek(4, FILE_CURRENT);
	//LPDWORD pIndexSize = (LPDWORD)DBFile.GetFilePointer();

	//// �C���f�b�N�X�擾
	//DBFile.Seek(4, FILE_CURRENT);
	//LPBYTE pIndex = (LPBYTE)DBFile.GetFilePointer();

	//// �O�Ɉ�x�ǂݍ��񂾂��Ƃ��Ȃ����`�F�b�N
	//BOOL bOpen = FALSE;
	//DWORD IndexOffset = 0;
	//for (DWORD i = 0; i < *pCtArc; i++) {
	//	DWORD nArcPathLen = *(LPDWORD)&pIndex[0];
	//	if (memcmp(pArc->GetArcPath().c_str(), &pIndex[4], nArcPathLen) == 0) {
	//		bOpen = TRUE;
	//		break;
	//	}
	//	DWORD offset = strlen(pIndex) + 17;
	//	pIndex += offset;
	//	IndexOffset += offset;
	//}

	//// �ǂݍ��񂾂��Ƃ��Ȃ����FALSE��Ԃ�
	//if (bOpen == FALSE)
	//	return FALSE;

	

	//tstring buf;
	//while (DBFile.ReadLine(buf) == TRUE) {
	//	// �f�[�^�x�[�X�ɓ����t�@�C�������������瓯�t�@�C���Ɣ��f���A�f�[�^�x�[�X����t�@�C�����擾
	//	if (buf == pArc->GetArcPath()) {
	//		//std::vector<CFI> *pCFileInfoList = &g_CFileInfoList;
	//		size_t size_prev = pArc->GetFileInfo().size();
	//		tstring sFileExt(_T("."));
	//		sFileExt += pOption->szExtMode;

	//		while (DBFile.ReadLine(buf) == TRUE) {
	//			// ���s�܂œǂ񂾂甲����
	//			if (buf == _T(""))
	//				break;

	//			FILEINFO infFile;
	//			//TCHAR filename[_MAX_FNAME];
	//			//buf >> infFile.name >> infFile.sizeOrg >> infFile.start;
	//			std::istringstream is(buf);
	//			is >> infFile.name >> infFile.sizeOrg >> infFile.start;
	//			//sscanf(buf.c_str(), "%s %d %x", filename, &cfile.size, &cfile.start);

	//			// ���[�h�ɍ������g���q�̃t�@�C����񂾂��擾(�W�����[�h�̂Ƃ��͂��ׂĎ擾)
	//			if ((lstrcmp(pOption->szExtMode, _T("�W��")) == 0) || (lstrcmpi(PathFindExtension(infFile.name.c_str()), sFileExt.c_str()) == 0)) {
	//				infFile.sizeCmp = infFile.sizeOrg;
	//				infFile.end = infFile.start + infFile.sizeOrg;
	//				infFile.format = extract.SetFileFormat(cfile.name);
	//				pArc->SetFileInfo(infFile);
	//			}
	//			// MPG���o���[�h��sfd�t�@�C�����ǂ܂ꂽ��擾
	//			else if ((lstrcmp(pOption.ExtMode, _T("MPG")) == 0) && (lstrcmpi(PathFindExtension(infFile.name.c_str()), _T(".sfd")) == 0)) {
	//				infFile.sizeCmp = infFile.sizeOrg;
	//				infFile.end = infFile.start + infFile.sizeOrg;
	//				infFile.format = extract.SetFileFormat(cfile.name);
	//				pArc->SetFileInfo(infFile);
	//			}
	//		}

	//		// �����Ă��Ȃ������炱�̃��[�h�ł͓ǂݍ��񂾂��Ƃ��Ȃ��Ɣ��f
	//		if (size_prev == pArc->GetFileInfo().size())
	//			return FALSE;
	//		else
	//			return TRUE;
	//	}
	//}

	return FALSE;
}

// �ǂݍ��񂾏����f�[�^�x�[�X�t�@�C���ɏ������ފ֐�
BOOL CDataBase::Add(CArcFile* pArc)
{
	// �f�[�^�x�[�X�@�\���g��Ȃ��ꍇ�̓f�[�^�x�[�X�t�@�C���ɏ������܂Ȃ�
	//if (pArc->GetOpt()->bUseDB == FALSE)
	//	return FALSE;

	//std::vector<FILEINFO>& pEnt = pArc->GetFileInfo();
	//size_t nStartEnt = pArc->GetStartEnt();
	//size_t nEndEnt = pEnt.size();
	//size_t ctFile = nEndEnt - nStartEnt;

	//// �Ō�܂œǂݍ��݁A���[�h�ɍ������g���q�̃t�@�C�������������Ƃ��f�[�^�x�[�X�t�@�C���ɏ�������
	//if (ctFile == 0)
	//	return FALSE;

	//CFileMap DBFile;
	//if (DBFile.Open(m_DBFilePath) == NULL)
	//	return FALSE;

	//// �w�b�_�`�F�b�N
	//if (memcmp(DBFile.GetFilePointer(), "EDDB", 4) != 0)
	//	return FALSE;

	//// �A�[�J�C�u���擾
	//DBFile.Seek(4, FILE_CURRENT);
	//LPDWORD pCtArc = (LPDWORD)DBFile.GetFilePointer();

	//// �C���f�b�N�X�T�C�Y�擾
	//DBFile.Seek(4, FILE_CURRENT);
	//LPDWORD pIndexSize = (LPDWORD)DBFile.GetFilePointer();

	//// �C���f�b�N�X�擾
	//DBFile.Seek(4, FILE_CURRENT);
	//LPBYTE pIndex = (LPBYTE)DBFile.GetFilePointer();

	//// �O�Ɉ�x�ǂݍ��񂾂��Ƃ��Ȃ����`�F�b�N
	//BOOL bOpen = FALSE;
	//DWORD IndexOffset = 0;
	//for (DWORD i = 0; i < *pCtArc; i++) {
	//	if (pArc->GetArcPath() == pIndex) {
	//		bOpen = TRUE;
	//		break;
	//	}
	//	DWORD offset = strlen(pIndex) + 17;
	//	pIndex += offset;
	//	IndexOffset += offset;
	//}

	//DBFile.Seek(IndexOffset, FILE_CURRENT);

	//if (bOpen == TRUE) {
	//	
	//}
	//else {
	//	// �w�b�_�X�V
	//	(*pCtArc)++;
	//	*pIndexSize += pArc->GetArcPath().length() + 13;

	//	// �C���f�b�N�X�ǉ�
	//	DBFile.Append(pArc->GetArcPath().c_str(), pArc->GetArcPath().length() + 1);
	//	LPDWORD pDataSize = (LPDWORD)DBFile.Append(&ctFile, 4);
	//	LPDWORD pOffset = DBFile.Append(NULL, 4);
	//	DBFile.Append(NULL, 4);

	//	// �f�[�^�ǉ�
	//	LPDWORD offset = DBFile.SeekEnd();
	//	for (size_t i = nStartEnt; i < nEndEnt; i++) {
	//		DBFile.Append(pEnt[i].name.c_str(), pEnt[i].name.length() + 1);
	//		DBFile.Append(pEnt[i].sizeOrg, 8);
	//		DBFile.Append(pEnt[i].start, 8);
	//	}

	//	*pDataSize = DBFile.GetFilePointer() - offset;
	//	*pOffset = *offset - *pIndexSize;
	//}

	return TRUE;
}