// DA-030: khai bao dung chung cho ba file nguon.
//   lib.cpp     - lop goi thu vien Win32: tao control, doc/ghi du lieu, font.
//   setting.cpp - cac chuc nang nghiep vu (them SV, diem danh, xep hang).
//   main.cpp    - giao dien: bo tri, xu ly su kien, diem vao chuong trinh.

#pragma once

#ifndef UNICODE
#define UNICODE
#endif
#ifndef _UNICODE
#define _UNICODE
#endif

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <commctrl.h>

#include <initializer_list>
#include <string>

// Khai bao phu thuoc Windows Common Controls 6.0 (visual styles).
#pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

// ---------------------------------------------------------------------------
// Ten lop cua so va chuoi dung chung.
// ---------------------------------------------------------------------------
inline constexpr const wchar_t* kLopChinh = L"DA030MainForm";
inline constexpr const wchar_t* kLopTrang = L"DA030TabPage";
inline constexpr const wchar_t* kTieuDe = L"DA-030 - Điểm danh và xếp hạng học tập";

enum : int {
    IDC_TAB = 100,

    IDC_MA_SV = 101,
    IDC_HO_TEN = 102,
    IDC_MA_LOP = 103,
    IDC_BTN_THEM_SV = 104,
    IDC_BANG_SINH_VIEN = 105,

    IDC_MA_DIEM_DANH = 110,
    IDC_NGAY_HOC = 111,
    IDC_CO_MAT = 112,
    IDC_BTN_LUU_DIEM_DANH = 113,
    IDC_BANG_DIEM_DANH = 114,

    IDC_TOP_K = 120,
    IDC_TOP_K_EDIT = 121,
    IDC_BTN_XEM_TOP_K = 122,
    IDC_BANG_XEP_HANG = 123
};

// Vi tri va kich thuoc dung chung cho ba bang.
inline constexpr int kBangX = 18;
inline constexpr int kBangY = 70;
inline constexpr int kBangChieuCaoToiThieu = 140;

// Trang thai giao dien: moi HWND cua form nam trong mot struct duy nhat.
struct GiaoDien {
    HWND cuaSo = nullptr;
    HWND tab = nullptr;
    HWND trang[3] = {nullptr, nullptr, nullptr};
    int trangDangChon = 0;

    // Tab "Sinh vien".
    HWND maSinhVien = nullptr;
    HWND hoTen = nullptr;
    HWND maLop = nullptr;
    HWND nhanMaLop = nullptr;
    HWND nutThemSv = nullptr;
    HWND bangSinhVien = nullptr;

    // Tab "Diem danh".
    HWND maDiemDanh = nullptr;
    HWND ngayHoc = nullptr;
    HWND coMat = nullptr;
    HWND nutLuuDiemDanh = nullptr;
    HWND bangDiemDanh = nullptr;

    // Tab "Xep hang".
    HWND topK = nullptr;
    HWND topKEdit = nullptr;
    HWND bangXepHang = nullptr;
};

// Bien toan cuc: hInst/gFont dinh nghia trong lib.cpp, g dinh nghia trong main.cpp.
extern HINSTANCE hInst;
extern HFONT gFont;
extern GiaoDien g;

// ---------------------------------------------------------------------------
// lib.cpp - lop goi thu vien Win32.
// ---------------------------------------------------------------------------
void KhoiTaoThuVien();
void CanGiuaCuaSo(HWND cuaSo);

HFONT TaoFont();
void HuyFont();
void DatFont(HWND dieuKhien);

HWND TaoNhan(HWND cha, const wchar_t* chu, int x, int y, int rong);
HWND TaoO(HWND cha, int id, int x, int y, int rong);
HWND TaoNut(HWND cha, int id, const wchar_t* chu, int x, int y, int rong);
HWND TaoBang(HWND cha, int id, std::initializer_list<const wchar_t*> cot, int y);

std::wstring LayChuoi(HWND o);
void DatChuoi(HWND o, const wchar_t* chu);
void ChonO(HWND o);
void ThemDong(HWND bang, std::initializer_list<const wchar_t*> giaTri);
SYSTEMTIME LayNgay(HWND chonNgay);
bool DangChon(HWND hopKiem);
void HienThongBao(HWND cha, const wchar_t* chu);
void HienCanhBao(HWND cha, const wchar_t* chu);

// ---------------------------------------------------------------------------
// setting.cpp - chuc nang nghiep vu.
// ---------------------------------------------------------------------------
void ThemSinhVien();
void LuuDiemDanh();
void XemXepHang();
