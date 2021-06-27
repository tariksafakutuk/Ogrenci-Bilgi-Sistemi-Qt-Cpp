#ifndef ADMINMENU_H
#define ADMINMENU_H

#include <QMainWindow>
#include <QSql>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QMessageBox>
#include "anamenu.h"

namespace Ui {
class AdminMenu;
}

class AdminMenu : public QMainWindow
{
    Q_OBJECT

public:
    explicit AdminMenu(QWidget *parent = nullptr);
    ~AdminMenu();

    void init(QString kontrol);

    QStringList FakulteleriAl();

    QStringList BolumleriAl();

    void OgrIdOlustur();

    void AkdKullaniciAdiOlustur();

    void AkdDersAdListele();

    void AkdDersIdOlustur();

    void DersIdOlustur();

private slots:
    void on_btnKapat_clicked();

    void on_btnGenislet_clicked();

    void on_btnKucult_clicked();

    void on_btnGeri_clicked();

    void on_btnOgrEkle_clicked();

    void on_btnOgrSil_clicked();

    void on_btnAkdEkle_clicked();

    void on_btnAkdDersEkle_clicked();

    void on_btnFakulteEkle_clicked();

    void on_btnBolumEkle_clicked();

    void on_btnDersEkle_clicked();

    void on_btnDersSil_clicked();

    void on_btnCikis_clicked();

    void on_cmBoxOgrEkleFakulte_currentIndexChanged();

    void on_cmBoxOgrEkleBolum_currentIndexChanged();

    void on_rdLisans_clicked();

    void on_rdLisansustu_clicked();

    void on_btnOgrEkleOnayla_clicked();

    void on_btnOgrSilOnayla_clicked();

    void on_btnAkdEkleOnayla_clicked();

    void on_cmBoxAkdDersEkleBolum_currentIndexChanged();

    void on_rdAkdDersLisans_clicked();

    void on_rdAkdDersLisansustu_clicked();

    void on_spBoxAkdDersSinif_textChanged();

    void on_rdAkdDersEkleGuz_clicked();

    void on_rdAkdDersEkleBahar_clicked();

    void on_cmBoxAkdEkleDers_currentIndexChanged();

    void on_btnAkdDersEkleOnayla_clicked();

    void on_btnFakulteEkleOnayla_clicked();

    void on_btnBolumEkleOnayla_clicked();

    void on_cmBoxDersEkleBolum_currentTextChanged();

    void on_spBoxDersEkleSinif_textChanged();

    void on_rdDersEkleGuz_clicked();

    void on_rdDersEkleBahar_clicked();

    void on_btnDersEkleOnayla_clicked();

    void on_btnDersSilOnayla_clicked();

private:
    Ui::AdminMenu *ui;
    QSqlDatabase db;
    int kontrol;
};

#endif // ADMINMENU_H
