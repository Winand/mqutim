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


#ifndef ICQACCOUNT_H
#define ICQACCOUNT_H

#include <QtGui/QWidget>
#include <QIcon>
#include <QVector>
#include <QHBoxLayout>

#include "oscarprotocol.h"
#include "quticqglobals.h"
#include "icqpluginsystem.h"
#include "accountbutton.h"
//#include "soundevents.h"

// Qt stndard classes
class QSystemTrayIcon;
class QGridLayout;
class QMenu;
class QAction;
class QTreeWidget;
class QTreeWidgetItem;
class QStackedWidget;
class QSystemTrayIcon;
class QActionGroup;

// qutIM own classes
class accountButton;
class icqSettings;
class networkSettings;
class statusSettings;
class oscarProtocol;
class statusDialog;
class customStatusDialog;
class SoundEvents;

class icqAccount : public QObject
{
    Q_OBJECT

public:
    icqAccount(QString, const QString &profile_name,QObject *parent = 0);
    ~icqAccount();
    void createAccountButton(QHBoxLayout *);
//    void createMenuAccount(QMenu *, QAction *);
    void createMenuAccount();
    void removeMenuAccount(QMenu *);
    void removeAccountButton();
	inline QAction* getAction() const { return accountAction; }
	inline QString getIcquin() const { return icqUin; }
	inline bool getChangeState() const { return settingsChanged; };
	inline QIcon getCurrentIcon() const { return currentIcon; };
    inline accountStatus getStatus() const { return getProtocol()->getStatus(); };
    void setTrayCurrent( bool curr ) { currentTrayStatus = curr; };
	void createTrayMenuStatus(QMenu *);
    void removeTrayMenuStatus(QMenu *);
    void setChooseStatusCheck(bool check);
//	inline bool getAutoConnect() const { return autoConnect; };
    void autoconnecting();
    void removeContactList();
    bool deleteingAccount;
    void readMessageStack();
    inline oscarProtocol *getProtocol() const { return thisIcqProtocol;};
    QString currentIconPath;
    QMenu *getAccountMenu() {return statusMenu;}
	void saveAccountSettings();
	void setStatusFromPlugin(accountStatus status, const QString &status_text);
	void restoreStatusFromPlugin();
	void setXstatusFromPlugin(int status, const QString &status_title, QString &status_text);
	void restoreXstatusFromPlugin();
public slots:
	void onUpdateTranslation();
	void onReloadGeneralSettings();

private slots:

	void setStatus();

	void setStatusIcon(accountStatus);
	void onOscarStatusChanged(accountStatus status);

	void emitChangeStatus();
	void systemMessage(const QString &);
	void userMessage(const QString &,const QString &,const QString &, userMessageType, bool);


	void networkSettingsChanged();
	void addToEvent(bool f) { emit addToEventList(f); }
	void updateStatusMenu(bool);


	void setVisibleForAll();
	void setVisibleForVis();
	void setNotVisibleForInv();
	void setVisibleForContact();
	void setInvisibleForAll();
	void deleteTrayWindow(QObject *);
	void generalSettingsChanged();
	void customStatusTriggered();
	void accountConnected(bool f) { iAmConnected = f; };
signals:
	void changeSettingsApply();
	void statusChanged(const QIcon &);
	void changeStatusInTrayMenu(const QString &);
	void getNewMessage();
	void readAllMessages();
	void addToEventList(bool);
	void updateTrayToolTip();
	void updateTranslation();

private:
	accountStatus m_restore_status;
	QString m_restore_status_text;

	int m_restore_xstatus_num;
	QString m_restore_xstatus_title;
	QString m_restore_xstatus_text;

	void createIcons();
	void createStatusMenu();
	void updateIconStatus();
	void loadAccountSettings();

	void createContacts();
	QString getIconPathForUin(const QString &) const;

	QIcon currentIcon;
	QString icqUin;
	accountButton *accountLineButton;
	QAction *accountAction;
	bool settingsChanged;
	bool menuExist;
	bool currentTrayStatus;
	bool statusTrayMenuExist;
	bool autoConnect;
  
  QMenu *protoMenu;
	QMenu *statusMenu;
	QMenu *privacyStatus;

	QAction *onlineAction;
	QAction *offlineAction;
	QAction *ffcAction;
	QAction *awayAction;
	QAction *naAction;
	QAction *occupiedAction;
	QAction *dndAction;
	QAction *invisibleAction;
	QAction *lunchAction;
	QAction *evilAction;
	QAction *depressionAction;
	QAction *atHomeAction;
	QAction *atWorkAction;
	QVector<QAction *> statusMenuActions;

	QAction *visibleForAll;
	QAction *visibleForVis;
	QAction *notVisibleForInv;
	QAction *visibleForContact;
	QAction *invisibleForAll;

	QAction *customStatus;

	QActionGroup *privacyGroup;

	oscarProtocol *thisIcqProtocol;
	QAction *chooseStatus;
	bool showCustomStatus;

	bool showBalloon;
	int balloonTime;
	bool dontShowifNA;

	bool showTrayMessages;
	int trayMessageWidth;
	int trayMessageHeight;
	int trayMessageTime;
	TrayPosition trayMessagePosition;;
	int trayMessageStyle;
	int positionInStack;

	bool firsTrayMessageIsShown;

	QString configPath;

	unsigned clientIndex;
	unsigned protocolVersion;
	QString clientCap1;
	QString clientCap2;
	QString clientCap3;
	bool checkClientIdentification(unsigned, unsigned, const QString &,
			const QString &, const QString &);
	int currentXstatus;

	int statusIconIndex;
	bool iAmConnected;


	
	QString m_profile_name;
	QMenu *m_account_menu;
	IcqPluginSystem &m_icq_plugin_system;
	QMenu *m_account_additional_menu;
};

#endif // ICQACCOUNT_H
