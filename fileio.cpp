// DA-030: luu va doc du lieu tu file
// Dung file text phan cach bang dau | cho de mo bang Notepad

#include "common.h"
#include "cautruc.h"
#include "fileio.h"

#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>

// Xoa khoang trang o dau/cuoi mot truong doc tu file bang.
void CatKhoangTrang(wchar_t* chuoi) {
    if (chuoi == NULL) return;

    wchar_t* dau = chuoi;
    while (*dau == L' ' || *dau == L'\t') dau++;
    if (dau != chuoi) {
        size_t doDai = wcslen(dau);
        wmemmove(chuoi, dau, doDai + 1);
    }

    size_t doDai = wcslen(chuoi);
    while (doDai > 0 && (chuoi[doDai - 1] == L' ' || chuoi[doDai - 1] == L'\t' ||
                         chuoi[doDai - 1] == L'\r' || chuoi[doDai - 1] == L'\n')) {
        chuoi[--doDai] = L'\0';
    }
}

// Hai file du lieu nam cung thu muc voi file .exe
const wchar_t* TEN_FILE_SV = L"sinhvien.txt";
const wchar_t* TEN_FILE_DD = L"diemdanh.txt";
const wchar_t* TEN_FILE_BC = L"baocao.txt";

// Ba cau truc du lieu chinh nam trong setting.cpp
extern DanhSachDac danhSachDac;
extern DanhSachLienKet danhSachLienKet;
extern CayAVL cayXepHang;
extern LichSuDiemDanh lichSu;

// Tinh lai diem cho tat ca sinh vien (dinh nghia trong setting.cpp)
void TinhLaiDiemTatCa();

// Xoa du lieu cu trong bo nho truoc khi nap du lieu moi tu file
void XoaDuLieuCu() {
    KhoiTaoDanhSachDac(&danhSachDac);
    XoaToanBoLienKet(&danhSachLienKet);
    XoaToanBoCay(&cayXepHang);
    KhoiTaoLichSu(&lichSu);

    XoaBang(bangSinhVien);
    XoaBang(bangDiemDanh);
    XoaBang(bangXepHang);
}

// Nap lai bang sinh vien tren man hinh tu danh sach lien ket
void NapBangSinhVien() {
    XoaBang(bangSinhVien);

    for (NutLienKet* p = danhSachLienKet.dau; p != NULL; p = p->next) {
        ThemDong(bangSinhVien, p->sv.ma, p->sv.hoTen, p->sv.maLop, NULL);
    }
}

// ===== LUU FILE =====
// sinhvien.txt : ma|hoTen|maLop|soBuoiCoMat|tongSoBuoi
// diemdanh.txt : ma|ngay|thang|nam|coMat
void LuuFile() {
    FILE* f = NULL;

    // --- Ghi danh sach sinh vien ---
    _wfopen_s(&f, TEN_FILE_SV, L"w, ccs=UTF-8");
    if (f == NULL) {
        CanhBao(L"Không mở được file sinhvien.txt để ghi.");
        return;
    }

    fwprintf(f, L"DANH SACH SINH VIEN - DA-030\n");
    fwprintf(f, L"==========================================================================\n");
    fwprintf(f, L"Ma SV        | Ho va ten                 | Ma lop | Co mat | Tong buoi\n");
    fwprintf(f, L"-------------+---------------------------+--------+--------+-----------\n");

    for (NutLienKet* p = danhSachLienKet.dau; p != NULL; p = p->next) {
        fwprintf(f, L"%-12s | %-25s | %-6s | %6d | %9d\n",
                 p->sv.ma, p->sv.hoTen, p->sv.maLop,
                 p->sv.soBuoiCoMat, p->sv.tongSoBuoi);
    }
    fwprintf(f, L"==========================================================================\n");
    fwprintf(f, L"Ghi chu: Diem duoc tinh tu so buoi co mat / tong so buoi * 10.\n");
    fclose(f);

    // --- Ghi lich su diem danh ---
    _wfopen_s(&f, TEN_FILE_DD, L"w, ccs=UTF-8");
    if (f == NULL) {
        CanhBao(L"Không mở được file diemdanh.txt để ghi.");
        return;
    }

    fwprintf(f, L"LICH SU DIEM DANH - DA-030\n");
    fwprintf(f, L"===============================================================\n");
    fwprintf(f, L"Ma SV        | Ngay hoc   | Trang thai\n");
    fwprintf(f, L"-------------+------------+-----------\n");
    for (int i = 0; i < lichSu.soLuong; i++) {
        fwprintf(f, L"%-12s | %02d/%02d/%04d | %s\n",
                 lichSu.ds[i].ma,
                 lichSu.ds[i].ngay, lichSu.ds[i].thang, lichSu.ds[i].nam,
                 lichSu.ds[i].coMat ? L"Co mat" : L"Vang");
    }
    fwprintf(f, L"===============================================================\n");
    fwprintf(f, L"Ghi chu: Co mat = da tham gia buoi hoc; Vang = khong tham gia.\n");
    fclose(f);

    ThongBao(L"Đã lưu dữ liệu vào file.");
}

