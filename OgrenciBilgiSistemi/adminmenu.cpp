#include "adminmenu.h"
#include "ui_adminmenu.h"
#include "QString"

AdminMenu::AdminMenu(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::AdminMenu)
{
    ui->setupUi(this);
    ui->stackedWidget->setCurrentIndex(0);
    setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
    this->showMaximized();
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("VeriTabani.db");
    if(!db.open())
    {
        qDebug() << "Veritabani acilisinda bir sorunla karsilasildi";
        return;
    }
}

AdminMenu::~AdminMenu()
{
    delete ui;
}

void AdminMenu::init(QString kontrol)
{
    if(kontrol.compare("OgrEkle") == 0)
    {
        ui->cmBoxOgrEkleFakulte->addItem("Seçiniz");
        ui->cmBoxOgrEkleBolum->addItem("Seçiniz");
        QStringList FakulteList = FakulteleriAl();
        foreach(const QString &fakulte_adi,FakulteList)
        {
            if(fakulte_adi.compare("") != 0)
                ui->cmBoxOgrEkleFakulte->addItem(fakulte_adi);
        }
    }
    else if(kontrol.compare("AkdEkle") == 0)
    {
        ui->cmBoxAkdEkleUnvan->addItem("Seçiniz:");
        ui->cmBoxAkdEkleUnvan->addItem("Profesör");
        ui->cmBoxAkdEkleUnvan->addItem("Doçent");
        ui->cmBoxAkdEkleUnvan->addItem("Öğretim Görevlisi");
        ui->cmBoxAkdEkleUnvan->addItem("Araştırma Görevlisi");
    }
    else if(kontrol.compare("AkdDersEkle") == 0)
    {
        ui->cmBoxAkdDersEkleBolum->addItem("Seçiniz");
        ui->cmBoxAkdEkleDers->addItem("Seçiniz");

        QStringList BolumList = BolumleriAl();
        foreach(const QString &bolum_adi,BolumList)
        {
            if(bolum_adi.compare("") != 0)
                ui->cmBoxAkdDersEkleBolum->addItem(bolum_adi);
        }
    }
    else if(kontrol.compare("FakulteEkle") == 0)
    {
        QStringList FakulteList = FakulteleriAl();
        int fakulte_id = FakulteList.size() + 9;
        ui->lnEkleFakulteId->setText(QString::number(fakulte_id));
    }
    else if(kontrol.compare("BolumEkle") == 0)
    {
        ui->cmBoxBolumEkleFakulte->addItem("Seçiniz");
        QStringList FakulteList = FakulteleriAl();
        foreach(const QString &fakulte_adi,FakulteList)
        {
            if(fakulte_adi.compare("") != 0)
                ui->cmBoxBolumEkleFakulte->addItem(fakulte_adi);
        }

        QStringList BolumList = BolumleriAl();
        int bolum_id = BolumList.size() + 19;
        ui->lnEkleBolumId->setText(QString::number(bolum_id));
    }
    else if(kontrol.compare("DersEkle") == 0)
    {
        ui->cmBoxDersEkleBolum->addItem("Seçiniz");
        QStringList BolumList = BolumleriAl();
        foreach(const QString &bolum_adi,BolumList)
        {
            if(bolum_adi.compare("") != 0)
                ui->cmBoxDersEkleBolum->addItem(bolum_adi);
        }
    }
}

void AdminMenu::on_btnKapat_clicked()
{
    this->hide();
}

void AdminMenu::on_btnGenislet_clicked()
{
    if(this->isMaximized() == false)
        this->showMaximized();
    else
        this->showNormal();
}

void AdminMenu::on_btnKucult_clicked()
{
    this->showMinimized();
}

QStringList AdminMenu::FakulteleriAl()
{
    QStringList FakulteList;
    QString stringList = "";
    QSqlQuery query;
    query.exec("SELECT fakulte_ad FROM Fakulteler");
    while(query.next())
    {
        QString fakulte_ad = query.value(0).toString();
        stringList += fakulte_ad + ",";
    }
    FakulteList = stringList.split(",");
    return FakulteList;
}

QStringList AdminMenu::BolumleriAl()
{
    QStringList BolumList;
    QString stringList = "";
    QSqlQuery query;
    query.exec("SELECT bolum_ad FROM Bolumler");
    while(query.next())
    {
        QString bolum_ad = query.value(0).toString();
        stringList += bolum_ad + ",";
    }
    BolumList = stringList.split(",");
    return BolumList;
}

