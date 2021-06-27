#include "akdmenu.h"
#include "ui_akdmenu.h"

AkdMenu::AkdMenu(QWidget *parent,QString kullanici_adi) :
    QMainWindow(parent),
    ui(new Ui::AkdMenu)
{
    ui->setupUi(this);
    this->kullanici_adi = kullanici_adi;
    init();
}

AkdMenu::~AkdMenu()
{
    delete ui;
}

void AkdMenu::init()
{
    this->showMaximized();
    setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
    ui->stackedWidget->setCurrentWidget(0);
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("VeriTabani.db");
    if(!db.open())
    {
        qDebug() << "Veritabani acilisinda bir sorunla karsilasildi";
        return;
    }

    QSqlQuery query;
    query.prepare("SELECT akd_ad,akd_soyad,akd_unvan FROM AkdBilgi WHERE akd_kullaniciadi='"+kullanici_adi+"'");
    query.exec();
    if(query.next())
        ui->lblUstBilgi->setText(query.value(2).toString() + " " +query.value(0).toString()+" "+query.value(1).toString());
}

void AkdMenu::on_btnKapat_clicked()
{
    this->hide();
}

void AkdMenu::on_btnGenislet_clicked()
{
    if(this->isMaximized() == false)
        this->showMaximized();
    else
        this->showNormal();
}

void AkdMenu::on_btnKucult_clicked()
{
    this->showMinimized();
}

QStringList AkdMenu::BolumleriAl()
{
    QStringList BolumList;
    QString stringList = "";
    QSqlQuery query;
    query.prepare("SELECT bolum_ad FROM Bolumler");
    query.exec();
    while(query.next())
    {
        QString bolum_ad = query.value(0).toString();
        stringList += bolum_ad + ",";
    }
    BolumList = stringList.split(",");
    return BolumList;
}

void AkdMenu::on_btnVerilenDersler_clicked()
{
    QSqlQuery query;
    int bolum_id=0;
    QStringList BolumList = BolumleriAl();
    ui->cmBoxVerilenDerslerBolum->addItem("Seçiniz");
    ui->cmBoxVerilenDerslerDonem->addItem("Seçiniz");
    foreach(const QString &bolum_adi,BolumList)
    {
        if(bolum_adi.compare("") != 0)
        {
            query.prepare("SELECT bolum_id FROM Bolumler WHERE bolum_ad='"+bolum_adi+"'");
            query.exec();
            if(query.next())
                bolum_id = query.value(0).toInt();

            query.prepare("SELECT * FROM AkdDers WHERE akd_kullaniciadi = '"+kullanici_adi+"' AND bolum_id = '"+QString::number(bolum_id)+"'");
            query.exec();
            if(query.next())
                ui->cmBoxVerilenDerslerBolum->addItem(bolum_adi);
        }
    }
    ui->stackedWidget->setCurrentIndex(1);
}

void AkdMenu::on_btnNotGiris_clicked()
{
    ui->cmBoxNotGirisDersId->clear();
    ui->lblNotGirisBolum->clear();
    ui->tableNotGiris->clearFocus();
    ui->cmBoxNotGirisDersId->addItem("Seçiniz");
    QSqlQuery query;
    query.prepare("SELECT ders_id FROM AkdDers WHERE akd_kullaniciadi = '"+kullanici_adi+"'");
    query.exec();
    while(query.next())
        ui->cmBoxNotGirisDersId->addItem(query.value(0).toString());

    ui->stackedWidget->setCurrentIndex(2);
}

void AkdMenu::on_cmBoxVerilenDerslerBolum_currentIndexChanged()
{
    if(ui->cmBoxVerilenDerslerBolum->currentText().compare("Seçiniz") == 0)
        return;
    ui->cmBoxVerilenDerslerDonem->clear();
    ui->cmBoxVerilenDerslerDonem->addItem("Seçiniz");
    QString bolum_ad = ui->cmBoxVerilenDerslerBolum->currentText();
    int bolum_id=0,ders_sinif=0,ders_donem=0,temp1=0,temp2=0;
    QSqlQuery query;
    query.prepare("SELECT bolum_id FROM Bolumler WHERE bolum_ad = '"+bolum_ad+"'");
    query.exec();
    if(query.next())
        bolum_id = query.value(0).toInt();

    query.exec("SELECT ders_sinif,ders_donem FROM AkdDers WHERE akd_kullaniciadi='"+kullanici_adi+"' AND bolum_id = '"+QString::number(bolum_id)+"'");
    while(query.next())
    {
        ders_sinif = query.value(0).toInt();
        ders_donem = query.value(1).toInt();

        if(ders_sinif != temp1 || ders_donem != temp2)
        {
            if(ders_donem == 1)
                ui->cmBoxVerilenDerslerDonem->addItem(QString::number(ders_sinif)+".Sınıf Güz Dönemi");
            else if(ders_donem == 2)
                ui->cmBoxVerilenDerslerDonem->addItem(QString::number(ders_sinif)+".Sınıf Bahar Dönemi");
        }

        temp1 = ders_sinif;
        temp2 = ders_donem;
    }

}

