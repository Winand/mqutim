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




#include "userinformation.h"
#include "icqpluginsystem.h" 

userInformation::userInformation(const QString &profile_name,bool owner, bool fromList,
		                         const QString &uin, const QString &mineUin, QWidget *parent)
    : QWidget(parent)
    , contactUin(uin)
    , ownUin(mineUin)
    , m_profile_name(profile_name)
{
	IcqPluginSystem &ips = IcqPluginSystem::instance();
	ui.setupUi(this);

	setWindowTitle(tr("%1 contact information").arg(uin));
	setWindowIcon(ips.getIcon("contactinfo"));
/*	pictureChanged= false;*/
	ui.maritalComboBox->setVisible(false);
	ui.label_7->setVisible(false);
/*	ui.pictureLabel->setText(QString ("<img src='") + ips.getIconFileName("noavatar") + "'>");*/
	if (mineUin == uin)
		owner = true;

	if ( owner )
		ui.uinEdit->setText(mineUin);
	else
		ui.uinEdit->setText(uin);

	ui.countryComboBox->insertItems(0, getCountryList());



	ui.origCountryComboBox->insertItems(0, getCountryList());


	ui.workCountryComboBox->insertItems(0, getCountryList());


	ui.langComboBox1->insertItems(0, getLangList());


	ui.langComboBox2->insertItems(0, getLangList());

	ui.langComboBox3->insertItems(0, getLangList());





	 ui.occupationComboBox->insertItems(0, QStringList()
	     << QString()
	     << QApplication::translate("searchUserClass", "Academic", 0, QApplication::UnicodeUTF8)
	     << QApplication::translate("searchUserClass", "Administrative", 0, QApplication::UnicodeUTF8)
	     << QApplication::translate("searchUserClass", "Art/Entertainment", 0, QApplication::UnicodeUTF8)
	     << QApplication::translate("searchUserClass", "College Student", 0, QApplication::UnicodeUTF8)
	     << QApplication::translate("searchUserClass", "Computers", 0, QApplication::UnicodeUTF8)
	     << QApplication::translate("searchUserClass", "Community & Social", 0, QApplication::UnicodeUTF8)
	     << QApplication::translate("searchUserClass", "Education", 0, QApplication::UnicodeUTF8)
	     << QApplication::translate("searchUserClass", "Engineering", 0, QApplication::UnicodeUTF8)
	     << QApplication::translate("searchUserClass", "Financial Services", 0, QApplication::UnicodeUTF8)
	     << QApplication::translate("searchUserClass", "Government", 0, QApplication::UnicodeUTF8)
	     << QApplication::translate("searchUserClass", "High School Student", 0, QApplication::UnicodeUTF8)
	     << QApplication::translate("searchUserClass", "Home", 0, QApplication::UnicodeUTF8)
	     << QApplication::translate("searchUserClass", "ICQ - Providing Help", 0, QApplication::UnicodeUTF8)
	     << QApplication::translate("searchUserClass", "Law", 0, QApplication::UnicodeUTF8)
	     << QApplication::translate("searchUserClass", "Managerial", 0, QApplication::UnicodeUTF8)
	     << QApplication::translate("searchUserClass", "Manufacturing", 0, QApplication::UnicodeUTF8)
	     << QApplication::translate("searchUserClass", "Medical/Health", 0, QApplication::UnicodeUTF8)
	     << QApplication::translate("searchUserClass", "Military", 0, QApplication::UnicodeUTF8)
	     << QApplication::translate("searchUserClass", "Non-Goverment Organisation", 0, QApplication::UnicodeUTF8)
	     << QApplication::translate("searchUserClass", "Professional", 0, QApplication::UnicodeUTF8)
	     << QApplication::translate("searchUserClass", "Retail", 0, QApplication::UnicodeUTF8)
	     << QApplication::translate("searchUserClass", "Retired", 0, QApplication::UnicodeUTF8)
	     << QApplication::translate("searchUserClass", "Science & Research", 0, QApplication::UnicodeUTF8)
	     << QApplication::translate("searchUserClass", "Sports", 0, QApplication::UnicodeUTF8)
	     << QApplication::translate("searchUserClass", "Technical", 0, QApplication::UnicodeUTF8)
	     << QApplication::translate("searchUserClass", "University student", 0, QApplication::UnicodeUTF8)
	     << QApplication::translate("searchUserClass", "Web building", 0, QApplication::UnicodeUTF8)
	     << QApplication::translate("searchUserClass", "Other services", 0, QApplication::UnicodeUTF8)
	    );


	 ui.interestsComboBox1->insertItems(0, getInterestList());
	 ui.interestsComboBox2->insertItems(0, getInterestList());
	 ui.interestsComboBox3->insertItems(0, getInterestList());
	 ui.interestsComboBox4->insertItems(0, getInterestList());

	 if ( !owner )
	 	{
		 	ui.authBox->setVisible(false);
		 	ui.additionalLabel->resize(ui.additionalLabel->width(),364);
	 		ui.birthDateEdit->setVisible(false);
	 		ui.birthDateEdit->setEnabled(false);
	 		ui.birthBox->setVisible(false);
	 		if ( !fromList)
	 		{
	 			ui.saveButton->setVisible(false);
	 			ui.requestButton->setEnabled(false);
	 		}
	 		ui.nickEdit->setReadOnly(true);
	 		ui.firstEdit->setReadOnly(true);
	 		ui.lastEdit->setReadOnly(true);
	 		ui.emailEdit->setReadOnly(true);
	 		ui.publishBox->setVisible(false);

	 		ui.countryComboBox->setEditable(true);
	 		ui.countryComboBox->lineEdit()->setReadOnly(true);
	 		ui.countryComboBox->installEventFilter(this);
//	 		ui.countryComboBox->setEnabled(false);
	 		ui.cityEdit->setReadOnly(true);
	 		ui.stateEdit->setReadOnly(true);
	 		ui.zipEdit->setReadOnly(true);
	 		ui.phoneEdit->setReadOnly(true);
	 		ui.faxEdit->setReadOnly(true);
	 		ui.cellularEdit->setReadOnly(true);
	 		ui.streeEdit->setReadOnly(true);

	 		ui.origCountryComboBox->setEditable(true);
	 		ui.origCountryComboBox->lineEdit()->setReadOnly(true);
	 		ui.origCountryComboBox->installEventFilter(this);
//	 		ui.origCountryComboBox->setEnabled(false);
	 		ui.origCityEdit->setReadOnly(true);
	 		ui.origStateEdit->setReadOnly(true);

	 		ui.workCountryComboBox->setEditable(true);
	 		ui.workCountryComboBox->lineEdit()->setReadOnly(true);
	 		ui.workCountryComboBox->installEventFilter(this);
//	 		ui.workCountryComboBox->setEnabled(false);

	 		ui.workCityEdit->setReadOnly(true);
	 		ui.workStateEdit->setReadOnly(true);
	 		ui.workZipEdit->setReadOnly(true);
	 		ui.workPhoneEdit->setReadOnly(true);
	 		ui.workFaxEdit->setReadOnly(true);
	 		ui.workStreetEdit->setReadOnly(true);
	 		ui.compNameEdit->setReadOnly(true);
	 		ui.divDeptEdit->setReadOnly(true);
	 		ui.positionEdit->setReadOnly(true);
	 		ui.webSiteEdit->setReadOnly(true);

	 		ui.occupationComboBox->setEditable(true);
	 		ui.occupationComboBox->lineEdit()->setReadOnly(true);
	 		ui.occupationComboBox->installEventFilter(this);
//	 		ui.occupationComboBox->setEnabled(false);
	 		ui.genderComboBox->setEditable(true);
	 		ui.genderComboBox->lineEdit()->setReadOnly(true);
	 		ui.genderComboBox->installEventFilter(this);
//	 		ui.genderComboBox->setEnabled(false);

	 		ui.homePageEdit->setReadOnly(true);

	 		ui.langComboBox1->setEditable(true);
	 		ui.langComboBox1->lineEdit()->setReadOnly(true);
	 		ui.langComboBox1->installEventFilter(this);
//	 		ui.langComboBox1->setEnabled(false);

	 		ui.langComboBox2->setEditable(true);
	 		ui.langComboBox2->lineEdit()->setReadOnly(true);
	 		ui.langComboBox2->installEventFilter(this);
//	 		ui.langComboBox2->setEnabled(false);

	 		ui.langComboBox3->setEditable(true);
	 		ui.langComboBox3->lineEdit()->setReadOnly(true);
	 		ui.langComboBox3->installEventFilter(this);
//	 		ui.langComboBox3->setEnabled(false);

	 		ui.interestsComboBox1->setEditable(true);
	 		ui.interestsComboBox1->lineEdit()->setReadOnly(true);
	 		ui.interestsComboBox1->installEventFilter(this);
//	 		ui.interestsComboBox1->setEnabled(false);

	 		ui.interestsComboBox2->setEditable(true);
	 		ui.interestsComboBox2->lineEdit()->setReadOnly(true);
	 		ui.interestsComboBox2->installEventFilter(this);
//	 		ui.interestsComboBox2->setEnabled(false);

	 		ui.interestsComboBox3->setEditable(true);
	 		ui.interestsComboBox3->lineEdit()->setReadOnly(true);
	 		ui.interestsComboBox3->installEventFilter(this);
//	 		ui.interestsComboBox3->setEnabled(false);

	 		ui.interestsComboBox4->setEditable(true);
	 		ui.interestsComboBox4->lineEdit()->setReadOnly(true);
	 		ui.interestsComboBox4->installEventFilter(this);
//	 		ui.interestsComboBox4->setEnabled(false);

	 		ui.interestsEdit1->setReadOnly(true);
	 		ui.interestsEdit2->setReadOnly(true);
	 		ui.interestsEdit3->setReadOnly(true);
	 		ui.interestsEdit4->setReadOnly(true);
	 		ui.aboutEdit->setReadOnly(true);
	 		ui.infoListWidget->setCurrentRow(0);
	 		if ( fromList )
	 		{
                                QSettings contacts(QSettings::NativeFormat, QSettings::UserScope, "qutim/qutim."+m_profile_name+"/ICQ."+ownUin, "contactlist");
	 			contacts.beginGroup(uin);


	 			setNick(contacts.value("nick", "").toString());
	 			setFirst(contacts.value("firstname", "").toString());
	 			setLast(contacts.value("lastname", "").toString());
	 			setEmail(contacts.value("email", "").toString());

	 			 setHomeCity(contacts.value("hcity", "").toString());
	 			 setHomeState(contacts.value("hstate", "").toString());
	 			 setHomePhone(contacts.value("hphone", "").toString());
	 			 setHomeFax(contacts.value("hfax", "").toString());
	 			 setHomeAddress(contacts.value("haddress", "").toString());
	 			 setCell(contacts.value("cell", "").toString());
	 			 setHomeZip(contacts.value("hzip", "").toString());
	 			 ui.countryComboBox->setCurrentIndex(contacts.value("country", 0).toUInt());
	 			 int age = contacts.value("age", 0).toUInt();
	 			 if ( age )
	 				 setAge(age);

	 			 setGender(contacts.value("gender", 0).toUInt());
	 			 setHomePage(contacts.value("hpage", "").toString());
	 			 quint16 year = contacts.value("birthyear", 0).toUInt();
	 			 quint8 month = contacts.value("birthmonth", 0).toUInt();
	 			 quint8 day = contacts.value("birthday", 0).toUInt();
	 			 if ( year && month && day)
	 			 {
	 				 ui.birthDateEdit->setVisible(true);
	 				 setBirthDay(year, month,day);
	 			 }
	 			 setLang(1,contacts.value("lang1", 0).toUInt());
	 			 setLang(2,contacts.value("lang2", 0).toUInt());
	 			 setLang(3,contacts.value("lang3", 0).toUInt());

	 			 setOriginalCity(contacts.value("ocity", "").toString());
	 			 setOriginalState(contacts.value("ostate", "").toString());
	 			 ui.origCountryComboBox->setCurrentIndex(contacts.value("ocountry", 0).toUInt());

	 			 setWorkCity(contacts.value("wcity", "").toString());
	 			 setWorkState(contacts.value("wstate", "").toString());
	 			 setWorkPhone(contacts.value("wphone", "").toString());
	 			 setWorkFax(contacts.value("wfax", "").toString());
	 			 setWorkAddress(contacts.value("waddress", "").toString());
	 			 setWorkZip(contacts.value("wzip", "").toString());

	 			 ui.workCountryComboBox->setCurrentIndex(contacts.value("wcountry", 0).toUInt());
	 			 setWorkCompany(contacts.value("company", "").toString());
	 			 setWorkDepartment(contacts.value("department", "").toString());
	 			 setWorkPosition(contacts.value("wposition", "").toString());
	 			 ui.occupationComboBox->setCurrentIndex(contacts.value("occupation", 0).toUInt());

	 			 setWorkWebPage(contacts.value("wpage", "").toString());
	 			 setInterests(contacts.value("inter1", "").toString(), contacts.value("incode1", "").toUInt(), 1);
	 			 setInterests(contacts.value("inter2", "").toString(), contacts.value("incode2", "").toUInt(), 2);
	 			 setInterests(contacts.value("inter3", "").toString(), contacts.value("incode3", "").toUInt(), 3);
	 			 setInterests(contacts.value("inter4", "").toString(), contacts.value("incode4", "").toUInt(), 4);

	 			 setAboutInfo(contacts.value("about", "").toString());

/*	 			QByteArray hash = contacts.value("iconhash").toByteArray();
	 			if ( !hash.isEmpty() )
	 			{
	 			 QString avatarPath = checkForAvatar(contacts.fileName().section('/', 0, -3),
	 					  hash);



	 			 if ( !avatarPath.isEmpty() )
	 			 {
	 				QSize picSize = getPictureSize(avatarPath);
	 			 	ui.pictureLabel->setText(tr("<img src='%1' height='%2' width='%3'>").arg(avatarPath).arg(picSize.height()).arg(picSize.width()));
	 			 }
                                }*/
	 			contacts.endGroup();
	 			makeSummary();
	 		}
	 	}

	 if ( owner )
	 {
/*                        QSettings settings(QSettings::NativeFormat, QSettings::UserScope, "qutim/qutim."+m_profile_name+"/ICQ."+ownUin, "accountsettings");

                 QByteArray hash = settings.value("main/iconhash").toByteArray();
			if ( !hash.isEmpty() )
			{
			 QString avatarPath = checkForAvatar(settings.fileName().section('/', 0, -3),
					  hash);



			 if ( !avatarPath.isEmpty() )
			 {
				QSize picSize = getPictureSize(avatarPath);
			 	ui.pictureLabel->setText(tr("<img src='%1' height='%2' width='%3'>").arg(avatarPath).arg(picSize.height()).arg(picSize.width()));
			 }
                        }*/

			ui.label->setVisible(false);
			ui.ageEdit->setVisible(false);
	 }

}

