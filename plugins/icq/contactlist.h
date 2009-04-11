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


#ifndef CONTACTLIST_H_
#define CONTACTLIST_H_

#include "statusiconsclass.h"
#include "clientidentify.h"
#include "modifyitem.h"

#include "quticqglobals.h"
#include "icqpluginsystem.h"
#include "treegroupitem.h"

// Qt standard classes
class QTcpSocket;
class QTreeWidget;
class QTreeWidgetItem;
class QTextCodec;
class QLabel;
class QWidgetAction;
class QAction;
class QMenu;
class QDateTime;

// qutIM own classes
class icqBuffer;
class treeBuddyItem;
//class treeGroupItem;
class messageFormat;
class chatWindow;
class contactSeparator;
/*class FileTransfer;*/
class metaInformation;
class buddyPicture;
class searchUser;
class userInformation;
class privacyListWindow;
class readAwayDialog;
class noteWidget;

struct listFont
{
	QString fontFamily;
	int fontSize;
	QColor fontColor;
};

struct itemFromList
{
	quint16 groupId;
	quint16 itemId;
};

class contactListTree : public QObject
{
	Q_OBJECT
public:
	contactListTree(QTcpSocket *, icqBuffer *, const QString &,
			const QString &profile_name, quint16 &, quint32 &,quint16 &, QObject *parent = 0);
	~contactListTree();
	void removeContactList();
	void readMessageStack();
	void doubleClickedBuddy(treeBuddyItem *);
	void showItemContextMenu(const QList<QAction*> &action_list, 
			const QString &item_name, int item_type, const QPoint &point_menu);
	void readMessageFrom(treeBuddyItem *);
    inline QHash<QString, treeBuddyItem*> getBuddyList() const { return buddyList; }
	QHash<quint16, treeGroupItem*> groupList;
	void updateSorting();
	void goingOnline(bool);
	statusIconClass *statusIconObject;
	void changePrivacy(quint8);
//      accountStatus currentStatus;
	void appExiting();
	QString accountNickname;
	void initializaMenus(QMenu *);
	void showGroupMenu(treeGroupItem *, const QPoint &);
	void showBuddyMenu(const QList<QAction*> &action_list, treeBuddyItem *, const QPoint &);
	void hideRoot(bool);
	void offlineHideButtonClicked(bool);
	QByteArray convertPassToCodePage(const QString &);
	void itemActivated(const QString &uin);
//	pluginSystem *plugins;
	void sendMessageTo(const QString &contact_uin, const QString &message, int message_icon_position);
	QStringList getAdditionalInfoAboutContact(const QString &item_name, int item_type ) const;

/*	void sendImageTo(const QString &contact_uin, const QByteArray &image_raw);*/
/*        void sendFileTo(const QString &contact_uin, const QStringList &file_names);*/
	void sendTypingNotifications(const QString &, quint16);
/*	FileTransfer *fileTransferObject;*/
	void moveItemSignalFromCL(const TreeModelItem &old_item, const TreeModelItem &new_item);
	void deleteItemSignalFromCL(const QString &item_name, int item_type);
	const QString getItemToolTip(const QString &contact_name);
	void chatWindowOpened(const QString &contact_uin, bool new_wind = false);
signals:
	void userMessage(const QString &, const QString &, const QString&, userMessageType, bool);
	void getNewMessage();
	void readAllMessages();
	void incFlapSeq();
	void incSnacSeq();
	void incMetaSeq();
	void sendGroupList(QHash<quint16, treeGroupItem>);
	void buddyChangeStatus(treeBuddyItem *,bool);
	void updateOnlineList();
	void reupdateList();
	void updateStatus();
	void restartAutoAway(bool, quint32);
	void updateStatusMenu(bool);
	void sendSystemMessage(const QString &);
	void soundSettingsChanged();
	void playSoundEvent(const SoundEvent::Events &, const accountStatus &);

public slots:
	void onUpdateTranslation();
	void onReloadGeneralSettings();
	void onStatusChanged(accountStatus status);
	void contactSettingsChanged();
	void onSystemMessage()
	{
		emit playSoundEvent(SoundEvent::SystemEvent, currentStatus);
	};
	void openInfoWindow(const QString &, const QString &n = 0, const QString & f = 0, const QString & l = 0);
	void clearPrivacyLists();
private slots:
	void createContact(bool);
	void oncomingBuddy(const QString &, quint16);
	void offlineBuddy(const QString &, quint16);
	void getMessage(quint16);
	void getOfflineMessage();
	void setMessageIconToContact();
	void deleteChatWindow(QObject *);
	void deleteHistoryWindow(QObject *);
	void sendMessage(const messageFormat &);

