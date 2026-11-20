// DA-030: giao dien.
// Chi con phan dung giao dien: bo tri, xu ly su kien va diem vao chuong trinh.
// Lop goi thu vien o lib.cpp, chuc nang nghiep vu o setting.cpp.

#include "common.h"

GiaoDien g;

// ---------------------------------------------------------------------------
// Bo tri.
// ---------------------------------------------------------------------------

// Gian cac cot de bang phu kin chieu rong (tuong duong AutoSizeColumnsMode = Fill).
void GianCot(HWND bang) {
    HWND tieuDe = ListView_GetHeader(bang);
    int soCot = tieuDe ? Header_GetItemCount(tieuDe) : 0;
    if (soCot <= 0) return;

    RECT rc{};
    GetClientRect(bang, &rc);
    int khaDung = rc.right - GetSystemMetrics(SM_CXVSCROLL) - 2;
    if (khaDung < soCot * 40) khaDung = soCot * 40;
    int moiCot = khaDung / soCot;

    for (int i = 0; i < soCot; ++i) {
        int rong = (i == soCot - 1) ? (khaDung - moiCot * (soCot - 1)) : moiCot;
        ListView_SetColumnWidth(bang, i, rong);
    }
}

void BoTriBang(HWND bang, int y) {
    if (!bang) return;

    HWND cha = GetParent(bang);
    RECT rc{};
    GetClientRect(cha, &rc);

    int rong = rc.right - kBangX * 2;
    int cao = rc.bottom - y - 40;
    if (rong < 200) rong = 200;
    if (cao < kBangChieuCaoToiThieu) cao = kBangChieuCaoToiThieu;

    MoveWindow(bang, kBangX, y, rong, cao, TRUE);
    GianCot(bang);
}

// Vung hien thi cua tab (khong tinh hang tieu de tab).
RECT VungTrang() {
    RECT vungTab{};
    GetClientRect(g.tab, &vungTab);
    TabCtrl_AdjustRect(g.tab, FALSE, &vungTab);
    if (vungTab.right - vungTab.left < 100) vungTab.right = vungTab.left + 100;
    if (vungTab.bottom - vungTab.top < 100) vungTab.bottom = vungTab.top + 100;
    return vungTab;
}

// Hang nhap cua trang Sinh vien: neo nut va nhom "Ma lop" theo mep phai de
// khong tran ra ngoai khi cua so hep; o "Ho ten" gian theo phan con lai.
void BoTriHangSinhVien() {
    if (!g.trang[0]) return;

    RECT rc{};
    GetClientRect(g.trang[0], &rc);
    const int w = rc.right;
    const int le = 18;
    const int nutRong = 120;
    const int khoang = 16;
    const int oMaLopRong = 125;
    const int nhanMaLopRong = 65;

    // Giu vi tri goc khi du rong, keo ve ben trai khi cua so hep.
    int nutX = 742;
    if (nutX + nutRong > w - le) nutX = w - le - nutRong;

    int oMaLopX = nutX - khoang - oMaLopRong;
    int nhanMaLopX = oMaLopX - 8 - nhanMaLopRong;

    int oHoTenX = 304;
    int oHoTenRong = nhanMaLopX - 8 - oHoTenX;
    if (oHoTenRong < 100) oHoTenRong = 100;

    if (g.nutThemSv) MoveWindow(g.nutThemSv, nutX, 18, nutRong, 32, TRUE);
    if (g.maLop) MoveWindow(g.maLop, oMaLopX, 20, oMaLopRong, 26, TRUE);
    if (g.nhanMaLop) MoveWindow(g.nhanMaLop, nhanMaLopX, 22, nhanMaLopRong, 28, TRUE);
    if (g.hoTen) MoveWindow(g.hoTen, oHoTenX, 20, oHoTenRong, 26, TRUE);
}

