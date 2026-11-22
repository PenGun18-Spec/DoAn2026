#include "cautruc.h"

// ===========================================================================
// 1. DANH SACH DAC
// ===========================================================================

// Ban dau danh sach rong
void KhoiTaoDanhSachDac(DanhSachDac* d) {
    d->soLuong = 0;
}

// Them mot sinh vien vao cuoi mang
int ThemDac(DanhSachDac* d, const SinhVien* sv) {
    if (d->soLuong >= MAX_SINH_VIEN) {
        return 0;                       // mang da day
    }

    d->ds[d->soLuong] = *sv;
    d->soLuong++;
    return 1;
}

// Tim sinh vien theo ma, tra ve chi so trong mang; khong co thi tra -1
int TimDac(DanhSachDac* d, const wchar_t* ma) {
    for (int i = 0; i < d->soLuong; i++) {
        if (wcscmp(d->ds[i].ma, ma) == 0) {
            return i;
        }
    }
    return -1;
}

// Xoa sinh vien theo ma: don cac phan tu phia sau len mot vi tri
int XoaDac(DanhSachDac* d, const wchar_t* ma) {
    int viTri = TimDac(d, ma);
    if (viTri == -1) {
        return 0;
    }

    for (int i = viTri; i < d->soLuong - 1; i++) {
        d->ds[i] = d->ds[i + 1];
    }
    d->soLuong--;
    return 1;
}

// ===========================================================================
// 2. DANH SACH LIEN KET
// ===========================================================================

// Ban dau danh sach rong
void KhoiTaoDanhSachLienKet(DanhSachLienKet* d) {
    d->dau = NULL;
    d->soLuong = 0;
}

// Them mot sinh vien vao cuoi danh sach
int ThemLienKet(DanhSachLienKet* d, const SinhVien* sv) {
    NutLienKet* moi = new NutLienKet;
    moi->sv = *sv;
    moi->next = NULL;

    if (d->dau == NULL) {
        // Danh sach dang rong
        d->dau = moi;
    } else {
        // Di den nut cuoi roi noi them
        NutLienKet* cuoi = d->dau;
        while (cuoi->next != NULL) {
            cuoi = cuoi->next;
        }
        cuoi->next = moi;
    }

    d->soLuong++;
    return 1;
}

// Tim nut chua sinh vien co ma can tim
NutLienKet* TimLienKet(DanhSachLienKet* d, const wchar_t* ma) {
    NutLienKet* p = d->dau;
    while (p != NULL) {
        if (wcscmp(p->sv.ma, ma) == 0) {
            return p;
        }
        p = p->next;
    }
    return NULL;
}

// Xoa nut chua ma can tim
int XoaLienKet(DanhSachLienKet* d, const wchar_t* ma) {
    NutLienKet* truoc = NULL;
    NutLienKet* p = d->dau;

    while (p != NULL) {
        if (wcscmp(p->sv.ma, ma) == 0) {
            if (truoc == NULL) {
                d->dau = p->next;       // xoa nut dau
            } else {
                truoc->next = p->next;  // noi tat qua nut p
            }
            delete p;
            d->soLuong--;
            return 1;
        }
        truoc = p;
        p = p->next;
    }
    return 0;
}

// Giai phong toan bo nut
void XoaToanBoLienKet(DanhSachLienKet* d) {
    NutLienKet* p = d->dau;
    while (p != NULL) {
        NutLienKet* sau = p->next;
        delete p;
        p = sau;
    }
    d->dau = NULL;
    d->soLuong = 0;
}

// ===========================================================================
// 3. CAY AVL va ORDER-STATISTIC TREE
// ===========================================================================

// Ban dau cay rong
void KhoiTaoCay(CayAVL* c) {
    c->goc = NULL;
    c->soLuong = 0;
}

// Do cao cua mot nut, nut rong co do cao 0
int ChieuCao(NutAVL* n) {
    if (n == NULL) {
        return 0;
    }
    return n->chieuCao;
}

// So nut cua cay con, nut rong co size 0
int LaySize(NutAVL* n) {
    if (n == NULL) {
        return 0;
    }
    return n->size;
}

