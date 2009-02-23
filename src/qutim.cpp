/*
    qutim

    Copyright (c) 2008 by Rustam Chakin <qutim.develop@gmail.com>
						  Dmitri Arekhta <DaemonES@gmail.com>

 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************
*/

#include <QMutexLocker>
#include <QCloseEvent>
#include <QSettings>
#include <QMutex>
#include <QDebug>
#include <QMenu>
#include <QSoftMenuBar>
#include <QtopiaApplication>

#include "aboutinfo.h"
#include "contactseparator.h"

#include "qutim.h"
#include "ui_qutim.h"

#include "abstractlayer.h"
#include "abstractchatlayer.h"
#include "notifications/abstractnotificationlayer.h"
#include "abstractsoundlayer.h"
#include "abstractcontextlayer.h"

bool eventEater::eventFilter(QObject *obj, QEvent *event)
{
	if ( event->type() == QEvent::MouseButtonDblClick ||
			event->type() == QEvent::MouseButtonPress ||
			event->type() == QEvent::MouseButtonRelease ||
			event->type() == QEvent::MouseMove ||
			event->type() == QEvent::MouseTrackingChange ||
			event->type() == QEvent::KeyPress ||
			event->type() == QEvent::KeyRelease ||
			event->type() == QEvent::KeyboardLayoutChange)
	{
		fChanged = true;
	}
	return QObject::eventFilter(obj, event);
}


// Statics

qutIM				*qutIM::fInstance = NULL;
QMutex				qutIM::fInstanceGuard;

qutIM::qutIM(QWidget *parent, Qt::WFlags f ) :
  QTabWidget(parent),
	bShouldRun(true),
	m_iconManager (IconManager::instance()),
	m_abstract_layer(AbstractLayer::instance()),
	m_contact_list(AbstractContactList::instance()),
        m_switch_user(false), unreadMessages(false)
{
//	qApp->installTranslator(&applicationTranslator);

	m_abstract_layer.setPointers(this);
        if ( !m_abstract_layer.showLoginDialog() )
	{
		bShouldRun = false;
		QApplication::quit();
		return;
        }

//	m_abstract_layer->loadCurrentProfile();
	m_profile_name = m_abstract_layer.getCurrentProfile();
	reloadStyleLanguage();
	quitAction = NULL;
	msgIcon = false;
	m_plugin_settings = 0;

  contactListContainer = new QWidget(this);
	ui.setupUi(contactListContainer);
  ui.contactListView->header()->hide();
  contactListContainer->setFocusProxy(ui.contactListView);
  addTab(contactListContainer, QString::null);
  setCurrentWidget(contactListContainer);
  QSoftMenuBar::setLabel(this, Qt::Key_Back, QSoftMenuBar::NoLabel);

	createActions();
	createMainMenu();
//	ui.contactListView->setFocusProxy(this);
	eventObject = new eventEater(this);
	qApp->installEventFilter(eventObject);
	letMeQuit = false;
	setAttribute(Qt::WA_AlwaysShowToolTips, true);
	setFocus(Qt::ActiveWindowFocusReason);

	loadMainSettings();

	if (!bShouldRun)
		return;

	QTimer::singleShot(60000, this, SLOT(checkEventChanging()));

	aboutWindowOpen = false;
	connect(&fIdleDetector, SIGNAL(secondsIdle(int)), this, SLOT(onSecondsIdle(int)));

	// Activate idle detector
	fIdleDetector.start();

	QMutexLocker locker(&fInstanceGuard);
	fInstance = this;

  initIcons();

  m_abstract_layer.initializePointers(ui.contactListView, ui.hboxLayout, trayMenu,
                  settingsAction);
  AbstractContextLayer::instance().createActions();
}

qutIM::~qutIM()
{
	// Stop idle detector
	fIdleDetector.stop();
}

qutIM *qutIM::getInstance()
{
	QMutexLocker locker(&fInstanceGuard);
	return fInstance;
}

void qutIM::createActions()
{
	quitAction = new QAction(m_iconManager.getIcon("exit"),
			tr("&Quit"), this);
	settingsAction = new QAction(m_iconManager.getIcon("settings"),
			tr("&Settings..."), this);
	m_pluginSettingsAction = new QAction(m_iconManager.getIcon("additional"),
			tr("Plug-in settings..."), this);
	switchUserAction = new QAction(m_iconManager.getIcon("switch_user"),
			tr("Switch profile"), this);
	connect(quitAction            , SIGNAL(triggered()), this, SLOT(appQuit()));
	connect(settingsAction        , SIGNAL(triggered()), this, SLOT(qutimSettingsMenu()));
	connect(m_pluginSettingsAction, SIGNAL(triggered()), this, SLOT(qutimPluginSettingsMenu()));
	connect(switchUserAction      , SIGNAL(triggered()), this, SLOT(switchUser()));
}

