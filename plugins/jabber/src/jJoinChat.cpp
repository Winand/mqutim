/*
Copyright (c) 2008 by Rustam Chakin <qutim.develop@gmail.com>,
Denis Daschenko <daschenko@gmail.com>

***************************************************************************
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
*                                                                         *
***************************************************************************
*/

#include "jJoinChat.h"

jJoinChat::jJoinChat(jAccount *jabber_account, const QString &room, const QString &password, bool showBookmark, QWidget *parent) : QWidget(parent)
{
	m_jabber_account = jabber_account;
	m_room = room;
	m_password = password;
	ui.setupUi(this);
	ui.bookmarkBox->setVisible(false);
	ui.autoJoinCheckBox->setVisible(false);
	ui.conferenceNameLineEdit->setEnabled(false);
	ui.saveButton->setVisible(false);
	ui.conferenceLineEdit->setCompleter(0);
//	ui.exampleLabel->setText("<font color='#808080'>(room@conference.server.url)</font>");
	move(desktopCenter());
	setAttribute(Qt::WA_QuitOnClose, false);
	setAttribute(Qt::WA_DeleteOnClose, true);
	connect(ui.conferenceListWidget,SIGNAL(currentItemChanged(QListWidgetItem*, QListWidgetItem*)), this, SLOT(showConference(QListWidgetItem*, QListWidgetItem*)));
	connect(ui.conferenceListWidget,SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(joinBookmark(QListWidgetItem*)));
	connect(ui.conferenceLineEdit, SIGNAL(currentIndexChanged(int)), this, SLOT(changeRecent(int)));
	ui.joinButton->setIcon(m_jabber_account->getPluginSystem().getIcon("apply"));
	ui.closeButton->setIcon(m_jabber_account->getPluginSystem().getIcon("cancel"));
//	ui.searchButton->setIcon(m_jabber_account->getPluginSystem().getIcon("search"));
	ui.saveButton->setIcon(m_jabber_account->getPluginSystem().getIcon("save_all"));
	ui.addConferenceButton->setIcon(m_jabber_account->getPluginSystem().getIcon("add"));
	ui.removeConferenceButton->setIcon(m_jabber_account->getPluginSystem().getIcon("remove"));
	m_settings = "qutim/qutim."+m_jabber_account->getProfileName()+"/jabber."+m_jabber_account->getAccountName();
	QSettings *recent_settings = new QSettings(QSettings::NativeFormat, QSettings::UserScope, m_settings, "recent");
	ui.conferenceLineEdit->addItem("");
	int size = recent_settings->beginReadArray("conferences");
	qDebug() << "------------------";
	for (int i = 0; i < size; ++i) {
		recent_settings->setArrayIndex(i);
		recent_conf << recent_settings->value("name").toString();
		qDebug() << "1";
		recent_nick << recent_settings->value("nick").toString();
		qDebug() << "2";
		recent_pass << recent_settings->value("password").toString();
		qDebug() << "***********************";
		ui.conferenceLineEdit->addItem(recent_conf[i]+" ("+recent_nick[i]+")");
	}
	recent_settings->endArray();
	ui.conferenceLineEdit->setCurrentIndex(0);
	m_account_settings = new QSettings(QSettings::NativeFormat, QSettings::UserScope, m_settings, "accountsettings");
	m_account_settings->beginGroup("main");
	isLocal = m_account_settings->value("localbookmark",false).toBool();
	m_account_settings->endGroup();
	if (showBookmark)
		if (isLocal)
		{
			getLocalBookmarks();
			fillConferences();
		}
		else
		{
			m_bookmark_storage = m_jabber_account->getProtocol()->getBookmarkStorage();
			//m_jabber_account->getProtocol()->requestBookmarks();
			getRecentBookmarks();
			fillConferences();
		}
}

jJoinChat::~jJoinChat()
{

}