// Tinh lai do cao va size sau khi cay con thay doi
void CapNhat(NutAVL* n) {
    if (n == NULL) {
        return;
    }

    int caoTrai = ChieuCao(n->trai);
    int caoPhai = ChieuCao(n->phai);

    if (caoTrai > caoPhai) {
        n->chieuCao = caoTrai + 1;
    } else {
        n->chieuCao = caoPhai + 1;
    }

    n->size = LaySize(n->trai) + LaySize(n->phai) + 1;
}

// Xoay trai: dua nut ben phai len thay vi tri nut n
NutAVL* XoayTrai(NutAVL* n) {
    NutAVL* moi = n->phai;

    n->phai = moi->trai;
    moi->trai = n;

    CapNhat(n);
    CapNhat(moi);
    return moi;
}

// Xoay phai: dua nut ben trai len thay vi tri nut n
NutAVL* XoayPhai(NutAVL* n) {
    NutAVL* moi = n->trai;

    n->trai = moi->phai;
    moi->phai = n;

    CapNhat(n);
    CapNhat(moi);
    return moi;
}

// So sanh hai sinh vien de biet ai dung truoc trong cay
// Tra ve -1: a dung truoc b (diem cao hon)
// Tra ve  1: a dung sau b
// Tra ve  0: bang nhau
int SoSanh(const SinhVien* a, const SinhVien* b) {
    if (a->diem > b->diem) {
        return -1;
    }
    if (a->diem < b->diem) {
        return 1;
    }
    // Bang diem thi so theo ma cho thu tu ro rang
    return wcscmp(a->ma, b->ma);
}

// Kiem tra nut p co bi lech khong, neu lech thi xoay lai cho can bang
NutAVL* CanBang(NutAVL* p) {
    int lech = ChieuCao(p->trai) - ChieuCao(p->phai);

    // Ben trai cao hon ben phai qua 1
    if (lech > 1) {
        // Truong hop lech trai-phai: xoay trai nut con truoc
        if (ChieuCao(p->trai->trai) < ChieuCao(p->trai->phai)) {
            p->trai = XoayTrai(p->trai);
        }
        return XoayPhai(p);
    }

    // Ben phai cao hon ben trai qua 1
    if (lech < -1) {
        // Truong hop lech phai-trai: xoay phai nut con truoc
        if (ChieuCao(p->phai->phai) < ChieuCao(p->phai->trai)) {
            p->phai = XoayPhai(p->phai);
        }
        return XoayTrai(p);
    }

    return p;                           // da can bang, khong can xoay
}

// Chen sinh vien vao cay con co goc la p, tra ve goc moi
NutAVL* ChenNut(NutAVL* p, const SinhVien* sv) {
    if (p == NULL) {
        // Tao nut moi
        NutAVL* moi = new NutAVL;
        moi->sv = *sv;
        moi->chieuCao = 1;
        moi->size = 1;
        moi->trai = NULL;
        moi->phai = NULL;
        return moi;
    }

    int soSanh = SoSanh(sv, &p->sv);

    if (soSanh < 0) {
        p->trai = ChenNut(p->trai, sv);
    } else if (soSanh > 0) {
        p->phai = ChenNut(p->phai, sv);
    } else {
        return p;                       // trung ma sinh vien, khong chen
    }

    CapNhat(p);
    return CanBang(p);
}

// Chen sinh vien vao cay, tra ve 1 neu chen duoc
int ThemCay(CayAVL* c, const SinhVien* sv) {
    if (TimCay(c, sv->ma) == 1) {
        return 0;                       // da co sinh vien nay
    }

    c->goc = ChenNut(c->goc, sv);
    c->soLuong++;
    return 1;
}

// Tim nut co chua ma sinh vien (duyet het cay vi cay sap theo diem, khong theo ma)
NutAVL* TimNut(NutAVL* p, const wchar_t* ma) {
    if (p == NULL) {
        return NULL;
    }

    if (wcscmp(p->sv.ma, ma) == 0) {
        return p;
    }

    NutAVL* benTrai = TimNut(p->trai, ma);
    if (benTrai != NULL) {
        return benTrai;
    }

    return TimNut(p->phai, ma);
}