void qutIM::appQuit()
{
	AbstractContactList::instance().saveSettings();
//	letMeQuit = true;
	QCoreApplication::exit(0);
//	close();
}

void qutIM::loadMainSettings()
{
  QSettings settings(QSettings::NativeFormat, QSettings::UserScope, "qutim/qutim."+m_profile_name, "profilesettings");
	createMenuAccounts = settings.value("general/accountmenu", true).toBool();

	m_abstract_layer.setCurrentAccountIconName(settings.value("general/currentaccount", "").toString());

	m_auto_away = settings.value("general/autoaway", true).toBool();
	m_auto_away_minutes = settings.value("general/awaymin", 10).toUInt();

	if(!settings.value("contactlist/showoffline",true).toBool())
	{
		ui.showHideButton->setIcon(m_iconManager.getIcon("hideoffline"));
		ui.showHideButton->setChecked(true);
	}
	else
	{
		ui.showHideButton->setIcon(m_iconManager.getIcon("shhideoffline"));
		ui.showHideButton->setChecked(true);
	}
	if(!(settings.value("contactlist/modeltype",0).toInt()&2))
		ui.showHideGroupsButton->setChecked(true);
	if(settings.value("enable",true).toBool())
		ui.soundOnOffButton->setChecked(true);
}

void qutIM::qutimSettingsMenu()
{
	settingsAction->setDisabled(true);
	switchUserAction->setDisabled(true);
	ui.showHideButton->setEnabled(false);
	ui.showHideGroupsButton->setEnabled(false);
	ui.soundOnOffButton->setEnabled(false);
	m_abstract_layer.openSettingsDialog();
}

void qutIM::qutimPluginSettingsMenu()
{
	m_plugin_settings = new PluginSettings;
	connect(m_plugin_settings, SIGNAL(destroyed ( QObject * )),
			this, SLOT(pluginSettingsDeleted(QObject *)));
	m_plugin_settings->show();
	m_pluginSettingsAction->setEnabled(false);
}

void qutIM::destroySettings()
{
	settingsAction->setDisabled(false);
	switchUserAction->setDisabled(false);
	ui.showHideButton->setEnabled(true);
	ui.showHideGroupsButton->setEnabled(true);
	ui.soundOnOffButton->setEnabled(true);
}

void qutIM::reloadGeneralSettings()
{
  QSettings settings(QSettings::NativeFormat, QSettings::UserScope, "qutim/qutim."+m_profile_name, "profilesettings");

	bool cr_m_acc = settings.value("general/accountmenu", true).toBool();
	if ( cr_m_acc != createMenuAccounts )
	{
		createMenuAccounts = cr_m_acc;
		m_abstract_layer.addAccountMenusToTrayMenu(createMenuAccounts);
	}
	bool visible_now = isVisible();

	Qt::WindowFlags flags = windowFlags();
	if ( settings.value("general/ontop", false).toBool())
		flags |= Qt::WindowStaysOnTopHint;
	else
	    flags &= ~Qt::WindowStaysOnTopHint;
	setWindowFlags(flags);

	setVisible(visible_now);
	m_abstract_layer.setCurrentAccountIconName(settings.value("general/currentaccount", "").toString());
	m_auto_away = settings.value("general/autoaway", true).toBool();
	m_auto_away_minutes = settings.value("general/awaymin", 10).toUInt();
}

void qutIM::switchUser()
{
	m_switch_user = true;
        QSettings settings(QSettings::NativeFormat, QSettings::UserScope, "qutim", "qutimsettings");
	settings.setValue("general/switch", true);
	QProcess::startDetached(qApp->applicationFilePath());
	appQuit();
}

void qutIM::keyPressEvent( QKeyEvent *event )
{
  if (event->key() == Qt::Key_Back)
    return; // ignore
}


void qutIM::checkEventChanging()
{
	if ( eventObject->getChanged() )
	{
		eventObject->setChanged(false);
	}
	QTimer::singleShot(60000, this, SLOT(checkEventChanging()));
}

void qutIM::updateTrayToolTip()
{
}

void qutIM::createMainMenu()
{
  mainMenu = QSoftMenuBar::menuFor(this);
	if ( m_plugin_actions.count() > 0)
	{
		foreach(QAction *action, m_plugin_actions)
		{
			mainMenu->addAction(action);
		}
		mainMenu->addSeparator();
	}
	mainMenu->addAction(settingsAction);
	//mainMenu->addAction(m_pluginSettingsAction);
	mainMenu->addAction(switchUserAction);
	mainMenu->addSeparator();
	mainMenu->addAction(quitAction);
}

void qutIM::infoWindowDestroyed(QObject *)
{
	aboutWindowOpen = false;

}

