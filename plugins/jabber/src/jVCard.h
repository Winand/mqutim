/*
Copyright (c) 2008 by Denis Daschenko <daschenko@gmail.com>
***************************************************************************
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
*                                                                         *
***************************************************************************
*/
/*
#ifndef JVCARD_H
#define JVCARD_H

#include <QtGui>
#include "ui_jVCard.h"
#include "ui_wPersonal.h"

#include <gloox/vcard.h>
#include "jPluginSystem.h"
#include "VCardRecord.h"
#include "VCardBirthday.h"
#include "VCardAvatar.h"
#include "VCardConst.h"

using namespace gloox;

class VCardRecord;

class jVCard : public QWidget
{
	Q_OBJECT

	public:
		jVCard(const QString &jid, bool mode, QWidget *parent = 0);
		~jVCard();
		void setVCard(const VCard *vcard, const QString &avatarUrl);
		void fillData();

	signals:
		void saveVCard(VCard *vcard);
		void requestVCard(const QString &jid);
		void closeVCard(const QString &jid);

	protected:
		void closeEvent(QCloseEvent *event);

	private slots:
		void s_saveVCard();
		void refreshVCard();
		void s_getPhoto();
		void s_addName() {addName(VCardConst::emptyString());};
		void s_addNick() {addNick(VCardConst::emptyString());};
		void s_addBirthday() {addBirthday(VCardConst::emptyString());};
		void s_addUrl() {addUrl(VCardConst::emptyString());};
		void s_addEmail() {addEmail(VCardConst::emptyString(), VCardConst::emptyMailStatus());};
		void s_addPhone() {addPhone(VCardConst::emptyString(), VCardConst::emptyPhoneStatus());};
		void s_addOrgName() {addOrgName(VCardConst::emptyString());};
		void s_addOrgUnit() {addOrgUnit(VCardConst::emptyString());};
		void s_addTitle() {addTitle(VCardConst::emptyString());};
		void s_addRole() {addRole(VCardConst::emptyString());};
		void s_addHomeCountry() {addHomeCountry(VCardConst::emptyString());};
		void s_addHomeRegion() {addHomeRegion(VCardConst::emptyString());};
		void s_addHomePostcode() {addHomePostcode(VCardConst::emptyString());};
		void s_addHomeCity() {addHomeCity(VCardConst::emptyString());};
		void s_addHomeStreet() {addHomeStreet(VCardConst::emptyString());};
		void s_addHomePostbox() {addHomePostbox(VCardConst::emptyString());};
		void s_addWorkCountry() {addWorkCountry(VCardConst::emptyString());};
		void s_addWorkRegion() {addWorkRegion(VCardConst::emptyString());};
		void s_addWorkCity() {addWorkCity(VCardConst::emptyString());};
		void s_addWorkPostcode() {addWorkPostcode(VCardConst::emptyString());};
		void s_addWorkStreet() {addWorkStreet(VCardConst::emptyString());};
		void s_addWorkPostbox() {addWorkPostbox(VCardConst::emptyString());};
		void showDeleteButton();
		void hideDeleteButton();
		void deleteEntry();

	private:
		void updatePhoto(const QString &file);
		void addName(const QString &name);
		void addNick(const QString &nick);
		void addMiscBox();
		void addBirthday(const QString &date);
		void addUrl(const QString &url);
		void addEmailBox();
		void addEmail(const QString &address, const QString &type);
		void addPhoneBox();
		void addPhone(const QString &number, const QString &type);
		void addHomeBox();
		void addWorkBox();
		VCardRecord* addAddressEntry(const QString &entry, QVBoxLayout *parent, VCardRecord *label, int layer);
		void addOrgName(const QString &name);
		void addOrgUnit(const QString &unit);
		void addTitle(const QString &title);
		void addRole(const QString &role);
		void addHomeCountry(const QString &county);
		void addHomeRegion(const QString &region);
		void addHomePostcode(const QString &pcode);
		void addHomeCity(const QString &city);
		void addHomeStreet(const QString &street);
		void addHomePostbox(const QString &pbox);
		void addWorkCountry(const QString &county);
		void addWorkRegion(const QString &region);
		void addWorkPostcode(const QString &pcode);
		void addWorkCity(const QString &city);
		void addWorkStreet(const QString &street);
		void addWorkPostbox(const QString &pbox);
		void checkMiscEntry();
		void checkHomeEntry();
		void checkWorkEntry();
		QString avatarUrl;
		bool m_mode;
		QString m_jid;
		QWidget *personalWidget;
		Ui::jVCard ui_main;
		Ui::Personal ui;
		VCard *m_vcard;
		VCardAvatar *avatarLabel;
		VCardRecord *nameLabel;
		VCardRecord *nickLabel;
		VCardBirthday *birthdayLabel;
		VCardRecord *urlLabel;
		QList<VCardRecord*> emailAddresses;
		QList<VCardRecord*> phoneNumbers;
		QGroupBox *miscBox;
		QVBoxLayout *miscList;
		QGroupBox *emailBox;
		QHBoxLayout *emailLayout;
		QVBoxLayout *emailList;
		QGroupBox *phoneBox;
		QHBoxLayout *phoneLayout;
		QVBoxLayout *phoneList;
		QGroupBox *homeBox;
		QHBoxLayout *homeLayout;
		QVBoxLayout *homeList;
		VCardRecord *homeCountryLabel;
		VCardRecord *homeRegionLabel;
		VCardRecord *homeCityLabel;
		VCardRecord *homePostcodeLabel;
		VCardRecord *homeStreetLabel;
		VCardRecord *homePostboxLabel;
		QGroupBox *workBox;
		QHBoxLayout *workLayout;
		QVBoxLayout *workList;
		VCardRecord *workCountryLabel;
		VCardRecord *workRegionLabel;
		VCardRecord *workCityLabel;
		VCardRecord *workPostcodeLabel;
		VCardRecord *workStreetLabel;
		VCardRecord *workPostboxLabel;
		VCardRecord *orgnameLabel;
		VCardRecord *orgunitLabel;
		VCardRecord *titleLabel;
		VCardRecord *roleLabel;
		int isAvatar, isName, isNick, isMisc, isBirthday, isUrl, isEmail, isPhone, isHome, isWork;
		int isOrgname, isOrgunit, isTitle, isRole;
		int isHomeCountry, isHomeRegion, isHomeCity, isHomePostcode, isHomeStreet, isHomePostbox;
		int isWorkCountry, isWorkRegion, isWorkCity, isWorkPostcode, isWorkStreet, isWorkPostbox;
		QMenu *menuAddEntry;
		QAction *actionUpdatePhoto;
		QAction *actionAddName;
		QAction *actionAddNick;
		QAction *actionAddBirthday;
		QAction *actionAddUrl;
		QAction *actionAddEmail;
		QAction *actionAddPhone;
		QAction *actionAddHomeCountry;
		QAction *actionAddHomeRegion;
		QAction *actionAddHomeCity;
		QAction *actionAddHomePostcode;
		QAction *actionAddHomeStreet;
		QAction *actionAddHomePostbox;
		QAction *actionAddWorkOrgname;
		QAction *actionAddWorkOrgunit;
		QAction *actionAddWorkTitle;
		QAction *actionAddWorkRole;
		QAction *actionAddWorkCountry;
		QAction *actionAddWorkRegion;
		QAction *actionAddWorkCity;
		QAction *actionAddWorkPostcode;
		QAction *actionAddWorkStreet;
		QAction *actionAddWorkPostbox;
		QMenu *menuEmailStatus;
		QAction *actionEmailPersonal;
		QAction *actionEmailWork;
		QAction *actionEmailEmpty;
		QSpacerItem *verticalSpacer;
		VCardEntry *hintLayout;
};

#endif // JVCARD_H
*/