	void msgSettingsChanged();
	void statusSettingsChanged();


	void activateWindow(const QString &);
	void readMetaData(quint16, bool notalone);

	void getTypingNotification(quint16);
	void clearNotifList() { notifList.clear(); }
	void showHistory(const QString &);
	void showServiceHistory();
	void disableJustStarted() { justStarted = false;  }
	void startChatWith(const QString &);
	void askForAvatars(const QByteArray &, const QString &);
	void readSSTserver(quint16);
	void emptyAvatarList();
	void updateAvatar(const QString &, QByteArray);
	void findAddUser();
	void findUserWindowClosed(QObject *);
	void searchForUsers(int);
	void openChatWindowWithFounded(const QString &,const QString &);

	void infoUserWindowClosed(QObject *);
	void askForFullUserInfo(const QString &);
	void checkStatusFor(const QString &);
	void getStatusCheck(quint16);
	void addUserToList(const QString &, const QString &,bool);
	void getModifyItemFromServer(quint16);
	void youWereAdded(quint16);
	void openPrivacyWindow();
	void deletePrivacyWindow(QObject *);
	void deleteFromPrivacyList(const QString &, int);
	void openSelfInfo();
        void saveOwnerInfo(/*bool, const QString &*/);
	void getUploadIconData(quint16);
	void openChangePasswordDialog();
	void createNewGroup();
	void renameSelectedGroup();
	void deleteSelectedGroup();


	void sendMessageActionTriggered();
	void userInformationActionTriggered();
	void statusCheckActionTriggered();
	void messageHistoryActionTriggered();
	void readAwayActionTriggered();
	void deleteAwayWindow(QObject *);

	void getAwayMessage(quint16);


	void renameContactActionTriggered();
	void deleteContactActionTriggered();
	void moveContactActionTriggered();

	void addToVisibleActionTriggered();
	void addToInvisibleActionTriggered();
	void addToIgnoreActionTriggered();

	void deleteFromVisibleActionTriggered();
	void deleteFromInvisibleActionTriggered();
	void deleteFromIgnoreActionTriggered();

	void requestAuthorizationActionTriggered();

	void copyUINActionTriggered();

	void getAuthorizationRequest(quint16);
	void authorizationAcceptedAnswer(quint16);

	void addToContactListActionTriggered();
	void allowToAddMeTriggered();
	void removeMyselfTriggered();

	void sendFile(QByteArray &, QByteArray &, QByteArray &);
	void sendFileActionTriggered();
	void sendCancelSending(QByteArray &);

	void redirectToProxy(const QByteArray &);

	void sendAcceptMessage(const QByteArray &);

	void sendImage(const QString &,const QString &);

	void readXstatusTriggered();

	void sendAuthReqAnswer(bool, const QString &);

	void askForXstatusTimerTick();

	void sendFileFromWindow(const QString &);
	void editNoteActionTriggered();
	void deleteNoteWindow(QObject *);
	void getChangeFontSignal(const QFont &, const QColor &, const QColor &);
	void getMessageAck(quint16);
private:
        QString getCurrentAwayMessage() const;
	void initializeWindow(chatWindow *);
	void createContactList();
        void createSoundEvents();
	quint16 byteArrayToInt16(const QByteArray &);
	quint16 byteArrayToLEInt16(const QByteArray &);
	quint32 byteArrayToInt32(const QByteArray &);
	QString convertToStringStatus(contactStatus);
	void createNil();
	void loadSettings();
	QByteArray convertToByteArray(const quint16 &);
//	QHash<quint16, treeGroupItem *> groupList;
	QHash<QString, treeBuddyItem*> buddyList;
	QHash<QString, treeBuddyItem*> messageList;
	QHash<QString, chatWindow *> chatWindowList;
	QTcpSocket *tcpSocket;
	icqBuffer *socket;
	QString icqUin;
//	QTreeWidgetItem *rootItem;
	bool newMessages;
	quint16 *flapSeq;
	quint32 *snacSeq;
	quint16 *metaSeq;
	QTextCodec *codec;

