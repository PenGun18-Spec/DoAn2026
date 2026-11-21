// DA-030: luu va doc du lieu tu file
// Dung file text phan cach bang dau | cho de mo bang Notepad

#include "common.h"
#include "cautruc.h"
#include "fileio.h"

#include <stdio.h>
#include <stdlib.h>

// Hai file du lieu nam cung thu muc voi file .exe
const wchar_t* TEN_FILE_SV = L"sinhvien.txt";
const wchar_t* TEN_FILE_DD = L"diemdanh.txt";

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

    for (NutLienKet* p = danhSachLienKet.dau; p != NULL; p = p->next) {
        fwprintf(f, L"%s|%s|%s|%d|%d\n",
                 p->sv.ma, p->sv.hoTen, p->sv.maLop,
                 p->sv.soBuoiCoMat, p->sv.tongSoBuoi);
    }
    fclose(f);

    // --- Ghi lich su diem danh ---
    _wfopen_s(&f, TEN_FILE_DD, L"w, ccs=UTF-8");
    if (f == NULL) {
        CanhBao(L"Không mở được file diemdanh.txt để ghi.");
        return;
    }

    for (int i = 0; i < lichSu.soLuong; i++) {
        fwprintf(f, L"%s|%d|%d|%d|%d\n",
                 lichSu.ds[i].ma,
                 lichSu.ds[i].ngay, lichSu.ds[i].thang, lichSu.ds[i].nam,
                 lichSu.ds[i].coMat);
    }
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

    // Moi dong co dang: ma|hoTen|maLop|soBuoiCoMat|tongSoBuoi
    while (fwscanf_s(f, L"%99[^|]|%99[^|]|%99[^|]|%d|%d\n",
                     ma, (unsigned)_countof(ma),
                     ten, (unsigned)_countof(ten),
                     lop, (unsigned)_countof(lop),
                     &soBuoiCoMat, &tongSoBuoi) == 5) {
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
        int ngay, thang, nam, coMat;

        // Moi dong co dang: ma|ngay|thang|nam|coMat
        while (fwscanf_s(f, L"%99[^|]|%d|%d|%d|%d\n",
                         maDD, (unsigned)_countof(maDD),
                         &ngay, &thang, &nam, &coMat) == 5) {
            ThemBanGhi(&lichSu, maDD, ngay, thang, nam, coMat);
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
