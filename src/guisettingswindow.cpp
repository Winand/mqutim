/*
    GuiSetttingsWindow

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

#include <QDesktopWidget>
#include "guisettingswindow.h"
#include <QSettings>
#include <QDebug>
#include <QDir>
#include <QStringList>
#include "abstractchatlayer.h"
#include "notifications/abstractnotificationlayer.h"
#include "abstractlayer.h"
#include "abstractcontextlayer.h"


class Resource: public QResource
{
	public:
	using QResource::children;
	using QResource::isFile;
	using QResource::isDir;
	Resource() : QResource(){}
};

GuiSetttingsWindow::GuiSetttingsWindow(const QString &profile_name, QWidget *parent)
    : QWidget(parent)
    , m_profile_name(profile_name)
{
	ui.setupUi(this);
	setMinimumSize(size());
	setAttribute(Qt::WA_QuitOnClose, false); //don't close app on "Settings" exit
	setAttribute(Qt::WA_DeleteOnClose, true);
	move(desktopCenter());
	addUiContent();
	loadSettings();
	connect(ui.emoticonsComboBox, SIGNAL(currentIndexChanged(int)),
			this, SLOT(enableApplyButton()));
	connect(ui.chatComboBox, SIGNAL(currentIndexChanged(int)),
			this, SLOT(enableApplyButton()));
	connect(ui.webkitComboBox, SIGNAL(currentIndexChanged(int)),
			this, SLOT(enableApplyButton()));
	connect(ui.popupComboBox, SIGNAL(currentIndexChanged(int)),
			this, SLOT(enableApplyButton()));
	connect(ui.systemIconComboBox, SIGNAL(currentIndexChanged(int)),
			this, SLOT(enableApplyButton()));
	connect(ui.statusIconComboBox, SIGNAL(currentIndexChanged(int)),
			this, SLOT(enableApplyButton()));
	connect(ui.listThemeComboBox, SIGNAL(currentIndexChanged(int)),
			this, SLOT(enableApplyButton()));
	connect(ui.languageBox, SIGNAL(currentIndexChanged(int)),
			this, SLOT(enableApplyButton()));
	connect(ui.styleBox, SIGNAL(currentIndexChanged(int)),
			this, SLOT(enableApplyButton()));
	connect(ui.borderComboBox, SIGNAL(currentIndexChanged(int)),
			this, SLOT(enableApplyButton()));
}

GuiSetttingsWindow::~GuiSetttingsWindow()
{

}

QPoint GuiSetttingsWindow::desktopCenter()
{
	QDesktopWidget desktop;
	return QPoint(desktop.width() / 2 - size().width() / 2, desktop.height() / 2 - size().height() / 2);
}

void GuiSetttingsWindow::enableApplyButton()
{
	ui.applyButton->setEnabled(true);
}

void GuiSetttingsWindow::addUiContent()
{
        QSettings settings(QSettings::NativeFormat, QSettings::UserScope, "qutim", "qutimsettings");

	QStringList paths;
	QString appPath = qApp->applicationDirPath();
	if (appPath.at(appPath.size()-1) == '/') appPath.chop(1);
#if !defined(Q_WS_WIN)
	// ../share/qutim
	paths << appPath.section('/', 0, -2) + "/share/qutim";
#endif
	// ./
	paths << appPath;
	// ~/.config/qutim or Application Data
	paths << settings.fileName().section('/', 0, -2);
#if defined(Q_WS_MAC)
	// ./ Application bundle
	paths << qApp->applicationDirPath().section('/', 0, -2) + "/Resources";
#endif
	addWebkitThemes("");
	foreach(QString path, paths)
	{
		QString emoticons_path = path + "/emoticons";
		QString chat_path = path + "/chatstyle";
		QString webkit_path = path + "/webkitstyle";
		QString popup_path = path + "/traythemes";
		QString status_path = path + "/statusicons";
		QString system_path = path + "/systemicons";
		QString contact_list_theme_path = path + "/clstyles";
		QString languages_path = path + "/languages";
		QString application_styles_path = path + "/styles";
		QString border_themes_path = path + "/borders";
		addEmoticonThemes(emoticons_path);
		addListThemes(contact_list_theme_path);
		addChatThemes(chat_path);
		addWebkitThemes(webkit_path);
		addPopupThemes(popup_path);
		addStatusThemes(status_path);
		addSystemThemes(system_path);
		addLanguages(languages_path);
		addApplicationStyles(application_styles_path);
		addBorderThemes(border_themes_path);
	}
}

void GuiSetttingsWindow::addLanguages(const QString &path)
{
	QDir lang_dir(path);
	QStringList themes(lang_dir.entryList(QDir::AllDirs | QDir::NoDotAndDotDot));

	foreach(QString dirName, themes)
	{
		ui.languageBox->addItem(dirName, path + "/" + dirName);
	}
}

void GuiSetttingsWindow::addApplicationStyles(const QString &path)
{
	QDir style_path(path);
	QStringList themes(style_path.entryList(QDir::AllDirs | QDir::NoDotAndDotDot));
	foreach(QString dirName, themes)
	{
		QDir dir(path + "/" + dirName);
		QStringList filter;
		filter << "*.qss";
		QStringList file_qss = dir.entryList(filter,QDir::Files);
		if (file_qss.count())
		{
			ui.styleBox->addItem(dirName, dir.path() + "/" + file_qss.at(0));
		}
	}
}

void GuiSetttingsWindow::addEmoticonThemes(const QString &path)
{
	QDir emoticonPath(path);
	QStringList themes(emoticonPath.entryList(QDir::AllDirs | QDir::NoDotAndDotDot));
	foreach(QString dirName, themes)
	{
		QDir dir(path + "/" + dirName);
		QStringList filter;
		filter << "*.xml";
		QStringList fileXML = dir.entryList(filter,QDir::Files);
		if (fileXML.count())
		{
			ui.emoticonsComboBox->addItem(dirName, dir.path() + "/" + fileXML.at(0));
		}
	}
}

void GuiSetttingsWindow::addListThemes(const QString &path)
{
	QDir list_dir(path);
	QStringList filters;
	filters << "*.ListTheme" << "*.ListQutim";
	
	QStringList dirs_with_themes(list_dir.entryList(QDir::AllDirs | QDir::NoDotAndDotDot));//.entryList(filters, QDir::Dirs | QDir::NoDotAndDotDot | QDir::Files));
	foreach(QString dir_with_themes, dirs_with_themes)
	{
		QDir dir(path+"/"+dir_with_themes);
		QStringList themes(dir.entryList(filters, QDir::Dirs | QDir::NoDotAndDotDot | QDir::Files));
		foreach(QString dir_name, themes)
			ui.listThemeComboBox->addItem(dir_name.left(dir_name.lastIndexOf('.'))+(dir_name.endsWith(".ListTheme")?" (Adium Style)":" (qutIM Style)"), path+"/"+dir_with_themes+"/"+dir_name);
	}
}

void GuiSetttingsWindow::addChatThemes(const QString &path)
{
	QDir chat_dir(path);
	QStringList themes(chat_dir.entryList(QDir::AllDirs | QDir::NoDotAndDotDot));

	foreach(QString dirName, themes)
	{
		ui.chatComboBox->addItem(dirName, path + "/" + dirName);
	}
}

void GuiSetttingsWindow::addWebkitThemes(const QString &path)
{
	if(path.isEmpty())
	{
		Resource variantDir;
		variantDir.setFileName("webkit/Contents/Resources/Variants");
		if(!variantDir.isDir())
			return;
		ui.webkitComboBox->clear();
		QStringList variantList = variantDir.children();
		foreach(QString variantName, variantList)
		{
			if(variantName.endsWith(".css"))
			{
				QString variantPath = "Variants/"+variantName;
				variantName = variantName.left(variantName.lastIndexOf("."));
				QStringList params;
				params<<""<<variantName;
				ui.webkitComboBox->addItem("Default (" + variantName + ")", params);
			}
		}
		return;
	}
	QDir chat_dir(path);
	QStringList themes(chat_dir.entryList(QDir::AllDirs | QDir::NoDotAndDotDot));

	foreach(QString dirName, themes)
	{
		QString theme_path = path + "/" + dirName + "/Contents/Resources/";
		QDir variant_dir(theme_path + "/Variants");
		QStringList variants(variant_dir.entryList(QDir::Files | QDir::NoDotAndDotDot));
		QSettings settings(path + "/" + dirName + "/Contents/Info.plist",PluginSystem::instance().plistFormat());
		QString style_name = settings.value("CFBundleName",dirName).toString();
		if(settings.value("MessageViewVersion").toInt()<3 && settings.contains("DisplayNameForNoVariant"))
		{
			QStringList params;
			params<<theme_path<<"";
			ui.webkitComboBox->addItem(style_name + " (" + settings.value("DisplayNameForNoVariant").toString() + ")", params);
		}
		if ( variants.count() )
		{
			foreach(QString variant, variants)
			{
				QStringList params;
				QFileInfo file_info(variant);
				params<<theme_path<<file_info.completeBaseName();
				ui.webkitComboBox->addItem(style_name + " (" + file_info.completeBaseName() + ")", params);
			}
		}
		else if(!settings.contains("DisplayNameForNoVariant"))
		{
			QStringList params;
			params<<theme_path<<"";
			ui.webkitComboBox->addItem(style_name, params);
		}
	}
}

void GuiSetttingsWindow::addStatusThemes(const QString &path)
{
	QDir chat_dir(path);
	QStringList themes(chat_dir.entryList(QDir::AllDirs | QDir::NoDotAndDotDot));
	
	foreach(QString dir_name, themes)
	{
		if(dir_name.indexOf(".")>-1)
			ui.statusIconComboBox->addItem(dir_name.left(dir_name.lastIndexOf('.')),path+"/"+dir_name);
		else
			ui.statusIconComboBox->addItem(dir_name, path+"/"+dir_name);
	
	}
}

void GuiSetttingsWindow::loadSettings()
{
        QSettings settings(QSettings::NativeFormat, QSettings::UserScope, "qutim/qutim."+m_profile_name, "profilesettings");
	settings.beginGroup("gui");
	int emoticon_index = ui.emoticonsComboBox->findData(settings.value("emoticons","").toString());
	ui.emoticonsComboBox->setCurrentIndex(emoticon_index > -1 ?emoticon_index:0);
	int list_theme_index = ui.listThemeComboBox->findData(settings.value("listtheme","").toString());
	ui.listThemeComboBox->setCurrentIndex(list_theme_index > -1 ?list_theme_index:0);
	int chat_index = ui.chatComboBox->findData(settings.value("chat","").toString());
	ui.chatComboBox->setCurrentIndex(chat_index > -1 ?chat_index:0);
	int popup_index = ui.popupComboBox->findData(settings.value("popup","").toString());
	ui.popupComboBox->setCurrentIndex(popup_index > -1 ?popup_index:0);
	int system_index = ui.systemIconComboBox->findData(settings.value("systemicon","").toString());
	ui.systemIconComboBox->setCurrentIndex(system_index > -1 ?system_index:0);
	int status_index = ui.statusIconComboBox->findData(settings.value("statusicon","").toString());
	ui.statusIconComboBox->setCurrentIndex(status_index > -1 ?status_index:0);
	QStringList webkit_param;
	webkit_param<<settings.value("wstyle","").toString()
				<<settings.value("wvariant","").toString();
	int webkit_index = ui.webkitComboBox->findData(webkit_param); 
	ui.webkitComboBox->setCurrentIndex(webkit_index > -1 ?webkit_index:0);
	int lang_index = ui.languageBox->findData(settings.value("language","").toString());
	ui.languageBox->setCurrentIndex(lang_index > -1 ?lang_index:0);
	int style_index = ui.styleBox->findData(settings.value("style","").toString());
	ui.styleBox->setCurrentIndex(style_index > -1 ?style_index:0);
	int border_index = ui.borderComboBox->findData(settings.value("borders","").toString());
	ui.borderComboBox->setCurrentIndex(border_index > -1 ?border_index:0);
	settings.endGroup();
}

void GuiSetttingsWindow::saveSettings()
{
        QSettings settings(QSettings::NativeFormat, QSettings::UserScope, "qutim/qutim."+m_profile_name, "profilesettings");
	settings.beginGroup("gui");
	settings.setValue("emoticons", ui.emoticonsComboBox->itemData(ui.emoticonsComboBox->currentIndex()).toString());
	settings.setValue("listtheme", ui.listThemeComboBox->itemData(ui.listThemeComboBox->currentIndex()).toString());
	settings.setValue("chat", ui.chatComboBox->itemData(ui.chatComboBox->currentIndex()).toString());
	settings.setValue("popup", ui.popupComboBox->itemData(ui.popupComboBox->currentIndex()).toString());
	settings.setValue("systemicon", ui.systemIconComboBox->itemData(ui.systemIconComboBox->currentIndex()).toString());
	settings.setValue("statusicon", ui.statusIconComboBox->itemData(ui.statusIconComboBox->currentIndex()).toString());
	QStringList webkit_param = ui.webkitComboBox->itemData(ui.webkitComboBox->currentIndex()).toStringList();
	if ( webkit_param.count() )
	{
		settings.setValue("wstyle", webkit_param.at(0));
		settings.setValue("wvariant", webkit_param.at(1));
	}
	else
	{
		settings.setValue("wstyle", "");
		settings.setValue("wvariant", "");
	}
	settings.setValue("language", ui.languageBox->itemData(ui.languageBox->currentIndex()).toString());
	settings.setValue("style", ui.styleBox->itemData(ui.styleBox->currentIndex()).toString());
	settings.setValue("borders", ui.borderComboBox->itemData(ui.borderComboBox->currentIndex()).toString());
	settings.endGroup();
	reloadContent();
}

void GuiSetttingsWindow::on_applyButton_clicked()
{
	saveSettings();
	ui.applyButton->setEnabled(false);
}

void GuiSetttingsWindow::on_okButton_clicked()
{
	if ( ui.applyButton->isEnabled() )
	{
		saveSettings();
	}
	close();
}

void GuiSetttingsWindow::reloadContent()
{
	AbstractChatLayer &acl = AbstractChatLayer::instance();
	acl.reloadContent();
	AbstractNotificationLayer::instance().loadTheme();
	AbstractContactList::instance().loadGuiSettings();
	AbstractLayer::instance().reloadStyleLanguage();
        AbstractContextLayer::instance().createActions();
}

void GuiSetttingsWindow::addPopupThemes(const QString &path)
{
	QDir popup_dir(path);
	QStringList themes(popup_dir.entryList(QDir::AllDirs | QDir::NoDotAndDotDot));

	foreach(QString dirName, themes)
	{
		ui.popupComboBox->addItem(dirName, path + "/" + dirName);
	}
}
void GuiSetttingsWindow::addSystemThemes(const QString &path)
{
	QDir popup_dir(path);
	QStringList themes(popup_dir.entryList(QDir::AllDirs | QDir::NoDotAndDotDot));

	foreach(QString dirName, themes)
	{
		ui.systemIconComboBox->addItem(dirName, path + "/" + dirName);
	}	
}

void GuiSetttingsWindow::addBorderThemes(const QString &path)
{
	QDir border_dir(path);
	QStringList borders(border_dir.entryList(QDir::AllDirs | QDir::NoDotAndDotDot));

	foreach(QString dirName, borders)
	{
		ui.borderComboBox->addItem(dirName, path + "/" + dirName);
	}
}
