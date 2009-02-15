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

#include "jServiceDiscovery.h"

jServiceDiscovery::jServiceDiscovery(const QString &serviceType, QTreeWidgetItem *item)
{
	m_service_type = serviceType;
	m_parent_item = item;
}

jServiceDiscovery::~jServiceDiscovery()
{
}

void jServiceDiscovery::search(const QString &jid, const QString &category, const QString &type, const QString &node)
{
	m_category = category;
	m_type = type;
	if (m_category.isEmpty() || m_type.isEmpty())
		emit getDiscoInfo(jid, node, (DiscoHandler*) this);
	else
		emit getDiscoItem(jid, node, (DiscoHandler*) this);
}

void jServiceDiscovery::search(const jDiscoItem &disco_item)
{
	m_disco_item = disco_item;
	if (m_disco_item.identities().isEmpty())
		emit getDiscoInfo(m_disco_item.jid(), m_disco_item.node(), (DiscoHandler*) this);
	else
		emit getDiscoItem(m_disco_item.jid(), m_disco_item.node(), (DiscoHandler*) this);
}

void jServiceDiscovery::handleDiscoInfo(const JID &from, const Disco::Info &info, int context)
{
	/*if (m_disco_item.identities().isEmpty())
	{
		m_disco_item.setJID(jProtocol::fromStd(from.full()));
		m_disco_item.setNode(jProtocol::fromStd(info.node()));
		m_disco_item.setName(m_name_library.value(m_disco_item.jid()+m_disco_item.node()));
		foreach (Disco::Identity *identity, info.identities())
		{
			m_disco_item = modifyItem(m_disco_item, identity);
		}
		if (m_disco_item.expand())
			emit getDiscoItem(jProtocol::fromStd(from.full()), jProtocol::fromStd(info.node()), (DiscoHandler*) this);
		if (m_service_type == "proxy")
			emit finishSelfSearch(m_disco_item);
		else
			emit finishSelfSearch(m_parent_item, m_disco_item);
	}
	else
	{
		foreach (Disco::Identity *identity, info.identities())
		{
			jDiscoItem disco_item;
			disco_item.setJID(jProtocol::fromStd(from.full()));
			disco_item.setNode(jProtocol::fromStd(info.node()));
			disco_item.setName(m_name_library.value(disco_item.jid()+disco_item.node()));
			if (m_service_type == "conference")
			{
				if (jProtocol::fromStd(identity->category()) == "conference"  || jProtocol::fromStd(identity->category()) == "server")
				{
					disco_item = modifyItem(disco_item, identity);
					emit finishSearch(m_parent_item, disco_item);
				}
			}
			else if (m_service_type == "gateway")
			{
				if (jProtocol::fromStd(identity->category()) == "gateway" || jProtocol::fromStd(identity->category()) == "server" || jProtocol::fromStd(identity->category()) == "automation")
				{
					disco_item = modifyItem(disco_item, identity);
					emit finishSearch(m_parent_item, disco_item);
				}
			}
			if (m_service_type == "proxy")
			{
				if (jProtocol::fromStd(identity->category()) == "proxy" && jProtocol::fromStd(identity->type()) == "bytestreams")
				{
					disco_item = modifyItem(disco_item, identity);
					emit finishSearch(m_parent_item, disco_item);
				}
			}
			else
			{
				disco_item = modifyItem(disco_item, identity);
				emit finishSearch(m_parent_item, disco_item);
			}
		}
	}*/
	//--------------------------------------------------------------
	if (m_service_type == "conference")
	{
                if(!m_parent_item)
                    return;
		if (m_category.isEmpty() || m_type.isEmpty())
		{
			QString name;
			foreach (Disco::Identity *identity, info.identities())
			{
				m_category = jProtocol::fromStd(identity->category());
				m_type = jProtocol::fromStd(identity->type());
				name = jProtocol::fromStd(identity->name());
				if (m_category == "server")
					break;
			}
			QStringList entity_list;
			entity_list << name << ""<<  m_category << m_type << jProtocol::fromStd(info.node());
			if (jProtocol::fromStd(from.username()).isEmpty())
			{
				emit getDiscoItem(jProtocol::fromStd(from.full()), jProtocol::fromStd(info.node()), (DiscoHandler*) this);
			}
			emit finishSearch(m_parent_item, entity_list);
		}
		else
		{
			foreach (Disco::Identity *identity, info.identities())
			{
				if (jProtocol::fromStd(identity->category()) == "conference" && jProtocol::fromStd(identity->type()) == "text" || jProtocol::fromStd(identity->category()) == "server")
				{
					QStringList entity_list;
					entity_list << jProtocol::fromStd(identity->name()) << jProtocol::fromStd(from.full()) << jProtocol::fromStd(identity->category()) << jProtocol::fromStd(identity->type()) << jProtocol::fromStd(info.node());
					emit finishSearch(m_parent_item, entity_list);
				}
			}
		}
	}
	else if (m_service_type == "gateway")
	{
                if(!m_parent_item)
                    return;
		if (m_category.isEmpty() || m_type.isEmpty())
		{
			QString name;
			foreach (Disco::Identity *identity, info.identities())
			{
				m_category = jProtocol::fromStd(identity->category());
				m_type = jProtocol::fromStd(identity->type());
				name = jProtocol::fromStd(identity->name());
				if (m_category == "server")
					break;
			}
			emit getDiscoItem(jProtocol::fromStd(from.full()), "", (DiscoHandler*) this);
			QStringList entity_list;
			entity_list << name << "" << m_category << m_type << jProtocol::fromStd(info.node());
			emit finishSearch(m_parent_item, entity_list);
		}
		else
		{
			if (m_category == "server")
				foreach (Disco::Identity *identity, info.identities())
				{
					if (jProtocol::fromStd(identity->category()) == "gateway" || jProtocol::fromStd(identity->category()) == "server")
					{
						QStringList entity_list;
						entity_list << jProtocol::fromStd(identity->name()) << jProtocol::fromStd(from.full()) << jProtocol::fromStd(identity->category()) << jProtocol::fromStd(identity->type()) << jProtocol::fromStd(info.node());
						emit finishSearch(m_parent_item, entity_list);
					}
				}
			else
				foreach (Disco::Identity *identity, info.identities())
				{
					QStringList entity_list;
					entity_list << jProtocol::fromStd(identity->name()) << jProtocol::fromStd(from.full()) << jProtocol::fromStd(identity->category()) << jProtocol::fromStd(identity->type()) << jProtocol::fromStd(info.node());
					emit finishSearch(m_parent_item, entity_list);
				}
		}
	}
	else if (m_service_type == "proxy")
		{
		if (m_category.isEmpty() || m_type.isEmpty())
		{
			foreach (Disco::Identity *identity, info.identities())
			{
				m_category = jProtocol::fromStd(identity->category());
				m_type = jProtocol::fromStd(identity->type());
				if (m_category == "server")
					break;
			}
			emit getDiscoItem(jProtocol::fromStd(from.full()), "", (DiscoHandler*) this);
		}
		else
		{
			foreach (Disco::Identity *identity, info.identities())
			{
				if (jProtocol::fromStd(identity->category()) == "proxy" && jProtocol::fromStd(identity->type()) == "bytestreams")
				{
					emit finishSearch(jProtocol::fromStd(from.full()));
				}
			}
		}
	}
}