// Kiem tra cay co sinh vien mang ma nay khong
int TimCay(CayAVL* c, const wchar_t* ma) {
    if (TimNut(c->goc, ma) != NULL) {
        return 1;
    }
    return 0;
}

// Tim nut nho nhat cua mot cay con
NutAVL* NutNhoNhat(NutAVL* p) {
    while (p->trai != NULL) {
        p = p->trai;
    }
    return p;
}

// Xoa nut chua sinh vien sv khoi cay con goc p, tra ve goc moi
// Phai so sanh theo cung quy tac voi luc chen (theo diem, bang diem thi theo ma)
NutAVL* XoaNut(NutAVL* p, const SinhVien* sv, int* daXoa) {
    if (p == NULL) {
        return NULL;
    }

    int soSanh = SoSanh(sv, &p->sv);

    if (soSanh < 0) {
        p->trai = XoaNut(p->trai, sv, daXoa);
    } else if (soSanh > 0) {
        p->phai = XoaNut(p->phai, sv, daXoa);
    } else {
        // Tim thay nut can xoa
        if (p->trai == NULL || p->phai == NULL) {
            // Co nhieu nhat mot nut con
            NutAVL* con;
            if (p->trai != NULL) {
                con = p->trai;
            } else {
                con = p->phai;
            }
            delete p;
            *daXoa = 1;
            return con;
        }

        // Co hai nut con: lay nut nho nhat ben phai thay vao
        NutAVL* thay = NutNhoNhat(p->phai);
        p->sv = thay->sv;
        p->phai = XoaNut(p->phai, &thay->sv, daXoa);
    }

    CapNhat(p);
    return CanBang(p);
}

// Xoa sinh vien theo ma, tra ve 1 neu xoa duoc
int XoaCay(CayAVL* c, const wchar_t* ma) {
    // Tim nut truoc de biet diem cua sinh vien can xoa
    NutAVL* nut = TimNut(c->goc, ma);
    if (nut == NULL) {
        return 0;                       // khong co sinh vien nay
    }

    SinhVien canXoa = nut->sv;

    int daXoa = 0;
    c->goc = XoaNut(c->goc, &canXoa, &daXoa);

    if (daXoa == 1) {
        c->soLuong--;
    }
    return daXoa;
}

// Cap nhat lai mot sinh vien trong cay khi diem thay doi.
// Cay sap theo diem nen khong the sua truc tiep trong nut duoc,
// phai xoa nut cu ra roi chen lai voi diem moi.
// Tra ve 1 neu cap nhat duoc.
int CapNhatCay(CayAVL* c, const SinhVien* sv) {
    if (XoaCay(c, sv->ma) == 0) {
        return 0;                       // khong tim thay sinh vien
    }
    return ThemCay(c, sv);
}

// Giai phong toan bo nut cua cay con
void XoaHetNut(NutAVL* p) {
    if (p == NULL) {
        return;
    }

    XoaHetNut(p->trai);
    XoaHetNut(p->phai);
    delete p;
}

// Xoa sach cay
void XoaToanBoCay(CayAVL* c) {
    XoaHetNut(c->goc);
    c->goc = NULL;
    c->soLuong = 0;
}

// ===========================================================================
// ORDER-STATISTIC TREE
// ===========================================================================

// Tim nguoi thu k, tinh tu diem cao xuong thap
NutAVL* Select(CayAVL* c, int k) {
    if (k < 1 || k > c->soLuong) {
        return NULL;
    }

    NutAVL* p = c->goc;

    while (p != NULL) {
        // Vi tri cua nut p = so nut ben trai + 1
        int viTri = LaySize(p->trai) + 1;

        if (k == viTri) {
            return p;
        }

        if (k < viTri) {
            // Nam ben trai
            p = p->trai;
        } else {
            // Nam ben phai, phai tru di phan ben trai va nut p
            k = k - viTri;
            p = p->phai;
        }
    }

    return NULL;
}

