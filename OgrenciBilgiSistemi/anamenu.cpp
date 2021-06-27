#include "anamenu.h"
#include "ui_anamenu.h"

AnaMenu::AnaMenu(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::AnaMenu)
{
    ui->setupUi(this);
    init();
}

AnaMenu::~AnaMenu()
{
    delete ui;
}

void AnaMenu::init()
{
    setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("VeriTabani.db");

    if(!db.open())
    {
        qDebug() << "Veritabani acilisinda bir sorunla karsilasildi";
        return;
    }

    ui->stackedWidget->setCurrentIndex(0);
    kontrol = 0;
    DatabaseOlustur();
}

void AnaMenu::on_btnKapat_clicked()
{
    this->hide();
}

void AnaMenu::on_btnGenislet_clicked()
{
    if(this->isMaximized() == false)
        this->showMaximized();
    else
        this->showNormal();
}

void AnaMenu::on_btnKucult_clicked()
{
    this->showMinimized();
}

void AnaMenu::DatabaseOlustur()
{
    QSqlQuery query;

    query.exec("CREATE TABLE IF NOT EXISTS Fakulteler ("
                       "fakulte_id INTEGER PRIMARY KEY,"
                       "fakulte_ad VARCHAR(50))");

    query.exec("CREATE TABLE IF NOT EXISTS Bolumler ("
                       "bolum_fakulte_id INTEGER,"
                       "bolum_id INTEGER PRIMARY KEY,"
                       "bolum_ad VARCHAR(50))");

    query.exec("CREATE TABLE IF NOT EXISTS Dersler ("
                       "ders_bolum_id INTEGER,"
                       "ders_id VARCHAR(10) PRIMARY KEY,"
                       "ders_egitim INTEGER,"
                       "ders_sinif INTEGER,"
                       "ders_donem INTEGER,"
                       "ders_ad VARCHAR(30),"
                       "ders_kredi INTEGER,"
                       "ders_akts INTEGER)");

    query.exec("CREATE TABLE IF NOT EXISTS OgrBilgi ("
                       "ogr_no VARCHAR(20) PRIMARY KEY,"
                       "ogr_sifre VARCHAR(50),"
                       "ogr_ad VARCHAR(30),"
                       "ogr_soyad VARCHAR(20),"
                       "ogr_cinsiyet VARCHAR(5),"
                       "ogr_adres VARCHAR(50),"
                       "ogr_postakodu VARCHAR(10),"
                       "ogr_sehir VARCHAR(50),"
                       "ogr_ailetelNo VARCHAR(15),"
                       "ogr_telNo VARCHAR(15),"
                       "ogr_eposta VARCHAR(30),"
                       "ogr_sosyalmedya VARCHAR(20),"
                       "ogr_websayfa VARCHAR(50),"
                       "ogr_fakulte_id INTEGER,"
                       "ogr_bolum_id INTEGER,"
                       "ogr_egitim INTEGER,"
                       "ogr_sinif INTEGER,"
                       "ogr_toplam_kredi INTEGER,"
                       "ogr_toplam_akts INTEGER,"
                       "ogr_genel_ort REAL)");

    query.exec("CREATE TABLE IF NOT EXISTS OgrDers ("
                       "ogr_no VARCHAR(20),"
                       "ogr_sinif INTEGER,"
                       "ogr_donem INTEGER,"
                       "ders_id VARCHAR(10),"
                       "ders_ad VARCHAR(20),"
                       "vize_not INTEGER,"
                       "final_not INTEGER,"
                       "ortalama INTEGER,"
                       "ders_gecmedurum INTEGER,"
                       "harf_notu VARCHAR(2))");

    query.exec("CREATE TABLE IF NOT EXISTS OgrOrtalama ("
                       "ogr_no VARCHAR(20) PRIMARY KEY,"
                       "ogr_sinif INTEGER,"
                       "ogr_donem INTEGER,"
                       "ogr_donem_ort REAL)");

    query.exec("CREATE TABLE IF NOT EXISTS AkdBilgi ("
                       "akd_kullaniciadi VARCHAR(20) PRIMARY KEY,"
                       "akd_sifre VARCHAR(50),"
                       "akd_ad VARCHAR(30),"
                       "akd_soyad VARCHAR(20),"
                       "akd_cinsiyet VARCHAR(5),"
                       "akd_eposta VARCHAR(30),"
                       "akd_telNo VARCHAR(20),"
                       "akd_adres VARCHAR(50),"
                       "akd_unvan VARCHAR(20))");

    query.exec("CREATE TABLE IF NOT EXISTS AkdDers ("
                       "akd_kullaniciadi VARCHAR(20),"
                       "bolum_id INTEGER,"
                       "ders_egitim INTEGER,"
                       "ders_sinif INTEGER,"
                       "ders_donem INTEGER,"
                       "ders_id VARCHAR(10) PRIMARY KEY,"
                       "ders_ad VARCHAR(30),"
                       "ders_kredi INTEGER,"
                       "ders_akts INTEGER)");

}