// Hang nhap cua trang Diem danh: giu nguyen vi tri khi du rong, keo nut ve
// ben trai khi cua so hep de khong bi cat.
void BoTriHangDiemDanh() {
    if (!g.trang[1] || !g.nutLuuDiemDanh) return;

    RECT rc{};
    GetClientRect(g.trang[1], &rc);
    const int le = 18;
    const int nutRong = 150;

    int nutX = 630;
    if (nutX + nutRong > rc.right - le) nutX = rc.right - le - nutRong;
    if (nutX < 490 + 120 + 16) nutX = 490 + 120 + 16;

    MoveWindow(g.nutLuuDiemDanh, nutX, 18, nutRong, 32, TRUE);
}

void BoTriCuaSo(HWND cuaSo) {
    RECT rc{};
    GetClientRect(cuaSo, &rc);

    MoveWindow(g.tab, 0, 0, rc.right, rc.bottom, TRUE);

    RECT vungTab = VungTrang();
    for (HWND trang : g.trang) {
        if (trang) {
            MoveWindow(trang, vungTab.left, vungTab.top,
                       vungTab.right - vungTab.left, vungTab.bottom - vungTab.top, TRUE);
        }
    }

    BoTriHangSinhVien();
    BoTriHangDiemDanh();
    BoTriBang(g.bangSinhVien, kBangY);
    BoTriBang(g.bangDiemDanh, kBangY);
    BoTriBang(g.bangXepHang, kBangY);
}

void HienTrang(int chiSo) {
    if (chiSo < 0 || chiSo >= 3) return;
    g.trangDangChon = chiSo;
    for (int i = 0; i < 3; ++i) {
        if (g.trang[i]) ShowWindow(g.trang[i], i == chiSo ? SW_SHOW : SW_HIDE);
    }
}

// ---------------------------------------------------------------------------
// Tao giao dien.
// ---------------------------------------------------------------------------

void TaoTrangSinhVien(HWND trang) {
    TaoNhan(trang, L"Mã SV", 18, 22, 60);
    g.maSinhVien = TaoO(trang, IDC_MA_SV, 85, 20, 130);

    TaoNhan(trang, L"Họ tên", 232, 22, 68);
    g.hoTen = TaoO(trang, IDC_HO_TEN, 304, 20, 210);

    g.nhanMaLop = TaoNhan(trang, L"Mã lớp", 530, 22, 65);
    g.maLop = TaoO(trang, IDC_MA_LOP, 598, 20, 125);

    g.nutThemSv = TaoNut(trang, IDC_BTN_THEM_SV, L"Thêm SV", 742, 18, 120);

    g.bangSinhVien = TaoBang(trang, IDC_BANG_SINH_VIEN, {L"Mã SV", L"Họ tên", L"Mã lớp"}, 70);
}

void TaoTrangDiemDanh(HWND trang) {
    TaoNhan(trang, L"Mã SV", 18, 22, 60);
    g.maDiemDanh = TaoO(trang, IDC_MA_DIEM_DANH, 85, 20, 130);

    TaoNhan(trang, L"Ngày học", 232, 22, 85);
    g.ngayHoc = CreateWindowExW(0, DATETIMEPICK_CLASSW, L"",
                                WS_CHILD | WS_VISIBLE | WS_TABSTOP | DTS_SHORTDATEFORMAT,
                                320, 20, 150, 26, trang, (HMENU)(INT_PTR)IDC_NGAY_HOC, hInst, nullptr);
    DatFont(g.ngayHoc);

    g.coMat = CreateWindowExW(0, L"BUTTON", L"Có mặt",
                              WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_AUTOCHECKBOX,
                              490, 22, 120, 26, trang, (HMENU)(INT_PTR)IDC_CO_MAT, hInst, nullptr);
    DatFont(g.coMat);
    SendMessageW(g.coMat, BM_SETCHECK, BST_CHECKED, 0);

    g.nutLuuDiemDanh = TaoNut(trang, IDC_BTN_LUU_DIEM_DANH, L"Lưu điểm danh", 630, 18, 150);

    g.bangDiemDanh = TaoBang(trang, IDC_BANG_DIEM_DANH, {L"Mã SV", L"Ngày học", L"Trạng thái"}, 70);
}

