#include "ogrmenu.h"
#include "ui_ogrmenu.h"
#include "anamenu.h"

OgrMenu::OgrMenu(QWidget *parent,QString ogr_no) :
    QMainWindow(parent),
    ui(new Ui::OgrMenu)
{
    ui->setupUi(this);
    this->ogr_no = ogr_no;
    init();
}

OgrMenu::~OgrMenu()
{
    delete ui;
}

void OgrMenu::init()
{
    this->showMaximized();
    setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
    ui->stackedWidget->setCurrentIndex(0);
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("VeriTabani.db");
    if(!db.open())
    {
        qDebug() << "Veritabani acilisinda bir sorunla karsilasildi";
        return;
    }

    QSqlQuery query;
    query.prepare("SELECT ogr_ad,ogr_soyad FROM OgrBilgi WHERE ogr_no='"+ogr_no+"'");
    query.exec();
    if(query.next())
        ui->lblUstBilgi->setText(ogr_no + "-" +query.value(0).toString()+" "+query.value(1).toString());
}

void OgrMenu::on_btnKapat_clicked()
{
    this->hide();
}

void OgrMenu::on_btnGenislet_clicked()
{
    if(this->isMaximized() == false)
        this->showMaximized();
    else
        this->showNormal();
}

void OgrMenu::on_btnKucult_clicked()
{
    this->showMinimized();
}

void OgrMenu::on_btnOzluk_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
    QString ogr_ad,ogr_soyad,ogr_adres,ogr_postakodu,ogr_ailetelNo,ogr_telNo,ogr_eposta,ogr_sosyalmedya,ogr_websayfa;
    QStringList ogr_sehir;
    int ogr_fakulte_id=0,ogr_bolum_id=0,ogr_sinif=0;
    QString fakulte_ad,bolum_ad;

    QSqlQuery query;
    query.exec("SELECT ogr_ad,ogr_soyad,ogr_adres,ogr_postakodu,ogr_sehir,ogr_ailetelNo,ogr_telNo,ogr_eposta,ogr_sosyalmedya,"
                       "ogr_websayfa,ogr_fakulte_id,ogr_bolum_id,ogr_sinif FROM OgrBilgi WHERE ogr_no = '"+ogr_no+"'");

    if(query.next())
    {
        ogr_ad = query.value(0).toString();
        ogr_soyad = query.value(1).toString();
        ogr_adres = query.value(2).toString();
        ogr_postakodu = query.value(3).toString();
        ogr_sehir = query.value(4).toString().split(",");
        ogr_ailetelNo = query.value(5).toString();
        ogr_telNo = query.value(6).toString();
        ogr_eposta = query.value(7).toString();
        ogr_sosyalmedya = query.value(8).toString();
        ogr_websayfa = query.value(9).toString();
        ogr_fakulte_id = query.value(10).toInt();
        ogr_bolum_id = query.value(11).toInt();
        ogr_sinif = query.value(12).toInt();
    }

    ui->lnOgrNo->setText(ogr_no);
    ui->lnAdSoyad->setText(ogr_ad + " " + ogr_soyad);
    ui->lnSinif->setText(QString::number(ogr_sinif));
    ui->lnAdres->setText(ogr_adres);
    ui->lnPostaKodu->setText(ogr_postakodu);
    if(ogr_sehir.size() == 2)
    {
        ui->lnSehir->setText(ogr_sehir.at(0));
        ui->lnIlce->setText(ogr_sehir.at(1));
    }
    ui->lnAileTelNo->setText(ogr_ailetelNo);
    ui->lnOgrTelNo->setText(ogr_telNo);
    ui->lnEposta->setText(ogr_eposta);
    ui->lnSosyalMedya->setText(ogr_sosyalmedya);
    ui->lnWeb->setText(ogr_websayfa);

    query.exec("SELECT fakulte_ad FROM Fakulteler WHERE fakulte_id = '"+QString::number(ogr_fakulte_id)+"' ");
    if(query.next())
        fakulte_ad = query.value(0).toString();

    query.exec("SELECT bolum_ad FROM Bolumler WHERE bolum_id = '"+QString::number(ogr_bolum_id)+"' ");
    if(query.next())
        bolum_ad = query.value(0).toString();

    ui->lnFakulte->setText(fakulte_ad);
    ui->lnBolum->setText(bolum_ad);
    ui->lnProgram->setText(bolum_ad);
}

