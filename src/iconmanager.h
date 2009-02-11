/*
    iconManager

    Copyright (c) 2008 by Alexey Pereslavtsev <steel1.0@mail.ru>
    						Nigmatullin Ruslan <euroelessar@gmail.com>

 This file is a part of qutIM project.
 You can find more information at http://qutim.org


 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************
*/


#ifndef ICONMANAGER_H
#define ICONMANAGER_H
#include <QString>
#include <QIcon>
#include <QHash>
#include <QtXml>

/*!
 * Icon manager class.
 * If you use icons, then you should use this class to get path to your icon.
 * IconManager knows about some kinds of icons.
 */
class IconManager
{
public:
	//! The constructor sets all path to internal.
	IconManager();

	//! Call instance of IconManager
	static IconManager &instance();

	//! This function returns full path to some system icon.
	//! \param Short name of icon (can be without extension)
	//! \example ui.deleteButton->setIcon (QIcon (iconManager->getSystemIconName("delete_user")));
	QString getIconFileName(const QString & icon_name);

	//! This function returns some system icon
	//! \param Short name of icon (can be without extension)
	//! \example ui.deleteButton->setIcon (iconManager->getSystemIcon("delete_user"));
	QIcon getIcon(const QString & icon_name);

	QString getStatusIconFileName(const QString & icon_name, const QString & default_path = "");
	QIcon getStatusIcon(const QString & icon_name, const QString & default_path = "");
	bool setStatusIconPackage(const QString & path);
	bool setIconPackage(const QString & path);
	void loadSettings();
	void loadProfile(const QString & profile_name);
private:
	QString m_profile_name;
	QString m_path_to_system_icons;
	QString m_path_to_status_icons;
	QString m_default_path_to_system_icons;
	QString m_default_path_to_status_icons;
	QString m_default_path_to_icons;
	QHash<QString,QIcon> m_status_icons;
	QHash<QString,QString> m_status_icons_name;
	QHash<QString, QString> m_from_adium;
	QHash<QString,QIcon> m_icons;
	QHash<QString,QString> m_system_icons_name;
};



#endif //ICONMANAGER_H