void AdminMenu::OgrIdOlustur()
{
    if(ui->cmBoxOgrEkleFakulte->currentText().compare("Seçiniz") == 0 || ui->cmBoxOgrEkleBolum->currentText().compare("Seçiniz") == 0)
    {
        ui->lnOgrEkleOgrNo->clear();
        return;
    }
    QString fakulte_ad = ui->cmBoxOgrEkleFakulte->currentText();
    QString bolum_ad = ui->cmBoxOgrEkleBolum->currentText();
    int ogr_fakulte_id=0,ogr_bolum_id=0,egitim=0,ogr_no=0;
    if(ui->rdLisans->isChecked())
        egitim = 1;
    else if(ui->rdLisansustu->isChecked())
        egitim = 2;
    else if(!ui->rdLisans->isChecked() && !ui->rdLisansustu->isChecked())
        return;
    QSqlQuery query;
    query.prepare("SELECT fakulte_id FROM Fakulteler WHERE fakulte_ad = '"+fakulte_ad+"'");
    query.exec();
    if(query.next())
        ogr_fakulte_id = query.value(0).toInt();

    query.prepare("SELECT bolum_id FROM Bolumler WHERE bolum_ad = '"+bolum_ad+"'");
    query.exec();
    if(query.next())
        ogr_bolum_id = query.value(0).toInt();

    query.prepare("SELECT ogr_no,ogr_fakulte_id,ogr_bolum_id,ogr_egitim FROM OgrBilgi");
    query.exec();
    while(query.next())
    {
        if(query.value(1).toInt() == ogr_fakulte_id && query.value(2).toInt() == ogr_bolum_id && query.value(3).toInt() == egitim)
        {
            ogr_no=query.value(0).toString().right(2).toInt();
        }
    }
    ogr_no++;
    ogr_fakulte_id += 100;
    ogr_bolum_id += 100;
    if(ogr_no < 10)
        ui->lnOgrEkleOgrNo->setText(QString::number(ogr_fakulte_id)+QString::number(ogr_bolum_id)+QString::number(egitim)+"0"+QString::number(ogr_no));
    else
        ui->lnOgrEkleOgrNo->setText(QString::number(ogr_fakulte_id)+QString::number(ogr_bolum_id)+QString::number(egitim)+QString::number(ogr_no));
}

void AdminMenu::AkdKullaniciAdiOlustur()
{
    int akd_kullaniciadi=0;
    QSqlQuery query;

    query.prepare("SELECT * FROM AkdBilgi");
    query.exec();
    while(query.next())
        akd_kullaniciadi++;
    akd_kullaniciadi++;
    if(akd_kullaniciadi<10)
        ui->lnAkdEkleKullaniciAdi->setText("20000"+QString::number(akd_kullaniciadi));
    else if(akd_kullaniciadi<100)
        ui->lnAkdEkleKullaniciAdi->setText("2000"+QString::number(akd_kullaniciadi));
    else
        ui->lnAkdEkleKullaniciAdi->setText("200"+QString::number(akd_kullaniciadi));
}

void AdminMenu::AkdDersAdListele()
{
    if(ui->cmBoxAkdDersEkleBolum->currentText().compare("Seçiniz") == 0)
        return;
    ui->cmBoxAkdEkleDers->clear();
    ui->cmBoxAkdEkleDers->addItem("Seçiniz");
    QString bolum_ad = ui->cmBoxAkdDersEkleBolum->currentText();
    int bolum_id=0,ders_egitim=0,ders_sinif=0,ders_donem=0;
    QSqlQuery query;
    query.prepare("SELECT bolum_id FROM Bolumler WHERE bolum_ad = '"+bolum_ad+"'");
    query.exec();
    if(query.next())
        bolum_id = query.value(0).toInt();

    if(ui->rdAkdDersLisans->isChecked())
        ders_egitim = 1;
    else if(ui->rdAkdDersLisansustu->isChecked())
        ders_egitim = 2;
    else if(!ui->rdAkdDersLisans->isChecked() && !ui->rdAkdDersLisansustu->isChecked())
        return;

    ders_sinif = ui->spBoxAkdDersSinif->text().toInt();

    if(ui->rdAkdDersEkleGuz->isChecked())
        ders_donem = 1;
    else if(ui->rdAkdDersEkleBahar->isChecked())
        ders_donem = 2;
    else if(!ui->rdAkdDersEkleGuz->isChecked() && !ui->rdAkdDersEkleBahar->isChecked())
        return;

    query.prepare("SELECT ders_ad FROM Dersler WHERE ders_bolum_id='"+QString::number(bolum_id)+"' AND ders_egitim='"+QString::number(ders_egitim)+"'"
                  "AND ders_sinif ='"+QString::number(ders_sinif)+"' AND ders_donem='"+QString::number(ders_donem)+"'");
    query.exec();
    while(query.next())
        ui->cmBoxAkdEkleDers->addItem(query.value(0).toString());

}

