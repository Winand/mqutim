/*
    contactListTree

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

//#include <QtGui>
#include <QTcpSocket>
#include <QNetworkProxy>

#include <QtopiaApplication>

//#include "treegroupitem.h"
#include "treebuddyitem.h"
#include "icqmessage.h"
#include "buffer.h"
#include "metainformation.h"
//#include "statusiconsclass.h"
#include "servicessetup.h"
#include "buddypicture.h"
#include "searchuser.h"
#include "userinformation.h"
#include "addbuddydialog.h"
//#include "modifyitem.h"
#include "multiplesending.h"
#include "privacylistwindow.h"
#include "passwordchangedialog.h"
#include "addrenamedialog.h"
#include "readawaydialog.h"
#include "deletecontactdialog.h"
#include "requestauthdialog.h"
#include "acceptauthdialog.h"
//#include "clientidentify.h"
/*#include "filetransfer.h"*/
#include "snac.h"
#include "tlv.h"
#include "contactlist.h"
#include "notewidget.h"

contactListTree::contactListTree(QTcpSocket *s, icqBuffer *buff,
		                         const QString &uin, const QString &profile_name, quint16 &flap,  quint32 &snac, quint16 &meta, QObject *parent)
	: QObject(parent)
	, statusIconObject(statusIconClass::getInstance())
	, tcpSocket(s)
	, socket(buff)
	, icqUin(uin)
	, account_name(uin)
	, m_profile_name(profile_name)
	, m_icq_plugin_system(IcqPluginSystem::instance())
{
	flapSeq = &flap;
	snacSeq = &snac;
	metaSeq = &meta;
	newMessages = false;
	isMergeAccounts = false;
	contactListChanged = false;
	iAmOnline = false;
	showGroups = false;
	clearNil = false;
	waitForMineInfo = false;
	mineMetaSeq = 0;
	pdInfoID = 0;
	justStarted = false;

	codec = QTextCodec::codecForName("Windows-1251");
	if (!codec)
		codec = QTextCodec::codecForLocale();

	Q_ASSERT(codec);

	currentStatus = offline;
	findUserWindowOpen = false;
	multipleSendingOpen = false;
	privacyListWindowOpen = false;
	waitForIconUpload = false;
	movingBuddy = false;
	avatarModified = false;
	iconObject.itemId = 0;
	currentContextGroup = 0;
	currentContextMenu = 0;
	allGroupCount = 0;
	allContactCount = 0;
	allContactTmpCount = 0;
        QSettings settings(QSettings::NativeFormat, QSettings::UserScope, "qutim/qutim."+m_profile_name, "icqsettings");
	iconPath = settings.fileName().section('/', 0, -2) + "/icqicons/";


	letMeAskYouAboutXstatusPlease = true;
	TreeModelItem root_item;
	root_item.m_protocol_name = "ICQ";
	root_item.m_account_name = icqUin;
	root_item.m_item_name = icqUin;
	root_item.m_item_type = TreeModelItem::Account;
	m_icq_plugin_system.addItemToContactList(root_item, icqUin);

	loadSettings();
	createContactList();
	loadUnreadedMessages();
	buddyConnection = new buddyPicture(m_profile_name, icqUin, this);

	connect ( buddyConnection, SIGNAL(emptyAvatarList()),this, SLOT(emptyAvatarList()));
	connect ( buddyConnection, SIGNAL(updateAvatar(const QString &, QByteArray)),
					this, SLOT(updateAvatar(const QString &, QByteArray)));
	avatarPort = 0;

/*	fileTransferObject = new FileTransfer(icqUin, this);
	connect ( fileTransferObject, SIGNAL(sendFile(QByteArray &, QByteArray &, QByteArray &)),
			this, SLOT(sendFile(QByteArray &, QByteArray &, QByteArray &)));


	connect ( fileTransferObject, SIGNAL(emitCancelSending(QByteArray &)),
			this, SLOT(sendCancelSending(QByteArray &)));

	connect ( fileTransferObject, SIGNAL(sendRedirectToProxy(const QByteArray &)),
			this, SLOT(redirectToProxy(const QByteArray &)));
	connect ( fileTransferObject, SIGNAL(emitAcceptSending(const QByteArray &)),
                        this, SLOT(sendAcceptMessage(const QByteArray &)));*/

	createContactListActions();
	createSoundEvents();
}

contactListTree::~contactListTree()
{


        QSettings settings(QSettings::NativeFormat, QSettings::UserScope, "qutim/qutim."+m_profile_name, "icqsettings");

	if ( findUserWindowOpen )
			delete searchWin;

	if ( multipleSendingOpen )
		delete multipleSendingWin;
	if (privacyListWindowOpen)
		delete privacyWindow;

        qDeleteAll(infoWindowList);
	infoWindowList.clear();

        qDeleteAll(awayMessageList);
	awayMessageList.clear();

	if ( currentContextMenu)
	{
		delete menuLabel;
		delete currentContextMenu;
	}

	qDeleteAll(noteWidgetsList);
	noteWidgetsList.clear();
}



void contactListTree::createContact(bool last)
{
        QSettings settings(QSettings::NativeFormat, QSettings::UserScope, "qutim/qutim."+m_profile_name+"/ICQ."+account_name, "contactlist");

	Q_ASSERT(socket);
	if (!socket)
		return ;

	socket->read(1);
	quint16 arraySize = byteArrayToInt16(socket->read(2));
	QStringList groups = settings.value("list/group").toStringList();
	QStringList buddies = settings.value("list/contacts").toStringList();

	for ( quint16 i = 0; i < arraySize; i++ )
	{
		quint16 stringLength = byteArrayToInt16(socket->read(2));
		QString itemName = QString::fromUtf8(socket->read(stringLength));
		quint16 groupID = byteArrayToInt16(socket->read(2));
		quint16 itemId = byteArrayToInt16(socket->read(2));
		quint16 itemType = byteArrayToInt16(socket->read(2));

		if ( itemType == 0x0001 && groupID != 0x0000 )
		{
			settings.beginGroup(QString::number(groupID));
			if ( !groupList.contains(groupID))
			{
				treeGroupItem *group = new treeGroupItem;
				group->setOnOffLists();
				groupList.insert(groupID, group);
				group->setGroupText(itemName);
				quint16 groupLength = byteArrayToInt16(socket->read(2));
				group->readData(socket, groupLength);
				settings.setValue("id", groupID);
				contactListChanged = true;
				allContactCount += group->userCount;
				addGroupToCL(groupID, group->name);
			} else {
				if ( groupList.value(groupID) && itemName != groupList.value(groupID)->name)
					contactListChanged = true;

				if (groupList.contains(groupID))
				{
					renameGroupInCL(itemName, groupID);
					groupList.value(groupID)->setGroupText(itemName);
					quint16 groupLength = byteArrayToInt16(socket->read(2));
					groupList.value(groupID)->readData(socket, groupLength);
					allContactCount += groupList.value(groupID)->userCount;
					getGroups.removeAll(QString::number(groupID));

				}
			}

			settings.setValue("name", itemName);
			settings.endGroup();
			if ( !groups.contains(QString::number(groupID)))
				groups<<QString::number(groupID);

		}else if ( itemType == 0x0001 && !groupID )
		{
			quint16 tmpLength = byteArrayToInt16(socket->read(2));
			tlv rootTlv;

			for ( ;tmpLength > 0; )
			{
				tlv tmpTlv;
				tmpTlv.readData(socket);

				tmpLength -= tmpTlv.getLength();

				if ( tmpTlv.getTlvType() == 0x00c8)
					rootTlv = tmpTlv;
			}
			allGroupCount = rootTlv.getTlvLength() / 2;

		}
		else if (itemType == 0x0000 )
		{
			allContactTmpCount++;
			idBuddyList.append(itemId);
			settings.beginGroup(itemName);
			if ( !buddyList.contains(itemName))
			{
				if ( treeGroupItem *group = groupList.value(groupID) )
				{
					treeBuddyItem *buddy = new treeBuddyItem(icqUin, m_profile_name);

					buddy->itemId = itemId;
					initializeBuddy(buddy);
					buddy->underline = !dontUnderlineNotAutho;
					buddy->groupID = groupID;
					buddy->groupName = group->name;
					buddyList.insert(itemName, buddy);
					buddy->setBuddyUin(itemName);
					quint16 buddyLength = byteArrayToInt16(socket->read(2));
					buddy->readData(socket, buddyLength);
					settings.setValue("nickname", buddy->getName());
					settings.setValue("authorized", !buddy->getNotAutho());
					settings.setValue("lastonline", buddy->lastonlineTime);
					contactListChanged = true;
					if (itemName == buddy->getName())
						requestUinInformation(itemName);
					addContactToCL(buddy->groupID, itemName, buddy->getName());
				}
			} else {
				quint16 buddyLength = byteArrayToInt16(socket->read(2));
				treeBuddyItem *tmpBuddy = buddyList.value(itemName);
				tmpBuddy->readData(socket, buddyLength);
				tmpBuddy->itemId = itemId;

				if (tmpBuddy->getName() != itemName)
				{
					settings.setValue("nickname", tmpBuddy->getName());
					renameContactInCL(tmpBuddy->groupID, tmpBuddy->getUin(),
							tmpBuddy->getName());
				}
//				else
//					requestUinInformation(itemName);
				settings.setValue("lastonline", tmpBuddy->lastonlineTime);
				settings.setValue("authorized", !tmpBuddy->getNotAutho());
				if ( groupID != tmpBuddy->groupID  && groupList.contains(tmpBuddy->groupID))
				{
					treeGroupItem *oldGrp =  groupList.value(tmpBuddy->groupID);

					Q_ASSERT(oldGrp);
					oldGrp->userCount--;
					treeGroupItem *newGrp = groupList.value(groupID);
					Q_ASSERT(newGrp);
					newGrp->userCount++;
					moveContactFromGroupToGroup(tmpBuddy->groupID, groupID, tmpBuddy->getUin());
					settings.setValue("groupid", groupID);
					contactListChanged = true;
				}

			}

			getBuddies.removeAll(itemName);
			settings.setValue("name", itemName);
			settings.setValue("groupid", groupID);
			settings.endGroup();

			if ( ! buddies.contains(itemName))
				buddies<<itemName;

		} else if (itemType == 0x000E)
		{
			quint16 tmpLength = byteArrayToInt16(socket->read(2));
			socket->read(tmpLength);
			ignoreList<<itemName;
			itemFromList object;
			object.groupId = groupID;
			object.itemId = itemId;
			ignoreObjectList.insert(itemName, object);

		} else if ( itemType == 0x0002)
		{
			quint16 tmpLength = byteArrayToInt16(socket->read(2));
			socket->read(tmpLength);
			visibleList<<itemName;

			itemFromList object;
			object.groupId = groupID;
			object.itemId = itemId;
			visibleObjectList.insert(itemName, object);

		} else if ( itemType == 0x0003 )
		{
			quint16 tmpLength = byteArrayToInt16(socket->read(2));
			socket->read(tmpLength);
			invisibleList<<itemName;

			itemFromList object;
			object.groupId = groupID;
			object.itemId = itemId;
			invisibleObjectList.insert(itemName, object);
		}
		else if ( itemType ==  0x0004)
		{
			pdInfoID = itemId;
			pdInfoGroupId = groupID;
			quint16 tmpLength = byteArrayToInt16(socket->read(2));
			socket->read(tmpLength);
		} else if ( itemType == 0x0014)
		{

			iconObject.groupId = groupID;
			iconObject.itemId = itemId;

			quint16 tmpLength = byteArrayToInt16(socket->read(2));
			bool iconGetted = false;
			tlv iconTlv;

			for ( ;tmpLength > 0; )
			{
				tlv tmpTlv;
				tmpTlv.readData(socket);

				tmpLength -= tmpTlv.getLength();

				if ( tmpTlv.getTlvType() == 0x00d5)
				{
					iconGetted = true;
					iconTlv = tmpTlv;
				}
			}


			if ( iconGetted )
			{

				checkForOwnIcon(iconTlv.getTlvData());
			} else {
				removeIconHash();
			}
		}
		else {
			quint16 tmpLength = byteArrayToInt16(socket->read(2));
			socket->read(tmpLength);
		}

	}

// W00T W00T W00T
//	if ((allGroupCount == (groupList.count() - 1)) && (allContactCount == allContactTmpCount))
//		last = true;

	socket->read(4);
	if ( last )
	{

		foreach(const QString &name, getGroups)
		{
			getGroups.removeAll(name);
			groups.removeAll(name);
			settings.remove(name);


			treeGroupItem *tmpItem = groupList.value(name.toShort());
			removeGroupFromCl(name.toShort());
			delete tmpItem;
			tmpItem = NULL;
			groupList.remove(name.toShort());
			contactListChanged = true;

			QStringList buddiesList = settings.value("list/contacts").toStringList();

			foreach(treeBuddyItem *buddy, buddyList)
			{
				Q_ASSERT(buddy);
				if (!buddy)
					continue;

				if ( buddy->groupID == name.toShort())
				{
					buddiesList.removeAll(buddy->getUin());
					getBuddies.removeAll(buddy->getUin());
					settings.remove(buddy->getUin());
				}
			}

			settings.setValue("list/contacts", buddiesList);


		}
		getGroups = settings.value("list/group").toStringList();

		foreach(const QString &uin, getBuddies)
		{

			getBuddies.removeAll(uin);
			treeBuddyItem *nilBuddy = buddyList.value(uin);
			if ( nilBuddy && nilBuddy->groupID)
			{
				if ( groupList.contains(nilBuddy->groupID))
				{
					removeContactFromCl(nilBuddy->groupID, nilBuddy->getUin());
				}

				nilBuddy->groupID = 0;
				treeGroupItem *newGrp = groupList.value(0);
				addContactToCL(0, nilBuddy->getUin(), nilBuddy->getName());
				newGrp->userCount++;
				settings.setValue(uin + "/groupid", 0);
				contactListChanged = true;
			}
		}
		getBuddies = settings.value("list/contacts").toStringList();
		if ( contactListChanged  && isMergeAccounts )
			emit reupdateList();

		emit incSnacSeq();
		servicesSetup privacySetup(icqUin, m_profile_name);
		privacySetup.flap1309seq = *flapSeq;
		privacySetup.snac1309seq = *snacSeq;
		privacySetup.setPrivacy(icqUin, pdInfoID, pdInfoGroupId, tcpSocket);
		emit incFlapSeq();


		emit incSnacSeq();
		emit incMetaSeq();
		metaInformation metaInfo(icqUin);
		metaInfo.sendShortInfoReq(tcpSocket, *flapSeq, *snacSeq, *metaSeq, icqUin);
		mineMetaSeq = ((*metaSeq )% 0x100) * 0x100 + ((*metaSeq )/ 0x100);
		waitForMineInfo = true;
		emit incFlapSeq();

		sendReqForRedirect();



		justStarted = true;
		QTimer::singleShot(3000, this, SLOT(disableJustStarted()));
		settings.setValue("list/ignore", ignoreList);
		settings.setValue("list/visible", visibleList);
		settings.setValue("list/invisible", invisibleList);

		if ( ignoreList.count() || visibleList.count() || invisibleList.count() )
		{
			//Q_ASSERT(privacyWindow);
			if ( privacyListWindowOpen && privacyWindow)
				privacyWindow->createLists();
		}
		allGroupCount = 0;
		allContactCount = 0;

//		fileTransferObject->connectionProxy = tcpSocket->proxy();
		setPrivacyIconsToContacts();
	}
	settings.setValue("list/group", groups);
	settings.setValue("list/contacts", buddies);
}


void contactListTree::removeContactList()
{

	TreeModelItem root_item;
	root_item.m_protocol_name = "ICQ";
	root_item.m_account_name = icqUin;
	root_item.m_item_name = icqUin;
	root_item.m_item_type = TreeModelItem::Account;
	m_icq_plugin_system.removeItemFromContactList(root_item);
}

quint16 contactListTree::byteArrayToInt16(const QByteArray &array)
{
	bool ok;
	return array.toHex().toUInt(&ok,16);
}

quint32 contactListTree::byteArrayToInt32(const QByteArray &array)
{
	bool ok;
	return array.toHex().toULong(&ok,16);
}

void contactListTree::oncomingBuddy(const QString &uin, quint16 length)
{

	bool fromOffline = false;
	if ( treeBuddyItem *buddy = buddyList.value(uin) )
	{

//		buddy->xStatusCaption.clear();
//		buddy->xStatusMsg.clear();
		treeGroupItem *group = groupList.value(buddy->groupID);

		Q_ASSERT(group);
		if (group && buddy && buddy->isOffline )
		{

			fromOffline = true;


		}
		buddy->oncoming(socket, length);
		identifyContactClient.addContactClientId(buddy);
		buddy->checkForXStatus();

		if ( buddy->xStatusPresent )
			xStatusTickList.append(buddy);

		if ( letMeAskYouAboutXstatusPlease && buddy->xStatusPresent)
		{
			letMeAskYouAboutXstatusPlease = false;

			askForXstatusTimerTick();
		}

		if ( !showXStatuses)
			buddy->waitingForAuth(buddy->authorizeMe);

		if ( buddy->statusChanged )
		{
			updateChatBuddyStatus(buddy->getUin(),
				(statusIconClass::getInstance()->*(buddy->statusIconMethod))());

			if (!justStarted)
		{
				// play sound
				emit playSoundEvent(static_cast<SoundEvent::Events>(buddy->getStatus()),
					currentStatus);


			if ( !fromOffline && changeStatusNot )// && !contactList->hasFocus())
			{
				userMessage(uin, buddy->getName(), convertToStringStatus(buddy->getStatus()), statusNotyfication, true);
			}

		}
		}

		emit updateOnlineList();

		if ( buddy->getAvatarHash().length() != 16 )
		{
                        QSettings contacts(QSettings::NativeFormat, QSettings::UserScope, "qutim/qutim."+m_profile_name+"/ICQ."+account_name, "contactlist");
			contacts.remove(uin + "/iconhash");
		}



		if (! disableAvatars)
			askForAvatars(buddy->getAvatarHash(), uin);
//		if ( chatWindowList.contains(uin))
//			chatWindowList.value(uin)->setContactClient(buddy->clientId);
	}
	else
	{
		socket->read(length);
	}
}

void contactListTree::offlineBuddy(const QString &uin, quint16 length)
{
	if ( treeBuddyItem *buddy = buddyList.value(uin) )
	{
			if ( !buddy->isOffline )
			{
                        // play sound
                        playSoundEvent(SoundEvent::ContactOffline, currentStatus);

				treeGroupItem *group = groupList.value(buddy->groupID);

				buddy->buddyOffline();
				updateChatBuddyStatus(buddy->getUin(), (statusIconClass::getInstance()->*(buddy->statusIconMethod))());

			}
//			if ( chatWindowList.contains(uin))
//				chatWindowList.value(uin)->setContactClient(buddy->clientId);

	}
	socket->read(length);
}

QString contactListTree::convertToStringStatus(contactStatus status)
{
	switch ( status )
		{
		case contactOnline:
			return tr("is online");
		case contactAway:
			return tr("is away");
		case contactDnd:
			return tr("is dnd");
		case contactNa:
			return tr("is n/a");
		case contactOccupied:
			return tr("is occupied");
		case contactFfc:
			return tr("is free for chat");
		case contactInvisible:
			return tr("is invisible");
		case contactOffline:
			return tr("is offline");
		case contactAtHome:
			return tr("at home");
		case contactAtWork:
			return tr("at work");
		case contactLunch:
			return tr("having lunch");
		case contactEvil:
			return tr("is evil");
		case contactDepression:
			return tr("in depression");
		default:
			return tr("is online");
		}
}

