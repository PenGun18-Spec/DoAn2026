# Khung desktop app DA-030

Day la **khung giao dien WinForms duoc viet lai bang C++ thuan + Win32 API** (khong dung .NET / C++/CLI). Van co ba tab: Sinh vien, Diem danh, Xep hang. Cac bang dang trong; bam nut se hien thong bao chua cai dat. Tim `TODO` trong `setting.cpp` de bat dau.

## Cau truc

- Ba file nguon + mot header dung chung, khong phu thuoc .NET Framework:
  - `common.h` - khai bao dung chung: ID control, `struct GiaoDien`, bien toan cuc, prototype.
  - `lib.cpp` - lop goi thu vien Win32: tao control, doc/ghi du lieu, font (`TaoNhan`, `TaoO`, `TaoNut`, `TaoBang`, `LayChuoi`, `ThemDong`, `LayNgay`, `DangChon`, `HienThongBao`...).
  - `setting.cpp` - chuc nang nghiep vu va cau truc du lieu cua nhom (`ThemSinhVien`, `LuuDiemDanh`, `XemXepHang`).
  - `Main.cpp` - giao dien: bo tri, xu ly su kien, `wWinMain`.
- Giao dien: `TabControl` chua 3 trang rieng (lop cua so `DA030TabPage`), moi trang co cac control tuong ung voi ban WinForms:
  - Trang Sinh vien: ba o nhap, nut "Them SV", `ListView` bao cao.
  - Trang Diem danh: o nhap ma SV, `DateTimePicker`, `CheckBox` "Co mat", nut luu, `ListView`.
  - Trang Xep hang: o `Edit` + `UpDown` (Top K, 1..1000), nut "Xem Top K", `ListView`.
- Cac ham tao control: `TaoNhan`, `TaoO`, `TaoNut`, `TaoBang` trong `lib.cpp` (tuong duong `taoNhan`, `taoO`, `taoNut`, `taoBang` cua ban C++/CLI).
- Ba handler giu nguyen khung trong `setting.cpp`: `ThemSinhVien`, `LuuDiemDanh`, `XemXepHang`.

## Mo du an

- Mo thu muc nay bang Visual Studio Code de doc va chinh sua cac file nguon.
- De bien dich, mo `DA030.sln` bang Visual Studio 2022 tren Windows; can bo cong cu C++ (workload Desktop development with C++). Chon `Debug | x64` roi chay.
- Hoac build bang dong lenh:

```
MSBuild DA030.vcxproj /p:Configuration=Debug /p:Platform=x64
```

## Lam lan luot

1. `ThemSinhVien`: doc ba o nhap bang `LayChuoi` va them mot hang vao `bangSinhVien` bang `ThemDong`; du lieu luu trong `gDanhSachSinhVien` (xem khoi `namespace` dau `setting.cpp`).
2. `LuuDiemDanh`: tim ma sinh vien, luu trang thai co mat/vang theo ngay (`LayNgay`).
3. `XemXepHang`: thong nhat cach tinh diem voi nhom, sau do cai cay AVL va Order-Statistic Tree de co `Rank(x)` va `Select(k)`; doc K bang `LayChuoi` tren `g.topKEdit`.
4. Bo sung luu/mo file va xuat bao cao sau khi ba phan tren chay dung.

Luu y: day la giao dien thuc hanh, **chua phai do an hoan chinh**. Giu ca bon file nguon (`Main.cpp`, `lib.cpp`, `setting.cpp`, `common.h`) va `DA030.vcxproj` trong cung thu muc. Ban C++/CLI cu duoc giu lai o `Main.cli.cpp.bak` de doi chieu.
