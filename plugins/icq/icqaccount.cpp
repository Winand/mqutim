/*
    icqAccount

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

#include <QtGui>

#include "oscarprotocol.h"
#include "customstatusdialog.h"
#include "contactlist.h"
#include "icqaccount.h"
#include "filetransfer.h"


icqAccount::icqAccount(QString string, const QString &profile_name, QObject *parent)
	: QObject(parent)
	, icqUin(string)
	, m_profile_name(profile_name)
	, m_icq_plugin_system(IcqPluginSystem::instance())
{
	currentTrayStatus = false;
	statusTrayMenuExist = false;
	deleteingAccount = false;
	menuExist = false;
	firsTrayMessageIsShown = false;
	positionInStack = 1;
	currentXstatus = 0;
	statusIconIndex = 0;
	iAmConnected = false;
	thisIcqProtocol = new oscarProtocol(icqUin, m_profile_name, this);
	connect(thisIcqProtocol, SIGNAL(statusChanged(accountStatus)),
			this, SLOT(setStatusIcon(accountStatus)));
	connect(thisIcqProtocol, SIGNAL(statusChanged(accountStatus)),
			this, SLOT(onOscarStatusChanged(accountStatus)));

	connect(thisIcqProtocol, SIGNAL(accountConnected(bool)),
			this, SLOT(accountConnected(bool)));
	connect(thisIcqProtocol, SIGNAL(systemMessage(const QString &)),
			this, SLOT(systemMessage(const QString &)));
	connect(thisIcqProtocol, SIGNAL(userMessage(const QString &, const QString &, const QString &, userMessageType, bool)),
				this, SLOT(userMessage(const QString &, const QString &, const QString &, userMessageType, bool)));
	connect(thisIcqProtocol, SIGNAL(getNewMessage()),
                this, SIGNAL(getNewMessage()));
	connect(thisIcqProtocol, SIGNAL(readAllMessages()),
						this, SIGNAL(readAllMessages()));

	connect(thisIcqProtocol->getContactListClass(), SIGNAL(updateStatusMenu(bool)),
						this, SLOT(updateStatusMenu(bool)));
	connect(this, SIGNAL(updateTranslation()),
		thisIcqProtocol, SIGNAL(updateTranslation()));
//	connect(this, SIGNAL(soundSettingsChanged()),
//		getProtocol()->getContactListClass(),
//		SIGNAL(soundSettingsUpdated()));


	createIcons();
	createStatusMenu();

	chooseStatus = new QAction(currentIcon, icqUin, this);
	chooseStatus->setCheckable(true);
	connect ( chooseStatus, SIGNAL(triggered()),
			this, SLOT(emitChangeStatus()));

	loadAccountSettings();
        QSettings settings(QSettings::NativeFormat, QSettings::UserScope, "qutim/qutim."+m_profile_name, "icqsettings");
	configPath = settings.fileName().section('/', 0, -2);
	
	createMenuAccount();
//	plugins = new pluginSystem(icqUin, this);
//	thisIcqProtocol->getContactListClass()->plugins = plugins;

//	createContacts();
		m_restore_xstatus_num = -10;
		m_restore_status = restoreAccount;
}

icqAccount::~icqAccount()
{
//	delete thisIcqProtocol;
	delete accountLineButton;
	delete privacyStatus;
	delete m_account_additional_menu;
	delete statusMenu;
//	delete m_account_menu;
}

void icqAccount::createAccountButton(QHBoxLayout *boxLayout)
{
	accountLineButton = new accountButton;
#if defined(Q_OS_MAC)
	boxLayout->addWidget(accountLineButton, 0, Qt::AlignLeft);
#else
	boxLayout->addWidget(accountLineButton, 0, Qt::AlignRight);
#endif
//	boxLayout->addWidget(accountLineButton, 0, index);
        accountLineButton->setWhatsThis(icqUin);
	accountLineButton->setIcon(currentIcon);
	accountLineButton->setPopupMode(QToolButton::InstantPopup);
	accountLineButton->setMenu(statusMenu);
}

//void icqAccount::createMenuAccount(QMenu *menu, QAction *before)
//{
//	accountAction = menu->insertMenu(before, statusMenu);
//	menuExist = true;
//}

void icqAccount::createMenuAccount()
{
//	m_account_menu = new QMenu(icqUin);
//	m_account_menu->setIcon(currentIcon);
//	m_account_menu->addMenu(statusMenu);
	menuExist = true;
}

void icqAccount::removeMenuAccount(QMenu *menu)
{
	menuExist = false;
	menu->removeAction(accountAction);
}

void icqAccount::removeAccountButton()
{
	delete accountLineButton;
}

void icqAccount::createIcons()
{
	/*onlineIcon = new QIcon(statusIconClass::getInstance()->getOnlineIcon());
	offlineIcon = new QIcon(statusIconClass::getInstance()->getOfflineIcon());
	ffcIcon = new QIcon(statusIconClass::getInstance()->getFreeForChatIcon());
	awayIcon = new QIcon(statusIconClass::getInstance()->getAwayIcon());
	naIcon = new QIcon(statusIconClass::getInstance()->getNotAvailableIcon());
	occupiedIcon = new QIcon(statusIconClass::getInstance()->getOccupiedIcon());
	dndIcon = new QIcon(statusIconClass::getInstance()->getDoNotDisturbIcon());
	invisibleIcon = new QIcon(statusIconClass::getInstance()->getInvisibleIcon());
	connectingIcon = new QIcon(statusIconClass::getInstance()->getConnectingIcon());
	currentIcon = new QIcon(statusIconClass::getInstance()->getOfflineIcon());

	currentIconPath = statusIconClass::getInstance()->getIconFactory()->getOfflinePath();

	lunchIcon = new QIcon(statusIconClass::getInstance()->getLunchIcon());
	evilIcon = new QIcon(statusIconClass::getInstance()->getEvilIcon());
	depressionIcon = new QIcon(statusIconClass::getInstance()->getDepressionIcon());
	atHomeIcon = new QIcon(statusIconClass::getInstance()->getAtHomeIcon());
	atWorkIcon = new QIcon(statusIconClass::getInstance()->getAtWorkIcon());
	*/
	currentIcon = m_icq_plugin_system.getStatusIcon("offline", "icq");
	currentIconPath = m_icq_plugin_system.getStatusIconFileName("offline", "icq");
}
void icqAccount::createStatusMenu()
{

        QSettings settings(QSettings::NativeFormat, QSettings::UserScope, "qutim/qutim."+m_profile_name, "icqsettings");
        QSettings account_settings(QSettings::NativeFormat, QSettings::UserScope, "qutim/qutim."+m_profile_name+"/ICQ."+icqUin, "accountsettings");
	showCustomStatus = settings.value("statuses/customstat",true).toBool();
	onlineAction = new QAction(m_icq_plugin_system.getStatusIcon("online", "icq"), tr("Online"), this);
		connect(onlineAction, SIGNAL(triggered()), this, SLOT(setStatus()));
		statusMenuActions.push_back(onlineAction);

		offlineAction = new QAction(m_icq_plugin_system.getStatusIcon("offline", "icq"), tr("Offline"), this);
		connect(offlineAction, SIGNAL(triggered()), this, SLOT(setStatus()));
		statusMenuActions.push_back(offlineAction);

		ffcAction = new QAction(m_icq_plugin_system.getStatusIcon("ffc", "icq"), tr("Free for chat"), this);
		connect(ffcAction, SIGNAL(triggered()), this, SLOT(setStatus()));
		statusMenuActions.push_back(ffcAction);

		awayAction = new QAction(m_icq_plugin_system.getStatusIcon("away", "icq"), tr("Away"), this);
		connect(awayAction, SIGNAL(triggered()), this, SLOT(setStatus()));
		statusMenuActions.push_back(awayAction);

		naAction = new QAction(m_icq_plugin_system.getStatusIcon("na", "icq"), tr("NA"), this);
		connect(naAction, SIGNAL(triggered()), this, SLOT(setStatus()));
		statusMenuActions.push_back(naAction);

		occupiedAction = new QAction(m_icq_plugin_system.getStatusIcon("occupied", "icq"), tr("Occupied"), this);
		connect(occupiedAction, SIGNAL(triggered()), this, SLOT(setStatus()));
		statusMenuActions.push_back(occupiedAction);

		dndAction = new QAction(m_icq_plugin_system.getStatusIcon("dnd", "icq"), tr("DND"), this);
		connect(dndAction, SIGNAL(triggered()), this, SLOT(setStatus()));
		statusMenuActions.push_back(dndAction);

		invisibleAction = new QAction(m_icq_plugin_system.getStatusIcon("invisible", "icq"), tr("Invisible"), this);
		connect(invisibleAction, SIGNAL(triggered()), this, SLOT(setStatus()));
		statusMenuActions.push_back(invisibleAction);

		lunchAction = new QAction(m_icq_plugin_system.getStatusIcon("lunch", "icq"), tr("Lunch"), this);
		connect(lunchAction, SIGNAL(triggered()), this, SLOT(setStatus()));
		statusMenuActions.push_back(lunchAction);

		evilAction = new QAction(m_icq_plugin_system.getStatusIcon("evil", "icq"), tr("Evil"), this);
		connect(evilAction, SIGNAL(triggered()), this, SLOT(setStatus()));
		statusMenuActions.push_back(evilAction);

		depressionAction = new QAction(m_icq_plugin_system.getStatusIcon("depression", "icq"), tr("Depression"), this);
		connect(depressionAction, SIGNAL(triggered()), this, SLOT(setStatus()));
		statusMenuActions.push_back(depressionAction);

		atHomeAction = new QAction(m_icq_plugin_system.getStatusIcon("athome", "icq"), tr("At Home"), this);
		connect(atHomeAction, SIGNAL(triggered()), this, SLOT(setStatus()));
		statusMenuActions.push_back(atHomeAction);

		atWorkAction = new QAction(m_icq_plugin_system.getStatusIcon("atwork", "icq"), tr("At Work"), this);
		connect(atWorkAction, SIGNAL(triggered()), this, SLOT(setStatus()));
		statusMenuActions.push_back(atWorkAction);

	// Set all of status actions as unchecked
	QVectorIterator<QAction *> iterator(statusMenuActions);
	while (iterator.hasNext())
	{
		iterator.next()->setCheckable(true);
	}

	customStatus = new QAction(tr("Custom status"), this);
	connect(customStatus, SIGNAL(triggered()), this, SLOT(customStatusTriggered()));

	statusMenu = new QMenu;
	privacyStatus = new QMenu(tr("Privacy status"));
	privacyStatus->setIcon(IcqPluginSystem::instance().getIcon("privacy"));

	privacyGroup = new QActionGroup(this);

	visibleForAll = new QAction(tr("Visible for all"), this);
	visibleForAll->setCheckable(true);
	privacyGroup->addAction(visibleForAll);
	connect(visibleForAll, SIGNAL(triggered()), this, SLOT(setVisibleForAll()));

	visibleForVis = new QAction(tr("Visible only for visible list"), this);
	visibleForVis->setCheckable(true);
	privacyGroup->addAction(visibleForVis);
	connect(visibleForVis, SIGNAL(triggered()), this, SLOT(setVisibleForVis()));

	notVisibleForInv = new QAction(tr("Invisible only for invisible list"), this);
	notVisibleForInv->setCheckable(true);
	privacyGroup->addAction(notVisibleForInv);
	connect(notVisibleForInv, SIGNAL(triggered()), this, SLOT(setNotVisibleForInv()));

	visibleForContact = new QAction(tr("Visible only for contact list"), this);
	visibleForContact->setCheckable(true);
	privacyGroup->addAction(visibleForContact);
	connect(visibleForContact, SIGNAL(triggered()), this, SLOT(setVisibleForContact()));

	invisibleForAll = new QAction(tr("Invisible for all"), this);
	invisibleForAll->setCheckable(true);
	privacyGroup->addAction(invisibleForAll);
	connect(invisibleForAll, SIGNAL(triggered()), this, SLOT(setInvisibleForAll()));

	privacyStatus->addAction(visibleForAll);
	privacyStatus->addAction(visibleForVis);
	privacyStatus->addAction(notVisibleForInv);
	privacyStatus->addAction(visibleForContact);
	privacyStatus->addAction(invisibleForAll);



	statusMenu->setTitle(icqUin);
	statusMenu->setIcon(currentIcon);
	statusMenu->addAction(onlineAction);
	statusMenu->addAction(ffcAction);
	statusMenu->addAction(awayAction);

	statusMenu->addAction(lunchAction);
	statusMenu->addAction(evilAction);
	statusMenu->addAction(depressionAction);
	statusMenu->addAction(atHomeAction);
	statusMenu->addAction(atWorkAction);

	statusMenu->addAction(naAction);
	statusMenu->addAction(occupiedAction);
	statusMenu->addAction(dndAction);
	statusMenu->addAction(invisibleAction);
	statusMenu->addSeparator();
	statusMenu->addAction(customStatus);
	statusMenu->addSeparator();
	statusMenu->addMenu(privacyStatus);
	m_account_additional_menu = new QMenu(tr("Additional"));
	statusMenu->addMenu(m_account_additional_menu);
	thisIcqProtocol->getContactListClass()->initializaMenus(m_account_additional_menu);
	statusMenu->addAction(offlineAction);

	updateStatusMenu(showCustomStatus);



	quint32 privacy = account_settings.value("statuses/privacy", 4).toUInt();

	switch( privacy )
	{
	case 1:
		visibleForAll->setChecked(true);
		break;
	case 2:
		visibleForVis->setChecked(true);
		break;
	case 3:
		notVisibleForInv->setChecked(true);
		break;
	case 4:
		visibleForContact->setChecked(true);
		break;
	case 5:
		invisibleForAll->setChecked(true);
		break;
	default:
		visibleForContact->setChecked(true);
	}


	
}


