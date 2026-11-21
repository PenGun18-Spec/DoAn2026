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

// Khai bao Common Controls 6.0 (de nut va bang co giao dien hien dai)
#pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

// So ky tu toi da cua mot o nhap
const int MAX_CHU = 100;

// ===== ID cua cac control =====
const int IDC_TAB = 100;

const int IDC_MA_SV = 101;
const int IDC_HO_TEN = 102;
const int IDC_MA_LOP = 103;
const int IDC_BTN_THEM_SV = 104;
const int IDC_BANG_SINH_VIEN = 105;
const int IDC_BTN_XOA_SV = 106;
const int IDC_BTN_SUA_SV = 107;
const int IDC_BTN_LUU_FILE = 108;
const int IDC_BTN_DOC_FILE = 109;

const int IDC_MA_DIEM_DANH = 110;
const int IDC_NGAY_HOC = 111;
const int IDC_CO_MAT = 112;
const int IDC_BTN_LUU_DIEM_DANH = 113;
const int IDC_BANG_DIEM_DANH = 114;

const int IDC_TOP_K_EDIT = 121;
const int IDC_BTN_XEM_TOP_K = 122;
const int IDC_BANG_XEP_HANG = 123;

// ===== Bien toan cuc =====
// hInst va gFont dinh nghia trong lib.cpp
extern HINSTANCE hInst;
extern HFONT gFont;

// Cac cua so con, dinh nghia trong Main.cpp
extern HWND cuaSo, tab;
extern HWND trang[3];

extern HWND maSinhVien, hoTen, maLop, bangSinhVien;
extern HWND maDiemDanh, ngayHoc, coMat, bangDiemDanh;
extern HWND topKEdit, bangXepHang;

// ===== Mot sinh vien =====
struct SinhVien {
    wchar_t ma[MAX_CHU];        // ma sinh vien
    wchar_t hoTen[MAX_CHU];     // ho va ten
    wchar_t maLop[MAX_CHU];     // lop
    int soBuoiCoMat;            // so buoi di hoc
    int tongSoBuoi;             // tong so buoi da diem danh
    double diem;                // diem tong ket = ti le chuyen can * 10
};

// ===== Cac ham dung chung =====
void KhoiTaoThuVien();
HFONT TaoFont();
void DatFont(HWND h);

HWND TaoNhan(HWND cha, const wchar_t* chu, int x, int y, int rong);
HWND TaoO(HWND cha, int id, int x, int y, int rong);
HWND TaoNut(HWND cha, int id, const wchar_t* chu, int x, int y, int rong);
HWND TaoBang(HWND cha, int id, const wchar_t* cot0, const wchar_t* cot1,
             const wchar_t* cot2, const wchar_t* cot3, int y);

void DocO(HWND o, wchar_t* ketQua);
void DatChuoi(HWND o, const wchar_t* chu);
int  LaySo(HWND o);                         // doc mot so nguyen tu o nhap
void ChonO(HWND o);
void ThemDong(HWND bang, const wchar_t* cot0, const wchar_t* cot1,
              const wchar_t* cot2, const wchar_t* cot3);
void XoaBang(HWND bang);                    // xoa het dong trong bang
void XoaDongTheoMa(HWND bang, const wchar_t* ma);   // xoa dong co cot 0 = ma
SYSTEMTIME LayNgay(HWND chonNgay);
bool DangChon(HWND hopKiem);
int  HoiXacNhan(const wchar_t* chu);        // hop thoai Co/Khong, 1 = Co
void ThongBao(const wchar_t* chu);
void CanhBao(const wchar_t* chu);
void CanGiuaCuaSo(HWND h);

// ===== setting.cpp: cac chuc nang =====
void KhoiTaoDuLieu();
void ThemSinhVien();
void XoaSinhVien();
void SuaSinhVien();
void LuuDiemDanh();
void XemXepHang();

// ===== fileio.cpp: luu doc file =====
void LuuFile();
void DocFile();
