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
#include "jVCard.h"
#include "jProtocol.h"
#include <QFile>

jVCard::jVCard(const QString &jid, bool mode, QWidget *parent) : QWidget(parent)
{
	m_mode = mode;
	m_jid = jid;
	ui_main.setupUi(this);
	verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::MinimumExpanding);
	ui_main.widgetLayout->insertSpacerItem(0, verticalSpacer);
	ui_main.saveButton->setVisible(m_mode);
	ui_main.saveButton->setIcon(jPluginSystem::instance().getIcon("save_all"));
	ui_main.requestButton->setEnabled(false);
	ui_main.requestButton->setIcon(jPluginSystem::instance().getIcon("request"));
	ui_main.closeButton->setIcon(jPluginSystem::instance().getIcon("cancel"));
	connect(ui_main.saveButton, SIGNAL(clicked()), this, SLOT(s_saveVCard()));
	connect(ui_main.requestButton, SIGNAL(clicked()), this, SLOT(refreshVCard()));
	menuAddEntry = new QMenu();
	actionUpdatePhoto = new QAction(tr("Update photo"), this);
	connect(actionUpdatePhoto, SIGNAL(triggered()), this, SLOT(s_getPhoto()));
	menuAddEntry->addAction(actionUpdatePhoto);
	actionAddName = new QAction(tr("Add name"), this);
	connect(actionAddName, SIGNAL(triggered()), this, SLOT(s_addName()));
	menuAddEntry->addAction(actionAddName);
	actionAddNick = new QAction(tr("Add nick"), this);
	connect(actionAddNick, SIGNAL(triggered()), this, SLOT(s_addNick()));
	menuAddEntry->addAction(actionAddNick);
	actionAddBirthday = new QAction(tr("Add birthday"), this);
	connect(actionAddBirthday, SIGNAL(triggered()), this, SLOT(s_addBirthday()));
	menuAddEntry->addAction(actionAddBirthday);
	actionAddUrl = new QAction(tr("Add homepage"), this);
	connect(actionAddUrl, SIGNAL(triggered()), this, SLOT(s_addUrl()));
	menuAddEntry->addAction(actionAddUrl);
	actionAddEmail = new QAction(tr("Add e-mail"), this);
	connect(actionAddEmail, SIGNAL(triggered()), this, SLOT(s_addEmail()));
	menuAddEntry->addAction(actionAddEmail);
	actionAddPhone = new QAction(tr("Add phone"), this);
	connect(actionAddPhone, SIGNAL(triggered()), this, SLOT(s_addPhone()));
	menuAddEntry->addAction(actionAddPhone);
	QMenu *menuAddHomeEntry = new QMenu(tr("Update home address"));
	menuAddEntry->addMenu(menuAddHomeEntry);
	actionAddHomeCountry = new QAction(tr("Add country"), this);
	connect(actionAddHomeCountry, SIGNAL(triggered()), this, SLOT(s_addHomeCountry()));
	menuAddHomeEntry->addAction(actionAddHomeCountry);
	actionAddHomeRegion = new QAction(tr("Add region"), this);
	connect(actionAddHomeRegion, SIGNAL(triggered()), this, SLOT(s_addHomeRegion()));
	menuAddHomeEntry->addAction(actionAddHomeRegion);
	actionAddHomeCity = new QAction(tr("Add city"), this);
	connect(actionAddHomeCity, SIGNAL(triggered()), this, SLOT(s_addHomeCity()));
	menuAddHomeEntry->addAction(actionAddHomeCity);
	actionAddHomePostcode = new QAction(tr("Add postcode"), this);
	connect(actionAddHomePostcode, SIGNAL(triggered()), this, SLOT(s_addHomePostcode()));
	menuAddHomeEntry->addAction(actionAddHomePostcode);
	actionAddHomeStreet = new QAction(tr("Add street"), this);
	connect(actionAddHomeStreet, SIGNAL(triggered()), this, SLOT(s_addHomeStreet()));
	menuAddHomeEntry->addAction(actionAddHomeStreet);
	actionAddHomePostbox = new QAction(tr("Add PO box"), this);
	connect(actionAddHomePostbox, SIGNAL(triggered()), this, SLOT(s_addHomePostbox()));
	menuAddHomeEntry->addAction(actionAddHomePostbox);
	QMenu *menuAddWorkEntry = new QMenu(tr("Update work place"));
	menuAddEntry->addMenu(menuAddWorkEntry);
	actionAddWorkOrgname = new QAction(tr("Add organization name"), this);
	connect(actionAddWorkOrgname, SIGNAL(triggered()), this, SLOT(s_addOrgName()));
	menuAddWorkEntry->addAction(actionAddWorkOrgname);
	actionAddWorkOrgunit = new QAction(tr("Add organization unit"), this);
	connect(actionAddWorkOrgunit, SIGNAL(triggered()), this, SLOT(s_addOrgUnit()));
	menuAddWorkEntry->addAction(actionAddWorkOrgunit);
	actionAddWorkTitle = new QAction(tr("Add title"), this);
	connect(actionAddWorkTitle, SIGNAL(triggered()), this, SLOT(s_addTitle()));
	menuAddWorkEntry->addAction(actionAddWorkTitle);
	actionAddWorkRole = new QAction(tr("Add role"), this);
	connect(actionAddWorkRole, SIGNAL(triggered()), this, SLOT(s_addRole()));
	menuAddWorkEntry->addAction(actionAddWorkRole);
	actionAddWorkCountry = new QAction(tr("Add country"), this);
	connect(actionAddWorkCountry, SIGNAL(triggered()), this, SLOT(s_addWorkCountry()));
	menuAddWorkEntry->addAction(actionAddWorkCountry);
	actionAddWorkRegion = new QAction(tr("Add region"), this);
	connect(actionAddWorkRegion, SIGNAL(triggered()), this, SLOT(s_addWorkRegion()));
	menuAddWorkEntry->addAction(actionAddWorkRegion);
	actionAddWorkCity = new QAction(tr("Add city"), this);
	connect(actionAddWorkCity, SIGNAL(triggered()), this, SLOT(s_addWorkCity()));
	menuAddWorkEntry->addAction(actionAddWorkCity);
	actionAddWorkPostcode = new QAction(tr("Add postcode"), this);
	connect(actionAddWorkPostcode, SIGNAL(triggered()), this, SLOT(s_addWorkPostcode()));
	menuAddWorkEntry->addAction(actionAddWorkPostcode);
	actionAddWorkStreet = new QAction(tr("Add street"), this);
	connect(actionAddWorkStreet, SIGNAL(triggered()), this, SLOT(s_addWorkStreet()));
	menuAddWorkEntry->addAction(actionAddWorkStreet);
	actionAddWorkPostbox = new QAction(tr("Add PO box"), this);
	connect(actionAddWorkPostbox, SIGNAL(triggered()), this, SLOT(s_addWorkPostbox()));
	menuAddWorkEntry->addAction(actionAddWorkPostbox);
	menuEmailStatus = new QMenu();
}