void OgrMenu::on_btnOzlukKaydet_clicked()
{
    QString ogr_adres,ogr_postakodu,ogr_sehir,ogr_ailetelNo,ogr_telNo,ogr_eposta,ogr_sosyalmedya,ogr_websayfa;
    QString sehirad,ilce;
    ogr_adres = ui->lnAdres->text();
    ogr_postakodu = ui->lnPostaKodu->text();
    sehirad= ui->lnSehir->text();
    ilce = ui->lnIlce->text();
    ogr_sehir = sehirad+","+ilce;
    ogr_ailetelNo = ui->lnAileTelNo->text();
    ogr_telNo = ui->lnOgrTelNo->text();
    ogr_eposta = ui->lnEposta->text();
    ogr_sosyalmedya = ui->lnSosyalMedya->text();
    ogr_websayfa = ui->lnWeb->text();

    QSqlQuery query;
    query.exec("UPDATE OgrBilgi set ogr_adres = '"+ogr_adres+"', "
                                "ogr_postakodu = '"+ogr_postakodu+"', "
                                "ogr_sehir = '"+ogr_sehir+"', "
                                "ogr_ailetelNo = '"+ogr_ailetelNo+"', "
                                "ogr_telNo = '"+ogr_telNo+"', "
                                "ogr_eposta = '"+ogr_eposta+"', "
                                "ogr_sosyalmedya = '"+ogr_sosyalmedya+"', "
                                "ogr_websayfa = '"+ogr_websayfa+"' "
                                "WHERE ogr_no = '"+ogr_no+"' ");
}

void OgrMenu::on_btnDersKayit_clicked()
{
    ui->stackedWidget->setCurrentIndex(4);
    int ogr_sinif=1,donem=1,ders_bolum_id=0;
    int toplam_ders=0,toplam_kredi=0,toplam_akts=0;
    QSqlQuery query;
    query.exec("SELECT ogr_bolum_id,ogr_sinif FROM OgrBilgi WHERE ogr_no = '"+ogr_no+"'");
    if(query.next())
    {
        ders_bolum_id = query.value(0).toInt();
        ogr_sinif = query.value(1).toInt();
    }
    QSqlQueryModel *model = new QSqlQueryModel();
    query.exec("SELECT ders_id,ders_ad,ders_sinif,ders_kredi,ders_akts FROM Dersler "
               "WHERE ders_bolum_id ='"+QString::number(ders_bolum_id)+"' AND ders_sinif = '"+QString::number(ogr_sinif)+"' AND ders_donem='"+QString::number(donem)+"' ");

    model->setQuery(query);
    model->setHeaderData(0,Qt::Horizontal,QObject::tr("Ders Kodu"));
    model->setHeaderData(1,Qt::Horizontal,QObject::tr("Ders Adı"));
    model->setHeaderData(2,Qt::Horizontal,QObject::tr("Sınıf"));
    model->setHeaderData(3,Qt::Horizontal,QObject::tr("Kredi"));
    model->setHeaderData(4,Qt::Horizontal,QObject::tr("Akts"));

    query.exec("SELECT ogr_sinif,ogr_donem FROM OgrDers WHERE ogr_no = '"+ogr_no+"'");
    while(query.next())
    {
        if(query.value(0).toInt() == ogr_sinif && query.value(1).toInt() == donem)
        {
            ui->tableDersKayit->setModel(model);
            ui->lblDersKayitDurum->setText("Öğrenci Kesinleştirme Durumu: Kesinleştirdi");
            QMessageBox::information(this,"","Bu Döneme Ait Ders Kaydi İslemi Bulunmamaktadir","Tamam");
            return;
        }
    }

    model->insertColumn(5);
    model->setHeaderData(5,Qt::Horizontal,QObject::tr(""));
    ui->tableDersKayit->setModel(model);

    ui->lblDersKayitDurum->setText("Öğrenci Kesinleştirme Durumu: Kesinleştirmedi");
    for(int p=0; p<model->rowCount(); p++)
    {
        toplam_ders++;
        toplam_kredi += model->index(p,3).data().toInt();
        toplam_akts += model->index(p,4).data().toInt();
        QWidget *checkBoxWidget = new QWidget(this);
        checkBoxWidget->setStyleSheet("QWidget{background-color: rgb(9,5,13)}");
        QCheckBox *checkBox = new QCheckBox(this);
        QHBoxLayout *checkBoxLayout = new QHBoxLayout(checkBoxWidget);
        checkBoxLayout->addWidget(checkBox);
        checkBoxLayout->setAlignment(Qt::AlignCenter);
        checkBoxLayout->setContentsMargins(0,0,0,0);
        ui->tableDersKayit->setIndexWidget(model->index(p,5),checkBoxWidget);
        model->setData(model->index(p,5),Qt::Unchecked,Qt::CheckStateRole);
    }

    ui->lblDersKayitKredi->setText("Toplam;  Kredi: "+QString::number(toplam_kredi)+" AKTS: "+QString::number(toplam_akts)+
                                   " Ders Sayısı: "+QString::number(toplam_ders));
}

