// DA-030: giao dien

#include "common.h"

// ===== Cac cua so con =====
HWND cuaSo = NULL;
HWND tab = NULL;
HWND trang[3] = {NULL, NULL, NULL};

HWND maSinhVien = NULL, hoTen = NULL, maLop = NULL, bangSinhVien = NULL;
HWND maDiemDanh = NULL, ngayHoc = NULL, coMat = NULL, bangDiemDanh = NULL;
HWND topKEdit = NULL, bangXepHang = NULL;

// ===== Tao giao dien tung trang =====

// Trang Sinh vien: ba o nhap, nut Them SV, bang
void TaoTrangSinhVien(HWND p) {
    TaoNhan(p, L"Mã SV", 18, 22, 60);
    maSinhVien = TaoO(p, IDC_MA_SV, 85, 20, 130);

    TaoNhan(p, L"Họ tên", 232, 22, 68);
    hoTen = TaoO(p, IDC_HO_TEN, 304, 20, 210);

    TaoNhan(p, L"Mã lớp", 530, 22, 65);
    maLop = TaoO(p, IDC_MA_LOP, 598, 20, 125);

    TaoNut(p, IDC_BTN_THEM_SV, L"Thêm SV", 640, 18, 110);
    TaoNut(p, IDC_BTN_XOA_SV, L"Xóa SV", 760, 18, 100);

    bangSinhVien = TaoBang(p, IDC_BANG_SINH_VIEN,
                           L"Mã SV", L"Họ tên", L"Mã lớp", NULL, 70);
}

// Trang Diem danh: o ma SV, chon ngay, check Co mat, nut Luu, bang
void TaoTrangDiemDanh(HWND p) {
    TaoNhan(p, L"Mã SV", 18, 22, 60);
    maDiemDanh = TaoO(p, IDC_MA_DIEM_DANH, 85, 20, 130);

    TaoNhan(p, L"Ngày học", 232, 22, 85);
    ngayHoc = CreateWindowExW(0, DATETIMEPICK_CLASSW, L"",
                              WS_CHILD | WS_VISIBLE | WS_TABSTOP | DTS_SHORTDATEFORMAT,
                              320, 20, 150, 26, p,
                              (HMENU)(INT_PTR)IDC_NGAY_HOC, hInst, NULL);
    DatFont(ngayHoc);

    coMat = CreateWindowExW(0, L"BUTTON", L"Có mặt",
                            WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_AUTOCHECKBOX,
                            490, 22, 120, 26, p,
                            (HMENU)(INT_PTR)IDC_CO_MAT, hInst, NULL);
    DatFont(coMat);
    SendMessageW(coMat, BM_SETCHECK, BST_CHECKED, 0);

    TaoNut(p, IDC_BTN_LUU_DIEM_DANH, L"Lưu điểm danh", 630, 18, 150);

    bangDiemDanh = TaoBang(p, IDC_BANG_DIEM_DANH,
                           L"Mã SV", L"Ngày học", L"Trạng thái", NULL, 70);
}

// Trang Xep hang: o nhap K, nut tang giam, nut Xem, bang
void TaoTrangXepHang(HWND p) {
    TaoNhan(p, L"Số sinh viên Top K", 18, 22, 160);

    topKEdit = CreateWindowExW(WS_EX_CLIENTEDGE, L"EDIT", L"1",
                               WS_CHILD | WS_VISIBLE | WS_TABSTOP | ES_NUMBER | ES_RIGHT,
                               184, 20, 100, 26, p,
                               (HMENU)(INT_PTR)IDC_TOP_K_EDIT, hInst, NULL);
    DatFont(topKEdit);

    // Nut tang giam so, gan vao o topKEdit
    HWND nutTangGiam = CreateWindowExW(0, UPDOWN_CLASSW, L"",
                                       WS_CHILD | WS_VISIBLE | UDS_SETBUDDYINT |
                                       UDS_ALIGNRIGHT | UDS_ARROWKEYS,
                                       268, 20, 16, 26, p, NULL, hInst, NULL);
    SendMessageW(nutTangGiam, UDM_SETBUDDY, (WPARAM)topKEdit, 0);
    SendMessageW(nutTangGiam, UDM_SETRANGE32, 1, 1000);

    TaoNut(p, IDC_BTN_XEM_TOP_K, L"Xem Top K", 320, 18, 130);

    bangXepHang = TaoBang(p, IDC_BANG_XEP_HANG,
                          L"Hạng", L"Mã SV", L"Họ tên", L"Điểm", 70);
}