void icqAccount::setStatusIcon(accountStatus status)
{
	switch (status)
	{
	case online:
		currentIcon = m_icq_plugin_system.getStatusIcon("online", "icq");
		currentIconPath = m_icq_plugin_system.getStatusIconFileName("offline", "icq");
		break;

	case ffc:
		currentIcon = m_icq_plugin_system.getStatusIcon("ffc", "icq");
		currentIconPath = m_icq_plugin_system.getStatusIconFileName("ffc", "icq");
		break;

	case away:
		currentIcon = m_icq_plugin_system.getStatusIcon("away", "icq");;
		currentIconPath = m_icq_plugin_system.getStatusIconFileName("away", "icq");
		break;

	case na:
		currentIcon = m_icq_plugin_system.getStatusIcon("na", "icq");;
		currentIconPath = m_icq_plugin_system.getStatusIconFileName("na", "icq");
		break;

	case occupied:
		currentIcon = m_icq_plugin_system.getStatusIcon("occupied", "icq");;
		currentIconPath = m_icq_plugin_system.getStatusIconFileName("occupied", "icq");
		break;

	case dnd:
		currentIcon = m_icq_plugin_system.getStatusIcon("dnd", "icq");;
		currentIconPath = m_icq_plugin_system.getStatusIconFileName("dnd", "icq");
		break;

	case invisible:
		currentIcon = m_icq_plugin_system.getStatusIcon("invisible", "icq");;
		currentIconPath = m_icq_plugin_system.getStatusIconFileName("invisible", "icq");
		break;

	case offline:
		currentIcon = m_icq_plugin_system.getStatusIcon("offline", "icq");
		currentIconPath = m_icq_plugin_system.getStatusIconFileName("offline", "icq");
		iAmConnected = false;
//              emit playSoundEvent(SoundEvent::Disconnected, getStatus());
		break;

	case connecting:
		currentIcon = m_icq_plugin_system.getStatusIcon("connecting", "icq");
		currentIconPath = m_icq_plugin_system.getStatusIconFileName("connecting", "icq");
		break;

	case lunch:
		currentIcon = m_icq_plugin_system.getStatusIcon("lunch", "icq");
		currentIconPath = m_icq_plugin_system.getStatusIconFileName("lunch", "icq");
		break;

	case evil:
		currentIcon = m_icq_plugin_system.getStatusIcon("evil", "icq");;
		currentIconPath = m_icq_plugin_system.getStatusIconFileName("evil", "icq");
		break;

	case depression:
		currentIcon = m_icq_plugin_system.getStatusIcon("depression", "icq");;
		currentIconPath = m_icq_plugin_system.getStatusIconFileName("depression", "icq");
		break;

	case athome:
		currentIcon = m_icq_plugin_system.getStatusIcon("athome", "icq");
		currentIconPath = m_icq_plugin_system.getStatusIconFileName("athome", "icq");
		break;
	case atwork:
		currentIcon = m_icq_plugin_system.getStatusIcon("atwork", "icq");;
		currentIconPath = m_icq_plugin_system.getStatusIconFileName("atwork", "icq");
		break;

	default:
		break;
	}

	if ( statusIconIndex != 1)
	{
		updateIconStatus();
		emit updateTrayToolTip();
	} else
	{
		if ( !currentXstatus || status==offline)
		{
			updateIconStatus();
			emit updateTrayToolTip();
		} else if (iAmConnected){
			currentIconPath = statusIconClass::getInstance()->xstatusList.at(currentXstatus - 1);
			currentIcon = QIcon(currentIconPath);
			updateIconStatus();

			emit updateTrayToolTip();
		}



	}
}