// Tim hang cua mot muc diem: dem xem co bao nhieu nguoi dung truoc no
// Hang 1 la diem cao nhat
int Rank(CayAVL* c, double diem) {
    int hang = 0;
    NutAVL* p = c->goc;

    while (p != NULL) {
        if (diem > p->sv.diem) {
            // Diem can tim cao hon nut nay: chi can xet ben trai
            p = p->trai;
        } else if (diem < p->sv.diem) {
            // Diem can tim thap hon: ca cay con trai va nut nay dung truoc no
            hang = hang + LaySize(p->trai) + 1;
            p = p->phai;
        } else {
            // Tim thay diem: cong them phan ben trai
            hang = hang + LaySize(p->trai) + 1;
            return hang;
        }
    }

    return 0;                           // khong co diem nay trong cay
}

// ===========================================================================
// 4. LICH SU DIEM DANH
// ===========================================================================

// Ban dau chua co ban ghi nao
void KhoiTaoLichSu(LichSuDiemDanh* l) {
    l->soLuong = 0;
}

// Kiem tra mot sinh vien da duoc diem danh trong ngay do chua
// Tra ve 1 neu da co, 0 neu chua
int DaDiemDanh(LichSuDiemDanh* l, const wchar_t* ma, int ngay, int thang, int nam) {
    for (int i = 0; i < l->soLuong; i++) {
        BanGhiDiemDanh* b = &l->ds[i];

        int cungMa = (wcscmp(b->ma, ma) == 0);
        int cungNgay = (b->ngay == ngay && b->thang == thang && b->nam == nam);

        if (cungMa && cungNgay) {
            return 1;
        }
    }
    return 0;
}

// Them mot ban ghi diem danh, tra ve 1 neu them duoc
int ThemBanGhi(LichSuDiemDanh* l, const wchar_t* ma, int ngay, int thang, int nam, int coMat) {
    if (l->soLuong >= MAX_DIEM_DANH) {
        return 0;                       // lich su da day
    }

    BanGhiDiemDanh* b = &l->ds[l->soLuong];

    wcscpy_s(b->ma, ma);
    b->ngay = ngay;
    b->thang = thang;
    b->nam = nam;
    b->coMat = coMat;

    l->soLuong++;
    return 1;
}

// Dem tong so buoi hoc da diem danh cua ca lop
// Mot buoi = mot cap (ngay, thang, nam) xuat hien trong lich su
// Day la mau so dung de tinh ti le chuyen can
int DemTongBuoiCuaLop(LichSuDiemDanh* l) {
    int dem = 0;

    for (int i = 0; i < l->soLuong; i++) {
        BanGhiDiemDanh* b = &l->ds[i];

        // Kiem tra buoi nay da duoc dem chua (cac ban ghi cung ngay nam truoc)
        int daDem = 0;
        for (int j = 0; j < i; j++) {
            BanGhiDiemDanh* truoc = &l->ds[j];
            if (truoc->ngay == b->ngay && truoc->thang == b->thang && truoc->nam == b->nam) {
                daDem = 1;
                break;
            }
        }

        if (daDem == 0) {
            dem++;
        }
    }

    return dem;
}

// Dem so buoi co mat cua mot sinh vien
int DemBuoiCoMatCuaSV(LichSuDiemDanh* l, const wchar_t* ma) {
    int dem = 0;

    for (int i = 0; i < l->soLuong; i++) {
        BanGhiDiemDanh* b = &l->ds[i];
        if (wcscmp(b->ma, ma) == 0 && b->coMat == 1) {
            dem++;
        }
    }

    return dem;
}

// Xoa tat ca ban ghi diem danh cua mot sinh vien
// Duyet mang, giu lai cac ban ghi cua nguoi khac, tra ve so ban ghi da xoa
int XoaBanGhiCuaSV(LichSuDiemDanh* l, const wchar_t* ma) {
    int daXoa = 0;
    int moi = 0;

    for (int i = 0; i < l->soLuong; i++) {
        if (wcscmp(l->ds[i].ma, ma) == 0) {
            daXoa++;                    // ban ghi cua sinh vien can xoa -> bo qua
        } else {
            l->ds[moi] = l->ds[i];      // ban ghi cua nguoi khac -> giu lai
            moi++;
        }
    }

    l->soLuong = moi;
    return daXoa;
}