// ===== DOC FILE =====
void DocFile() {
    FILE* f = NULL;

    // --- Doc danh sach sinh vien ---
    _wfopen_s(&f, TEN_FILE_SV, L"r, ccs=UTF-8");
    if (f == NULL) {
        CanhBao(L"Không tìm thấy file sinhvien.txt.");
        return;
    }

    // Xoa het du lieu cu trong bo nho roi nap lai tu dau
    XoaDuLieuCu();

    wchar_t ma[MAX_CHU];
    wchar_t ten[MAX_CHU];
    wchar_t lop[MAX_CHU];
    int soBuoiCoMat;
    int tongSoBuoi;

    // Doc ca dinh dang bang moi va dinh dang cu dang ma|ten|lop|coMat|tongBuoi.
    wchar_t dong[512];
    while (fgetws(dong, (int)_countof(dong), f) != NULL) {
        CatKhoangTrang(dong);
        if (dong[0] == L'\0' || dong[0] == L'=' || dong[0] == L'-' ||
            wcsncmp(dong, L"DANH SACH", 9) == 0 ||
            wcsncmp(dong, L"Ma SV", 5) == 0 ||
            wcsncmp(dong, L"Ghi chu:", 8) == 0) {
            continue;
        }

        if (swscanf_s(dong, L"%99[^|]|%99[^|]|%99[^|]|%d|%d",
                      ma, (unsigned)_countof(ma),
                      ten, (unsigned)_countof(ten),
                      lop, (unsigned)_countof(lop),
                      &soBuoiCoMat, &tongSoBuoi) != 5) {
            continue;
        }
        CatKhoangTrang(ma);
        CatKhoangTrang(ten);
        CatKhoangTrang(lop);

        SinhVien sv;
        wcscpy_s(sv.ma, ma);
        wcscpy_s(sv.hoTen, ten);
        wcscpy_s(sv.maLop, lop);
        sv.soBuoiCoMat = soBuoiCoMat;
        sv.tongSoBuoi = tongSoBuoi;
        sv.diem = 0;

        ThemDac(&danhSachDac, &sv);
        ThemLienKet(&danhSachLienKet, &sv);
    }
    fclose(f);

    // --- Doc lich su diem danh ---
    _wfopen_s(&f, TEN_FILE_DD, L"r, ccs=UTF-8");

    if (f != NULL) {
        wchar_t maDD[MAX_CHU];
        wchar_t trangThai[MAX_CHU];
        int ngay, thang, nam, coMat;
        wchar_t dongDD[512];

        // Doc dinh dang moi: ma | dd/mm/yyyy | Co mat/Vang.
        // Van ho tro dinh dang cu: ma|ngay|thang|nam|0/1.
        while (fgetws(dongDD, (int)_countof(dongDD), f) != NULL) {
            CatKhoangTrang(dongDD);
            if (dongDD[0] == L'\0' || dongDD[0] == L'=' || dongDD[0] == L'-' ||
                wcsncmp(dongDD, L"LICH SU", 7) == 0 ||
                wcsncmp(dongDD, L"Ma SV", 5) == 0 ||
                wcsncmp(dongDD, L"Ghi chu:", 8) == 0) {
                continue;
            }

            int soTruong = swscanf_s(dongDD, L"%99[^|]|%d/%d/%d|%99[^\\r\\n]",
                                     maDD, (unsigned)_countof(maDD),
                                     &ngay, &thang, &nam,
                                     trangThai, (unsigned)_countof(trangThai));
            if (soTruong == 5) {
                CatKhoangTrang(maDD);
                CatKhoangTrang(trangThai);
                coMat = (_wcsnicmp(trangThai, L"Co mat", 6) == 0) ? 1 : 0;
                ThemBanGhi(&lichSu, maDD, ngay, thang, nam, coMat);
                continue;
            }

            // Fallback cho file cu co 5 truong so nguyen.
            if (swscanf_s(dongDD, L"%99[^|]|%d|%d|%d|%d",
                          maDD, (unsigned)_countof(maDD),
                          &ngay, &thang, &nam, &coMat) == 5) {
                CatKhoangTrang(maDD);
                ThemBanGhi(&lichSu, maDD, ngay, thang, nam, coMat);
            }
        }
        fclose(f);
    }

    if (lichSu.soLuong > 0) {
        // Co lich su diem danh thi tinh lai diem tu lich su
        TinhLaiDiemTatCa();
    } else {
        // Khong co lich su thi dung so buoi da luu san trong file sinh vien
        for (int i = 0; i < danhSachDac.soLuong; i++) {
            SinhVien* sv = &danhSachDac.ds[i];

            if (sv->tongSoBuoi > 0) {
                sv->diem = (double)sv->soBuoiCoMat / (double)sv->tongSoBuoi * 10.0;
            } else {
                sv->diem = 0;
            }

            ThemCay(&cayXepHang, sv);
        }
    }

    // Nap lai bang sinh vien tren man hinh
    NapBangSinhVien();

    // Nap lai bang diem danh tren man hinh
    for (int i = 0; i < lichSu.soLuong; i++) {
        wchar_t ngayChu[MAX_CHU];
        swprintf_s(ngayChu, L"%02d/%02d/%04d",
                   lichSu.ds[i].ngay, lichSu.ds[i].thang, lichSu.ds[i].nam);

        wchar_t trangThai[MAX_CHU];
        if (lichSu.ds[i].coMat == 1) {
            wcscpy_s(trangThai, L"Có mặt");
        } else {
            wcscpy_s(trangThai, L"Vắng");
        }

        ThemDong(bangDiemDanh, lichSu.ds[i].ma, ngayChu, trangThai, NULL);
    }

    ThongBao(L"Đã đọc dữ liệu từ file.");
}