void AdminMenu::AkdDersIdOlustur()
{
    if(ui->cmBoxAkdEkleDers->currentText().compare("Seçiniz") == 0)
        return;

    QString ders_id;
    QString ders_ad = ui->cmBoxAkdEkleDers->currentText();
    QSqlQuery query;
    query.exec("SELECT ders_id FROM Dersler WHERE ders_ad = '"+ders_ad+"'");
    if(query.next())
        ders_id = query.value(0).toString();

    ui->lnAkdEkleDersId->setText(ders_id);
}

void AdminMenu::DersIdOlustur()
{
    if(ui->cmBoxDersEkleBolum->currentText().compare("Seçiniz") == 0)
    {
        ui->lnEkleDersId->clear();
        return;
    }
    QString bolum_ad = ui->cmBoxDersEkleBolum->currentText();
    int ders_bolum_id=0,ders_sinif=1,ders_id=100,ders_donem=0;
    ders_sinif = ui->spBoxDersEkleSinif->text().toInt();
    if(ui->rdDersEkleGuz->isChecked())
        ders_donem = 1;
    else if(ui->rdDersEkleBahar->isChecked())
        ders_donem = 2;
    else if(!ui->rdDersEkleGuz->isChecked() && !ui->rdDersEkleBahar->isChecked())
        return;
    QSqlQuery query;
    ders_id = ders_id * ders_sinif;
    if(ders_donem == 1)
        ders_id += 1;
    else if(ders_donem == 2)
        ders_id += 2;

    query.prepare("SELECT bolum_id FROM Bolumler WHERE bolum_ad = '"+bolum_ad+"'");
    query.exec();
    if(query.next())
        ders_bolum_id = query.value(0).toInt();

    query.prepare("SELECT ders_bolum_id,ders_id,ders_sinif,ders_donem FROM Dersler");
    query.exec();
    bool kontrol = false;
    while(query.next())
    {
        if(query.value(0).toInt() == ders_bolum_id && query.value(2).toInt() == ders_sinif && query.value(3).toInt() == ders_donem)
            ders_id += 2;
    }
    ui->lnEkleDersId->setText(bolum_ad.left(3).toUpper()+QString::number(ders_id));
}

void AdminMenu::on_btnGeri_clicked()
{
    ui->lblBaslik->setText("Admin Menü");
    ui->stackedWidget->setCurrentIndex(0);
}

void AdminMenu::on_btnOgrEkle_clicked()
{
    ui->lnOgrEkleAd->clear();
    ui->lnOgrEkleSoyad->clear();
    if(ui->rdOgrErkek->isChecked())
    {
        ui->rdOgrErkek->setCheckable(false);
        ui->rdOgrErkek->setCheckable(true);
    }
    else if(ui->rdOgrKadin->isChecked())
    {
        ui->rdOgrKadin->setCheckable(false);
        ui->rdOgrKadin->setCheckable(true);
    }
    ui->cmBoxOgrEkleFakulte->clear();
    ui->cmBoxOgrEkleBolum->clear();
    if(ui->rdLisans->isChecked())
    {
        ui->rdLisans->setCheckable(false);
        ui->rdLisans->setCheckable(true);
    }
    else if(ui->rdLisansustu->isChecked())
    {
        ui->rdLisansustu->setCheckable(false);
        ui->rdLisansustu->setCheckable(true);
    }
    ui->spBoxOgrEkleSinif->setValue(1);
    ui->lnOgrEkleOgrNo->clear();
    ui->lnOgrEkleOgrNo->setEnabled(false);
    init("OgrEkle");
    ui->lblBaslik->setText("Öğrenci Ekle");
    ui->stackedWidget->setCurrentIndex(1);
}

void AdminMenu::on_btnOgrSil_clicked()
{
    ui->lnOgrSilOgrNo->clear();
    ui->lblBaslik->setText("Öğrenci Sil");
    ui->stackedWidget->setCurrentIndex(2);
}

void AdminMenu::on_btnAkdEkle_clicked()
{
    ui->lnAkdEkleAd->clear();
    ui->lnAkdEkleSoyad->clear();
    ui->cmBoxAkdEkleUnvan->clear();
    if(ui->rdAkdErkek->isChecked())
    {
        ui->rdAkdErkek->setCheckable(false);
        ui->rdAkdErkek->setCheckable(true);
    }
    else if(ui->rdAkdKadin->isChecked())
    {
        ui->rdAkdKadin->setCheckable(false);
        ui->rdAkdKadin->setCheckable(true);
    }
    ui->lnAkdEkleKullaniciAdi->clear();
    ui->lnAkdEkleKullaniciAdi->setEnabled(false);
    init("AkdEkle");
    AkdKullaniciAdiOlustur();
    ui->lblBaslik->setText("Akademisyen Ekle");
    ui->stackedWidget->setCurrentIndex(3);
}