void TaoTrangXepHang(HWND trang) {
    TaoNhan(trang, L"Số sinh viên Top K", 18, 22, 160);

    g.topKEdit = CreateWindowExW(WS_EX_CLIENTEDGE, L"EDIT", L"1",
                                 WS_CHILD | WS_VISIBLE | WS_TABSTOP | ES_NUMBER | ES_RIGHT,
                                 184, 20, 100, 26, trang, (HMENU)(INT_PTR)IDC_TOP_K_EDIT, hInst, nullptr);
    DatFont(g.topKEdit);

    g.topK = CreateWindowExW(0, UPDOWN_CLASSW, L"",
                             WS_CHILD | WS_VISIBLE | UDS_SETBUDDYINT | UDS_ALIGNRIGHT | UDS_ARROWKEYS,
                             268, 20, 16, 26, trang, (HMENU)(INT_PTR)IDC_TOP_K, hInst, nullptr);
    DatFont(g.topK);
    SendMessageW(g.topK, UDM_SETBUDDY, (WPARAM)g.topKEdit, 0);
    SendMessageW(g.topK, UDM_SETRANGE32, 1, 1000);
    SendMessageW(g.topK, UDM_SETPOS32, 0, 1);

    TaoNut(trang, IDC_BTN_XEM_TOP_K, L"Xem Top K", 320, 18, 130);

    g.bangXepHang = TaoBang(trang, IDC_BANG_XEP_HANG, {L"Hạng", L"Mã SV", L"Họ tên", L"Điểm"}, 70);
}

void TaoGiaoDien(HWND cuaSo) {
    g.cuaSo = cuaSo;

    g.tab = CreateWindowExW(0, WC_TABCONTROLW, L"",
                            WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | TCS_FOCUSNEVER,
                            0, 0, 0, 0, cuaSo, (HMENU)(INT_PTR)IDC_TAB, hInst, nullptr);
    DatFont(g.tab);

    const wchar_t* tenTrang[3] = {L"Sinh viên", L"Điểm danh", L"Xếp hạng"};
    for (int i = 0; i < 3; ++i) {
        TCITEMW muc{};
        muc.mask = TCIF_TEXT;
        muc.pszText = const_cast<wchar_t*>(tenTrang[i]);
        TabCtrl_InsertItem(g.tab, i, &muc);
    }

    RECT vungTab{};
    GetClientRect(g.tab, &vungTab);
    TabCtrl_AdjustRect(g.tab, FALSE, &vungTab);
    for (int i = 0; i < 3; ++i) {
        g.trang[i] = CreateWindowExW(WS_EX_CONTROLPARENT, kLopTrang, L"",
                                     WS_CHILD | (i == 0 ? WS_VISIBLE : 0),
                                     vungTab.left, vungTab.top,
                                     vungTab.right - vungTab.left, vungTab.bottom - vungTab.top,
                                     g.tab, nullptr, hInst, nullptr);
        DatFont(g.trang[i]);
    }

    TaoTrangSinhVien(g.trang[0]);
    TaoTrangDiemDanh(g.trang[1]);
    TaoTrangXepHang(g.trang[2]);

    HienTrang(0);
}

// ---------------------------------------------------------------------------
// Xu ly su kien.
// ---------------------------------------------------------------------------

void XuLyLenh(WORD id, WORD thongBao) {
    if (thongBao != BN_CLICKED) return;
    switch (id) {
    case IDC_BTN_THEM_SV:
        ThemSinhVien();
        break;
    case IDC_BTN_LUU_DIEM_DANH:
        LuuDiemDanh();
        break;
    case IDC_BTN_XEM_TOP_K:
        XemXepHang();
        break;
    default:
        break;
    }
}

