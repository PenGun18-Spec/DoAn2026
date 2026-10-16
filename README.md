# Khung desktop app DA-030

Đây là **khung giao diện WinForms bằng C++/CLI** để bạn tự làm từng chức năng. Có ba tab: Sinh viên, Điểm danh, Xếp hạng. Các bảng đang trống; bấm nút sẽ hiện thông báo chưa cài đặt. Tìm `TODO` trong `Main.cpp` để bắt đầu.

## Mở dự án

- Mở thư mục này bằng Visual Studio Code để đọc và chỉnh sửa `Main.cpp`.
- Để biên dịch giao diện Windows Forms, mở `DA030.sln` bằng Visual Studio 2022 trên Windows; cần bộ công cụ C++ và C++/CLI. Chọn `Debug | x64` rồi chạy.

## Làm lần lượt

1. `themSinhVien`: đọc ba ô nhập và thêm một hàng vào bảng. Sau đó mới bổ sung lớp dữ liệu sinh viên.
2. `luuDiemDanh`: tìm mã sinh viên, lưu trạng thái có mặt/vắng theo ngày.
3. `xemXepHang`: thống nhất cách tính điểm với nhóm, sau đó cài cây AVL và Order-Statistic Tree để có `Rank(x)` và `Select(k)`.
4. Bổ sung lưu/mở file và xuất báo cáo sau khi ba phần trên chạy đúng.

Lưu ý: đây là giao diện thực hành, **chưa phải đồ án hoàn chỉnh**. Giữ `Main.cpp` và `DA030.vcxproj` trong cùng thư mục.