void icqAccount::onOscarStatusChanged(accountStatus status)
{
	// Set all of status actions as unchecked
	QVectorIterator<QAction *> iterator(statusMenuActions);
	while (iterator.hasNext())
	{
		iterator.next()->setChecked(false);
	}

	if (status == offline)
	{
		offlineAction->setChecked(true);
	}
	else if (status == online)
	{
		onlineAction->setChecked(true);
	}
	else if (status == ffc)
	{
		ffcAction->setChecked(true);
	}
	else if (status == away)
	{
		awayAction->setChecked(true);
	}
	else if (status == na)
	{
		naAction->setChecked(true);
	}
	else if (status == occupied)
	{
		occupiedAction->setChecked(true);
	}
	else if (status == dnd)
	{
		dndAction->setChecked(true);
	}
	else if (status == invisible)
	{
		invisibleAction->setChecked(true);
	}
	else if (status == lunch)
	{
		lunchAction->setChecked(true);
	}
	else if (status ==  evil)
	{
		evilAction->setChecked(true);
	}
	else if (status == depression)
	{
		depressionAction->setChecked(true);
	}
	else if (status == athome)
	{
		atHomeAction->setChecked(true);
	}
	else if (status == atwork)
	{
		atWorkAction->setChecked(true);
	}
}