void AdminMenu::on_btnAkdDersEkle_clicked()
{
    ui->cmBoxAkdDersEkleBolum->clear();
    if(ui->rdAkdDersLisans->isChecked())
    {
        ui->rdAkdDersLisans->setCheckable(false);
        ui->rdAkdDersLisans->setCheckable(true);
    }
    else if(ui->rdAkdDersLisansustu->isChecked())
    {
        ui->rdAkdDersLisansustu->setCheckable(false);
        ui->rdAkdDersLisansustu->setCheckable(true);
    }
    ui->spBoxAkdDersSinif->setValue(1);
    if(ui->rdAkdDersEkleGuz->isChecked())
    {
        ui->rdAkdDersEkleGuz->setCheckable(false);
        ui->rdAkdDersEkleGuz->setCheckable(true);
    }
    else if(ui->rdAkdDersEkleBahar->isChecked())
    {
        ui->rdAkdDersEkleBahar->setCheckable(false);
        ui->rdAkdDersEkleBahar->setCheckable(true);
    }
    ui->cmBoxAkdEkleDers->clear();
    ui->lnAkdEkleDersId->clear();
    ui->lblBaslik->setText("Akademisyen Ders Ekle");
    init("AkdDersEkle");
    ui->stackedWidget->setCurrentIndex(4);
}

void AdminMenu::on_btnFakulteEkle_clicked()
{
    ui->lnEkleFakulteAd->clear();
    ui->lnEkleFakulteId->setEnabled(false);
    init("FakulteEkle");
    ui->lblBaslik->setText("Fakülte Ekle");
    ui->stackedWidget->setCurrentIndex(5);
}

void AdminMenu::on_btnBolumEkle_clicked()
{
    ui->cmBoxBolumEkleFakulte->clear();
    ui->lnEkleBolumAd->clear();
    ui->lnEkleBolumId->setEnabled(false);
    init("BolumEkle");
    ui->lblBaslik->setText("Bölüm Ekle");
    ui->stackedWidget->setCurrentIndex(6);
}

void AdminMenu::on_btnDersEkle_clicked()
{
    ui->cmBoxDersEkleBolum->clear();
    ui->spBoxDersEkleSinif->setValue(1);
    if(ui->rdDersEkleGuz->isChecked())
    {
        ui->rdDersEkleGuz->setCheckable(false);
        ui->rdDersEkleGuz->setCheckable(true);
    }
    else if(ui->rdDersEkleBahar->isChecked())
    {
        ui->rdDersEkleBahar->setCheckable(false);
        ui->rdDersEkleBahar->setCheckable(true);
    }
    ui->lnEkleDersAd->clear();
    ui->spBoxDersKredi->setValue(0);
    ui->spBoxDersAkts->setValue(0);
    ui->lnEkleDersId->clear();
    ui->lnEkleDersId->setEnabled(false);
    init("DersEkle");
    ui->lblBaslik->setText("Ders Ekle");
    ui->stackedWidget->setCurrentIndex(7);
}

void AdminMenu::on_btnDersSil_clicked()
{
    ui->lnSilDersId->clear();
    ui->lblBaslik->setText("Ders Sil");
    ui->stackedWidget->setCurrentIndex(8);
}

void AdminMenu::on_btnCikis_clicked()
{
    db.close();
    QSqlDatabase::removeDatabase("VeriTabani.db");
    this->hide();
    AnaMenu *anamenu = new AnaMenu();
    anamenu->show();
}

void AdminMenu::on_cmBoxOgrEkleFakulte_currentIndexChanged()
{
    if(ui->cmBoxOgrEkleFakulte->currentText().compare("Seçiniz") == 0)
        return;
    ui->cmBoxOgrEkleBolum->clear();
    ui->cmBoxOgrEkleBolum->addItem("Seçiniz");
    QString fakulte_ad = ui->cmBoxOgrEkleFakulte->currentText();
    int fakulte_id=0;
    QSqlQuery query;
    query.prepare("SELECT fakulte_id FROM Fakulteler WHERE fakulte_ad = '"+fakulte_ad+"'");
    query.exec();
    if(query.next())
        fakulte_id = query.value(0).toInt();

    query.prepare("SELECT bolum_ad FROM Bolumler WHERE bolum_fakulte_id = '"+QString::number(fakulte_id)+"'");
    query.exec();
    while(query.next())
        ui->cmBoxOgrEkleBolum->addItem(query.value(0).toString());
}

void AdminMenu::on_cmBoxOgrEkleBolum_currentIndexChanged()
{
    OgrIdOlustur();
}

void AdminMenu::on_rdLisans_clicked()
{
    OgrIdOlustur();
}

void AdminMenu::on_rdLisansustu_clicked()
{
    OgrIdOlustur();
}