userInformation::~userInformation() {
    //
}

void userInformation::setCountry(quint16 code) {
	ui.countryComboBox->setCurrentIndex(getIndexFromCountryCode(code));
}


int userInformation::getIndexFromCountryCode(quint16 code)
{
	int countryCode = 0;

	switch(code )
				{
				case 93:
					countryCode = 1;
					break;
				case 355:
					countryCode = 2;
					break;
				case 213:
					countryCode = 3;
					break;
				case 684:
					countryCode = 4;
					break;
				case 376:
					countryCode = 5;
					break;
				case 244:
					countryCode = 6;
					break;
				case 101:
					countryCode = 7;
					break;
				case 102:
					countryCode = 8;
					break;
				case 1021:
					countryCode = 9;
					break;
				case 5902:
					countryCode = 10;
					break;
				case 54:
					countryCode = 11;
					break;
				case 374:
					countryCode = 12;
					break;
				case 297:
					countryCode = 13;
					break;
				case 247:
					countryCode = 14;
					break;
				case 61:
					countryCode = 15;
					break;
				case 43:
					countryCode = 16;
					break;
				case 994:
					countryCode = 17;
					break;
				case 103:
					countryCode = 18;
					break;
				case 973:
					countryCode = 19;
					break;
				case 880:
					countryCode = 20;
					break;
				case 104:
					countryCode = 21;
					break;
				case 120:
					countryCode = 22;
					break;
				case 375:
					countryCode = 23;
					break;
				case 32:
					countryCode = 24;
					break;
				case 501:
					countryCode = 25;
					break;
				case 229:
					countryCode = 26;
					break;
				case 105:
					countryCode = 27;
					break;
				case 975:
					countryCode = 28;
					break;
				case 591:
					countryCode = 29;
					break;
				case 267:
					countryCode = 30;
					break;
				case 55:
					countryCode = 31;
					break;
				case 673:
					countryCode = 32;
					break;
				case 359:
					countryCode = 33;
					break;
				case 226:
					countryCode = 34;
					break;
				case 257:
					countryCode = 35;
					break;
				case 855:
					countryCode = 36;
					break;
				case 237:
					countryCode = 37;
					break;
				case 107:
					countryCode = 38;
					break;
				case 178:
					countryCode = 39;
					break;
				case 108:
					countryCode = 40;
					break;
				case 235:
					countryCode = 41;
					break;
				case 56:
					countryCode = 42;
					break;
				case 86:
					countryCode = 43;
					break;
				case 672:
					countryCode = 44;
					break;
				case 57:
					countryCode = 45;
					break;
				case 2691:
					countryCode = 46;
					break;
				case 682:
					countryCode = 47;
					break;
				case 506:
					countryCode = 48;
					break;
				case 385:
					countryCode = 49;
					break;
				case 53:
					countryCode = 50;
					break;
				case 357:
					countryCode = 51;
					break;
				case 42:
					countryCode = 52;
					break;
				case 45:
					countryCode = 53;
					break;
				case 246:
					countryCode = 54;
					break;
				case 253:
					countryCode = 55;
					break;
				case 109:
					countryCode = 56;
					break;
				case 110:
					countryCode = 57;
					break;
				case 593:
					countryCode = 58;
					break;
				case 20:
					countryCode = 59;
					break;
				case 503:
					countryCode = 60;
					break;
				case 291:
					countryCode = 61;
					break;
				case 372:
					countryCode = 62;
					break;
				case 251:
					countryCode = 63;
					break;
				case 298:
					countryCode = 64;
					break;
				case 500:
					countryCode = 65;
					break;
				case 679:
					countryCode = 66;
					break;
				case 358:
					countryCode = 67;
					break;
				case 33:
					countryCode = 68;
					break;
				case 5901:
					countryCode = 69;
					break;
				case 594:
					countryCode = 70;
					break;
				case 689:
					countryCode = 71;
					break;
				case 241:
					countryCode = 72;
					break;
				case 220:
					countryCode = 73;
					break;
				case 995:
					countryCode = 74;
					break;
				case 49:
					countryCode = 75;
					break;
				case 233:
					countryCode = 76;
					break;
				case 350:
					countryCode = 77;
					break;
				case 30:
					countryCode = 78;
					break;
				case 299:
					countryCode = 79;
					break;
				case 111:
					countryCode = 80;
					break;
				case 590:
					countryCode = 81;
					break;
				case 502:
					countryCode = 82;
					break;
				case 224:
					countryCode = 83;
					break;
				case 245:
					countryCode = 84;
					break;
				case 592:
					countryCode = 85;
					break;
				case 509:
					countryCode = 86;
					break;
				case 504:
					countryCode = 87;
					break;
				case 852:
					countryCode = 88;
					break;
				case 36:
					countryCode = 89;
					break;
				case 354:
					countryCode = 90;
					break;
				case 91:
					countryCode = 91;
					break;
				case 62:
					countryCode = 92;
					break;
				case 964:
					countryCode = 93;
					break;
				case 353:
					countryCode = 94;
					break;
				case 972:
					countryCode = 95;
					break;
				case 39:
					countryCode = 96;
					break;
				case 112:
					countryCode = 97;
					break;
				case 81:
					countryCode = 98;
					break;
				case 962:
					countryCode = 99;
					break;
				case 705:
					countryCode = 100;
					break;
				case 254:
					countryCode = 101;
					break;
				case 686:
					countryCode = 102;
					break;
				case 850:
					countryCode = 103;
					break;
				case 82:
					countryCode = 104;
					break;
				case 965:
					countryCode = 105;
					break;
				case 706:
					countryCode = 106;
					break;
				case 856:
					countryCode = 107;
					break;
				case 371:
					countryCode = 108;
					break;
				case 961:
					countryCode = 109;
					break;
				case 266:
					countryCode = 110;
					break;
				case 231:
					countryCode = 111;
					break;
				case 4101:
					countryCode = 112;
					break;
				case 370:
					countryCode = 113;
					break;
				case 352:
					countryCode = 114;
					break;
				case 853:
					countryCode = 115;
					break;
				case 261:
					countryCode = 116;
					break;
				case 265:
					countryCode = 117;
					break;
				case 60:
					countryCode = 118;
					break;
				case 960:
					countryCode = 119;
					break;
				case 223:
					countryCode = 120;
					break;
				case 356:
					countryCode = 121;
					break;
				case 692:
					countryCode = 122;
					break;
				case 596:
					countryCode = 123;
					break;
				case 222:
					countryCode = 124;
					break;
				case 230:
					countryCode = 125;
					break;
				case 269:
					countryCode = 126;
					break;
				case 52:
					countryCode = 127;
					break;
				case 373:
					countryCode = 128;
					break;
				case 377:
					countryCode = 129;
					break;
				case 976:
					countryCode = 130;
					break;
				case 113:
					countryCode = 131;
					break;
				case 212:
					countryCode = 132;
					break;
				case 258:
					countryCode = 133;
					break;
				case 95:
					countryCode = 134;
					break;
				case 264:
					countryCode = 135;
					break;
				case 674:
					countryCode = 136;
					break;
				case 977:
					countryCode = 137;
					break;
				case 31:
					countryCode = 138;
					break;
				case 114:
					countryCode = 139;
					break;
				case 687:
					countryCode = 140;
					break;
				case 64:
					countryCode = 141;
					break;
				case 505:
					countryCode = 142;
					break;
				case 227:
					countryCode = 143;
					break;
				case 234:
					countryCode = 144;
					break;
				case 683:
					countryCode = 145;
					break;
				case 6722:
					countryCode = 146;
					break;
				case 47:
					countryCode = 147;
					break;
				case 968:
					countryCode = 148;
					break;
				case 92:
					countryCode = 149;
					break;
				case 680:
					countryCode = 150;
					break;
				case 507:
					countryCode = 151;
					break;
				case 675:
					countryCode = 152;
					break;
				case 595:
					countryCode = 153;
					break;
				case 51:
					countryCode = 154;
					break;
				case 63:
					countryCode = 155;
					break;
				case 48:
					countryCode = 156;
					break;
				case 351:
					countryCode = 157;
					break;
				case 121:
					countryCode = 158;
					break;
				case 974:
					countryCode = 159;
					break;
				case 262:
					countryCode = 160;
					break;
				case 40:
					countryCode = 161;
					break;
				case 6701:
					countryCode = 162;
					break;
				case 7:
					countryCode = 163;
					break;
				case 250:
					countryCode = 164;
					break;
				case 122:
					countryCode = 165;
					break;
				case 670:
					countryCode = 166;
					break;
				case 378:
					countryCode = 167;
					break;
				case 966:
					countryCode = 168;
					break;
				case 442:
					countryCode = 169;
					break;
				case 221:
					countryCode = 170;
					break;
				case 248:
					countryCode = 171;
					break;
				case 232:
					countryCode = 172;
					break;
				case 65:
					countryCode = 173;
					break;
				case 4201:
					countryCode = 174;
					break;
				case 386:
					countryCode = 175;
					break;
				case 677:
					countryCode = 176;
					break;
				case 252:
					countryCode = 177;
					break;
				case 27:
					countryCode = 178;
					break;
				case 34:
					countryCode = 179;
					break;
				case 94:
					countryCode = 180;
					break;
				case 290:
					countryCode = 181;
					break;
				case 115:
					countryCode = 182;
					break;
				case 249:
					countryCode = 183;
					break;
				case 597:
					countryCode = 184;
					break;
				case 268:
					countryCode = 185;
					break;
				case 46:
					countryCode = 186;
					break;
				case 41:
					countryCode = 187;
					break;
				case 963:
					countryCode = 188;
					break;
				case 886:
					countryCode = 189;
					break;
				case 708:
					countryCode = 190;
					break;
				case 255:
					countryCode = 191;
					break;
				case 66:
					countryCode = 192;
					break;
				case 6702:
					countryCode = 193;
					break;
				case 228:
					countryCode = 194;
					break;
				case 690:
					countryCode = 195;
					break;
				case 676:
					countryCode = 196;
					break;
				case 216:
					countryCode = 197;
					break;
				case 90:
					countryCode = 198;
					break;
				case 709:
					countryCode = 199;
					break;
				case 688:
					countryCode = 200;
					break;
				case 256:
					countryCode = 201;
					break;
				case 380:
					countryCode = 202;
					break;
				case 44:
					countryCode = 203;
					break;
				case 598:
					countryCode = 204;
					break;
				case 1:
					countryCode = 205;
					break;
				case 711:
					countryCode = 206;
					break;
				case 678:
					countryCode = 207;
					break;
				case 379:
					countryCode = 208;
					break;
				case 58:
					countryCode = 209;
					break;
				case 84:
					countryCode = 210;
					break;
				case 441:
					countryCode = 211;
					break;
				case 685:
					countryCode = 212;
					break;
				case 967:
					countryCode = 213;
					break;
				case 381:
					countryCode = 214;
					break;
				case 382:
					countryCode = 215;
					break;
				case 3811:
					countryCode = 216;
					break;
				case 260:
					countryCode = 217;
					break;
				case 263:
					countryCode = 218;
					break;
				}
	return countryCode;
}