void contactListTree::getMessage(quint16 l)
{
	icqMessage newMessage(codepage);
	newMessage.readData(socket, l);


/*	if ( newMessage.fileAnswer)
	{

		if ( !buddyList.contains(newMessage.from))
			return;
		if( !newMessage.reason  )
			fileTransferObject->requestToRedirect(newMessage.from, newMessage.msgCookie, newMessage.connectToPeer,
					newMessage.peerIP, newMessage.peerPort, buddyList.value(newMessage.from)->getName(),
					newMessage.fileName, newMessage.fileSize, newMessage.aolProxyIP);
		if ( newMessage.reason == 0x0001)
			fileTransferObject->contactCanceled(newMessage.from, newMessage.msgCookie);
		if ( newMessage.reason == 0x0002)
			fileTransferObject->contactAccept(newMessage.from, newMessage.msgCookie);
        }*/

	if (!buddyList.contains(newMessage.from))
	{
		if ( newMessage.messageType == 2  &&
				!checkMessageForValidation(newMessage.from , newMessage.msg, 1))
		{
			return;
		}
		if ( newMessage.messageType == 0  &&
				!checkMessageForValidation(newMessage.from , newMessage.msg, 0))
		{
			return;
		}
	}

//    if (!buddyList.contains(newMessage.from))
//    {
//    if (getOnlyFromContactList && ( !newMessage.messageType || newMessage.messageType == 4) )
//	{
//		if ( notifyAboutBlocked)
//			notifyBlockedMessage(newMessage.from, newMessage.msg);
//
//		if ( saveServiceHistory )
//			saveBlocked(newMessage.from, newMessage.msg, QDateTime::currentDateTime());
//
//		return;
//	}
//
//                        if (blockAuth && newMessage.messageType == 2) return;
//
//                        if (blockUrlMessage && checkMessageForUrl(newMessage.msg)
//                                || newMessage.messageType == 3)
//		{
//			if ( notifyAboutBlocked && ( !newMessage.messageType || newMessage.messageType == 4) )
//						notifyBlockedMessage(newMessage.from, newMessage.msg);
//			return;
//		}
//
//                        if (enableAntiSpamBot && ( !newMessage.messageType || newMessage.messageType == 4) )
//                                if (turnOnAntiSpamBot(newMessage.from, newMessage.msg,
//                                        QDateTime::currentDateTime())) return;
//	}

	if ( newMessage.messageType == 0 )
	{
		bool playSound = true;
		messageFormat *msg = new messageFormat;
		msg->fromUin = newMessage.from;
		treeBuddyItem *tmpBuddy = 0;
		if ( (tmpBuddy = buddyList.value(msg->fromUin)) )
		{

			msg->from = tmpBuddy->getName();

                }
                else
                {
                        QSettings settings(QSettings::NativeFormat, QSettings::UserScope, "qutim/qutim."+m_profile_name+"/ICQ."+account_name, "contactlist");

			treeGroupItem *group = groupList.value(0);
			msg->from = msg->fromUin;
			treeBuddyItem *buddy = new treeBuddyItem(icqUin, m_profile_name);
			initializeBuddy(buddy);
			buddy->underline = !dontUnderlineNotAutho;
			buddy->groupID = 0;
			buddy->groupName = group->name;
			group->userCount++;
			buddyList.insert(msg->fromUin, buddy);
			buddy->setBuddyUin(msg->fromUin);
			buddy->setName(msg->fromUin);
			buddy->updateBuddyText();
			updateNil();
			requestUinInformation(buddy->getUin());
			settings.beginGroup(buddy->getUin());
			settings.setValue("name", buddy->getUin());
			settings.setValue("groupid", 0);
			settings.setValue("nickname", buddy->getName());
			settings.endGroup();
			addContactToCL(0,buddy->getUin(), buddy->getName());
			QStringList contacts = settings.value("list/contacts").toStringList();
			contacts<<buddy->getUin();
			settings.setValue("list/contacts", contacts);

		}

		msg->message = newMessage.msg;

		msg->date = QDateTime::currentDateTime();
		addMessageFromContact(msg->fromUin,
				buddyList.contains(msg->fromUin)?buddyList.value(msg->fromUin)->groupID : 0
				, msg->message, msg->date);
		sendMessageRecieved(msg->fromUin, newMessage.msgCookie);
//		if ( chatWindowList.contains(msg->fromUin))
//		{
//			chatWindow *w = chatWindowList.value(msg->fromUin);
//			w->setMessage(msg->from,msg->message, msg->date);
//			if ( !disableButtonBlinking)
//				qApp->alert(w, 0);
//			if ( tabMode )
//				generalChatWindow->setMessageTab(w);
//			if ( !w->isActiveWindow() )
//			{
//
//				if ( messageList.contains(msg->fromUin))
//				{
//					messageList.value(msg->fromUin)->messageList.append(msg);
//				} else {
//					messageList.insert(msg->fromUin, buddyList.value(msg->fromUin));
//					messageList.value(msg->fromUin)->messageList.append(msg);
//				}
//				if ( !newMessages )
//				{
//				newMessages = true;
//				QTimer::singleShot(1000,this, SLOT(setMessageIconToContact()));
//				}
//				if ( !disableTrayBlinking )
//					emit getNewMessage();
//				if ( ! dontShowEvents )
//				{
//					if ( !dontShowIncomingMessagesInTrayEvents)
//						emit userMessage(msg->fromUin, msg->from, msg->message, messageNotification, true);
//					else
//						emit userMessage(msg->fromUin, msg->from, tr("New message"), messageNotification, true);
//				}
//			}
//                                else if (!sounds->playIfChatWindowIsActive())
//                                        playSound = false;
//                        }
//                        else
//                        {
//			if ( messageList.contains(msg->fromUin))
//			{
//				messageList.value(msg->fromUin)->messageList.append(msg);
//			} else {
//				messageList.insert(msg->fromUin, buddyList.value(msg->fromUin));
//				messageList.value(msg->fromUin)->messageList.append(msg);
//			}
//			if ( !dontShowIncomingMessagesInTrayEvents)
//				emit userMessage(msg->fromUin, msg->from, msg->message, messageNotification, true);
//			else
//				emit userMessage(msg->fromUin, msg->from, tr("New message"), messageNotification, true);
//			if ( !disableTrayBlinking )
//				emit getNewMessage();
//		}
//		if ( !newMessages )
//		{
//			newMessages = true;
//			QTimer::singleShot(1000,this, SLOT(setMessageIconToContact()));
//		}
//
//		if ( openNew )
//			doubleClickedBuddy(buddyList.value(msg->fromUin));
		if (playSound)
		{
                      // play sound
                      emit playSoundEvent(SoundEvent::MessageGet, currentStatus);
		};

		delete msg;

        }
        else if ( newMessage.messageType == 1 )
	{
		if ( buddyList.contains(newMessage.from))
		{
//			emit userMessage(tr("%1 is reading your away message").arg(buddyList.value(newMessage.from)->getName()));
			if ( m_notify_about_reading_status )
                                emit userMessage(newMessage.from, buddyList.value(newMessage.from)->getName(),
                                        "", readNotification, true);
//			setServiceMessageToWin(buddyList.value(newMessage.from)->getUin(), tr("is reading your away message"));
			addServiceMessage(buddyList.value(newMessage.from)->getUin(), buddyList.value(newMessage.from)->groupID,
					tr("%1 is reading your away message").arg(buddyList.value(newMessage.from)->getName()));

			emit incSnacSeq();

			newMessage.sendAutoreply(tcpSocket, getCurrentAwayMessage(),*flapSeq, *snacSeq );
			emit incFlapSeq();


		}
		else
		{
//			emit userMessage(tr("%1 is reading your away message ( not from list )").arg(newMessage.from));
			if ( m_notify_about_reading_status )
				emit userMessage(newMessage.from, "", "", readNotification, false);
			}
		}
        else if (newMessage.messageType == 4)
	{
		bool playSound = true;
		messageFormat *msg = new messageFormat;
		msg->fromUin = newMessage.from;


		if ( buddyList.contains(msg->fromUin) )
		{
			treeBuddyItem *buddy = buddyList.value(msg->fromUin);
			msg->from = buddy->getName();
//			addImage( buddy->getUin(), buddy->groupID, newMessage.byteArrayMsg );
         }
         else
                {

                        QSettings settings(QSettings::NativeFormat, QSettings::UserScope, "qutim/qutim."+m_profile_name+"/ICQ."+account_name, "contactlist");

			treeGroupItem *group = groupList.value(0);
			msg->from = msg->fromUin;
			treeBuddyItem *buddy = new treeBuddyItem(icqUin, m_profile_name);
			initializeBuddy(buddy);
			buddy->underline = !dontUnderlineNotAutho;
			buddy->groupID = 0;
			buddy->groupName = group->name;
			group->userCount++;
			group->updateText();
			buddyList.insert(msg->fromUin, buddy);
			buddy->setBuddyUin(msg->fromUin);
			buddy->setName(msg->fromUin);
			buddy->updateBuddyText();
			requestUinInformation(buddy->getUin());
			settings.beginGroup(buddy->getUin());
			settings.setValue("name", buddy->getUin());
			settings.setValue("groupid", 0);
			settings.setValue("nickname", buddy->getName());
			settings.endGroup();
			addContactToCL(0,buddy->getUin(), buddy->getName());
			QStringList contacts = settings.value("list/contacts").toStringList();
			contacts<<buddy->getUin();
			settings.setValue("list/contacts", contacts);

			if ( contactListChanged  && isMergeAccounts )
						emit reupdateList();


		}


		if (buddyList.contains(msg->fromUin))
		{

			if ( buddyList.value(msg->fromUin)->UTF8 )
				msg->message = newMessage.msg;
			else
				msg->message = codec->toUnicode(newMessage.byteArrayMsg);
		}
		else {
			msg->message = newMessage.msg;
		}

		msg->date = QDateTime::currentDateTime();

		addMessageFromContact(msg->fromUin,
				buddyList.contains(msg->fromUin)?buddyList.value(msg->fromUin)->groupID : 0
				, msg->message, msg->date);
		sendMessageRecieved(msg->fromUin, newMessage.msgCookie);
//		if ( chatWindowList.contains(msg->fromUin))
//		{
//			chatWindow *w = chatWindowList.value(msg->fromUin);
//			w->setMessage(msg->from,msg->message, msg->date);
//			if ( !disableButtonBlinking)
//				qApp->alert(w, 0);
//			if ( tabMode )
//				generalChatWindow->setMessageTab(w);
//			if ( !w->isActiveWindow() )
//			{
//
//				if ( messageList.contains(msg->fromUin))
//					messageList.value(msg->fromUin)->messageList.append(msg);
//                                else
//                                {
//					messageList.insert(msg->fromUin, buddyList.value(msg->fromUin));
//					messageList.value(msg->fromUin)->messageList.append(msg);
//				}
//				newMessages = true;
//				QTimer::singleShot(1000,this, SLOT(setMessageIconToContact()));
//				if ( !disableTrayBlinking )
//					emit getNewMessage();
//				if ( ! dontShowEvents )
//				{
//					if ( !dontShowIncomingMessagesInTrayEvents)
//                                                emit userMessage(msg->fromUin, msg->from, msg->message,
//                                                        messageNotification, true);
//					else
//                                                emit userMessage(msg->fromUin, msg->from, tr("New message"),
//                                                        messageNotification, true);
//				}
//			}
//                        else if (!sounds->playIfChatWindowIsActive())
//                                playSound = false;
//                }
//                else
//                {
//			if ( messageList.contains(msg->fromUin))
//			{
//				messageList.value(msg->fromUin)->messageList.append(msg);
//                        }
//                        else
//                        {
//				messageList.insert(msg->fromUin, buddyList.value(msg->fromUin));
//				messageList.value(msg->fromUin)->messageList.append(msg);
//			}
//			if ( dontShowIncomingMessagesInTrayEvents )
//                                emit userMessage(msg->fromUin, msg->from, msg->message,
//                                        messageNotification, true);
//			else
//                                emit userMessage(msg->fromUin, msg->from, tr("New message"),
//                                        messageNotification, true);
//
//			if ( !disableTrayBlinking )
//				emit getNewMessage();
//		}
//		if ( !newMessages )
//		{
//			newMessages = true;
//			QTimer::singleShot(1000,this, SLOT(setMessageIconToContact()));
//		}
//
//		if ( openNew )
//			doubleClickedBuddy(buddyList.value(msg->fromUin));
		if (playSound)
		{
                        // play sound
                        emit playSoundEvent(SoundEvent::MessageGet, currentStatus);
		};
		delete msg;
        }
        else if ( newMessage.messageType == 7)
	{
		if ( buddyList.contains(newMessage.from))
		{

			if ( m_notify_about_reading_status )
                        emit userMessage(newMessage.from, buddyList.value(newMessage.from)->getName(),
                                "", xstatusReadNotification, true);
			setServiceMessageToWin(buddyList.value(newMessage.from)->getUin(),
					tr("%1 is reading your x-status message").arg(buddyList.value(newMessage.from)->getName()));

			emit incSnacSeq();

			newMessage.sendXstatusReply(tcpSocket, icqUin, m_profile_name, *flapSeq, *snacSeq );
			emit incFlapSeq();

		}
		else
		{

			if ( m_notify_about_reading_status )
				emit userMessage(newMessage.from, "", "", xstatusReadNotification, false);
		}
	} else if (newMessage.messageType == 8)
	{

		if ( buddyList.contains(newMessage.from) )
		{
			treeBuddyItem *buddy = buddyList.value(newMessage.from);
			addImage( buddy->getUin(), buddy->groupID, newMessage.byteArrayMsg );
         }
	}

}

void contactListTree::readMessageStack()
{
	foreach(treeBuddyItem *item, messageList)
	{
		readMessageFrom(item);
	}
}

void contactListTree::setMessageIconToContact()
{
	if ( !messageList.empty() )
	{
		foreach(treeBuddyItem *item, messageList)
		{
				item->setMessageIcon(item->messageIcon);
			item->messageIcon = !item->messageIcon;
		}
		QTimer::singleShot(1000,this, SLOT(setMessageIconToContact()));
	} else {
		newMessages = false;
	}
}

void contactListTree::doubleClickedBuddy(treeBuddyItem *buddy)
{
	if ( buddy->authorizeMe )
	{
		openAuthReqFromBuddy(buddy);
		return;
	}
}

void contactListTree::sendMessage(const messageFormat &msg)
{
	if ( buddyList.contains(msg.fromUin ) )
	{
		treeBuddyItem *buddy = buddyList.value(msg.fromUin);
		emit incSnacSeq();
		icqMessage message(codepage);

	if ( buddy->getStatus()!= contactOffline )
		if ( buddy->m_channel_two_support && buddyList.value(msg.fromUin)->UTF8)
			message.sendMessageChannel2(tcpSocket, msg,*flapSeq, *snacSeq, buddyList.value(msg.fromUin)->UTF8);
		else
			message.sendMessage(tcpSocket, msg,*flapSeq, *snacSeq, buddyList.value(msg.fromUin)->UTF8);
	else
		message.sendMessage(tcpSocket, msg,*flapSeq, *snacSeq, false);
	emit incFlapSeq();

	messageCursorPositions.insert(message.msgCookie, msg.position);



                // play sound
                emit playSoundEvent(SoundEvent::MessageSend, currentStatus);
    }
	else
	{
		emit incSnacSeq();
		icqMessage message(codepage);
		message.sendMessage(tcpSocket, msg,*flapSeq, *snacSeq, false);
		emit incFlapSeq();
	}
}

void contactListTree::getOfflineMessage()
{
	socket->read(2);
	quint32 tempSenderUin = byteArrayToInt32(socket->read(4));
	quint32 senderUin = (tempSenderUin % 0x100) * 0x1000000 +
	(tempSenderUin % 0x10000 / 0x100) * 0x10000 +
	(tempSenderUin % 0x1000000 / 0x10000) * 0x100 +
	(tempSenderUin / 0x1000000);
	messageFormat *msg = new messageFormat;
	msg->fromUin = QString::number(senderUin, 10);
	bool messageFromList;

	bool ok;
		QDateTime offlineDateTime;
		quint16 year = (quint8)socket->read(1).toHex().toUShort(&ok, 16);
		year += ((quint8)socket->read(1).toHex().toUShort(&ok, 16) * 0x100);
		int month = (quint8)socket->read(1).toHex().toUShort(&ok, 16);
		int day = (quint8)socket->read(1).toHex().toUShort(&ok, 16);
		int hour = (quint8)socket->read(1).toHex().toUShort(&ok, 16);
		int min = (quint8)socket->read(1).toHex().toUShort(&ok, 16);
		QDate offlineDate(year, month, day);

		QTime offlineTime(hour, min);

		offlineDateTime.setDate(offlineDate);
		offlineDateTime.setTime(offlineTime);

		quint8 messageType = (quint8)socket->read(1).toHex().toUShort(&ok, 16);
		socket->read(1);
		quint16 tmpLength = byteArrayToInt16(socket->read(2));
		quint16 length = (tmpLength % 0x100) * 0x100 + tmpLength / 0x100;

		msg->message = codec->toUnicode(socket->read(length - 1));

		QDateTime curTime = QDateTime::currentDateTime();

		// TO-DO: What does this means? It seems calculating offset timestamp for offline messages.
		// It seems QT includes in local time not only offset from UTC time, but also Daylight Savings Time offset
		int offset = (curTime.toLocalTime().time().hour() - curTime.toUTC().time().hour()) * 3600 +
			(curTime.toLocalTime().time().minute() - curTime.toUTC().time().minute()) * 60;// - daylight * 3600;
		offlineDateTime = offlineDateTime.addSecs(offset);

		msg->date = offlineDateTime;
		socket->read(1);

		if (!buddyList.contains(msg->fromUin))
		{
			if ( messageType == 0x06  &&
					!checkMessageForValidation(msg->fromUin, msg->message, 1))
			{
				return;
			}
			if ( !checkMessageForValidation(msg->fromUin, msg->message, 0))
			{
				return;
			}
		}

		if ( buddyList.contains(msg->fromUin) )
		{
			treeBuddyItem *buddy = buddyList.value(msg->fromUin);
			msg->from = buddy->getName();

			messageFromList = true;

		}
    else
    {
                QSettings settings(QSettings::NativeFormat, QSettings::UserScope, "qutim/qutim."+m_profile_name+"/ICQ."+account_name, "contactlist");

					treeGroupItem *group = groupList.value(0);
					msg->from = msg->fromUin;
					treeBuddyItem *buddy = new treeBuddyItem(icqUin, m_profile_name);
					initializeBuddy(buddy);
					buddy->underline = !dontUnderlineNotAutho;
					buddy->groupID = 0;
					buddy->groupName = group->name;
					group->userCount++;
					group->updateText();
					buddyList.insert(msg->fromUin, buddy);
					buddy->setBuddyUin(msg->fromUin);
					buddy->setName(msg->fromUin);
					buddy->updateBuddyText();
					requestUinInformation(buddy->getUin());
					settings.beginGroup(buddy->getUin());
					settings.setValue("name", buddy->getUin());
					settings.setValue("groupid", 0);
					settings.setValue("nickname", buddy->getName());
					settings.endGroup();
					addContactToCL(0,buddy->getUin(), buddy->getName());
					QStringList contacts = settings.value("list/contacts").toStringList();
					contacts<<buddy->getUin();
					settings.setValue("list/contacts", contacts);

					if ( contactListChanged  && isMergeAccounts )
								emit reupdateList();

					messageFromList = false;


		}

	if ( messageType == 0x01 )
	{
		addMessageFromContact(msg->fromUin,
			buddyList.contains(msg->fromUin)?buddyList.value(msg->fromUin)->groupID : 0
			, msg->message, offlineDateTime);

	}
}

void contactListTree::updateSorting()
{
	foreach( treeGroupItem *group, groupList)
	{
		group->updateOnline();
	}
}

void contactListTree::createContactList()
{
        QSettings settings(QSettings::NativeFormat, QSettings::UserScope, "qutim/qutim."+m_profile_name+"/ICQ."+account_name, "contactlist");
	getGroups = settings.value("list/group").toStringList();
	getBuddies = settings.value("list/contacts").toStringList();
	visibleList = settings.value("list/visible").toStringList();
	invisibleList = settings.value("list/invisible").toStringList();
	ignoreList = settings.value("list/ignore").toStringList();
	if (getGroups.size())
	{
	foreach(const QString &groupId, getGroups)
	{
		treeGroupItem *group = new treeGroupItem;
		group->setOnOffLists();
		groupList.insert(groupId.toInt(), group);
		group->setGroupText(settings.value(groupId + "/name").toString());
		addGroupToCL(groupId.toInt(), group->name);
	}
	createNil();


        // Days to birthday
        int birthTo;
	if ( getBuddies.size())
	{
		foreach(const QString &buddyUin, getBuddies)
		{
			int group_id = settings.value(buddyUin + "/groupid").toInt();
			if ( treeGroupItem *group = groupList.value(group_id) )
			{
					treeBuddyItem *buddy = new treeBuddyItem(icqUin, m_profile_name);
					initializeBuddy(buddy);
					settings.beginGroup(buddyUin);
					buddy->underline = !dontUnderlineNotAutho;
					buddy->groupID = settings.value("groupid").toInt();

					buddy->birth = !hideBirth;
					buddy->birthDay = QDate(settings.value("birthyear", 0).toInt(),
							settings.value("birthmonth", 0).toInt(),
							settings.value("birthday", 0).toInt());

                                // if birthday is comming -> play sound
                                birthTo = QDate::currentDate().daysTo(buddy->birthDay);
                                if ((birthTo >= 0) && (birthTo <= 3))
                                // we don't check if birthday sound has already been played
                                // for today. SoundEvents class does it for us.
                                		notifyAboutBirthday(buddy->getUin(), buddy->groupID);

					buddy->groupName = group->name;
					group->userCount++;
					group->updateText();
					buddyList.insert(buddyUin, buddy);
					buddy->setBuddyUin(buddyUin);
					buddy->setName(settings.value("nickname").toString());

					addContactToCL(group_id, buddyUin, buddy->getName());
					buddy->setAvatarHash(QByteArray::fromHex(settings.value("iconhash").toByteArray()));
					buddy->setNotAuthorizated(!settings.value("authorized",true).toBool());
					buddy->lastonlineTime = settings.value("lastonline",0).toInt();
					buddy->updateBuddyText();
					settings.endGroup();

					updateNil();

			}
		}
	}
	}
	if ( clearNil )
		clearNilUsers();


	QStringList chatWithList = settings.value("list/chatwindow").toStringList();
	foreach(const QString &buddyUin, chatWithList)
	{
		if ( buddyList.contains(buddyUin ) )
			doubleClickedBuddy(buddyList.value(buddyUin));
	}
	settings.remove("list/chatwindow");

	setPrivacyIconsToContacts();
}

void contactListTree::createSoundEvents()
{

}

void contactListTree::createNil()
{
	treeGroupItem *group = new treeGroupItem;
	group->setOnOffLists();
	groupList.insert(0, group);
	group->setGroupText("");
}

void contactListTree::updateNil()
{

}

void contactListTree::goingOnline(bool iAmOnlineSignal)
{
	TreeModelItem item;
	item.m_protocol_name = "ICQ";
	item.m_account_name = icqUin;
	item.m_item_type = TreeModelItem::Account;
	m_icq_plugin_system.setAccountIsOnline(item, iAmOnlineSignal);
	if ( iAmOnline = iAmOnlineSignal )
	{
//		foreach(chatWindow *w, chatWindowList)
//		w->setOnline(true);

		findUser->setEnabled(true);
		sendMultiple->setEnabled(true);
		changePassword->setEnabled(true);
		if ( privacyListWindowOpen)
			privacyWindow->setOnline(true);
//		privacyList->setEnabled(true);

	} else {
/*		fileTransferObject->disconnectFromAll();*/
		visibleList.clear();
		invisibleList.clear();
		ignoreList.clear();
		findUser->setEnabled(false);
		sendMultiple->setEnabled(false);
		changePassword->setEnabled(false);
		waitForIconUpload = false;

		if ( privacyListWindowOpen)
			privacyWindow->setOnline(false);

//		privacyList->setEnabled(false);
		foreach(treeBuddyItem *buddy, buddyList)
		{
			if ( !buddy->isOffline )
			{

				treeGroupItem *group = groupList.value(buddy->groupID);

				buddy->buddyOffline();
				updateChatBuddyStatus(buddy->getUin(), (statusIconClass::getInstance()->*(buddy->statusIconMethod))());
					group->buddyOffline();
			}
		}

//		foreach(chatWindow *w, chatWindowList)
//			w->setOnline(false);
		buddyConnection->disconnectFromSST();
		avatarAddress.clear();
		avatarCookie.clear();

		avatartList.clear();
	}




}

void contactListTree::loadSettings()
{
        QSettings settings(QSettings::NativeFormat, QSettings::UserScope, "qutim/qutim."+m_profile_name, "icqsettings");
        QSettings account_settings(QSettings::NativeFormat, QSettings::UserScope, "qutim/qutim."+m_profile_name+"/ICQ."+account_name, "accountsettings");
	accountNickname = account_settings.value("main/nick", icqUin).toString();
	disableAvatars = settings.value("connection/disavatars", false).toBool();

	//messaging settings

	codepage = settings.value("general/codepage", "Windows-1251").toString();
	codec = QTextCodec::codecForName(codepage.toLocal8Bit());

	//status settings

	settings.beginGroup("statuses");

	webAware = settings.value("webaware", false).toBool();

	showXStatuses = settings.value("xstatus", true).toBool();

	showXstatusesinToolTips = settings.value("xstattool", true).toBool();
	m_notify_about_reading_status = settings.value("notify", true).toBool();
	settings.endGroup();

	settings.beginGroup("contacts");
	m_show_xstatus_icon = settings.value("xstaticon", true).toBool();
	m_show_birthday_icon = settings.value("birthicon", true).toBool();
	m_show_auth_icon = settings.value("authicon", true).toBool();
	m_show_vis_icon = settings.value("visicon", true).toBool();
	m_show_invis_icon = settings.value("invisicon", true).toBool();
	m_show_ignore_icon = settings.value("ignoreicon", true).toBool();
	m_show_xstatus_text = settings.value("xstattext", true).toBool();
	settings.endGroup();

}



void contactListTree::createOnOffGroups()
{

}

void contactListTree::removeGroups()
{

}

void contactListTree::prepareForMerge()
{

}

void contactListTree::showHideGroups()
{

}

void contactListTree::prepareForShowGroups()
{

}

void contactListTree::hideEmptyGroups(bool hide)
{

}

void contactListTree::showOfflineUsers()
{

}

void contactListTree::clearNilUsers()
{
	if ( groupList.contains(0))
	{
	groupList.value(0)->userCount = 0;
	groupList.value(0)->updateText();
        QSettings contacts(QSettings::NativeFormat, QSettings::UserScope, "qutim/qutim."+m_profile_name+"/ICQ."+account_name, "contactlist");
	QStringList notNil = contacts.value("list/contacts").toStringList();
	foreach(treeBuddyItem *buddy, buddyList)
	{
		if ( !buddy->groupID )
		{
			removeContactFromCl(0,buddy->getUin());
			notNil.removeAll(buddy->getUin());
			contacts.remove(buddy->getUin());
			getBuddies.removeAll(buddy->getUin());
			buddyList.remove(buddy->getUin());
			delete buddy;
		}
	}
	contacts.setValue("list/contacts", notNil);
	}
}

void contactListTree::updateBuddyListFlags()
{
	foreach(treeBuddyItem *buddy, buddyList)
	{
		buddy->underline = !dontUnderlineNotAutho;
		buddy->birth = !hideBirth;
		buddy->updateBuddyText();
	}
}

void contactListTree::updateGroupCustomFont()
{
	foreach(treeGroupItem *group, groupList)
		group->setCustomFont(grpFont.fontFamily, grpFont.fontSize, grpFont.fontColor);
}

void contactListTree::updateContactsCustomFont()
{

}


void contactListTree::msgSettingsChanged()
{
        QSettings settings(QSettings::NativeFormat, QSettings::UserScope, "qutim/qutim."+m_profile_name, "icqsettings");
	QString tmp_codepage = settings.value("general/codepage", "Windows-1251").toString();
	if ( tmp_codepage == codepage )
		return;
	codepage = tmp_codepage;
	codec = QTextCodec::codecForName(codepage.toLocal8Bit());
	settings.beginGroup("messaging");
	bool tbMd = settings.value("tab", true).toBool();

	bool shNms = settings.value("chatnames", true).toBool();
	quint8 tmstmp = settings.value("timestamp", 1).toInt();
	bool onEnter = settings.value("onenter", false).toBool();
	bool clsOnSnd = settings.value("closeonsend", false).toBool();
	bool sndTpng = settings.value("typing", false).toBool();
//	if ( showNames != shNms  || timestamp != tmstmp || sendOnEnter != onEnter ||
//			closeOnSend != clsOnSnd  || sendTyping != sndTpng )
//	{
//		showNames = shNms;
//		timestamp = tmstmp;
//		sendOnEnter = onEnter;
//		closeOnSend = clsOnSnd;
//		sendTyping = sndTpng;
//		foreach(chatWindow *w, chatWindowList)
//		{
//			w->showNames = showNames;
//			w->timestamp = timestamp;
//			w->setOnEnter(sendOnEnter);
//			w->closeOnSend = closeOnSend;
//			w->sendTyping = sendTyping;
//		}
//
//	}
	dontShowEvents = settings.value("event", false).toBool();
	openNew = settings.value("opennew", false).toBool();
//	delete codec;


	lightChatView = settings.value("lightchat", false).toBool();
	dontShowIncomingMessagesInTrayEvents = settings.value("dshowmsg", false).toBool();
	settings.endGroup();
}

void contactListTree::updateChatBuddyStatus(const QString &buddy, const QIcon &icon)
{
}