void jJoinChat::getLocalBookmarks()
{
	m_c_list.clear();
	int size = m_account_settings->beginReadArray("bookmarks");
	for (int i = 0; i < size; ++i) {
		m_account_settings->setArrayIndex(i);
		ConferenceListItem item;
		item.name = jProtocol::toStd(m_account_settings->value("name").toString());
		item.jid = jProtocol::toStd(m_account_settings->value("jid").toString());
		item.nick = jProtocol::toStd(m_account_settings->value("nick").toString());
		item.password = jProtocol::toStd(m_account_settings->value("password").toString());
		m_c_list << item;
	}
	m_account_settings->endArray();
}

void jJoinChat::getRecentBookmarks()
{
	m_c_list.clear();
	m_b_list.clear();
	QSettings *recent_settings = new QSettings(QSettings::NativeFormat, QSettings::UserScope, m_settings, "recent");
	int size = recent_settings->beginReadArray("bookmarks");
	for (int i = 0; i < size; ++i)
	{
		recent_settings->setArrayIndex(i);
		ConferenceListItem item;
		item.name = jProtocol::toStd(recent_settings->value("name").toString());
		item.jid = jProtocol::toStd(recent_settings->value("jid").toString());
		item.nick = jProtocol::toStd(recent_settings->value("nick").toString());
		item.password = jProtocol::toStd(recent_settings->value("password").toString());
		item.autojoin = recent_settings->value("autojoin").toBool();
		m_c_list << item;
	}
	recent_settings->endArray();
	size = recent_settings->beginReadArray("urlmarks");
	for (int i = 0; i < size; ++i)
	{
		recent_settings->setArrayIndex(i);
		BookmarkListItem item;
		item.name = jProtocol::toStd(recent_settings->value("name").toString());
		item.url = jProtocol::toStd(recent_settings->value("url").toString());
		m_b_list << item;
	}
	recent_settings->endArray();
}


void jJoinChat::saveLocalBookmarks()
{
	m_account_settings->beginGroup("bookmarks");
	m_account_settings->remove("");
	m_account_settings->endGroup();
	m_account_settings->beginWriteArray("bookmarks");
	for (int i = 0; i < m_c_list.size(); ++i) {
		m_account_settings->setArrayIndex(i);
		m_account_settings->setValue("name", jProtocol::fromStd(m_c_list[i].name));
		m_account_settings->setValue("jid", jProtocol::fromStd(m_c_list[i].jid));
		m_account_settings->setValue("nick", jProtocol::fromStd(m_c_list[i].nick));
		m_account_settings->setValue("password", jProtocol::fromStd(m_c_list[i].password));
	}
	m_account_settings->endArray();
}

void jJoinChat::setConferences()
{
	getRecentBookmarks();
	fillConferences();
}

void jJoinChat::fillConferences()
{
	ui.bookmarkBox->setVisible(true);
	if (!isLocal)
		ui.autoJoinCheckBox->setVisible(true);
	ui.conferenceNameLineEdit->setEnabled(true);
	ui.saveButton->setVisible(true);
	int row = ui.conferenceListWidget->currentRow();
	ui.conferenceListWidget->clear();
	QString empty_name = tr("new chat");
	ui.conferenceListWidget->addItem(empty_name);
	foreach(ConferenceListItem item, m_c_list)
	{
		QString name = jProtocol::fromStd(item.name);
		if(name.isEmpty())
			name = jProtocol::fromStd(item.jid);
		ui.conferenceListWidget->addItem(name);
	}
	if (row == -1 || row > m_c_list.size()+1)
		showConference(0, 0);
	else
		ui.conferenceListWidget->setCurrentRow(row);
}

