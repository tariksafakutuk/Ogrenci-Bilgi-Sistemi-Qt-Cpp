#ifndef OGRMENU_H
#define OGRMENU_H

#include <QMainWindow>
#include <QSql>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QSortFilterProxyModel>
#include <QCheckBox>
#include <QStyleOptionViewItem>

namespace Ui {
class OgrMenu;
}

class OgrMenu : public QMainWindow
{
    Q_OBJECT

public:
    explicit OgrMenu(QWidget *parent = nullptr,QString ogr_no="");
    ~OgrMenu();

    void init();

private slots:
    void on_btnKapat_clicked();

    void on_btnGenislet_clicked();

    void on_btnKucult_clicked();

    void on_btnOzluk_clicked();

    void on_btnOzlukKaydet_clicked();

    void on_btnDersKayit_clicked();

    void on_btnDersKayitOnayla_clicked();

    void on_btnAlinanDersler_clicked();

    void on_cmBoxAlinanDersler_currentIndexChanged();

    void on_btnNotListesi_clicked();

    void on_cmBoxNotListesi_currentIndexChanged();

    void on_btnCikis_clicked();

private:
    Ui::OgrMenu *ui;
    QSqlDatabase db;
    QString ogr_no;
};

#endif // OGRMENU_H