void contactListTree::initializeWindow(chatWindow *w)
{
}

void contactListTree::activateWindow(const QString &uin)
{
	if ( messageList.contains(uin) )
	{

		readMessageFrom(messageList.value(uin));
	}
}


void contactListTree::requestUinInformation(const QString &uin)
{
	emit incSnacSeq();
	emit incMetaSeq();
	metaInformation metaInfo(icqUin);
	metaInfo.sendShortInfoReq(tcpSocket, *flapSeq, *snacSeq, *metaSeq, uin);
	int tmpMetaSeq = ((*metaSeq )% 0x100) * 0x100 + ((*metaSeq )/ 0x100);
	metaInfoRequestList.insert(tmpMetaSeq ,uin);
	emit incFlapSeq();
}

quint16 contactListTree::byteArrayToLEInt16(const QByteArray &array)
{
	bool ok;
	quint16 tmp = array.toHex().toUInt(&ok,16);
	return ((tmp % 0x100) * 0x100 + (tmp)/ 0x100);
}

void contactListTree::readShortInfo(const metaInformation &meta, quint16 metaSeq)
{

	if ( waitForMineInfo && metaSeq == mineMetaSeq)
	{
		 if ( meta.nick.size() )
			 accountNickname = codec->toUnicode(meta.nick);
		 else
			 accountNickname = icqUin;
                        QSettings settings(QSettings::NativeFormat, QSettings::UserScope, "qutim/qutim."+m_profile_name+"/ICQ."+account_name, "accountsettings");
		 settings.setValue("main/nick", accountNickname);
		waitForMineInfo = false;
	}

	if ( metaInfoRequestList.contains(metaSeq) )
	{
		treeBuddyItem *buddy = buddyList.value(metaInfoRequestList.value(metaSeq));
		if ( meta.nick.size())
			buddy->setName(codec->toUnicode(meta.nick));
		else
			buddy->setName(buddy->getUin());
                QSettings settings(QSettings::NativeFormat, QSettings::UserScope, "qutim/qutim."+m_profile_name+"/ICQ."+account_name, "contactlist");
		buddy->updateBuddyText();
		settings.setValue(buddy->getUin() + "/nickname", buddy->getName());
		metaInfoRequestList.remove(metaSeq);
	}
}

void contactListTree::readMetaData(quint16 length, bool notAlone)
{
	quint16 metaSeq = byteArrayToInt16(socket->read(2));
	quint16 dataSubType = byteArrayToInt16(socket->read(2));
	metaInformation metaInfo(icqUin);
	quint8 success;
	switch( dataSubType )
	{
	case 0x0401:
		length = length - 4 - metaInfo.readShortInfo(socket);
		readShortInfo(metaInfo, metaSeq);
		if ( length  )
			socket->read(length);
		break;
	case 0xc800:
		length = length - 4 - metaInfo.readBasicUserInfo(socket);

		readBasicUserInfo(metaInfo, metaSeq);

		fullIndoEnd(metaSeq, notAlone);

		if ( length )
			socket->read(length);
		break;

	case 0xdc00:
		length = length - 4 - metaInfo.readMoreUserInfo(socket);

		readMoreUserInfo(metaInfo, metaSeq);

		fullIndoEnd(metaSeq, notAlone);

		if ( length )
			socket->read(length);
		break;
	case 0xd200:
		length = length - 4 - metaInfo.readWorkUserInfo(socket);

		readWorkUserInfo(metaInfo, metaSeq);

		fullIndoEnd(metaSeq, notAlone);

		if ( length )
			socket->read(length);
		break;
	case 0xf000:
		length = length - 4 - metaInfo.readInterestsUserInfo(socket);

		readInterestsUserInfo(metaInfo, metaSeq);

		fullIndoEnd(metaSeq, notAlone);

		if ( length > 0 )
			socket->read(length);
		break;
	case 0xe600:
		length = length - 4 - metaInfo.readAboutUserInfo(socket);

		readAboutUserInfo(metaInfo, metaSeq);

		fullIndoEnd(metaSeq, notAlone);

		if ( length > 0 )
			socket->read(length);
		break;
	case 0xeb00:
		fullIndoEnd(metaSeq, notAlone);
		socket->read(length - 4);
		break;
	case 0x0e01:
		fullIndoEnd(metaSeq, notAlone);
		socket->read(length - 4);
		break;
	case 0xfa00:
		fullIndoEnd(metaSeq, notAlone);
		socket->read(length - 4);
		break;
	case 0xa401:
		length = length - 4 - metaInfo.readSearchResult(socket, false);

		addSearchResult(false, metaInfo.founded, metaInfo.foundedUin, metaInfo.foundedNick,
				metaInfo.foundedFirst, metaInfo.foundedLast, metaInfo.foundedEmail,
				metaInfo.authFlag, metaInfo.foundedStatus, metaInfo.foundedGender,
				metaInfo.foundedAge);

		if ( length )
			socket->read(length);
		break;
	case 0xae01:
		length = length - 4 - metaInfo.readSearchResult(socket, true);

		addSearchResult(true, metaInfo.founded, metaInfo.foundedUin, metaInfo.foundedNick,
				metaInfo.foundedFirst, metaInfo.foundedLast, metaInfo.foundedEmail,
				metaInfo.authFlag, metaInfo.foundedStatus, metaInfo.foundedGender,
				metaInfo.foundedAge);

		if ( length )
			socket->read(length);
		break;

	case 0xaa00:
		success = convertToInt8(socket->read(1));
		length--;

		if ( success == 0x0a )
			emit sendSystemMessage(tr("Password is successfully changed"));
		else
			emit sendSystemMessage(tr("Password is not changed"));

		if ( length)
			socket->read(length);

		break;
	default:
		socket->read(length - 4);
	}
}

QByteArray contactListTree::convertToByteArray(const quint16 &d)
{
	QByteArray packet;
		packet[0] = (d / 0x100);
		packet[1] = (d % 0x100);
		return packet;
}

void contactListTree::sendTypingNotifications(const QString &uin, quint16 type)
{
	emit incSnacSeq();
	QByteArray packet;
	packet[0] = 0x2a;
	packet[1] = 0x02;

	packet.append(convertToByteArray((quint16)*flapSeq));


	snac snac0414;
	snac0414.setFamily(0x0004);
	snac0414.setSubType(0x0014);
	snac0414.setReqId(*snacSeq);
	quint16 length = 10;
	QByteArray typeNotif;
	typeNotif[0] = 0x00;
	typeNotif[1] = 0x00;
	typeNotif[2] = 0x00;
	typeNotif[3] = 0x00;
	typeNotif[4] = 0x00;
	typeNotif[5] = 0x00;
	typeNotif[6] = 0x00;
	typeNotif[7] = 0x00;
	typeNotif[8] = 0x00;
	typeNotif[9] = 0x01;
	typeNotif[10] = (quint8)(uin.length());
	typeNotif.append(uin);
	typeNotif.append(convertToByteArray((quint16)type));
	length += typeNotif.size();
	packet.append(convertToByteArray((quint16)length));
	packet.append(snac0414.getData());
	packet.append(typeNotif);

	tcpSocket->write(packet);
	emit incFlapSeq();
}

void contactListTree::getTypingNotification(quint16 length)
{
	int tmpLength = 13;
	socket->read(8);
	quint16 channel = byteArrayToInt16(socket->read(2));
	bool ok;
	quint8 uinlength = socket->read(1).toHex().toUInt(&ok, 16);
	QString uin(socket->read(uinlength));

	quint16 notifType = byteArrayToInt16(socket->read(2));

	if ( buddyList.contains( uin ))
	{
	if ( channel == 0x0001 )
	{
		if ( notifType  == 0x0002 )
		{
			if ( !notifList.count() )
				QTimer::singleShot(5000, this, SLOT(clearNotifList()));
			if ( !notifList.contains(uin) )
			{
				notifList<<uin;
				contactTyping(uin, buddyList.value(uin)->groupID, true);
			}
		} else if ( notifType == 0x0000)
		{
			contactTyping(uin, buddyList.value(uin)->groupID, false);
		}
	}
	} else {
		contactTyping(uin, 0, true);
		if(typingNot)
			emit userMessage("", uin, "", typingNotification, false);
//		emit userMessage(tr("%1 is typing ( not from list )").arg(uin));
	}

	tmpLength += uinlength;
	if ( length - tmpLength > 0 )
		socket->read(length - tmpLength);
}

void contactListTree::statusSettingsChanged()
{
        QSettings settings(QSettings::NativeFormat, QSettings::UserScope, "qutim/qutim."+m_profile_name, "icqsettings");
	settings.beginGroup("statuses");
	showXstatusesinToolTips = settings.value("xstattool", true).toBool();
	bool wbAware = settings.value("webaware", false).toBool();
	bool autoAw = settings.value("autoaway", true).toBool();
	quint32 awMin = settings.value("awaymin", 10).toUInt();
	m_notify_about_reading_status = settings.value("notify", true).toBool();

	if ( wbAware != webAware )
	{
		webAware = wbAware;
		emit updateStatus();
	}

	if ( autoAw != autoAway || awMin != awayMin )
	{
		autoAway = autoAw;
		awayMin = awMin;
		emit restartAutoAway(autoAway, awayMin);
	}

	emit updateStatusMenu(settings.value("customstat", true).toBool());
	settings.endGroup();
}

void contactListTree::initializeBuddy(treeBuddyItem * buddy )
{
	buddy->m_show_xstatus_icon = m_show_xstatus_icon;
	buddy->m_show_birthday_icon = m_show_birthday_icon;
	buddy->m_show_auth_icon = m_show_auth_icon;
	buddy->m_show_vis_icon = m_show_vis_icon;
	buddy->m_show_invis_icon = m_show_invis_icon;
	buddy->m_show_ignore_icon = m_show_ignore_icon;
	buddy->m_show_xstatus_text= m_show_xstatus_text;
	buddy->updateIcons();
}

void contactListTree::changePrivacy(quint8 flag)
{
        QSettings settings(QSettings::NativeFormat, QSettings::UserScope, "qutim/qutim."+m_profile_name+"/ICQ."+account_name, "accountsettings");
	settings.setValue("statuses/privacy", flag);


	emit incSnacSeq();
	servicesSetup privacySetup(icqUin, m_profile_name);
	privacySetup.flap1309seq = *flapSeq;
	privacySetup.snac1309seq = *snacSeq;
	privacySetup.setPrivacy(icqUin, pdInfoID, pdInfoGroupId, tcpSocket);
	emit incFlapSeq();
}

QString contactListTree::getCurrentAwayMessage() const
{
        QSettings settings(QSettings::NativeFormat, QSettings::UserScope, "qutim/qutim."+m_profile_name+"/ICQ."+account_name, "accountsettings");
	QString awayMessage;
	switch(currentStatus)
	{
	case away:
		awayMessage = settings.value("autoreply/awaymsg", "").toString();
		awayMessage.append(QChar(0x00));
		return awayMessage;
	case lunch:
		awayMessage = settings.value("autoreply/lunchmsg", "").toString();
		awayMessage.append(QChar(0x00));
		return awayMessage;

	case evil:
		awayMessage = settings.value("autoreply/evilmsg", "").toString();
		awayMessage.append(QChar(0x00));
		return awayMessage;

	case depression:
		awayMessage = settings.value("autoreply/depressionmsg", "").toString();
		awayMessage.append(QChar(0x00));
		return awayMessage;

	case athome:
		awayMessage = settings.value("autoreply/athomemsg", "").toString();
		awayMessage.append(QChar(0x00));
		return awayMessage;

	case atwork:
		awayMessage = settings.value("autoreply/atworkmsg", "").toString();
		awayMessage.append(QChar(0x00));
		return awayMessage;

	case na:
		awayMessage = settings.value("autoreply/namsg", "").toString();
		awayMessage.append(QChar(0x00));
		return awayMessage;

	case occupied:
		awayMessage = settings.value("autoreply/occupiedmsg", "").toString();
		awayMessage.append(QChar(0x00));
		return awayMessage;

	case dnd:
		awayMessage = settings.value("autoreply/dndmsg", "").toString();
		awayMessage.append(QChar(0x00));
		return awayMessage;

	default:
		return QChar(0x00);
	}
}


void contactListTree::appExiting()
{


}

void contactListTree::loadUnreadedMessages()
{

}

void contactListTree::showHistory(const QString &uin)
{


}

void contactListTree::showServiceHistory()
{

}

void contactListTree::setServiceMessageToWin(const QString &uin, const QString &msg)
{
	if ( buddyList.contains(uin) )
	{
		treeBuddyItem *buddy = buddyList.value(uin);
		addServiceMessage(uin, buddy->groupID, msg);
	}
}

void contactListTree::setHideSeparators(bool hide)
{

}


void contactListTree::startChatWith(const QString &uin)
{
	if ( buddyList.contains(uin) )
	{
		doubleClickedBuddy(buddyList.value(uin));
	}
}

bool contactListTree::checkBuddyPictureHash(const QByteArray &hash)
{
        QSettings settings(QSettings::NativeFormat, QSettings::UserScope, "qutim/qutim."+m_profile_name, "icqsettings");
	return QFile::exists(settings.fileName().section('/', 0, -2) + "/icqicons/" + hash.toHex());

}

void contactListTree::askForAvatars(const QByteArray &hash, const QString &uin)
{



	if ( !hash.isEmpty() && (hash.size() == 16 ))
	{
		if ( !checkBuddyPictureHash(hash) )
		{

			QHostAddress hostAddr = QHostAddress(avatarAddress);
			if ( !hostAddr.isNull())
			{

				if ( !buddyConnection->connectedToServ )
				{

					avatartList.insert(uin, hash);
                                        buddyConnection->connectToServ(avatarAddress, avatarPort, avatarCookie/*, tcpSocket->proxy()*/);

				}
				else
				{
					if ( buddyConnection->canSendReqForAvatars )
					{


						buddyConnection->sendHash(uin,hash);
					}
					else
					{

						avatartList.insert(uin, hash);
					}
				}
			} else {

				avatartList.insert(uin, hash);
			}

		} else {
                        QSettings settings(QSettings::NativeFormat, QSettings::UserScope, "qutim/qutim."+m_profile_name+"/ICQ."+account_name, "contactlist");
			settings.setValue(uin + "/iconhash", hash.toHex());
		}
	}



}

void contactListTree::sendReqForRedirect()
{
	emit incSnacSeq();

	QByteArray packet;
	packet[0] = 0x2a;
	packet[1] = 0x02;
	packet.append(convertToByteArray((quint16)*flapSeq));

	packet.append(convertToByteArray((quint16)12));
	snac snac0104;
	snac0104.setFamily(0x0001);
	snac0104.setSubType(0x0004);
	snac0104.setReqId(*snacSeq);
	packet.append(snac0104.getData());

	packet.append(convertToByteArray((quint16)0x0010));



	emit incFlapSeq();

	tcpSocket->write(packet);
}

void contactListTree::readSSTserver(quint16 length)
{

	socket->read(2);
	length -= 2;
	quint16 familyID = 0;

		for ( ;length > 0; )
		{
			tlv tmpTlv;
			tmpTlv.readData(socket);
			length -= tmpTlv.getLength();
			switch(tmpTlv.getTlvType())
			{
			case 0x000d:

				familyID = byteArrayToInt16(tmpTlv.getTlvData());
				break;
			case 0x0005:

				avatarAddress = tmpTlv.getTlvData();
				break;

			case 0x0006:
				avatarCookie = tmpTlv.getTlvData();
				break;
			default:
				;
			}
		}
		if ( familyID != 0x0010 )
		{
			avatarPort = 0;
		} else
			avatarPort = tcpSocket->peerPort();

		if ( length )
			socket->read(length);


		if ( avatartList.count() || waitForIconUpload )
		{
			QHostAddress hostAddr = QHostAddress(avatarAddress);
						if ( !hostAddr.isNull())
							if ( !buddyConnection->connectedToServ )
                                                                buddyConnection->connectToServ(avatarAddress, avatarPort, avatarCookie/*, tcpSocket->proxy()*/);
		}

		if ( !disableAvatars )
		{
		QHostAddress hostAddr = QHostAddress(avatarAddress);
		if ( !hostAddr.isNull())
				if ( !buddyConnection->connectedToServ )
                                                buddyConnection->connectToServ(avatarAddress, avatarPort, avatarCookie/*, tcpSocket->proxy()*/);
		}


}

void contactListTree::emptyAvatarList()
{
	if ( avatartList.count() )
	{
		foreach(const QString &uin, avatartList.keys() )
		{
			askForAvatars(avatartList.value(uin), uin);
		}

		avatartList.clear();
	}

	if ( waitForIconUpload )
	{
		buddyConnection->uploadIcon(ownIconPath);
		waitForIconUpload = false;
	}
}

void contactListTree::updateAvatar(const QString &uin, QByteArray hash)
{
	if ( buddyList.contains(uin) )
	{
		buddyList.value(uin)->setAvatarHash(hash);
	}
}



void contactListTree::notifyBlockedMessage(const QString &from, const QString &message)
{
	emit userMessage(from, from, message, blockedMessage, false);
}

void contactListTree::saveBlocked(const QString &from, const QString& msg, const QDateTime &date)
{

}

bool contactListTree::checkMessageForUrl(const QString &msg)
{


	bool containsURLs = false;
	containsURLs = msg.contains("http:", Qt::CaseInsensitive) ? true : containsURLs;
	containsURLs = msg.contains("ftp:", Qt::CaseInsensitive) ? true : containsURLs;
	containsURLs = msg.contains("www.", Qt::CaseInsensitive) ? true : containsURLs;
	return containsURLs;
}


bool contactListTree::turnOnAntiSpamBot(const QString &from, const QString &msg, const QDateTime &date)
{
        QSettings settings(QSettings::NativeFormat, QSettings::UserScope, "qutim/qutim."+m_profile_name+"/ICQ."+account_name, "accountsettings");
	quint32 privacy = settings.value("statuses/privacy", 4).toUInt();


	if ( notifyAboutBlocked)
			notifyBlockedMessage(from, msg);

	if ( saveServiceHistory )
		saveBlocked(from, msg, date);

	if ( dontAnswerBotIfInvis && (currentStatus == invisible ||
				privacy == 5) )
			return true;

	if ( msg == answer)
	{
		messageFormat sendAnswerMessage;
		sendAnswerMessage.fromUin = from;
		sendAnswerMessage.message = messageAfterAnswer;
		emit incSnacSeq();
		icqMessage message(codepage);

		message.sendMessage(tcpSocket, sendAnswerMessage,*flapSeq, *snacSeq, false);
		emit incFlapSeq();
		blockedBotList.removeAll(from);
		return false;
	}

	if ( !blockedBotList.contains(from) )
	{
		blockedBotList.append(from);
		messageFormat sendAnswerMessage;
		sendAnswerMessage.fromUin = from;
		sendAnswerMessage.message = question;
		emit incSnacSeq();
		icqMessage message(codepage);

		message.sendMessage(tcpSocket, sendAnswerMessage,*flapSeq, *snacSeq, false);
		emit incFlapSeq();
		return true;
	}

	return true;

}

void contactListTree::setAvatarDisabled(bool disable)
{
	disableAvatars = disable;

}

void contactListTree::initializaMenus(QMenu *accountAdditionalMenu)
{
//	serviceMessages = new QAction(m_icq_plugin_system.getIcon(""), icqUin, this);
//	connect(serviceMessages, SIGNAL(triggered()), this, SLOT(showServiceHistory()));

	findUser = new QAction(m_icq_plugin_system.getIcon("search"),
			tr("Add/find users"), this);
	connect(findUser, SIGNAL(triggered()), this, SLOT(findAddUser()));
	findUser->setEnabled(false);

	sendMultiple = new QAction(m_icq_plugin_system.getIcon("multiple"),
			tr("Send multiple"), this);
	connect(sendMultiple, SIGNAL(triggered()), this, SLOT(sendMultipleWindow()));
	sendMultiple->setEnabled(false);

	privacyList = new QAction(m_icq_plugin_system.getIcon("privacylist"),
			tr("Privacy lists"), this);
	connect(privacyList, SIGNAL(triggered()), this, SLOT(openPrivacyWindow()));
//	privacyList->setEnabled(false);

	selfInfo = new QAction(m_icq_plugin_system.getIcon("changedetails"),
			tr("View/change my details"), this);
	connect(selfInfo, SIGNAL(triggered()), this, SLOT(openSelfInfo()));

	changePassword = new QAction(m_icq_plugin_system.getIcon("password"),
			tr("Change my password"), this);
	connect(changePassword, SIGNAL(triggered()), this, SLOT(openChangePasswordDialog()));
	changePassword->setEnabled(false);

	accountAdditionalMenu->addAction(findUser);
//	accountAdditionalMenu->addAction(serviceMessages);
	accountAdditionalMenu->addAction(sendMultiple);
	accountAdditionalMenu->addAction(privacyList);
	accountAdditionalMenu->addAction(selfInfo);
	accountAdditionalMenu->addAction(changePassword);

}

void contactListTree::findAddUser()
{
	searchWin = new searchUser (m_profile_name);
	connect ( searchWin, SIGNAL(openChatWithFounded(const QString &, const QString &)),
			this, SLOT(openChatWindowWithFounded(const QString &, const QString &)));
	connect ( searchWin, SIGNAL(openInfoWindow(const QString &, const QString &, const QString &, const QString &)),
				this, SLOT(openInfoWindow(const QString &, const QString &, const QString &, const QString &)));
	connect( searchWin, SIGNAL(checkStatusFor(const QString &)),
					this, SLOT(checkStatusFor(const QString &)));
	connect( searchWin, SIGNAL(addUserToContactList(const QString &, const QString &, bool)),
					this, SLOT(addUserToList(const QString &, const QString &,bool)));
	findUserWindowOpen = true;
	searchWin->setAttribute(Qt::WA_QuitOnClose, false);
	searchWin->setAttribute(Qt::WA_DeleteOnClose, true);
	connect( searchWin, SIGNAL(destroyed ( QObject *)),
			this, SLOT(findUserWindowClosed(QObject *)));
	connect( searchWin, SIGNAL(findAskedUsers(int)),
				this, SLOT(searchForUsers(int)));
	findUser->setEnabled(false);
	searchWin->show();
}

void contactListTree::findUserWindowClosed(QObject */*obj*/)
{
	findUserWindowOpen = false;
	findUser->setEnabled(true);
}

void contactListTree::searchForUsers(int index)
{
	if ( tcpSocket->state() == QAbstractSocket::ConnectedState)
	{
		if ( index == 0)
		{
			emit incSnacSeq();
			emit incMetaSeq();
			metaInformation metaInfo(icqUin);
			metaInfo.searchByUin(tcpSocket, *flapSeq, *snacSeq, *metaSeq, searchWin->getUin());
			emit incFlapSeq();
		} else if ( index == 1 )
		{
			emit incSnacSeq();
			emit incMetaSeq();
			metaInformation metaInfo(icqUin);
			metaInfo.searchByEmail(tcpSocket, *flapSeq, *snacSeq, *metaSeq, searchWin->getEmail());
			emit incFlapSeq();
		} else if (index == 2)
		{
			emit incSnacSeq();
			emit incMetaSeq();
			metaInformation metaInfo(icqUin);
			metaInfo.searchByOther(tcpSocket, *flapSeq, *snacSeq, *metaSeq,
					searchWin->onlineOnly(), codec->fromUnicode(searchWin->getNick()), codec->fromUnicode(searchWin->getFirst()),
					codec->fromUnicode(searchWin->getLast()), searchWin->gender, searchWin->minAge, searchWin->maxAge,
					searchWin->countryCode, codec->fromUnicode(searchWin->getCity()), searchWin->interestsCode,
					searchWin->languageCode, searchWin->occupationCode, codec->fromUnicode(searchWin->getKeyWords()));
			emit incFlapSeq();
		}


	}
}

void contactListTree::addSearchResult(bool last, bool founded, const QString &uin, const QString &nick,
		const QString &firstName, const QString &lastName, const QString &email, const quint8 &authFlag,
		const quint16 &status, const quint8 &gender, const quint16 &age)
{

	QByteArray nickArray;
	nickArray.append(nick);
	QByteArray firstArray;
	firstArray.append(firstName);
	QByteArray lastArray;
	lastArray.append(lastName);



	if ( findUserWindowOpen )
		searchWin->addFoundedContact(last,founded, uin, codec->toUnicode(nickArray),
				codec->toUnicode(firstArray), codec->toUnicode(lastArray),
				email, authFlag, status, gender, age);
}