void icqAccount::setStatus()
// shold not be used directly!!!
// use statusAction->trigger() if you have to
// call setStatus() slot
{
	// default status
	accountStatus status = online;
	// Getting triggered action
	QAction *act = qobject_cast<QAction *>(sender());

	// Set all of status actions as unchecked
	QVectorIterator<QAction *> iterator(statusMenuActions);
	while (iterator.hasNext())
	{
		iterator.next()->setChecked(false);
	}

	// Set button checked
	//act->setChecked(true);

	// setting status
	if (act == offlineAction)status = offline;
	else if (act == ffcAction) status = ffc;
	else if (act == awayAction) status = away;
	else if (act == naAction) status = na;
	else if (act == occupiedAction) status = occupied;
	else if (act == dndAction) status = dnd;
	else if (act == invisibleAction) status = invisible;
	else if (act == lunchAction) status = lunch;
	else if (act == evilAction) status = evil;
	else if (act == depressionAction) status = depression;
	else if (act == atHomeAction) status = athome;
	else if (act == atWorkAction) status = atwork;
	m_restore_status = status;
	// Saving settings routine
	if ((status != online) && (status != offline) && (status != ffc)
		&& (status != invisible))
	{

		QString sName(StatusNames[static_cast<int>(status)]);
                QSettings settings(QSettings::NativeFormat, QSettings::UserScope, "qutim/qutim."+m_profile_name, "icqsettings");
                QSettings account_settings(QSettings::NativeFormat, QSettings::UserScope, "qutim/qutim."+m_profile_name+"/ICQ."+icqUin , "accountsettings");
		m_restore_status_text = account_settings.value("autoreply/" + sName
				+ "msg", "").toString();
		if (!settings.value("autoreply/" + sName + "dshow", false).toBool())
		{
//			statusDialog sDialog (m_iconManager);
			QString status_message = account_settings.value("autoreply/" + sName
					+ "msg", "").toString();
//			sDialog.setStatusMessage(account_settings.value("autoreply/" + sName
//				+ "msg", "").toString());
			IcqPluginSystem &ips = IcqPluginSystem::instance();
//			if ( sDialog.exec() )
			bool do_not_show = false;
			if ( ips.setStatusMessage(status_message, do_not_show) )
			{
//				bool do_not_show = sDialog.dontShow;
				settings.setValue("autoreply/" + sName + "dshow", do_not_show);
				if( do_not_show )
				{
					settings.setValue("autoreply/" + sName
//						+ "msg", sDialog.statusMessage.left(1000));
						+ "msg", status_message.left(1000));	
				}
				account_settings.setValue("autoreply/" + sName
//					+ "msg", sDialog.statusMessage.left(1000));
					+ "msg", status_message.left(1000));	
				thisIcqProtocol->setStatus(status);
			}
		} else
			thisIcqProtocol->setStatus(status);


	}
	// Disconnecting
	else
	{
		if (status == offline)
			thisIcqProtocol->userDisconnected = true;

		// Apply new status
		thisIcqProtocol->setStatus(status);
	}
}