jVCard::~jVCard()
{
}

void jVCard::closeEvent(QCloseEvent *event)
{
	emit closeVCard(m_jid);
}

void jVCard::fillData()
{
	emit requestVCard(m_jid);
}

void jVCard::refreshVCard()
{
	ui_main.requestButton->setEnabled(false);
	ui_main.widgetLayout->removeItem(ui_main.widgetLayout->takeAt(0));
	delete personalWidget;
	ui_main.widgetLayout->insertSpacerItem(0, verticalSpacer);
	emailAddresses.clear();
	phoneNumbers.clear();
	fillData();
}

void jVCard::setVCard(const VCard *vcard, const QString &avatarUrl)
{
	isAvatar = 0;
	isName = 0;
	isNick = 0;
	isMisc = 0;
	isBirthday = 0;
	isUrl = 0;
	isEmail = 0;
	isPhone = 0;
	isHome = 0;
	isWork = 0;
	isOrgname = 0;
	isOrgunit = 0;
	isTitle = 0;
	isRole = 0;
	isHomeCountry = 0;
	isHomeRegion = 0;
	isHomeCity = 0;
	isHomePostcode = 0;
	isHomeStreet = 0;
	isHomePostbox = 0;
	isWorkCountry = 0;
	isWorkRegion = 0;
	isWorkCity = 0;
	isWorkPostcode = 0;
	isWorkStreet = 0;
	isWorkPostbox = 0;
	ui_main.requestButton->setEnabled(true);
	personalWidget = new QWidget();
	ui.setupUi(personalWidget);
	ui.addButton->setVisible(m_mode);
	ui.addButton->setIcon(jPluginSystem::instance().getIcon("add"));
	ui.addButton->setPopupMode(QToolButton::InstantPopup);
	if (m_mode)
		ui.addButton->setMenu(menuAddEntry);
	ui.deleteButton->setVisible(false);
	ui.deleteButton->setIcon(jPluginSystem::instance().getIcon("remove"));
	connect(ui.deleteButton, SIGNAL(clicked()), this, SLOT(deleteEntry()));
	ui_main.widgetLayout->removeItem(ui_main.widgetLayout->takeAt(0));
	ui_main.widgetLayout->insertWidget(0, personalWidget);
	m_vcard = (VCard*) vcard;
	avatarLabel = new VCardAvatar(m_mode);
	connect(avatarLabel, SIGNAL(mouseOver()), this, SLOT(showDeleteButton()));
	connect(avatarLabel, SIGNAL(mouseOut()), this, SLOT(hideDeleteButton()));
	ui.aboutLayout->insertWidget(0, avatarLabel);
	if (avatarUrl != "")
		updatePhoto(avatarUrl);
	if (jProtocol::fromStd(m_vcard->formattedname()) != "")
		addName(jProtocol::fromStd(m_vcard->formattedname()));
	if (jProtocol::fromStd(m_vcard->nickname()) != "")
		addNick(jProtocol::fromStd(m_vcard->nickname()));
	if (jProtocol::fromStd(m_vcard->bday()) != "")
		addBirthday(jProtocol::fromStd(m_vcard->bday()));
	if (jProtocol::fromStd(m_vcard->url()) != "")
		addUrl(jProtocol::fromStd(m_vcard->url()));
	if (m_vcard->emailAddresses().size())
		foreach (VCard::Email email, m_vcard->emailAddresses())
		{
			QString type;
			if (email.home)
				type = VCardConst::personalMailStatus();
			else if (email.work)
				type = VCardConst::workMailStatus();
			else
				type = VCardConst::emptyMailStatus();
			addEmail(jProtocol::fromStd(email.userid), type);
		}
		if (m_vcard->telephone().size())
			foreach (VCard::Telephone phone, m_vcard->telephone())
			{
				QString type;
				if (phone.home)
					type = VCardConst::homePhoneStatus();
				else if (phone.work)
					type = VCardConst::workPhoneStatus();
				else if (phone.cell)
					type = VCardConst::celluarPhoneStatus();
				else
					type = VCardConst::emptyPhoneStatus();
				addPhone(jProtocol::fromStd(phone.number), type);
			}
			if (m_vcard->addresses().size())
				foreach (VCard::Address address, m_vcard->addresses())
				{
					if (address.home)
					{
						if (jProtocol::fromStd(address.ctry) != "")
							addHomeCountry(jProtocol::fromStd(address.ctry));
						if (jProtocol::fromStd(address.region) != "")
							addHomeRegion(jProtocol::fromStd(address.region));
						if (jProtocol::fromStd(address.locality) != "")
							addHomeCity(jProtocol::fromStd(address.locality));
						if (jProtocol::fromStd(address.pcode) != "")
							addHomePostcode(jProtocol::fromStd(address.pcode));
						if (jProtocol::fromStd(address.street) != "")
							addHomeStreet(jProtocol::fromStd(address.street));
						if (jProtocol::fromStd(address.pobox) != "")
							addHomePostbox(jProtocol::fromStd(address.pobox));
					}
					if (address.work)
					{
						if (jProtocol::fromStd(address.ctry) != "")
							addWorkCountry(jProtocol::fromStd(address.ctry));
						if (jProtocol::fromStd(address.region) != "")
							addWorkRegion(jProtocol::fromStd(address.region));
						if (jProtocol::fromStd(address.locality) != "")
							addWorkCity(jProtocol::fromStd(address.locality));
						if (jProtocol::fromStd(address.pcode) != "")
							addWorkPostcode(jProtocol::fromStd(address.pcode));
						if (jProtocol::fromStd(address.street) != "")
							addWorkStreet(jProtocol::fromStd(address.street));
						if (jProtocol::fromStd(address.pobox) != "")
							addWorkPostbox(jProtocol::fromStd(address.pobox));
					}
				}
				if (jProtocol::fromStd(m_vcard->org().name) != "")
					addOrgName(jProtocol::fromStd(m_vcard->org().name));
				if (!m_vcard->org().units.empty() && jProtocol::fromStd(m_vcard->org().units.front()) != "")
					addOrgUnit(jProtocol::fromStd(m_vcard->org().units.front()));
				if (jProtocol::fromStd(m_vcard->title()) != "")
					addTitle(jProtocol::fromStd(m_vcard->title()));
				if (jProtocol::fromStd(m_vcard->role()) != "")
					addRole(jProtocol::fromStd(m_vcard->role()));
				delete m_vcard;
}