void userInformation::setBirthDay(quint16 year, quint8 month, quint8 day)
{
	if ( contactUin != ownUin)
	{
		if ( year && month && day)
		{
			ui.birthDateEdit->setDate(QDate(year, month, day));
			ui.birthDateEdit->setVisible(true);
			ui.birthDateEdit->setEnabled(false);
		}
		else
		{
			ui.birthDateEdit->setEnabled(false);
			ui.birthDateEdit->setVisible(false);
		}
	} else {
		if ( year && month && day)
		{
			ui.birthDateEdit->setDate(QDate(year, month, day));
			ui.birthDateEdit->setVisible(true);
			ui.birthDateEdit->setEnabled(true);
			ui.birthBox->setChecked(true);
		}
	}
}

void userInformation::setLang(quint8 lang, quint8 code)
{
	if ( lang == 1)
		ui.langComboBox1->setCurrentIndex(code);
	if ( lang == 2)
			ui.langComboBox2->setCurrentIndex(code);
	if ( lang == 3)
			ui.langComboBox3->setCurrentIndex(code);
}

void userInformation::setOriginalCountry(quint16 code)
{
	ui.origCountryComboBox->setCurrentIndex(getIndexFromCountryCode(code));
}

void userInformation::setWorkCountry(quint16 code)
{
	ui.workCountryComboBox->setCurrentIndex(getIndexFromCountryCode(code));
}

void userInformation::setWorkOccupation(quint16 code)
{
	if ( code == 99)
		ui.occupationComboBox->setCurrentIndex(28);
	else
		ui.occupationComboBox->setCurrentIndex(code);
}

void userInformation::setInterests(const QString &keyWords, quint16 code, quint8 num)
{
	if ( num == 1 )
	{
		ui.interestsComboBox1->setCurrentIndex(code - 99);
		ui.interestsEdit1->setText(keyWords);
	}
	if ( num == 2 )
	{
		ui.interestsComboBox2->setCurrentIndex(code - 99);
		ui.interestsEdit2->setText(keyWords);
	}
	if ( num == 3 )
	{
		ui.interestsComboBox3->setCurrentIndex(code - 99);
		ui.interestsEdit3->setText(keyWords);
	}
	if ( num == 4 )
	{
		ui.interestsComboBox4->setCurrentIndex(code - 99);
		ui.interestsEdit4->setText(keyWords);
	}
}

/*QString userInformation::checkForAvatar(const QString &path, const QString &hash)
{

	if ( QFile::exists(path + "/icqicons/" + hash) )
		return (path + "/icqicons/" + hash);
	else
		return QString("");
}*/

void userInformation::on_saveButton_clicked()
{
	if ( contactUin != ownUin)
	{
                QSettings contacts(QSettings::NativeFormat, QSettings::UserScope, "qutim/qutim."+m_profile_name+"/ICQ."+ownUin, "contactlist");
	contacts.beginGroup(contactUin);

	contacts.setValue("nick", ui.nickEdit->text());
	contacts.setValue("firstname", ui.firstEdit->text());
	contacts.setValue("lastname", ui.lastEdit->text());
	contacts.setValue("email", ui.emailEdit->text());

	contacts.setValue("hcity", ui.cityEdit->text());
	contacts.setValue("hstate", ui.stateEdit->text());
	contacts.setValue("hphone", ui.phoneEdit->text());
	contacts.setValue("hfax", ui.faxEdit->text());
	contacts.setValue("haddress", ui.streeEdit->text());
	contacts.setValue("cell", ui.cellularEdit->text());
	contacts.setValue("hzip", ui.zipEdit->text());

	contacts.setValue("country", ui.countryComboBox->currentIndex());
	contacts.setValue("age", ui.ageEdit->text().toUInt());

	contacts.setValue("gender", ui.genderComboBox->currentIndex());
	contacts.setValue("hpage", ui.homePageEdit->text());
	if ( ui.birthDateEdit->isVisible() )
	{

		contacts.setValue("birthyear", ui.birthDateEdit->date().year());
		contacts.setValue("birthmonth", ui.birthDateEdit->date().month());
		contacts.setValue("birthday", ui.birthDateEdit->date().day());

	}

	contacts.setValue("lang1", ui.langComboBox1->currentIndex());
	contacts.setValue("lang2", ui.langComboBox2->currentIndex());
	contacts.setValue("lang3", ui.langComboBox3->currentIndex());

	contacts.setValue("ocity", ui.origCityEdit->text());
	contacts.setValue("ostate", ui.origStateEdit->text());
	contacts.setValue("ocountry", ui.origCountryComboBox->currentIndex());

	contacts.setValue("wcity", ui.workCityEdit->text());
	contacts.setValue("wstate", ui.workStateEdit->text());
	contacts.setValue("wphone", ui.workPhoneEdit->text());
	contacts.setValue("wfax", ui.workFaxEdit->text());
	contacts.setValue("waddress", ui.workStreetEdit->text());
	contacts.setValue("wzip", ui.workZipEdit->text());
	contacts.setValue("wcountry", ui.workCountryComboBox->currentIndex());
	contacts.setValue("company", ui.compNameEdit->text());
	contacts.setValue("department", ui.divDeptEdit->text());
	contacts.setValue("wposition", ui.positionEdit->text());
	contacts.setValue("occupation", ui.occupationComboBox->currentIndex());

	contacts.setValue("wpage", ui.webSiteEdit->text());

	contacts.setValue("inter1", ui.interestsEdit1->text());
	contacts.setValue("incode1", ui.interestsComboBox1->currentIndex() + 99);

	contacts.setValue("inter2", ui.interestsEdit2->text());
	contacts.setValue("incode2", ui.interestsComboBox2->currentIndex() + 99);

	contacts.setValue("inter3", ui.interestsEdit3->text());
	contacts.setValue("incode3", ui.interestsComboBox3->currentIndex() + 99);

	contacts.setValue("inter4", ui.interestsEdit4->text());
	contacts.setValue("incode4", ui.interestsComboBox4->currentIndex() + 99);

	contacts.setValue("about", ui.aboutEdit->toPlainText());

	contacts.endGroup();
	} else {
                emit saveOwnerInfo(/*pictureChanged, picturePath*/);
/*		pictureChanged = false;*/
	}
}

void userInformation::on_requestButton_clicked()
{
	ui.requestButton->setEnabled(false);
	emit requestUserInfo(contactUin);

}

void userInformation::makeSummary()
{
	QString summaryText;

	if ( !ui.nickEdit->text().isEmpty() )
		summaryText.append(tr("<b>Nick name:</b> %1 <br>").arg(Qt::escape(ui.nickEdit->text())));

	if ( !ui.firstEdit->text().isEmpty() )
		summaryText.append(tr("<b>First name:</b> %1 <br>").arg(Qt::escape(ui.firstEdit->text())));

	if ( !ui.lastEdit->text().isEmpty() )
		summaryText.append(tr("<b>Last name:</b> %1 <br>").arg(Qt::escape(ui.lastEdit->text())));
	if ( ui.countryComboBox->currentIndex() || !ui.cityEdit->text().isEmpty() )
	{

		summaryText.append(tr("<b>Home:</b> %1 %2<br>").arg(ui.countryComboBox->currentText()).arg(Qt::escape(ui.cityEdit->text())));
	}

	if ( ui.genderComboBox->currentIndex() )
		summaryText.append(tr("<b>Gender:</b> %1 <br>").arg(ui.genderComboBox->currentText()));
	if ( !ui.ageEdit->text().isEmpty() )
			summaryText.append(tr("<b>Age:</b> %1 <br>").arg(ui.ageEdit->text()));

	if ( ui.birthDateEdit->isEnabled() )
		summaryText.append(tr("<b>Birth date:</b> %1 <br>").arg(ui.birthDateEdit->date().toString()));

	if (ui.langComboBox1->currentIndex() || ui.langComboBox2->currentIndex() || ui.langComboBox3->currentIndex() )
		summaryText.append(tr("<b>Spoken languages:</b> %1 %2 %3<br>").arg(ui.langComboBox1->currentText()).arg(ui.langComboBox2->currentText()).arg(ui.langComboBox3->currentText()));

	ui.summaryLabel->setText(summaryText);
}

