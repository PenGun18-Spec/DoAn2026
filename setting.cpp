// DA-030: cac chuc nang nghiep vu.
// Moi thao tac doc/ghi control deu di qua lop goi thu vien trong lib.cpp
// (LayChuoi, ThemDong, LayNgay, DangChon, HienThongBao).

#include "common.h"

#include <string>
#include <vector>

namespace {

// ---------------------------------------------------------------------------
// Cai dat: cau truc du lieu cua nhom.
// Giu trong mot khoi de nhom tu mo rong (lop SinhVien, danh sach diem danh...).
// ---------------------------------------------------------------------------

struct SinhVien {
    std::wstring ma;
    std::wstring hoTen;
    std::wstring maLop;
};

// TODO: doi sang cay AVL / Order-Statistic Tree khi lam phan xep hang
// de co Rank(x) va Select(k) thay vi sap xep lai toan bo.
std::vector<SinhVien> gDanhSachSinhVien;

}  // namespace

// ---------------------------------------------------------------------------
// Ba chuc nang chinh: giu nguyen khung TODO de nhom tu cai dat.
// ---------------------------------------------------------------------------

void ThemSinhVien() {
    // TODO 1: Kiem tra maSinhVien, hoTen, maLop.
    SinhVien sv;
    sv.ma = LayChuoi(g.maSinhVien);
    sv.hoTen = LayChuoi(g.hoTen);
    sv.maLop = LayChuoi(g.maLop);

    if (sv.ma.empty() || sv.hoTen.empty() || sv.maLop.empty()) {
        HienCanhBao(g.cuaSo, L"Vui lòng nhập đầy đủ Mã SV, Họ tên, Mã lớp.");
        return;
    }

    // Kiem tra trung ma sinh vien.
    for (const SinhVien& cu : gDanhSachSinhVien) {
        if (cu.ma == sv.ma) {
            HienCanhBao(g.cuaSo, L"Mã sinh viên này đã tồn tại.");
            return;
        }
    }

    // TODO 2: Luu sinh vien vao cau truc du lieu cua nhom.
    gDanhSachSinhVien.push_back(sv);

    // TODO 3: Cap nhat bangSinhVien.
    ThemDong(g.bangSinhVien, {sv.ma.c_str(), sv.hoTen.c_str(), sv.maLop.c_str()});

    DatChuoi(g.maSinhVien, L"");
    DatChuoi(g.hoTen, L"");
    DatChuoi(g.maLop, L"");
    ChonO(g.maSinhVien);

    HienThongBao(g.cuaSo, L"Đã thêm sinh viên.");
}

void LuuDiemDanh() {
    // TODO 1: Tim sinh vien theo maDiemDanh.
    // TODO 2: Luu ngayHoc va trang thai coMat.
    // TODO 3: Cap nhat diem chuyen can va bangDiemDanh.
    HienThongBao(g.cuaSo, L"Chưa cài đặt chức năng điểm danh.");
}

void XemXepHang() {
    // TODO 1: Tinh diem tong ket theo quy tac nhom thong nhat.
    // TODO 2: Dung Order-Statistic Tree de lay Top K.
    // TODO 3: Dien ket qua vao bangXepHang.
    HienThongBao(g.cuaSo, L"Chưa cài đặt chức năng xếp hạng.");
}