void AdminMenu::on_btnOgrEkleOnayla_clicked()
{
    QString ogr_ad,ogr_soyad,ogr_no,ogr_cinsiyet,fakulte_ad,bolum_ad;
    int ogr_fakulte_id=0,ogr_bolum_id=0,ogr_egitim=0,ogr_sinif;
    ogr_ad = ui->lnOgrEkleAd->text();
    ogr_soyad = ui->lnOgrEkleSoyad->text();
    if(ui->rdOgrErkek->isChecked())
        ogr_cinsiyet = ui->rdOgrErkek->text();
    else if(ui->rdOgrKadin->isChecked())
        ogr_cinsiyet = ui->rdOgrKadin->text();

    if(ui->rdLisans->isChecked())
        ogr_egitim = 1;
    else if(ui->rdLisansustu->isChecked())
        ogr_egitim = 2;
    fakulte_ad = ui->cmBoxOgrEkleFakulte->currentText();
    bolum_ad = ui->cmBoxOgrEkleBolum->currentText();
    ogr_sinif = ui->spBoxOgrEkleSinif->text().toInt();
    ogr_no = ui->lnOgrEkleOgrNo->text();
    if(ogr_ad.isEmpty() || ogr_soyad.isEmpty() || ogr_cinsiyet.isEmpty() || fakulte_ad.compare("Seçiniz") == 0 || bolum_ad.compare("Seçiniz") == 0 || ogr_egitim == 0)
    {
        QMessageBox::information(this,"Hata","Doldurulması Zorunlu Alanlar Doldurulmalı","Tamam");
        return;
    }
    QSqlQuery query;
    query.prepare("SELECT fakulte_id FROM Fakulteler WHERE fakulte_ad = '"+fakulte_ad+"'");
    query.exec();
    if(query.next())
        ogr_fakulte_id = query.value(0).toInt();

    query.prepare("SELECT bolum_id FROM Bolumler WHERE bolum_ad = '"+bolum_ad+"'");
    query.exec();
    if(query.next())
        ogr_bolum_id = query.value(0).toInt();

    query.prepare("INSERT INTO OgrBilgi (ogr_no,ogr_sifre,ogr_ad,ogr_soyad,ogr_cinsiyet,ogr_adres,ogr_postakodu,ogr_sehir,ogr_ailetelNo,ogr_telNo,ogr_eposta,ogr_sosyalmedya,ogr_websayfa,ogr_fakulte_id,ogr_bolum_id,ogr_egitim,ogr_sinif,ogr_toplam_kredi,ogr_toplam_akts,ogr_genel_ort) "
                  "VALUES (:ogr_no, :ogr_sifre, :ogr_ad, :ogr_soyad, :ogr_cinsiyet, :ogr_adres, :ogr_postakodu, :ogr_sehir, :ogr_ailetelNo, :ogr_telNo, :ogr_eposta, :ogr_sosyalmedya, :ogr_websayfa, :ogr_fakulte_id, :ogr_bolum_id, :ogr_egitim, :ogr_sinif, :ogr_toplam_kredi, :ogr_toplam_akts, :ogr_genel_ort)");
    query.bindValue(":ogr_no",ogr_no);
    query.bindValue(":ogr_sifre","");
    query.bindValue(":ogr_ad",ogr_ad);
    query.bindValue(":ogr_soyad",ogr_soyad);
    query.bindValue(":ogr_cinsiyet",ogr_cinsiyet);
    query.bindValue(":ogr_adres","");
    query.bindValue(":ogr_postakodu","");
    query.bindValue(":ogr_sehir","");
    query.bindValue(":ogr_ailetelNo","");
    query.bindValue(":ogr_telNo","");
    query.bindValue(":ogr_eposta","");
    query.bindValue(":ogr_sosyalmedya","");
    query.bindValue(":ogr_websayfa","");
    query.bindValue(":ogr_fakulte_id",ogr_fakulte_id);
    query.bindValue(":ogr_bolum_id",ogr_bolum_id);
    query.bindValue(":ogr_egitim",ogr_egitim);
    query.bindValue(":ogr_sinif",ogr_sinif);
    query.bindValue(":ogr_toplam_kredi",0);
    query.bindValue(":ogr_toplam_akts",0);
    query.bindValue(":ogr_genel_ort",0.0);
    query.exec();
    QMessageBox::information(this,"","Öğrenci Basarili Bir Sekilde Sisteme Eklendi","Tamam");
    ui->stackedWidget->setCurrentIndex(0);
}