QSize userInformation::getPictureSize(const QString &path)
{
	QPixmap pic;
	QSize size;
	pic.load(path);
	size.setHeight(pic.height());
	size.setWidth(pic.width());
	if ( pic.height() >= pic.width() )
	{
		if (pic.height() > 64)
		{

			size.setHeight(64);
			size.setWidth( pic.width() / (pic.height() / 64.0));

		}

	} else {
		if (pic.width() > 64)
		{

			size.setWidth(64);
			size.setHeight( pic.height() / (pic.width() / 64.0));

		}
	}
	return size;
}

void userInformation::setAuth(quint8 auth, quint8 webaware, quint8 publishEMail)
{
	if ( !auth )
		ui.requiredButton->setChecked(true);
	else
		ui.allButton->setChecked(true);

	if (webaware)
		ui.webBox->setChecked(true);
	else
		ui.webBox->setChecked(false);
	ui.publishBox->setChecked(publishEMail);
}

quint16 userInformation::getCountryCodeFromIndex(int index)
{
	quint16 countryCode = 0;
	switch(index )
				{
				case 1:
					countryCode = 93;
					break;
				case 2:
					countryCode = 355;
					break;
				case 3:
					countryCode = 213;
					break;
				case 4:
					countryCode = 684;
					break;
				case 5:
					countryCode = 376;
					break;
				case 6:
					countryCode = 244;
					break;
				case 7:
					countryCode = 101;
					break;
				case 8:
					countryCode = 102;
					break;
				case 9:
					countryCode = 1021;
					break;
				case 10:
					countryCode = 5902;
					break;
				case 11:
					countryCode = 54;
					break;
				case 12:
					countryCode = 374;
					break;
				case 13:
					countryCode = 297;
					break;
				case 14:
					countryCode = 247;
					break;
				case 15:
					countryCode = 61;
					break;
				case 16:
					countryCode = 43;
					break;
				case 17:
					countryCode = 994;
					break;
				case 18:
					countryCode = 103;
					break;
				case 19:
					countryCode = 973;
					break;
				case 20:
					countryCode = 880;
					break;
				case 21:
					countryCode = 104;
					break;
				case 22:
					countryCode = 120;
					break;
				case 23:
					countryCode = 375;
					break;
				case 24:
					countryCode = 32;
					break;
				case 25:
					countryCode = 501;
					break;
				case 26:
					countryCode = 229;
					break;
				case 27:
					countryCode = 105;
					break;
				case 28:
					countryCode = 975;
					break;
				case 29:
					countryCode = 591;
					break;
				case 30:
					countryCode = 267;
					break;
				case 31:
					countryCode = 55;
					break;
				case 32:
					countryCode = 673;
					break;
				case 33:
					countryCode = 359;
					break;
				case 34:
					countryCode = 226;
					break;
				case 35:
					countryCode = 257;
					break;
				case 36:
					countryCode = 855;
					break;
				case 37:
					countryCode = 237;
					break;
				case 38:
					countryCode = 107;
					break;
				case 39:
					countryCode = 178;
					break;
				case 40:
					countryCode = 108;
					break;
				case 41:
					countryCode = 235;
					break;
				case 42:
					countryCode = 56;
					break;
				case 43:
					countryCode = 86;
					break;
				case 44:
					countryCode = 672;
					break;
				case 45:
					countryCode = 57;
					break;
				case 46:
					countryCode = 2691;
					break;
				case 47:
					countryCode = 682;
					break;
				case 48:
					countryCode = 506;
					break;
				case 49:
					countryCode = 385;
					break;
				case 50:
					countryCode = 53;
					break;
				case 51:
					countryCode = 357;
					break;
				case 52:
					countryCode = 42;
					break;
				case 53:
					countryCode = 45;
					break;
				case 54:
					countryCode = 246;
					break;
				case 55:
					countryCode = 253;
					break;
				case 56:
					countryCode = 109;
					break;
				case 57:
					countryCode = 110;
					break;
				case 58:
					countryCode = 593;
					break;
				case 59:
					countryCode = 20;
					break;
				case 60:
					countryCode = 503;
					break;
				case 61:
					countryCode = 291;
					break;
				case 62:
					countryCode = 372;
					break;
				case 63:
					countryCode = 251;
					break;
				case 64:
					countryCode = 298;
					break;
				case 65:
					countryCode = 500;
					break;
				case 66:
					countryCode = 679;
					break;
				case 67:
					countryCode = 358;
					break;
				case 68:
					countryCode = 33;
					break;
				case 69:
					countryCode = 5901;
					break;
				case 70:
					countryCode = 594;
					break;
				case 71:
					countryCode = 689;
					break;
				case 72:
					countryCode = 241;
					break;
				case 73:
					countryCode = 220;
					break;
				case 74:
					countryCode = 995;
					break;
				case 75:
					countryCode = 49;
					break;
				case 76:
					countryCode = 233;
					break;
				case 77:
					countryCode = 350;
					break;
				case 78:
					countryCode = 30;
					break;
				case 79:
					countryCode = 299;
					break;
				case 80:
					countryCode = 111;
					break;
				case 81:
					countryCode = 590;
					break;
				case 82:
					countryCode = 502;
					break;
				case 83:
					countryCode = 224;
					break;
				case 84:
					countryCode = 245;
					break;
				case 85:
					countryCode = 592;
					break;
				case 86:
					countryCode = 509;
					break;
				case 87:
					countryCode = 504;
					break;
				case 88:
					countryCode = 852;
					break;
				case 89:
					countryCode = 36;
					break;
				case 90:
					countryCode = 354;
					break;
				case 91:
					countryCode = 91;
					break;
				case 92:
					countryCode = 62;
					break;
				case 93:
					countryCode = 964;
					break;
				case 94:
					countryCode = 353;
					break;
				case 95:
					countryCode = 972;
					break;
				case 96:
					countryCode = 39;
					break;
				case 97:
					countryCode = 112;
					break;
				case 98:
					countryCode = 81;
					break;
				case 99:
					countryCode = 962;
					break;
				case 100:
					countryCode = 705;
					break;
				case 101:
					countryCode = 254;
					break;
				case 102:
					countryCode = 686;
					break;
				case 103:
					countryCode = 850;
					break;
				case 104:
					countryCode = 82;
					break;
				case 105:
					countryCode = 965;
					break;
				case 106:
					countryCode = 706;
					break;
				case 107:
					countryCode = 856;
					break;
				case 108:
					countryCode = 371;
					break;
				case 109:
					countryCode = 961;
					break;
				case 110:
					countryCode = 266;
					break;
				case 111:
					countryCode = 231;
					break;
				case 112:
					countryCode = 4101;
					break;
				case 113:
					countryCode = 370;
					break;
				case 114:
					countryCode = 352;
					break;
				case 115:
					countryCode = 853;
					break;
				case 116:
					countryCode = 261;
					break;
				case 117:
					countryCode = 265;
					break;
				case 118:
					countryCode = 60;
					break;
				case 119:
					countryCode = 960;
					break;
				case 120:
					countryCode = 223;
					break;
				case 121:
					countryCode = 356;
					break;
				case 122:
					countryCode = 692;
					break;
				case 123:
					countryCode = 596;
					break;
				case 124:
					countryCode = 222;
					break;
				case 125:
					countryCode = 230;
					break;
				case 126:
					countryCode = 269;
					break;
				case 127:
					countryCode = 52;
					break;
				case 128:
					countryCode = 373;
					break;
				case 129:
					countryCode = 377;
					break;
				case 130:
					countryCode = 976;
					break;
				case 131:
					countryCode = 113;
					break;
				case 132:
					countryCode = 212;
					break;
				case 133:
					countryCode = 258;
					break;
				case 134:
					countryCode = 95;
					break;
				case 135:
					countryCode = 264;
					break;
				case 136:
					countryCode = 674;
					break;
				case 137:
					countryCode = 977;
					break;
				case 138:
					countryCode = 31;
					break;
				case 139:
					countryCode = 114;
					break;
				case 140:
					countryCode = 687;
					break;
				case 141:
					countryCode = 64;
					break;
				case 142:
					countryCode = 505;
					break;
				case 143:
					countryCode = 227;
					break;
				case 144:
					countryCode = 234;
					break;
				case 145:
					countryCode = 683;
					break;
				case 146:
					countryCode = 6722;
					break;
				case 147:
					countryCode = 47;
					break;
				case 148:
					countryCode = 968;
					break;
				case 149:
					countryCode = 92;
					break;
				case 150:
					countryCode = 680;
					break;
				case 151:
					countryCode = 507;
					break;
				case 152:
					countryCode = 675;
					break;
				case 153:
					countryCode = 595;
					break;
				case 154:
					countryCode = 51;
					break;
				case 155:
					countryCode = 63;
					break;
				case 156:
					countryCode = 48;
					break;
				case 157:
					countryCode = 351;
					break;
				case 158:
					countryCode = 121;
					break;
				case 159:
					countryCode = 974;
					break;
				case 160:
					countryCode = 262;
					break;
				case 161:
					countryCode = 40;
					break;
				case 162:
					countryCode = 6701;
					break;
				case 163:
					countryCode = 7;
					break;
				case 164:
					countryCode = 250;
					break;
				case 165:
					countryCode = 122;
					break;
				case 166:
					countryCode = 670;
					break;
				case 167:
					countryCode = 378;
					break;
				case 168:
					countryCode = 966;
					break;
				case 169:
					countryCode = 442;
					break;
				case 170:
					countryCode = 221;
					break;
				case 171:
					countryCode = 248;
					break;
				case 172:
					countryCode = 232;
					break;
				case 173:
					countryCode = 65;
					break;
				case 174:
					countryCode = 4201;
					break;
				case 175:
					countryCode = 386;
					break;
				case 176:
					countryCode = 677;
					break;
				case 177:
					countryCode = 252;
					break;
				case 178:
					countryCode = 27;
					break;
				case 179:
					countryCode = 34;
					break;
				case 180:
					countryCode = 94;
					break;
				case 181:
					countryCode = 290;
					break;
				case 182:
					countryCode = 115;
					break;
				case 183:
					countryCode = 249;
					break;
				case 184:
					countryCode = 597;
					break;
				case 185:
					countryCode = 268;
					break;
				case 186:
					countryCode = 46;
					break;
				case 187:
					countryCode = 41;
					break;
				case 188:
					countryCode = 963;
					break;
				case 189:
					countryCode = 886;
					break;
				case 190:
					countryCode = 708;
					break;
				case 191:
					countryCode = 255;
					break;
				case 192:
					countryCode = 66;
					break;
				case 193:
					countryCode = 6702;
					break;
				case 194:
					countryCode = 228;
					break;
				case 195:
					countryCode = 690;
					break;
				case 196:
					countryCode = 676;
					break;
				case 197:
					countryCode = 216;
					break;
				case 198:
					countryCode = 90;
					break;
				case 199:
					countryCode = 709;
					break;
				case 200:
					countryCode = 688;
					break;
				case 201:
					countryCode = 256;
					break;
				case 202:
					countryCode = 380;
					break;
				case 203:
					countryCode = 44;
					break;
				case 204:
					countryCode = 598;
					break;
				case 205:
					countryCode = 1;
					break;
				case 206:
					countryCode = 711;
					break;
				case 207:
					countryCode = 678;
					break;
				case 208:
					countryCode = 379;
					break;
				case 209:
					countryCode = 58;
					break;
				case 210:
					countryCode = 84;
					break;
				case 211:
					countryCode = 441;
					break;
				case 212:
					countryCode = 685;
					break;
				case 213:
					countryCode = 967;
					break;
				case 214:
					countryCode = 381;
					break;
				case 215:
					countryCode = 382;
					break;
				case 216:
					countryCode = 3811;
					break;
				case 217:
					countryCode = 260;
					break;
				case 218:
					countryCode = 263;
					break;
				}
	return countryCode;
}