void jJoinChat::showConference(QListWidgetItem *current, QListWidgetItem *previous)
{
	int row = current==0?-1:ui.conferenceListWidget->row(current);
	if(row < 1 || row >= m_c_list.size()+1)
	{
		row = 0;
		ui.conferenceNameLineEdit->setText("");
		/*QString str = m_room;
		int num = ui.conferenceLineEdit->findText(str);
		if (num == -1)
		{
			num = 1;
			ui.conferenceLineEdit->insertItem(num, str);
		}
		ui.conferenceLineEdit->setCurrentIndex(num);*/
		ui.conferenceLineEdit->setEditText(m_room);
		ui.passwordLineEdit->setText(m_password);
		ui.nickLineEdit->setText(m_jabber_account->getProtocol()->getAccountName());
		ui.autoJoinCheckBox->setChecked(false);
		ui.conferenceListWidget->setCurrentRow(row);
	}
	else
	{
		m_room = "";
		m_password = "";
		ui.conferenceNameLineEdit->setText(jProtocol::fromStd(m_c_list[row-1].name));
		/*QString str = jProtocol::fromStd(m_c_list[row-1].jid);
		int num = ui.conferenceLineEdit->findText(str);
		if (num == -1)
		{
			num = 1;
			ui.conferenceLineEdit->insertItem(num, str);
		}
		ui.conferenceLineEdit->setCurrentIndex(num);*/
		ui.conferenceLineEdit->setEditText(jProtocol::fromStd(m_c_list[row-1].jid));
		ui.nickLineEdit->setText(jProtocol::fromStd(m_c_list[row-1].nick));
		ui.passwordLineEdit->setText(jProtocol::fromStd(m_c_list[row-1].password));
		ui.autoJoinCheckBox->setChecked(m_c_list[row-1].autojoin);
	}
}

void jJoinChat::setConferenceRoom(const QString &conference)
{
	showConference(0, 0);
	/*QString str = conference;
	int num = ui.conferenceLineEdit->findText(str);
	if (num == -1)
	{
		num = 1;
		ui.conferenceLineEdit->insertItem(num, str);
	}
	ui.conferenceLineEdit->setCurrentIndex(num);*/
	ui.conferenceLineEdit->setEditText(conference);
	ui.nickLineEdit->setText(m_jabber_account->getProtocol()->getAccountName());
}

void jJoinChat::finishSearch()
{
//	ui.searchButton->setEnabled(true);
}

QPoint jJoinChat::desktopCenter()
{
	QDesktopWidget desktop;
	return QPoint(desktop.width() / 2 - size().width() / 2, desktop.height() / 2 - size().height() / 2);
}

void jJoinChat::joinBookmark(QListWidgetItem *item)
{
	showConference(item, 0);
	if (ui.conferenceListWidget->currentRow() != 0)
		on_joinButton_clicked();
}

void jJoinChat::changeRecent(int i)
{
	if (!i)
		return;
	ui.conferenceLineEdit->setEditText(recent_conf[i-1]);
	ui.nickLineEdit->setText(recent_nick[i-1]);
	ui.passwordLineEdit->setText(recent_pass[i-1]);
}