void OgrMenu::on_btnDersKayitOnayla_clicked()
{
    QString dersler = "",ders_ad;
    QStringList DersList;
    int sayac=0;
    int ogr_sinif=1,ogr_donem=1;
    QAbstractItemModel *model = ui->tableDersKayit->model();
    for(int p=0;p<model->rowCount();p++)
    {
        QWidget *widget =ui->tableDersKayit->indexWidget(model->index(p,5));
        QCheckBox *cb = qobject_cast<QCheckBox*>(widget->focusWidget());
        if(cb != nullptr && cb->isChecked())
        {
            dersler += model->index(p,0).data().toString() + ",";
            sayac++;
        }
    }
    qDebug() << dersler;
    if(sayac != model->rowCount())
    {
        QMessageBox::critical(this,"Hata","Döneme Ait Tüm Derslerin Seçilmesi Gerekmektedir","Tamam");
        return;
    }
    DersList = dersler.split(",");
    QSqlQuery query;
    query.exec("SELECT ogr_sinif FROM OgrBilgi WHERE ogr_no = '"+ogr_no+"'");
    if(query.next())
        ogr_sinif = query.value(0).toInt();

    foreach(const QString &ders_id,DersList)
    {
        if(ders_id.compare("") != 0)
        {
            query.prepare("SELECT ders_ad FROM Dersler WHERE ders_id = '"+ders_id+"'");
            query.exec();
            if(query.next())
                ders_ad = query.value(0).toString();

            qDebug() << ders_ad;

            query.prepare("INSERT INTO OgrDers (ogr_no,ogr_sinif,ogr_donem,ders_id,ders_ad,vize_not,final_not,ortalama,ders_gecmedurum,harf_notu) "
                          "VALUES (:ogr_no, :ogr_sinif, :ogr_donem, :ders_id, :ders_ad, :vize_not, :final_not, :ortalama, :ders_gecmedurum, :harf_notu)");
            query.bindValue(":ogr_no",ogr_no);
            query.bindValue(":ogr_sinif",ogr_sinif);
            query.bindValue(":ogr_donem",ogr_donem);
            query.bindValue(":ders_id",ders_id);
            query.bindValue(":ders_ad",ders_ad);
            query.bindValue(":vize_not",-1);
            query.bindValue(":final_not",-1);
            query.bindValue(":ortalama",-1);
            query.bindValue(":ders_gecmedurum",-1);
            query.bindValue(":harf_notu","");
            query.exec();
        }
    }
    QMessageBox::information(this,"","Ders Secimi Basariyla Gerceklesti","Tamam");
    ui->lblDersKayitDurum->setText("Öğrenci Kesinleştirme Durumu: Kesinleştirdi");
}

void OgrMenu::on_btnAlinanDersler_clicked()
{
    ui->cmBoxAlinanDersler->clear();
    ui->cmBoxAlinanDersler->addItem("Seçiniz");
    int ogr_sinif=0,ogr_donem=0;
    bool kontrol = false;
    QSqlQuery query;
    query.exec("SELECT ogr_sinif,ogr_donem FROM OgrDers WHERE ogr_no='"+ogr_no+"'");
    while(query.next())
    {
        if(ogr_sinif != query.value(0).toInt())
        {
            ogr_sinif = query.value(0).toInt();
            kontrol = true;
        }

        if(ogr_donem != query.value(1).toInt())
        {
            ogr_donem = query.value(1).toInt();
            kontrol = true;
        }

        if(kontrol)
        {
            if(ogr_donem == 1)
                ui->cmBoxAlinanDersler->addItem(QString::number(ogr_sinif)+".Sınıf Güz Dönemi");
            else if(ogr_donem == 2)
                ui->cmBoxAlinanDersler->addItem(QString::number(ogr_sinif)+".Sınıf Bahar Dönemi");
            kontrol = false;
        }
    }

    ui->stackedWidget->setCurrentIndex(2);
}

void OgrMenu::on_cmBoxAlinanDersler_currentIndexChanged()
{
    if(ui->cmBoxAlinanDersler->currentText().compare("Seçiniz") == 0)
        return;

    int ogr_sinif=0,ogr_donem=0,ders_bolum_id=0;
    ogr_sinif = ui->cmBoxAlinanDersler->currentText().left(1).toInt();
    if(ui->cmBoxAlinanDersler->currentIndex() % 2 == 1)
        ogr_donem = 1;
    else if(ui->cmBoxAlinanDersler->currentIndex() % 2 == 0)
        ogr_donem = 2;

    QSqlQuery query;
    query.exec("SELECT ogr_bolum_id FROM OgrBilgi WHERE ogr_no='"+ogr_no+"' ");
    if(query.next())
        ders_bolum_id = query.value(0).toInt();

    QSqlQueryModel *model = new QSqlQueryModel();

    query.exec("SELECT ders_id,ders_sinif,ders_ad,ders_kredi,ders_akts FROM Dersler "
               "WHERE ders_bolum_id='"+QString::number(ders_bolum_id)+"' AND ders_sinif='"+QString::number(ogr_sinif)+"' AND ders_donem='"+QString::number(ogr_donem)+"'");

    model->setQuery(query);
    model->setHeaderData(0,Qt::Horizontal,QObject::tr("Ders Kodu"));
    model->setHeaderData(1,Qt::Horizontal,QObject::tr("Sınıf"));
    model->setHeaderData(2,Qt::Horizontal,QObject::tr("Ders Adı"));
    model->setHeaderData(3,Qt::Horizontal,QObject::tr("Kredi"));
    model->setHeaderData(4,Qt::Horizontal,QObject::tr("Akts"));

    ui->tableAlinanDersler->setModel(model);
}