quint16 userInformation::getOccupation()
{
	quint16 occupationCode = 0;
	if ( int index = ui.occupationComboBox->currentIndex() )
			{
				if ( index ==  28)
					occupationCode = 99;
				else
					occupationCode = index;

			} else
				occupationCode = 0;
	return occupationCode;
}

quint16 userInformation::getInterests(int index)
{
	quint16 interestsCode = 0;
	if ( index == 1 )
	if ( int index = ui.interestsComboBox1->currentIndex() )
	{
				interestsCode = index + 99;
	}

	if ( index == 2 )
	if ( int index = ui.interestsComboBox2->currentIndex() )
	{
				interestsCode = index + 99;
	}

	if ( index == 3 )
	if ( int index = ui.interestsComboBox3->currentIndex() )
	{
				interestsCode = index + 99;
	}

	if ( index == 4 )
	if ( int index = ui.interestsComboBox4->currentIndex() )
	{
				interestsCode = index + 99;
	}

	return interestsCode;
}

QString userInformation::getInterestString(int index)
{

	if ( index == 1 )
		return ui.interestsEdit1->text();

	if ( index == 2 )
			return ui.interestsEdit2->text();

	if ( index == 3 )
			return ui.interestsEdit3->text();

	if ( index == 4 )
			return ui.interestsEdit4->text();

	return QString();

}

quint8 userInformation::getAuth()
{
	if ( ui.requiredButton->isChecked() )
		return 1;
	if( ui.allButton->isChecked())
		return 0;

	return 0;
}

/*void userInformation::on_addButton_clicked()
{
//	QString fileName =   QFileDialog::getOpenFileName(this, tr("Open File"),
//			 "",
//	                                                 tr("Images (*.gif *.bmp *.jpg *.jpeg *.png)"));
        QFileDialog dialog(0,tr("Open File"),"",tr("Images (*.gif *.bmp *.jpg *.jpeg *.png)"));
        dialog.setAttribute(Qt::WA_QuitOnClose, false);
	QStringList fileList;

        if ( dialog.exec())
                fileList = dialog.selectedFiles();
	if ( fileList.count())
	{
		QString fileName = fileList.at(0);
		if ( !fileName.isEmpty())
		{
			QFile iconFile(fileName);
	//		if (iconFile.open(QIODevice::ReadOnly))
	//		{
				if ( iconFile.size() > (6 * 1024))
				{
					QMessageBox::warning(this, tr("Open error"),
					                   tr("Image size is too big"));
				}
				else
				{
	 				QSize picSize = getPictureSize(fileName);
	 			 	ui.pictureLabel->setText(tr("<img src='%1' height='%2' width='%3'>").arg(fileName).arg(picSize.height()).arg(picSize.width()));
	 			 	pictureChanged = true;
	 			 	picturePath = fileName;
				}
	//		}
		}
	}
}

void userInformation::on_removeButton_clicked()
{
	pictureChanged = true;
	picturePath.clear();
	ui.pictureLabel->clear();
}*/

