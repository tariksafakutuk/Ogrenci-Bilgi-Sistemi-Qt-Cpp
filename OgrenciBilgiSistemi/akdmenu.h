#ifndef AKDMENU_H
#define AKDMENU_H

#include <QMainWindow>
#include <QSql>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QLineEdit>
#include <QMessageBox>
#include "anamenu.h"

namespace Ui {
class AkdMenu;
}

class AkdMenu : public QMainWindow
{
    Q_OBJECT

public:
    explicit AkdMenu(QWidget *parent = nullptr,QString kullanici_adi="");
    ~AkdMenu();

    void init();
    QStringList BolumleriAl();

private slots:
    void on_btnKapat_clicked();

    void on_btnGenislet_clicked();

    void on_btnKucult_clicked();

    void on_btnVerilenDersler_clicked();

    void on_btnNotGiris_clicked();

    void on_cmBoxVerilenDerslerDonem_currentIndexChanged();

    void on_cmBoxVerilenDerslerBolum_currentIndexChanged();

    void on_cmBoxNotGirisDersId_currentIndexChanged();

    void on_btnNotGirisOnayla_clicked();

    void on_btnCikis_clicked();

private:
    Ui::AkdMenu *ui;
    QString kullanici_adi;
    QSqlDatabase db;
};

#endif // AKDMENU_H
