// DA-030: cac chuc nang cua chuong trinh

#include "common.h"
#include "cautruc.h"

#include <stdio.h>
#include <stdlib.h>

// Ba cau truc du lieu cua chuong trinh
DanhSachDac danhSachDac;        // mang tinh
DanhSachLienKet danhSachLienKet; // danh sach lien ket
CayAVL cayXepHang;              // cay AVL + Order-Statistic Tree
LichSuDiemDanh lichSu;          // lich su cac lan diem danh

// Dong bo ban ghi trong danh sach lien ket voi ban ghi chinh trong mang.
// fileio.cpp doc du lieu de ghi file tu danh sach lien ket, nen hai noi nay
// phai luon cung so buoi co mat, tong so buoi va diem.
void DongBoLienKet(const SinhVien* sv) {
    if (sv == NULL) return;

    NutLienKet* nut = TimLienKet(&danhSachLienKet, sv->ma);
    if (nut != NULL) {
        nut->sv = *sv;
    }
}

// Khoi tao ba cau truc, goi mot lan luc bat dau chuong trinh
void KhoiTaoDuLieu() {
    KhoiTaoDanhSachDac(&danhSachDac);
    KhoiTaoDanhSachLienKet(&danhSachLienKet);
    KhoiTaoCay(&cayXepHang);
    KhoiTaoLichSu(&lichSu);
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

// Tinh lai diem cho tat ca sinh vien con lai
// Goi sau khi xoa sinh vien, vi tong so buoi cua lop co the giam
void TinhLaiDiemTatCa() {
    int tongBuoi = DemTongBuoiCuaLop(&lichSu);

    for (int i = 0; i < danhSachDac.soLuong; i++) {
        SinhVien* sv = &danhSachDac.ds[i];

        sv->soBuoiCoMat = DemBuoiCoMatCuaSV(&lichSu, sv->ma);
        sv->tongSoBuoi = tongBuoi;

        if (tongBuoi > 0) {
            sv->diem = (double)sv->soBuoiCoMat / (double)tongBuoi * 10.0;
        } else {
            sv->diem = 0;
        }

        DongBoLienKet(sv);

        // Diem co the doi nen cap nhat lai trong cay xep hang
        // Neu sinh vien chua co trong cay (vua doc tu file) thi chen moi vao
        if (CapNhatCay(&cayXepHang, sv) == 0) {
            ThemCay(&cayXepHang, sv);
        }
    }
}

// Xoa mot sinh vien khoi danh sach
void XoaSinhVien() {
    wchar_t ma[MAX_CHU];
    DocO(maSinhVien, ma);

    // Kiem tra da nhap ma chua
    if (wcslen(ma) == 0) {
        CanhBao(L"Vui lòng nhập Mã SV cần xóa.");
        return;
    }

    // Tim sinh vien trong danh sach
    int viTri = TimDac(&danhSachDac, ma);
    if (viTri == -1) {
        CanhBao(L"Không tìm thấy sinh viên có mã này.");
        return;
    }

    // Hoi lai cho chac, tranh xoa nham
    wchar_t cauHoi[MAX_CHU];
    swprintf_s(cauHoi, L"Bạn có chắc muốn xóa sinh viên %s không?", ma);

    if (HoiXacNhan(cauHoi) == 0) {
        return;                         // nguoi dung bam Khong
    }

    // Xoa khoi ca ba cau truc du lieu
    XoaDac(&danhSachDac, ma);
    XoaLienKet(&danhSachLienKet, ma);
    XoaCay(&cayXepHang, ma);

    // Xoa cac ban ghi diem danh cua sinh vien nay
    XoaBanGhiCuaSV(&lichSu, ma);

    // Tinh lai diem cho nhung nguoi con lai (tong so buoi lop da thay doi)
    TinhLaiDiemTatCa();

    // Xoa dong trong bang
    XoaDongTheoMa(bangSinhVien, ma);

    // Xoa o nhap va dua con tro ve
    DatChuoi(maSinhVien, L"");
    ChonO(maSinhVien);

    ThongBao(L"Đã xóa sinh viên.");
}

// Sua thong tin mot sinh vien (ho ten va ma lop)
// Ma sinh vien la khoa lien ket ca ba cau truc nen khong cho doi ma
void SuaSinhVien() {
    wchar_t ma[MAX_CHU];
    wchar_t ten[MAX_CHU];
    wchar_t lop[MAX_CHU];

    DocO(maSinhVien, ma);
    DocO(hoTen, ten);
    DocO(maLop, lop);

    // Kiem tra da nhap ma chua
    if (wcslen(ma) == 0) {
        CanhBao(L"Vui lòng nhập Mã SV cần sửa.");
        return;
    }

    // Thong tin moi phai day du
    if (wcslen(ten) == 0 || wcslen(lop) == 0) {
        CanhBao(L"Vui lòng nhập Họ tên và Mã lớp mới.");
        return;
    }

    // Tim sinh vien trong danh sach
    int viTri = TimDac(&danhSachDac, ma);
    if (viTri == -1) {
        CanhBao(L"Không tìm thấy sinh viên có mã này.");
        return;
    }

    // Sua trong danh sach dac
    SinhVien* sv = &danhSachDac.ds[viTri];
    wcscpy_s(sv->hoTen, ten);
    wcscpy_s(sv->maLop, lop);

    // Sua trong danh sach lien ket
    NutLienKet* nut = TimLienKet(&danhSachLienKet, ma);
    if (nut != NULL) {
        wcscpy_s(nut->sv.hoTen, ten);
        wcscpy_s(nut->sv.maLop, lop);
    }

    // Sua trong cay xep hang (diem khong doi nhung ten trong nut phai cap nhat)
    CapNhatCay(&cayXepHang, sv);

    // Cap nhat lai dong trong bang: xoa dong cu roi them dong moi
    XoaDongTheoMa(bangSinhVien, ma);
    ThemDong(bangSinhVien, sv->ma, sv->hoTen, sv->maLop, NULL);

    // Xoa cac o nhap va dua con tro ve
    DatChuoi(maSinhVien, L"");
    DatChuoi(hoTen, L"");
    DatChuoi(maLop, L"");
    ChonO(maSinhVien);

    ThongBao(L"Đã sửa thông tin sinh viên.");
}

// Luu mot lan diem danh
void LuuDiemDanh() {
    wchar_t ma[MAX_CHU];
    DocO(maDiemDanh, ma);

    // Kiem tra da nhap ma sinh vien chua
    if (wcslen(ma) == 0) {
        CanhBao(L"Vui lòng nhập Mã SV cần điểm danh.");
        return;
    }

    // Tim sinh vien trong danh sach
    int viTri = TimDac(&danhSachDac, ma);
    if (viTri == -1) {
        CanhBao(L"Không tìm thấy sinh viên có mã này.");
        return;
    }

    // Lay ngay hoc va trang thai co mat
    SYSTEMTIME ngay = LayNgay(ngayHoc);
    bool coMatHomNay = DangChon(coMat);

    // Chan diem danh trung: mot sinh vien chi duoc diem danh mot lan trong mot ngay
    if (DaDiemDanh(&lichSu, ma, ngay.wDay, ngay.wMonth, ngay.wYear) == 1) {
        CanhBao(L"Sinh viên này đã được điểm danh trong ngày này.");
        return;
    }

    // Luu ban ghi vao lich su
    int coMatSo = 0;
    if (coMatHomNay) {
        coMatSo = 1;
    }

    if (ThemBanGhi(&lichSu, ma, ngay.wDay, ngay.wMonth, ngay.wYear, coMatSo) == 0) {
        CanhBao(L"Lịch sử điểm danh đã đầy.");
        return;
    }

    // Cap nhat so buoi cua tat ca sinh vien va tinh lai diem.
    // Ham nay cung dong bo danh sach lien ket, la noi LuuFile() se doc.
    TinhLaiDiemTatCa();

    // Lay lai ban ghi sau khi dong bo de cap nhat dung thong tin tren bang.
    SinhVien* sv = &danhSachDac.ds[viTri];

    // Hien ket qua len bang diem danh
    wchar_t ngayChu[MAX_CHU];
    swprintf_s(ngayChu, L"%02d/%02d/%04d", ngay.wDay, ngay.wMonth, ngay.wYear);

    wchar_t trangThai[MAX_CHU];
    if (coMatHomNay) {
        wcscpy_s(trangThai, L"Có mặt");
    } else {
        wcscpy_s(trangThai, L"Vắng");
    }

    ThemDong(bangDiemDanh, sv->ma, ngayChu, trangThai, NULL);

    // Xoa o nhap va dua con tro ve
    DatChuoi(maDiemDanh, L"");
    ChonO(maDiemDanh);

    ThongBao(L"Đã lưu điểm danh.");
}

// Xem bang xep hang Top K
void XemXepHang() {
    // Doc so K
    int k = LaySo(topKEdit);
    if (k < 1) {
        CanhBao(L"Số K phải lớn hơn 0.");
        return;
    }

    if (cayXepHang.soLuong == 0) {
        CanhBao(L"Chưa có sinh viên nào trong danh sách.");
        return;
    }

    // Neu K lon hon so sinh vien thi chi lay het danh sach
    if (k > cayXepHang.soLuong) {
        k = cayXepHang.soLuong;
    }

    // Xoa bang cu truoc khi dien ket qua moi
    XoaBang(bangXepHang);

    // Dung Select(k) cua Order-Statistic Tree de lay tung nguoi
    // Khong can sap xep lai danh sach, moi lan lay chi ton O(log n)
    for (int hang = 1; hang <= k; hang++) {
        NutAVL* nut = Select(&cayXepHang, hang);
        if (nut == NULL) {
            break;
        }

        // Doi so hang va diem ra chuoi
        wchar_t hangChu[MAX_CHU];
        swprintf_s(hangChu, L"%d", hang);

        wchar_t diemChu[MAX_CHU];
        swprintf_s(diemChu, L"%.2f", nut->sv.diem);

        ThemDong(bangXepHang, hangChu, nut->sv.ma, nut->sv.hoTen, diemChu);
    }

    // Bao ket qua
    wchar_t thongBao[MAX_CHU];
    swprintf_s(thongBao, L"Đã hiển thị Top %d sinh viên.", k);
    ThongBao(thongBao);
}