	bool isMergeAccounts;
	QStringList getGroups;
	QStringList getBuddies;
	bool contactListChanged;
	bool iAmOnline;
	bool showGroups;
	contactSeparator *onlineList;
	contactSeparator *offlineList;
	void createOnOffGroups();
	void removeGroups();
	void updateNil();
	void prepareForMerge();
	void showHideGroups();
	void hideEmptyGroups(bool);
	void prepareForShowGroups();
	bool hideEmpty;
	bool expandRoot;
	bool showOffline;
	void showOfflineUsers();
	bool clearNil;
	void clearNilUsers();
	bool dontUnderlineNotAutho;
	void updateBuddyListFlags();
	bool hideBirth;
	bool customAccount;
	bool customGroup;
	bool customOnline;
	bool customOffline;
	listFont grpFont;
	listFont onlFont;
	listFont offFont;
	void updateGroupCustomFont();
	void updateContactsCustomFont();
	bool tabMode;
	void updateChatBuddyStatus(const QString &, const QIcon &);
	bool showNames;
	quint8 timestamp;
	bool sendOnEnter;
	bool closeOnSend;
	bool dontShowEvents;
	bool openNew;
	QString codepage;
	void requestUinInformation(const QString &);
	QHash<quint16, QString> metaInfoRequestList;
	void readShortInfo(const metaInformation &, quint16);
	bool sendTyping;
	QStringList notifList;
	bool webAware;
	bool autoAway;
	quint32 awayMin;
	void initializeBuddy(treeBuddyItem *);
	quint16 pdInfoID;
	quint16 pdInfoGroupId;
	bool waitForMineInfo;
	int mineMetaSeq;

	bool saveHistory;
	bool saveNilHistory;
	bool saveServiceHistory;
	bool showRecent;
	bool onlineNotify;
	bool offlineNotify;
	bool readAwayNotify;
	quint8 recentCount;

	void loadUnreadedMessages();
	void setServiceMessageToWin(const QString&, const QString &);

	bool hideSeparators;
	void setHideSeparators(bool);

	bool disableTrayBlinking;
	bool disableButtonBlinking;

	bool signOnNot;
	bool signOffNot;
	bool typingNot;
	bool changeStatusNot;
	bool awayNot;
	bool justStarted;

	QHash<QString, QByteArray> avatartList;

	bool checkBuddyPictureHash(const QByteArray &);

	buddyPicture *buddyConnection;
	QString avatarAddress;
	quint16 avatarPort;
	void sendReqForRedirect();
	QByteArray avatarCookie;
	bool getOnlyFromContactList;
	bool notifyAboutBlocked;
	void notifyBlockedMessage(const QString &, const QString &);
	void saveBlocked(const QString&, const QString&, const QDateTime &);
	bool blockAuth;
	bool checkMessageForUrl(const QString &);
	bool blockUrlMessage;
	bool enableAntiSpamBot;
	bool turnOnAntiSpamBot(const QString &, const QString &, const QDateTime &);
	bool dontAnswerBotIfInvis;
	QString question;
	QString answer;
	QString messageAfterAnswer;
	QStringList blockedBotList;

	QAction *serviceMessages;
	QAction *findUser;
	QAction *privacyList;
	QAction *selfInfo;
	QAction *changePassword;
	searchUser *searchWin;
	bool findUserWindowOpen;
	void addSearchResult(bool, bool, const QString &, const QString &,
			const QString &, const QString &, const QString &,
			const quint8 &, const quint16 &, const quint8&, const quint16 &);

	QHash<QString, userInformation*> infoWindowList;


	QHash<quint16, QString> fullInfoRequests;
	void readBasicUserInfo(const metaInformation &, quint16);
	void readMoreUserInfo(const metaInformation &, quint16);
	void readWorkUserInfo(const metaInformation &, quint16);
	void readInterestsUserInfo(const metaInformation &, quint16);
	void readAboutUserInfo(const metaInformation &, quint16);
	void fullIndoEnd(quint16,bool);
	quint8 convertToInt8(const QByteArray &);


	QList<quint16> idBuddyList;
	void sendUserAddReq(const QString &, QString , bool, const QString &);
	void addModifiedBuddyToGroup(quint16, quint16, const QString &, bool, const QString &);
	QList<modifyObject> modifyReqList;
	QString iconPath;

	bool privacyListWindowOpen;
	privacyListWindow *privacyWindow;
	QStringList visibleList;
	QStringList invisibleList;
	QStringList ignoreList;
	QHash<QString, itemFromList> ignoreObjectList;
	QHash<QString, itemFromList> visibleObjectList;
	QHash<QString, itemFromList> invisibleObjectList;
	void checkForOwnIcon(QByteArray);