bool AnaMenu::DatabaseAc(QString kullanici_adi, QString kullanici_sifre)
{
    QSqlQuery query;

    if(kontrol == 1)
        query.exec("SELECT ogr_no,ogr_sifre FROM OgrBilgi");
    else if(kontrol == 2)
        query.exec("SELECT akd_kullaniciadi,akd_sifre FROM AkdBilgi");

    while(query.next())
    {
        QString kullaniciadi = query.value(0).toString();
        QString kullanicisifre = query.value(1).toString();
        if(kullanici_adi.compare(kullaniciadi)==0 && kullanici_sifre.compare(kullanicisifre)==0)
            return true;
        else if(kullanici_adi.compare(kullaniciadi)==0 && kullanicisifre.compare("")==0)
            return true;
    }
    return false;
}

void AnaMenu::RastgeleSayiUret()
{
    sayi1 = 10 + rand()%99;
    sayi2 = 1 + rand()%10;
    toplam = sayi1 + sayi2;
    ui->lblSayi_1->setText(QString::number(sayi1)+"  +");
    ui->lblSayi_2->setText(QString::number(sayi2)+"  =  ?");
}

void AnaMenu::on_btnOgrGiris_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
    RastgeleSayiUret();
    ui->label->setText("Öğrenci No:");
    kontrol = 1;
}

void AnaMenu::on_btnAkdGiris_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
    RastgeleSayiUret();
    ui->label->setText("Kullanıcı Adı:");
    kontrol = 2;
}

void AnaMenu::on_btnAdmin_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}

void AnaMenu::on_btnAdminGiris_clicked()
{
    if(ui->lnAdminAd->text().compare("admin")==0 && ui->lnAdminSifre->text().compare("1234")==0)
    {
        this->hide();
        AdminMenu *adminmenu = new AdminMenu();
        adminmenu->show();
    }
    else
        ui->statusbar->showMessage("Kullanici adi veya sifre hatali");
}

void AnaMenu::on_btnGiris_clicked()
{
    QString kullanici_adi,kullanici_sifre;
    kullanici_adi = ui->lnKullaniciAdi->text();
    kullanici_sifre = ui->lnSifre->text();

    if(ui->lnKullaniciAdi->text().isEmpty())
    {
        if(kontrol == 1)
            ui->statusbar->showMessage("Öğrenci Numarası Girilmedi");
        else if(kontrol == 2)
            ui->statusbar->showMessage("Kullanici Adi Girilmedi");
    }
    else if(ui->lnSifre->text().isEmpty())
        ui->statusbar->showMessage("Şifre Girilmedi");
    else if(ui->lnCevap->text().isEmpty())
        ui->statusbar->showMessage("Güvenlik Kodu Girilmedi");
    else
    {
        if(ui->lnCevap->text().toInt() == toplam)
        {
            if(kontrol == 1)
            {
                if(DatabaseAc(kullanici_adi,kullanici_sifre))
                {
                    db.close();
                    QSqlDatabase::removeDatabase("VeriTabani.db");
                    hide();
                    OgrMenu *ogrenciMenu = new OgrMenu(nullptr,kullanici_adi);
                    ogrenciMenu->show();
                }
                else
                {
                    ui->statusbar->showMessage("Öğrenci Numarası veya Şifre hatali");
                }
            }
            else if(kontrol == 2)
            {
                if(DatabaseAc(kullanici_adi,kullanici_sifre))
                {
                    db.close();
                    QSqlDatabase::removeDatabase("VeriTabani.db");
                    hide();
                    AkdMenu *akademisyenMenu = new AkdMenu(nullptr,kullanici_adi);
                    akademisyenMenu->show();
                }
                else
                {
                    ui->statusbar->showMessage("Kullanici Adi veya Şifre hatali");
                }
            }
        }
        else
        {
            ui->statusbar->showMessage("Güvenlik Kodu Hatalı");
        }
    }
}

void AnaMenu::on_btnAnaMenu_2_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void AnaMenu::on_btnKayitMenu_clicked()
{
    ui->stackedWidget->setCurrentIndex(3);
    if(kontrol == 1)
        ui->lblKayit->setText("Öğrenci Numarası:");
    else if(kontrol == 2)
        ui->lblKayit->setText("Kullancı Adı:");
}

void AnaMenu::on_btnKayit_clicked()
{
    QString kullanici_adi = ui->lnKayitNo->text();
    if(kontrol == 1)
    {
        if(DatabaseAc(kullanici_adi,""))
            ui->stackedWidget->setCurrentIndex(4);
        else
            ui->statusbar->showMessage("Sistemde bu numara ile kayıtlı öğrenci bulunamadı");
    }
    else if(kontrol == 2)
    {
        if(DatabaseAc(kullanici_adi,""))
            ui->stackedWidget->setCurrentIndex(4);
        else
            ui->statusbar->showMessage("Sistemde bu numara ile kayıtlı akademisyen bulunamadı");
    }
}

void AnaMenu::on_btnOnayla_clicked()
{
    if(ui->lnKayitSifre->text().isEmpty())
        ui->statusbar->showMessage("Bu alan boş bırakılamaz");
    else
    {
        QString kullanici_adi = ui->lnKayitNo->text();
        QString kullanici_sifre = ui->lnKayitSifre->text();

        QSqlQuery query;
        if(kontrol == 1)
            query.exec("UPDATE OgrBilgi set ogr_sifre='"+kullanici_sifre+"' WHERE ogr_no='"+kullanici_adi+"'");
        else if(kontrol == 2)
            query.exec("UPDATE AkdBilgi set akd_sifre='"+kullanici_sifre+"' WHERE akd_kullaniciadi='"+kullanici_adi+"'");
    }

    ui->stackedWidget->setCurrentIndex(2);
}
