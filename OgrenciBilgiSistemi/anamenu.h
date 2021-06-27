#ifndef ANAMENU_H
#define ANAMENU_H

#include <QMainWindow>
#include <QSql>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include "adminmenu.h"
#include "ogrmenu.h"
#include "akdmenu.h"

QT_BEGIN_NAMESPACE
namespace Ui { class AnaMenu; }
QT_END_NAMESPACE

class AnaMenu : public QMainWindow
{
    Q_OBJECT

public:
    AnaMenu(QWidget *parent = nullptr);
    ~AnaMenu();

    void init();

private slots:
    void on_btnKapat_clicked();

    void on_btnGenislet_clicked();

    void on_btnKucult_clicked();

    void DatabaseOlustur();

    bool DatabaseAc(QString kullanici_adi,QString sifre);

    void RastgeleSayiUret();

    void on_btnOgrGiris_clicked();

    void on_btnAkdGiris_clicked();

    void on_btnAdmin_clicked();

    void on_btnAdminGiris_clicked();

    void on_btnGiris_clicked();

    void on_btnAnaMenu_2_clicked();

    void on_btnKayitMenu_clicked();

    void on_btnKayit_clicked();

    void on_btnOnayla_clicked();

private:
    Ui::AnaMenu *ui;
    QSqlDatabase db;
    int kontrol,sayi1,sayi2,toplam;
};
#endif // ANAMENU_H
