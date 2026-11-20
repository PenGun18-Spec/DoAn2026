// DA-030: khai bao ba cau truc du lieu tu cai dat
//   Danh sach dac        - mang tinh
//   Danh sach lien ket   - cac nut noi nhau bang con tro
//   Cay AVL              - cay nhi phan tim kiem tu can bang
//   Order-Statistic Tree - cay AVL mo rong, moi nut luu them so nut cua cay con
//
// Nho hai ham Select(k) va Rank(x) cua Order-Statistic Tree ma tim nguoi thu k
// va tim thu hang cua mot nguoi chi ton O(log n), khong can sap xep lai danh sach.

#pragma once

#include "common.h"

// ===========================================================================
// 1. DANH SACH DAC (mang tinh)
// ===========================================================================
// Luu sinh vien trong mot mang, them vao cuoi.
// Them nhanh, nhung xoa phai don mang len.
// ===========================================================================

const int MAX_SINH_VIEN = 100;

struct DanhSachDac {
    SinhVien ds[MAX_SINH_VIEN];
    int soLuong;
};

void KhoiTaoDanhSachDac(DanhSachDac* d);
int  ThemDac(DanhSachDac* d, const SinhVien* sv);   // them vao cuoi, 0 neu mang day
int  TimDac(DanhSachDac* d, const wchar_t* ma);     // tra ve chi so, -1 neu khong co
int  XoaDac(DanhSachDac* d, const wchar_t* ma);     // 1 neu xoa duoc

// ===========================================================================
// 2. DANH SACH LIEN KET
// ===========================================================================
// Moi sinh vien nam trong mot nut, cac nut noi voi nhau bang con tro next.
// Them/xoa khong phai don mang, nhung muon lay nut thu i phai duyet tu dau.
// ===========================================================================

struct NutLienKet {
    SinhVien sv;
    NutLienKet* next;
};

struct DanhSachLienKet {
    NutLienKet* dau;
    int soLuong;
};

void KhoiTaoDanhSachLienKet(DanhSachLienKet* d);
int  ThemLienKet(DanhSachLienKet* d, const SinhVien* sv);   // them vao cuoi
NutLienKet* TimLienKet(DanhSachLienKet* d, const wchar_t* ma);
int  XoaLienKet(DanhSachLienKet* d, const wchar_t* ma);
void XoaToanBoLienKet(DanhSachLienKet* d);                  // giai phong bo nho

// ===========================================================================
// 3. CAY AVL va ORDER-STATISTIC TREE
// ===========================================================================
// Cay AVL: cay nhi phan tim kiem tu can bang. Sau moi lan chen/xoa, do cao hai
// ben chenh nhau khong qua 1, nho vay cay luon thap (~log n).
//
// Order-Statistic Tree: cay AVL co them truong "size" = so nut cua cay con
// (tinh ca nut do). Nho truong nay:
//   - Select(k): tim nut thu k theo thu tu tu diem cao xuong thap, O(log n)
//   - Rank(x)  : tim thu hang cua nut x, O(log n)
// ===========================================================================

struct NutAVL {
    SinhVien sv;
    int chieuCao;       // do cao cay con tai nut nay
    int size;           // so nut cay con (tinh ca nut nay) - dung cho OST
    NutAVL* trai;
    NutAVL* phai;
};

struct CayAVL {
    NutAVL* goc;
    int soLuong;
};

void KhoiTaoCay(CayAVL* c);
int  ChieuCao(NutAVL* n);
int  LaySize(NutAVL* n);
void CapNhat(NutAVL* n);                            // tinh lai chieuCao va size
NutAVL* XoayTrai(NutAVL* n);
NutAVL* XoayPhai(NutAVL* n);
NutAVL* CanBang(NutAVL* p);                         // xoay neu nut p bi lech
int  SoSanh(const SinhVien* a, const SinhVien* b);  // so sanh de sap xep trong cay
NutAVL* ChenNut(NutAVL* p, const SinhVien* sv);
NutAVL* TimNut(NutAVL* p, const wchar_t* ma);
NutAVL* NutNhoNhat(NutAVL* p);

int  ThemCay(CayAVL* c, const SinhVien* sv);        // chen theo diem
int  TimCay(CayAVL* c, const wchar_t* ma);          // 1 neu co
int  XoaCay(CayAVL* c, const wchar_t* ma);          // 1 neu xoa duoc
void XoaToanBoCay(CayAVL* c);

// ----- Order-Statistic Tree -----
// Cay sap theo diem giam dan (diem cao nam ben trai). Bang diem thi so theo ma
// sinh vien de thu tu duoc xac dinh ro rang.
NutAVL* Select(CayAVL* c, int k);                   // nguoi thu k (1..n), NULL neu ngoai
int     Rank(CayAVL* c, double diem);               // hang cua mot muc diem, 0 neu khong co