void contactListTree::openChatWindowWithFounded(const QString &uin, const QString &nick)
{

	if ( buddyList.contains(uin) )

	{
		createChat(uin, buddyList.value(uin)->groupID);
//		doubleClickedBuddy(buddyList.value(uin));
	} else {

                QSettings settings(QSettings::NativeFormat, QSettings::UserScope, "qutim/qutim."+m_profile_name+"/ICQ."+account_name, "contactlist");

		treeGroupItem *group = groupList.value(0);
		treeBuddyItem *buddy = new treeBuddyItem(icqUin,m_profile_name);
		initializeBuddy(buddy);
		buddy->underline = !dontUnderlineNotAutho;
		buddy->groupID = 0;
		buddy->groupName = group->name;
		group->userCount++;
		group->updateText();
		buddyList.insert(uin, buddy);
		buddy->setBuddyUin(uin);
		buddy->setName(nick);
		buddy->updateBuddyText();
		updateNil();
//		requestUinInformation(buddy->getUin());
		settings.beginGroup(buddy->getUin());
		settings.setValue("name", buddy->getUin());
		settings.setValue("groupid", 0);
		settings.setValue("nickname",nick);
		settings.endGroup();
		addContactToCL(0, buddy->getUin(), buddy->getName());
		QStringList contacts = settings.value("list/contacts").toStringList();
		contacts<<buddy->getUin();
		settings.setValue("list/contacts", contacts);

		createChat(uin, 0);
		//doubleClickedBuddy(buddy);

	}
}


void contactListTree::openInfoWindow(const QString &uin, const QString &nick, const QString &firstName, const QString &lastName)
{
	if ( infoWindowList.contains(uin) && uin != icqUin)
		return;

	userInformation *infoWin;
	if ( buddyList.contains(uin))
	{
		infoWin = new userInformation(m_profile_name, false, true, uin, icqUin);

		treeBuddyItem *buddy = buddyList.value(uin);

		infoWin->setAdditional(buddy->externalIP, buddy->internalIP,
				buddy->onlineTime, buddy->signonTime, buddy->regTime, buddy->idleSinceTime, buddy->getStatus(),
				buddy->clientId, buddy->capabilitiesList, buddy->shortCaps,
				buddy->lastInfoUpdate, buddy->lastExtStatusInfoUpdate,
				buddy->lastExtInfoUpdate, !buddy->isOffline, buddy->protocolVersion,
				buddy->userPort, buddy->Cookie);
		if ( iAmOnline)
			askForFullUserInfo(uin);
	}
	else
	{
		if ( uin == icqUin )
		{
			infoWin = new userInformation(m_profile_name, true, true, uin, icqUin);
			selfInfo->setEnabled(false);
		}
		else
		{
			infoWin = new userInformation(m_profile_name, false, false, uin, icqUin);
		}
	}
	infoWin->setAttribute(Qt::WA_QuitOnClose, false);
	infoWin->setAttribute(Qt::WA_DeleteOnClose, true);
	connect( infoWin, SIGNAL(destroyed ( QObject *)),
			this, SLOT(infoUserWindowClosed(QObject *)));
	connect( infoWin, SIGNAL(requestUserInfo(const QString &)),
				this, SLOT(askForFullUserInfo(const QString &)));
	connect( infoWin, SIGNAL(saveOwnerInfo(bool,const QString &)),
				this, SLOT(saveOwnerInfo(bool, const QString &)));

	infoWindowList.insert(uin, infoWin);

	if ( !buddyList.contains(uin) )
	{

		infoWin->setNick(nick);
		infoWin->setFirst(firstName);
		infoWin->setLast(lastName);
		askForFullUserInfo(uin);
	}

	infoWin->show();
}

void contactListTree::infoUserWindowClosed(QObject *obj)
{
	userInformation *tempWindow = (userInformation *)(obj);

	if ( tempWindow->contactUin == icqUin )
		selfInfo->setEnabled(true);

	infoWindowList.remove(infoWindowList.key(tempWindow));
}

void contactListTree::askForFullUserInfo(const QString &uin)
{
	emit incSnacSeq();
	emit incMetaSeq();
	metaInformation metaInfo(icqUin);
	metaInfo.getFullUserInfo(tcpSocket, *flapSeq, *snacSeq, *metaSeq, uin);
	int tmpMetaSeq = ((*metaSeq )% 0x100) * 0x100 + ((*metaSeq )/ 0x100);
	fullInfoRequests.insert(tmpMetaSeq ,uin);
	emit incFlapSeq();
}

void contactListTree::readBasicUserInfo(const metaInformation &metInfo, quint16 metSeqNum)
{

	if ( infoWindowList.contains(fullInfoRequests.value(metSeqNum)) && metInfo.basicInfoSuccess)
	{
		userInformation *infoWin = infoWindowList.value(fullInfoRequests.value(metSeqNum));

		infoWin->setNick(codec->toUnicode(metInfo.basicNick));
		infoWin->setFirst(codec->toUnicode(metInfo.basicFirst));
		infoWin->setLast(codec->toUnicode(metInfo.basicLast));
		infoWin->setEmail(codec->toUnicode(metInfo.basicEmail));
		infoWin->setHomeCity(codec->toUnicode(metInfo.basicCity));
		infoWin->setHomeState(codec->toUnicode(metInfo.basicState));
		infoWin->setHomePhone(codec->toUnicode(metInfo.basicPhone));
		infoWin->setHomeFax(codec->toUnicode(metInfo.basicFax));
		infoWin->setHomeAddress(codec->toUnicode(metInfo.basicAddress));
		infoWin->setCell(codec->toUnicode(metInfo.basicCell));
		infoWin->setHomeZip(codec->toUnicode(metInfo.basicZip));
		infoWin->setCountry(metInfo.country);
		infoWin->setAuth(metInfo.basicAuthFlag, metInfo.webAware, metInfo.publishEmail);

	}

	if ( !metInfo.basicInfoSuccess )
		fullIndoEnd(metSeqNum, false);
}

void contactListTree::fullIndoEnd(quint16 metaSeqNum, bool notAlone)
{
	if ( !notAlone )
	{
		if ( infoWindowList.contains(fullInfoRequests.value(metaSeqNum)))
			infoWindowList.value(fullInfoRequests.value(metaSeqNum))->enableRequestButton();
		fullInfoRequests.remove(metaSeqNum);

	}
}

void contactListTree::readMoreUserInfo(const metaInformation &metInfo, quint16 metSeqNum)
{

	if ( infoWindowList.contains(fullInfoRequests.value(metSeqNum)) && metInfo.moreInfoSuccess)
	{
		userInformation *infoWin = infoWindowList.value(fullInfoRequests.value(metSeqNum));

		infoWin->setAge(metInfo.moreAge);
		infoWin->setGender(metInfo.moreGender);
		infoWin->setHomePage(codec->toUnicode(metInfo.homepage));
		infoWin->setBirthDay(metInfo.moreBirthYear, metInfo.moreBirthMonth, metInfo.moreBirthDay);
		infoWin->setLang(1, metInfo.moreLang1);
		infoWin->setLang(2, metInfo.moreLang2);
		infoWin->setLang(3, metInfo.moreLang3);
		infoWin->setOriginalCity(codec->toUnicode(metInfo.moreCity));
		infoWin->setOriginalState(codec->toUnicode(metInfo.moreState));
		infoWin->setOriginalCountry(metInfo.moreCountry);


	}

	if ( !metInfo.moreInfoSuccess)
		fullIndoEnd(metSeqNum, false);
}

void contactListTree::readWorkUserInfo(const metaInformation &metInfo, quint16 metSeqNum)
{

	if ( infoWindowList.contains(fullInfoRequests.value(metSeqNum)) && metInfo.workInfoSuccess)
	{
		userInformation *infoWin = infoWindowList.value(fullInfoRequests.value(metSeqNum));

		infoWin->setWorkCity(codec->toUnicode(metInfo.workCity));
		infoWin->setWorkState(codec->toUnicode(metInfo.workState));
		infoWin->setWorkPhone(codec->toUnicode(metInfo.workPhone));
		infoWin->setWorkFax(codec->toUnicode(metInfo.workFax));
		infoWin->setWorkAddress(codec->toUnicode(metInfo.workAddress));
		infoWin->setWorkZip(codec->toUnicode(metInfo.workZip));
		infoWin->setWorkCountry(metInfo.workCountry);
		infoWin->setWorkCompany(codec->toUnicode(metInfo.workCompany));
		infoWin->setWorkDepartment(codec->toUnicode(metInfo.workDepartment));
		infoWin->setWorkPosition(codec->toUnicode(metInfo.workPosition));
		infoWin->setWorkOccupation(metInfo.workOccupation);

		infoWin->setWorkWebPage(codec->toUnicode(metInfo.workWebPage));


	}

	if ( !metInfo.workInfoSuccess)
		fullIndoEnd(metSeqNum, false);
}

void contactListTree::readInterestsUserInfo(const metaInformation &metInfo, quint16 metSeqNum)
{

	if ( infoWindowList.contains(fullInfoRequests.value(metSeqNum)) && metInfo.interestsInfoSuccess)
	{
		userInformation *infoWin = infoWindowList.value(fullInfoRequests.value(metSeqNum));

		infoWin->setInterests(codec->toUnicode(metInfo.interKeyWords1), metInfo.interCode1, 1);
		infoWin->setInterests(codec->toUnicode(metInfo.interKeyWords2), metInfo.interCode2, 2);
		infoWin->setInterests(codec->toUnicode(metInfo.interKeyWords3), metInfo.interCode3, 3);
		infoWin->setInterests(codec->toUnicode(metInfo.interKeyWords4), metInfo.interCode4, 4);



	}

	if ( !metInfo.interestsInfoSuccess)
		fullIndoEnd(metSeqNum, false);
}

void contactListTree::readAboutUserInfo(const metaInformation &metInfo, quint16 metSeqNum)
{

	if ( infoWindowList.contains(fullInfoRequests.value(metSeqNum)) && metInfo.aboutInfoSuccess)
	{
		userInformation *infoWin = infoWindowList.value(fullInfoRequests.value(metSeqNum));

		infoWin->setAboutInfo(codec->toUnicode(metInfo.about));



	}

	if ( !metInfo.aboutInfoSuccess)
		fullIndoEnd(metSeqNum, false);
}

void contactListTree::checkStatusFor(const QString &uin)
{



	emit incSnacSeq();
	QByteArray packet;
	packet[0] = 0x2a;
	packet[1] = 0x02;
	packet.append(convertToByteArray((quint16)*flapSeq));
	packet.append(convertToByteArray((quint16)(15 + uin.length())));

	snac snac0215;
	snac0215.setFamily(0x0002);
	snac0215.setSubType(0x0015);
	snac0215.setReqId(*snacSeq);
	packet.append(snac0215.getData());

	packet.append(convertToByteArray((quint16)0x0000));
	packet.append(convertToByteArray((quint16)0x0005));
	packet[packet.size()] = (quint8)uin.length();
	packet.append(uin);
	tcpSocket->write(packet);

	emit incFlapSeq();

}

void contactListTree::getStatusCheck(quint16 length)
{
	quint8 uinLength = convertToInt8(socket->read(1));
	length --;

	QString uin = socket->read(uinLength);

	length -= uinLength;

	socket->read(2);

	length -= 2;

	quint16 arraySize = byteArrayToInt16(socket->read(2));
	length -= 2;
	bool statusPresent = false;
	tlv tlv06;
	for ( int i = 0; i < arraySize; i++ )
	{
			tlv tmpTlv;
			tmpTlv.readData(socket);
			if ( tmpTlv.getTlvType() == 0x0006)
			{
				tlv06 = tmpTlv;
				statusPresent = true;
			}
			length -= tmpTlv.getLength();
	}
	QString statusString;
	if ( statusPresent )
	{


		quint16 userstatus = tlv06.getTlvData().at(2) * 0x100 + tlv06.getTlvData().at(3);

		switch ( userstatus )
		{
				case 0x0000:
					statusString = convertToStringStatus(contactOnline);
					break;
				case 0x0001:
					statusString = convertToStringStatus(contactAway);
					break;
				case 0x0002:
				case 0x0013:
					statusString = convertToStringStatus(contactDnd);
					break;
				case 0x0004:
				case 0x0005:
					statusString = convertToStringStatus(contactNa);
					break;
				case 0x0010:
				case 0x0011:
					statusString = convertToStringStatus(contactOccupied);
					break;
				case 0x0020:
					statusString = convertToStringStatus(contactFfc);
					break;
				case 0x0100:
					statusString = convertToStringStatus(contactInvisible);
					break;
				case 0x2001:
					statusString = convertToStringStatus(contactLunch);
					break;
				case 0x3000:
					statusString = convertToStringStatus(contactEvil);
					break;
				case 0x4000:
					statusString = convertToStringStatus(contactDepression);
					break;
				case 0x5000:
					statusString = convertToStringStatus(contactAtHome);
					break;
				case 0x6000:
					statusString = convertToStringStatus(contactAtWork);
					break;
				default:
					statusString = convertToStringStatus(contactOnline);
				}
	} else {
		statusString = tr("is offline");
	}

	if ( length )
		socket->read(length);

	if ( buddyList.contains(uin))
		emit userMessage(uin, buddyList.value(uin)->getName(), statusString, customMessage, true);
//	else
	if( !buddyList.contains(uin) && !uin.isEmpty() )
	{
		emit userMessage(uin, uin, statusString, statusNotyfication, true);
	}
}

quint8 contactListTree::convertToInt8(const QByteArray &packet)
{
	bool ok;
	return packet.toHex().toUInt(&ok,16);
}

void contactListTree::addUserToList(const QString &uin, const QString &nick, bool authReq)
{
	if ( tcpSocket->state() == QAbstractSocket::ConnectedState )
	{

		quint16 groupId = 1;
	if ( buddyList.contains(uin) )
		groupId = buddyList.value(uin)->groupID;

	if ( !buddyList.contains(uin) || !groupId)
		{
		addBuddyDialog addDialog;
			addDialog.setTitle(uin);
			QStringList groups;
			foreach (treeGroupItem *group, groupList)
			{
				if ( groupList.key(group))
					groups<<group->name;
			}
			addDialog.setContactData(nick, groups);


			if ( QtopiaApplication::execDialog(&addDialog) )
			{


				if ( !groupId)
				{
                                        QSettings contacts(QSettings::NativeFormat, QSettings::UserScope, "qutim/qutim."+m_profile_name+"/ICQ."+account_name, "contactlist");

					QStringList contactList = contacts.value("list/contacts").toStringList();

					contactList.removeAll(uin);
					contacts.setValue("list/contacts", contactList);

					contacts.remove(uin);

					treeBuddyItem *buddy = buddyList.value(uin);

							idBuddyList.removeAll(buddy->itemId);

							if ( infoWindowList.contains(uin) )
							{
								infoWindowList.value(uin)->close();
								infoWindowList.remove(uin);
							}

//							if ( chatWindowList.contains(uin) )
//							{
//								chatWindowList.value(uin)->close();
//								chatWindowList.remove(uin);
//							}
							quint16 groupCount;
							QString groupName;
							treeGroupItem *group = groupList.value(buddy->groupID);

								group->buddiesList.removeAll(buddy->itemId);

								group->userCount--;
								group->updateText();
								groupCount = group->buddiesList.count();
								groupName = group->name;
							removeContactFromCl(buddy->groupID,uin);
							buddyList.remove(uin);
							delete buddy;

				}




				sendUserAddReq(uin, addDialog.getNick(), authReq, addDialog.getGroup());
			}
		}
	}

}

void contactListTree::sendUserAddReq(const QString &uin, QString nick, bool authReq, const QString &groupName)
{
	quint16 groupId = 0;
	foreach(treeGroupItem *group, groupList)
	{
		if ( groupName == group->name)
			groupId = groupList.key(group);
	}



	if ( !groupId)
		return;

	quint16 userId = rand() % 0x03e6;
	for ( ;idBuddyList.contains(userId);)
		userId = rand() % 0x03e6;



	if (nick.trimmed().isEmpty())
			nick = uin;


	QByteArray editPack;

	emit incSnacSeq();
	QByteArray packet;
	packet[0] = 0x2a;
	packet[1] = 0x02;
	packet.append(convertToByteArray((quint16)*flapSeq));
	if (authReq)
		packet.append(convertToByteArray((quint16)14));
	else
		packet.append(convertToByteArray((quint16)10));

	snac snac1311;
	snac1311.setFamily(0x0013);
	snac1311.setSubType(0x0011);
	snac1311.setReqId(*snacSeq);
	packet.append(snac1311.getData());

	if ( authReq )
	{
		packet.append(convertToByteArray((quint16)0x0001));
		packet.append(convertToByteArray((quint16)0x0000));
	}
	emit incFlapSeq();
	editPack.append(packet);

	emit incSnacSeq();
	QByteArray packet2;
	packet2[0] = 0x2a;
	packet2[1] = 0x02;
	packet2.append(convertToByteArray((quint16)*flapSeq));
	if ( authReq)
		packet2.append(convertToByteArray((quint16)(28 + uin.length() + nick.toUtf8().length())));
	else
		packet2.append(convertToByteArray((quint16)(24 + uin.length() + nick.toUtf8().length())));

	snac snac1308;
	snac1308.setFamily(0x0013);
	snac1308.setSubType(0x0008);
	snac1308.setReqId(*snacSeq);
	packet2.append(snac1308.getData());
	packet2.append(convertToByteArray((quint16)uin.length()));
	packet2.append(uin);
	packet2.append(convertToByteArray((quint16)groupId));
	packet2.append(convertToByteArray((quint16)userId));

	packet2.append(convertToByteArray((quint16)0x0000));

	if ( authReq )
		packet2.append(convertToByteArray((quint16)(8 + nick.toUtf8().length())));
	else
		packet2.append(convertToByteArray((quint16)(4 + nick.toUtf8().length())));



	packet2.append(convertToByteArray((quint16)0x0131));
	packet2.append(convertToByteArray((quint16)nick.toUtf8().length()));
	packet2.append(nick.toUtf8());

	if ( authReq )
	{
		packet2.append(convertToByteArray((quint16)0x0066));
		packet2.append(convertToByteArray((quint16)0x0000));
	}
	emit incFlapSeq();

	editPack.append(packet2);


	tcpSocket->write(editPack);


	modifyObject addBuddy;

	addBuddy.itemId = userId;
	addBuddy.groupId = groupId;
	addBuddy.itemType = 0;
	addBuddy.operation = 0;
	addBuddy.buddyUin = uin;
	addBuddy.buddyName = nick;
	addBuddy.authReq = authReq;

	modifyReqList.append(addBuddy);

//	treeGroupItem *group = groupList.value(0);
//	treeBuddyItem *buddy;
//	if ( showGroups )
//			buddy = new treeBuddyItem(statusIconObject->getOfflineIcon(), this, group->offlineList);
//	else
//			buddy = new treeBuddyItem(statusIconObject->getOfflineIcon(), this, offlineList);
//	initializeBuddy(buddy);
//	if ( authReq)
//		buddy->notAutho = true;
//	buddy->underline = !dontUnderlineNotAutho;
//	buddy->groupID = 0;
//	buddy->groupName = group->name;
//	buddyList.insert(uin, buddy);
//	buddy->setBuddyUin(uin);
//	buddy->setName(nick);
//	buddy->updateBuddyText();
//	buddy->setCustomFont(offFont.fontFamily, offFont.fontSize, offFont.fontColor);
//	buddy->setHidden(true);



}

void contactListTree::getModifyItemFromServer(quint16 length)
{
	socket->read(8);

	length -= 8;

	int count = length / 2;
	for ( int i = 0; i < count; i++)
	{
		quint16 code = byteArrayToInt16(socket->read(2));
		length -= 2;
		if ( modifyReqList.count())
		{

			if ( code == 0x0000 )
			{

				modifyObject tmpObject = modifyReqList.at(0);

				if ( tmpObject.itemType == 0 && tmpObject.operation == 0)
				{
					addModifiedBuddyToGroup(tmpObject.groupId, tmpObject.itemId, tmpObject.buddyUin, tmpObject.authReq, tmpObject.buddyName);
				}

				else if ( tmpObject.itemType == 1 && tmpObject.groupId && tmpObject.operation == 0)
				{
					addNewGroupToRoot(tmpObject.buddyName, tmpObject.groupId);
				}

				else if ( tmpObject.itemType == 1 && tmpObject.groupId && tmpObject.operation == 1)
				{
					renameGroupToName(tmpObject.buddyName, tmpObject.groupId);
				}

				else if ( tmpObject.itemType == 1 && tmpObject.groupId && tmpObject.operation == 2)
				{
					deleteSelectedGroup(tmpObject.groupId);
				}

				else if ( tmpObject.itemType == 0 && tmpObject.operation == 1)
				{
					renameContact(tmpObject.buddyUin, tmpObject.buddyName);
				}

				else if ( tmpObject.itemType == 0 && tmpObject.operation == 2)
				{
					removeContact(tmpObject.buddyUin);
				}

				else if ( tmpObject.itemType == 0x0014 && tmpObject.operation == 1)
				{
					avatarModified = true;
				}
			}
			if ( code == 0x000e)
			{
				modifyObject tmpObject = modifyReqList.at(0);

				if ( tmpObject.itemType == 0 && tmpObject.operation == 0)
				{
					sendUserAddReq(tmpObject.buddyUin, tmpObject.buddyName, true, groupList.value(tmpObject.groupId)->name);
				}
			}

			modifyReqList.removeAt(0);
		}
	}

	if ( length )
		socket->read(2);


//	if ( itemType == 0x0000)
//	{
//		if ( buddyList.contains(itemName) )
//		{
//			if ( buddyList.value(itemName)->groupID != groupId)
//				addModifiedBuddyToGroup(groupId, itemId, itemName);
//		}
//	}


}

void contactListTree::addModifiedBuddyToGroup(quint16 groupId, quint16 itemId, const QString &uin, bool authReq, const QString &nick)
{
        QSettings settings(QSettings::NativeFormat, QSettings::UserScope, "qutim/qutim."+m_profile_name+"/ICQ."+account_name, "contactlist");
	if ( groupList.contains(groupId))
	{

			treeGroupItem *newGrp = groupList.value(groupId);
			treeBuddyItem *buddy = new treeBuddyItem(icqUin, m_profile_name);
			initializeBuddy(buddy);

			buddy->underline = !dontUnderlineNotAutho;
			buddy->groupID = groupId;
			buddy->itemId = itemId;
			buddy->groupName = newGrp->name;
			buddyList.insert(uin, buddy);
			newGrp->userCount++;
			newGrp->updateText();
			buddy->setBuddyUin(uin);
			buddy->setName(nick);
			buddy->updateBuddyText();
			addContactToCL(groupId, uin, nick);
			if ( authReq)
				buddy->setNotAuthorizated(true);

			idBuddyList.append(itemId);


			buddyList.insert(uin, buddy);


		settings.beginGroup(uin);

		settings.setValue("groupid", groupId);
		settings.setValue("name", uin);
		settings.setValue("authorized", !buddy->getNotAutho());
		settings.setValue("nickname", buddy->getName());
		settings.setValue("lastonline", buddy->lastonlineTime);

		settings.endGroup();

		QStringList buddies = settings.value("list/contacts").toStringList();

		if ( ! buddies.contains(uin))
		{
						buddies<<uin;
						settings.setValue("list/contacts", buddies);
		}


		treeGroupItem *editGroup = groupList.value(groupId);

		modifyObject groupObject;
		groupObject.groupId = groupId;
		groupObject.itemId = 0x0000;
		groupObject.buddyName = editGroup->name;
		groupObject.itemType = 1;
		groupObject.operation = 3;
		modifyReqList.append(groupObject);

		QByteArray editClose;
		int groupItemCount = editGroup->buddiesList.count();

		emit incSnacSeq();
		QByteArray packet;
		packet[0] = 0x2a;
		packet[1] = 0x02;
		packet.append(convertToByteArray((quint16)*flapSeq));

		packet.append(convertToByteArray((quint16)(24 + editGroup->name.toUtf8().length() + groupItemCount * 2)));

		snac snac1309;
		snac1309.setFamily(0x0013);
		snac1309.setSubType(0x0009);
		snac1309.setReqId(*snacSeq);
		packet.append(snac1309.getData());


			packet.append(convertToByteArray((quint16)editGroup->name.toUtf8().length()));
			packet.append(editGroup->name.toUtf8());
			packet.append(convertToByteArray((quint16)groupId));
			packet.append(convertToByteArray((quint16)0x0000));
			packet.append(convertToByteArray((quint16)0x0001));
			packet.append(convertToByteArray((quint16)(4 + groupItemCount * 2)));

			packet.append(convertToByteArray((quint16)0x00c8));

			packet.append(convertToByteArray((quint16)(groupItemCount * 2)));

			for ( int i = 0; i < groupItemCount; i++)
			{
				packet.append(convertToByteArray((quint16)editGroup->buddiesList.at(i)));
			}
		emit incFlapSeq();

		editClose.append(packet);
		emit incSnacSeq();
		QByteArray packet2;
		packet2[0] = 0x2a;
		packet2[1] = 0x02;
		packet2.append(convertToByteArray((quint16)*flapSeq));

		packet2.append(convertToByteArray((quint16)10));

		snac snac1312;
		snac1312.setFamily(0x0013);
		snac1312.setSubType(0x0012);
		snac1312.setReqId(*snacSeq);
		packet2.append(snac1312.getData());

		editClose.append(packet2);
		emit incFlapSeq();
		tcpSocket->write(editClose);

	}
}


