#ifndef PLUGINSYSTEM_H_
#define PLUGINSYSTEM_H_

#include "icqlayer.h"
//struct TreeModelItem
//{
//	QString m_protocol_name;
//	QString m_account_name;
//	QString m_item_name;
//	QString m_parent_name;
//	quint8 m_item_type; // 0 - buddy; 1 - group; 2 - account item;
//};


class IcqPluginSystem
{
public:
	IcqPluginSystem();
	~IcqPluginSystem();
	static IcqPluginSystem &instance();
	void setIcqLayer(IcqLayer *icq_layer) { m_parent_layer = icq_layer; }
	void updateStatusIcons();
	//Function for adding item to contact list
	bool addItemToContactList(TreeModelItem Item, QString name=QString());
	//Function for removing item from contact list
	bool removeItemFromContactList(TreeModelItem Item); 
	//Function for moving item in contact list
	bool moveItemInContactList(TreeModelItem OldItem, TreeModelItem NewItem);
	//Function for renaming contact
	bool setContactItemName(TreeModelItem Item, QString name);
	//Function for adding icon to contact (1,2 - left side; 3...12 - right side)
	bool setContactItemIcon(TreeModelItem Item, QIcon icon, int position); 
	//Function for adding text row below contact ( 1..3)
	bool setContactItemRow(TreeModelItem Item, QList<QVariant> row, int position);
	//Function for settings contact status, text - status name for adium icons packs,
	//mass status mass, 1000 - offline, 999-offline separator, -1 - online separator
	bool setContactItemStatus(TreeModelItem Item, QIcon icon, QString text, int mass);
	
	bool setStatusMessage(QString &status_message, bool &dshow);
	
	void addMessageFromContact(const TreeModelItem &item, const QString &message,
			const QDateTime &message_date);
	void addServiceMessage(const TreeModelItem &item, const QString &message);
	void addImage(const TreeModelItem &item, const QByteArray &image_raw);
	void contactTyping(const TreeModelItem &item, bool typing);
	void messageDelievered(const TreeModelItem &item, int position);
	bool checkForMessageValidation(const TreeModelItem &item, const QString &message,
			int message_type, bool special_status);
	QString getIconFileName(const QString & icon_name);
	QIcon getIcon(const QString & icon_name);
	QString getStatusIconFileName(const QString & icon_name, const QString & default_path);
	QIcon getStatusIcon(const QString & icon_name, const QString & default_path);
	void setAccountIsOnline(const TreeModelItem &Item, bool online);
	void createChat(const TreeModelItem &item);
	void notifyAboutBirthDay(const TreeModelItem &item);
	void systemNotifiacation(const TreeModelItem &item, const QString &message);
	void customNotifiacation(const TreeModelItem &item, const QString &message);
	void getQutimVersion(quint8 &major, quint8 &minor, quint8 &secminor, quint16 &svn);

private:
	IcqLayer *m_parent_layer;
};

#endif /*PLUGINSYSTEM_H_*/