void icqAccount::setStatusFromPlugin(accountStatus status, const QString &status_text)
{

	
	if ( status > -1)
	{
	
		if ((status != online) && (status != offline) && (status != ffc)
		&& (status != invisible))
	{
		QString sName(StatusNames[static_cast<int>(status)]);
                QSettings settings(QSettings::NativeFormat, QSettings::UserScope, "qutim/qutim."+m_profile_name, "icqsettings");
                QSettings account_settings(QSettings::NativeFormat, QSettings::UserScope, "qutim/qutim."+m_profile_name+"/ICQ."+icqUin , "accountsettings");
			if ( m_restore_status == restoreAccount )
				m_restore_status_text = account_settings.value("autoreply/" + sName
					+ "msg", "").toString();	
		account_settings.setValue("autoreply/" + sName
					+ "msg", status_text.left(1000));	
		thisIcqProtocol->setStatus(status);


	}
	// Disconnecting
	else
	{
		if (status == offline)
			thisIcqProtocol->userDisconnected = true;

		// Apply new status
		thisIcqProtocol->setStatus(status);
	}
			if ( m_restore_status == restoreAccount )
		m_restore_status = thisIcqProtocol->getStatus();
	} else
	{
		QString sName(StatusNames[static_cast<int>(m_restore_status)]);
		QSettings settings(QSettings::IniFormat, QSettings::UserScope, "qutim/qutim."+m_profile_name, "icqsettings");
		QSettings account_settings(QSettings::IniFormat, QSettings::UserScope, "qutim/qutim."+m_profile_name+"/ICQ."+icqUin , "accountsettings");
		m_restore_status_text = account_settings.value("autoreply/" + sName
				+ "msg", "").toString();	
		account_settings.setValue("autoreply/" + sName
					+ "msg", status_text.left(1000));	
	}
}

void icqAccount::restoreStatusFromPlugin()
{
	setStatusFromPlugin(m_restore_status, m_restore_status_text);
	 m_restore_status = restoreAccount;
}


