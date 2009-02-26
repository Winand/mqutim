/*!
 * Plugin Interface
 *
 * @author m0rph
 * Copyright (c) 2008 by m0rph «m0rph.mailbox@gmail.com»
 * encoding: UTF-8
 *
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************
 */

#ifndef QUTIM_SDK_PLUGININTERFACE_H
#define QUTIM_SDK_PLUGININTERFACE_H

#include <QtPlugin>
#include <QIcon>
#include <QDateTime>
#include <QAction>
#include <QSettings>

namespace qutim_sdk_0_2 {

class PluginSystemInterface;
class PluginInterface;
class SimplePluginInterface;

enum LayerType
{
    AntiSpamLayer = 0,  // + -
    ChatLayer,          // - -
    ContactListLayer,   // - -
    HistoryLayer,       // + -
    NotificationLayer,  // + -
    SoundEngineLayer,   // + -
    VideoEngineLayer,   // + -
    StatusLayer,        // + -
    UnknownLayer,
    UnknownLayer1,
    UnknownLayer2,
    UnknownLayer3,
    UnknownLayer4,
    UnknownLayer5,
    UnknownLayer6,
    UnknownLayer7,
    UnknownLayer8,
    InvalidLayer
};

class LayerInterface
{
public:
    virtual ~LayerInterface() {};
    virtual bool init(PluginSystemInterface *plugin_system) = 0;
    virtual void setProfileName(const QString &profile_name) = 0;
    virtual void cleanup() = 0;
    virtual void setLayerInterface( LayerType type, LayerInterface *interface) = 0;
    virtual void loadSettings() = 0;
    virtual void saveSettings() = 0;
};
/*!
 * @brief item of the TreeModel, used to represent contact, message or somewhat
 */
struct TreeModelItem
{
  enum Type
  {
    Undefined = -1,
    Separator = Undefined,
    Buddy = 0,
    Group = 1,
    Account = 2,

    ConferenceItem = 3,
    Conference = 4,
  };
  
  enum IconRole
  {
    LeftIconGroupStart = 1,
    LeftIconGroupEnd = 2,
    
    RightIconGroupStart = 3,
    RightIconGroupEnd = 12,    
  };
  