QStringList userInformation::getCountryList()
{
	return QStringList()
		     << QString()
		     << QApplication::translate("searchUserClass", "Afghanistan", 0, QApplication::UnicodeUTF8)
		     << QApplication::translate("searchUserClass", "Albania", 0, QApplication::UnicodeUTF8)
		     << QApplication::translate("searchUserClass", "Algeria", 0, QApplication::UnicodeUTF8)
		     << QApplication::translate("searchUserClass", "American Samoa", 0, QApplication::UnicodeUTF8)
		     << QApplication::translate("searchUserClass", "Andorra", 0, QApplication::UnicodeUTF8)
		     << QApplication::translate("searchUserClass", "Angola", 0, QApplication::UnicodeUTF8)
		     << QApplication::translate("searchUserClass", "Anguilla", 0, QApplication::UnicodeUTF8)
		     << QApplication::translate("searchUserClass", "Antigua", 0, QApplication::UnicodeUTF8)
		     << QApplication::translate("searchUserClass", "Antigua & Barbuda", 0, QApplication::UnicodeUTF8)
		     << QApplication::translate("searchUserClass", "Antilles", 0, QApplication::UnicodeUTF8)
		     << QApplication::translate("searchUserClass", "Argentina", 0, QApplication::UnicodeUTF8)
		     << QApplication::translate("searchUserClass", "Armenia", 0, QApplication::UnicodeUTF8)
		     << QApplication::translate("searchUserClass", "Aruba", 0, QApplication::UnicodeUTF8)
		     << QApplication::translate("searchUserClass", "AscensionIsland", 0, QApplication::UnicodeUTF8)
		     << QApplication::translate("searchUserClass", "Australia", 0, QApplication::UnicodeUTF8)
		     << QApplication::translate("searchUserClass", "Austria", 0, QApplication::UnicodeUTF8)
		     << QApplication::translate("searchUserClass", "Azerbaijan", 0, QApplication::UnicodeUTF8)
		     << QApplication::translate("searchUserClass", "Bahamas", 0, QApplication::UnicodeUTF8)
		     << QApplication::translate("searchUserClass", "Bahrain", 0, QApplication::UnicodeUTF8)
		     << QApplication::translate("searchUserClass", "Bangladesh", 0, QApplication::UnicodeUTF8)
		     << QApplication::translate("searchUserClass", "Barbados", 0, QApplication::UnicodeUTF8)
		     << QApplication::translate("searchUserClass", "Barbuda", 0, QApplication::UnicodeUTF8)
		     << QApplication::translate("searchUserClass", "Belarus", 0, QApplication::UnicodeUTF8)
		     << QApplication::translate("searchUserClass", "Belgium", 0, QApplication::UnicodeUTF8)
		     << QApplication::translate("searchUserClass", "Belize", 0, QApplication::UnicodeUTF8)
		     << QApplication::translate("searchUserClass", "Benin", 0, QApplication::UnicodeUTF8)
		     << QApplication::translate("searchUserClass", "Bermuda", 0, QApplication::UnicodeUTF8)
		     << QApplication::translate("searchUserClass", "Bhutan", 0, QApplication::UnicodeUTF8)
		     << QApplication::translate("searchUserClass", "Bolivia", 0, QApplication::UnicodeUTF8)
		     << QApplication::translate("searchUserClass", "Botswana", 0, QApplication::UnicodeUTF8)
		     << QApplication::translate("searchUserClass", "Brazil", 0, QApplication::UnicodeUTF8)
		     << QApplication::translate("searchUserClass", "Brunei", 0, QApplication::UnicodeUTF8)
		     << QApplication::translate("searchUserClass", "Bulgaria", 0, QApplication::UnicodeUTF8)
		     << QApplication::translate("searchUserClass", "Burkina Faso", 0, QApplication::UnicodeUTF8)
		     << QApplication::translate("searchUserClass", "Burundi", 0, QApplication::UnicodeUTF8)
		     << QApplication::translate("searchUserClass", "Cambodia", 0, QApplication::UnicodeUTF8)
		     << QApplication::translate("searchUserClass", "Cameroon", 0, QApplication::UnicodeUTF8)
		     << QApplication::translate("searchUserClass", "Canada", 0, QApplication::UnicodeUTF8)
		     << QApplication::translate("searchUserClass", "Canary Islands", 0, QApplication::UnicodeUTF8)
		     << QApplication::translate("searchUserClass", "Cayman Islands", 0, QApplication::UnicodeUTF8)
		     << QApplication::translate("searchUserClass", "Chad", 0, QApplication::UnicodeUTF8)
		     << QApplication::translate("searchUserClass", "Chile, Rep. of", 0, QApplication::UnicodeUTF8)
		     << QApplication::translate("searchUserClass", "China", 0, QApplication::UnicodeUTF8)
		     << QApplication::translate("searchUserClass", "Christmas Island", 0, QApplication::UnicodeUTF8)
		     << QApplication::translate("searchUserClass", "Colombia", 0, QApplication::UnicodeUTF8)
		     << QApplication::translate("searchUserClass", "Comoros", 0, QApplication::UnicodeUTF8)
		     << QApplication::translate("searchUserClass", "CookIslands", 0, QApplication::UnicodeUTF8)
		     << QApplication::translate("searchUserClass", "Costa Rica", 0, QApplication::UnicodeUTF8)
		     << QApplication::translate("searchUserClass", "Croatia", 0, QApplication::UnicodeUTF8)
		     << QApplication::translate("searchUserClass", "Cuba", 0, QApplication::UnicodeUTF8)
		     << QApplication::translate("searchUserClass", "Cyprus", 0, QApplication::UnicodeUTF8)
		     << QApplication::translate("searchUserClass", "Czech Rep.", 0, QApplication::UnicodeUTF8)
		     << QApplication::translate("searchUserClass", "Denmark", 0, QApplication::UnicodeUTF8)
		     << QApplication::translate("searchUserClass", "Diego Garcia", 0, QApplication::UnicodeUTF8)
		     << QApplication::translate("searchUserClass", "Djibouti", 0, QApplication::UnicodeUTF8)
		     << QApplication::translate("searchUserClass", "Dominica", 0, QApplication::UnicodeUTF8)
		     << QApplication::translate("searchUserClass", "Dominican Rep.", 0, QApplication::UnicodeUTF8)
		     << QApplication::translate("searchUserClass", "Ecuador", 0, QApplication::UnicodeUTF8)
		     << QApplication::translate("searchUserClass", "Egypt", 0, QApplication::UnicodeUTF8)
		     << QApplication::translate("searchUserClass", "El Salvador", 0, QApplication::UnicodeUTF8)
		     << QApplication::translate("searchUserClass", "Eritrea", 0, QApplication::UnicodeUTF8)
		     << QApplication::translate("searchUserClass", "Estonia", 0, QApplication::UnicodeUTF8)
		     << QApplication::translate("searchUserClass", "Ethiopia", 0, QApplication::UnicodeUTF8)
		     << QApplication::translate("searchUserClass", "Faeroe Islands", 0, QApplication::UnicodeUTF8)
		     << QApplication::translate("searchUserClass", "Falkland Islands", 0, QApplication::UnicodeUTF8)
		     << QApplication::translate("searchUserClass", "Fiji", 0, QApplication::UnicodeUTF8)
		     << QApplication::translate("searchUserClass", "Finland", 0, QApplication::UnicodeUTF8)
		     << QApplication::translate("searchUserClass", "France", 0, QApplication::UnicodeUTF8)
		     << QApplication::translate("searchUserClass", "FrenchAntilles", 0, QApplication::UnicodeUTF8)
		     << QApplication::translate("searchUserClass", "French Guiana", 0, QApplication::UnicodeUTF8)
		     << QApplication::translate("searchUserClass", "French Polynesia", 0, QApplication::UnicodeUTF8)
		     << QApplication::translate("searchUserClass", "Gabon", 0, QApplication::UnicodeUTF8)
		     << QApplication::translate("searchUserClass", "Gambia", 0, QApplication::UnicodeUTF8)
		     << QApplication::translate("searchUserClass", "Georgia", 0, QApplication::UnicodeUTF8)
		     << QApplication::translate("searchUserClass", "Germany", 0, QApplication::UnicodeUTF8)
		     << QApplication::translate("searchUserClass", "Ghana", 0, QApplication::UnicodeUTF8)
		     << QApplication::translate("searchUserClass", "Gibraltar", 0, QApplication::UnicodeUTF8)
		     << QApplication::translate("searchUserClass", "Greece", 0, QApplication::UnicodeUTF8)
		     << QApplication::translate("searchUserClass", "Greenland", 0, QApplication::UnicodeUTF8)
		     << QApplication::translate("searchUserClass", "Grenada", 0, QApplication::UnicodeUTF8)
		     << QApplication::translate("searchUserClass", "Guadeloupe", 0, QApplication::UnicodeUTF8)
		     << QApplication::translate("searchUserClass", "Guatemala", 0, QApplication::UnicodeUTF8)
		     << QApplication::translate("searchUserClass", "Guinea", 0, QApplication::UnicodeUTF8)
		     << QApplication::translate("searchUserClass", "Guinea-Bissau", 0, QApplication::UnicodeUTF8)
		     << QApplication::translate("searchUserClass", "Guyana", 0, QApplication::UnicodeUTF8)
		     << QApplication::translate("searchUserClass", "Haiti", 0, QApplication::UnicodeUTF8)
		     << QApplication::translate("searchUserClass", "Honduras", 0, QApplication::UnicodeUTF8)
		     << QApplication::translate("searchUserClass", "Hong Kong", 0, QApplication::UnicodeUTF8)
		     << QApplication::translate("searchUserClass", "Hungary", 0, QApplication::UnicodeUTF8)
		     << QApplication::translate("searchUserClass", "Iceland", 0, QApplication::UnicodeUTF8)
		     << QApplication::translate("searchUserClass", "India", 0, QApplication::UnicodeUTF8)
		     << QApplication::translate("searchUserClass", "Indonesia", 0, QApplication::UnicodeUTF8)
		     << QApplication::translate("searchUserClass", "Iraq", 0, QApplication::UnicodeUTF8)
		     << QApplication::translate("searchUserClass", "Ireland", 0, QApplication::UnicodeUTF8)
		     << QApplication::translate("searchUserClass", "Israel", 0, QApplication::UnicodeUTF8)
		     << QApplication::translate("searchUserClass", "Italy", 0, QApplication::UnicodeUTF8)
		     << QApplication::translate("searchUserClass", "Jamaica", 0, QApplication::UnicodeUTF8)
		     << QApplication::translate("searchUserClass", "Japan", 0, QApplication::UnicodeUTF8)
		     << QApplication::translate("searchUserClass", "Jordan", 0, QApplication::UnicodeUTF8)
		     << QApplication::translate("searchUserClass", "Kazakhstan", 0, QApplication::UnicodeUTF8)
		     << QApplication::translate("searchUserClass", "Kenya", 0, QApplication::UnicodeUTF8)
		     << QApplication::translate("searchUserClass", "Kiribati", 0, QApplication::UnicodeUTF8)
		     << QApplication::translate("searchUserClass", "Korea, North", 0, QApplication::UnicodeUTF8)
		     << QApplication::translate("searchUserClass", "Korea, South", 0, QApplication::UnicodeUTF8)
		     << QApplication::translate("searchUserClass", "Kuwait", 0, QApplication::UnicodeUTF8)
		     << QApplication::translate("searchUserClass", "Kyrgyzstan", 0, QApplication::UnicodeUTF8)
		     << QApplication::translate("searchUserClass", "Laos", 0, QApplication::UnicodeUTF8)
		     << QApplication::translate("searchUserClass", "Latvia", 0, QApplication::UnicodeUTF8)
		     << QApplication::translate("searchUserClass", "Lebanon", 0, QApplication::UnicodeUTF8)
		     << QApplication::translate("searchUserClass", "Lesotho", 0, QApplication::UnicodeUTF8)
		     << QApplication::translate("searchUserClass", "Liberia", 0, QApplication::UnicodeUTF8)
		     << QApplication::translate("searchUserClass", "Liechtenstein", 0, QApplication::UnicodeUTF8)
		     << QApplication::translate("searchUserClass", "Lithuania", 0, QApplication::UnicodeUTF8)
		     << QApplication::translate("searchUserClass", "Luxembourg", 0, QApplication::UnicodeUTF8)
		     << QApplication::translate("searchUserClass", "Macau", 0, QApplication::UnicodeUTF8)
		     << QApplication::translate("searchUserClass", "Madagascar", 0, QApplication::UnicodeUTF8)
		     << QApplication::translate("searchUserClass", "Malawi", 0, QApplication::UnicodeUTF8)
		     << QApplication::translate("searchUserClass", "Malaysia", 0, QApplication::UnicodeUTF8)
		     << QApplication::translate("searchUserClass", "Maldives", 0, QApplication::UnicodeUTF8)
		     << QApplication::translate("searchUserClass", "Mali", 0, QApplication::UnicodeUTF8)
		     << QApplication::translate("searchUserClass", "Malta", 0, QApplication::UnicodeUTF8)
		     << QApplication::translate("searchUserClass", "Marshall Islands", 0, QApplication::UnicodeUTF8)
		     << QApplication::translate("searchUserClass", "Martinique", 0, QApplication::UnicodeUTF8)
		     << QApplication::translate("searchUserClass", "Mauritania", 0, QApplication::UnicodeUTF8)
		     << QApplication::translate("searchUserClass", "Mauritius", 0, QApplication::UnicodeUTF8)
		     << QApplication::translate("searchUserClass", "MayotteIsland", 0, QApplication::UnicodeUTF8)
		     << QApplication::translate("searchUserClass", "Mexico", 0, QApplication::UnicodeUTF8)
		     << QApplication::translate("searchUserClass", "Moldova, Rep. of", 0, QApplication::UnicodeUTF8)
		     << QApplication::translate("searchUserClass", "Monaco", 0, QApplication::UnicodeUTF8)
		     << QApplication::translate("searchUserClass", "Mongolia", 0, QApplication::UnicodeUTF8)
		     << QApplication::translate("searchUserClass", "Montserrat", 0, QApplication::UnicodeUTF8)
		     << QApplication::translate("searchUserClass", "Morocco", 0, QApplication::UnicodeUTF8)
		     << QApplication::translate("searchUserClass", "Mozambique", 0, QApplication::UnicodeUTF8)
		     << QApplication::translate("searchUserClass", "Myanmar", 0, QApplication::UnicodeUTF8)
		     << QApplication::translate("searchUserClass", "Namibia", 0, QApplication::UnicodeUTF8)
		     << QApplication::translate("searchUserClass", "Nauru", 0, QApplication::UnicodeUTF8)
		     << QApplication::translate("searchUserClass", "Nepal", 0, QApplication::UnicodeUTF8)
		     << QApplication::translate("searchUserClass", "Netherlands", 0, QApplication::UnicodeUTF8)
		     << QApplication::translate("searchUserClass", "Nevis", 0, QApplication::UnicodeUTF8)
		     << QApplication::translate("searchUserClass", "NewCaledonia", 0, QApplication::UnicodeUTF8)
		     << QApplication::translate("searchUserClass", "New Zealand", 0, QApplication::UnicodeUTF8)
		     << QApplication::translate("searchUserClass", "Nicaragua", 0, QApplication::UnicodeUTF8)
		     << QApplication::translate("searchUserClass", "Niger", 0, QApplication::UnicodeUTF8)
		     << QApplication::translate("searchUserClass", "Nigeria", 0, QApplication::UnicodeUTF8)
		     << QApplication::translate("searchUserClass", "Niue", 0, QApplication::UnicodeUTF8)
		     << QApplication::translate("searchUserClass", "Norfolk Island", 0, QApplication::UnicodeUTF8)
		     << QApplication::translate("searchUserClass", "Norway", 0, QApplication::UnicodeUTF8)
		     << QApplication::translate("searchUserClass", "Oman", 0, QApplication::UnicodeUTF8)
		     << QApplication::translate("searchUserClass", "Pakistan", 0, QApplication::UnicodeUTF8)
		     << QApplication::translate("searchUserClass", "Palau", 0, QApplication::UnicodeUTF8)
		     << QApplication::translate("searchUserClass", "Panama", 0, QApplication::UnicodeUTF8)
		     << QApplication::translate("searchUserClass", "Papua New Guinea", 0, QApplication::UnicodeUTF8)
		     << QApplication::translate("searchUserClass", "Paraguay", 0, QApplication::UnicodeUTF8)
		     << QApplication::translate("searchUserClass", "Peru", 0, QApplication::UnicodeUTF8)
		     << QApplication::translate("searchUserClass", "Philippines", 0, QApplication::UnicodeUTF8)
		     << QApplication::translate("searchUserClass", "Poland", 0, QApplication::UnicodeUTF8)
		     << QApplication::translate("searchUserClass", "Portugal", 0, QApplication::UnicodeUTF8)
		     << QApplication::translate("searchUserClass", "Puerto Rico", 0, QApplication::UnicodeUTF8)
		     << QApplication::translate("searchUserClass", "Qatar", 0, QApplication::UnicodeUTF8)
		     << QApplication::translate("searchUserClass", "Reunion Island", 0, QApplication::UnicodeUTF8)
		     << QApplication::translate("searchUserClass", "Romania", 0, QApplication::UnicodeUTF8)
		     << QApplication::translate("searchUserClass", "Rota Island", 0, QApplication::UnicodeUTF8)
		     << QApplication::translate("searchUserClass", "Russia", 0, QApplication::UnicodeUTF8)
		     << QApplication::translate("searchUserClass", "Rwanda", 0, QApplication::UnicodeUTF8)
		     << QApplication::translate("searchUserClass", "Saint Lucia", 0, QApplication::UnicodeUTF8)
		     << QApplication::translate("searchUserClass", "Saipan Island", 0, QApplication::UnicodeUTF8)
		     << QApplication::translate("searchUserClass", "San Marino", 0, QApplication::UnicodeUTF8)
		     << QApplication::translate("searchUserClass", "Saudi Arabia", 0, QApplication::UnicodeUTF8)
		     << QApplication::translate("searchUserClass", "Scotland", 0, QApplication::UnicodeUTF8)
		     << QApplication::translate("searchUserClass", "Senegal", 0, QApplication::UnicodeUTF8)
		     << QApplication::translate("searchUserClass", "Seychelles", 0, QApplication::UnicodeUTF8)
		     << QApplication::translate("searchUserClass", "Sierra Leone", 0, QApplication::UnicodeUTF8)
		     << QApplication::translate("searchUserClass", "Singapore", 0, QApplication::UnicodeUTF8)
		     << QApplication::translate("searchUserClass", "Slovakia", 0, QApplication::UnicodeUTF8)
		     << QApplication::translate("searchUserClass", "Slovenia", 0, QApplication::UnicodeUTF8)
		     << QApplication::translate("searchUserClass", "Solomon Islands", 0, QApplication::UnicodeUTF8)
		     << QApplication::translate("searchUserClass", "Somalia", 0, QApplication::UnicodeUTF8)
		     << QApplication::translate("searchUserClass", "SouthAfrica", 0, QApplication::UnicodeUTF8)
		     << QApplication::translate("searchUserClass", "Spain", 0, QApplication::UnicodeUTF8)
		     << QApplication::translate("searchUserClass", "Sri Lanka", 0, QApplication::UnicodeUTF8)
		     << QApplication::translate("searchUserClass", "St. Helena", 0, QApplication::UnicodeUTF8)
		     << QApplication::translate("searchUserClass", "St. Kitts", 0, QApplication::UnicodeUTF8)
		     << QApplication::translate("searchUserClass", "Sudan", 0, QApplication::UnicodeUTF8)
		     << QApplication::translate("searchUserClass", "Suriname", 0, QApplication::UnicodeUTF8)
		     << QApplication::translate("searchUserClass", "Swaziland", 0, QApplication::UnicodeUTF8)
		     << QApplication::translate("searchUserClass", "Sweden", 0, QApplication::UnicodeUTF8)
		     << QApplication::translate("searchUserClass", "Switzerland", 0, QApplication::UnicodeUTF8)
		     << QApplication::translate("searchUserClass", "Syrian ArabRep.", 0, QApplication::UnicodeUTF8)
		     << QApplication::translate("searchUserClass", "Taiwan", 0, QApplication::UnicodeUTF8)
		     << QApplication::translate("searchUserClass", "Tajikistan", 0, QApplication::UnicodeUTF8)
		     << QApplication::translate("searchUserClass", "Tanzania", 0, QApplication::UnicodeUTF8)
		     << QApplication::translate("searchUserClass", "Thailand", 0, QApplication::UnicodeUTF8)
		     << QApplication::translate("searchUserClass", "Tinian Island", 0, QApplication::UnicodeUTF8)
		     << QApplication::translate("searchUserClass", "Togo", 0, QApplication::UnicodeUTF8)
		     << QApplication::translate("searchUserClass", "Tokelau", 0, QApplication::UnicodeUTF8)
		     << QApplication::translate("searchUserClass", "Tonga", 0, QApplication::UnicodeUTF8)
		     << QApplication::translate("searchUserClass", "Tunisia", 0, QApplication::UnicodeUTF8)
		     << QApplication::translate("searchUserClass", "Turkey", 0, QApplication::UnicodeUTF8)
		     << QApplication::translate("searchUserClass", "Turkmenistan", 0, QApplication::UnicodeUTF8)
		     << QApplication::translate("searchUserClass", "Tuvalu", 0, QApplication::UnicodeUTF8)
		     << QApplication::translate("searchUserClass", "Uganda", 0, QApplication::UnicodeUTF8)
		     << QApplication::translate("searchUserClass", "Ukraine", 0, QApplication::UnicodeUTF8)
		     << QApplication::translate("searchUserClass", "United Kingdom", 0, QApplication::UnicodeUTF8)
		     << QApplication::translate("searchUserClass", "Uruguay", 0, QApplication::UnicodeUTF8)
		     << QApplication::translate("searchUserClass", "USA", 0, QApplication::UnicodeUTF8)
		     << QApplication::translate("searchUserClass", "Uzbekistan", 0, QApplication::UnicodeUTF8)
		     << QApplication::translate("searchUserClass", "Vanuatu", 0, QApplication::UnicodeUTF8)
		     << QApplication::translate("searchUserClass", "Vatican City", 0, QApplication::UnicodeUTF8)
		     << QApplication::translate("searchUserClass", "Venezuela", 0, QApplication::UnicodeUTF8)
		     << QApplication::translate("searchUserClass", "Vietnam", 0, QApplication::UnicodeUTF8)
		     << QApplication::translate("searchUserClass", "Wales", 0, QApplication::UnicodeUTF8)
		     << QApplication::translate("searchUserClass", "Western Samoa", 0, QApplication::UnicodeUTF8)
		     << QApplication::translate("searchUserClass", "Yemen", 0, QApplication::UnicodeUTF8)
		     << QApplication::translate("searchUserClass", "Yugoslavia", 0, QApplication::UnicodeUTF8)
		     << QApplication::translate("searchUserClass", "Yugoslavia - Montenegro", 0, QApplication::UnicodeUTF8)
		     << QApplication::translate("searchUserClass", "Yugoslavia - Serbia", 0, QApplication::UnicodeUTF8)
		     << QApplication::translate("searchUserClass", "Zambia", 0, QApplication::UnicodeUTF8)
		     << QApplication::translate("searchUserClass", "Zimbabwe", 0, QApplication::UnicodeUTF8);
}

