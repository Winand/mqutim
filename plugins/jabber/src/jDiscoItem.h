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

#ifndef JDISKOITEM_H
#define JDISKOITEM_H

#include "jDiscoIdentity.h"

class jDiscoItem
{
	public:
		jDiscoItem() {}
		//~jDiscoItem();
		void setName(const QString &name) {m_name = name;}
		void setJID(const QString &jid) {m_jid = jid;}
		void setNode(const QString &node) {m_node = node;}
		void setExpand(bool expand) {m_expand = expand;}
		void addIdentity(const jDiscoIdentity &identity) {m_identities << identity;}
		QString name() const {return m_name;}
		QString jid() const {return m_jid;}
		QString node() const {return m_node;}
		bool expand() const {return m_expand;}
		QList<jDiscoIdentity> identities() {return m_identities;}

	private:
		QString m_name, m_jid, m_node;
		QList<jDiscoIdentity> m_identities;
		bool m_expand;

};

#endif //JDISKOITEM_H