	QString m_protocol_name; /*!< Protocol name */
	QString m_account_name; /*!< Account name */
	QString m_item_name; /*!< Item name */
	QString m_parent_name; /*!< Parent item name */
	Type m_item_type; /*!< Item type */
	QString m_item_history;
};
/*!
 * @brief item of the history
 *
 * In a few words - one separate message
 * HistoryItem structure is not fully understood by me, look at m_user and m_from
 */
struct HistoryItem
{
	QString m_message; /*!< Message */
	QDateTime m_time; /*!< Time of sending */
	TreeModelItem m_user;
	QString m_from;
	bool m_in;  /*!< Flag - incoming (TRUE) or outgoing (FALSE) message */
	qint8 m_type; /*!< 0 - system; 1 - private; */
};
/*!
 * @brief plugin event
 * @attention this structure is an unknown stuff, created by m0rph, noone can tell, what the hell it is
 */

enum EventType {
	ContactContextAction = 0,
	ItemAddedAction,
	ItemRemovedAction,
	ItemMovedAction,
	ItemChangedNameAction,
	ItemChangedStatusAction,
	AccountIsOnlineAction,
	SendingMessageBeforeShowing,
	SendingMessageAfterShowing,
	SengingMessageAfterShowingLastOutput,
	PointersAreInitialized,
	SoundAction,
	SystemNotification,
	UserNotification,
	ReceivingMessageFirstLevel,
	ReceivingMessageSecondLevel,
	ReceivingMessageThirdLevel,
	ReceivingMessageFourthLevel
};

enum NotificationType
{
    NotifyOnline = 0,
    NotifyOffline,
    NotifyStatusChange,
    NotifyBirthday,
    NotifyStartup,
    NotifyMessageGet,
    NotifyMessageSend,
    NotifySystem,
    NotifyTyping,
    NotifyBlockedMessage,
    NotifyCustom,
    EVENT_COUNT = NotifyTyping
};

enum MessageToProtocolEventType {
	SetStatus = 0,
	RestoreStatus
};


struct PluginEvent {
	EventType system_event_type;
	QList<void *> args;
};
/*!
 * @brief sound engine used for sounds
 */
enum SoundEngineSystem {
	NoSound = 0,
	LibPhonon,
	LibSound,
	UserCommand,
	PluginEngine
};
/*!
 * @brief event for sound engine
 */
const char* const XmlEventNames[] = { "c_online",
	"c_offline", "c_changed_status",
	"c_birth", "start",
	"m_get", "m_send", "sys_event", 0 };

/*!
 * Abstract interface of plugin system, visible to plug-ins
 * These functions can be called by plugin. Visible kernel functions.
 */
class PluginSystemInterface
{
public:
	virtual ~PluginSystemInterface() {}
	/*!
	 * @brief register handler for an event
	 *
	 * @attention this function is an unknown stuff, created by m0rph, noone can tell, what the hell it is
	 * @param event_id - ID of an event
	 * @param plugin - some other shit
	 * @see PluginInterface
	 */
	virtual void registerEventHandler(const EventType &type, SimplePluginInterface *plugin) = 0;
	/*!
	 * @brief release handler for an event
	 *
	 * @attention this function is an unknown stuff, created by m0rph, noone can tell, what the hell it is
	 * @param event_id - ID of an event
	 * @param plugin - some other shit
	 * @see PluginInterface
	 */
	virtual void releaseEventHandler(const QString &event_id, PluginInterface *plugin) = 0;
	/*!
	 * @brief process an event
	 *
	 * @attention this function is an unknown stuff, created by m0rph, noone can tell, what the hell it is
	 * @param event - event to be processed
	 * @see PluginEvent
	 */
	virtual void processEvent(PluginEvent &event) = 0;
	/*!
	 * @brief update status icons
	 *
	 * Called each time, when account status is changed
	 * It updates your status icon in system tray (may be also in tooltip, who knows)
	 */
	virtual void updateStatusIcons() = 0;
	/*!
	 * @brief add an item to contact-list
	 *
	 * Adds an contact to CL
	 *
	 * @param Item - item to be added
	 * @param name - name of the item
	 * @return boolean result of the attempt, TRUE if all is ok
	 * @see TreeModelItem
	 */
	virtual bool addItemToContactList(TreeModelItem Item, QString name=QString()) = 0;
	/*!
	 * @brief remove an item from contact-list
	 *
	 * Removes an contact from CL.
	 * Please, notice that we have no unique ID for an item
	 *
	 * @param Item - item to be deleted
	 * @return boolean result of the attempt, TRUE if all is ok
	 * @see TreeModelItem
	 */
	virtual bool removeItemFromContactList(TreeModelItem Item) = 0;
	/*!
	 * @brief move an item in contact-list
	 *
	 * Move item in the CL, e.g. change the group
	 * It's just an visualisation, server-side move should be done by the protocol!
	 *
	 * @param OldItem - item's old "status"
	 * @param OldItem - item's new "status"
	 * @return boolean result of the attempt, TRUE if all is ok
	 * @see TreeModelItem
	 */
	virtual bool moveItemInContactList(TreeModelItem OldItem, TreeModelItem NewItem) = 0;

