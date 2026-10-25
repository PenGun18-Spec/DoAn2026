// Native C++17 + Windows API. No C++/CLI or .NET required.
#ifndef UNICODE
#define UNICODE
#endif
#ifndef _UNICODE
#define _UNICODE
#endif
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#include <commctrl.h>
#include <array>
#include <string>
#include <vector>

namespace {
constexpr int ID_TABS = 100;
constexpr int ID_ADD = 101, ID_SAVE = 102, ID_RANK = 103;
HINSTANCE instance;
HWND tabs;
HFONT font;
std::array<HWND, 3> pages{};
HWND maSinhVien, hoTen, maLop, maDiemDanh, ngayHoc, coMat, topK;
HWND bangSinhVien, bangDiemDanh, bangXepHang;

// Mot sinh vien gom ma, ho ten, lop.
struct SinhVien {
    std::wstring maSV;
    std::wstring hoTen;
    std::wstring maLop;
};
std::vector<SinhVien> danhSach;

HWND control(HWND parent, const wchar_t* cls, const wchar_t* text,
             DWORD style, int x, int y, int width, int height, int id = 0) {
    HWND result = CreateWindowExW(0, cls, text, WS_CHILD | WS_VISIBLE | style,
        x, y, width, height, parent, reinterpret_cast<HMENU>(static_cast<INT_PTR>(id)), instance, nullptr);
    SendMessageW(result, WM_SETFONT, reinterpret_cast<WPARAM>(font), TRUE);
    return result;
}
void label(HWND page, const wchar_t* text, int x, int width) {
    control(page, L"STATIC", text, 0, x, 24, width, 24);
}
HWND edit(HWND page, int x, int width) {
    return control(page, L"EDIT", L"", WS_BORDER | WS_TABSTOP | ES_AUTOHSCROLL, x, 20, width, 26);
}
HWND table(HWND page, const wchar_t* const* columns, int count) {
    HWND result = control(page, WC_LISTVIEWW, L"", WS_BORDER | LVS_REPORT | LVS_SINGLESEL,
        18, 70, 925, 390);
    ListView_SetExtendedListViewStyle(result, LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
    for (int i = 0; i < count; ++i) {
        LVCOLUMNW column{};
        column.mask = LVCF_TEXT | LVCF_WIDTH;
        column.pszText = const_cast<wchar_t*>(columns[i]);
        column.cx = 900 / count;
        SendMessageW(result, LVM_INSERTCOLUMNW, i, reinterpret_cast<LPARAM>(&column));
    }
    return result;
}
// Doc noi dung mot o nhap (EDIT).
std::wstring docEdit(HWND editBox) {
    int len = GetWindowTextLengthW(editBox);
    std::wstring text(len, L'\0');
    if (len > 0) GetWindowTextW(editBox, &text[0], len + 1);
    return text;
}
// Them mot dong vao bang sinh vien (ListView).
void themDongBangSinhVien(const SinhVien& sv) {
    int row = ListView_GetItemCount(bangSinhVien);
    LVITEMW item{};
    item.mask = LVIF_TEXT;
    item.iItem = row;
    item.pszText = const_cast<wchar_t*>(sv.maSV.c_str());
    SendMessageW(bangSinhVien, LVM_INSERTITEMW, 0, reinterpret_cast<LPARAM>(&item));
    ListView_SetItemText(bangSinhVien, row, 1, const_cast<wchar_t*>(sv.hoTen.c_str()));
    ListView_SetItemText(bangSinhVien, row, 2, const_cast<wchar_t*>(sv.maLop.c_str()));
}
void themSinhVien(HWND owner) {
    SinhVien sv;
    sv.maSV = docEdit(maSinhVien);
    sv.hoTen = docEdit(hoTen);
    sv.maLop = docEdit(maLop);
    if (sv.maSV.empty() || sv.hoTen.empty() || sv.maLop.empty()) {
        MessageBoxW(owner, L"Vui long nhap day du Ma SV, Ho ten, Ma lop.", L"DA-030", MB_OK | MB_ICONWARNING);
        return;
    }
    // Kiem tra trung ma sinh vien.
    for (const SinhVien& old : danhSach) {
        if (old.maSV == sv.maSV) {
            MessageBoxW(owner, L"Ma sinh vien nay da ton tai.", L"DA-030", MB_OK | MB_ICONWARNING);
            return;
        }
    }
    danhSach.push_back(sv);
    themDongBangSinhVien(sv);
    SetWindowTextW(maSinhVien, L"");
    SetWindowTextW(hoTen, L"");
    SetWindowTextW(maLop, L"");
    SetFocus(maSinhVien);
    MessageBoxW(owner, L"Da them sinh vien.", L"DA-030", MB_OK | MB_ICONINFORMATION);
}
void luuDiemDanh(HWND owner) {
    // TODO: Find student, store date/presence, update bangDiemDanh.
    MessageBoxW(owner, L"Chua cai dat chuc nang diem danh.", L"DA-030", MB_OK);
}
void xemXepHang(HWND owner) {
    // TODO: Calculate scores and use an Order-Statistic Tree for Top K.
    MessageBoxW(owner, L"Chua cai dat chuc nang xep hang.", L"DA-030", MB_OK);
}
LRESULT CALLBACK pageProc(HWND window, UINT message, WPARAM wParam, LPARAM lParam) {
    if (message == WM_COMMAND) {
        return SendMessageW(GetParent(window), message, wParam, lParam);
    }
    return DefWindowProcW(window, message, wParam, lParam);
}
void layout(HWND window) {
    RECT area{};
    GetClientRect(window, &area);
    MoveWindow(tabs, 0, 0, area.right, area.bottom, TRUE);
    TabCtrl_AdjustRect(tabs, FALSE, &area);
    for (HWND page : pages) {
        MoveWindow(page, area.left, area.top, area.right - area.left, area.bottom - area.top, TRUE);
    }
    const HWND tables[] = {bangSinhVien, bangDiemDanh, bangXepHang};
    for (int i = 0; i < 3; ++i) {
        RECT client{};
        GetClientRect(pages[i], &client);
        int width = client.right > 36 ? client.right - 36 : 1;
        int height = client.bottom > 88 ? client.bottom - 88 : 1;
        MoveWindow(tables[i], 18, 70, width, height, TRUE);
        int count = Header_GetItemCount(ListView_GetHeader(tables[i]));
        for (int col = 0; col < count; ++col) ListView_SetColumnWidth(tables[i], col, (width - 4) / count);
    }
}
void createUi(HWND window) {
    font = static_cast<HFONT>(GetStockObject(DEFAULT_GUI_FONT));
    tabs = control(window, WC_TABCONTROLW, L"", WS_TABSTOP | WS_CLIPSIBLINGS, 0, 0, 100, 100, ID_TABS);
    const wchar_t* names[] = {L"Sinh vien", L"Diem danh", L"Xep hang"};
    for (int i = 0; i < 3; ++i) {
        TCITEMW item{};
        item.mask = TCIF_TEXT;
        item.pszText = const_cast<wchar_t*>(names[i]);
        SendMessageW(tabs, TCM_INSERTITEMW, i, reinterpret_cast<LPARAM>(&item));
        pages[i] = control(window, L"DA030Page", L"", WS_CLIPCHILDREN, 0, 0, 100, 100);
        ShowWindow(pages[i], i == 0 ? SW_SHOW : SW_HIDE);
    }
    HWND p = pages[0];
    label(p, L"Ma SV", 18, 60); maSinhVien = edit(p, 85, 130);
    label(p, L"Ho ten", 232, 68); hoTen = edit(p, 304, 210);
    label(p, L"Ma lop", 530, 65); maLop = edit(p, 598, 125);
    control(p, L"BUTTON", L"Them SV", WS_TABSTOP, 742, 18, 120, 32, ID_ADD);
    const wchar_t* studentColumns[] = {L"Ma SV", L"Ho ten", L"Ma lop"};
    bangSinhVien = table(p, studentColumns, 3);
    p = pages[1];
    label(p, L"Ma SV", 18, 60); maDiemDanh = edit(p, 85, 130);
    label(p, L"Ngay hoc", 232, 85);
    ngayHoc = control(p, DATETIMEPICK_CLASSW, L"", WS_TABSTOP | DTS_SHORTDATEFORMAT, 320, 20, 150, 26);
    coMat = control(p, L"BUTTON", L"Co mat", WS_TABSTOP | BS_AUTOCHECKBOX, 490, 20, 110, 26);
    SendMessageW(coMat, BM_SETCHECK, BST_CHECKED, 0);
    control(p, L"BUTTON", L"Luu diem danh", WS_TABSTOP, 630, 18, 150, 32, ID_SAVE);
    const wchar_t* attendanceColumns[] = {L"Ma SV", L"Ngay hoc", L"Trang thai"};
    bangDiemDanh = table(p, attendanceColumns, 3);
    p = pages[2];
    label(p, L"So sinh vien Top K", 18, 160);
    topK = control(p, L"EDIT", L"1", WS_BORDER | WS_TABSTOP | ES_NUMBER, 184, 20, 110, 26);
    HWND spin = control(p, UPDOWN_CLASSW, L"", UDS_ALIGNRIGHT | UDS_SETBUDDYINT | UDS_ARROWKEYS,
        0, 0, 0, 0);
    SendMessageW(spin, UDM_SETBUDDY, reinterpret_cast<WPARAM>(topK), 0);
    SendMessageW(spin, UDM_SETRANGE32, 1, 1000);
    SendMessageW(spin, UDM_SETPOS32, 0, 1);
    control(p, L"BUTTON", L"Xem Top K", WS_TABSTOP, 320, 18, 130, 32, ID_RANK);
    const wchar_t* rankingColumns[] = {L"Hang", L"Ma SV", L"Ho ten", L"Diem"};
    bangXepHang = table(p, rankingColumns, 4);
    layout(window);
}
LRESULT CALLBACK windowProc(HWND window, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
    case WM_CREATE: createUi(window); return 0;
    case WM_SIZE: if (tabs) layout(window); return 0;
    case WM_GETMINMAXINFO: {
        auto info = reinterpret_cast<MINMAXINFO*>(lParam);
        info->ptMinTrackSize = POINT{1000, 560}; return 0;
    }
    case WM_NOTIFY: {
        auto header = reinterpret_cast<NMHDR*>(lParam);
        if (header->hwndFrom == tabs && header->code == TCN_SELCHANGE) {
            int selected = TabCtrl_GetCurSel(tabs);
            for (int i = 0; i < 3; ++i) ShowWindow(pages[i], i == selected ? SW_SHOW : SW_HIDE);
            SetFocus(tabs);
        }
        return 0;
    }
    case WM_COMMAND:
        if (HIWORD(wParam) == BN_CLICKED) {
            switch (LOWORD(wParam)) {
            case ID_ADD: themSinhVien(window); break;
            case ID_SAVE: luuDiemDanh(window); break;
            case ID_RANK: xemXepHang(window); break;
            }
        }
        return 0;
    case WM_DESTROY: PostQuitMessage(0); return 0;
    }
    return DefWindowProcW(window, message, wParam, lParam);
}
}
int WINAPI WinMain(HINSTANCE appInstance, HINSTANCE, LPSTR, int show) {
    instance = appInstance;
    INITCOMMONCONTROLSEX common{sizeof(INITCOMMONCONTROLSEX), ICC_TAB_CLASSES | ICC_LISTVIEW_CLASSES | ICC_DATE_CLASSES | ICC_UPDOWN_CLASS};
    if (!InitCommonControlsEx(&common)) return 1;
    WNDCLASSW pageClass{};
    pageClass.lpfnWndProc = pageProc;
    pageClass.hInstance = instance;
    pageClass.lpszClassName = L"DA030Page";
    pageClass.hCursor = LoadCursorW(nullptr, IDC_ARROW);
    pageClass.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_BTNFACE + 1);
    if (!RegisterClassW(&pageClass)) return 1;
    WNDCLASSW windowClass = pageClass;
    windowClass.lpfnWndProc = windowProc;
    windowClass.lpszClassName = L"DA030Window";
    if (!RegisterClassW(&windowClass)) return 1;
    int x = (GetSystemMetrics(SM_CXSCREEN) - 1000) / 2;
    int y = (GetSystemMetrics(SM_CYSCREEN) - 660) / 2;
    HWND window = CreateWindowExW(0, windowClass.lpszClassName,
        L"DA-030 - Diem danh va xep hang hoc tap", WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN,
        x, y, 1000, 660, nullptr, nullptr, instance, nullptr);
    if (!window) return 1;
    ShowWindow(window, show);
    UpdateWindow(window);
    MSG message{};
    int result;
    while ((result = GetMessageW(&message, nullptr, 0, 0)) > 0) {
        if (!IsDialogMessageW(window, &message)) {
            TranslateMessage(&message);
            DispatchMessageW(&message);
        }
    }
    return result == -1 ? 1 : static_cast<int>(message.wParam);
}
