// DA-030: Khung giao dien de tu phat trien tung chuc nang.
// Visual Studio 2022: C++/CLI + Windows Forms, .NET Framework.
#include <windows.h>
using namespace System;
using namespace System::Drawing;
using namespace System::Windows::Forms;

public ref class MainForm : Form {
private:
    TextBox^ maSinhVien;
    TextBox^ hoTen;
    TextBox^ maLop;
    TextBox^ maDiemDanh;
    DateTimePicker^ ngayHoc;
    CheckBox^ coMat;
    NumericUpDown^ topK;
    DataGridView^ bangSinhVien;
    DataGridView^ bangDiemDanh;
    DataGridView^ bangXepHang;

    Label^ taoNhan(Control^ cha, String^ chu, int x, int y, int rong) {
        Label^ n = gcnew Label();
        n->Text = chu; n->Location = Point(x, y); n->Size = Drawing::Size(rong, 28);
        cha->Controls->Add(n); return n;
    }
    TextBox^ taoO(Control^ cha, int x, int y, int rong) {
        TextBox^ o = gcnew TextBox();
        o->Location = Point(x, y); o->Width = rong;
        cha->Controls->Add(o); return o;
    }
    Button^ taoNut(Control^ cha, String^ chu, int x, int y, int rong, EventHandler^ xuLy) {
        Button^ nut = gcnew Button();
        nut->Text = chu; nut->Location = Point(x, y); nut->Size = Drawing::Size(rong, 32);
        nut->Click += xuLy; cha->Controls->Add(nut); return nut;
    }
    DataGridView^ taoBang(Control^ cha, array<String^>^ cot, int y) {
        DataGridView^ bang = gcnew DataGridView();
        bang->Location = Point(18, y); bang->Size = Drawing::Size(925, 390);
        bang->Anchor = AnchorStyles::Top | AnchorStyles::Bottom | AnchorStyles::Left | AnchorStyles::Right;
        bang->ReadOnly = true; bang->AllowUserToAddRows = false;
        bang->RowHeadersVisible = false;
        bang->AutoSizeColumnsMode = DataGridViewAutoSizeColumnsMode::Fill;
        for each (String^ tenCot in cot) bang->Columns->Add(tenCot, tenCot);
        cha->Controls->Add(bang); return bang;
    }

    void themSinhVien(Object^, EventArgs^) {
        // TODO 1: Kiem tra maSinhVien, hoTen, maLop.
        // TODO 2: Luu sinh vien vao cau truc du lieu cua nhom.
        // TODO 3: Cap nhat bangSinhVien.
        MessageBox::Show("Chua cai dat chuc nang them sinh vien.");
    }
    void luuDiemDanh(Object^, EventArgs^) {
        // TODO 1: Tim sinh vien theo maDiemDanh->Text.
        // TODO 2: Luu ngayHoc->Value va coMat->Checked.
        // TODO 3: Cap nhat diem chuyen can va bangDiemDanh.
        MessageBox::Show("Chua cai dat chuc nang diem danh.");
    }
    void xemXepHang(Object^, EventArgs^) {
        // TODO 1: Tinh diem tong ket theo quy tac nhom thong nhat.
        // TODO 2: Dung Order-Statistic Tree de lay Top K.
        // TODO 3: Dien ket qua vao bangXepHang.
        MessageBox::Show("Chua cai dat chuc nang xep hang.");
    }

public:
    MainForm() {
        Text = "DA-030 - Diem danh va xep hang hoc tap";
        Size = Drawing::Size(1000, 660);
        MinimumSize = Drawing::Size(850, 560);
        StartPosition = FormStartPosition::CenterScreen;
        Font = gcnew Drawing::Font("Segoe UI", 10);

        TabControl^ tabs = gcnew TabControl();
        tabs->Dock = DockStyle::Fill;
        Controls->Add(tabs);

        TabPage^ trangSinhVien = gcnew TabPage("Sinh vien");
        TabPage^ trangDiemDanh = gcnew TabPage("Diem danh");
        TabPage^ trangXepHang = gcnew TabPage("Xep hang");
        tabs->TabPages->Add(trangSinhVien);
        tabs->TabPages->Add(trangDiemDanh);
        tabs->TabPages->Add(trangXepHang);

        taoNhan(trangSinhVien, "Ma SV", 18, 22, 60);
        maSinhVien = taoO(trangSinhVien, 85, 20, 130);
        taoNhan(trangSinhVien, "Ho ten", 232, 22, 68);
        hoTen = taoO(trangSinhVien, 304, 20, 210);
        taoNhan(trangSinhVien, "Ma lop", 530, 22, 65);
        maLop = taoO(trangSinhVien, 598, 20, 125);
        taoNut(trangSinhVien, "Them SV", 742, 18, 120,
            gcnew EventHandler(this, &MainForm::themSinhVien));
        bangSinhVien = taoBang(trangSinhVien,
            gcnew array<String^>{"Ma SV", "Ho ten", "Ma lop"}, 70);

        taoNhan(trangDiemDanh, "Ma SV", 18, 22, 60);
        maDiemDanh = taoO(trangDiemDanh, 85, 20, 130);
        taoNhan(trangDiemDanh, "Ngay hoc", 232, 22, 85);
        ngayHoc = gcnew DateTimePicker();
        ngayHoc->Location = Point(320, 20); ngayHoc->Width = 150;
        ngayHoc->Format = DateTimePickerFormat::Short;
        trangDiemDanh->Controls->Add(ngayHoc);
        coMat = gcnew CheckBox(); coMat->Text = "Co mat";
        coMat->Location = Point(490, 20); coMat->Checked = true;
        trangDiemDanh->Controls->Add(coMat);
        taoNut(trangDiemDanh, "Luu diem danh", 630, 18, 150,
            gcnew EventHandler(this, &MainForm::luuDiemDanh));
        bangDiemDanh = taoBang(trangDiemDanh,
            gcnew array<String^>{"Ma SV", "Ngay hoc", "Trang thai"}, 70);

        taoNhan(trangXepHang, "So sinh vien Top K", 18, 22, 160);
        topK = gcnew NumericUpDown();
        topK->Location = Point(184, 20); topK->Minimum = 1; topK->Maximum = 1000;
        trangXepHang->Controls->Add(topK);
        taoNut(trangXepHang, "Xem Top K", 320, 18, 130,
            gcnew EventHandler(this, &MainForm::xemXepHang));
        bangXepHang = taoBang(trangXepHang,
            gcnew array<String^>{"Hang", "Ma SV", "Ho ten", "Diem"}, 70);
    }
};

[STAThreadAttribute]
int main(array<String^>^) {
    Application::EnableVisualStyles();
    Application::SetCompatibleTextRenderingDefault(false);
    Application::Run(gcnew MainForm());
    return 0;
}
