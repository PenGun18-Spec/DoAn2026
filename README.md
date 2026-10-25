# DA-030 - C++17 native / Windows API

Du an da chuyen tu C++/CLI + Windows Forms sang C++17 native + Win32.
Khong can .NET Framework hay C++/CLI. Giao dien van co ba tab:
Sinh vien, Diem danh, Xep hang.

## Visual Studio

1. Cai workload Desktop development with C++ va Windows SDK.
2. Mo DA030.sln, chon Debug | x64 hoac Release | x64.
3. Build va chay bang Ctrl+F5. Project dung toolset v143 (VS 2022).
   Neu dung Visual Studio moi hon, retarget Platform Toolset trong Project Properties.

## MinGW-w64 / VS Code

Chay lenh trong terminal tai thu muc du an (can g++ cua MinGW-w64 trong PATH):

```powershell
g++ -std=c++17 -Wall -Wextra Main.cpp -o DA030.exe -mwindows -lcomctl32 -lgdi32 -luser32
.\DA030.exe
```

VS Code la trinh soan thao; can cai compiler rieng. Khong dung compiler C++/CLI.
Du an su dung Windows API nen chi build giao dien nay tren Windows.

## CMake (tuy chon)

```powershell
cmake -S . -B build
cmake --build build --config Debug
```

## Trang thai chuc nang

Day van la khung giao dien, chua phai ung dung hoan chinh.
Ba ham themSinhVien, luuDiemDanh, xemXepHang giu TODO nhu ban goc:
- Kiem tra va luu thong tin sinh vien, cap nhat bang.
- Tim sinh vien, luu ngay hoc va trang thai diem danh.
- Tinh diem theo quy tac nhom, cai AVL / Order-Statistic Tree de xep hang.
- Bo sung luu/mo file va xuat bao cao sau.

Du lieu chua duoc luu. Cac nut hien thong bao chua cai dat.

## Cap nhat GitHub

Sua cac file trong thu muc repository ma GitHub Desktop mo bang Show in Explorer.
Sau do Commit to main va Push origin. Ban sao o thu muc khac khong tu dong dong bo.