LRESULT CALLBACK XuLyTrang(HWND hwnd, UINT thongDiep, WPARAM wParam, LPARAM lParam) {
    switch (thongDiep) {
    case WM_ERASEBKGND: {
        RECT rc{};
        GetClientRect(hwnd, &rc);
        FillRect(reinterpret_cast<HDC>(wParam), &rc, GetSysColorBrush(COLOR_BTNFACE));
        return 1;
    }
    case WM_CTLCOLORSTATIC: {
        HDC hdc = reinterpret_cast<HDC>(wParam);
        SetBkMode(hdc, TRANSPARENT);
        return reinterpret_cast<LRESULT>(GetSysColorBrush(COLOR_BTNFACE));
    }
    case WM_COMMAND:
        // Nut nam trong tung trang (con cua TabControl), nen WM_COMMAND duoc
        // gui toi trang chu khong toi cua so chinh: xu ly ngay tai day.
        XuLyLenh(LOWORD(wParam), HIWORD(wParam));
        return 0;
    default:
        break;
    }
    return DefWindowProcW(hwnd, thongDiep, wParam, lParam);
}

LRESULT CALLBACK XuLyCuaSo(HWND hwnd, UINT thongDiep, WPARAM wParam, LPARAM lParam) {
    switch (thongDiep) {
    case WM_CREATE:
        TaoGiaoDien(hwnd);
        return 0;

    case WM_SIZE:
        BoTriCuaSo(hwnd);
        return 0;

    case WM_GETMINMAXINFO: {
        auto* thongTin = reinterpret_cast<MINMAXINFO*>(lParam);
        thongTin->ptMinTrackSize.x = 850;
        thongTin->ptMinTrackSize.y = 560;
        return 0;
    }

    case WM_NOTIFY: {
        auto* thongBao = reinterpret_cast<NMHDR*>(lParam);
        if (thongBao && thongBao->hwndFrom == g.tab && thongBao->code == TCN_SELCHANGE) {
            HienTrang(TabCtrl_GetCurSel(g.tab));
            return 0;
        }
        break;
    }

    case WM_COMMAND:
        XuLyLenh(LOWORD(wParam), HIWORD(wParam));
        return 0;

    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;

    default:
        break;
    }
    return DefWindowProcW(hwnd, thongDiep, wParam, lParam);
}

void DangKyLop() {
    WNDCLASSEXW lop{};
    lop.cbSize = sizeof(lop);
    lop.hInstance = hInst;
    lop.hCursor = LoadCursorW(nullptr, IDC_ARROW);

    lop.lpfnWndProc = XuLyCuaSo;
    lop.lpszClassName = kLopChinh;
    lop.hbrBackground = nullptr;
    lop.style = CS_HREDRAW | CS_VREDRAW;
    RegisterClassExW(&lop);

    lop.lpfnWndProc = XuLyTrang;
    lop.lpszClassName = kLopTrang;
    lop.style = 0;
    RegisterClassExW(&lop);
}

// ---------------------------------------------------------------------------
// Diem vao chuong trinh.
// ---------------------------------------------------------------------------

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR, int nShowCmd) {
    hInst = hInstance;

    KhoiTaoThuVien();

    gFont = TaoFont();
    DangKyLop();

    HWND cuaSo = CreateWindowExW(0, kLopChinh, kTieuDe,
                                 WS_OVERLAPPEDWINDOW,
                                 CW_USEDEFAULT, CW_USEDEFAULT, 1000, 660,
                                 nullptr, nullptr, hInst, nullptr);
    if (!cuaSo) {
        HuyFont();
        return 1;
    }

    CanGiuaCuaSo(cuaSo);
    ShowWindow(cuaSo, nShowCmd);
    UpdateWindow(cuaSo);

    MSG msg{};
    while (GetMessageW(&msg, nullptr, 0, 0) > 0) {
        if (!IsDialogMessageW(cuaSo, &msg)) {
            TranslateMessage(&msg);
            DispatchMessageW(&msg);
        }
    }

    HuyFont();
    return static_cast<int>(msg.wParam);
}