void contactListTree::youWereAdded(quint16 length)
{
	socket->read(8);
	length -= 8;

	quint8 uinLength = convertToInt8(socket->read(1));

	length -= 1;

	QString uin = socket->read(uinLength);

	length -= uinLength;

	QString addedMsg(tr("You were added"));

	if ( !buddyList.contains(uin) &&
			!checkMessageForValidation(uin, addedMsg, 0))
	{
		return;
	}


	messageFormat *msg = new messageFormat;
	msg->fromUin = uin;
	msg->message = addedMsg;
	msg->date = QDateTime::currentDateTime();

	if ( buddyList.contains(msg->fromUin) )
	{
		treeBuddyItem *buddy = buddyList.value(msg->fromUin);
		msg->from = buddy->getName();




	} else {

                QSettings settings(QSettings::NativeFormat, QSettings::UserScope, "qutim/qutim."+m_profile_name+"/ICQ."+account_name, "contactlist");

		treeGroupItem *group = groupList.value(0);
		msg->from = msg->fromUin;
		treeBuddyItem *buddy = new treeBuddyItem(icqUin, m_profile_name);
		initializeBuddy(buddy);
		buddy->underline = !dontUnderlineNotAutho;
		buddy->groupID = 0;
		buddy->groupName = group->name;
		group->userCount++;
		group->updateText();
		buddyList.insert(msg->fromUin, buddy);
		buddy->setBuddyUin(msg->fromUin);
		buddy->setName(msg->fromUin);
		buddy->updateBuddyText();
			updateNil();
		requestUinInformation(buddy->getUin());
		settings.beginGroup(buddy->getUin());
		settings.setValue("name", buddy->getUin());
		settings.setValue("groupid", 0);
		settings.setValue("nickname", buddy->getName());
		settings.endGroup();
		addContactToCL(0, buddy->getUin(), buddy->getName());
		QStringList contacts = settings.value("list/contacts").toStringList();
		contacts<<buddy->getUin();
		settings.setValue("list/contacts", contacts);

		if ( contactListChanged  && isMergeAccounts )
					emit reupdateList();


	}
	addMessageFromContact(msg->fromUin,
			buddyList.contains(msg->fromUin)?buddyList.value(msg->fromUin)->groupID : 0
			, msg->message, msg->date);
}

void contactListTree::sendMultipleWindow()
{
	multipleSendingWin = new multipleSending();
	multipleSendingWin->setTreeModel(icqUin, &groupList, &buddyList);
	multipleSendingOpen = true;
	multipleSendingWin->setAttribute(Qt::WA_QuitOnClose, false);
	multipleSendingWin->setAttribute(Qt::WA_DeleteOnClose, true);
		connect( multipleSendingWin, SIGNAL(destroyed ( QObject *)),
				this, SLOT(deleteSendMultipleWindow(QObject *)));
		connect( multipleSendingWin, SIGNAL(sendMessageToContact(const messageFormat &)),
				this, SLOT(sendMessage(const messageFormat &)));
	sendMultiple->setEnabled(false);
	multipleSendingWin->show();
}

void contactListTree::deleteSendMultipleWindow(QObject */*obj*/)
{
	multipleSendingOpen = false;
	sendMultiple->setEnabled(true);
}

void contactListTree::openPrivacyWindow()
{
	privacyWindow = new privacyListWindow(icqUin, m_profile_name);
	privacyWindow->setOnline(iAmOnline);
	privacyListWindowOpen = true;
	privacyWindow->setAttribute(Qt::WA_QuitOnClose, false);
	privacyWindow->setAttribute(Qt::WA_DeleteOnClose, true);
		connect( privacyWindow, SIGNAL(destroyed ( QObject *)),
				this, SLOT(deletePrivacyWindow(QObject *)));
		connect( privacyWindow, SIGNAL(openInfo( const QString &, const QString &, const QString &, const QString &)),
						this, SLOT(openInfoWindow(const QString &, const QString &, const QString &, const QString &)));
		connect( privacyWindow, SIGNAL(deleteFromPrivacyList(const QString &, int)),
						this, SLOT(deleteFromPrivacyList(const QString &, int)));
	privacyList->setEnabled(false);
	privacyWindow->show();
}

void contactListTree::deletePrivacyWindow(QObject */*obj*/)
{
	privacyListWindowOpen = false;
	privacyList->setEnabled(true);
}

void contactListTree::deleteFromPrivacyList(const QString &uin, int type)
{
        QSettings contacts(QSettings::NativeFormat, QSettings::UserScope, "qutim/qutim."+m_profile_name+"/ICQ."+account_name, "contactlist");

	emit incSnacSeq();
	QByteArray packet2;
	packet2[0] = 0x2a;
	packet2[1] = 0x02;
	packet2.append(convertToByteArray((quint16)*flapSeq));

	packet2.append(convertToByteArray((quint16)(20 + uin.length())));
	snac snac130a;
	snac130a.setFamily(0x0013);
	snac130a.setSubType(0x000a);
	snac130a.setReqId(*snacSeq);
	packet2.append(snac130a.getData());


	itemFromList object;
	quint16 objectType = 0x0002;

	if (type == 0)
	{
		object = visibleObjectList.value(uin);
		objectType = 0x0002;
		if ( buddyList.contains(uin))
		{
			buddyList.value(uin)->m_visible_contact = false;
			buddyList.value(uin)->setCustomIcon(QIcon(),5);
		}
		visibleList.removeAll(uin);
		visibleObjectList.remove(uin);
		contacts.setValue("list/visible", visibleList);
	}
	else if (type == 1)
	{
		object = invisibleObjectList.value(uin);
		objectType = 0x0003;
		if ( buddyList.contains(uin))
		{
			buddyList.value(uin)->m_invisible_contact = false;
			buddyList.value(uin)->setCustomIcon(QIcon(),6);
		}
		invisibleList.removeAll(uin);
		invisibleObjectList.remove(uin);
		contacts.setValue("list/invisible", invisibleList);
	}
	else if (type == 2)
	{
		object = ignoreObjectList.value(uin);
		objectType = 0x000e;
		if (  buddyList.contains(uin))
		{
			buddyList.value(uin)->m_ignore_contact = false;
			buddyList.value(uin)->setCustomIcon(QIcon(),7);
		}
		ignoreList.removeAll(uin);
		ignoreObjectList.remove(uin);
		contacts.setValue("list/ignore", ignoreList);
	}

	packet2.append(convertToByteArray((quint16)uin.length()));
	packet2.append(uin);

	packet2.append(convertToByteArray((quint16)0));
	packet2.append(convertToByteArray((quint16)object.itemId));
	packet2.append(convertToByteArray((quint16)objectType));

	modifyObject addBuddy;

	addBuddy.itemId = object.itemId;
	addBuddy.groupId = object.groupId;
	addBuddy.itemType = objectType;
	addBuddy.operation = 2;
	addBuddy.buddyUin = uin;

	modifyReqList.append(addBuddy);

	packet2.append(convertToByteArray((quint16)0x0000));
	emit incFlapSeq();
	tcpSocket->write(packet2);
}

void contactListTree::openSelfInfo()
{
	openInfoWindow(icqUin);
}

void contactListTree::saveOwnerInfo(bool saveAvatar, const QString &avatarPath)
{
	if ( infoWindowList.contains(icqUin))
	{
		userInformation *win = infoWindowList.value(icqUin);
		emit incSnacSeq();
		emit incMetaSeq();
		metaInformation metaInfo(icqUin);
		metaInfo.basicNick = codec->fromUnicode(win->getNick());
		metaInfo.basicFirst = codec->fromUnicode(win->getFirst());
		metaInfo.basicLast = codec->fromUnicode(win->getLast());
		metaInfo.basicEmail = codec->fromUnicode(win->getEmail());
		metaInfo.publishEmail = win->getPublish();
		metaInfo.country = win->getHomeCountry();

		metaInfo.basicCity = codec->fromUnicode(win->getHomeCity());
		metaInfo.basicState = codec->fromUnicode(win->getHomeState());
		metaInfo.zip = win->getHomeZip();

		metaInfo.basicPhone = codec->fromUnicode(win->getHomePhone());
		metaInfo.basicFax = codec->fromUnicode(win->getHomeFax());
		metaInfo.basicCell = codec->fromUnicode(win->getCellular());
		metaInfo.basicAddress = codec->fromUnicode(win->getHomeStreet());

		metaInfo.moreCountry = win->getOrigCountry();
		metaInfo.moreCity = codec->fromUnicode(win->getOrigCity());
		metaInfo.moreState = codec->fromUnicode(win->getOrigState());

		metaInfo.workCountry = win->getWorkCountry();
		metaInfo.workCity = codec->fromUnicode(win->getWorkCity());
		metaInfo.workState = codec->fromUnicode(win->getWorkState());
		metaInfo.wzip = win->getWorkZip();

		metaInfo.workPhone = codec->fromUnicode(win->getWorkPhone());
		metaInfo.workFax = codec->fromUnicode(win->getWorkFax());
		metaInfo.workAddress = codec->fromUnicode(win->getWorkStreet());

		metaInfo.workCompany = codec->fromUnicode(win->getCompanyName());
		metaInfo.workOccupation = win->getOccupation();
		metaInfo.workDepartment = codec->fromUnicode(win->getDepartment());
		metaInfo.workPosition = codec->fromUnicode(win->getPosition());
		metaInfo.workWebPage = codec->fromUnicode(win->getWebPage());

		metaInfo.foundedGender = win->getGender();
		metaInfo.homepage = codec->fromUnicode(win->getHomePage());
		metaInfo.setBirth = win->sendBirth();
		QDate birth = win->getBirth();

		metaInfo.moreBirthYear = birth.year();
		metaInfo.moreBirthMonth = birth.month();
		metaInfo.moreBirthDay = birth.day();

		metaInfo.moreLang1 = win->getLang1();
		metaInfo.moreLang2 = win->getLang2();
		metaInfo.moreLang3 = win->getLang3();


		metaInfo.interCode1 = win->getInterests(1);
		metaInfo.interKeyWords1 = codec->fromUnicode(win->getInterestString(1));


		metaInfo.interCode2 = win->getInterests(2);
		metaInfo.interKeyWords2 = codec->fromUnicode(win->getInterestString(2));

		metaInfo.interCode3 = win->getInterests(3);
		metaInfo.interKeyWords3 = codec->fromUnicode(win->getInterestString(3));

		metaInfo.interCode4 = win->getInterests(4);
		metaInfo.interKeyWords4 = codec->fromUnicode(win->getInterestString(4));

		metaInfo.about = codec->fromUnicode(win->getAbout());

		metaInfo.authFlag = win->getAuth();
		metaInfo.webAware = win->webAware();

		metaInfo.saveOwnerInfo(tcpSocket, *flapSeq, *snacSeq, *metaSeq);



		emit incFlapSeq();
//
//		emit incSnacSeq();
//		emit incMetaSeq();
//		metaInfo.sendMoreInfo(tcpSocket, *flapSeq, *snacSeq, *metaSeq);
//		emit incFlapSeq();

		if ( !waitForIconUpload && saveAvatar)
		{
			waitForIconUpload = saveAvatar;
			ownIconPath = avatarPath;
			uploadIcon();
		}
	}
}

void contactListTree::checkForOwnIcon(QByteArray avatarArray)
{
        QSettings settings(QSettings::NativeFormat, QSettings::UserScope, "qutim/qutim."+m_profile_name+"/ICQ."+account_name, "accountsettings");
	QByteArray icon = avatarArray.right(16);

	if ( icon.length() == 16)
	{
		if ( settings.value("main/iconhash").toByteArray() != icon.toHex() )
		{

			askForAvatars(icon, icqUin);
			settings.setValue("main/iconhash", icon.toHex());
		}
	}
}

void contactListTree::uploadIcon()
{



	if ( QFile::exists(ownIconPath))
	{
                QSettings settings(QSettings::NativeFormat, QSettings::UserScope, "qutim/qutim."+m_profile_name+"/ICQ."+account_name, "accountsettings");
		QFile pic(ownIconPath);
		if ( pic.open(QIODevice::ReadOnly))
		{
			QCryptographicHash hash(QCryptographicHash::Md5);
		hash.addData(pic.readAll());

		QByteArray hashArray = hash.result();
		settings.setValue("main/iconhash",hashArray.toHex());
		pic.copy(iconPath + hashArray.toHex() );

		bool iconExistsOnServer = true;
		if ( !iconObject.itemId)
		{
			iconExistsOnServer = false;
			iconObject.itemId = rand() % 0x03e6;
		}
			emit incSnacSeq();
			QByteArray packet;
			packet[0] = 0x2a;
			packet[1] = 0x02;
			packet.append(convertToByteArray((quint16)*flapSeq));
			packet.append(convertToByteArray((quint16)43));


			snac snac1308;
			snac1308.setFamily(0x0013);
			if (iconExistsOnServer)
				snac1308.setSubType(0x0009);
			else
				snac1308.setSubType(0x0008);
			snac1308.setReqId(*snacSeq);
			packet.append(snac1308.getData());

			packet.append(convertToByteArray((quint16)0x0001));
			packet.append(QString("1"));

			packet.append(convertToByteArray((quint16)iconObject.groupId));
			packet.append(convertToByteArray((quint16)iconObject.itemId));
			packet.append(convertToByteArray((quint16)0x0014));
			packet.append(convertToByteArray((quint16)22));

			packet.append(convertToByteArray((quint16)0x00d5));
			packet.append(convertToByteArray((quint16)0x0012));

			packet.append(convertToByteArray((quint16)0x0110));
			packet.append(hashArray);



			modifyObject addBuddy;

			addBuddy.itemId = iconObject.itemId;
			addBuddy.groupId = iconObject.groupId;
			addBuddy.itemType = 0x0014;
			addBuddy.operation = 1;
			addBuddy.buddyName = "1";

			modifyReqList.append(addBuddy);

			tcpSocket->write(packet);

			emit incFlapSeq();
		}
	} else {

		bool iconExistsOnServer = true;
		if ( !iconObject.itemId)
		{
			iconExistsOnServer = false;
			iconObject.itemId = rand() % 0x03e6;
		}
			emit incSnacSeq();
			QByteArray packet;
			packet[0] = 0x2a;
			packet[1] = 0x02;
			packet.append(convertToByteArray((quint16)*flapSeq));
			packet.append(convertToByteArray((quint16)32));


			snac snac1308;
			snac1308.setFamily(0x0013);
			if (iconExistsOnServer)
				snac1308.setSubType(0x0009);
			else
				snac1308.setSubType(0x0008);
			snac1308.setReqId(*snacSeq);
			packet.append(snac1308.getData());

			packet.append(convertToByteArray((quint16)0x0001));
			packet.append(QString("1"));

			packet.append(convertToByteArray((quint16)iconObject.groupId));
			packet.append(convertToByteArray((quint16)iconObject.itemId));
			packet.append(convertToByteArray((quint16)0x0014));
			packet.append(convertToByteArray((quint16)11));

			packet.append(convertToByteArray((quint16)0x00d5));
			packet.append(convertToByteArray((quint16)0x0007));
			packet.append(convertToByteArray((quint16)0x0005));
			packet.append(convertToByteArray((quint16)0x0201));
			packet.append(convertToByteArray((quint16)0xd204));
			packet.append(QChar(0x72));


			modifyObject addBuddy;

			addBuddy.itemId = iconObject.itemId;
			addBuddy.groupId = iconObject.groupId;
			addBuddy.itemType = 0x0014;
			addBuddy.operation = 1;
			addBuddy.buddyName = "1";

			modifyReqList.append(addBuddy);

			tcpSocket->write(packet);

			emit incFlapSeq();
			removeIconHash();


	}
}

void contactListTree::getUploadIconData(quint16 length)
{
	socket->read(8);
	length -= 8;


	quint16 noticeType = byteArrayToInt16(socket->read(2));
	length -= 2;

	socket->read(length);

	if ( noticeType == 1 && avatarModified)
	{
		avatarModified = false;
		QHostAddress hostAddr = QHostAddress(avatarAddress);
		if ( !hostAddr.isNull() && avatarCookie.length() == 256)
		{
			if ( !buddyConnection->connectedToServ )
                                buddyConnection->connectToServ(avatarAddress, avatarPort, avatarCookie/*, tcpSocket->proxy()*/);
			else
			{

				if ( buddyConnection->canSendReqForAvatars )
				{
					waitForIconUpload = false;
					buddyConnection->uploadIcon(ownIconPath);
				}
			}
		}
	}
}

void contactListTree::removeIconHash()
{
        QSettings settings(QSettings::NativeFormat, QSettings::UserScope, "qutim/qutim."+m_profile_name+"/ICQ."+account_name, "accountsettings");
	settings.remove("main/iconhash");
}

void contactListTree::openChangePasswordDialog()
{
	passwordChangeDialog dialog(icqUin, m_profile_name);

	if ( QtopiaApplication::execDialog(&dialog) )
	{
		emit incSnacSeq();
		emit incMetaSeq();
		metaInformation metaInfo(icqUin);
		metaInfo.changePassword(tcpSocket, *flapSeq, *snacSeq, *metaSeq, dialog.newPass);
		emit incFlapSeq();
	}
}

void contactListTree::showGroupMenu(treeGroupItem *group, const QPoint &point)
{

	currentContextGroup = group;

	currentContextMenu->clear();
	currentContextMenu->addAction(menuTitle);
	menuLabel->setText("<b>"+ group->name +"</b>");
	if ( iAmOnline)
	{
		currentContextMenu->addAction(createGroup);

		if ( groupList.key(group))
		{
			currentContextMenu->addAction(renameGroup);
			currentContextMenu->addAction(deleteGroup);
		}
	}
	currentContextMenu->popup(point);


}

void contactListTree::createContactListActions()
{
	currentContextMenu = new QMenu;


	menuLabel = new QLabel;
	menuLabel->setAlignment(Qt::AlignCenter);
	menuTitle = new QWidgetAction(this);
	menuTitle->setDefaultWidget(menuLabel);



	createGroup = new QAction(m_icq_plugin_system.getIcon("add"), tr("New group"), this);
	connect(createGroup, SIGNAL(triggered()), this, SLOT(createNewGroup()));

	renameGroup = new QAction(m_icq_plugin_system.getIcon("clear"), tr("Rename group"), this);
	connect(renameGroup, SIGNAL(triggered()), this, SLOT(renameSelectedGroup()));

	deleteGroup = new QAction(m_icq_plugin_system.getIcon("remove"), tr("Delete group"), this);
	connect(deleteGroup, SIGNAL(triggered()), this, SLOT(deleteSelectedGroup()));

	sendMessageAction = new QAction(m_icq_plugin_system.getIcon("message"), tr("Send message"), this);
	connect(sendMessageAction, SIGNAL(triggered()), this, SLOT(sendMessageActionTriggered()));

	userInformationAction = new QAction(m_icq_plugin_system.getIcon("contactinfo"), tr("Contact details"), this);
	connect(userInformationAction, SIGNAL(triggered()), this, SLOT(userInformationActionTriggered()));

	copyUINAction = new QAction(m_icq_plugin_system.getIcon("copy_uin"), tr("Copy UIN to clipboard"), this);
	connect(copyUINAction, SIGNAL(triggered()), this, SLOT(copyUINActionTriggered()));

	statusCheckAction= new QAction(m_icq_plugin_system.getIcon("checkstat"), tr("Contact status check"), this);
	connect(statusCheckAction, SIGNAL(triggered()), this, SLOT(statusCheckActionTriggered()));

	messageHistoryAction = new QAction(m_icq_plugin_system.getIcon("history"), tr("Message history"), this);
	connect(messageHistoryAction, SIGNAL(triggered()), this, SLOT(messageHistoryActionTriggered()));

	readAwayAction = new QAction(m_icq_plugin_system.getIcon("readaway"), tr("Read away message"), this);
	connect(readAwayAction, SIGNAL(triggered()), this, SLOT(readAwayActionTriggered()));

	renameContactAction = new QAction(m_icq_plugin_system.getIcon("edituser"), tr("Rename contact"), this);
	connect(renameContactAction, SIGNAL(triggered()), this, SLOT(renameContactActionTriggered()));

	deleteContactAction = new QAction(m_icq_plugin_system.getIcon("deleteuser"), tr("Delete contact"), this);
	connect(deleteContactAction, SIGNAL(triggered()), this, SLOT(deleteContactActionTriggered()));

	moveContactAction = new QAction(m_icq_plugin_system.getIcon("moveuser"), tr("Move to group"), this);
	connect(moveContactAction, SIGNAL(triggered()), this, SLOT(moveContactActionTriggered()));

	addToVisibleAction = new QAction(m_icq_plugin_system.getIcon("visible"), tr("Add to visible list"), this);
	connect(addToVisibleAction, SIGNAL(triggered()), this, SLOT(addToVisibleActionTriggered()));

	addToInvisibleAction = new QAction(m_icq_plugin_system.getIcon("privacy"), tr("Add to invisible list"), this);
	connect(addToInvisibleAction, SIGNAL(triggered()), this, SLOT(addToInvisibleActionTriggered()));

	addToIgnoreAction = new QAction(QIcon(":/icons/ignorelist"), tr("Add to ignore list"), this);
	connect(addToIgnoreAction , SIGNAL(triggered()), this, SLOT(addToIgnoreActionTriggered()));

	deleteFromVisibleAction = new QAction(m_icq_plugin_system.getIcon("visible"), tr("Delete from visible list"), this);
	connect(deleteFromVisibleAction, SIGNAL(triggered()), this, SLOT(deleteFromVisibleActionTriggered()));

	deleteFromInvisibleAction = new QAction(m_icq_plugin_system.getIcon("privacy"), tr("Delete from invisible list"), this);
	connect(deleteFromInvisibleAction, SIGNAL(triggered()), this, SLOT(deleteFromInvisibleActionTriggered()));

	deleteFromIgnoreAction = new QAction(QIcon(":/icons/ignorelist"), tr("Delete from ignore list"), this);
	connect(deleteFromIgnoreAction , SIGNAL(triggered()), this, SLOT(deleteFromIgnoreActionTriggered()));

	// TO-DO: Investigate a better way of adding auth icon
	requestAuthorizationAction = new QAction(statusIconClass::getInstance()->getConnectingIcon(), tr("Authorization request"), this);
	connect(requestAuthorizationAction , SIGNAL(triggered()), this, SLOT(requestAuthorizationActionTriggered()));

	addToContactListAction = new QAction(m_icq_plugin_system.getIcon("add_user"), tr("Add to contact list"), this);
	connect(addToContactListAction , SIGNAL(triggered()), this, SLOT(addToContactListActionTriggered()));

	allowToAddMe = new QAction(m_icq_plugin_system.getIcon("apply"), tr("Allow contact to add me"), this);
	connect(allowToAddMe , SIGNAL(triggered()), this, SLOT(allowToAddMeTriggered()));

	removeMyself = new QAction(m_icq_plugin_system.getIcon("deletetab2"), tr("Remove myself from contact's list"), this);
	connect(removeMyself , SIGNAL(triggered()), this, SLOT(removeMyselfTriggered()));

/*	connect(fileTransferObject->getSendFileAction() , SIGNAL(triggered()), this, SLOT(sendFileActionTriggered()));*/

	readXstatus = new QAction(m_icq_plugin_system.getIcon("xstatus"), tr("Read custom status"), this);
	connect(readXstatus , SIGNAL(triggered()), this, SLOT(readXstatusTriggered()));

	editNoteAction = new QAction(m_icq_plugin_system.getIcon("note"), tr("Edit note"), this);
	connect(editNoteAction , SIGNAL(triggered()), this, SLOT(editNoteActionTriggered()));
}

