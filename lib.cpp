// DA-030: lop goi thu vien.
// Tap trung moi loi goi Win32 API tho (CreateWindowExW, SendMessageW,
// ListView_*, DateTime_*, SystemParametersInfoW...) de phan giao dien va
// nghiep vu khong phai lam viec truc tiep voi API.

#include "common.h"

HINSTANCE hInst = nullptr;
HFONT gFont = nullptr;

void KhoiTaoThuVien() {
    INITCOMMONCONTROLSEX icc{};
    icc.dwSize = sizeof(icc);
    icc.dwICC = ICC_WIN95_CLASSES | ICC_TAB_CLASSES | ICC_LISTVIEW_CLASSES |
                ICC_DATE_CLASSES | ICC_UPDOWN_CLASS | ICC_STANDARD_CLASSES;
    InitCommonControlsEx(&icc);
}

// ---------------------------------------------------------------------------
// Font.
// ---------------------------------------------------------------------------

HFONT TaoFont() {
    HDC manHinh = GetDC(nullptr);
    int dpi = manHinh ? GetDeviceCaps(manHinh, LOGPIXELSY) : 96;
    if (manHinh) ReleaseDC(nullptr, manHinh);

    LOGFONTW lf{};
    lf.lfHeight = -MulDiv(10, dpi, 72);
    lf.lfWeight = FW_NORMAL;
    lf.lfCharSet = DEFAULT_CHARSET;
    lstrcpyW(lf.lfFaceName, L"Segoe UI");
    return CreateFontIndirectW(&lf);
}

void HuyFont() {
    if (gFont) {
        DeleteObject(gFont);
        gFont = nullptr;
    }
}

void DatFont(HWND dieuKhien) {
    if (dieuKhien && gFont) SendMessageW(dieuKhien, WM_SETFONT, (WPARAM)gFont, TRUE);
}

// ---------------------------------------------------------------------------
// Tao control.
// ---------------------------------------------------------------------------

HWND TaoNhan(HWND cha, const wchar_t* chu, int x, int y, int rong) {
    HWND nhan = CreateWindowExW(0, L"STATIC", chu, WS_CHILD | WS_VISIBLE | SS_LEFT,
                                x, y, rong, 28, cha, nullptr, hInst, nullptr);
    DatFont(nhan);
    return nhan;
}

HWND TaoO(HWND cha, int id, int x, int y, int rong) {
    HWND o = CreateWindowExW(WS_EX_CLIENTEDGE, L"EDIT", L"",
                             WS_CHILD | WS_VISIBLE | WS_TABSTOP | ES_AUTOHSCROLL,
                             x, y, rong, 26, cha, (HMENU)(INT_PTR)id, hInst, nullptr);
    DatFont(o);
    return o;
}

HWND TaoNut(HWND cha, int id, const wchar_t* chu, int x, int y, int rong) {
    HWND nut = CreateWindowExW(0, L"BUTTON", chu, WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_PUSHBUTTON,
                               x, y, rong, 32, cha, (HMENU)(INT_PTR)id, hInst, nullptr);
    DatFont(nut);
    return nut;
}

HWND TaoBang(HWND cha, int id, std::initializer_list<const wchar_t*> cot, int y) {
    HWND bang = CreateWindowExW(WS_EX_CLIENTEDGE, WC_LISTVIEWW, L"",
                                WS_CHILD | WS_VISIBLE | WS_TABSTOP | LVS_REPORT | LVS_SINGLESEL | LVS_SHOWSELALWAYS,
                                kBangX, y, 925, 390, cha, (HMENU)(INT_PTR)id, hInst, nullptr);
    ListView_SetExtendedListViewStyle(bang, LVS_EX_FULLROWSELECT | LVS_EX_DOUBLEBUFFER);

    int chiSo = 0;
    for (const wchar_t* tenCot : cot) {
        LVCOLUMNW c{};
        c.mask = LVCF_TEXT | LVCF_WIDTH | LVCF_SUBITEM;
        c.pszText = const_cast<wchar_t*>(tenCot);
        c.cx = 200;
        c.iSubItem = chiSo++;
        ListView_InsertColumn(bang, c.iSubItem, &c);
    }

    DatFont(bang);
    return bang;
}

// ---------------------------------------------------------------------------
// Doc/ghi du lieu tren control.
// ---------------------------------------------------------------------------

std::wstring LayChuoi(HWND o) {
    if (!o) return std::wstring();

    int doDai = GetWindowTextLengthW(o);
    if (doDai <= 0) return std::wstring();

    std::wstring chu(static_cast<size_t>(doDai) + 1, L'\0');
    int docDuoc = GetWindowTextW(o, chu.data(), doDai + 1);
    chu.resize(docDuoc > 0 ? static_cast<size_t>(docDuoc) : 0);
    return chu;
}

// Ghi chuoi vao o nhap (EDIT).
void DatChuoi(HWND o, const wchar_t* chu) {
    if (o) SetWindowTextW(o, chu ? chu : L"");
}

// Chuyen focus ve o nhap de nhap tiep.
void ChonO(HWND o) {
    if (o) SetFocus(o);
}

// Them mot dong vao ListView bao cao: phan tu dau la cot 0, con lai la subitem.
void ThemDong(HWND bang, std::initializer_list<const wchar_t*> giaTri) {
    if (!bang || giaTri.size() == 0) return;

    int dong = ListView_GetItemCount(bang);
    auto it = giaTri.begin();

    LVITEMW muc{};
    muc.mask = LVIF_TEXT;
    muc.iItem = dong;
    muc.iSubItem = 0;
    muc.pszText = const_cast<wchar_t*>(*it++);
    ListView_InsertItem(bang, &muc);

    int cot = 1;
    for (; it != giaTri.end(); ++it, ++cot) {
        ListView_SetItemText(bang, dong, cot, const_cast<wchar_t*>(*it));
    }
}

SYSTEMTIME LayNgay(HWND chonNgay) {
    SYSTEMTIME ngay{};
    if (chonNgay) DateTime_GetSystemtime(chonNgay, &ngay);
    return ngay;
}

bool DangChon(HWND hopKiem) {
    return hopKiem && SendMessageW(hopKiem, BM_GETCHECK, 0, 0) == BST_CHECKED;
}

void HienThongBao(HWND cha, const wchar_t* chu) {
    MessageBoxW(cha, chu, kTieuDe, MB_OK | MB_ICONINFORMATION);
}

void HienCanhBao(HWND cha, const wchar_t* chu) {
    MessageBoxW(cha, chu, kTieuDe, MB_OK | MB_ICONWARNING);
}

// ---------------------------------------------------------------------------
// Cua so.
// ---------------------------------------------------------------------------

void CanGiuaCuaSo(HWND cuaSo) {
    RECT rc{};
    GetWindowRect(cuaSo, &rc);

    RECT vung{};
    SystemParametersInfoW(SPI_GETWORKAREA, 0, &vung, 0);

    int rong = rc.right - rc.left;
    int cao = rc.bottom - rc.top;
    int x = vung.left + ((vung.right - vung.left) - rong) / 2;
    int y = vung.top + ((vung.bottom - vung.top) - cao) / 2;
    SetWindowPos(cuaSo, nullptr, x, y, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
}
