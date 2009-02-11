#include "icqpluginsystem.h"


IcqPluginSystem::IcqPluginSystem()
{
	
}

IcqPluginSystem::~IcqPluginSystem()
{

}

IcqPluginSystem &IcqPluginSystem::instance()
{
	static IcqPluginSystem ips;
	return ips;
}

void IcqPluginSystem::updateStatusIcons()
{
	m_parent_layer->getMainPluginSystemPointer()->updateStatusIcons();
}

bool IcqPluginSystem::addItemToContactList(TreeModelItem Item, QString name)
{
	return m_parent_layer->getMainPluginSystemPointer()->addItemToContactList(Item, name);
}

bool IcqPluginSystem::removeItemFromContactList(TreeModelItem Item)
{
	return m_parent_layer->getMainPluginSystemPointer()->removeItemFromContactList(Item);
}

bool IcqPluginSystem::moveItemInContactList(TreeModelItem OldItem, TreeModelItem NewItem)
{
	return m_parent_layer->getMainPluginSystemPointer()->moveItemInContactList(OldItem, NewItem);
}

bool IcqPluginSystem::setContactItemName(TreeModelItem Item, QString name)
{
	return m_parent_layer->getMainPluginSystemPointer()->setContactItemName(Item, name);
}

bool IcqPluginSystem::setContactItemIcon(TreeModelItem Item, QIcon icon, int position)
{
	return m_parent_layer->getMainPluginSystemPointer()->setContactItemIcon(Item, icon, position);
}

bool IcqPluginSystem::setContactItemRow(TreeModelItem Item, QList<QVariant> row, int position)
{
	return m_parent_layer->getMainPluginSystemPointer()->setContactItemRow(Item, row, position);
}

bool IcqPluginSystem::setContactItemStatus(TreeModelItem Item, QIcon icon, QString text, int mass)
{
	return m_parent_layer->getMainPluginSystemPointer()->setContactItemStatus(Item, icon, text, mass);
}

bool IcqPluginSystem::setStatusMessage(QString &status_message, bool &dshow)
{
	return m_parent_layer->getMainPluginSystemPointer()->setStatusMessage(status_message, dshow);
}

void IcqPluginSystem::addMessageFromContact(const TreeModelItem &item, const QString &message,
		const QDateTime &message_date)
{
	m_parent_layer->getMainPluginSystemPointer()->addMessageFromContact(item, message, message_date);
}

void IcqPluginSystem::addServiceMessage(const TreeModelItem &item, const QString &message)
{
	m_parent_layer->getMainPluginSystemPointer()->addServiceMessage(item, message);
}

void IcqPluginSystem::addImage(const TreeModelItem &item, const QByteArray &image_raw)
{
	m_parent_layer->getMainPluginSystemPointer()->addImage(item,image_raw);
}

void IcqPluginSystem::contactTyping(const TreeModelItem &item, bool typing)
{
	m_parent_layer->getMainPluginSystemPointer()->contactTyping(item, typing);
}

void IcqPluginSystem::messageDelievered(const TreeModelItem &item, int position)
{
	m_parent_layer->getMainPluginSystemPointer()->messageDelievered(item, position);
}

bool IcqPluginSystem::checkForMessageValidation(const TreeModelItem &item, const QString &message,
		int message_type, bool special_status)
{
	return m_parent_layer->getMainPluginSystemPointer()->checkForMessageValidation(item,
			message, message_type, special_status);
}
QString IcqPluginSystem::getIconFileName(const QString & icon_name)
{
	return m_parent_layer->getMainPluginSystemPointer()->getIconFileName(icon_name);
}
QIcon IcqPluginSystem::getIcon(const QString & icon_name)
{
	return m_parent_layer->getMainPluginSystemPointer()->getIcon(icon_name);	
}
QString IcqPluginSystem::getStatusIconFileName(const QString & icon_name, const QString & default_path)
{
	return m_parent_layer->getMainPluginSystemPointer()->getStatusIconFileName(icon_name, default_path);	
}
QIcon IcqPluginSystem::getStatusIcon(const QString & icon_name, const QString & default_path)
{
	return m_parent_layer->getMainPluginSystemPointer()->getStatusIcon(icon_name, default_path);	
}
void IcqPluginSystem::setAccountIsOnline(const TreeModelItem &item, bool online)
{
	m_parent_layer->getMainPluginSystemPointer()->setAccountIsOnline(item, online);
}

void IcqPluginSystem::createChat(const TreeModelItem &item)
{
	m_parent_layer->getMainPluginSystemPointer()->createChat(item);
}

void IcqPluginSystem::notifyAboutBirthDay(const TreeModelItem &item)
{
	m_parent_layer->getMainPluginSystemPointer()->notifyAboutBirthDay(item);
}

void IcqPluginSystem::systemNotifiacation(const TreeModelItem &item, const QString &message)
{
	m_parent_layer->getMainPluginSystemPointer()->systemNotifiacation(item ,message);
}

void IcqPluginSystem::customNotifiacation(const TreeModelItem &item, const QString &message)
{
	m_parent_layer->getMainPluginSystemPointer()->customNotifiacation(item ,message);
}

void IcqPluginSystem::getQutimVersion(quint8 &major, quint8 &minor, quint8 &secminor, quint16 &svn)
{
	m_parent_layer->getMainPluginSystemPointer()->getQutimVersion(major, minor, secminor, svn);
}