QStringList userInformation::getLangList()
{
	return QStringList()
    << QString()
    << QApplication::translate("searchUserClass", "Arabic", 0, QApplication::UnicodeUTF8)
    << QApplication::translate("searchUserClass", "Bhojpuri", 0, QApplication::UnicodeUTF8)
    << QApplication::translate("searchUserClass", "Bulgarian", 0, QApplication::UnicodeUTF8)
    << QApplication::translate("searchUserClass", "Burmese", 0, QApplication::UnicodeUTF8)
    << QApplication::translate("searchUserClass", "Cantonese", 0, QApplication::UnicodeUTF8)
    << QApplication::translate("searchUserClass", "Catalan", 0, QApplication::UnicodeUTF8)
    << QApplication::translate("searchUserClass", "Chinese", 0, QApplication::UnicodeUTF8)
    << QApplication::translate("searchUserClass", "Croatian", 0, QApplication::UnicodeUTF8)
    << QApplication::translate("searchUserClass", "Czech", 0, QApplication::UnicodeUTF8)
    << QApplication::translate("searchUserClass", "Danish", 0, QApplication::UnicodeUTF8)
    << QApplication::translate("searchUserClass", "Dutch", 0, QApplication::UnicodeUTF8)
    << QApplication::translate("searchUserClass", "English", 0, QApplication::UnicodeUTF8)
    << QApplication::translate("searchUserClass", "Esperanto", 0, QApplication::UnicodeUTF8)
    << QApplication::translate("searchUserClass", "Estonian", 0, QApplication::UnicodeUTF8)
    << QApplication::translate("searchUserClass", "Farsi", 0, QApplication::UnicodeUTF8)
    << QApplication::translate("searchUserClass", "Finnish", 0, QApplication::UnicodeUTF8)
    << QApplication::translate("searchUserClass", "French", 0, QApplication::UnicodeUTF8)
    << QApplication::translate("searchUserClass", "Gaelic", 0, QApplication::UnicodeUTF8)
    << QApplication::translate("searchUserClass", "German", 0, QApplication::UnicodeUTF8)
    << QApplication::translate("searchUserClass", "Greek", 0, QApplication::UnicodeUTF8)
    << QApplication::translate("searchUserClass", "Hebrew", 0, QApplication::UnicodeUTF8)
    << QApplication::translate("searchUserClass", "Hindi", 0, QApplication::UnicodeUTF8)
    << QApplication::translate("searchUserClass", "Hungarian", 0, QApplication::UnicodeUTF8)
    << QApplication::translate("searchUserClass", "Icelandic", 0, QApplication::UnicodeUTF8)
    << QApplication::translate("searchUserClass", "Indonesian", 0, QApplication::UnicodeUTF8)
    << QApplication::translate("searchUserClass", "Italian", 0, QApplication::UnicodeUTF8)
    << QApplication::translate("searchUserClass", "Japanese", 0, QApplication::UnicodeUTF8)
    << QApplication::translate("searchUserClass", "Khmer", 0, QApplication::UnicodeUTF8)
    << QApplication::translate("searchUserClass", "Korean", 0, QApplication::UnicodeUTF8)
    << QApplication::translate("searchUserClass", "Lao", 0, QApplication::UnicodeUTF8)
    << QApplication::translate("searchUserClass", "Latvian", 0, QApplication::UnicodeUTF8)
    << QApplication::translate("searchUserClass", "Lithuanian", 0, QApplication::UnicodeUTF8)
    << QApplication::translate("searchUserClass", "Malay", 0, QApplication::UnicodeUTF8)
    << QApplication::translate("searchUserClass", "Norwegian", 0, QApplication::UnicodeUTF8)
    << QApplication::translate("searchUserClass", "Polish", 0, QApplication::UnicodeUTF8)
    << QApplication::translate("searchUserClass", "Portuguese", 0, QApplication::UnicodeUTF8)
    << QApplication::translate("searchUserClass", "Romanian", 0, QApplication::UnicodeUTF8)
    << QApplication::translate("searchUserClass", "Russian", 0, QApplication::UnicodeUTF8)
    << QApplication::translate("searchUserClass", "Serbian", 0, QApplication::UnicodeUTF8)
    << QApplication::translate("searchUserClass", "Slovak", 0, QApplication::UnicodeUTF8)
    << QApplication::translate("searchUserClass", "Slovenian", 0, QApplication::UnicodeUTF8)
    << QApplication::translate("searchUserClass", "Somali", 0, QApplication::UnicodeUTF8)
    << QApplication::translate("searchUserClass", "Spanish", 0, QApplication::UnicodeUTF8)
    << QApplication::translate("searchUserClass", "Swahili", 0, QApplication::UnicodeUTF8)
    << QApplication::translate("searchUserClass", "Swedish", 0, QApplication::UnicodeUTF8)
    << QApplication::translate("searchUserClass", "Tagalog", 0, QApplication::UnicodeUTF8)
    << QApplication::translate("searchUserClass", "Tatar", 0, QApplication::UnicodeUTF8)
    << QApplication::translate("searchUserClass", "Thai", 0, QApplication::UnicodeUTF8)
    << QApplication::translate("searchUserClass", "Turkish", 0, QApplication::UnicodeUTF8)
    << QApplication::translate("searchUserClass", "Ukrainian", 0, QApplication::UnicodeUTF8)
    << QApplication::translate("searchUserClass", "Urdu", 0, QApplication::UnicodeUTF8)
    << QApplication::translate("searchUserClass", "Vietnamese", 0, QApplication::UnicodeUTF8)
    << QApplication::translate("searchUserClass", "Yiddish", 0, QApplication::UnicodeUTF8)
    << QApplication::translate("searchUserClass", "Yoruba", 0, QApplication::UnicodeUTF8)
    << QApplication::translate("searchUserClass", "Afrikaans", 0, QApplication::UnicodeUTF8)
    << QApplication::translate("searchUserClass", "Persian", 0, QApplication::UnicodeUTF8)
    << QApplication::translate("searchUserClass", "Albanian", 0, QApplication::UnicodeUTF8)
    << QApplication::translate("searchUserClass", "Armenian", 0, QApplication::UnicodeUTF8)
    << QApplication::translate("searchUserClass", "Kyrgyz", 0, QApplication::UnicodeUTF8)
    << QApplication::translate("searchUserClass", "Maltese", 0, QApplication::UnicodeUTF8);

}

