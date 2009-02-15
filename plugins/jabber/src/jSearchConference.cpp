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

#include "jSearchConference.h"

jSearchConference::jSearchConference(const QString &target, jAccount *jabber_account, QWidget *parent) : QWidget(parent)
{
	m_jabber_account = jabber_account;
	m_target = target;
	ui.setupUi(this);
	this->setWindowTitle(QApplication::translate("SearchConference", "Search conference on", 0, QApplication::UnicodeUTF8) + " " + jabber_account->getAccountName());
	ui.conferenceServer->installEventFilter(this);
	ui.searchButton->setIcon(m_jabber_account->getPluginSystem().getIcon("search"));
	ui.selectButton->setIcon(m_jabber_account->getPluginSystem().getIcon("apply"));
	ui.closeButton->setIcon(m_jabber_account->getPluginSystem().getIcon("cancel"));
	connect(ui.conferenceTreeWidget,SIGNAL(itemDoubleClicked(QTreeWidgetItem*, int)), this, SLOT(joinConference(QTreeWidgetItem*, int)));
	connect(ui.conferenceTreeWidget,SIGNAL(itemExpanded(QTreeWidgetItem*)), this, SLOT(joinConference(QTreeWidgetItem*)));
	QString server = jProtocol::fromStd(JID(jProtocol::toStd(m_jabber_account->getAccountName())).server());
	ui.conferenceServer->addItem(server);
	on_searchButton_clicked();
}

jSearchConference::~jSearchConference()
{
}

void jSearchConference::closeEvent(QCloseEvent*)
{
	emit finishSearch();
}

bool jSearchConference::eventFilter(QObject *obj, QEvent *event)
{
	if (event->type() == QEvent::KeyPress) {
		QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
		if (keyEvent->key() == Qt::Key_Return || keyEvent->key() == Qt::Key_Enter)
			on_searchButton_clicked();
		return QObject::eventFilter(obj, event);
	} else {
		return QObject::eventFilter(obj, event);
	}
}

void jSearchConference::setConferencesList(QTreeWidgetItem *parent_item, const QList<QStringList> &confList)
{
	foreach (QStringList entityList, confList)
		setConferencesList(parent_item, entityList);
}

void jSearchConference::setConferencesList(QTreeWidgetItem *parent_item, const QStringList &entityList)
{
	//QStringList conferenceList = entityList[0];
	if (!parent_item)
	{
		return;
	}
	if (entityList[1].isEmpty())
	{
		if (entityList[2] == "server")
			parent_item->setIcon(0, m_jabber_account->getPluginSystem().getIcon("jabber_server"));
		else if (entityList[2] == "conference")
		{
			//if (entityList[2] != "nosearch")
			if (jProtocol::fromStd(JID(jProtocol::toStd(parent_item->text(1))).username()).isEmpty())
				parent_item->setIcon(0, m_jabber_account->getPluginSystem().getIcon("jabber_conf_server"));
			else
				parent_item->setIcon(0, m_jabber_account->getPluginSystem().getIcon("jabber_conf"));
		}
		parent_item->setText(0, entityList[0]);
		parent_item->setText(2, entityList[2]);
		parent_item->setText(3, entityList[3]);
		parent_item->setText(4, entityList[4]);
		return;
	}
	QTreeWidgetItem *item = new QTreeWidgetItem(parent_item);
	item->setText(0, entityList[0]);
	item->setText(1, entityList[1]);
	item->setText(2, entityList[2]);
	item->setText(3, entityList[3]);
	item->setText(4, entityList[4]);
	if (entityList[2] == "server")
	{
		item->setIcon(0, m_jabber_account->getPluginSystem().getIcon("jabber_server"));
		item->setChildIndicatorPolicy(QTreeWidgetItem::ShowIndicator);
	}
	else if (entityList[2] == "conference")
	{
		if (jProtocol::fromStd(JID(jProtocol::toStd(item->text(1))).username()).isEmpty())
		{
			item->setChildIndicatorPolicy(QTreeWidgetItem::ShowIndicator);
			item->setIcon(0, m_jabber_account->getPluginSystem().getIcon("jabber_conf_server"));
		}
		else
			item->setIcon(0, m_jabber_account->getPluginSystem().getIcon("jabber_conf"));
	}
	else
	{
		item->setChildIndicatorPolicy(QTreeWidgetItem::ShowIndicator);
	}
	parent_item->setExpanded(true);
	item->setExpanded(false);
}