jDiscoItem jServiceDiscovery::modifyItem(const jDiscoItem &disco_item, Disco::Identity *identity)
{
	jDiscoItem di = disco_item;
	jDiscoIdentity disco_identity;
	disco_identity.setName(jProtocol::fromStd(identity->name()));
	disco_identity.setCategory(jProtocol::fromStd(identity->category()));
	disco_identity.setType(jProtocol::fromStd(identity->type()));
	bool expand = false;
	if (disco_identity.category() == "automation")
	{
		if (disco_identity.type() == "command-list")
			expand = true;
		else
		{
			disco_identity.setAction(EXEC);
		}
	}
	else if (disco_identity.category() == "directory")
	{
		disco_identity.setAction(SEARCH);
	}
	else if (disco_identity.category() == "gateway")
	{
		disco_identity.setAction(REGISTER);
		expand = true;
	}
	else if (disco_identity.category() == "conference")
	{
		if (disco_identity.type() == "irc")
			disco_identity.setAction(JOIN);
		else if (disco_identity.type() == "text")
			if (jProtocol::fromStd(JID(jProtocol::toStd(di.name())).username()).isEmpty())
				expand = true;
			else if (jProtocol::fromStd(JID(jProtocol::toStd(di.name())).resource()).isEmpty())
			{
				disco_identity.setAction(JOIN);
				expand = true;
			}
	}
	else if (disco_identity.category() == "proxy")
	{

	}
	else if (disco_identity.category() == "server")
	{
		expand = true;
	}
	di.addIdentity(disco_identity);
	di.setExpand(di.expand() | expand);
	return di;
}

