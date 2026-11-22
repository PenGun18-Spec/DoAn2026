#pragma once

#include "common.h"

// ===========================================================================
// 1. DANH SACH DAC (mang tinh)
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
int  CapNhatCay(CayAVL* c, const SinhVien* sv);     // diem doi: xoa roi chen lai
void XoaToanBoCay(CayAVL* c);

// ----- Order-Statistic Tree -----
// Cay sap theo diem giam dan (diem cao nam ben trai). Bang diem thi so theo ma
// sinh vien de thu tu duoc xac dinh ro rang.
NutAVL* Select(CayAVL* c, int k);                   // nguoi thu k (1..n), NULL neu ngoai
int     Rank(CayAVL* c, double diem);               // hang cua mot muc diem, 0 neu khong co

// ===========================================================================
// 4. LICH SU DIEM DANH
// ===========================================================================


const int MAX_DIEM_DANH = 1000;     // so ban ghi toi da

struct BanGhiDiemDanh {
    wchar_t ma[MAX_CHU];            // ma sinh vien
    int ngay;                       // ngay hoc (1..31)
    int thang;                      // thang hoc (1..12)
    int nam;                        // nam hoc
    int coMat;                      // 1 = co mat, 0 = vang
};

struct LichSuDiemDanh {
    BanGhiDiemDanh ds[MAX_DIEM_DANH];
    int soLuong;
};

void KhoiTaoLichSu(LichSuDiemDanh* l);
int  DaDiemDanh(LichSuDiemDanh* l, const wchar_t* ma, int ngay, int thang, int nam);
int  ThemBanGhi(LichSuDiemDanh* l, const wchar_t* ma, int ngay, int thang, int nam, int coMat);
int  DemTongBuoiCuaLop(LichSuDiemDanh* l);          // so buoi hoc da diem danh
int  DemBuoiCoMatCuaSV(LichSuDiemDanh* l, const wchar_t* ma);
int  XoaBanGhiCuaSV(LichSuDiemDanh* l, const wchar_t* ma);  // xoa moi ban ghi cua 1 SV