void AkdMenu::on_cmBoxVerilenDerslerDonem_currentIndexChanged()
{
    if(ui->cmBoxVerilenDerslerDonem->currentText().compare("Seçiniz") == 0)
        return;

    int ders_sinif=0,ders_donem=0,ders_bolum_id=0;
    QString bolum_ad;
    ders_sinif = ui->cmBoxVerilenDerslerDonem->currentText().left(1).toInt();
    if(ui->cmBoxVerilenDerslerDonem->currentIndex() % 2 == 1)
        ders_donem = 1;
    else if(ui->cmBoxVerilenDerslerDonem->currentIndex() % 2 == 0)
        ders_donem = 2;

    bolum_ad = ui->cmBoxVerilenDerslerBolum->currentText();

    QSqlQuery query;
    query.prepare("SELECT bolum_id FROM Bolumler WHERE bolum_ad = '"+bolum_ad+"'");
    query.exec();
    if(query.next())
        ders_bolum_id = query.value(0).toInt();

    QSqlQueryModel *model = new QSqlQueryModel();

    query.exec("SELECT ders_id,ders_sinif,ders_ad,ders_kredi,ders_akts FROM AkdDers WHERE akd_kullaniciadi ='"+kullanici_adi+"'"
               "AND bolum_id='"+QString::number(ders_bolum_id)+"' AND ders_sinif='"+QString::number(ders_sinif)+"' AND ders_donem='"+QString::number(ders_donem)+"'");

    model->setQuery(query);
    model->setHeaderData(0,Qt::Horizontal,QObject::tr("Ders Kodu"));
    model->setHeaderData(1,Qt::Horizontal,QObject::tr("Sınıf"));
    model->setHeaderData(2,Qt::Horizontal,QObject::tr("Ders Adı"));
    model->setHeaderData(3,Qt::Horizontal,QObject::tr("Kredi"));
    model->setHeaderData(4,Qt::Horizontal,QObject::tr("Akts"));

    ui->tableVerilenDersler->setModel(model);
}

void AkdMenu::on_cmBoxNotGirisDersId_currentIndexChanged()
{
    if(ui->cmBoxNotGirisDersId->currentText().compare("Seçiniz") == 0)
        return;
    QString ders_id = ui->cmBoxNotGirisDersId->currentText();
    QSqlQuery query;
    query.prepare("SELECT ders_bolum_id FROM Dersler WHERE ders_id = '"+ders_id+"'");
    query.exec();
    if(query.next())
        ui->lblNotGirisBolum->setText(query.value(0).toString());

    QSqlQueryModel *model = new QSqlQueryModel();

    query.exec("SELECT ogr_no,vize_not,final_not FROM OgrDers WHERE ders_id ='"+ders_id+"'");

    model->setQuery(query);
    model->setHeaderData(0,Qt::Horizontal,QObject::tr("Öğrenci Numarası"));
    model->setHeaderData(1,Qt::Horizontal,QObject::tr("Vize Not"));
    model->setHeaderData(2,Qt::Horizontal,QObject::tr("Final Not"));

    ui->tableNotGiris->setModel(model);

    for(int p=0; p<model->rowCount(); p++)
    {
        QWidget *lnVizeWidget = new QWidget(this);
        lnVizeWidget->setStyleSheet("QWidget{background-color: rgb(9,5,13)}");
        QLineEdit *lnVize = new QLineEdit(this);
        lnVize->setStyleSheet("QWidget{background-color: rgb(255,255,255)}");
        lnVize->setAlignment(Qt::AlignCenter);
        QHBoxLayout *lnVizeLayout = new QHBoxLayout(lnVizeWidget);
        lnVizeLayout->addWidget(lnVize);
        lnVizeLayout->setContentsMargins(5,5,5,5);
        ui->tableNotGiris->setIndexWidget(model->index(p,1),lnVizeWidget);
        model->setData(model->index(p,1),Qt::Unchecked,Qt::CheckStateRole);

        QWidget *lnFinalWidget = new QWidget(this);
        lnFinalWidget->setStyleSheet("QWidget{background-color: rgb(9,5,13)}");
        QLineEdit *lnFinal = new QLineEdit(this);
        lnFinal->setStyleSheet("QWidget{background-color: rgb(255,255,255)}");
        lnFinal->setAlignment(Qt::AlignCenter);
        QHBoxLayout *lnFinalLayout = new QHBoxLayout(lnFinalWidget);
        lnFinalLayout->addWidget(lnFinal);
        lnFinalLayout->setContentsMargins(5,5,5,5);
        ui->tableNotGiris->setIndexWidget(model->index(p,2),lnFinalWidget);
        model->setData(model->index(p,2),Qt::Unchecked,Qt::CheckStateRole);

        query.prepare("SELECT vize_not,final_not FROM OgrDers WHERE ders_id ='"+ders_id+"' AND ogr_no = '"+model->index(p,0).data().toString()+"'");
        query.exec();
        if(query.next())
        {
            if(query.value(0).toInt() == -1 && query.value(1).toInt() == -1)
            {
                lnFinal->setReadOnly(true);
            }
            else if(query.value(0).toInt() != -1 && query.value(1).toInt() != -1)
            {
                lnVize->setReadOnly(true);
                lnVize->setText(query.value(0).toString());

                lnFinal->setReadOnly(true);
                lnFinal->setText(query.value(1).toString());
            }
            else
            {
                lnVize->setReadOnly(true);
                lnVize->setText(query.value(0).toString());
            }
        }
    }
}