void jVCard::s_getPhoto()
{
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
			if ( iconFile.size() > (64 * 1024))
			{
				QMessageBox::warning(this, tr("Open error"), tr("Image size is too big"));
				//pixmap = pixmap.scaled(QSize(96,96),Qt::IgnoreAspectRatio,Qt::SmoothTransformation);
			}
			else
			{
				updatePhoto(fileName);
			}
		}
	}
}

void jVCard::updatePhoto(const QString &file)
{
	avatarLabel->setPhoto(file);
	avatarUrl = file;
	isAvatar = 1;
}

void jVCard::addName(const QString &name)
{
	nameLabel = new VCardRecord(m_mode, "name");
	connect(nameLabel, SIGNAL(mouseOver()), this, SLOT(showDeleteButton()));
	connect(nameLabel, SIGNAL(mouseOut()), this, SLOT(hideDeleteButton()));
	QFont font;
	font.setBold(true);
	font.setWeight(75);
	nameLabel->setFont(font);
	nameLabel->setText(name);
	ui.personalLayout->insertWidget(0, nameLabel);
	isName = 1;
	actionAddName->setEnabled(false);
}

void jVCard::addNick(const QString &nick)
{
	nickLabel = new VCardRecord(m_mode, "nick");
	connect(nickLabel, SIGNAL(mouseOver()), this, SLOT(showDeleteButton()));
	connect(nickLabel, SIGNAL(mouseOut()), this, SLOT(hideDeleteButton()));
	nickLabel->setText(nick);
	ui.personalLayout->insertWidget(isName, nickLabel);
	isNick = 1;
	actionAddNick->setEnabled(false);
}

void jVCard::addMiscBox()
{
	miscBox = new QGroupBox(tr(""));
	miscBox->setFlat(true);
	ui.userinfoLayout->insertWidget(1, miscBox);
	miscList = new QVBoxLayout();
	miscList->setSpacing(0);
	miscList->setMargin(0);
	miscBox->setLayout(miscList);
	isMisc = 1;
}


