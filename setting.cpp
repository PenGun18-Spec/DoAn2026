// DA-030: cac chuc nang cua chuong trinh

#include "common.h"
#include "cautruc.h"

// Ba cau truc du lieu cua chuong trinh
DanhSachDac danhSachDac;        // mang tinh
DanhSachLienKet danhSachLienKet; // danh sach lien ket
CayAVL cayXepHang;              // cay AVL + Order-Statistic Tree

// Khoi tao ba cau truc, goi mot lan luc bat dau chuong trinh
void KhoiTaoDuLieu() {
    KhoiTaoDanhSachDac(&danhSachDac);
    KhoiTaoDanhSachLienKet(&danhSachLienKet);
    KhoiTaoCay(&cayXepHang);
}

// Them mot sinh vien vao danh sach va vao bang
void ThemSinhVien() {
    wchar_t ma[MAX_CHU];
    wchar_t ten[MAX_CHU];
    wchar_t lop[MAX_CHU];

    DocO(maSinhVien, ma);
    DocO(hoTen, ten);
    DocO(maLop, lop);

    // Kiem tra da nhap day du chua
    if (wcslen(ma) == 0 || wcslen(ten) == 0 || wcslen(lop) == 0) {
        CanhBao(L"Vui lòng nhập đầy đủ Mã SV, Họ tên, Mã lớp.");
        return;
    }

    // Kiem tra trung ma sinh vien
    if (TimDac(&danhSachDac, ma) != -1) {
        CanhBao(L"Mã sinh viên này đã tồn tại.");
        return;
    }

    // Tao mot sinh vien moi
    SinhVien sv;
    wcscpy_s(sv.ma, ma);
    wcscpy_s(sv.hoTen, ten);
    wcscpy_s(sv.maLop, lop);
    sv.soBuoiCoMat = 0;
    sv.tongSoBuoi = 0;
    sv.diem = 0;

    // Luu vao ca ba cau truc du lieu
    if (ThemDac(&danhSachDac, &sv) == 0) {
        CanhBao(L"Danh sách sinh viên đã đầy.");
        return;
    }
    ThemLienKet(&danhSachLienKet, &sv);
    ThemCay(&cayXepHang, &sv);

    // Them mot dong vao bang
    ThemDong(bangSinhVien, ma, ten, lop, NULL);

    // Xoa cac o nhap va dua con tro ve o Ma SV
    DatChuoi(maSinhVien, L"");
    DatChuoi(hoTen, L"");
    DatChuoi(maLop, L"");
    ChonO(maSinhVien);

    ThongBao(L"Đã thêm sinh viên.");
}

// Luu mot lan diem danh
void LuuDiemDanh() {
    // TODO: Doc ma sinh vien, ngay hoc, trang thai co mat.
    //       Cap nhat soBuoiCoMat / tongSoBuoi, tinh lai diem,
    //       roi cap nhat lai cay xep hang.
    ThongBao(L"Chưa cài đặt chức năng điểm danh.");
}

// Xem bang xep hang Top K
void XemXepHang() {
    // TODO: Doc K, dung Select(k) cua Order-Statistic Tree
    //       de lay K nguoi diem cao nhat, dien vao bang xep hang.
    ThongBao(L"Chưa cài đặt chức năng xếp hạng.");
}