	bool waitForIconUpload;
	QString ownIconPath;
	void uploadIcon();
	itemFromList iconObject;
	void removeIconHash();

	void createContactListActions();
	QLabel *menuLabel;
	QWidgetAction *menuTitle;
	QAction *createGroup;
	QAction *renameGroup;
	QAction *deleteGroup;
	QAction *sendMessageAction;
	QAction *userInformationAction;
	QAction *editNoteAction;
	QAction *statusCheckAction;
	QAction *messageHistoryAction;
	QAction *readAwayAction;

	treeGroupItem *currentContextGroup;
	QMenu *currentContextMenu;
	void addNewGroupToRoot(const QString &, quint16);
	void renameGroupToName(const QString &, quint16);
	void deleteSelectedGroup(quint16);
	treeBuddyItem *currentContextBuddy;
	QHash<QByteArray, readAwayDialog *> awayMessageList;

	QAction *renameContactAction;
	QAction *deleteContactAction;
	QAction *moveContactAction;

	void renameContact(const QString &, const QString &);
	void removeContact(const QString &);
	bool movingBuddy;

	QAction *addToVisibleAction;
	QAction *addToInvisibleAction;
	QAction *addToIgnoreAction;
	QAction *deleteFromVisibleAction;
	QAction *deleteFromInvisibleAction;
	QAction *deleteFromIgnoreAction;

	QAction *requestAuthorizationAction;

	void openAuthReqFromBuddy(treeBuddyItem *);

	QAction *addToContactListAction;

	QAction *allowToAddMe;

	QAction *removeMyself;

	QAction *copyUINAction;

	clientIdentify identifyContactClient;
	bool avatarModified;


	QString emoticonXMLPath;

	QAction *readXstatus;

	bool showXStatuses;

	bool showXstatusesinToolTips;

    QString addXstatusMessage(const QString &,QByteArray &);
    QString findTitle(QString );
    QString findMessage(QString );
    QString xTraAway(QString);
    QList<treeBuddyItem *> xStatusTickList;
    bool letMeAskYouAboutXstatusPlease;

    bool lightChatView;
    bool dontShowIncomingMessagesInTrayEvents;

    QHash<QString, noteWidget*> noteWidgetsList;

    quint32 allGroupCount;
    quint32 allContactCount;
    quint32 allContactTmpCount;
        accountStatus currentStatus;

    QHash<QByteArray, quint64> messageCursorPositions;
    
    QString m_profile_name;
    QString account_name;
    IcqPluginSystem &m_icq_plugin_system;
    void addGroupToCL(quint16 id, const QString &);
    void renameGroupInCL(const QString&,  quint16 new_id);
    void addContactToCL(quint16 group_id, const QString &contact_uin
    		, const QString &contact_name);
    void renameContactInCL(quint16 group_id, const QString &, const QString &);
    void moveContactFromGroupToGroup(quint16 old_group_id, 
    		quint16 new_roup_id, const QString &contact_uin);
    void removeGroupFromCl(quint16 group_id);
    void removeContactFromCl(quint16 group_id, const QString&contact_uin);
    void setPrivacyIconsToContacts();
    void addMessageFromContact(const QString &contact_uin, quint16 group_id, const QString &message
    		, const QDateTime &message_date = QDateTime::currentDateTime());
    void addServiceMessage(const QString &contact_uin, quint16 group_id, const QString &message);
	void addImage(const QString &contact_uin, quint16 group_id, const QByteArray &image_raw);
	void contactTyping(const QString &contact_uin, quint16 group_id, bool typing);
	void messageDelievered(const QString &contact_uin, quint16 group_id, int position);
	bool checkMessageForValidation(const QString &contact_uin, const QString &message,
			int message_type);
	bool m_notify_about_reading_status;
	bool m_show_xstatus_icon;
	bool m_show_birthday_icon;
	bool m_show_auth_icon;
	bool m_show_vis_icon;
	bool m_show_invis_icon;
	bool m_show_ignore_icon;
	bool m_show_xstatus_text;
	void notifyAboutBirthday(const QString &uin, quint16 group_id);
	void createChat(const QString &uin, quint16 groupd_id);
	void sendMessageRecieved(const QString &uin, const QByteArray &msg_cookie);
};

#endif /*CONTACTLIST_H_*/