void AdminMenu::on_btnOgrSilOnayla_clicked()
{
    QString silinecekno = ui->lnOgrSilOgrNo->text();
    if(silinecekno.isEmpty())
    {
        QMessageBox::information(this,"Hata","Doldurulması Zorunlu Alanlar Doldurulmalı","Tamam");
        return;
    }
    QSqlQuery query;
    query.prepare("SELECT * from OgrBilgi WHERE ogr_no = '"+silinecekno+"'");
    query.exec();
    if(!query.next())
    {
        QMessageBox::information(this,"Hata","Bu Numaraya Ait Öğrenci Bulunamadı","Tamam");
        return;
    }

    query.prepare("DELETE from OgrBilgi where ogr_no= '"+silinecekno+"'");
    query.exec();
    QMessageBox::information(this,"","Öğrenci Basarili Bir Sekilde Sistemden Silindi","Tamam");
    ui->stackedWidget->setCurrentIndex(0);
}

void AdminMenu::on_btnAkdEkleOnayla_clicked()
{
    QString akd_ad,akd_soyad,akd_kullaniciadi,akd_unvan,akd_cinsiyet;
    akd_ad = ui->lnAkdEkleAd->text();
    akd_soyad = ui->lnAkdEkleSoyad->text();
    if(ui->rdAkdErkek->isChecked())
        akd_cinsiyet = ui->rdAkdErkek->text();
    else if(ui->rdAkdKadin->isChecked())
        akd_cinsiyet = ui->rdAkdKadin->text();

    akd_unvan = ui->cmBoxAkdEkleUnvan->currentText();
    akd_kullaniciadi = ui->lnAkdEkleKullaniciAdi->text();
    if(akd_unvan.compare("Seçiniz") == 0 || akd_ad.isEmpty() || akd_soyad.isEmpty() || akd_cinsiyet.isEmpty())
    {
        QMessageBox::information(this,"Hata","Doldurulması Zorunlu Alanlar Doldurulmalı","Tamam");
        return;
    }
    QSqlQuery query;
    query.prepare("INSERT INTO AkdBilgi (akd_kullaniciadi,akd_sifre,akd_ad,akd_soyad,akd_cinsiyet,akd_eposta,akd_telNo,akd_adres,akd_unvan)"
                  "VALUES (:akd_kullaniciadi, :akd_sifre, :akd_ad, :akd_soyad, :akd_cinsiyet, :akd_eposta, :akd_telNo, :akd_adres, :akd_unvan)");
    query.bindValue(":akd_kullaniciadi",akd_kullaniciadi);
    query.bindValue(":akd_sifre","");
    query.bindValue(":akd_ad",akd_ad);
    query.bindValue(":akd_soyad",akd_soyad);
    query.bindValue(":akd_cinsiyet",akd_cinsiyet);
    query.bindValue(":akd_eposta","");
    query.bindValue(":akd_telNo","");
    query.bindValue(":akd_adres","");
    query.bindValue(":akd_unvan",akd_unvan);
    query.exec();
    QMessageBox::information(this,"","Akademisyen Basarili Bir Sekilde Sisteme Eklendi","Tamam");
    ui->stackedWidget->setCurrentIndex(0);
}

void AdminMenu::on_cmBoxAkdDersEkleBolum_currentIndexChanged()
{
    AkdDersAdListele();
}

void AdminMenu::on_rdAkdDersLisans_clicked()
{
    AkdDersAdListele();
}

void AdminMenu::on_rdAkdDersLisansustu_clicked()
{
    AkdDersAdListele();
}

void AdminMenu::on_spBoxAkdDersSinif_textChanged()
{
    AkdDersAdListele();
}

void AdminMenu::on_rdAkdDersEkleGuz_clicked()
{
    AkdDersAdListele();
}

void AdminMenu::on_rdAkdDersEkleBahar_clicked()
{
    AkdDersAdListele();
}

void AdminMenu::on_cmBoxAkdEkleDers_currentIndexChanged()
{
    AkdDersIdOlustur();
}

