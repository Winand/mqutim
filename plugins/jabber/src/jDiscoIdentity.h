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

#ifndef JDISKOIDENTITY_H
#define JDISKOIDENTITY_H

enum jDiscoAction {LIST, EXEC, REGISTER, JOIN, SEARCH};

class jDiscoIdentity
{
	public:
		jDiscoIdentity() {};
		//jDiscoIdentity(const QString &name, const QString &category, const QString &type, jDiscoAction action)
		//{
		//	setName(name);
		//	setCategory(category);
		//	setType(type);
		//	setAction(action);
		//}
		//~jDiscoIdentity();
		void setName(const QString &name) {m_name = name;}
		void setCategory(const QString &category) {m_category = category;}
		void setType(const QString &type) {m_type = type;}
		void setAction(jDiscoAction action) {m_action = action;}
		QString name() {return m_name;}
		QString category() {return m_category;}
		QString type() {return m_type;}
		jDiscoAction action() {return m_action;}

	private:
		QString m_name, m_category, m_type;
		jDiscoAction m_action;

};

#endif //JDISKOIDENTITY_H