void qutIM::on_showHideButton_clicked()
{
  QSettings settings(QSettings::NativeFormat, QSettings::UserScope, "qutim/qutim."+m_profile_name, "profilesettings");
	settings.beginGroup("contactlist");
	bool show = settings.value("showoffline",true).toBool();
	settings.setValue("showoffline",!show);
	settings.endGroup();
	AbstractContactList &acl = AbstractContactList::instance();
	if(show)
		ui.showHideButton->setIcon(m_iconManager.getIcon("hideoffline"));
	else
		ui.showHideButton->setIcon(m_iconManager.getIcon("shhideoffline"));
	acl.loadSettings();
}


void qutIM::initIcons()
{
	ui.showHideGroupsButton    ->setIcon(m_iconManager.getIcon("folder"         ));
	ui.soundOnOffButton    ->setIcon(m_iconManager.getIcon("player_volume"         ));
	ui.showHideButton->setIcon(m_iconManager.getIcon("shhideoffline"));
}

void qutIM::onSecondsIdle(int seconds)
{
	// Autoaway is disabled, do not touch anything
	if (!m_auto_away)
		return ;

	// if activity is detected
	if (seconds == 0)
	{
		m_abstract_layer.setStatusAfterAutoAway();
	}

	// If idle time is bigger than autoaway time
	if (seconds > static_cast<int>(m_auto_away_minutes) * 60)
	{
		m_abstract_layer.setAutoAway();
	}
}

void qutIM::on_showHideGroupsButton_clicked()
{
        QSettings settings(QSettings::NativeFormat, QSettings::UserScope, "qutim/qutim."+m_profile_name, "profilesettings");
	settings.beginGroup("contactlist");
	int type = settings.value("modeltype",0).toInt();
	if((type&2)==2)
		type-=2;
	else
		type+=2;
	settings.setValue("modeltype",type);
	settings.endGroup();
	AbstractContactList &acl = AbstractContactList::instance();
	acl.loadSettings();
}

void qutIM::showBallon(const QString &title, const QString &message, int time)
{
}

void qutIM::on_soundOnOffButton_clicked()
{
        QSettings settings(QSettings::NativeFormat, QSettings::UserScope, "qutim/qutim."+m_profile_name, "profilesettings");
	settings.beginGroup("sounds");
	bool enable = settings.value("enable",true).toBool();
	settings.setValue("enable",!enable);
	settings.endGroup();
	AbstractSoundLayer::instance().loadSettings();
}

void qutIM::reloadStyleLanguage()
{
        QSettings settings(QSettings::NativeFormat, QSettings::UserScope, "qutim/qutim."+m_profile_name, "profilesettings");
	qApp->removeTranslator(&applicationTranslator);
	QString translation_path = settings.value("gui/language", "").toString() + "/main.qm";
	applicationTranslator.load(translation_path);
	qApp->installTranslator(&applicationTranslator);
	QString current_style = settings.value("gui/style", "").toString();
	if ( !current_style.isEmpty() )
	{
		QString path_to_style = current_style.section("/", 0, -2);
		QFile file(current_style);
		if(file.open(QFile::ReadOnly))
		{
			qApp->setStyleSheet("");
		    QString styleSheet = QLatin1String(file.readAll());
		    qApp->setStyleSheet(styleSheet.replace("%path%", path_to_style));
		    file.close();
		}
	}
	else
		qApp->setStyleSheet("");
}

void qutIM::pluginSettingsDeleted(QObject *)
{
	m_pluginSettingsAction->setEnabled(true);
}

void qutIM::addActionToList(QAction *action)
{
	m_plugin_actions.append(action);
}

bool qutIM::eventFilter(QObject *watched, QEvent *event)
{
  QWidget *w_receiver = qobject_cast<QWidget *>(watched);
  if (w_receiver)
  {
    int tab_index = indexOf(w_receiver);
    if (tab_index<0)
      return false; // not a tab

    if (event->type()==QEvent::WindowIconChange)
    {
      qDebug() << "Tab" << tab_index << "icon changed";
      //setTabIcon(tab_index, w_receiver->windowIcon());
    }
    else if (event->type()==QEvent::WindowTitleChange)
    {
      qDebug() << "Tab" << tab_index << "title changed to" << w_receiver;
      setTabText(tab_index, w_receiver->windowTitle());
    }
  }
  return false;
}

void qutIM::tabInserted(int index)
{
  qDebug() << "Widget inserted at" << index;
  QWidget *w = widget(index);
  if (w)
  {
    w->installEventFilter(this);
    //setTabIcon(index, w->windowIcon());
    if (!w->windowTitle().isEmpty())
      setTabText(index, w->windowTitle());
  }
  if (count()==1)
    hideTabBar();
  else
    showTabBar();
}

void qutIM::tabRemoved(int index)
{
  if (count()==1)
    hideTabBar();
}

void qutIM::hideTabBar()
{
  tabBar()->hide();
}

void qutIM::showTabBar()
{
  tabBar()->show();
}
