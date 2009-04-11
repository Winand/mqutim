/*
    qutim

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


#ifndef QUTIM_H
#define QUTIM_H

#include <memory>
#include <QWidget>
#include <QTranslator>
#include <QSettings>
#include <QFile>
#include "abstractcontactlist.h"

#include "iconmanager.h"
#include "pluginsettings.h"
#include "ui_qutim.h"
#include "idle/idle.h"

class QCloseEvent;
class QMutex;
class QMenu;
class aboutInfo;
class AbstractLayer;

class eventEater : public QObject
{
	Q_OBJECT

public:
	eventEater(QWidget *parent = 0) : QObject(parent), fChanged(false) { }

        bool getChanged() const { return fChanged; };
        void setChanged(const bool changed) { fChanged = changed; };

protected:
	bool fChanged;

	bool eventFilter(QObject *obj, QEvent *event);

};


class qutIM : public QTabWidget
{
    Q_OBJECT

public:
    qutIM(QWidget *parent = 0, Qt::WFlags f = 0 );
	virtual ~qutIM();

	static qutIM *getInstance();

  void hideTabBar();
  void showTabBar();

/*	bool isShouldRun() { return bShouldRun; }*/
	void reloadGeneralSettings();

        void updateTrayIcon(const QIcon &);
	void showBallon(const QString &title, const QString &message, int time);

	void addActionToList(QAction *);

  bool eventFilter (QObject *watched, QEvent *event); // Intercept some events to provide pseudo-wm
                                                      // capabilities for tabs
private slots:
	void appQuit();
	void qutimSettingsMenu();
	void qutimPluginSettingsMenu();//!< Now it only shows plug-in settings window.
	void pluginSettingsDeleted(QObject *);
	void destroySettings();
	void checkEventChanging();
	void updateTrayToolTip();
	void infoWindowDestroyed(QObject *);
	void on_showHideButton_clicked();
	void onSecondsIdle(int);
	void on_showHideGroupsButton_clicked();
	void on_soundOnOffButton_clicked();
//	void on_toolButton_clicked();
protected:
  void tabInserted(int index); // monitor new tabs
  void tabRemoved(int index);

	void keyPressEvent(QKeyEvent *event);
private:
	static qutIM		*fInstance;
	static QMutex		fInstanceGuard;

  Ui::qutIMClass ui;
  QWidget *contactListContainer;

  QList<QAction *> m_plugin_actions;
  QAction *quitAction;
  QAction *settingsAction;
  QAction *m_pluginSettingsAction;//!< This action connected with qutimPluginSettingsMenu();
  QAction *switchUserAction;
  QAction *m_gui_settings_action;
  QMenu *mainMenu;
  QMenu *trayMenu;

/*  bool bShouldRun;*/
  bool createMenuAccounts;
/*  bool letMeQuit;*/
  bool unreadMessages;
  bool m_auto_away;
  bool msgIcon;
  quint32 m_auto_away_minutes;

  IconManager& m_iconManager;//!< use it to get icons from file or program

  eventEater *eventObject;
  aboutInfo *infoWindow;
  bool aboutWindowOpen;
  QTranslator applicationTranslator;

  QIcon tempIcon;
// Idle detector
	Idle fIdleDetector;
//    void createTrayIcon();
  void createActions();
  void createMainMenu();
  void saveMainSettings();
  void loadMainSettings();

  void initIcons();//!< It loads all icons by icon manager.

  AbstractLayer &m_abstract_layer;
  AbstractContactList &m_contact_list;
  QString m_profile_name;
  bool m_switch_user;
  PluginSettings *m_plugin_settings;
signals:
	void updateTranslation();

};

#endif // QUTIM_H