void contactListTree::createNewGroup()
{
	addRenameDialog dialog;

	dialog.setWindowTitle(tr("Create group"));

	if ( QtopiaApplication::execDialog(&dialog) )
	{
		quint16 groupId = rand() % 0x03e6;
		for ( ;groupList.contains(groupId);)
			groupId = rand() % 0x03e6;


		QString groupName = dialog.name;
		QByteArray editPack;

			emit incSnacSeq();
			QByteArray packet;
			packet[0] = 0x2a;
			packet[1] = 0x02;
			packet.append(convertToByteArray((quint16)*flapSeq));
			packet.append(convertToByteArray((quint16)10));

			snac snac1311;
			snac1311.setFamily(0x0013);
			snac1311.setSubType(0x0011);
			snac1311.setReqId(*snacSeq);
			packet.append(snac1311.getData());
			emit incFlapSeq();
			editPack.append(packet);

			emit incSnacSeq();
			QByteArray packet2;
			packet2[0] = 0x2a;
			packet2[1] = 0x02;
			packet2.append(convertToByteArray((quint16)*flapSeq));
			packet2.append(convertToByteArray((quint16)(24 + groupName.toUtf8().length())));

			snac snac1308;
			snac1308.setFamily(0x0013);
			snac1308.setSubType(0x0008);
			snac1308.setReqId(*snacSeq);
			packet2.append(snac1308.getData());


			packet2.append(convertToByteArray((quint16)groupName.toUtf8().length()));
			packet2.append(groupName.toUtf8());

			packet2.append(convertToByteArray((quint16)groupId));
			packet2.append(convertToByteArray((quint16)0x0000));

			packet2.append(convertToByteArray((quint16)0x0001));


			packet2.append(convertToByteArray((quint16)0x0004));
			packet2.append(convertToByteArray((quint16)0x00c8));
			packet2.append(convertToByteArray((quint16)0x0000));

			emit incFlapSeq();

			editPack.append(packet2);


			tcpSocket->write(editPack);


			modifyObject addGroup;

			addGroup.itemId = 0;
			addGroup.groupId = groupId;
			addGroup.itemType = 1;
			addGroup.operation = 0;
			addGroup.buddyName = groupName;

			modifyReqList.append(addGroup);
	}
}

void contactListTree::renameSelectedGroup()
{
	addRenameDialog dialog;

		dialog.setWindowTitle(tr("Rename group"));

		if ( QtopiaApplication::execDialog(&dialog) )
		{

			quint16 groupId = groupList.key(currentContextGroup);
			QString groupName = dialog.name;
			QByteArray editPack;

				emit incSnacSeq();
				QByteArray packet;
				packet[0] = 0x2a;
				packet[1] = 0x02;
				packet.append(convertToByteArray((quint16)*flapSeq));
				packet.append(convertToByteArray((quint16)10));

				snac snac1311;
				snac1311.setFamily(0x0013);
				snac1311.setSubType(0x0011);
				snac1311.setReqId(*snacSeq);
				packet.append(snac1311.getData());
				emit incFlapSeq();
				editPack.append(packet);

				emit incSnacSeq();
				QByteArray packet2;
				packet2[0] = 0x2a;
				packet2[1] = 0x02;
				packet2.append(convertToByteArray((quint16)*flapSeq));
				packet2.append(convertToByteArray((quint16)(24 + groupName.toUtf8().length() + currentContextGroup->buddiesList.count() * 2)));

				snac snac1308;
				snac1308.setFamily(0x0013);
				snac1308.setSubType(0x0009);
				snac1308.setReqId(*snacSeq);
				packet2.append(snac1308.getData());


				packet2.append(convertToByteArray((quint16)groupName.toUtf8().length()));
				packet2.append(groupName.toUtf8());

				packet2.append(convertToByteArray((quint16)groupId));
				packet2.append(convertToByteArray((quint16)0x0000));

				packet2.append(convertToByteArray((quint16)0x0001));


				packet2.append(convertToByteArray((quint16)(0x0004 + currentContextGroup->buddiesList.count() * 2)));
				packet2.append(convertToByteArray((quint16)0x00c8));
				packet2.append(convertToByteArray((quint16)currentContextGroup->buddiesList.count() * 2));

				foreach(quint16 listgroupId, currentContextGroup->buddiesList)
				{
					if ( listgroupId)
						packet2.append(convertToByteArray((quint16)listgroupId));
				}

				emit incFlapSeq();

				editPack.append(packet2);


				tcpSocket->write(editPack);


				modifyObject addGroup;

				addGroup.itemId = 0;
				addGroup.groupId = groupId;
				addGroup.itemType = 1;
				addGroup.operation = 1;
				addGroup.buddyName = groupName;

				modifyReqList.append(addGroup);
		}
}

void contactListTree::deleteSelectedGroup()
{
	QMessageBox msgBox(QMessageBox::NoIcon, tr("Delete group"),
	tr("Delete group \"%1\"?").arg(currentContextGroup->name), QMessageBox::Yes | QMessageBox::No);
	switch( QtopiaApplication::execDialog(&msgBox) )
	{
		case QMessageBox::Yes:
			break;

		case QMessageBox::No:
			return;
			break;

		default:
			return;
			break;
	}

	QString groupName = currentContextGroup->name;
	quint16 groupId = groupList.key(currentContextGroup);
	QByteArray editPack;

		emit incSnacSeq();
		QByteArray packet;
		packet[0] = 0x2a;
		packet[1] = 0x02;
		packet.append(convertToByteArray((quint16)*flapSeq));
		packet.append(convertToByteArray((quint16)10));

		snac snac1311;
		snac1311.setFamily(0x0013);
		snac1311.setSubType(0x0011);
		snac1311.setReqId(*snacSeq);
		packet.append(snac1311.getData());
		emit incFlapSeq();
		editPack.append(packet);

		emit incSnacSeq();
		QByteArray packet2;
		packet2[0] = 0x2a;
		packet2[1] = 0x02;
		packet2.append(convertToByteArray((quint16)*flapSeq));
		packet2.append(convertToByteArray((quint16)(24 + groupName.toUtf8().length())));

		snac snac130a;
		snac130a.setFamily(0x0013);
		snac130a.setSubType(0x000a);
		snac130a.setReqId(*snacSeq);
		packet2.append(snac130a.getData());


		packet2.append(convertToByteArray((quint16)groupName.toUtf8().length()));
		packet2.append(groupName.toUtf8());

		packet2.append(convertToByteArray((quint16)groupId));
		packet2.append(convertToByteArray((quint16)0x0000));

		packet2.append(convertToByteArray((quint16)0x0001));


		packet2.append(convertToByteArray((quint16)0x0004));
		packet2.append(convertToByteArray((quint16)0x00c8));
		packet2.append(convertToByteArray((quint16)0x0000));

		emit incFlapSeq();

		editPack.append(packet2);


		tcpSocket->write(editPack);


		modifyObject delGroup;

		delGroup.itemId = 0;
		delGroup.groupId = groupId;
		delGroup.itemType = 1;
		delGroup.operation = 2;
		delGroup.buddyName = groupName;

		modifyReqList.append(delGroup);


}



void contactListTree::addNewGroupToRoot(const QString &name, quint16 groupID)
{

        QSettings contacts(QSettings::NativeFormat, QSettings::UserScope, "qutim/qutim."+m_profile_name+"/ICQ."+account_name, "contactlist");
	contacts.beginGroup(QString::number(groupID));

	treeGroupItem *group = new treeGroupItem;
	group->setOnOffLists();
	groupList.insert(groupID, group);
	group->setGroupText(name);
	contacts.setValue("id", groupID);
	contacts.setValue("name", name);
	contacts.endGroup();
	addGroupToCL(groupID, name);
	QStringList groups = contacts.value("list/group").toStringList();
	groups<<QString::number(groupID);
	contacts.setValue("list/group", groups);


	quint16 groupCount = groupList.count();
	if ( groupList.contains(0) )
		groupCount--;

	QByteArray editPack;



		emit incSnacSeq();
		QByteArray packet2;
		packet2[0] = 0x2a;
		packet2[1] = 0x02;
		packet2.append(convertToByteArray((quint16)*flapSeq));
		packet2.append(convertToByteArray((quint16)(24 + groupCount * 2)));

		snac snac1308;
		snac1308.setFamily(0x0013);
		snac1308.setSubType(0x0009);
		snac1308.setReqId(*snacSeq);
		packet2.append(snac1308.getData());


		packet2.append(convertToByteArray((quint16)0));

		packet2.append(convertToByteArray((quint16)0));
		packet2.append(convertToByteArray((quint16)0));

		packet2.append(convertToByteArray((quint16)0x0001));


		packet2.append(convertToByteArray((quint16)(4 + groupCount * 2)));
		packet2.append(convertToByteArray((quint16)0x00c8));
		packet2.append(convertToByteArray((quint16)(groupCount * 2)));

		foreach(quint16 listgroupId, groupList.keys())
		{
			if ( listgroupId)
				packet2.append(convertToByteArray((quint16)listgroupId));
		}


		emit incFlapSeq();

		editPack.append(packet2);


		emit incSnacSeq();
		QByteArray packet;
		packet[0] = 0x2a;
		packet[1] = 0x02;
		packet.append(convertToByteArray((quint16)*flapSeq));
		packet.append(convertToByteArray((quint16)10));

		snac snac1312;
		snac1312.setFamily(0x0013);
		snac1312.setSubType(0x0012);
		snac1312.setReqId(*snacSeq);
		packet.append(snac1312.getData());
		emit incFlapSeq();
		editPack.append(packet);


		tcpSocket->write(editPack);


		modifyObject addGroup;

		addGroup.itemId = 0;
		addGroup.groupId = 0;
		addGroup.itemType = 1;
		addGroup.operation = 1;

		modifyReqList.append(addGroup);
}

void contactListTree::renameGroupToName(const QString &name, quint16 groupId)
{
	treeGroupItem *group = groupList.value(groupId);
        QSettings contacts(QSettings::NativeFormat, QSettings::UserScope, "qutim/qutim."+m_profile_name+"/ICQ."+account_name, "contactlist");
	contacts.setValue(QString::number(groupId) + "/name", name);
	renameGroupInCL(name, groupId);
	group->setGroupText(name);

	if ( isMergeAccounts )
		emit reupdateList();

}

void contactListTree::deleteSelectedGroup(quint16 groupId)
{
        QSettings contacts(QSettings::NativeFormat, QSettings::UserScope, "qutim/qutim."+m_profile_name+"/ICQ."+account_name, "contactlist");
	contacts.remove(QString::number(groupId));

		removeGroupFromCl(groupId);
		delete groupList.value(groupId);
		groupList.remove(groupId);

		QStringList buddiesList = contacts.value("list/contacts").toStringList();

		foreach(treeBuddyItem *buddy, buddyList)
		{
			if ( buddy->groupID == groupId)
			{
				buddiesList.removeAll(buddy->getUin());
				contacts.remove(buddy->getUin());
			}
		}

		contacts.setValue("list/contacts", buddiesList);
		QStringList groups = contacts.value("list/group").toStringList();
		groups.removeAll(QString::number(groupId));
		contacts.setValue("list/group", groups);


		quint16 groupCount = groupList.count();
		if ( groupList.contains(0) )
			groupCount--;

		QByteArray editPack;



			emit incSnacSeq();
			QByteArray packet2;
			packet2[0] = 0x2a;
			packet2[1] = 0x02;
			packet2.append(convertToByteArray((quint16)*flapSeq));
			packet2.append(convertToByteArray((quint16)(24 + groupCount * 2)));

			snac snac1308;
			snac1308.setFamily(0x0013);
			snac1308.setSubType(0x0009);
			snac1308.setReqId(*snacSeq);
			packet2.append(snac1308.getData());


			packet2.append(convertToByteArray((quint16)0));

			packet2.append(convertToByteArray((quint16)0));
			packet2.append(convertToByteArray((quint16)0));

			packet2.append(convertToByteArray((quint16)0x0001));


			packet2.append(convertToByteArray((quint16)(4 + groupCount * 2)));
			packet2.append(convertToByteArray((quint16)0x00c8));
			packet2.append(convertToByteArray((quint16)(groupCount* 2)));

			foreach(quint16 listgroupId, groupList.keys())
			{
				if ( listgroupId)
					packet2.append(convertToByteArray((quint16)listgroupId));
			}


			emit incFlapSeq();

			editPack.append(packet2);


			emit incSnacSeq();
			QByteArray packet;
			packet[0] = 0x2a;
			packet[1] = 0x02;
			packet.append(convertToByteArray((quint16)*flapSeq));
			packet.append(convertToByteArray((quint16)10));

			snac snac1312;
			snac1312.setFamily(0x0013);
			snac1312.setSubType(0x0012);
			snac1312.setReqId(*snacSeq);
			packet.append(snac1312.getData());
			emit incFlapSeq();
			editPack.append(packet);


			tcpSocket->write(editPack);


			modifyObject addGroup;

			addGroup.itemId = 0;
			addGroup.groupId = 0;
			addGroup.itemType = 1;
			addGroup.operation = 1;

			modifyReqList.append(addGroup);
}

void contactListTree::showBuddyMenu(const QList<QAction*> &action_list,
		treeBuddyItem *buddy, const QPoint &point)
{
	currentContextBuddy = buddy;

	currentContextMenu->clear();
	currentContextMenu->addAction(menuTitle);
	menuLabel->setText("<b>"+ buddy->getName() +"</b>");




	currentContextMenu->addAction(action_list.at(0));
	if ( iAmOnline )
	{
/*		if ( currentContextBuddy->getStatus() != contactOffline)
		{
			if ( currentContextBuddy->fileTransferSupport )
				currentContextMenu->addAction(fileTransferObject->getSendFileAction());
                }*/

		if ( currentContextBuddy->getNotAutho())
		{
			currentContextMenu->addAction(requestAuthorizationAction);

		}

		if ( currentContextBuddy->getStatus() == contactOffline )
		{
			currentContextMenu->addAction(statusCheckAction);
		}
		if ( currentContextBuddy->getStatus() != contactOffline &&
				currentContextBuddy->getStatus() != contactOnline &&
				currentContextBuddy->getStatus() != contactInvisible )
		{
			currentContextMenu->addAction(readAwayAction);
		}

		if ( currentContextBuddy->xStatusPresent)
			currentContextMenu->addAction(readXstatus);


	}

	currentContextMenu->addAction(copyUINAction);
	currentContextMenu->addAction(editNoteAction);
	currentContextMenu->addAction(action_list.at(2));
	currentContextMenu->addAction(action_list.at(1));

	if ( noteWidgetsList.contains(currentContextBuddy->getUin()))
		editNoteAction->setEnabled(false);
	else
		editNoteAction->setEnabled(true);

	if ( iAmOnline &&  currentContextBuddy->groupID)
	{
		currentContextMenu->addSeparator();
		currentContextMenu->addAction(renameContactAction);
		currentContextMenu->addAction(deleteContactAction);
		currentContextMenu->addAction(moveContactAction);
		currentContextMenu->addSeparator();
		if ( !visibleList.contains(currentContextBuddy->getUin()))
			currentContextMenu->addAction(addToVisibleAction);
		else
			currentContextMenu->addAction(deleteFromVisibleAction);
		if ( !invisibleList.contains(currentContextBuddy->getUin()))
			currentContextMenu->addAction(addToInvisibleAction);
		else
			currentContextMenu->addAction(deleteFromInvisibleAction);
		if ( !ignoreList.contains(currentContextBuddy->getUin()))
			currentContextMenu->addAction(addToIgnoreAction);
		else
			currentContextMenu->addAction(deleteFromIgnoreAction);
		currentContextMenu->addAction(allowToAddMe);
		currentContextMenu->addAction(removeMyself);
	}

	if ( !currentContextBuddy->groupID )
	{
		currentContextMenu->addAction(deleteContactAction);
		if ( iAmOnline)
		{
			currentContextMenu->addAction(addToContactListAction);
			currentContextMenu->addAction(addToIgnoreAction);
			currentContextMenu->addAction(allowToAddMe);
			currentContextMenu->addAction(removeMyself);
		}
	}

	if ( iAmOnline )
		currentContextMenu->addAction(createGroup);

	currentContextMenu->addSeparator();
	int action_count = action_list.count() - 3;
	if ( action_count > 0 )
	{
		for ( int i = 0; i < action_count; i++ )
		{
			currentContextMenu->addAction(action_list.at(3 + i));
		}
	}

	currentContextMenu->popup(point);
}

void contactListTree::sendMessageActionTriggered()
{
	TreeModelItem contact_item;
	contact_item.m_protocol_name = "ICQ";
	contact_item.m_account_name = icqUin;
	contact_item.m_item_name = currentContextBuddy->getUin();
	int group_id = currentContextBuddy->groupID;
	contact_item.m_parent_name = group_id?QString::number(group_id):"";
	contact_item.m_item_type = TreeModelItem::Buddy;
	m_icq_plugin_system.createChat(contact_item);
}

void contactListTree::userInformationActionTriggered()
{
	openInfoWindow(currentContextBuddy->getUin());
}

void contactListTree::statusCheckActionTriggered()
{
	checkStatusFor(currentContextBuddy->getUin());
}

void contactListTree::messageHistoryActionTriggered()
{
	showHistory(currentContextBuddy->getUin());
}

void contactListTree::readAwayActionTriggered()
{
	emit incSnacSeq();
	icqMessage message(codepage);

	if ( currentContextBuddy->icqLite )
	{
		message.awayType = 0x1a;
	} else {

	if ( currentContextBuddy->getStatus() == contactOccupied )
		message.awayType = 0xe9;
	else if ( currentContextBuddy->getStatus() == contactNa )
		message.awayType = 0xea;
	else if ( currentContextBuddy->getStatus() == contactDnd )
		message.awayType = 0xeb;
	else if ( currentContextBuddy->getStatus() == contactFfc )
		message.awayType = 0xec;
	else
		message.awayType = 0xe8;
	}
	message.requestAutoreply(tcpSocket, currentContextBuddy->getUin(),*flapSeq, *snacSeq);
	emit incFlapSeq();

	readAwayDialog *dialog = new readAwayDialog;
	dialog->setWindowTitle(tr("%1 away message").arg(currentContextBuddy->getName()));
	dialog->setAttribute(Qt::WA_DeleteOnClose, true);
		connect( dialog, SIGNAL(destroyed ( QObject *)),
				this, SLOT(deleteAwayWindow(QObject *)));
	QtopiaApplication::showDialog(dialog);
	awayMessageList.insert(message.msgCookie, dialog);
}

void contactListTree::deleteAwayWindow(QObject *o)
{
	readAwayDialog *tempWindow = (readAwayDialog  *)(o);
	awayMessageList.remove(awayMessageList.key(tempWindow));
}

void contactListTree::getAwayMessage(quint16 length)
{
	icqMessage message(codepage);
	message.getAwayMessage(socket, length);
	if ( message.msgType == 0xe8 ||
			message.msgType == 0xe9 ||
			message.msgType == 0xea ||
			message.msgType == 0xeb ||
			message.msgType == 0xec)
	{
		if ( awayMessageList.contains(message.msgCookie))
		{
			awayMessageList.value(message.msgCookie)->addMessage(message.msg);
		}
	} else if (message.msgType == 0x1a )
	{

		QString xStatusmsg = addXstatusMessage(message.from, message.byteArrayMsg);

		if ( awayMessageList.contains(message.msgCookie))
		{
			awayMessageList.value(message.msgCookie)->addXstatus(xStatusmsg);
		}
	} else if (message.msgType == 0x01 )
	{
		if ( messageCursorPositions.contains(message.msgCookie))
		{
			messageDelievered(message.from,0,messageCursorPositions.value(message.msgCookie));
		}

		messageCursorPositions.remove(message.msgCookie);
	}
}

void contactListTree::renameContactActionTriggered()
{
	addRenameDialog dialog;

			dialog.setWindowTitle(tr("Rename contact"));

			if ( QtopiaApplication::execDialog(&dialog) )
			{

				QString contactName = dialog.name;
				QString uin = currentContextBuddy->getUin();

				emit incSnacSeq();
				QByteArray packet2;
				packet2[0] = 0x2a;
				packet2[1] = 0x02;
				packet2.append(convertToByteArray((quint16)*flapSeq));
				if ( currentContextBuddy->getNotAutho() )
					packet2.append(convertToByteArray((quint16)(28 + contactName.toUtf8().length() + uin.length())));
				else
					packet2.append(convertToByteArray((quint16)(24 + contactName.toUtf8().length() + uin. toUtf8().length())));

				snac snac1308;
				snac1308.setFamily(0x0013);
				snac1308.setSubType(0x0009);
				snac1308.setReqId(*snacSeq);
				packet2.append(snac1308.getData());


				packet2.append(convertToByteArray((quint16)uin.toUtf8().length()));
				packet2.append(uin.toUtf8());

					packet2.append(convertToByteArray((quint16)currentContextBuddy->groupID));
					packet2.append(convertToByteArray((quint16)currentContextBuddy->itemId));

					packet2.append(convertToByteArray((quint16)0x0000));

					if ( currentContextBuddy->getNotAutho())
						packet2.append(convertToByteArray((quint16)(8 + contactName.toUtf8().length())));
					else
						packet2.append(convertToByteArray((quint16)(4 + contactName.toUtf8().length())));
					packet2.append(convertToByteArray((quint16)0x0131));
					packet2.append(convertToByteArray((quint16)contactName.toUtf8().length()));

					packet2.append(contactName.toUtf8());

					if ( currentContextBuddy->getNotAutho())
					{
						packet2.append(convertToByteArray((quint16)0x0066));
						packet2.append(convertToByteArray((quint16)0));
					}

					emit incFlapSeq();



					tcpSocket->write(packet2);


					modifyObject renameBuddy;

					renameBuddy.itemId = currentContextBuddy->itemId;
					renameBuddy.groupId = currentContextBuddy->groupID;
					renameBuddy.itemType = 0;
					renameBuddy.operation = 1;
					renameBuddy.buddyName = contactName;
					renameBuddy.buddyUin = uin;

					modifyReqList.append(renameBuddy);
			}
}

void contactListTree::deleteContactActionTriggered()
{

	deleteContactDialog dialog;
	dialog.setWindowTitle(tr("Delete %1").arg(currentContextBuddy->getName()));

	if ( !movingBuddy)
	{
		if ( !QtopiaApplication::execDialog(&dialog) )
		 return;
	}
		QString contactName = currentContextBuddy->getName();
		QString uin = currentContextBuddy->getUin();

		if ( !movingBuddy)
		{
			if ( dialog.deleteHistory() )
			{
				//TODO: delet history
			}
		}


	movingBuddy = false;
		if ( buddyList.contains(uin))
		{
			if (!buddyList.value(uin)->groupID)
			{
                                QSettings contacts(QSettings::NativeFormat, QSettings::UserScope, "qutim/qutim."+m_profile_name+"/ICQ."+account_name, "contactlist");

				QStringList contactList = contacts.value("list/contacts").toStringList();

				contactList.removeAll(uin);
				contacts.setValue("list/contacts", contactList);

				contacts.remove(uin);

				treeBuddyItem *buddy = buddyList.value(uin);

						idBuddyList.removeAll(buddy->itemId);

						if ( infoWindowList.contains(uin) )
						{
							infoWindowList.value(uin)->close();
							infoWindowList.remove(uin);
						}


						quint16 groupCount;
						QString groupName;
						treeGroupItem *group = groupList.value(buddy->groupID);

							group->buddiesList.removeAll(buddy->itemId);

							group->userCount--;
							group->updateText();
							groupCount = group->buddiesList.count();
							groupName = group->name;
							removeContactFromCl(buddy->groupID, uin);
						buddyList.remove(uin);
						delete buddy;

						return;
			}
		}



		QByteArray editPack;

			emit incSnacSeq();
			QByteArray packet;
			packet[0] = 0x2a;
			packet[1] = 0x02;
			packet.append(convertToByteArray((quint16)*flapSeq));
			packet.append(convertToByteArray((quint16)10));

			snac snac1311;
			snac1311.setFamily(0x0013);
			snac1311.setSubType(0x0011);
			snac1311.setReqId(*snacSeq);
			packet.append(snac1311.getData());
			emit incFlapSeq();
			editPack.append(packet);

			emit incSnacSeq();
			QByteArray packet2;
			packet2[0] = 0x2a;
			packet2[1] = 0x02;
			packet2.append(convertToByteArray((quint16)*flapSeq));
			if ( currentContextBuddy->getNotAutho())
				packet2.append(convertToByteArray((quint16)(28 + contactName.toUtf8().length() + uin.length())));
			else
				packet2.append(convertToByteArray((quint16)(24 + contactName.toUtf8().length() + uin. toUtf8().length())));

			snac snac130a;
			snac130a.setFamily(0x0013);
			snac130a.setSubType(0x000a);
			snac130a.setReqId(*snacSeq);
			packet2.append(snac130a.getData());


			packet2.append(convertToByteArray((quint16)uin.toUtf8().length()));
			packet2.append(uin.toUtf8());

			packet2.append(convertToByteArray((quint16)currentContextBuddy->groupID));
			packet2.append(convertToByteArray((quint16)currentContextBuddy->itemId));

			packet2.append(convertToByteArray((quint16)0x0000));

			if ( currentContextBuddy->getNotAutho())
				packet2.append(convertToByteArray((quint16)(8 + contactName.toUtf8().length())));
			else
				packet2.append(convertToByteArray((quint16)(4 + contactName.toUtf8().length())));
			packet2.append(convertToByteArray((quint16)0x0131));
				packet2.append(convertToByteArray((quint16)contactName.toUtf8().length()));

				packet2.append(contactName.toUtf8());

				if ( currentContextBuddy->getNotAutho())
				{
					packet2.append(convertToByteArray((quint16)0x0066));
					packet2.append(convertToByteArray((quint16)0));
				}

				emit incFlapSeq();

			editPack.append(packet2);


			tcpSocket->write(editPack);


			modifyObject delBuddy;

			delBuddy.itemId = currentContextBuddy->itemId;
			delBuddy.groupId = currentContextBuddy->groupID;
			delBuddy.itemType = 0;
			delBuddy.operation = 2;
			delBuddy.buddyName = contactName;
			delBuddy.buddyUin = uin;

			modifyReqList.append(delBuddy);
}