void OgrMenu::on_btnNotListesi_clicked()
{
    ui->cmBoxNotListesi->clear();
    ui->cmBoxNotListesi->addItem("Seçiniz");
    int ogr_sinif=0,ogr_donem=0;
    bool kontrol = false;
    QSqlQuery query;
    query.exec("SELECT ogr_sinif,ogr_donem FROM OgrDers WHERE ogr_no='"+ogr_no+"'");
    while(query.next())
    {
        if(ogr_sinif != query.value(0).toInt())
        {
            ogr_sinif = query.value(0).toInt();
            kontrol = true;
        }

        if(ogr_donem != query.value(1).toInt())
        {
            ogr_donem = query.value(1).toInt();
            kontrol = true;
        }

        if(kontrol)
        {
            if(ogr_donem == 1)
                ui->cmBoxNotListesi->addItem(QString::number(ogr_sinif)+".Sınıf Güz Dönemi");
            else if(ogr_donem == 2)
                ui->cmBoxNotListesi->addItem(QString::number(ogr_sinif)+".Sınıf Bahar Dönemi");
            kontrol = false;
        }
    }

    ui->stackedWidget->setCurrentIndex(5);
}

void OgrMenu::on_cmBoxNotListesi_currentIndexChanged()
{
    if(ui->cmBoxNotListesi->currentText().compare("Seçiniz") == 0)
        return;
    int ogr_sinif=0,ogr_donem=0;
    ogr_sinif = ui->cmBoxNotListesi->currentText().left(1).toInt();
    if(ui->cmBoxNotListesi->currentIndex() % 2 == 1)
        ogr_donem = 1;
    else if(ui->cmBoxNotListesi->currentIndex() % 2 == 0)
        ogr_donem = 2;

    QSqlQuery query;
    QSqlQueryModel *model = new QSqlQueryModel();

    query.exec("SELECT ders_id,ders_ad,vize_not,final_not,ortalama,harf_notu,ders_gecmedurum FROM OgrDers "
               "WHERE ogr_no='"+ogr_no+"' AND ogr_sinif='"+QString::number(ogr_sinif)+"' AND ogr_donem='"+QString::number(ogr_donem)+"'");

    model->setQuery(query);
    model->setHeaderData(0,Qt::Horizontal,QObject::tr("Ders Kodu"));
    model->setHeaderData(1,Qt::Horizontal,QObject::tr("Ders Adı"));
    model->setHeaderData(2,Qt::Horizontal,QObject::tr("Vize Not"));
    model->setHeaderData(3,Qt::Horizontal,QObject::tr("Final Not"));
    model->setHeaderData(4,Qt::Horizontal,QObject::tr("Ortalama"));
    model->setHeaderData(5,Qt::Horizontal,QObject::tr("Harf Notu"));
    model->setHeaderData(6,Qt::Horizontal,QObject::tr("Durum"));

    ui->tableNotListesi->setModel(model);

    for(int p=0; p<model->rowCount(); p++)
    {
        if(model->index(p,2).data().toString().compare("-1") == 0)
            model->index(p,2).data() = "";

        if(model->index(p,3).data().toString().compare("-1") == 0)
            model->index(p,3).data() = "";

        if(model->index(p,4).data().toString().compare("-1") == 0)
            model->index(p,4).data() = "";

        if(model->index(p,6).data().toString().compare("-1") == 0)
            model->index(p,2).data() = "Sonuçlandırılmadı";
        else if(model->index(p,6).data().toString().compare("1") == 0)
            model->index(p,2).data() = "Geçti";
        else if(model->index(p,6).data().toString().compare("0") == 0)
            model->index(p,2).data() = "Kaldı";
    }
}

void OgrMenu::on_btnCikis_clicked()
{
    db.close();
    QSqlDatabase::removeDatabase("VeriTabani.db");
    this->hide();
    AnaMenu *anamenu = new AnaMenu();
    anamenu->show();
}