	/*!
	 * @brief set the name for the item in contact-list
	 *
	 * Sets the name for the item (contact, group, somewhat) in the CL
	 *
	 * @param Item - item, to be changed
	 * @param name - new name for the item
	 * @return boolean result of the attempt, TRUE if all is ok
	 * @see TreeModelItem
	 */
	virtual bool setContactItemName(TreeModelItem Item, QString name) = 0;
	/*!
	 * @brief set an icon for the contact
	 *
	 * Sets an icon for the contact
	 * Item have two places left and ten places right for the icon
	 * Position 1 is reserved for the avatar
	 * Position 12 is reserved for the client icon
	 *
	 * @param Item - item in CL
	 * @param icon - icon to be added
	 * @param position - position in which icon should be placed, can be from 0 to 11
	 * @return boolean result of the attempt, TRUE if all is ok
	 * @see TreeModelItem
	 */
	virtual bool setContactItemIcon(TreeModelItem Item, QIcon icon, int position) = 0;
	/*!
	 * @brief set an row for contact-list item
	 *
	 * Each item has 3 additional rows below, which can be set by this function
	 *
	 * @param Item - item in CL
	 * @param row - row to be set
	 * @param position - position in which row should be placed, can be from 0 to 3
	 * @return boolean result of the attempt, TRUE if all is ok
	 * @see TreeModelItem
	 */
	virtual bool setContactItemRow(TreeModelItem Item, QList<QVariant> row, int position) = 0;
	/*!
	 * @brief set contact-list item status
	 *
	 * Sets the status of the item.
	 *
	 * @param Item - item in CL
	 * @param icon - icon for the status
	 * @param text - text for the status
	 * @param mass - "weight" of the item, used for item sort, can be from 1 to 1000 (offline)
	 * @return boolean result of the attempt, TRUE if all is ok
	 * @see TreeModelItem
	 */
	virtual bool setContactItemStatus(TreeModelItem Item, QIcon icon, QString text, int mass) = 0;
	/*!
	 * @brief set status message
	 *
	 * @param status_message - status message
	 * @param dshow - if TRUE, don't show this dialog again
	 * @return boolean result of the attempt, TRUE if all is ok
	 */
	virtual bool setStatusMessage(QString &status_message, bool &dshow) = 0;
	/*!
	 * @brief transfer message from some contact
	 *
	 * Create message, received from the contact
	 *
	 * @param item - contact, which sends a message
	 * @param message - message sent
	 * @param message_date - message sending datetime
	 * @see TreeModelItem
	 */
	virtual void addMessageFromContact(const TreeModelItem &item, const QString &message, const QDateTime &message_date) = 0;
	/*!
	 * @brief transfer service message
	 *
	 * Create service message
	 *
	 * @param item - contact, related to the message
	 * @param message - message
	 * @see TreeModelItem
	 */
	virtual void addServiceMessage(const TreeModelItem &item, const QString &message) = 0;
	/*!
	 * @brief send raw image
	 *
	 * Send raw-format image
	 *
	 * @param item - contact who sent an image
	 * @param image_raw - image in raw format
	 * @see TreeModelItem
	 */
	virtual void addImage(const TreeModelItem &item, const QByteArray &image_raw) = 0;
	/*!
	 * @brief create typing notify
	 *
	 * Create a notification, that some contact is typing
	 *
	 * @param item - contact, which is typing
	 * @param typing - typing status, true or false
	 * @see TreeModelItem
	 */
	virtual void contactTyping(const TreeModelItem &item, bool typing) = 0;
	/*!
	 * @brief set the flag that message is delivered
	 *
	 * Marks (by checkmark usually) message sent to contact as delivered
	 *
	 * @param item - contact, whom message is sent to
	 * @param message_position - position of the message, got from kernel
	 * @see TreeModelItem
	 */
	virtual void messageDelievered(const TreeModelItem &item, int message_position) = 0;
	/*!
	 * @brief check the message through the anti-spam
	 *
	 * @param item - contact who sent the message
	 * @param message - the message to be checked
	 * @param message_type - type of the message, 0 - if simple message, 1 - if authorisation request
	 * @param special_status - invisible for all or any other status, which require silence and non-responding
	 * @return boolean result of the check, TRUE if all is ok
	 * @see TreeModelItem
	 */
	virtual bool checkForMessageValidation(const TreeModelItem &item, const QString &message,
			int message_type, bool special_status) = 0;
	/*!
	 * @brief notify about contact birthday
	 *
	 * Create a notification about contact birthday
	 *
	 * @param item - birthday person :)
	 * @see TreeModelItem
	 */
	virtual void notifyAboutBirthDay(const TreeModelItem &item) = 0;
	/*!
	 * @brief system notification
	 *
	 * @param item - notification info
	 * @param message - notification message
	 * @see TreeModelItem
	 */
	virtual void systemNotifiacation(const TreeModelItem &item, const QString &message) = 0;
	/*!
	 * @brief custom notification
	 *
	 * @param item - notification info
	 * @param message - notification message
	 * @see TreeModelItem
	 */
	virtual void customNotifiacation(const TreeModelItem &item, const QString &message) = 0;
	/*!
	 * @brief get filename of the icon
	 *
	 * @param icon_name - name of the icon
	 * @return QString - filename of the icon
	 */
	virtual QString getIconFileName(const QString & icon_name) = 0;
	/*!
	 * @brief get icon by its name
	 *
	 * @param icon_name - name of the icon
	 * @return QIcon - the icon requested
	 */
	virtual QIcon getIcon(const QString & icon_name) = 0;
	/*!
	 * @brief get filename of the status icon
	 *
	 * @param icon_name - name of the icon
	 * @param default_path - path, where icon should be found
	 * @return QString - filename of the status icon
	 */
	virtual QString getStatusIconFileName(const QString & icon_name, const QString & default_path) = 0;
	/*!
	 * @brief get status icon
	 *
	 * @param icon_name - name of the icon
	 * @param default_path - path where icon should be found
	 * @return QIcon - the icon requested
	 */
	virtual QIcon getStatusIcon(const QString & icon_name, const QString & default_path) = 0;
	/*!
	 * @brief set contant online status
	 *
	 * @param item - affected account
	 * @param online - contact status, TRUE if online
	 * @see TreeModelItem
	 */
	virtual void setAccountIsOnline(const TreeModelItem &Item, bool online) = 0;
	/*!
	 * @brief create chat with the target contact
	 *
	 * @param item - contact to create chat with
	 * @see TreeModelItem
	 */
	virtual void createChat(const TreeModelItem &item) = 0;
	/*!
	 * @brief get qutIM version
	 *
	 * @param major - major version
	 * @param minot - minor version
	 * @param secminor - lesser minor version
	 * @param svn - svn release
	 */
	virtual void getQutimVersion(quint8 &major, quint8 &minor, quint8 &secminor, quint16 &svn) = 0;
	/*!
	 * @brief create a conference
	 *
	 * @param protocol_name - protocol to be used
	 * @param conference_name - name of the conference
	 * @param account_name - account to be used
	 */
	virtual void createConference(const QString &protocol_name,
			const QString &conference_name,
			const QString &account_name) = 0;
	/*!
	 * @brief add message to the target conference
	 *
	 * @param protocol_name - name of the protocol
	 * @param conference_name - name of the conference
	 * @param account_name - name of the account
	 * @param from - message sender
	 * @param message - message sent
	 * @param date - date of the message
	 * @param history - has the message been taken from history or not
	 */
	virtual void addMessageToConference(const QString &protocol_name,
			const QString &conference_name, const QString &account_name,
			const QString &from,
			const QString &message, const QDateTime &date, bool history) = 0;
	/*!
	 * @brief change own nickname in the conference
	 *
	 * @param protocol_name - name of the protocol
	 * @param conference_name - name of the conference
	 * @param account_name - name of the account
	 * @param nickname - nickname to be set
	 */
	virtual void changeOwnConferenceNickName(const QString &protocol_name,
			const QString &conference_name,
			const QString &account_name,
			const QString &nickname) = 0;
	virtual void setConferenceTopic(const QString &protocol_name,
			const QString &conference_name,
			const QString &account_name,
			const QString &topic) = 0;
	virtual void addSystemMessageToConference(const QString &protocol_name,
			const QString &conference_name, const QString &account_name,
			const QString &message, const QDateTime &date, bool history) = 0;
	virtual void addConferenceItem(const QString &protocol_name,
			const QString &conference_name,
			const QString &account_name,
			const QString &nickname) = 0;
	virtual void removeConferenceItem(const QString &protocol_name,
			const QString &conference_name,
			const QString &account_name,
			const QString &nickname) = 0;
	virtual void renameConferenceItem(const QString &protocol_name,
			const QString &conference_name,
			const QString &account_name,
			const QString &nickname,
			const QString &new_nickname) = 0;
	virtual void setConferenceItemStatus(const QString &protocol_name,
			const QString &conference_name,
			const QString &account_name,
			const QString &nickname,
			const QIcon &icon, const QString &status, int mass) = 0;
	virtual void setConferenceItemIcon(const QString &protocol_name,
			const QString &conference_name,
			const QString &account_name,
			const QString &nickname,
			const QIcon &icon, int position) = 0;
	virtual void setConferenceItemRole(const QString &protocol_name,
			const QString &conference_name,
			const QString &account_name,
			const QString &nickname,
			const QIcon &icon, const QString &role, int mass) = 0;
	virtual QStringList getConferenceItemsList(const QString &protocol_name,
			const QString &conference_name, const QString &account_name) = 0;
	virtual void setItemVisible(const TreeModelItem &item, bool visible) = 0;
	virtual void setItemInvisible(const TreeModelItem &item, bool invisible) = 0;
	virtual void registerContactMenuAction(QAction *plugin_action, SimplePluginInterface *pointer_this) = 0;
	virtual void sendCustomMessage(const TreeModelItem &item, const QString &message, bool silent = false) = 0;
	virtual void registerMainMenuAction(QAction *menu_action) = 0;
	virtual void redirectEventToProtocol(const QStringList &protocol_name, const QList<void*> &event) = 0;
        virtual void playSound(NotificationType event) = 0;
	virtual void playSound(const QString &file_name) = 0;
	virtual QSettings::Format plistFormat() = 0;
        virtual bool changeChatWindowID(const TreeModelItem &item, const QString &id) = 0;
        virtual QStringList getAdditionalInfoAboutContact(const TreeModelItem &item) const = 0;
        virtual bool setLayerInterface( LayerType type, LayerInterface *interface) = 0;
};


/*!
 * Abstract plug-in interface, all plug-ins must be inherited from this
 * All the following functions must be realized in plug-in, mention it.
 */
class PluginInterface
{
public:
	virtual ~PluginInterface() {}