void jJoinChat::on_joinButton_clicked()
{
	QSettings *recent_settings = new QSettings(QSettings::NativeFormat, QSettings::UserScope, m_settings, "recent");
	bool exist;
	for (int i = 0; i < recent_conf.size(); i++)
		if (recent_conf[i] == ui.conferenceLineEdit->currentText() && recent_nick[i] == ui.nickLineEdit->text())
			exist = true;
	if (!exist)
	{
		recent_conf.insert(0, ui.conferenceLineEdit->currentText());
		recent_nick.insert(0, ui.nickLineEdit->text());
		recent_pass.insert(0, ui.passwordLineEdit->text());
		recent_settings->beginWriteArray("conferences");
		for (int i = 0; i < recent_conf.size() && i < 10; ++i) {
			recent_settings->setArrayIndex(i);
			recent_settings->setValue("name", recent_conf[i]);
			recent_settings->setValue("nick", recent_nick[i]);
			recent_settings->setValue("password", recent_pass[i]);
		}
		recent_settings->endArray();
	}
	emit createConferenceRoom(ui.conferenceLineEdit->currentText(), ui.nickLineEdit->text(), ui.passwordLineEdit->text());
	close();
}
/*
void jJoinChat::on_searchButton_clicked()
{
	jSearchConference *jSConf = new jSearchConference("conference", m_jabber_account);
	connect(jSConf, SIGNAL(selectConference(const QString&)), this, SLOT(setConferenceRoom(const QString&)));
	connect(jSConf, SIGNAL(finishSearch()), this, SLOT(finishSearch()));
	jSConf->show();
	ui.searchButton->setEnabled(false);
}
*/
void jJoinChat::on_saveButton_clicked()
{
	int row = ui.conferenceListWidget->currentRow() - 1;
	if (ui.conferenceNameLineEdit->text().isEmpty())
	{
		if (ui.conferenceLineEdit->currentText().isEmpty())
			ui.conferenceNameLineEdit->text() = tr("New conference");
		else
			ui.conferenceNameLineEdit->setText(ui.conferenceLineEdit->currentText());
	}
	bool isNew = false;
	if (row < 0)
	{
		row = m_c_list.size();
		ConferenceListItem item;
		m_c_list.append(item);
		//QString b_name = ui.conferenceNameLineEdit->text();
		QListWidgetItem *w_item = new QListWidgetItem("", ui.conferenceListWidget);
		ui.conferenceListWidget->addItem(w_item);
		isNew = true;
	}
	m_c_list[row].name = jProtocol::toStd(ui.conferenceNameLineEdit->text());
	m_c_list[row].jid = jProtocol::toStd(ui.conferenceLineEdit->currentText());
	m_c_list[row].nick = jProtocol::toStd(ui.nickLineEdit->text());
	m_c_list[row].password = jProtocol::toStd(ui.passwordLineEdit->text());
	m_c_list[row].autojoin = ui.autoJoinCheckBox->isChecked();
	if (isNew)
		ui.conferenceListWidget->setCurrentRow(m_c_list.size());
	ui.conferenceListWidget->currentItem()->setText(ui.conferenceNameLineEdit->text());
	if (isLocal)
		saveLocalBookmarks();
	else
	{
// fixme
//		m_bookmark_storage->storeBookmarks(m_b_list.toAscii().data(), m_c_list.toAscii().data());
//		m_jabber_account->getProtocol()->setRecentBookmarks(m_b_list.toAscii().data(), m_c_list.toAscii().data());
	}
	//m_bookmark_storage->requestBookmarks();
}

void  jJoinChat::on_addConferenceButton_clicked()
{
	ConferenceListItem item;
	m_c_list.append(item);
	int row = m_c_list.size() - 1;
	m_c_list[row].name = jProtocol::toStd(tr("New conference"));
	m_c_list[row].nick = jProtocol::toStd(m_jabber_account->getProtocol()->getAccountName());
	m_c_list[row].autojoin = false;
	QListWidgetItem *w_item = new QListWidgetItem(tr("New conference"),ui.conferenceListWidget);
	ui.conferenceListWidget->addItem(w_item);
	ui.conferenceListWidget->setCurrentItem(w_item);
	ui.conferenceNameLineEdit->setFocus();
	ui.conferenceNameLineEdit->selectAll();
}

void  jJoinChat::on_removeConferenceButton_clicked()
{
	QListWidgetItem *item = ui.conferenceListWidget->currentItem();
	int row = ui.conferenceListWidget->row(item);
	if (row == 0)
		return;
	m_c_list.removeAt(row-1);
	delete item;
	if (isLocal)
		saveLocalBookmarks();
	else
	{
// fixme
//		m_bookmark_storage->storeBookmarks(m_b_list.toAscii().data(), m_c_list.toAscii().data());
//		m_jabber_account->getProtocol()->setRecentBookmarks(m_b_list.toAscii().data(), m_c_list.toAscii().data());
	}
	//m_bookmark_storage->requestBookmarks();
}
