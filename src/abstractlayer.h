/*
    AbstractLayer

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

#ifndef ABSTRACTLAYER_H_
#define ABSTRACTLAYER_H_

#include <QtGui>
#include <QSettings>
#include <QMenu>
#include "pluginsystem.h"
#include "abstractcontactlist.h"
class qutIM;
class IconManager;
class QHBoxLayout;

class AbstractLayer
{
public:
	AbstractLayer();
	~AbstractLayer();
  
  static AbstractLayer &instance();
  
	void setupProfile();
  
	void openSettingsDialog();
	void setPointers(qutIM *);
	void createNewAccount();
  void initializePointers(QTreeView *, QHBoxLayout *, QMenu *, QAction *);
	void clearMenuFromStatuses();
	void addStatusesToMenu();
	void reloadGeneralSettings();
	void addAccountMenusToTrayMenu(bool add);
	void setCurrentAccountIconName(const QString &account_name) 
	{ m_current_account_icon_name = account_name; }
	void updateStausMenusInTrayMenu();
	void setAutoAway();
	void setStatusAfterAutoAway();
	QString getCurrentProfile() const {return m_current_profile; }
	qutIM *getParent();
	void addActionToMainMenu(QAction *action);
private:
	QString m_current_profile;
	PluginSystem &m_plugin_system;
	qutIM *m_parent;
	IconManager &m_icon_manager;
	QMenu *m_tray_menu;
	QList<QMenu*> m_account_status_menu_list;
	AbstractContactList &m_contact_list_management;
	QAction *m_action_tray_menu_before;
	bool m_show_account_menus;
	bool m_is_new_profile;
	QString m_current_account_icon_name;
};

#endif /*ABSTRACTLAYER_H_*/