QStringList userInformation::getInterestList()
{
	return QStringList()
    << QString()
    << QApplication::translate("searchUserClass", "Art", 0, QApplication::UnicodeUTF8)
    << QApplication::translate("searchUserClass", "Cars", 0, QApplication::UnicodeUTF8)
    << QApplication::translate("searchUserClass", "Celebrity Fans", 0, QApplication::UnicodeUTF8)
    << QApplication::translate("searchUserClass", "Collections", 0, QApplication::UnicodeUTF8)
    << QApplication::translate("searchUserClass", "Computers", 0, QApplication::UnicodeUTF8)
    << QApplication::translate("searchUserClass", "Culture & Literature", 0, QApplication::UnicodeUTF8)
    << QApplication::translate("searchUserClass", "Fitness", 0, QApplication::UnicodeUTF8)
    << QApplication::translate("searchUserClass", "Games", 0, QApplication::UnicodeUTF8)
    << QApplication::translate("searchUserClass", "Hobbies", 0, QApplication::UnicodeUTF8)
    << QApplication::translate("searchUserClass", "ICQ - Providing Help", 0, QApplication::UnicodeUTF8)
    << QApplication::translate("searchUserClass", "Internet", 0, QApplication::UnicodeUTF8)
    << QApplication::translate("searchUserClass", "Lifestyle", 0, QApplication::UnicodeUTF8)
    << QApplication::translate("searchUserClass", "Movies/TV", 0, QApplication::UnicodeUTF8)
    << QApplication::translate("searchUserClass", "Music", 0, QApplication::UnicodeUTF8)
    << QApplication::translate("searchUserClass", "Outdoor Activities", 0, QApplication::UnicodeUTF8)
    << QApplication::translate("searchUserClass", "Parenting", 0, QApplication::UnicodeUTF8)
    << QApplication::translate("searchUserClass", "Pets/Animals", 0, QApplication::UnicodeUTF8)
    << QApplication::translate("searchUserClass", "Religion", 0, QApplication::UnicodeUTF8)
    << QApplication::translate("searchUserClass", "Science/Technology", 0, QApplication::UnicodeUTF8)
    << QApplication::translate("searchUserClass", "Skills", 0, QApplication::UnicodeUTF8)
    << QApplication::translate("searchUserClass", "Sports", 0, QApplication::UnicodeUTF8)
    << QApplication::translate("searchUserClass", "Web Design", 0, QApplication::UnicodeUTF8)
    << QApplication::translate("searchUserClass", "Nature and Environment", 0, QApplication::UnicodeUTF8)
    << QApplication::translate("searchUserClass", "News & Media", 0, QApplication::UnicodeUTF8)
    << QApplication::translate("searchUserClass", "Government", 0, QApplication::UnicodeUTF8)
    << QApplication::translate("searchUserClass", "Business & Economy", 0, QApplication::UnicodeUTF8)
    << QApplication::translate("searchUserClass", "Mystics", 0, QApplication::UnicodeUTF8)
    << QApplication::translate("searchUserClass", "Travel", 0, QApplication::UnicodeUTF8)
    << QApplication::translate("searchUserClass", "Astronomy", 0, QApplication::UnicodeUTF8)
    << QApplication::translate("searchUserClass", "Space", 0, QApplication::UnicodeUTF8)
    << QApplication::translate("searchUserClass", "Clothing", 0, QApplication::UnicodeUTF8)
    << QApplication::translate("searchUserClass", "Parties", 0, QApplication::UnicodeUTF8)
    << QApplication::translate("searchUserClass", "Women", 0, QApplication::UnicodeUTF8)
    << QApplication::translate("searchUserClass", "Social science", 0, QApplication::UnicodeUTF8)
    << QApplication::translate("searchUserClass", "60's", 0, QApplication::UnicodeUTF8)
    << QApplication::translate("searchUserClass", "70's", 0, QApplication::UnicodeUTF8)
    << QApplication::translate("searchUserClass", "80's", 0, QApplication::UnicodeUTF8)
    << QApplication::translate("searchUserClass", "50's", 0, QApplication::UnicodeUTF8)
    << QApplication::translate("searchUserClass", "Finance and corporate", 0, QApplication::UnicodeUTF8)
    << QApplication::translate("searchUserClass", "Entertainment", 0, QApplication::UnicodeUTF8)
    << QApplication::translate("searchUserClass", "Consumer electronics", 0, QApplication::UnicodeUTF8)
    << QApplication::translate("searchUserClass", "Retail stores", 0, QApplication::UnicodeUTF8)
    << QApplication::translate("searchUserClass", "Health and beauty", 0, QApplication::UnicodeUTF8)
    << QApplication::translate("searchUserClass", "Media", 0, QApplication::UnicodeUTF8)
    << QApplication::translate("searchUserClass", "Household products", 0, QApplication::UnicodeUTF8)
    << QApplication::translate("searchUserClass", "Mail order catalog", 0, QApplication::UnicodeUTF8)
    << QApplication::translate("searchUserClass", "Business services", 0, QApplication::UnicodeUTF8)
    << QApplication::translate("searchUserClass", "Audio and visual", 0, QApplication::UnicodeUTF8)
    << QApplication::translate("searchUserClass", "Sporting and athletic", 0, QApplication::UnicodeUTF8)
    << QApplication::translate("searchUserClass", "Publishing", 0, QApplication::UnicodeUTF8)
    << QApplication::translate("searchUserClass", "Home automation", 0, QApplication::UnicodeUTF8);

}

bool userInformation::eventFilter( QObject * o, QEvent * e )
{

	if ( (QWidget*)o != this && (e->type() == QEvent::MouseButtonPress ||
			e->type() == QEvent::MouseButtonDblClick ||
			e->type() == QEvent::MouseButtonRelease ||
			e->type() == QEvent::KeyPress||
			e->type() == QEvent::KeyRelease ||
			e->type() == QEvent::Wheel))
	{
		e->ignore();
		return true;
	}
	return false;
}

void userInformation::setAdditional(quint32 externalIP,quint32 internalIP,quint32 onlineTime,quint32 signonTime,quint32 regTime,
		quint32 idleSinceTime, contactStatus status, const QString &clientId, const QList<QByteArray> &capList, const QList<quint16> &shortCaps,
		quint32 lastInfoUpdate, quint32 lastExtStatusInfoUpdate, quint32 lastExtInfoUpdate, bool online, quint8 protocol,
		quint32 userPort, quint32 Cookie)
{
	QString customToolTip;



	if ( externalIP )
	{
		customToolTip.append(QObject::tr("<b>External ip:</b> %1.%2.%3.%4<br>").arg(externalIP / 0x1000000).arg(
				externalIP % 0x1000000 / 0x10000).arg( externalIP % 0x10000 / 0x100).arg(externalIP % 0x100));
	}

	if ( internalIP )
	{
		customToolTip.append(QObject::tr("<b>Internal ip:</b> %1.%2.%3.%4<br>").arg(internalIP / 0x1000000).arg(
				internalIP % 0x1000000 / 0x10000).arg( internalIP % 0x10000 / 0x100).arg(internalIP % 0x100));
	}

	if ( online )
	{
		QDateTime time;
		time = time.toUTC();
		time.setTime_t(0);
		time = time.addSecs(onlineTime);

		customToolTip.append(QObject::tr("<b>Online time:</b> %1d %2h %3m %4s<br>").arg(time.date().day() - 1).arg(
				time.time().hour()).arg(time.time().minute()).arg(time.time().second()));

		time.setTime_t(signonTime);
		customToolTip.append(QObject::tr("<b>Signed on:</b> %1<br>").arg(time.toLocalTime().toString()));
		ui.lastLoginEdit->setText(time.toLocalTime().toString());
		time.setTime_t(regTime);

		if ( idleSinceTime ) {
			time.setTime_t(idleSinceTime);
			if (status == contactAway)
				customToolTip.append(QObject::tr("<b>Away since:</b> %1<br>").arg(time.toLocalTime().toString()));
			else if (status == contactNa)
				customToolTip.append(QObject::tr("<b>N/A since:</b> %1<br>").arg(time.toLocalTime().toString()));
		}

		customToolTip.append(QObject::tr("<b>Reg. date:</b> %1<br>").arg(time.toString()));

		ui.registrationEdit->setText(time.toString());

		customToolTip.append(QObject::tr("<b>Possible client:</b> %1<br>").arg(clientId));

		customToolTip.append(tr("<b>Protocol version: </b>%1<br>").arg(protocol));

		customToolTip.append(tr("<b>[Capabilities]</b><br>"));

		for ( int i = 0; i < capList.count(); i++)
		{
			customToolTip.append(capList.at(i).toHex() + "<br>");
		}

		if (shortCaps.count()) {
			customToolTip.append(tr("<b>[Short capabilities]</b><br>"));

			for ( int i = 0; i < shortCaps.count(); i++)
			{
				customToolTip.append("0x" + QString::number(shortCaps.at(i), 16) + "<br>");
			}
			}

		if (lastInfoUpdate | lastExtStatusInfoUpdate | lastExtInfoUpdate | userPort |Cookie) {
			customToolTip.append(tr("<b>[Direct connection extra info]</b><br>"));
			customToolTip.append("<b>lastInfoUpdate:</b> 0x" + QString::number(lastInfoUpdate, 16) + "<br>");
			customToolTip.append("<b>lastExtStatusInfoUpdate:</b> 0x" + QString::number(lastExtStatusInfoUpdate, 16) + "<br>");
			customToolTip.append("<b>lastExtInfoUpdate:</b> 0x" + QString::number(lastExtInfoUpdate, 16) + "<br>");
			customToolTip.append("<b>User Port:</b> " + QString::number(userPort, 10) + "<br>");
			customToolTip.append("<b>Cookie:</b> " + QString::number(Cookie, 10) + "<br>");
		}
	}

	ui.additionalLabel->setText(customToolTip);


}