void AdminMenu::on_btnAkdDersEkleOnayla_clicked()
{
    QString akd_kullaniciadi,bolum_ad,ders_ad,ders_id;
    int bolum_id=0,ders_egitim=0,ders_sinif=0,ders_donem=0,ders_kredi=0,ders_akts=0;
    akd_kullaniciadi = ui->lnAkdEkleId->text();
    ders_ad = ui->cmBoxAkdEkleDers->currentText();
    ders_id = ui->lnAkdEkleDersId->text();
    bolum_ad = ui->cmBoxAkdDersEkleBolum->currentText();
    if(ui->rdAkdDersLisans->isChecked())
        ders_egitim = 1;
    else if(ui->rdAkdDersLisansustu->isChecked())
        ders_egitim = 2;

    ders_sinif = ui->spBoxAkdDersSinif->text().toInt();
    if(ui->rdAkdDersEkleGuz->isChecked())
        ders_donem = 1;
    else if(ui->rdAkdDersEkleBahar->isChecked())
        ders_donem = 2;
    if(akd_kullaniciadi.isEmpty() || bolum_ad.compare("Seçiniz") == 0 || ders_ad.compare("Seçiniz") == 0)
    {
        QMessageBox::information(this,"Hata","Doldurulması Zorunlu Alanlar Doldurulmalı","Tamam");
        return;
    }
    QSqlQuery query;
    query.prepare("SELECT * FROM AkdBilgi WHERE akd_kullaniciadi='"+akd_kullaniciadi+"'");
    query.exec();
    if(!query.next())
    {
        QMessageBox::information(this, "Hata", "Bu Kullanici Adina Sahip Akademisyen Bulunamadi");
        return;
    }

    query.prepare("SELECT * FROM AkdDers WHERE ders_id = '"+ders_id+"'");
    query.exec();
    if(query.next())
    {
        QMessageBox::information(this, "Hata", "Bu Ders Bu Akademisyene Zaten Kayitli");
        return;
    }

    query.prepare("SELECT ders_kredi,ders_akts FROM Dersler WHERE ders_id = '"+ders_id+"'");
    query.exec();
    if(query.next())
    {
        ders_kredi = query.value(0).toInt();
        ders_akts = query.value(1).toInt();
    }

    query.prepare("SELECT bolum_id FROM Bolumler WHERE bolum_ad = '"+bolum_ad+"'");
    query.exec();
    if(query.next())
        bolum_id = query.value(0).toInt();

    query.prepare("INSERT INTO AkdDers (akd_kullaniciadi,bolum_id,ders_egitim,ders_sinif,ders_donem,ders_id,ders_ad,ders_kredi,ders_akts)"
                  "VALUES (:akd_kullaniciadi, :bolum_id, :ders_egitim, :ders_sinif, :ders_donem, :ders_id, :ders_ad, :ders_kredi, :ders_akts)");
    query.bindValue(":akd_kullaniciadi",akd_kullaniciadi);
    query.bindValue(":bolum_id",bolum_id);
    query.bindValue(":ders_egitim",ders_egitim);
    query.bindValue(":ders_sinif",ders_sinif);
    query.bindValue(":ders_donem",ders_donem);
    query.bindValue(":ders_id",ders_id);
    query.bindValue(":ders_ad",ders_ad);
    query.bindValue(":ders_kredi",ders_kredi);
    query.bindValue(":ders_akts",ders_akts);

    query.exec();
    QMessageBox::information(this,"","Ders Basarili Bir Sekilde Akademisyene Eklendi","Tamam");
    ui->stackedWidget->setCurrentIndex(0);
}

void AdminMenu::on_btnFakulteEkleOnayla_clicked()
{
    QString fakulte_ad;
    int fakulte_id;
    fakulte_ad = ui->lnEkleFakulteAd->text();
    fakulte_id = ui->lnEkleFakulteId->text().toInt();
    if(fakulte_ad.isEmpty())
    {
        QMessageBox::information(this,"Hata","Doldurulması Zorunlu Alanlar Doldurulmalı","Tamam");
        return;
    }
    QSqlQuery query;
    query.prepare("SELECT * FROM Fakulteler WHERE fakulte_ad = '"+fakulte_ad+"'");
    query.exec();
    if(query.next())
    {
        QMessageBox::information(this, "Hata", "Bu Fakulte Adi Sistemde Zaten Kayitli");
        return;
    }

    query.prepare("INSERT INTO Fakulteler (fakulte_id,fakulte_ad) "
                  "VALUES (:fakulte_id, :fakulte_ad)");
    query.bindValue(":fakulte_id",fakulte_id);
    query.bindValue(":fakulte_ad",fakulte_ad);
    query.exec();
    QMessageBox::information(this,"","Fakulte Basarili Bir Sekilde Sisteme Eklendi","Tamam");
    ui->stackedWidget->setCurrentIndex(0);
}

void AdminMenu::on_btnBolumEkleOnayla_clicked()
{
    QString bolum_ad;
    int bolum_fakulte_id,bolum_id;
    bolum_fakulte_id = ui->cmBoxBolumEkleFakulte->currentIndex()+9;
    bolum_id = ui->lnEkleBolumId->text().toInt();
    bolum_ad = ui->lnEkleBolumAd->text();
    if(ui->cmBoxBolumEkleFakulte->currentText().compare("Seçiniz") == 0 || bolum_ad.isEmpty())
    {
        QMessageBox::information(this,"Hata","Doldurulması Zorunlu Alanlar Doldurulmalı","Tamam");
        return;
    }
    QSqlQuery query;
    query.prepare("SELECT * FROM Bolumler WHERE bolum_ad = '"+bolum_ad+"'");
    query.exec();
    if(query.next())
    {
        QMessageBox::information(this, "Hata", "Bu Bolum Adi Sistemde Zaten Kayitli");
        return;
    }

    query.prepare("INSERT INTO Bolumler (bolum_fakulte_id,bolum_id,bolum_ad) "
                  "VALUES (:bolum_fakulte_id,:bolum_id, :bolum_ad)");
    query.bindValue(":bolum_fakulte_id",bolum_fakulte_id);
    query.bindValue(":bolum_id",bolum_id);
    query.bindValue(":bolum_ad",bolum_ad);
    query.exec();
    QMessageBox::information(this,"","Bolum Basarili Bir Sekilde Sisteme Eklendi","Tamam");
    ui->stackedWidget->setCurrentIndex(0);
}