void AkdMenu::on_btnNotGirisOnayla_clicked()
{
    QString ogr_no,ders_id,harf_notu;
    int vize_not=-1,final_not=-1,gecme_durum=-1,ortalama=-1;
    QAbstractItemModel *model = ui->tableNotGiris->model();
    QSqlQuery query;
    ders_id = ui->cmBoxNotGirisDersId->currentText();
    for(int p=0;p<model->rowCount();p++)
    {
        ogr_no = model->index(p,0).data().toString();

        QWidget *vizeWidget =ui->tableNotGiris->indexWidget(model->index(p,1));
        QLineEdit *vizeLineEdit = qobject_cast<QLineEdit*>(vizeWidget->focusWidget());

        QWidget *finalWidget =ui->tableNotGiris->indexWidget(model->index(p,2));
        QLineEdit *finalLineEdit = qobject_cast<QLineEdit*>(finalWidget->focusWidget());

        if(vizeLineEdit != nullptr && vizeLineEdit->isReadOnly()==false)
            vize_not = vizeLineEdit->text().toInt();
        else if(vizeLineEdit != nullptr && vizeLineEdit->isReadOnly())
            vize_not = vizeLineEdit->text().toInt();

        if(finalLineEdit != nullptr && finalLineEdit->isReadOnly() == false)
        {
            final_not = finalLineEdit->text().toInt();
            ortalama = vize_not*0.4 + final_not*0.6;
            if(90<ortalama)
            {
                gecme_durum = 1;
                harf_notu = "AA";
            }
            else if(85<ortalama)
            {
                gecme_durum = 1;
                harf_notu = "BA";
            }
            else if(75<ortalama)
            {
                gecme_durum = 1;
                harf_notu = "BB";
            }
            else if(65<ortalama)
            {
                gecme_durum = 1;
                harf_notu = "CB";
            }
            else if(60<ortalama)
            {
                gecme_durum = 1;
                harf_notu = "CC";
            }
            else if(ortalama<60)
            {
                gecme_durum = 0;
                harf_notu = "FF";
            }
        }

        query.prepare("UPDATE OgrDers set vize_not='"+QString::number(vize_not)+"' ,final_not='"+QString::number(final_not)+"' ,"
                      "ortalama='"+QString::number(ortalama)+"' ,ders_gecmedurum='"+QString::number(gecme_durum)+"' ,harf_notu='"+harf_notu+"'"
                      "WHERE ogr_no='"+ogr_no+"' AND ders_id='"+ders_id+"'");

        query.exec();

    }
    QMessageBox::information(this,"","Not Giris İslemi Basariyla Gerceklesti","Tamam");
}

void AkdMenu::on_btnCikis_clicked()
{
    db.close();
    QSqlDatabase::removeDatabase("VeriTabani.db");
    this->hide();
    AnaMenu *anamenu = new AnaMenu();
    anamenu->show();
}
