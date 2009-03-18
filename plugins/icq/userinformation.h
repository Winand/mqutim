/*
    userInformation

    Copyright (c) 2008 by Rustam Chakin <qutim.develop@gmail.com>

 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************
*/


#ifndef USERINFORMATION_H
#define USERINFORMATION_H

#include <QtGui>
#include "ui_userinformation.h"
#include "quticqglobals.h"

class userInformation : public QWidget
{
    Q_OBJECT

public:
    userInformation(const QString &profile_name, bool, bool, const QString &,const QString &, QWidget *parent = 0);
    ~userInformation();

    void setNick(const QString &t){ui.nickEdit->setText(t);}
    void setFirst(const QString &t){ui.firstEdit->setText(t);}
    void setLast(const QString &t){ui.lastEdit->setText(t);}
    void setEmail(const QString &t){ui.emailEdit->setText(t);}
    void setHomeCity(const QString &t){ui.cityEdit->setText(t);}
    void setHomeState(const QString &t){ui.stateEdit->setText(t);}
    void setHomePhone(const QString &t){ui.phoneEdit->setText(t);}
    void setHomeFax(const QString &t){ui.faxEdit->setText(t);}
    void setHomeAddress(const QString &t){ui.streeEdit->setText(t);}
    void setCell(const QString &t){ui.cellularEdit->setText(t);}
    void setHomeZip(const QString &t){ui.zipEdit->setText(t);}

    void setCountry(quint16);

    void setAge(quint16 a) {
    	if ( a )
    		ui.ageEdit->setText(QString::number(a));
    	}

    void setGender(quint8 g) { ui.genderComboBox->setCurrentIndex(g);}

    void setHomePage(const QString &t){ui.homePageEdit->setText(t);}

    void setBirthDay(quint16, quint8,quint8);
    void setLang(quint8, quint8);

    void setOriginalCity(const QString &t){ui.origCityEdit->setText(t);}
    void setOriginalState(const QString &t){ui.origStateEdit->setText(t);}
    void setOriginalCountry(quint16);

    void setWorkCity(const QString &t){ui.workCityEdit->setText(t);}
    void setWorkState(const QString &t){ui.workStateEdit->setText(t);}
    void setWorkPhone(const QString &t){ui.workPhoneEdit->setText(t);}
    void setWorkFax(const QString &t){ui.workFaxEdit->setText(t);}
    void setWorkAddress(const QString &t){ui.workStreetEdit->setText(t);}
    void setWorkZip(const QString &t){ui.workZipEdit->setText(t);}

    void setWorkCountry(quint16);

    void setWorkCompany(const QString &t){ui.compNameEdit->setText(t);}
    void setWorkDepartment(const QString &t){ui.divDeptEdit->setText(t);}
    void setWorkPosition(const QString &t){ui.positionEdit->setText(t);}

    void setWorkOccupation(quint16);

    void setWorkWebPage(const QString &t){ui.webSiteEdit->setText(t);}

    void setInterests(const QString&, quint16, quint8);

    void setAboutInfo(const QString & t){ui.aboutEdit->setPlainText(t); }

    void enableRequestButton() { ui.requestButton->setEnabled(true); makeSummary();}

    void setAuth(quint8, quint8,quint8);

    void setAdditional(quint32,quint32,quint32,quint32,quint32, quint32, contactStatus, const QString &, const QList<QByteArray> &, const QList<quint16> &, quint32, quint32, quint32, bool, quint8,quint32,
    		quint32);


    QString getNick(){return ui.nickEdit->text();}
    QString getFirst(){return ui.firstEdit->text();}
    QString getLast(){return ui.lastEdit->text();}
    QString getEmail(){return ui.emailEdit->text();}
    bool getPublish(){return ui.publishBox->isChecked();}
    QString contactUin;

    quint16 getHomeCountry(){return getCountryCodeFromIndex(ui.countryComboBox->currentIndex());}
    QString getHomeCity(){return ui.cityEdit->text();}
    QString getHomeState(){return ui.stateEdit->text();}
    QString getHomeZip(){return ui.zipEdit->text();}
    QString getHomePhone(){return ui.phoneEdit->text();}
    QString getHomeFax(){return ui.faxEdit->text();}
    QString getCellular(){return ui.cellularEdit->text();}
    QString getHomeStreet(){return ui.streeEdit->text();}

    quint16 getOrigCountry(){return getCountryCodeFromIndex(ui.origCountryComboBox->currentIndex());}
    QString getOrigCity(){return ui.origCityEdit->text();}
    QString getOrigState(){return ui.origStateEdit->text();}

    quint16 getWorkCountry(){return getCountryCodeFromIndex(ui.workCountryComboBox->currentIndex());}
    QString getWorkCity(){return ui.workCityEdit->text();}
    QString getWorkState(){return ui.workStateEdit->text();}
    QString getWorkZip(){return ui.workZipEdit->text();}
    QString getWorkPhone(){return ui.workPhoneEdit->text();}
    QString getWorkFax(){return ui.workFaxEdit->text();}
    QString getWorkStreet(){return ui.workStreetEdit->text();}

    QString getCompanyName(){return ui.compNameEdit->text();}
    quint16 getOccupation();
    QString getDepartment(){return ui.divDeptEdit->text();}
    QString getPosition() { return ui.positionEdit->text();}
    QString getWebPage() { return ui.webSiteEdit->text();}

    quint8 getGender() { return ui.genderComboBox->currentIndex();}
    QString getHomePage() { return ui.homePageEdit->text(); }
    bool sendBirth(){return ui.birthBox->isChecked();}
    QDate getBirth() {return ui.birthDateEdit->date();}
    quint8 getLang1() { return ui.langComboBox1->currentIndex();}
    quint8 getLang2() { return ui.langComboBox2->currentIndex();}
    quint8 getLang3() { return ui.langComboBox3->currentIndex();}
    quint16 getInterests(int);
    QString getInterestString(int);
    QString getAbout(){return ui.aboutEdit->toPlainText().left(2000);}
    quint8 getAuth();
    quint8 webAware(){return ui.webBox->isChecked();}

protected:
	bool eventFilter( QObject * o, QEvent * e );
private slots:
	void on_saveButton_clicked();
	void on_requestButton_clicked();
/*	void on_addButton_clicked();
        void on_removeButton_clicked();*/


signals:
	void requestUserInfo(const QString &);
        void saveOwnerInfo(/*bool, const QString &*/);

private:
    Ui::userInformationClass ui;
    int getIndexFromCountryCode(quint16);

/*    QString checkForAvatar(const QString&, const QString &);*/
    QString ownUin;
    void makeSummary();
    QSize getPictureSize(const QString &);
    quint16 getCountryCodeFromIndex(int);
/*    bool pictureChanged;
    QString picturePath;*/
    QStringList getCountryList();
    QStringList getLangList();
    QStringList getInterestList();
    QString m_profile_name;
};

#endif // USERINFORMATION_H