void AdminMenu::on_cmBoxDersEkleBolum_currentTextChanged()
{
    DersIdOlustur();
}

void AdminMenu::on_spBoxDersEkleSinif_textChanged()
{
    DersIdOlustur();
}

void AdminMenu::on_rdDersEkleGuz_clicked()
{
    DersIdOlustur();
}


void AdminMenu::on_rdDersEkleBahar_clicked()
{
    DersIdOlustur();
}

void AdminMenu::on_btnDersEkleOnayla_clicked()
{
    QString bolum_ad,ders_ad,ders_id;
    int ders_bolum_id=0,ders_egitim=0,ders_sinif=1,ders_donem=0,ders_kredi=0,ders_akts=0;
    ders_sinif = ui->spBoxDersEkleSinif->text().toInt();
    if(ui->rdDersEkleLisans->isChecked())
        ders_egitim = 1;
    else if(ui->rdDersEkleLisansustu->isChecked())
        ders_egitim = 2;

    if(ui->rdDersEkleGuz->isChecked())
        ders_donem = 1;
    else if(ui->rdDersEkleBahar->isChecked())
        ders_donem = 2;
    bolum_ad = ui->cmBoxDersEkleBolum->currentText();
    ders_ad = ui->lnEkleDersAd->text();
    ders_kredi = ui->spBoxDersKredi->text().toInt();
    ders_akts = ui->spBoxDersAkts->text().toInt();
    ders_id = ui->lnEkleDersId->text();
    if(bolum_ad.compare("Seçiniz") == 0 || ders_egitim == 0 || ders_donem == 0 || ders_ad.isEmpty() || ders_kredi == 0 || ders_akts == 0)
    {
        QMessageBox::information(this,"Hata","Doldurulması Zorunlu Alanlar Doldurulmalı","Tamam");
        return;
    }
    QSqlQuery query;
    query.prepare("SELECT bolum_id FROM Bolumler WHERE bolum_ad = '"+bolum_ad+"'");
    query.exec();
    if(query.next())
        ders_bolum_id = query.value(0).toInt();

    query.prepare("INSERT INTO Dersler (ders_bolum_id,ders_id,ders_egitim,ders_sinif,ders_donem,ders_ad,ders_kredi,ders_akts) "
                  "VALUES (:ders_bolum_id, :ders_id, :ders_egitim, :ders_sinif, :ders_donem, :ders_ad, :ders_kredi, :ders_akts)");
    query.bindValue(":ders_bolum_id",ders_bolum_id);
    query.bindValue(":ders_id",ders_id);
    query.bindValue(":ders_egitim",ders_egitim);
    query.bindValue(":ders_sinif",ders_sinif);
    query.bindValue(":ders_donem",ders_donem);
    query.bindValue(":ders_ad",ders_ad);
    query.bindValue(":ders_kredi",ders_kredi);
    query.bindValue(":ders_akts",ders_akts);

    query.exec();

    QMessageBox::information(this,"","Ders Basarili Bir Sekilde Sisteme Eklendi","Tamam");
    ui->stackedWidget->setCurrentIndex(0);
}

void AdminMenu::on_btnDersSilOnayla_clicked()
{
    QString silinecekid = ui->lnSilDersId->text();
    if(silinecekid.isEmpty())
    {
        QMessageBox::information(this,"Hata","Doldurulması Zorunlu Alanlar Doldurulmalı","Tamam");
        return;
    }
    QSqlQuery query;
    query.prepare("SELECT * from Dersler WHERE ders_id = '"+silinecekid+"'");
    query.exec();
    if(!query.next())
    {
        QMessageBox::information(this,"Hata","Bu Ders Id'sine Sahip Ders Bulunamadı","Tamam");
        return;
    }
    query.prepare("DELETE from Dersler where ders_id= '"+silinecekid+"'");
    query.exec();
    QMessageBox::information(this,"","Seçilen Ders Basarili Bir Sekilde Sistemden Silindi","Tamam");
    ui->stackedWidget->setCurrentIndex(0);
}