void jServiceDiscovery::handleDiscoItems(const JID &from, const Disco::Items &items, int context)
{
	/*if (m_service_type == "conference")
	{
		bool isConf = false, isIRC = false;
		foreach (jDiscoIdentity identity, m_disco_item.identities())
			if (identity.category() == "conference")
			{
				isConf = true;
				//if (identity.type() == "irc")
				//	isIRC = true;
			}
		if (!isConf || isIRC)
			return;
		QList<jDiscoItem> conf_list;
		foreach (Disco::Item *item, items.items())
		{
			jDiscoItem disco_item;
			jDiscoIdentity disco_identity;
			disco_identity.setName(jProtocol::fromStd(item->name()));
			disco_identity.setCategory("conference");
			disco_identity.setType("text");
			if (jProtocol::fromStd(from.username()).isEmpty())
			{
				disco_identity.setAction(JOIN);
				disco_item.setExpand(true);
			}
			disco_item.addIdentity(disco_identity);
			conf_list << disco_item;
		}
		emit finishSearch(m_parent_item, conf_list);
	}
	else
	{
		foreach (Disco::Item *item, items.items())
		{
			m_name_library.insert(jProtocol::fromStd(item->jid().full())+jProtocol::fromStd(item->node()), jProtocol::fromStd(item->name()));
			emit getDiscoInfo(jProtocol::fromStd(item->jid().full()), jProtocol::fromStd(item->node()), (DiscoHandler*) this);
		}
	}*/
	//------------------------------------------------------
	if (m_service_type == "conference")
	{
		if (m_category == "conference" && m_type == "text")
		{
			if(!m_parent_item)
                                return;
			QList<QStringList> conf_list;
			foreach (Disco::Item *item, items.items())
			{
				QStringList entity_list;
				entity_list << jProtocol::fromStd(item->name()) << jProtocol::fromStd(item->jid().full()) << m_category << m_type << jProtocol::fromStd(item->node());
				conf_list << entity_list;
			}
			emit finishSearch(m_parent_item, conf_list);
		}
		else
			foreach (Disco::Item *item, items.items())
			{
				emit getDiscoInfo(jProtocol::fromStd(item->jid().full()), jProtocol::fromStd(item->node()), (DiscoHandler*) this);
			}
	}
	else
	{
		foreach (Disco::Item *item, items.items())
		{
			emit getDiscoInfo(jProtocol::fromStd(item->jid().full()), jProtocol::fromStd(item->node()), (DiscoHandler*) this);
		}
	}
}

void jServiceDiscovery::handleDiscoError(const JID &from, const Error *error, int context)
{

}

bool jServiceDiscovery::handleDiscoSet(const IQ &iq)
{
	return false;
}