void icqAccount::updateIconStatus()
{
	accountLineButton->setIcon(currentIcon);
//	if ( menuExist )
//		accountAction->setIcon(currentIcon);
	statusMenu->setIcon(currentIcon);
	if ( statusTrayMenuExist)
		chooseStatus->setIcon(currentIcon);

	if ( currentTrayStatus )
		emit statusChanged(currentIcon);
	
	IcqPluginSystem &ips = IcqPluginSystem::instance();
	ips.updateStatusIcons();
}

void icqAccount::createTrayMenuStatus(QMenu *menu)
{
	menu->addAction(chooseStatus);
	chooseStatus->setIcon(currentIcon);
	statusTrayMenuExist = true;
}

void icqAccount::removeTrayMenuStatus(QMenu *menu)
{
	menu->removeAction(chooseStatus);
	statusTrayMenuExist = false;
}

void icqAccount::emitChangeStatus()
{
        QSettings settings(QSettings::NativeFormat, QSettings::UserScope, "qutim", "qutimsettings");
	settings.setValue("systray/current", icqUin);
	emit changeStatusInTrayMenu(icqUin);
}

void icqAccount::loadAccountSettings()
{
        QSettings settings(QSettings::NativeFormat, QSettings::UserScope, "qutim/qutim."+m_profile_name, "icqsettings");
        QSettings account_settings(QSettings::NativeFormat, QSettings::UserScope, "qutim/qutim."+m_profile_name+"/ICQ."+icqUin, "accountsettings");
	autoConnect = settings.value("connection/auto", true).toBool();
	statusIconIndex = settings.value("main/staticon", 0).toInt();
	thisIcqProtocol->reconnectOnDisc = settings.value("connection/reconnect", true).toBool();
	thisIcqProtocol->getContactListClass()->fileTransferObject->setListenPort(settings.value("connection/listen", 5191).toUInt());
	settings.beginGroup("clientid");

	clientIndex = settings.value("index", 0).toUInt();
	protocolVersion = settings.value("protocol", 1).toUInt();

		clientCap1 = settings.value("cap1").toString();
		clientCap2 = settings.value("cap2").toString();
		clientCap3 = settings.value("cap3").toString();

	settings.endGroup();


	networkSettingsChanged();



	currentXstatus = account_settings.value("xstatus/index", 0).toInt();
	if ( currentXstatus )
	{
		customStatus->setIcon(QIcon(statusIconClass::getInstance()->xstatusList.at(currentXstatus - 1)));
	}else
		customStatus->setIcon(QIcon());
}

void icqAccount::saveAccountSettings()
{
        QSettings settings(QSettings::NativeFormat, QSettings::UserScope, "qutim/qutim."+m_profile_name+"/ICQ."+icqUin, "accountsettings");
	if (getStatus() != offline)
	{
		settings.setValue("connection/currstatus", getStatus());
	}
	else
	{
		settings.remove("connection/currstatus");
	}

}

void icqAccount::autoconnecting()
{
	if ( autoConnect )
	{
                QSettings settings(QSettings::NativeFormat, QSettings::UserScope, "qutim/qutim."+m_profile_name, "icqsettings");
                QSettings account_settings(QSettings::NativeFormat, QSettings::UserScope, "qutim/qutim."+m_profile_name+"/ICQ."+icqUin, "accountsettings");
		if ( settings.value("connection/statonexit", true).toBool() )
			thisIcqProtocol->setStatus(static_cast<accountStatus>(account_settings.value("connection/currstatus", 0).toInt()));
		else
			thisIcqProtocol->setStatus(online);
	}
}

void icqAccount::createContacts()
{

}

void icqAccount::systemMessage(const QString &message)
{
	TreeModelItem contact_item;
	contact_item.m_protocol_name = "ICQ";
	contact_item.m_account_name = icqUin;
	contact_item.m_item_name = icqUin;
	contact_item.m_item_type = 2;
	IcqPluginSystem::instance().systemNotifiacation(contact_item, message);
}

void icqAccount::userMessage(const QString &fromUin, const QString &from, const QString &message, userMessageType type, bool fromList)
{
	TreeModelItem contact_item;
	contact_item.m_protocol_name = "ICQ";
	contact_item.m_account_name = icqUin;
	contact_item.m_item_name = fromUin;
	contact_item.m_item_type = 0;
	if ( type == readNotification )
	{
		IcqPluginSystem::instance().customNotifiacation(contact_item, tr("is reading your away message"));
	} else if ( type == xstatusReadNotification )
	{
		IcqPluginSystem::instance().customNotifiacation(contact_item, tr("is reading your x-status message"));
	} else if ( type == customMessage )
	{
		IcqPluginSystem::instance().customNotifiacation(contact_item, message);
	}
	
}

void icqAccount::removeContactList()
{
	thisIcqProtocol->removeContactList();
}

void icqAccount::readMessageStack()
{
	thisIcqProtocol->readreadMessageStack();
}