// ===== Tao toan bo giao dien =====

void TaoGiaoDien(HWND h) {
    cuaSo = h;

    tab = CreateWindowExW(0, WC_TABCONTROLW, L"",
                          WS_CHILD | WS_VISIBLE | TCS_FOCUSNEVER,
                          0, 0, 0, 0, h, (HMENU)(INT_PTR)IDC_TAB, hInst, NULL);
    DatFont(tab);

    // Them ba tab
    TCITEMW muc = {0};
    muc.mask = TCIF_TEXT;

    muc.pszText = (wchar_t*)L"Sinh viên";
    TabCtrl_InsertItem(tab, 0, &muc);

    muc.pszText = (wchar_t*)L"Điểm danh";
    TabCtrl_InsertItem(tab, 1, &muc);

    muc.pszText = (wchar_t*)L"Xếp hạng";
    TabCtrl_InsertItem(tab, 2, &muc);

    // Tao ba trang con cua tab
    // WS_EX_CONTROLPARENT de phim Tab chuyen duoc focus giua cac o nhap ben trong
    for (int i = 0; i < 3; i++) {
        trang[i] = CreateWindowExW(WS_EX_CONTROLPARENT, L"DA030Trang", L"",
                                   WS_CHILD | (i == 0 ? WS_VISIBLE : 0),
                                   0, 0, 0, 0, tab, NULL, hInst, NULL);
        DatFont(trang[i]);
    }

    // Dat cac control len tung trang
    TaoTrangSinhVien(trang[0]);
    TaoTrangDiemDanh(trang[1]);
    TaoTrangXepHang(trang[2]);
}

// ===== Bo tri khi cua so doi kich thuoc =====

void BoTriCuaSo(HWND h) {
    RECT rc;
    GetClientRect(h, &rc);
    MoveWindow(tab, 0, 0, rc.right, rc.bottom, TRUE);

    // Vung ben trong tab (khong tinh hang tieu de tab)
    RECT vung = rc;
    TabCtrl_AdjustRect(tab, FALSE, &vung);
    int rong = vung.right - vung.left;
    int cao = vung.bottom - vung.top;

    // Ba trang con lap day vung trong
    for (int i = 0; i < 3; i++) {
        MoveWindow(trang[i], vung.left, vung.top, rong, cao, TRUE);
    }

    // Ba bang gian theo kich thuoc cua so
    HWND bang[3] = {bangSinhVien, bangDiemDanh, bangXepHang};
    for (int i = 0; i < 3; i++) {
        MoveWindow(bang[i], 18, 70, rong - 36, cao - 100, TRUE);
    }
}

// ===== An hien trang theo tab duoc chon =====

void HienTrang(int chiSo) {
    for (int i = 0; i < 3; i++) {
        if (i == chiSo) ShowWindow(trang[i], SW_SHOW);
        else ShowWindow(trang[i], SW_HIDE);
    }
}

// ===== Xu ly su kien =====

// Xu ly khi bam nut
// Chi xu ly khi bam chuot (BN_CLICKED); bo qua BN_SETFOCUS,
// neu khong moi lan bam nut se chay ham hai lan.
void XuLyLenh(WORD id, WORD thongBao) {
    if (thongBao != BN_CLICKED) return;

    if (id == IDC_BTN_THEM_SV) ThemSinhVien();
    else if (id == IDC_BTN_XOA_SV) XoaSinhVien();
    else if (id == IDC_BTN_LUU_DIEM_DANH) LuuDiemDanh();
    else if (id == IDC_BTN_XEM_TOP_K) XemXepHang();
}