void jSearchConference::on_searchButton_clicked()
{
	ui.conferenceTreeWidget->clear();
	QTreeWidgetItem *root_item = new QTreeWidgetItem();
	root_item->setText(0, "");
	root_item->setText(1, ui.conferenceServer->currentText());
	root_item->setText(2, "");
	root_item->setText(3, "");
	root_item->setText(4, "");
	ui.conferenceTreeWidget->addTopLevelItem(root_item);
	joinConference(root_item);
	/*
	jServiceDiscovery *handler = new jServiceDiscovery(root_item, "conference");
	connect(handler, SIGNAL(getDiscoItem(const QString&, const QString&, DiscoHandler*)), this, SLOT(getDiscoItem(const QString&, const QString&, DiscoHandler*)));
	connect(handler, SIGNAL(getDiscoInfo(const QString&, const QString&, DiscoHandler*)), this, SLOT(getDiscoInfo(const QString&, const QString&, DiscoHandler*)));
	connect(handler, SIGNAL(finishSearch(QTreeWidgetItem*, const QStringList&)), this, SLOT(setConferencesList(QTreeWidgetItem*, const QStringList&)));
	handler->search(ui.conferenceServer->currentText());
	*/
	//handler->search(ui.conferenceServer->currentText(), category, type);
	//m_jabber_account->getDiskoInfo(ui.conferenceServer->currentText(), "", "request-conference");
}

void jSearchConference::getDiscoItem(const QString &server, const QString &node, DiscoHandler *dh)
{
	m_jabber_account->getDiskoItem(server, node, dh);
}

void jSearchConference::getDiscoInfo(const QString &server, const QString &node, DiscoHandler *dh)
{
	m_jabber_account->getDiskoInfo(server, node, dh);
}

void jSearchConference::joinConference(QTreeWidgetItem *item, int) //double clicked
{
	//if (item->text(2) != "server" && !(item->text(2) == "conference" && jProtocol::fromStd(JID(jProtocol::toStd(item->text(1))).username()).isEmpty()))
	//{
		on_selectButton_clicked();
	//}
}

void jSearchConference::joinConference(QTreeWidgetItem *item) //expanded
{
	//on_selectButton_clicked();
	if (!item->childCount() && !(item->text(2) == "conference" && !jProtocol::fromStd(JID(jProtocol::toStd(item->text(1))).username()).isEmpty()))
	{
                        jServiceDiscovery *handler = new jServiceDiscovery(m_target, item);
			connect(handler, SIGNAL(getDiscoItem(const QString&, const QString&, DiscoHandler*)), this, SLOT(getDiscoItem(const QString&, const QString&, DiscoHandler*)));
			connect(handler, SIGNAL(getDiscoInfo(const QString&, const QString&, DiscoHandler*)), this, SLOT(getDiscoInfo(const QString&, const QString&, DiscoHandler*)));
			connect(handler, SIGNAL(finishSearch(QTreeWidgetItem*, const QStringList&)), this, SLOT(setConferencesList(QTreeWidgetItem*, const QStringList&)));
			connect(handler, SIGNAL(finishSearch(QTreeWidgetItem*, const QList<QStringList>&)), this, SLOT(setConferencesList(QTreeWidgetItem*, const QList<QStringList>&)));
			handler->search(item->text(1), item->text(2), item->text(3), item->text(4));
	}
}

void jSearchConference::on_selectButton_clicked()
{
	QTreeWidgetItem *item = ui.conferenceTreeWidget->currentItem();
	if (item->text(2) == "conference" && !jProtocol::fromStd(JID(jProtocol::toStd(item->text(1))).username()).isEmpty())
	{
		emit selectConference(item->text(1));
		close();
	}
	else if (item->text(2) == "gateway")
	{
                m_jabber_account->getProtocol()->showTransportReg(item->text(1));
	}
                else if (item->text(3) == "command-node")
        {
                jAdhoc *m_adhoc = new jAdhoc(m_jabber_account, item->text(1), item->text(4));
        }

	//int row = ui.conferenceListWidget->currentRow();
	//if (row > 0 && row < m_item_count)
	//{
	//	emit selectConference(ui.conferenceListWidget->currentItem()->text());
	//	close();
	//}
}

void jSearchConference::on_closeButton_clicked()
{
	close();
}
