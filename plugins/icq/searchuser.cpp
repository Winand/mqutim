/*
    searchUser

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


#include "searchuser.h"
#include "statusiconsclass.h"
#include "icqpluginsystem.h"

searchUser::searchUser(const QString &profile_name, QWidget *parent)
    : QWidget(parent)
    , m_profile_name(profile_name)
{
	ui.setupUi(this);
	createContextMenu();
	QRegExp rx("[1-9][0-9-]{1,11}");
	QValidator *validator = new QRegExpValidator(rx, this);
	ui.uinEdit->setValidator(validator);

	QRegExp rxMail("([a-zA-Z0-9\\-\\_\\.]+@([a-zA-Z0-9\\-\\_]+\\.)+[a-zA-Z]+)");
	QValidator *validatorMail = new QRegExpValidator(rxMail, this);
	ui.emailEdit->setValidator(validatorMail);

	ui.resultTreeWidget->hideColumn(9);

	ui.moreBox->setVisible(false);
	adjustSize();
	setWindowTitle(tr("Add/find users"));
	setWindowIcon(IcqPluginSystem::instance().getIcon("search"));

	ui.uinButton->setChecked(true);
	ui.resultTreeWidget->setColumnWidth(0,22);
	ui.resultTreeWidget->setColumnWidth(1,22);
	ui.resultTreeWidget->setColumnWidth(6,60);
	ui.resultTreeWidget->setColumnWidth(7,70);

//	addFoundedContact(true, true, "1245345", "lo", "vasia", "koz", "r@t.ru", 0, 2, 2, 15);
//	addFoundedContact(true, true, "1245345", "lo", "vasia", "koz", "r@t.ru", 0, 2, 2, 15);
	gender = 0;
	minAge = 0;
	maxAge = 0;
	countryCode = 0;
	interestsCode = 0;
	languageCode = 0;
	ui.maritalComboBox->setVisible(false);
	ui.maritalLabel->setVisible(false);
}

searchUser::~searchUser() {
    //
}

void searchUser::on_moreButton_toggled(bool toggled) {
    ui.moreBox->setVisible(toggled);
}

void searchUser::on_clearButton_clicked()
{
	ui.uinEdit->clear();
	ui.emailEdit->clear();
	ui.nickEdit->clear();
	ui.firstEdit->clear();
	ui.lastEdit->clear();
	ui.genderComboBox->setCurrentIndex(0);
	ui.ageComboBox->setCurrentIndex(0);
	ui.countryComboBox->setCurrentIndex(0);
	ui.cityEdit->clear();
	ui.interestsComboBox->setCurrentIndex(0);
	ui.maritalComboBox->setCurrentIndex(0);
	ui.languageComboBox->setCurrentIndex(0);
	ui.occupationComboBox->setCurrentIndex(0);
	ui.keyWordEdit->clear();
}

QString searchUser::getUin()
{
	return ui.uinEdit->text().remove('-');
}

void searchUser::on_searchButton_clicked()
{
	if ( ui.uinButton->isChecked() && ui.uinEdit->text().isEmpty())
		return;

	if ( ui.emailButton->isChecked() && ui.emailEdit->text().isEmpty())
			return;

	if ( ui.otherButton->isChecked() )
	{
		bool sendSearchPacket = false;

		if ( !ui.nickEdit->text().isEmpty() )
			sendSearchPacket = true;

		if ( !ui.firstEdit->text().isEmpty() )
					sendSearchPacket = true;

		if ( !ui.lastEdit->text().isEmpty() )
					sendSearchPacket = true;

		if ( !ui.cityEdit->text().isEmpty() )
							sendSearchPacket = true;

		if ( !ui.keyWordEdit->text().isEmpty() )
							sendSearchPacket = true;

		if ( int index = ui.genderComboBox->currentIndex() )
		{
			gender = index;
			sendSearchPacket = true;
		} else
			gender = 0;

		if ( int index = ui.ageComboBox->currentIndex() )
		{
			switch(index)
			{
			case 1:
				minAge = 13;
				maxAge = 17;
				break;
			case 2:
				minAge = 18;
				maxAge = 22;
				break;
			case 3:
				minAge = 23;
				maxAge = 29;
				break;
			case 4:
				minAge = 30;
				maxAge = 39;
				break;
			case 5:
				minAge = 40;
				maxAge = 49;
				break;
			case 6:
				minAge = 50;
				maxAge = 59;
				break;
			case 7:
				minAge = 60;
				maxAge = 90;
				break;
			}


			sendSearchPacket = true;
		} else {
			minAge = 0;
			maxAge = 0;
		}

		if (int index =  ui.countryComboBox->currentIndex() )
		{

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



			sendSearchPacket = true;
		} else {
			countryCode = 0;
		}

		if ( int index = ui.interestsComboBox->currentIndex() )
		{
			interestsCode = index + 99;

			sendSearchPacket = true;
		} else
			interestsCode = 0;

		if ( int index = ui.languageComboBox->currentIndex() )
		{
			languageCode = index;
			sendSearchPacket = true;
		} else
			languageCode = 0;


		if ( int index = ui.occupationComboBox->currentIndex() )
		{
			if ( index ==  28)
				occupationCode = 99;
			else
				occupationCode = index;

			sendSearchPacket = true;
		} else
			occupationCode = 0;


		if ( !sendSearchPacket )
			return;
	}

	if ( !ui.previousBox->isChecked() )
		ui.resultTreeWidget->clear();

	ui.statusLabel->setText(tr("Searching"));
	if ( ui.uinButton->isChecked() )
		emit findAskedUsers(0);

	if ( ui.emailButton->isChecked() )
		emit findAskedUsers(1);

	if ( ui.otherButton->isChecked())
		emit findAskedUsers(2);
}

void searchUser::addFoundedContact(bool last, bool founded, const QString &uin, const QString &nick,
		const QString &firstName, const QString &lastName, const QString &email, const quint8 &authFlag,
		const quint16 &status, const quint8 &gender, const quint16 &age)
{
	IcqPluginSystem &ips = IcqPluginSystem::instance();
	
	if ( !founded )
	{
			ui.statusLabel->setText(tr("Nothing found"));
			return;
	}
	if ( last )
		ui.statusLabel->setText(tr("Done"));

	QTreeWidgetItem *contact = new QTreeWidgetItem(ui.resultTreeWidget);

	contact->setIcon(0, ips.getIcon("contactinfo"));
	contact->setIcon(1, statusIconClass::getInstance()->getContentIcon());
	contact->setText(2, uin);


	switch( status )
	{
	case 0:
		contact->setIcon(2, statusIconClass::getInstance()->getOfflineIcon());
		break;
	case 1:
		contact->setIcon(2, statusIconClass::getInstance()->getOnlineIcon());
		break;
	case 2:
		contact->setIcon(2, statusIconClass::getInstance()->getConnectingIcon());
		break;
	default:
		contact->setIcon(2, statusIconClass::getInstance()->getConnectingIcon());
	}

	contact->setText(3, nick);
	contact->setText(4, firstName);
	contact->setText(5, lastName);
	contact->setText(6, email);

	QString ageColumn;

	if ( gender == 1 )
		ageColumn = "F - ";
	else if (gender == 2)
		ageColumn = "M - ";

	ageColumn.append(QString::number(age));

	contact->setText(7, ageColumn);

	if ( authFlag )
	{
		contact->setText(8, tr("Always"));
	contact->setText(9, "0");
	}
	else
	{
		contact->setText(8, tr("Authorize"));
		contact->setText(9, "1");
	}

}

void searchUser::on_resultTreeWidget_itemClicked(QTreeWidgetItem *item, int column)
{
	if ( column == 1 )
		emit openChatWithFounded(item->text(2), item->text(3));
	else if ( column == 0)
		emit openInfoWindow(item->text(2), item->text(3),  item->text(4),  item->text(5));
}

QString searchUser::getEmail()
{
	return ui.emailEdit->text();
}

bool searchUser::onlineOnly()
{
	return ui.onlineOnlyBox->isChecked();
}

void searchUser::createContextMenu()
{
	IcqPluginSystem &ips = IcqPluginSystem::instance();
	contextMenu = new QMenu(ui.resultTreeWidget);
	addUser = new QAction(ips.getIcon("add_user"),tr("Add to contact list"),ui.resultTreeWidget );
	connect(addUser, SIGNAL(triggered()), this, SLOT(addUserActionActivated()) );

	userInformationAction = new QAction(ips.getIcon("contactinfo"),tr("Contact details"),ui.resultTreeWidget );

	connect(userInformationAction, SIGNAL(triggered()), this, SLOT(userInformationActionActivated()) );

	sendMessageAction = new QAction(statusIconClass::getInstance()->getContentIcon(),tr("Send message"),ui.resultTreeWidget );

	connect(sendMessageAction, SIGNAL(triggered()), this, SLOT(sendMessageActionActivated()) );

	checkStatus = new QAction(ips.getIcon("checkstat"),tr("Contact status check"),ui.resultTreeWidget );

	connect(checkStatus , SIGNAL(triggered()), this, SLOT(checkStatusActionActivated()) );

	contextMenu->addAction(addUser);
	contextMenu->addAction(userInformationAction);
	contextMenu->addAction(sendMessageAction);
	contextMenu->addAction(checkStatus);
}

void searchUser::on_resultTreeWidget_customContextMenuRequested ( const QPoint & point)
{
	clickedItemForContext = 0;

	clickedItemForContext = ui.resultTreeWidget->itemAt(point);

	if ( clickedItemForContext )
	{
		contextMenu->popup(ui.resultTreeWidget->mapToGlobal(ui.resultTreeWidget->mapFromGlobal(QCursor::pos())));
	}
}

void searchUser::addUserActionActivated()
{
	if ( clickedItemForContext )
	{
	if ( clickedItemForContext->text(9) == "0")
		emit addUserToContactList(clickedItemForContext->text(2),
				clickedItemForContext->text(3), false);
	else
		emit addUserToContactList(clickedItemForContext->text(2),
				clickedItemForContext->text(3), true);
	}
}

void searchUser::checkStatusActionActivated()
{
	if ( clickedItemForContext )
		emit checkStatusFor(clickedItemForContext->text(2));
}

void searchUser::userInformationActionActivated()
{
	if ( clickedItemForContext )
		emit openInfoWindow(clickedItemForContext->text(2), clickedItemForContext->text(3),  clickedItemForContext->text(4),  clickedItemForContext->text(5));
}

void searchUser::sendMessageActionActivated()
{
	if ( clickedItemForContext )
		emit openChatWithFounded(clickedItemForContext->text(2), clickedItemForContext->text(3));
}

void searchUser::on_resultTreeWidget_itemDoubleClicked( QTreeWidgetItem *item , int /* col  */)
{
	if ( item )
		{
		if ( item->text(9) == "0")
			emit addUserToContactList(item->text(2),
					item->text(3), false);
		else
			emit addUserToContactList(item->text(2),
					item->text(3), true);
		}
}