// ===== XUAT BAO CAO HOC VU =====
// Ghi ra baocao.txt: thong ke tong ket + bang xep hang day du
// Bang xep hang lay tu cay Order-Statistic Tree qua ham Select(k)
void XuatBaoCao() {
    if (danhSachDac.soLuong == 0) {
        CanhBao(L"Chưa có sinh viên nào để xuất báo cáo.");
        return;
    }

    FILE* f = NULL;
    _wfopen_s(&f, TEN_FILE_BC, L"w, ccs=UTF-8");
    if (f == NULL) {
        CanhBao(L"Không mở được file baocao.txt để ghi.");
        return;
    }

    // Lay ngay thang hien tai de ghi vao bao cao
    SYSTEMTIME t;
    GetLocalTime(&t);

    fwprintf(f, L"================================================================================\n");
    fwprintf(f, L"                 BAO CAO HOC VU - DA-030\n");
    fwprintf(f, L"                 DIEM DANH VA XEP HANG\n");
    fwprintf(f, L"Ngay xuat bao cao: %02d/%02d/%04d\n", t.wDay, t.wMonth, t.wYear);
    fwprintf(f, L"================================================================================\n\n");

    // ----- Phan 1: thong ke tong ket cua lop -----
    int tongBuoi = DemTongBuoiCuaLop(&lichSu);

    double tongDiem = 0;
    for (int i = 0; i < danhSachDac.soLuong; i++) {
        tongDiem += danhSachDac.ds[i].diem;
    }

    double diemTrungBinh = 0;
    if (danhSachDac.soLuong > 0) {
        diemTrungBinh = tongDiem / danhSachDac.soLuong;
    }

    fwprintf(f, L"------------------------------ THONG KE LOP -------------------------------\n");
    fwprintf(f, L"Tong so sinh vien       : %d\n", danhSachDac.soLuong);
    fwprintf(f, L"Tong so buoi hoc        : %d\n", tongBuoi);
    fwprintf(f, L"Diem trung binh lop     : %.2f\n", diemTrungBinh);
    fwprintf(f, L"================================================================================\n\n");

    // ----- Phan 2: bang xep hang day du -----
    // Duyet cay Order-Statistic Tree: Select(k) tra ve nguoi thu k
    // tu diem cao xuong thap, moi lan goi ton O(log n)
    fwprintf(f, L"------------------------------ BANG XEP HANG -------------------------------\n");
    fwprintf(f, L"Hang | Ma SV      | Ho va ten                 | Lop    | Co mat | Tong buoi | Diem\n");
    fwprintf(f, L"-----+------------+---------------------------+--------+--------+-----------+-----\n");

    for (int hang = 1; hang <= cayXepHang.soLuong; hang++) {
        NutAVL* nut = Select(&cayXepHang, hang);
        if (nut == NULL) {
            break;
        }

        fwprintf(f, L"%4d | %-10s | %-25s | %-6s | %6d | %9d | %4.2f\n",
                 hang, nut->sv.ma, nut->sv.hoTen, nut->sv.maLop,
                 nut->sv.soBuoiCoMat, nut->sv.tongSoBuoi, nut->sv.diem);
    }

    fwprintf(f, L"\n================================================================================\n");
    fwprintf(f, L"Cong thuc diem: so buoi co mat / tong so buoi cua lop * 10\n");
    fwprintf(f, L"Ghi chu: Hang 1 la sinh vien co diem chuyen can cao nhat.\n");

    fclose(f);

    ThongBao(L"Đã xuất báo cáo học vụ ra file baocao.txt.");
}