void icqAccount::networkSettingsChanged()
{
        QSettings settings(QSettings::NativeFormat, QSettings::UserScope, "qutim/qutim."+m_profile_name, "icqsettings");
	thisIcqProtocol->sendKeepAlive(settings.value("connection/alive", true).toBool());
	thisIcqProtocol->getContactListClass()->fileTransferObject->setListenPort(settings.value("connection/listen", 5191).toUInt());
}

void icqAccount::updateStatusMenu(bool f)
{
	showCustomStatus = f;
	lunchAction->setVisible(f);
	evilAction->setVisible(f);
	depressionAction->setVisible(f);
	atHomeAction->setVisible(f);
	atWorkAction->setVisible(f);
}

void icqAccount::setVisibleForAll()
{
	thisIcqProtocol->getContactListClass()->changePrivacy(1);
}

void icqAccount::setVisibleForVis()
{
	thisIcqProtocol->getContactListClass()->changePrivacy(2);
}

void icqAccount::setNotVisibleForInv()
{
	thisIcqProtocol->getContactListClass()->changePrivacy(3);
}

void icqAccount::setVisibleForContact()
{
	thisIcqProtocol->getContactListClass()->changePrivacy(4);
}

void icqAccount::setInvisibleForAll()
{
	thisIcqProtocol->getContactListClass()->changePrivacy(5);
}


void icqAccount::deleteTrayWindow(QObject *obj)
{
}

QString icqAccount::getIconPathForUin(const QString &uin) const
{
        QSettings settings(QSettings::NativeFormat, QSettings::UserScope, "qutim/qutim."+m_profile_name, "icqsettings");


	QString hashName = settings.value((uin + "/iconhash"), "").toByteArray();
	if ( !hashName.isEmpty( ) )
		return (configPath + "/icqicons/" + hashName);
	else
		return QString("");
}

void icqAccount::generalSettingsChanged()
{
        QSettings settings(QSettings::NativeFormat, QSettings::UserScope, "qutim/qutim."+m_profile_name, "icqsettings");
	thisIcqProtocol->getContactListClass()->setAvatarDisabled(settings.value("connection/disavatars", false).toBool());
	thisIcqProtocol->reconnectOnDisc = settings.value("connection/reconnect", true).toBool();

	int statindex = settings.value("main/staticon", 0).toInt();

	if ( statusIconIndex != statindex )
	{
		statusIconIndex = statindex;

		if ( statusIconIndex != 1)
		{
			updateIconStatus();
			emit updateTrayToolTip();
		} else
		{
			if ( !currentXstatus)
				updateIconStatus();
				emit updateTrayToolTip();
		}

		if (currentXstatus )
		{
			if ( statusIconIndex == 2 || statusIconIndex == 1)
			{
				currentIconPath = statusIconClass::getInstance()->xstatusList.at(currentXstatus - 1);
				currentIcon = QIcon(currentIconPath);

				updateIconStatus();
				emit updateTrayToolTip();
			}			else
				setStatusIcon(getStatus());
		}
		else
		{
			setStatusIcon(getStatus());
		}
		}

	settings.beginGroup("clientid");

	unsigned newClientIndex = settings.value("index", 0).toUInt();
	unsigned newProtocolVersion = settings.value("protocol", 1).toUInt();

	QString newClientCap1 = settings.value("cap1").toString();
	QString	newClientCap2 = settings.value("cap2").toString();
	QString	newClientCap3 = settings.value("cap3").toString();

	settings.endGroup();

	if (checkClientIdentification(newClientIndex, newProtocolVersion,
			newClientCap1, newClientCap2, newClientCap3))
		thisIcqProtocol->resendCapabilities();

}

bool icqAccount::checkClientIdentification(unsigned index, unsigned pVersion, const QString &cap1, const QString &cap2, const QString &cap3)
{
	bool changed = false;

	changed = (index == clientIndex ? changed : true);
	changed = (pVersion == protocolVersion ? changed : true);
	changed = (cap1 == clientCap1 ? changed : true);
	changed = (cap2 == clientCap2 ? changed : true);
	changed = (cap3 == clientCap3 ? changed : true);


	clientIndex = index;
	protocolVersion = pVersion;
	clientCap1 = cap1;
	clientCap2 = cap2;
	clientCap3 = cap3;

	return changed;
}