void contactListTree::moveContactActionTriggered()
{
	addBuddyDialog addDialog;
	addDialog.setWindowTitle(tr("Move %1 to:").arg(currentContextBuddy->getUin()));
	QStringList groups;
	foreach (treeGroupItem *group, groupList)
	{
		if ( groupList.key(group))
			groups<<group->name;
	}
	addDialog.setMoveData(groups);
	if ( QtopiaApplication::execDialog(&addDialog) )
	{
		QString uin = currentContextBuddy->getUin();
		QString name = currentContextBuddy->getName();
		bool authReq = currentContextBuddy->getNotAutho();

		movingBuddy = true;
		deleteContactActionTriggered();
		sendUserAddReq(uin, name,authReq , addDialog.getGroup());

	}
}

void contactListTree::renameContact(const QString &uin, const QString &name)
{
	if ( buddyList.contains(uin))
	{
                QSettings contacts(QSettings::NativeFormat, QSettings::UserScope, "qutim/qutim."+m_profile_name+"/ICQ."+account_name, "contactlist");

		contacts.setValue(uin + "/nickname", name);
		buddyList.value(uin)->setName(name);
		buddyList.value(uin)->updateBuddyText();
	}


}

void contactListTree::removeContact(const QString &uin)
{
	if ( buddyList.contains(uin) )
	{
                QSettings contacts(QSettings::NativeFormat, QSettings::UserScope, "qutim/qutim."+m_profile_name+"/ICQ."+account_name, "contactlist");

		QStringList contactList = contacts.value("list/contacts").toStringList();

		contactList.removeAll(uin);
		contacts.setValue("list/contacts", contactList);

		contacts.remove(uin);

		treeBuddyItem *buddy = buddyList.value(uin);

		idBuddyList.removeAll(buddy->itemId);

		if ( infoWindowList.contains(uin) )
		{
			infoWindowList.value(uin)->close();
			infoWindowList.remove(uin);
		}


		quint16 groupCount;
		QString groupName;
			treeGroupItem *group = groupList.value(buddy->groupID);

			group->buddiesList.removeAll(buddy->itemId);

			group->userCount--;
			group->updateText();
			groupCount = group->buddiesList.count();
			groupName = group->name;

			if ( !buddy->isOffline)
			{
				group->buddyOffline();
			}

			removeContactFromCl(buddy->groupID, uin);
		buddyList.remove(uin);
		messageList.remove(uin);
		delete buddy;



		QByteArray editPack;



		emit incSnacSeq();
		QByteArray packet2;
		packet2[0] = 0x2a;
		packet2[1] = 0x02;
		packet2.append(convertToByteArray((quint16)*flapSeq));
		packet2.append(convertToByteArray((quint16)(24 + groupCount * 2 + groupName.toUtf8().length())));

		snac snac1308;
		snac1308.setFamily(0x0013);
		snac1308.setSubType(0x0009);
		snac1308.setReqId(*snacSeq);
		packet2.append(snac1308.getData());


		packet2.append(convertToByteArray((quint16)groupName.toUtf8().length()));

		packet2.append(groupName.toUtf8());
		packet2.append(convertToByteArray((quint16)groupList.key(group)));

		packet2.append(convertToByteArray((quint16)0x0000));

		packet2.append(convertToByteArray((quint16)0x0001));

		packet2.append(convertToByteArray((quint16)(4 + groupCount * 2)));
		packet2.append(convertToByteArray((quint16)0x00c8));
		packet2.append(convertToByteArray((quint16)(groupCount* 2)));

		foreach(quint16 buddyId, group->buddiesList)
		{
			if ( buddyId)
					packet2.append(convertToByteArray((quint16)buddyId));
		}


		emit incFlapSeq();

		editPack.append(packet2);


		emit incSnacSeq();
		QByteArray packet;
		packet[0] = 0x2a;
		packet[1] = 0x02;
		packet.append(convertToByteArray((quint16)*flapSeq));
		packet.append(convertToByteArray((quint16)10));

		snac snac1312;
		snac1312.setFamily(0x0013);
		snac1312.setSubType(0x0012);
		snac1312.setReqId(*snacSeq);
		packet.append(snac1312.getData());
		emit incFlapSeq();
		editPack.append(packet);


		tcpSocket->write(editPack);


		modifyObject addGroup;

					addGroup.itemId = 0;
					addGroup.groupId = groupList.key(group);
					addGroup.itemType = 1;
					addGroup.operation = 1;
					addGroup.buddyName = group->name;

					modifyReqList.append(addGroup);



	}
}

void contactListTree::addToVisibleActionTriggered()
{
	if ( visibleList.contains(currentContextBuddy->getUin()))
		return;

	if ( invisibleList.contains(currentContextBuddy->getUin()))
		deleteFromPrivacyList(currentContextBuddy->getUin(),1);


		QString uin = currentContextBuddy->getUin();
		QString name = currentContextBuddy->getName();
		emit incSnacSeq();
		QByteArray packet2;
		packet2[0] = 0x2a;
		packet2[1] = 0x02;
		packet2.append(convertToByteArray((quint16)*flapSeq));
		packet2.append(convertToByteArray((quint16)(24 + name.toUtf8().length() + uin.toUtf8().length())));

		snac snac1308;
		snac1308.setFamily(0x0013);
		snac1308.setSubType(0x0008);
		snac1308.setReqId(*snacSeq);
		packet2.append(snac1308.getData());


		packet2.append(convertToByteArray((quint16)uin.toUtf8().length()));
		packet2.append(uin.toUtf8());

		packet2.append(convertToByteArray((quint16)0));
		packet2.append(convertToByteArray((quint16)currentContextBuddy->itemId));

		packet2.append(convertToByteArray((quint16)0x0002));

		packet2.append(convertToByteArray((quint16)(4 + name.toUtf8().length())));
		packet2.append(convertToByteArray((quint16)0x0131));
		packet2.append(convertToByteArray((quint16)name.toUtf8().length()));

		packet2.append(name.toUtf8());


			emit incFlapSeq();



		tcpSocket->write(packet2);


		modifyObject visBuddy;

		visBuddy.itemId = currentContextBuddy->itemId;
		visBuddy.groupId = 0;
		visBuddy.itemType = 2;
		visBuddy.operation = 0;
		visBuddy.buddyName = name;
		visBuddy.buddyUin = uin;

		modifyReqList.append(visBuddy);

		visibleList.append(uin);

		itemFromList object;
		object.groupId = currentContextBuddy->groupID;
		object.itemId = currentContextBuddy->itemId;
		visibleObjectList.insert(uin, object);

                QSettings contacts(QSettings::NativeFormat, QSettings::UserScope, "qutim/qutim."+m_profile_name+"/ICQ."+account_name, "contactlist");
		contacts.setValue("list/visible", visibleList);
		if ( privacyListWindowOpen)
			privacyWindow->createLists();
		currentContextBuddy->m_visible_contact = true;
		currentContextBuddy->setCustomIcon(m_icq_plugin_system.getIcon("visible"),5);

}

void contactListTree::addToInvisibleActionTriggered()
{
	if ( invisibleList.contains(currentContextBuddy->getUin()))
		return;


	if ( visibleList.contains(currentContextBuddy->getUin()))
		deleteFromPrivacyList(currentContextBuddy->getUin(),0);


	QString uin = currentContextBuddy->getUin();
	QString name = currentContextBuddy->getName();
	emit incSnacSeq();
	QByteArray packet2;
	packet2[0] = 0x2a;
	packet2[1] = 0x02;
	packet2.append(convertToByteArray((quint16)*flapSeq));
	packet2.append(convertToByteArray((quint16)(24 + name.toUtf8().length() + uin.toUtf8().length())));

	snac snac1308;
	snac1308.setFamily(0x0013);
	snac1308.setSubType(0x0008);
	snac1308.setReqId(*snacSeq);
	packet2.append(snac1308.getData());


	packet2.append(convertToByteArray((quint16)uin.toUtf8().length()));
	packet2.append(uin.toUtf8());

	packet2.append(convertToByteArray((quint16)0));
	packet2.append(convertToByteArray((quint16)currentContextBuddy->itemId));

	packet2.append(convertToByteArray((quint16)0x0003));

	packet2.append(convertToByteArray((quint16)(4 + name.toUtf8().length())));
	packet2.append(convertToByteArray((quint16)0x0131));
	packet2.append(convertToByteArray((quint16)name.toUtf8().length()));

	packet2.append(name.toUtf8());


		emit incFlapSeq();



	tcpSocket->write(packet2);


	modifyObject visBuddy;

	visBuddy.itemId = currentContextBuddy->itemId;
	visBuddy.groupId = 0;
	visBuddy.itemType = 3;
	visBuddy.operation = 0;
	visBuddy.buddyName = name;
	visBuddy.buddyUin = uin;

	modifyReqList.append(visBuddy);

	invisibleList.append(uin);

	itemFromList object;
	object.groupId = currentContextBuddy->groupID;
	object.itemId = currentContextBuddy->itemId;
	invisibleObjectList.insert(uin, object);

        QSettings contacts(QSettings::NativeFormat, QSettings::UserScope, "qutim/qutim."+m_profile_name+"/ICQ."+account_name, "contactlist");
	contacts.setValue("list/invisible", invisibleList);
	if ( privacyListWindowOpen)
		privacyWindow->createLists();
	currentContextBuddy->m_invisible_contact = true;
	currentContextBuddy->setCustomIcon(m_icq_plugin_system.getIcon("privacy"),6);
}

void contactListTree::addToIgnoreActionTriggered()
{
	if ( ignoreList.contains(currentContextBuddy->getUin()))
		return;

	QString uin = currentContextBuddy->getUin();
	QString name = currentContextBuddy->getName();
	emit incSnacSeq();
	QByteArray packet2;
	packet2[0] = 0x2a;
	packet2[1] = 0x02;
	packet2.append(convertToByteArray((quint16)*flapSeq));
	packet2.append(convertToByteArray((quint16)(24 + name.toUtf8().length() + uin.toUtf8().length())));

	snac snac1308;
	snac1308.setFamily(0x0013);
	snac1308.setSubType(0x0008);
	snac1308.setReqId(*snacSeq);
	packet2.append(snac1308.getData());


	packet2.append(convertToByteArray((quint16)uin.toUtf8().length()));
	packet2.append(uin.toUtf8());

	packet2.append(convertToByteArray((quint16)0));
	packet2.append(convertToByteArray((quint16)currentContextBuddy->itemId));

	packet2.append(convertToByteArray((quint16)0x000e));

	packet2.append(convertToByteArray((quint16)(4 + name.toUtf8().length())));
	packet2.append(convertToByteArray((quint16)0x0131));
	packet2.append(convertToByteArray((quint16)name.toUtf8().length()));

	packet2.append(name.toUtf8());


		emit incFlapSeq();



	tcpSocket->write(packet2);


	modifyObject visBuddy;

	visBuddy.itemId = currentContextBuddy->itemId;
	visBuddy.groupId = 0;
	visBuddy.itemType = 0x000e;
	visBuddy.operation = 0;
	visBuddy.buddyName = name;
	visBuddy.buddyUin = uin;

	modifyReqList.append(visBuddy);

	ignoreList.append(uin);

	itemFromList object;
	object.groupId = currentContextBuddy->groupID;
	object.itemId = currentContextBuddy->itemId;
	ignoreObjectList.insert(uin, object);

        QSettings contacts(QSettings::NativeFormat, QSettings::UserScope, "qutim/qutim."+m_profile_name+"/ICQ."+account_name, "contactlist");
	contacts.setValue("list/ignore", ignoreList);
	if ( privacyListWindowOpen)
		privacyWindow->createLists();
	currentContextBuddy->m_ignore_contact = true;
	currentContextBuddy->setCustomIcon(m_icq_plugin_system.getIcon("ignorelist"),7);
}

void contactListTree::requestAuthorizationActionTriggered()
{
	requestAuthDialog dialog;

	if ( QtopiaApplication::execDialog(&dialog) )
	{
		QString uin = currentContextBuddy->getUin();
		QString msg = dialog.getMessage();

		QByteArray authPack;

		emit incSnacSeq();
		QByteArray packet2;
		packet2[0] = 0x2a;
		packet2[1] = 0x02;
		packet2.append(convertToByteArray((quint16)*flapSeq));
		packet2.append(convertToByteArray((quint16)(15 + uin.toUtf8().length())));

		snac snac1314;
		snac1314.setFamily(0x0013);
		snac1314.setSubType(0x0014);
		snac1314.setReqId(*snacSeq);
		packet2.append(snac1314.getData());

		packet2[packet2.length()] = (quint8)uin.toUtf8().length();
		packet2.append(uin.toUtf8());


		packet2.append(convertToByteArray((quint16)0));
		packet2.append(convertToByteArray((quint16)0));

		authPack.append(packet2);

		emit incFlapSeq();
		emit incSnacSeq();


		QByteArray packet;
		packet[0] = 0x2a;
		packet[1] = 0x02;
		packet.append(convertToByteArray((quint16)*flapSeq));
		packet.append(convertToByteArray((quint16)(15 + uin.toUtf8().length() + msg.toUtf8().length())));

		snac snac1318;
		snac1318.setFamily(0x0013);
		snac1318.setSubType(0x0018);
		snac1318.setReqId(*snacSeq);
		packet.append(snac1318.getData());

		packet[packet.length()] = (quint8)uin.toUtf8().length();
		packet.append(uin.toUtf8());


		packet.append(convertToByteArray((quint16)msg.toUtf8().length()));
		packet.append(msg.toUtf8());
		packet.append(convertToByteArray((quint16)0));

		authPack.append(packet);
		emit incFlapSeq();
		tcpSocket->write(authPack);


	}
}

void contactListTree::getAuthorizationRequest(quint16 length)
{
	socket->read(8);

	length -= 8;

	quint8 uinLength = convertToInt8(socket->read(1));
	length--;

	QString uin = socket->read(uinLength);
	length -= uinLength;
	quint16 msgLength = byteArrayToInt16(socket->read(2));
	length -= 2;

	QString msg = QString::fromUtf8(socket->read(msgLength));
	length -= msgLength;

	socket->read(2);
	length -= 2;





	if ( buddyList.contains(uin) )
	{
		treeBuddyItem *buddy = buddyList.value(uin);

		Q_ASSERT(buddy);
		if (buddy)
		{
			buddy->waitingForAuth(true);
			buddy->authMessage = msg;
			addMessageFromContact(buddy->getUin(),buddy->groupID, msg,
					QDateTime::currentDateTime());
		}

	}
	else {

	if ( checkMessageForValidation(uin, "", 1) )
	{

                        QSettings settings(QSettings::NativeFormat, QSettings::UserScope, "qutim/qutim."+m_profile_name+"/ICQ."+account_name, "contactlist");

			treeGroupItem *group = groupList.value(0);

//			Q_ASSERT(group);
			if (!group)
				return ;

			treeBuddyItem *buddy = new treeBuddyItem(icqUin, m_profile_name);


			Q_ASSERT(buddy);
			if (!buddy)
				return ;

			initializeBuddy(buddy);
			buddy->underline = !dontUnderlineNotAutho;
			buddy->groupID = 0;
			buddy->groupName = group->name;
			group->userCount++;
			group->updateText();
			buddyList.insert(uin, buddy);
			buddy->setBuddyUin(uin);
			buddy->setName(uin);
			buddy->updateBuddyText();
				updateNil();
			requestUinInformation(buddy->getUin());
			settings.beginGroup(buddy->getUin());
			settings.setValue("name", buddy->getUin());
			settings.setValue("groupid", 0);
			settings.setValue("nickname", buddy->getName());
			settings.endGroup();
			addContactToCL(0,buddy->getUin(), buddy->getName());
			QStringList contacts = settings.value("list/contacts").toStringList();
			contacts<<buddy->getUin();
			settings.setValue("list/contacts", contacts);

			if ( contactListChanged  && isMergeAccounts )
						emit reupdateList();

			buddy->waitingForAuth(true);
			buddy->authMessage = msg;
			addMessageFromContact(buddy->getUin(),buddy->groupID, msg,
					QDateTime::currentDateTime());
	}
	}




	if ( length)
		socket->read(length);
}

void contactListTree::openAuthReqFromBuddy(treeBuddyItem *buddy)
{

	acceptAuthDialog *dialog = new acceptAuthDialog(buddy->getUin());
	dialog->setWindowTitle(tr("Accept authorization from %1").arg(buddy->getName()));
	dialog->setMessage(buddy->authMessage);
	buddy->waitingForAuth(false);

	connect(dialog, SIGNAL(sendAuthReqAnswer(bool, const QString &)), this, SLOT(sendAuthReqAnswer(bool, const QString &)));
	QtopiaApplication::showDialog(dialog);
}

void contactListTree::authorizationAcceptedAnswer(quint16 length)
{
	socket->read(8);
	length -= 8;

	quint8 uinLength = convertToInt8(socket->read(1));
	length--;

	QString uin = socket->read(uinLength);
	length -= uinLength;

	quint8 mesgAccept = convertToInt8(socket->read(1));
	length --;

	QString message;
	if ( mesgAccept )
	{
		message = tr("Authorization accepted");
		if ( buddyList.contains(uin) )
		{
			buddyList.value(uin)->setNotAuthorizated(false);
			buddyList.value(uin)->updateBuddyText();
		}
	}
	else
		message = tr("Authorization declined");

	if ( buddyList.contains(uin))
		emit userMessage(uin, buddyList.value(uin)->getName(), message, messageNotification, true);
	else
		emit userMessage(uin, uin, message, messageNotification, false);

	if ( length )
		socket->read(length);
}

void contactListTree::addToContactListActionTriggered()
{
	addUserToList(currentContextBuddy->getUin(), currentContextBuddy->getName(), currentContextBuddy->getNotAutho());
}

void contactListTree::allowToAddMeTriggered()
{
	QString uin = currentContextBuddy->getUin();
	emit incSnacSeq();
	QByteArray packet2;
	packet2[0] = 0x2a;
	packet2[1] = 0x02;
	packet2.append(convertToByteArray((quint16)*flapSeq));
	packet2.append(convertToByteArray((quint16)(15 + uin.toUtf8().length())));

	snac snac1314;
	snac1314.setFamily(0x0013);
	snac1314.setSubType(0x0014);
	snac1314.setReqId(*snacSeq);
	packet2.append(snac1314.getData());

	packet2[packet2.length()] = (quint8)uin.toUtf8().length();
	packet2.append(uin.toUtf8());


	packet2.append(convertToByteArray((quint16)0));
	packet2.append(convertToByteArray((quint16)0));


	emit incFlapSeq();

	tcpSocket->write(packet2);
}

void contactListTree::removeMyselfTriggered()
{
	QString uin = currentContextBuddy->getUin();
	emit incSnacSeq();
	QByteArray packet2;
	packet2[0] = 0x2a;
	packet2[1] = 0x02;
	packet2.append(convertToByteArray((quint16)*flapSeq));
	packet2.append(convertToByteArray((quint16)(11 + uin.toUtf8().length())));

	snac snac1316;
	snac1316.setFamily(0x0013);
	snac1316.setSubType(0x0016);
	snac1316.setReqId(*snacSeq);
	packet2.append(snac1316.getData());

	packet2[packet2.length()] = (quint8)uin.toUtf8().length();
	packet2.append(uin.toUtf8());


	emit incFlapSeq();

	tcpSocket->write(packet2);
}

void contactListTree::sendFile(QByteArray &part1, QByteArray &part2, QByteArray &part3)
{
	QByteArray packet;
	emit incSnacSeq();
	packet[0] = 0x2a;
	packet[1] = 0x02;
	packet.append(convertToByteArray((quint16)*flapSeq));
	QByteArray tmpPacket;

	snac snac0406;
	snac0406.setFamily(0x0004);
	snac0406.setSubType(0x0006);
	snac0406.setReqId(*snacSeq);
	tmpPacket.append(snac0406.getData());


	tmpPacket.append(part1);

	tmpPacket.append(convertToByteArray((quint16)0x0005));

	tmpPacket.append(convertToByteArray((quint16)(24 + part2.length() + part3.length())));

	quint32 localIP = tcpSocket->localAddress().toIPv4Address();

	tmpPacket.append(part2);
	tlv tlv02;
	tlv02.setType(0x0002);
	tlv02.setData((quint32)localIP);

	tlv tlv16;
	tlv16.setType(0x0016);
	tlv16.setData((quint32)(localIP ^ 0xffffffff));

	tlv tlv03;
	tlv03.setType(0x0003);
	tlv03.setData((quint32)localIP);

	tmpPacket.append(tlv02.getData());
	tmpPacket.append(tlv16.getData());
	tmpPacket.append(tlv03.getData());

	tmpPacket.append(part3);

	packet.append(convertToByteArray((quint16)tmpPacket.length()));
	packet.append(tmpPacket);

	emit incFlapSeq();

	tcpSocket->write(packet);

}

void contactListTree::sendFileActionTriggered()
{
        sendFileFromWindow(currentContextBuddy->getUin());
}

void contactListTree::sendCancelSending(QByteArray &part)
{
	QByteArray packet;
	emit incSnacSeq();
	packet[0] = 0x2a;
	packet[1] = 0x02;
	packet.append(convertToByteArray((quint16)*flapSeq));
	packet.append(convertToByteArray((quint16)(10 + part.length())));
	snac snac0406;
	snac0406.setFamily(0x0004);
	snac0406.setSubType(0x0006);
	snac0406.setReqId(*snacSeq);
	packet.append(snac0406.getData());
	packet.append(part);

	emit incFlapSeq();
	tcpSocket->write(packet);
}

void contactListTree::redirectToProxy(const QByteArray &part)
{
	QByteArray packet;
	emit incSnacSeq();
	packet[0] = 0x2a;
	packet[1] = 0x02;
	packet.append(convertToByteArray((quint16)*flapSeq));
	packet.append(convertToByteArray((quint16)(10 + part.length())));
	snac snac0406;
	snac0406.setFamily(0x0004);
	snac0406.setSubType(0x0006);
	snac0406.setReqId(*snacSeq);
	packet.append(snac0406.getData());
	packet.append(part);

	emit incFlapSeq();
	tcpSocket->write(packet);
}

void contactListTree::sendAcceptMessage(const QByteArray &part)
{
	QByteArray packet;
	emit incSnacSeq();
	packet[0] = 0x2a;
	packet[1] = 0x02;
	packet.append(convertToByteArray((quint16)*flapSeq));
	packet.append(convertToByteArray((quint16)(10 + part.length())));
	snac snac0406;
	snac0406.setFamily(0x0004);
	snac0406.setSubType(0x0006);
	snac0406.setReqId(*snacSeq);
	packet.append(snac0406.getData());
	packet.append(part);

	emit incFlapSeq();
	tcpSocket->write(packet);
}


void contactListTree::sendImage(const QString &uin, const QString &path)
{
}

void contactListTree::readXstatusTriggered()
{
	emit incSnacSeq();
	icqMessage message(codepage);
	message.requestXStatus(tcpSocket, currentContextBuddy->getUin(),
			icqUin, *flapSeq, *snacSeq);
	emit incFlapSeq();




	readAwayDialog *dialog = new readAwayDialog;
	dialog->setWindowTitle(tr("%1 xStatus message").arg(currentContextBuddy->getName()));
	dialog->setAttribute(Qt::WA_QuitOnClose, false);
	dialog->setAttribute(Qt::WA_DeleteOnClose, true);
		connect( dialog, SIGNAL(destroyed ( QObject *)),
				this, SLOT(deleteAwayWindow(QObject *)));
	QtopiaApplication::showDialog(dialog);
	awayMessageList.insert(message.msgCookie, dialog);
}