	/*!
	 * @brief initialization of the plug-in
	 *
	 * @param plugin_system - plugin system realization
	 * @return boolean result of initialization, TRUE if all is ok
	 * @see PluginSystemInterface
	 */
	virtual bool init(PluginSystemInterface *plugin_system)
	{
		m_plugin_system = plugin_system;
		return true;
	}

	/*!
	 * @brief release resources
	 */
	virtual void release() = 0;

	/*!
	 * @brief the name of the plugin
	 *
	 * @return QString - plugin name
	 */
	virtual QString name() = 0;

	/*!
	 * @brief the description of the plugin
	 *
	 * @return QString - plugin description
	 */
	virtual QString description() = 0;

	/*!
	 * @brief plugin type
	 *
	 * For protocols it should be "protocol", in other cases... something else?
	 *
	 * @return QString - plugin type
	 */
	virtual QString type() = 0;

	/*!
	 * @brief plugin icon
	 *
	 * @return QIcon - plugin icon
	 */
	virtual QIcon *icon() = 0;

	/*!
	 * @brief set profile name
	 *
	 * @param profile_name - name to be set
	 */
	virtual void setProfileName(const QString &profile_name) = 0;

protected:
	PluginSystemInterface *m_plugin_system; /*!< plugin system instance */
};

class SimplePluginInterface : public PluginInterface
{
public:
	/*!
	 * @brief process event
	 *
	 * Processes some event, sent to the plugin
	 *
	 * @attention this function is an unknown stuff, created by m0rph, noone can tell, what the hell it is
	 * @param event - event, sent to the plugin
	 * @see PluginEvent
	 */
	virtual void processEvent(PluginEvent &event) = 0;

	/*!
	 * @brief plugin settings widget
	 *
	 * @return QWidget, representing plugin settings window
	 */
	virtual QWidget *settingsWidget() = 0;

	virtual void removeSettingsWidget() = 0;
	virtual void saveSettings() = 0;
};

}//end namespace qutim_sdk_0_2

Q_DECLARE_INTERFACE(qutim_sdk_0_2::PluginInterface, "org.qutim.plugininterface/0.2");


#endif //#ifndef QUTIM_SDK_PLUGININTERFACE_H