void jVCard::addBirthday(const QString &date)
{
	if (!isMisc)
		addMiscBox();
	birthdayLabel = new VCardBirthday(m_mode);
	connect(birthdayLabel, SIGNAL(mouseOver()), this, SLOT(showDeleteButton()));
	connect(birthdayLabel, SIGNAL(mouseOut()), this, SLOT(hideDeleteButton()));
	birthdayLabel->setDate(date);
	miscList->insertWidget(0, birthdayLabel);
	isBirthday = 1;
	actionAddBirthday->setEnabled(false);
}

void jVCard::addUrl(const QString &url)
{
	if (!isMisc)
		addMiscBox();
	urlLabel = new VCardRecord(m_mode, "url");
	connect(urlLabel, SIGNAL(mouseOver()), this, SLOT(showDeleteButton()));
	connect(urlLabel, SIGNAL(mouseOut()), this, SLOT(hideDeleteButton()));
	urlLabel->setText(url);
	miscList->insertWidget(isBirthday, urlLabel);
	isUrl = 1;
	actionAddUrl->setEnabled(false);
}

void jVCard::addEmailBox()
{
	emailBox = new QGroupBox(tr("E-mail"));
	emailBox->setFlat(true);
	ui.userinfoLayout->insertWidget(isMisc+1, emailBox);
	emailLayout = new QHBoxLayout();
	emailLayout->setSpacing(0);
	emailLayout->setMargin(0);
	emailBox->setLayout(emailLayout);
	emailList = new QVBoxLayout();
	emailList->setSpacing(0);
	emailList->setMargin(0);
	emailLayout->addLayout(emailList);
	QSpacerItem *horizontalSpacer = new QSpacerItem(283, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
	emailLayout->addItem(horizontalSpacer);
	isEmail = 1;
}

void jVCard::addEmail(const QString &address, const QString &type)
{
	int emailNum = emailAddresses.size();
	if (!emailNum)
		addEmailBox();
	VCardRecord *emailLabel = new VCardRecord(m_mode, "email");
	connect(emailLabel, SIGNAL(mouseOver()), this, SLOT(showDeleteButton()));
	connect(emailLabel, SIGNAL(mouseOut()), this, SLOT(hideDeleteButton()));
	emailLabel->setText(address);
	emailLabel->setStatus(type);
	emailList->addWidget(emailLabel);
	emailAddresses << emailLabel;
}

void jVCard::addPhoneBox()
{
	phoneBox = new QGroupBox(tr("Phone"));
	phoneBox->setFlat(true);
	ui.userinfoLayout->insertWidget(isMisc+isEmail+1, phoneBox);
	phoneLayout = new QHBoxLayout();
	phoneLayout->setSpacing(0);
	phoneLayout->setMargin(0);
	phoneBox->setLayout(phoneLayout);
	phoneList = new QVBoxLayout();
	phoneList->setSpacing(0);
	phoneList->setMargin(0);
	phoneLayout->addLayout(phoneList);
	QSpacerItem *horizontalSpacer = new QSpacerItem(283, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
	phoneLayout->addItem(horizontalSpacer);
	isPhone = 1;
}

void jVCard::addPhone(const QString &number, const QString &type)
{
	int phoneNum = phoneNumbers.size();
	if (!phoneNum)
		addPhoneBox();
	VCardRecord *phoneLabel = new VCardRecord(m_mode, "phone");
	connect(phoneLabel, SIGNAL(mouseOver()), this, SLOT(showDeleteButton()));
	connect(phoneLabel, SIGNAL(mouseOut()), this, SLOT(hideDeleteButton()));
	phoneLabel->setText(number);
	phoneLabel->setStatus(type);
	phoneList->addWidget(phoneLabel);
	phoneNumbers << phoneLabel;
}

void jVCard::addHomeBox()
{
	homeBox = new QGroupBox(tr("Home"));
	homeBox->setFlat(true);
	ui.userinfoLayout->insertWidget(isMisc+isEmail+isPhone+1, homeBox);
	homeLayout = new QHBoxLayout();
	homeLayout->setSpacing(0);
	homeLayout->setMargin(0);
	homeBox->setLayout(homeLayout);
	homeList = new QVBoxLayout();
	homeList->setSpacing(0);
	homeList->setMargin(0);
	homeLayout->addLayout(homeList);
	QSpacerItem *horizontalSpacer = new QSpacerItem(283, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
	homeLayout->addItem(horizontalSpacer);
	isHome = 1;
}

void jVCard::addWorkBox()
{
	workBox = new QGroupBox(tr("Work"));
	workBox->setFlat(true);
	ui.userinfoLayout->insertWidget(isMisc+isEmail+isPhone+isHome+1, workBox);
	workLayout = new QHBoxLayout();
	workLayout->setSpacing(0);
	workLayout->setMargin(0);
	workBox->setLayout(workLayout);
	workList = new QVBoxLayout();
	workList->setSpacing(0);
	workList->setMargin(0);
	workLayout->addLayout(workList);
	QSpacerItem *horizontalSpacer = new QSpacerItem(283, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
	workLayout->addItem(horizontalSpacer);
	isWork = 1;
}

void jVCard::addOrgName(const QString &name)
{
	if (!isWork)
		addWorkBox();
	orgnameLabel = new VCardRecord(m_mode, "orgname");
	connect(orgnameLabel, SIGNAL(mouseOver()), this, SLOT(showDeleteButton()));
	connect(orgnameLabel, SIGNAL(mouseOut()), this, SLOT(hideDeleteButton()));
	orgnameLabel->setText(name);
	workList->insertWidget(0, orgnameLabel);
	isOrgname = 1;
	actionAddWorkOrgname->setEnabled(false);
}

void jVCard::addOrgUnit(const QString &unit)
{
	if (!isWork)
		addWorkBox();
	orgunitLabel = new VCardRecord(m_mode, "orgunit");
	connect(orgunitLabel, SIGNAL(mouseOver()), this, SLOT(showDeleteButton()));
	connect(orgunitLabel, SIGNAL(mouseOut()), this, SLOT(hideDeleteButton()));
	orgunitLabel->setText(unit);
	workList->insertWidget(isOrgname, orgunitLabel);
	isOrgunit = 1;
	actionAddWorkOrgunit->setEnabled(false);
}

void jVCard::addTitle(const QString &title)
{
	if (!isWork)
		addWorkBox();
	titleLabel = new VCardRecord(m_mode, "title");
	connect(titleLabel, SIGNAL(mouseOver()), this, SLOT(showDeleteButton()));
	connect(titleLabel, SIGNAL(mouseOut()), this, SLOT(hideDeleteButton()));
	titleLabel->setText(title);
	workList->insertWidget(isOrgname+isOrgunit, titleLabel);
	isTitle = 1;
	actionAddWorkTitle->setEnabled(false);
}

void jVCard::addRole(const QString &role)
{
	if (!isWork)
		addWorkBox();
	roleLabel = new VCardRecord(m_mode, "role");
	connect(roleLabel, SIGNAL(mouseOver()), this, SLOT(showDeleteButton()));
	connect(roleLabel, SIGNAL(mouseOut()), this, SLOT(hideDeleteButton()));
	roleLabel->setText(role);
	workList->insertWidget(isOrgname+isOrgunit+isTitle, roleLabel);
	isRole = 1;
	actionAddWorkRole->setEnabled(false);
}

void jVCard::addHomeCountry(const QString &country)
{
	if (!isHome)
		addHomeBox();
	homeCountryLabel = new VCardRecord(m_mode, "homecountry");
	connect(homeCountryLabel, SIGNAL(mouseOver()), this, SLOT(showDeleteButton()));
	connect(homeCountryLabel, SIGNAL(mouseOut()), this, SLOT(hideDeleteButton()));
	homeCountryLabel->setText(country);
	homeList->insertWidget(0, homeCountryLabel);
	isHomeCountry = 1;
	actionAddHomeCountry->setEnabled(false);
}

void jVCard::addHomeRegion(const QString &region)
{
	if (!isHome)
		addHomeBox();
	homeRegionLabel = new VCardRecord(m_mode, "homeregion");
	connect(homeRegionLabel, SIGNAL(mouseOver()), this, SLOT(showDeleteButton()));
	connect(homeRegionLabel, SIGNAL(mouseOut()), this, SLOT(hideDeleteButton()));
	homeRegionLabel->setText(region);
	homeList->insertWidget(isHomeCountry, homeRegionLabel);
	isHomeRegion = 1;
	actionAddHomeRegion->setEnabled(false);
}

void jVCard::addHomeCity(const QString &city)
{
	if (!isHome)
		addHomeBox();
	homeCityLabel = new VCardRecord(m_mode, "homecity");
	connect(homeCityLabel, SIGNAL(mouseOver()), this, SLOT(showDeleteButton()));
	connect(homeCityLabel, SIGNAL(mouseOut()), this, SLOT(hideDeleteButton()));
	homeCityLabel->setText(city);
	homeList->insertWidget(isHomeCountry+isHomeRegion, homeCityLabel);
	isHomeCity = 1;
	actionAddHomeCity->setEnabled(false);
}

void jVCard::addHomePostcode(const QString &pcode)
{
	if (!isHome)
		addHomeBox();
	homePostcodeLabel = new VCardRecord(m_mode, "homepostcode");
	connect(homePostcodeLabel, SIGNAL(mouseOver()), this, SLOT(showDeleteButton()));
	connect(homePostcodeLabel, SIGNAL(mouseOut()), this, SLOT(hideDeleteButton()));
	homePostcodeLabel->setText(pcode);
	homeList->insertWidget(isHomeCountry+isHomeRegion+isHomeCity, homePostcodeLabel);
	isHomePostcode = 1;
	actionAddHomePostcode->setEnabled(false);
}

void jVCard::addHomeStreet(const QString &street)
{
	if (!isHome)
		addHomeBox();
	homeStreetLabel = new VCardRecord(m_mode, "homestreet");
	connect(homeStreetLabel, SIGNAL(mouseOver()), this, SLOT(showDeleteButton()));
	connect(homeStreetLabel, SIGNAL(mouseOut()), this, SLOT(hideDeleteButton()));
	homeStreetLabel->setText(street);
	homeList->insertWidget(isHomeCountry+isHomeRegion+isHomePostcode+isHomeCity, homeStreetLabel);
	isHomeStreet = 1;
	actionAddHomeStreet->setEnabled(false);
}

void jVCard::addHomePostbox(const QString &pbox)
{
	if (!isHome)
		addHomeBox();
	homePostboxLabel = new VCardRecord(m_mode, "homepostbox");
	connect(homePostboxLabel, SIGNAL(mouseOver()), this, SLOT(showDeleteButton()));
	connect(homePostboxLabel, SIGNAL(mouseOut()), this, SLOT(hideDeleteButton()));
	homePostboxLabel->setText(pbox);
	homeList->insertWidget(isHomeCountry+isHomeRegion+isHomePostcode+isHomeCity+isHomeStreet, homePostboxLabel);
	isHomePostbox = 1;
	actionAddHomePostbox->setEnabled(false);
}

void jVCard::addWorkCountry(const QString &country)
{
	if (!isWork)
		addWorkBox();
	workCountryLabel = new VCardRecord(m_mode, "workcountry");
	connect(workCountryLabel, SIGNAL(mouseOver()), this, SLOT(showDeleteButton()));
	connect(workCountryLabel, SIGNAL(mouseOut()), this, SLOT(hideDeleteButton()));
	workCountryLabel->setText(country);
	workList->insertWidget(isOrgname+isOrgunit+isTitle+isRole, workCountryLabel);
	isWorkCountry = 1;
	actionAddWorkCountry->setEnabled(false);
}

void jVCard::addWorkRegion(const QString &region)
{
	if (!isWork)
		addWorkBox();
	workRegionLabel = new VCardRecord(m_mode, "workregion");
	connect(workRegionLabel, SIGNAL(mouseOver()), this, SLOT(showDeleteButton()));
	connect(workRegionLabel, SIGNAL(mouseOut()), this, SLOT(hideDeleteButton()));
	workRegionLabel->setText(region);
	workList->insertWidget(isOrgname+isOrgunit+isTitle+isRole+isWorkCountry, workRegionLabel);
	isWorkRegion = 1;
	actionAddWorkRegion->setEnabled(false);
}

void jVCard::addWorkCity(const QString &city)
{
	if (!isWork)
		addWorkBox();
	workCityLabel = new VCardRecord(m_mode, "workcity");
	connect(workCityLabel, SIGNAL(mouseOver()), this, SLOT(showDeleteButton()));
	connect(workCityLabel, SIGNAL(mouseOut()), this, SLOT(hideDeleteButton()));
	workCityLabel->setText(city);
	workList->insertWidget(isOrgname+isOrgunit+isTitle+isRole+isWorkCountry+isWorkRegion, workCityLabel);
	isWorkCity = 1;
	actionAddWorkCity->setEnabled(false);
}

void jVCard::addWorkPostcode(const QString &pcode)
{
	if (!isWork)
		addWorkBox();
	workPostcodeLabel = new VCardRecord(m_mode, "workpostcode");
	connect(workPostcodeLabel, SIGNAL(mouseOver()), this, SLOT(showDeleteButton()));
	connect(workPostcodeLabel, SIGNAL(mouseOut()), this, SLOT(hideDeleteButton()));
	workPostcodeLabel->setText(pcode);
	workList->insertWidget(isOrgname+isOrgunit+isTitle+isRole+isWorkCountry+isWorkRegion+isWorkCity, workPostcodeLabel);
	isWorkPostcode = 1;
	actionAddWorkPostcode->setEnabled(false);
}

void jVCard::addWorkStreet(const QString &street)
{
	if (!isWork)
		addWorkBox();
	workStreetLabel = new VCardRecord(m_mode, "workstreet");
	connect(workStreetLabel, SIGNAL(mouseOver()), this, SLOT(showDeleteButton()));
	connect(workStreetLabel, SIGNAL(mouseOut()), this, SLOT(hideDeleteButton()));
	workStreetLabel->setText(street);
	workList->insertWidget(isOrgname+isOrgunit+isTitle+isRole+isWorkCountry+isWorkRegion+isWorkPostcode+isWorkCity, workStreetLabel);
	isWorkStreet = 1;
	actionAddWorkStreet->setEnabled(false);
}

void jVCard::addWorkPostbox(const QString &pbox)
{
	if (!isWork)
		addWorkBox();
	workPostboxLabel = new VCardRecord(m_mode, "workpostbox");
	connect(workPostboxLabel, SIGNAL(mouseOver()), this, SLOT(showDeleteButton()));
	connect(workPostboxLabel, SIGNAL(mouseOut()), this, SLOT(hideDeleteButton()));
	workPostboxLabel->setText(pbox);
	workList->insertWidget(isOrgname+isOrgunit+isTitle+isRole+isWorkCountry+isWorkRegion+isWorkPostcode+isWorkCity+isWorkStreet, workPostboxLabel);
	isWorkPostbox = 1;
	actionAddWorkPostbox->setEnabled(false);
}

void jVCard::showDeleteButton()
{
	hintLayout = qobject_cast<VCardEntry*>(sender());
	hintLayout->addDeleteButton(ui.deleteButton);
}

void jVCard::hideDeleteButton()
{
	VCardEntry *layout = qobject_cast<VCardEntry*>(sender());
	if (layout == hintLayout)
		ui.deleteButton->setParent(personalWidget);
}

void jVCard::deleteEntry()
{
	ui.deleteButton->setVisible(false);
	ui.deleteButton->setParent(personalWidget);

		QString type = hintLayout->type();
		if (type != "avatar")
			delete hintLayout;
		if (type == "avatar")
		{
			qobject_cast<VCardAvatar*>(hintLayout)->setPhoto("");
			isAvatar = 0;
		}
		else if (type == "name")
		{
			isName = 0;
			actionAddName->setEnabled(true);
		}
		else if (type == "nick")
		{
			isNick = 0;
			actionAddNick->setEnabled(true);
		}
		else if (type == "birthday")
		{
			isBirthday = 0;
			actionAddBirthday->setEnabled(true);
			checkMiscEntry();
		}
		else if (type == "url")
		{
			isUrl = 0;
			actionAddUrl->setEnabled(true);
			checkMiscEntry();
		}
		else if (type == "email")
		{
			emailAddresses.removeOne((VCardRecord*) hintLayout);
			if (emailAddresses.empty())
			{
				isEmail = 0;
				delete emailBox;
			}
		}
		else if (type == "phone")
		{
			phoneNumbers.removeOne((VCardRecord*) hintLayout);
			if (phoneNumbers.empty())
			{
				isPhone = 0;
				delete phoneBox;
			}
		}
		else if (type == "orgname")
		{
			isOrgname = 0;
			actionAddWorkOrgname->setEnabled(true);
			checkWorkEntry();
		}
		else if (type == "orgunit")
		{
			isOrgunit = 0;
			actionAddWorkOrgunit->setEnabled(true);
			checkWorkEntry();
		}
		else if (type == "title")
		{
			isTitle = 0;
			actionAddWorkTitle->setEnabled(true);
			checkWorkEntry();
		}
		else if (type == "role")
		{
			isRole = 0;
			actionAddWorkRole->setEnabled(true);
			checkWorkEntry();
		}
		else if (type == "workcountry")
		{
			isWorkCountry = 0;
			actionAddWorkCountry->setEnabled(true);
			checkWorkEntry();
		}
		else if (type == "workregion")
		{
			isWorkRegion = 0;
			actionAddWorkRegion->setEnabled(true);
			checkWorkEntry();
		}
		else if (type == "workcity")
		{
			isWorkCity = 0;
			actionAddWorkCity->setEnabled(true);
			checkWorkEntry();
		}
		else if (type == "workpostcode")
		{
			isWorkPostcode = 0;
			actionAddWorkPostcode->setEnabled(true);
			checkWorkEntry();
		}
		else if (type == "workstreet")
		{
			isWorkStreet = 0;
			actionAddWorkStreet->setEnabled(true);
			checkWorkEntry();
		}
		else if (type == "workpostbox")
		{
			isWorkPostbox = 0;
			actionAddWorkPostbox->setEnabled(true);
			checkWorkEntry();
		}
		else if (type == "homecountry")
		{
			isHomeCountry = 0;
			actionAddHomeCountry->setEnabled(true);
			checkHomeEntry();
		}
		else if (type == "homeregion")
		{
			isHomeRegion = 0;
			actionAddHomeRegion->setEnabled(true);
			checkHomeEntry();
		}
		else if (type == "homecity")
		{
			isHomeCity = 0;
			actionAddHomeCity->setEnabled(true);
			checkHomeEntry();
		}
		else if (type == "homepostcode")
		{
			isHomePostcode = 0;
			actionAddHomePostcode->setEnabled(true);
			checkHomeEntry();
		}
		else if (type == "homestreet")
		{
			isHomeStreet = 0;
			actionAddHomeStreet->setEnabled(true);
			checkHomeEntry();
		}
		else if (type == "homepostbox")
		{
			isHomePostbox = 0;
			actionAddHomePostbox->setEnabled(true);
			checkHomeEntry();
		}

}

void jVCard::checkMiscEntry()
{
	if (isBirthday+isUrl == 0)
	{
		isMisc = 0;
		delete miscBox;
	}
}

void jVCard::checkHomeEntry()
{
	if (isHomeCountry + isHomeRegion + isHomeCity + isHomePostcode + isHomeStreet + isHomePostbox == 0)
	{
		isHome = 0;
		delete homeBox;
	}
}

void jVCard::checkWorkEntry()
{
	if (isOrgname + isOrgunit + isTitle + isRole + isWorkCountry + isWorkRegion + isWorkCity + isWorkPostcode + isWorkStreet + isWorkPostbox == 0)
	{
		isWork = 0;
		delete workBox;
	}
}

void jVCard::s_saveVCard()
{
	VCard *vcard = new VCard();
	if (isAvatar)
	{
		QFile file(avatarUrl);
		if(file.open(QIODevice::ReadOnly))
		{
			QByteArray data = file.readAll();
			const std::string std_data (data.data(),data.size());
			file.close();
			vcard->setPhoto(jProtocol::toStd("undefined"), std_data);
		}
	}
	if (isName && nameLabel->text() != VCardConst::emptyString())
		vcard->setFormattedname(jProtocol::toStd(nameLabel->text()));
	if (isNick && nickLabel->text() != VCardConst::emptyString())
		vcard->setNickname(jProtocol::toStd(nickLabel->text()));
	if (isBirthday && birthdayLabel->date() != VCardConst::emptyString())
		vcard->setBday(jProtocol::toStd(birthdayLabel->date()));
	if (isUrl && urlLabel->text() != VCardConst::emptyString())
		vcard->setUrl(jProtocol::toStd(urlLabel->text()));
	if (isEmail)
	{
		int emailCount = emailAddresses.count();
		for (int emailNum = 0; emailNum < emailCount; emailNum++)
			if (emailAddresses[emailNum] && emailAddresses[emailNum]->text() != VCardConst::emptyString())
			{
				QString email = emailAddresses[emailNum]->text();
				QString status = emailAddresses[emailNum]->status();
				int type;
				if (status == VCardConst::personalMailStatus())
					type = VCard::AddrTypeHome;
				else if (status == VCardConst::workMailStatus())
					type = VCard::AddrTypeWork;
				else
					type = 0;
				vcard->addEmail(jProtocol::toStd(email), type);
			}
	}
	if (isPhone)
	{
		int phoneCount = phoneNumbers.count();
		for (int phoneNum = 0; phoneNum < phoneCount; phoneNum++)
			if (phoneNumbers[phoneNum] && phoneNumbers[phoneNum]->text() != VCardConst::emptyString())
			{
				QString phone = phoneNumbers[phoneNum]->text();
				QString status = phoneNumbers[phoneNum]->status();
				int type;
				if (status == VCardConst::homePhoneStatus())
					type = VCard::AddrTypeHome;
				else if (status == VCardConst::workPhoneStatus())
					type = VCard::AddrTypeWork;
				else if (status == VCardConst::celluarPhoneStatus())
					type = VCard::AddrTypeCell;
				else
					type = 0;
				vcard->addTelephone(jProtocol::toStd(phone), type);
			}
	}
	if (isHomeCountry+isHomeRegion+isHomeCity+isHomePostcode+isHomeStreet+isHomePostbox)
	{
		QString country;
		QString region = "";
		QString city = "";
		QString pcode = "";
		QString street = "";
		QString pbox = "";
		if (isHomeCountry && homeCountryLabel->text() != VCardConst::emptyString())
			country = homeCountryLabel->text();
		if (isHomeRegion && homeRegionLabel->text() != VCardConst::emptyString())
			region = homeRegionLabel->text();
		if (isHomeCity && homeCityLabel->text() != VCardConst::emptyString())
			city = homeCityLabel->text();
		if (isHomePostcode && homePostcodeLabel->text() != VCardConst::emptyString())
			pcode = homePostcodeLabel->text();
		if (isHomeStreet && homeStreetLabel->text() != VCardConst::emptyString())
			street = homeStreetLabel->text();
		if (isHomePostbox && homePostboxLabel->text() != VCardConst::emptyString())
			pbox = homePostboxLabel->text();
		vcard->addAddress(jProtocol::toStd(pbox), jProtocol::toStd(""), jProtocol::toStd(street), jProtocol::toStd(city), jProtocol::toStd(region), jProtocol::toStd(pcode), jProtocol::toStd(country), VCard::AddrTypeHome);
	}
	if (isWorkCountry+isWorkRegion+isWorkCity+isWorkPostcode+isWorkStreet+isWorkPostbox)
	{
		QString country = "";
		QString region = "";
		QString city = "";
		QString pcode = "";
		QString street = "";
		QString pbox = "";
		if (isWorkCountry && workCountryLabel->text() != VCardConst::emptyString())
			country = workCountryLabel->text();
		if (isWorkRegion && workRegionLabel->text() != VCardConst::emptyString())
			region = workRegionLabel->text();
		if (isWorkCity && workCityLabel->text() != VCardConst::emptyString())
			city = workCityLabel->text();
		if (isWorkPostcode && workPostcodeLabel->text() != VCardConst::emptyString())
			pcode = workPostcodeLabel->text();
		if (isWorkStreet && workStreetLabel->text() != VCardConst::emptyString())
			street = workStreetLabel->text();
		if (isWorkPostbox && workPostboxLabel->text() != VCardConst::emptyString())
			pbox = workPostboxLabel->text();
		vcard->addAddress(jProtocol::toStd(pbox), jProtocol::toStd(""), jProtocol::toStd(street), jProtocol::toStd(city), jProtocol::toStd(region), jProtocol::toStd(pcode), jProtocol::toStd(country), VCard::AddrTypeWork);
	}
	if (isOrgname+isOrgunit)
	{
		QString name = "";
		if (isOrgname && orgnameLabel->text() != VCardConst::emptyString())
			name = orgnameLabel->text();
		gloox::StringList units;
		if (isOrgunit && orgunitLabel->text() != VCardConst::emptyString())
			units.assign(1, jProtocol::toStd(orgunitLabel->text()));
		vcard->setOrganization(jProtocol::toStd(name), units);
	}
	if (isTitle && titleLabel->text() != VCardConst::emptyString())
		vcard->setTitle(jProtocol::toStd(titleLabel->text()));
	if (isRole && roleLabel->text() != VCardConst::emptyString())
		vcard->setRole(jProtocol::toStd(roleLabel->text()));
	emit saveVCard(vcard);
}
*/