// Xu ly su kien cua tung trang
LRESULT CALLBACK XuLyTrang(HWND h, UINT thongDiep, WPARAM wParam, LPARAM lParam) {
    // To nen xam cho trang
    if (thongDiep == WM_ERASEBKGND) {
        RECT rc;
        GetClientRect(h, &rc);
        FillRect((HDC)wParam, &rc, GetSysColorBrush(COLOR_BTNFACE));
        return 1;
    }

    // Nen trong suot cho cac nhan
    if (thongDiep == WM_CTLCOLORSTATIC) {
        SetBkMode((HDC)wParam, TRANSPARENT);
        return (LRESULT)GetSysColorBrush(COLOR_BTNFACE);
    }

    // Nut nam trong trang nen thong diep gui toi trang
    if (thongDiep == WM_COMMAND) {
        XuLyLenh(LOWORD(wParam), HIWORD(wParam));
        return 0;
    }

    return DefWindowProcW(h, thongDiep, wParam, lParam);
}

// Xu ly su kien cua cua so chinh
LRESULT CALLBACK XuLyCuaSo(HWND h, UINT thongDiep, WPARAM wParam, LPARAM lParam) {
    if (thongDiep == WM_CREATE) {
        TaoGiaoDien(h);
        return 0;
    }

    if (thongDiep == WM_SIZE) {
        BoTriCuaSo(h);
        return 0;
    }

    // Khong cho thu nho qua 850 x 560
    if (thongDiep == WM_GETMINMAXINFO) {
        MINMAXINFO* info = (MINMAXINFO*)lParam;
        info->ptMinTrackSize.x = 850;
        info->ptMinTrackSize.y = 560;
        return 0;
    }

    // Doi tab
    if (thongDiep == WM_NOTIFY) {
        NMHDR* nm = (NMHDR*)lParam;
        if (nm->hwndFrom == tab && nm->code == TCN_SELCHANGE) {
            HienTrang(TabCtrl_GetCurSel(tab));
            return 0;
        }
    }

    if (thongDiep == WM_COMMAND) {
        XuLyLenh(LOWORD(wParam), HIWORD(wParam));
        return 0;
    }

    if (thongDiep == WM_DESTROY) {
        PostQuitMessage(0);
        return 0;
    }

    return DefWindowProcW(h, thongDiep, wParam, lParam);
}

// ===== Dang ky lop cua so =====

void DangKyLop() {
    WNDCLASSEXW lop = {0};
    lop.cbSize = sizeof(lop);
    lop.hInstance = hInst;
    lop.hCursor = LoadCursorW(NULL, IDC_ARROW);
    lop.hbrBackground = NULL;

    // Lop cho cua so chinh
    lop.lpfnWndProc = XuLyCuaSo;
    lop.lpszClassName = L"DA030MainForm";
    lop.style = CS_HREDRAW | CS_VREDRAW;
    RegisterClassExW(&lop);

    // Lop cho cac trang con trong tab
    lop.lpfnWndProc = XuLyTrang;
    lop.lpszClassName = L"DA030Trang";
    lop.style = 0;
    RegisterClassExW(&lop);
}

// ===== Ham main =====

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR, int nShowCmd) {
    hInst = hInstance;

    KhoiTaoThuVien();
    gFont = TaoFont();
    KhoiTaoDuLieu();
    DangKyLop();

    cuaSo = CreateWindowExW(0, L"DA030MainForm",
                            L"DA-030 - Điểm danh và xếp hạng học tập",
                            WS_OVERLAPPEDWINDOW,
                            CW_USEDEFAULT, CW_USEDEFAULT, 1000, 660,
                            NULL, NULL, hInst, NULL);
    if (cuaSo == NULL) return 1;

    CanGiuaCuaSo(cuaSo);
    ShowWindow(cuaSo, nShowCmd);
    UpdateWindow(cuaSo);

    // Vong lap nhan thong diep
    // IsDialogMessageW de phim Tab va Enter xu ly duoc trong cac o nhap
    MSG msg;
    while (GetMessageW(&msg, NULL, 0, 0) > 0) {
        if (IsDialogMessageW(cuaSo, &msg) == FALSE) {
            TranslateMessage(&msg);
            DispatchMessageW(&msg);
        }
    }
    return 0;
}