void icqAccount::customStatusTriggered()
{
        QSettings settings(QSettings::NativeFormat, QSettings::UserScope, "qutim/qutim."+m_profile_name+"/ICQ."+icqUin, "accountsettings");
	settings.beginGroup("xstatus");
	customStatusDialog dialog (icqUin, m_profile_name);
	QPoint point = accountLineButton->mapToGlobal(QPoint(0,0));
	dialog.move(point.x() - dialog.width(), point.y() - dialog.height());
	dialog.setStatuses(settings.value("index", 0).toInt(), statusIconClass::getInstance()->xstatusList);

	settings.endGroup();


	if ( dialog.exec() )
	{

		currentXstatus = dialog.status;
		if ( currentXstatus )
		{
			customStatus->setIcon(QIcon(statusIconClass::getInstance()->xstatusList.at(currentXstatus - 1)));
		} else
			customStatus->setIcon(QIcon());
		thisIcqProtocol->sendOnlyCapabilities();


		if (currentXstatus )
		{
			if ( statusIconIndex == 2 || statusIconIndex == 1)
			{
				currentIconPath = statusIconClass::getInstance()->xstatusList.at(currentXstatus - 1);
				currentIcon = QIcon(currentIconPath);
				updateIconStatus();

				emit updateTrayToolTip();
			}
			else
				setStatusIcon(getStatus());
		}
		else
		{
			setStatusIcon(getStatus());
		}
		}
}

void icqAccount::onUpdateTranslation()
{
	onlineAction->setText(tr("Online"));
	offlineAction->setText(tr("Offline"));
	ffcAction->setText(tr("Free for chat"));
	awayAction->setText(tr("Away"));
	naAction->setText(tr("NA"));
	occupiedAction->setText(tr("Occupied"));
	dndAction->setText(tr("DND"));
	invisibleAction->setText(tr("Invisible"));
	lunchAction->setText(tr("Lunch"));
	evilAction->setText(tr("Evil"));
	depressionAction->setText(tr("Depression"));
	atHomeAction->setText(tr("At Home"));
	atWorkAction->setText(tr("At Work"));
	customStatus->setText(tr("Custom status"));

	privacyStatus->setTitle(tr("Privacy status"));

	visibleForAll->setText(tr("Visible for all"));
	visibleForVis->setText(tr("Visible only for visible list"));
	notVisibleForInv->setText(tr("Invisible only for invisible list"));
	visibleForContact->setText(tr("Visible only for contact list"));
	invisibleForAll->setText(tr("Invisible for all"));

	emit updateTranslation();
}

void icqAccount::onReloadGeneralSettings()
{
	// Update status icons for status menu
	onlineAction->setIcon(statusIconClass::getInstance()->getOnlineIcon());
	offlineAction->setIcon(statusIconClass::getInstance()->getOfflineIcon());
	ffcAction->setIcon(statusIconClass::getInstance()->getFreeForChatIcon());
	awayAction->setIcon(statusIconClass::getInstance()->getAwayIcon());
	naAction->setIcon(statusIconClass::getInstance()->getNotAvailableIcon());
	occupiedAction->setIcon(statusIconClass::getInstance()->getOccupiedIcon());
	dndAction->setIcon(statusIconClass::getInstance()->getDoNotDisturbIcon());
	invisibleAction->setIcon(statusIconClass::getInstance()->getInvisibleIcon());
	lunchAction->setIcon(statusIconClass::getInstance()->getLunchIcon());
	evilAction->setIcon(statusIconClass::getInstance()->getEvilIcon());
	depressionAction->setIcon(statusIconClass::getInstance()->getDepressionIcon());
	atHomeAction->setIcon(statusIconClass::getInstance()->getAtHomeIcon());
	atWorkAction->setIcon(statusIconClass::getInstance()->getAtWorkIcon());
	accountLineButton->setIcon(currentIcon);

	//Update current icon
	setStatusIcon(getStatus());
	// Update button with status menu
	thisIcqProtocol->onReloadGeneralSettings();
}


void icqAccount::setChooseStatusCheck(bool check)
{
	chooseStatus->setChecked(check);
}

void icqAccount::setXstatusFromPlugin(int status, const QString &status_title, QString &status_text)
{

	
	QSettings settings(QSettings::IniFormat, QSettings::UserScope, "qutim/qutim."+m_profile_name+"/ICQ."+icqUin, "accountsettings");

	if ( m_restore_xstatus_num == -10 )
	{
		m_restore_xstatus_num = settings.value("xstatus/index",0).toInt();
		m_restore_xstatus_title = settings.value("xstatus/caption", "").toString();
		m_restore_xstatus_text = settings.value("xstatus/message", "").toString();
	}

	if ( status > -1)
	{
		settings.setValue("xstatus/index", status);
		settings.setValue("xstatus"+ QString::number(status - 1) + "/caption", status_title);
		settings.setValue("xstatus"+ QString::number(status - 1) + "/message", status_text);
	}
	settings.setValue("xstatus/caption", status_title);
	settings.setValue("xstatus/message", status_text);
	thisIcqProtocol->sendOnlyCapabilities();
}

void icqAccount::restoreXstatusFromPlugin()
{
	setXstatusFromPlugin(m_restore_xstatus_num, m_restore_xstatus_title, m_restore_xstatus_text);
	m_restore_xstatus_num = -10;
}
