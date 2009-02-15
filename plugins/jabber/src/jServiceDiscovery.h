/*
Copyright (c) 2008 by Denis Daschenko <daschenko@gmail.com>
***************************************************************************
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
*                                                                         *
***************************************************************************
*/

#ifndef JSERVICEDISCOVERY_H
#define JSERVICEDISCOVERY_H

#include <QObject>
#include <QString>
#include <QStringList>
#include <gloox/discohandler.h>
#include <gloox/disco.h>
#include "jProtocol.h"
#include "jDiscoItem.h"
#include "jDiscoIdentity.h"

using namespace gloox;

class jServiceDiscovery : public QObject, DiscoHandler
{
	Q_OBJECT

	public:
                jServiceDiscovery(const QString &type, QTreeWidgetItem *item = 0);
		~jServiceDiscovery();
		void search(const QString &jid, const QString &category, const QString &type, const QString &node);
		void search(const jDiscoItem &disco_item);
		void handleDiscoInfo(const JID &from, const Disco::Info &info, int context);
		void handleDiscoItems(const JID &from, const Disco::Items &items, int context);
		void handleDiscoError(const JID &from, const Error *error, int context);
		bool handleDiscoSet(const IQ &iq);

	signals:
		void getDiscoInfo(const QString &server, const QString &node, DiscoHandler *dh);
		void getDiscoItem(const QString &server, const QString &node, DiscoHandler *dh);
		void finishSearch(QTreeWidgetItem *parent_item, const QStringList &entityList);
		void finishSearch(QTreeWidgetItem *parent_item, const QList<QStringList> &confList);
                void finishSearch(QTreeWidgetItem *parent_item, const jDiscoItem &disco_item);
                void finishSearch(QTreeWidgetItem *parent_item, const QList<jDiscoItem> &disco_item);
                void finishSearch(const QString &jid);
		void finishSelfSearch(QTreeWidgetItem *parent_item, const jDiscoItem &disco_item);

	private:
		jDiscoItem modifyItem(const jDiscoItem &disco_item, Disco::Identity *identity);
		QString m_service_type;
		jDiscoItem m_disco_item;
		QString m_category;
		QString m_type;
		QTreeWidgetItem *m_parent_item;
		QHash<QString, QString> m_name_library;

};

#endif //JSERVICEDISCOVERY_H