void contactListTree::sendAuthReqAnswer(bool accep, const QString &uin)
{
	emit incSnacSeq();


	QByteArray packet;
	packet[0] = 0x2a;
	packet[1] = 0x02;
	packet.append(convertToByteArray((quint16)*flapSeq));
	packet.append(convertToByteArray((quint16)(16 + uin.toUtf8().length())));

	snac snac131a;
	snac131a.setFamily(0x0013);
	snac131a.setSubType(0x001a);
	snac131a.setReqId(*snacSeq);
	packet.append(snac131a.getData());

	packet[packet.length()] = (quint8)uin.toUtf8().length();
	packet.append(uin.toUtf8());


	if ( accep)
		packet[packet.length()] = (quint8)1;
	else
		packet[packet.length()] = (quint8)0;

	packet.append(convertToByteArray((quint16)0));
	packet.append(convertToByteArray((quint16)0));

	emit incFlapSeq();

	tcpSocket->write(packet);
}

void contactListTree::hideRoot(bool flag)
{

}



QString contactListTree::addXstatusMessage(const QString &xtrauin,QByteArray &msg)
{
	if (msg.contains(QByteArray::fromHex("4177617920537461747573204d657373616765")))
	{


		msg = msg.right(msg.length() - 19);
		QString xtraaw = xTraAway(QString::fromUtf8(msg));
		if ( buddyList.contains(xtrauin))
		{
			treeBuddyItem *buddy = buddyList.value(xtrauin);
			buddy->setXstatusCaptionAndMessage(xtraaw, "");
			buddy->setXstatusText();
			chatWindowOpened(buddy->getUin());
		}

		return xtraaw;

	}
	else
	{
		msg = msg.right(msg.length() - 98);
		msg.chop(13);
		QString title = findTitle(QString::fromUtf8(msg));
		QString message = findMessage(QString::fromUtf8(msg));

		if ( buddyList.contains(xtrauin))
		{
			treeBuddyItem *buddy = buddyList.value(xtrauin);
			buddy->setXstatusCaptionAndMessage(title, message);
			buddy ->setXstatusText();
			chatWindowOpened(buddy->getUin());
		}
		QString xtraaw = "<b>" + title +"</b><br>" + message.replace("\n", "<br>");
		return xtraaw;
	}
}

QString contactListTree::findTitle(QString _givenText)
{
	QRegExp regExp("[&][l][t][;][t][i][t][l][e][&][g][t][;](.+)[&][l][t][;][/][t][i][t][l][e][&][g][t][;]");
	int pos = 0;
	regExp.indexIn(_givenText, pos);
	QString rez = regExp.cap(0);
	return rez.mid(13, rez.length() - 27);
}

QString contactListTree::findMessage(QString _givenText)
{
	QRegExp regExp("[&][l][t][;][d][e][s][c][&][g][t][;](.+)[&][l][t][;][/][d][e][s][c][&][g][t][;]");
	int pos = 0;
	regExp.indexIn(_givenText, pos);
	QString rez = regExp.cap(0);
	return rez.mid(12, rez.length() - 25);
}

QString contactListTree::xTraAway(QString _givenText)
{

	QRegExp regExp("[<][B][O][D][Y][>](.+)[<][/][B][O][D][Y][>]");
	int pos = 0;
	regExp.indexIn(_givenText, pos);
	QString rez = regExp.cap(0);

	return rez.mid(6, rez.length() - 13);
}

void contactListTree::offlineHideButtonClicked(bool flag)
{

	if ( showOffline != flag)
	{

		showOffline = flag;

                QSettings settings(QSettings::NativeFormat, QSettings::UserScope, "qutim/qutim."+m_profile_name, "icqsettings");
		settings.setValue("contactlist/hideoff", showOffline);
		showOfflineUsers();
	}
}

void contactListTree::askForXstatusTimerTick()
{
	if ( xStatusTickList.count() )
	{

		treeBuddyItem *buddy = xStatusTickList.at(0);

		if ( buddy->m_xstatus_already_readed )
		{
			chatWindowOpened(buddy->getUin());
		}

		if (buddy->xStatusPresent && showXstatusesinToolTips &&
				!buddy->m_xstatus_already_readed )
		{
			if ( !buddy->icqLite)
			{
			emit incSnacSeq();
			icqMessage message(codepage);
			message.requestXStatus(tcpSocket, buddy->getUin(),
					icqUin, *flapSeq, *snacSeq);
			emit incFlapSeq();
			} else {
				emit incSnacSeq();
				icqMessage message(codepage);
				message.awayType = 0x1a;
				message.requestAutoreply(tcpSocket, buddy->getUin(),*flapSeq, *snacSeq);
				emit incFlapSeq();
			}
		}
		else
		{
			xStatusTickList.removeAt(0);
			askForXstatusTimerTick();
		}

		xStatusTickList.removeAt(0);
		QTimer::singleShot(500,this, SLOT(askForXstatusTimerTick()));
	} else
		letMeAskYouAboutXstatusPlease = true;
}

void contactListTree::onUpdateTranslation()
{
	createGroup->setText(tr("New group"));
	renameGroup->setText(tr("Rename group"));
	deleteGroup->setText(tr("Delete group"));
	sendMessageAction->setText(tr("Send message"));
	userInformationAction->setText(tr("Contact details"));
	copyUINAction->setText(tr("Copy UIN to clipboard"));
	statusCheckAction->setText(tr("Contact status check"));
	messageHistoryAction->setText(tr("Message history"));
	readAwayAction->setText(tr("Read away message"));
	renameContactAction->setText(tr("Rename contact"));
	deleteContactAction->setText(tr("Delete contact"));
	moveContactAction->setText(tr("Move to group"));
	addToVisibleAction->setText(tr("Add to visible list"));
	addToInvisibleAction->setText(tr("Add to invisible list"));
	addToIgnoreAction->setText(tr("Add to ignore list"));
	requestAuthorizationAction->setText(tr("Authorization request"));
	addToContactListAction->setText(tr("Add to contact list"));
	allowToAddMe->setText(tr("Allow contact to add me"));
	removeMyself->setText(tr("Remove myself from contact's list"));
	readXstatus->setText(tr("Read custom status"));
}

void contactListTree::onReloadGeneralSettings()
{
}

void contactListTree::sendFileFromWindow(const QString &uin)
{
}

void contactListTree::deleteFromIgnoreActionTriggered()
{
	deleteFromPrivacyList(currentContextBuddy->getUin(), 2);
	if ( privacyListWindowOpen)
		privacyWindow->createLists();
	currentContextBuddy->m_ignore_contact = false;
	currentContextBuddy->setCustomIcon(QIcon(),7);
}

void contactListTree::deleteFromInvisibleActionTriggered()
{
	deleteFromPrivacyList(currentContextBuddy->getUin(), 1);
	if ( privacyListWindowOpen)
		privacyWindow->createLists();
	currentContextBuddy->m_invisible_contact = false;
	currentContextBuddy->setCustomIcon(QIcon(),6);
}

void contactListTree::deleteFromVisibleActionTriggered()
{
	deleteFromPrivacyList(currentContextBuddy->getUin(), 0);
	if ( privacyListWindowOpen)
		privacyWindow->createLists();
	currentContextBuddy->m_visible_contact = false;
	currentContextBuddy->setCustomIcon(QIcon(),5);
}

void contactListTree::copyUINActionTriggered()
{
	QApplication::clipboard()->setText(currentContextBuddy->getUin());
}

void contactListTree::editNoteActionTriggered()
{
	noteWidget *noteW = new noteWidget(icqUin, currentContextBuddy->getUin(), currentContextBuddy->getName(), m_profile_name);
	connect( noteW, SIGNAL(destroyed ( QObject *)),
			this, SLOT(deleteNoteWindow(QObject *)));
	noteWidgetsList.insert(currentContextBuddy->getUin(), noteW);
	noteW->show();
}

void contactListTree::onStatusChanged(accountStatus status)
{
        if (currentStatus == status) return;

        // playing sounds
        // going online
        if (((currentStatus == offline) || (currentStatus == connecting))
                && ((status != offline) && (status != connecting)))
                emit playSoundEvent(SoundEvent::Connected, status);
        // going offline
        else if (status == offline)
                emit playSoundEvent(SoundEvent::Disconnected, offline);

        // saving status
        currentStatus = status;
}

void contactListTree::deleteNoteWindow(QObject *obj)
{
	noteWidget *tempWindow = (noteWidget  *)(obj);
	noteWidgetsList.remove(noteWidgetsList.key(tempWindow));
}

void contactListTree::getChangeFontSignal(const QFont &f, const QColor &fc, const QColor &c)
{
}

void contactListTree::getMessageAck(quint16 length)
{
	QByteArray msgCookie = socket->read(8);
	socket->read(2);
	length -= 10;

	quint8 uinLength = convertToInt8(socket->read(1));
	length--;

	QString uin = socket->read(uinLength);

	length -= uinLength;

	if ( length )
		socket->read(length);

//	if ( chatWindowList.contains(uin) && messageCursorPositions.contains(msgCookie))
//	{
//		chatWindowList.value(uin)->messageApply(messageCursorPositions.value(msgCookie));
//	}
//	if ( messageCursorPositions.contains(msgCookie))
//	{
//		messageDelievered(uin,0,messageCursorPositions.value(msgCookie));
//	}
//
//	messageCursorPositions.remove(msgCookie);
}

QByteArray contactListTree::convertPassToCodePage(const QString &pass)
{
	if (!codec)
		return pass.toLocal8Bit();

	return codec->fromUnicode(pass);
}

void contactListTree::addGroupToCL(quint16 group_id, const QString &group_name)
{
	TreeModelItem group_item;
	group_item.m_protocol_name = "ICQ";
	group_item.m_account_name = icqUin;
	group_item.m_item_name = group_id?QString::number(group_id):"";
	group_item.m_parent_name = icqUin;
	group_item.m_item_type = TreeModelItem::Group;
	m_icq_plugin_system.addItemToContactList(group_item, group_name);
}

void contactListTree::renameGroupInCL(const QString &new_name,
		quint16 group_id)
{
	TreeModelItem group_item;
	group_item.m_protocol_name = "ICQ";
	group_item.m_account_name = icqUin;
	group_item.m_item_name = group_id?QString::number(group_id):"";
	group_item.m_parent_name = icqUin;
	group_item.m_item_type = TreeModelItem::Group;
	m_icq_plugin_system.setContactItemName(group_item, new_name);
}

void contactListTree::addContactToCL(quint16 group_id, const QString &contact_uin, const QString& contact_name)
{
	TreeModelItem contact_item;
	contact_item.m_protocol_name = "ICQ";
	contact_item.m_account_name = icqUin;
	contact_item.m_item_name = contact_uin;
	contact_item.m_parent_name = group_id?QString::number(group_id):"";
	contact_item.m_item_type = TreeModelItem::Buddy;
	m_icq_plugin_system.addItemToContactList(contact_item, contact_name);
	if ( group_id )
	{
		m_icq_plugin_system.setContactItemStatus(contact_item, statusIconClass::getInstance()->getOfflineIcon(),
				"offline", 1000);
	}
	else
	{
//		m_icq_plugin_system.setContactItemIcon(contact_item, m_icq_plugin_system.getStatusIcon("noauth", "icq"), 0);
		m_icq_plugin_system.setContactItemStatus(contact_item, m_icq_plugin_system.getStatusIcon("noauth", "icq"),
				"offline", 1000);
	}
	if ( buddyList.contains(contact_uin) )
	{
		initializeBuddy(buddyList.value(contact_uin));
	}
}

void contactListTree::renameContactInCL(quint16 group_id,
		const QString &contact_uin, const QString& contact_name)
{
	TreeModelItem contact_item;
	contact_item.m_protocol_name = "ICQ";
	contact_item.m_account_name = icqUin;
	contact_item.m_item_name = contact_uin;
	contact_item.m_parent_name = group_id?QString::number(group_id):"";
	contact_item.m_item_type = TreeModelItem::Buddy;
	m_icq_plugin_system.setContactItemName(contact_item, contact_name);
}

void contactListTree::moveContactFromGroupToGroup(quint16 old_group_id,
		quint16 new_group_id, const QString &contact_uin)
{
	TreeModelItem old_item;
	old_item.m_protocol_name = "ICQ";
	old_item.m_account_name = icqUin;
	old_item.m_item_name = contact_uin;
	old_item.m_parent_name = old_group_id?QString::number(old_group_id):"";
	old_item.m_item_type = TreeModelItem::Buddy;
	TreeModelItem new_item;
	new_item.m_protocol_name = "ICQ";
	new_item.m_account_name = icqUin;
	new_item.m_item_name = contact_uin;
	new_item.m_parent_name = new_group_id?QString::number(new_group_id):"";
	new_item.m_item_type = TreeModelItem::Buddy;
	m_icq_plugin_system.moveItemInContactList(old_item, new_item);
}

void contactListTree::removeGroupFromCl(quint16 group_id)
{
	TreeModelItem group_item;
	group_item.m_protocol_name = "ICQ";
	group_item.m_account_name = icqUin;
	group_item.m_item_name = group_id?QString::number(group_id):"";
	group_item.m_parent_name = icqUin;
	group_item.m_item_type = TreeModelItem::Group;
	m_icq_plugin_system.removeItemFromContactList(group_item);
}

void contactListTree::removeContactFromCl(quint16 group_id, const QString &contact_uin)
{
	TreeModelItem contact_item;
	contact_item.m_protocol_name = "ICQ";
	contact_item.m_account_name = icqUin;
	contact_item.m_item_name = contact_uin;
	contact_item.m_parent_name = group_id?QString::number(group_id):"";
	contact_item.m_item_type = TreeModelItem::Buddy;
	m_icq_plugin_system.removeItemFromContactList(contact_item);
}

void contactListTree::setPrivacyIconsToContacts()
{
	foreach(const QString &uin, visibleList)
	{
		if ( buddyList.contains(uin) )
		{
			buddyList.value(uin)->m_visible_contact = true;
			buddyList.value(uin)->setCustomIcon(m_icq_plugin_system.getIcon("visible"),5);
		}
	}
	foreach(const QString &uin, invisibleList)
	{
		if ( buddyList.contains(uin) )
		{
			buddyList.value(uin)->m_invisible_contact = true;
			buddyList.value(uin)->setCustomIcon(m_icq_plugin_system.getIcon("privacy"),6);
		}
	}
	foreach(const QString &uin, ignoreList)
	{
		if ( buddyList.contains(uin) )
		{
			buddyList.value(uin)->m_ignore_contact = true;
			buddyList.value(uin)->setCustomIcon(m_icq_plugin_system.getIcon("ignorelist"),7);
		}
	}
}

void contactListTree::itemActivated(const QString &clicked_uin)
{

	if ( buddyList.contains(clicked_uin) )
	{
		doubleClickedBuddy(buddyList.value(clicked_uin));
	}
}

void contactListTree::showItemContextMenu(const QList<QAction*> &action_list,
		const QString &item_name, int item_type, const QPoint &menu_point)
{
	if ( item_type == 1 )
	{

		if ( groupList.contains(item_name.toUInt()))
		{
			showGroupMenu(groupList.value(item_name.toUInt()), menu_point);
		}
	}
	else if ( item_type == 0)
	{
		if ( buddyList.contains(item_name) )
		{
			showBuddyMenu(action_list,buddyList.value(item_name), menu_point);
		}
	}
}

void contactListTree::addMessageFromContact(const QString &contact_uin,
		quint16 group_id, const QString &message, const QDateTime &message_date)
{
	TreeModelItem contact_item;
	contact_item.m_protocol_name = "ICQ";
	contact_item.m_account_name = icqUin;
	contact_item.m_item_name = contact_uin;
	contact_item.m_parent_name = group_id?QString::number(group_id):"";
	contact_item.m_item_type = TreeModelItem::Buddy;
	m_icq_plugin_system.addMessageFromContact(contact_item, message, message_date);
}

void contactListTree::sendMessageTo(const QString &contact_uin, const QString &message, int message_icon_position)
{
	messageFormat new_message;
	new_message.date = QDateTime::currentDateTime();
	new_message.fromUin = contact_uin;
	new_message.from = icqUin;
	new_message.message = message;
	new_message.position = message_icon_position;
	sendMessage(new_message);
}

void contactListTree::addServiceMessage(const QString &contact_uin, quint16 group_id, const QString &message)
{
	TreeModelItem contact_item;
	contact_item.m_protocol_name = "ICQ";
	contact_item.m_account_name = icqUin;
	contact_item.m_item_name = contact_uin;
	contact_item.m_parent_name = group_id?QString::number(group_id):"";
	contact_item.m_item_type = TreeModelItem::Buddy;
	m_icq_plugin_system.addServiceMessage(contact_item, message);
}

void contactListTree::createChat(const QString &uin, quint16 group_id)
{
	TreeModelItem contact_item;
	contact_item.m_protocol_name = "ICQ";
	contact_item.m_account_name = icqUin;
	contact_item.m_item_name = uin;
	contact_item.m_parent_name = group_id?QString::number(group_id):"";
	contact_item.m_item_type = TreeModelItem::Buddy;
	m_icq_plugin_system.createChat(contact_item);
}

QStringList contactListTree::getAdditionalInfoAboutContact(const QString &item_name, int item_type ) const
{
	if ( !item_type )
	{
		if (item_name == icqUin)
		{
                        QSettings settings(QSettings::NativeFormat, QSettings::UserScope, "qutim/qutim."+m_profile_name+"/ICQ."+account_name, "accountsettings");
			QStringList additional_info_list;
			QString avatar_hash = settings.value("main/iconhash").toByteArray();
			additional_info_list<<accountNickname
								<<(avatar_hash.isEmpty()?"":(iconPath + avatar_hash));
			return additional_info_list;
		}
		else if ( buddyList.contains(item_name) )
		{
                        QSettings contacts(QSettings::NativeFormat, QSettings::UserScope, "qutim/qutim."+m_profile_name+"/ICQ."+account_name, "contactlist");
			QStringList additional_info_list;
			treeBuddyItem *tmp_buddy = buddyList.value(item_name);
			QString avatar_hash = tmp_buddy->getAvatarHash().toHex();
			additional_info_list<<tmp_buddy->getName()
								<<(avatar_hash.isEmpty()?"":(iconPath + avatar_hash))
								<<tmp_buddy->clientId
								<<contacts.value(item_name + "/note","").toString();
			return additional_info_list;
		}
	}
	QStringList tmp_list;
	return tmp_list;
}

void contactListTree::addImage(const QString &contact_uin, quint16 group_id,
		const QByteArray &image_raw)
{
	TreeModelItem contact_item;
	contact_item.m_protocol_name = "ICQ";
	contact_item.m_account_name = icqUin;
	contact_item.m_item_name = contact_uin;
	contact_item.m_parent_name = group_id?QString::number(group_id):"";
	contact_item.m_item_type = TreeModelItem::Buddy;
	m_icq_plugin_system.addImage(contact_item,image_raw);
}

void contactListTree::contactTyping(const QString &contact_uin,
		quint16 group_id, bool typing)
{
	TreeModelItem contact_item;
	contact_item.m_protocol_name = "ICQ";
	contact_item.m_account_name = icqUin;
	contact_item.m_item_name = contact_uin;
	contact_item.m_parent_name = group_id?QString::number(group_id):"";
	contact_item.m_item_type = TreeModelItem::Buddy;
	m_icq_plugin_system.contactTyping(contact_item, typing);
}

void contactListTree::messageDelievered(const QString &contact_uin,
		quint16 group_id, int position)
{
	TreeModelItem contact_item;
	contact_item.m_protocol_name = "ICQ";
	contact_item.m_account_name = icqUin;
	contact_item.m_item_name = contact_uin;
	contact_item.m_parent_name = group_id?QString::number(group_id):"";
	contact_item.m_item_type = TreeModelItem::Buddy;
	m_icq_plugin_system.messageDelievered(contact_item, position);
}

bool contactListTree::checkMessageForValidation(const QString &contact_uin, const QString &message,
		int message_type)
{
        QSettings settings(QSettings::NativeFormat, QSettings::UserScope, "qutim/qutim."+m_profile_name+"/ICQ."+account_name, "accountsettings");
	quint32 privacy = settings.value("statuses/privacy", 4).toUInt();

	TreeModelItem contact_item;
	contact_item.m_protocol_name = "ICQ";
	contact_item.m_account_name = icqUin;
	contact_item.m_item_name = contact_uin;
	contact_item.m_parent_name = "";
	contact_item.m_item_type = TreeModelItem::Buddy;

	return m_icq_plugin_system.checkForMessageValidation(contact_item, message,
			message_type, privacy == 5 ? true : false);
}

void contactListTree::contactSettingsChanged()
{
        QSettings settings(QSettings::NativeFormat, QSettings::UserScope, "qutim/qutim."+m_profile_name, "icqsettings");
	settings.beginGroup("contacts");
	m_show_xstatus_icon = settings.value("xstaticon", true).toBool();
	m_show_birthday_icon = settings.value("birthicon", true).toBool();
	m_show_auth_icon = settings.value("authicon", true).toBool();
	m_show_vis_icon = settings.value("visicon", true).toBool();
	m_show_invis_icon = settings.value("invisicon", true).toBool();
	m_show_ignore_icon = settings.value("ignoreicon", true).toBool();
	m_show_xstatus_text = settings.value("xstattext", true).toBool();
	settings.endGroup();

	foreach(treeBuddyItem *buddy, buddyList)
	{
		initializeBuddy(buddy);
	}
}

void contactListTree::notifyAboutBirthday(const QString &contact_uin, quint16 group_id)
{
	TreeModelItem contact_item;
	contact_item.m_protocol_name = "ICQ";
	contact_item.m_account_name = icqUin;
	contact_item.m_item_name = contact_uin;
	contact_item.m_parent_name = group_id?QString::number(group_id):"";
	contact_item.m_item_type = TreeModelItem::Buddy;
	m_icq_plugin_system.notifyAboutBirthDay(contact_item);
}

void contactListTree::moveItemSignalFromCL(const TreeModelItem &old_item, const TreeModelItem &new_item)
{

	if ( iAmOnline && buddyList.contains(old_item.m_item_name )
			&& groupList.contains(new_item.m_parent_name.toUInt()))
	{
		treeBuddyItem *buddy = buddyList.value(old_item.m_item_name);
		QString uin = buddy->getUin();
		QString name = buddy->getName();
		bool authReq = buddy->getNotAutho();
		currentContextBuddy = buddy;
		movingBuddy = true;
		deleteContactActionTriggered();
		sendUserAddReq(uin, name,authReq ,
				groupList.value(new_item.m_parent_name.toUInt())->name);
	}
}

void contactListTree::deleteItemSignalFromCL(const QString &item_name, int item_type)
{
	if ( iAmOnline )
	{
		if ( !item_type && buddyList.contains(item_name))
		{
			currentContextBuddy = buddyList.value(item_name);
			deleteContactActionTriggered();
		}
		else if (item_type == 1 && groupList.contains(item_name.toInt()))
		{
			currentContextGroup = groupList.value(item_name.toInt());
			deleteSelectedGroup();
		}
	}
}

const QString contactListTree::getItemToolTip(const QString &contact_name)
{
	if ( buddyList.contains(contact_name) )
	{
		return buddyList.value(contact_name)->createToolTip();
	}
  return QString::null;
}

void contactListTree::clearPrivacyLists()
{
	visibleList.clear();
	invisibleList.clear();
	ignoreList.clear();
}

void contactListTree::chatWindowOpened(const QString &contact_uin, bool new_wind)
{
	if ( buddyList.contains(contact_uin) )
	{
		treeBuddyItem *buddy = buddyList.value(contact_uin);
		QString xstatus_caption, xstatus_message;
		xstatus_caption = buddy->xStatusCaption;
		xstatus_message = buddy->xStatusMsg;
		if ( ( !xstatus_caption.trimmed().isEmpty()
				|| !xstatus_message.trimmed().isEmpty() ) && ( buddy->m_xstatus_changed || new_wind ))
		{
			QString tmp_xstatus;
			if ( !xstatus_caption.trimmed().isEmpty())
			{
				tmp_xstatus.append(xstatus_caption);
				if ( !xstatus_message.trimmed().isEmpty() )
					tmp_xstatus.append(" - ");
			}

			if ( !xstatus_message.trimmed().isEmpty())
				tmp_xstatus.append(xstatus_message);
			addServiceMessage(contact_uin, buddy->groupID, tmp_xstatus);
		}
		doubleClickedBuddy(buddy);
	}
}

void contactListTree::sendMessageRecieved(const QString &uin, const QByteArray &msg_cookie)
{
	if ( !msg_cookie.isEmpty() )
	{
	emit incSnacSeq();
	icqMessage recieve_message(icqUin);
	recieve_message.sendMessageRecieved(tcpSocket, uin, msg_cookie, *flapSeq, *snacSeq);
	emit incFlapSeq();
	}
}
