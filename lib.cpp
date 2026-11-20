// DA-030: cac ham tao control va doc/ghi du lieu

#include "common.h"

#include <stdlib.h>

HINSTANCE hInst = NULL;
HFONT gFont = NULL;

// Khoi tao cac thu vien Common Controls
void KhoiTaoThuVien() {
    INITCOMMONCONTROLSEX icc;
    icc.dwSize = sizeof(icc);
    icc.dwICC = ICC_WIN95_CLASSES | ICC_TAB_CLASSES | ICC_LISTVIEW_CLASSES |
                ICC_DATE_CLASSES | ICC_UPDOWN_CLASS | ICC_STANDARD_CLASSES;
    InitCommonControlsEx(&icc);
}

// Tao font chu cho toan bo giao dien
HFONT TaoFont() {
    return CreateFontW(16, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
                       DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
                       DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE,
                       L"Segoe UI");
}

// Gan font cho mot control
void DatFont(HWND h) {
    SendMessageW(h, WM_SETFONT, (WPARAM)gFont, TRUE);
}

// ===== Tao control =====

// Tao mot nhan (chu hien thi)
HWND TaoNhan(HWND cha, const wchar_t* chu, int x, int y, int rong) {
    HWND h = CreateWindowExW(0, L"STATIC", chu, WS_CHILD | WS_VISIBLE,
                             x, y, rong, 28, cha, NULL, hInst, NULL);
    DatFont(h);
    return h;
}

// Tao mot o nhap
HWND TaoO(HWND cha, int id, int x, int y, int rong) {
    // Ep id qua INT_PTR truoc khi ep sang HMENU, neu khong se loi tren ban x64
    HWND h = CreateWindowExW(WS_EX_CLIENTEDGE, L"EDIT", L"",
                             WS_CHILD | WS_VISIBLE | WS_TABSTOP | ES_AUTOHSCROLL,
                             x, y, rong, 26, cha, (HMENU)(INT_PTR)id, hInst, NULL);
    DatFont(h);
    return h;
}

// Tao mot nut bam
HWND TaoNut(HWND cha, int id, const wchar_t* chu, int x, int y, int rong) {
    HWND h = CreateWindowExW(0, L"BUTTON", chu,
                             WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_PUSHBUTTON,
                             x, y, rong, 32, cha, (HMENU)(INT_PTR)id, hInst, NULL);
    DatFont(h);
    return h;
}

// Them mot cot vao bang
void ThemCot(HWND bang, int chiSo, const wchar_t* ten) {
    LVCOLUMNW c = {0};
    c.mask = LVCF_TEXT | LVCF_WIDTH | LVCF_SUBITEM;
    c.pszText = (wchar_t*)ten;
    c.cx = 200;
    c.iSubItem = chiSo;
    ListView_InsertColumn(bang, chiSo, &c);
}

// Tao mot bang (ListView). Cot nao khong dung thi truyen NULL.
HWND TaoBang(HWND cha, int id, const wchar_t* cot0, const wchar_t* cot1,
             const wchar_t* cot2, const wchar_t* cot3, int y) {
    HWND bang = CreateWindowExW(WS_EX_CLIENTEDGE, WC_LISTVIEWW, L"",
                                WS_CHILD | WS_VISIBLE | WS_TABSTOP |
                                LVS_REPORT | LVS_SINGLESEL,
                                18, y, 900, 380, cha, (HMENU)(INT_PTR)id,
                                hInst, NULL);
    ListView_SetExtendedListViewStyle(bang, LVS_EX_FULLROWSELECT | LVS_EX_DOUBLEBUFFER);

    ThemCot(bang, 0, cot0);
    if (cot1) ThemCot(bang, 1, cot1);
    if (cot2) ThemCot(bang, 2, cot2);
    if (cot3) ThemCot(bang, 3, cot3);

    DatFont(bang);
    return bang;
}

// ===== Doc va ghi du lieu =====

// Doc chu trong mot o nhap ra ketQua
void DocO(HWND o, wchar_t* ketQua) {
    ketQua[0] = L'\0';
    if (o) GetWindowTextW(o, ketQua, MAX_CHU);
}

// Ghi chu vao mot o nhap
void DatChuoi(HWND o, const wchar_t* chu) {
    if (o) SetWindowTextW(o, chu);
}

// Doc mot so nguyen trong o nhap, khong doc duoc thi tra 0
int LaySo(HWND o) {
    wchar_t chu[MAX_CHU];
    DocO(o, chu);
    return _wtoi(chu);
}

// Dua con tro nhap ve o
void ChonO(HWND o) {
    if (o) SetFocus(o);
}

// Them mot dong vao bang. Cot nao khong dung thi truyen NULL.
void ThemDong(HWND bang, const wchar_t* cot0, const wchar_t* cot1,
              const wchar_t* cot2, const wchar_t* cot3) {
    int dong = ListView_GetItemCount(bang);

    LVITEMW muc = {0};
    muc.mask = LVIF_TEXT;
    muc.iItem = dong;
    muc.iSubItem = 0;
    muc.pszText = (wchar_t*)cot0;
    ListView_InsertItem(bang, &muc);

    if (cot1) ListView_SetItemText(bang, dong, 1, (wchar_t*)cot1);
    if (cot2) ListView_SetItemText(bang, dong, 2, (wchar_t*)cot2);
    if (cot3) ListView_SetItemText(bang, dong, 3, (wchar_t*)cot3);
}

// Xoa het cac dong trong bang
void XoaBang(HWND bang) {
    ListView_DeleteAllItems(bang);
}

// Lay ngay dang chon trong DateTimePicker
SYSTEMTIME LayNgay(HWND chonNgay) {
    SYSTEMTIME ngay = {0};
    DateTime_GetSystemtime(chonNgay, &ngay);
    return ngay;
}

// Kiem tra CheckBox co duoc danh dau khong
bool DangChon(HWND hopKiem) {
    return SendMessageW(hopKiem, BM_GETCHECK, 0, 0) == BST_CHECKED;
}

// Hien hop thoai thong bao
void ThongBao(const wchar_t* chu) {
    MessageBoxW(cuaSo, chu, L"DA-030", MB_OK | MB_ICONINFORMATION);
}

// Hien hop thoai canh bao
void CanhBao(const wchar_t* chu) {
    MessageBoxW(cuaSo, chu, L"DA-030", MB_OK | MB_ICONWARNING);
}

// Dat cua so ra giua man hinh
void CanGiuaCuaSo(HWND h) {
    RECT rc;
    GetWindowRect(h, &rc);
    int rong = rc.right - rc.left;
    int cao = rc.bottom - rc.top;
    int manRong = GetSystemMetrics(SM_CXSCREEN);
    int manCao = GetSystemMetrics(SM_CYSCREEN);
    SetWindowPos(h, NULL, (manRong - rong) / 2, (manCao - cao) / 2, 0, 0,
                 SWP_NOSIZE | SWP_NOZORDER);
}
