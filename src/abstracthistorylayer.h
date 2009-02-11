/*
    Abstract History Layer

    Copyright (c) 2008 by Nigmatullin Ruslan <euroelessar@gmail.com>

 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************
*/

#ifndef ABSTRACTHISTORYLAYER_H_
#define ABSTRACTHISTORYLAYER_H_
#include <QDebug>
#include <QList>
#include <QDateTime>
#include "src/history/historyengine.h"
#include "history/historywindow.h"

using namespace qutim_sdk_0_2;

class AbstractHistoryLayer
{
public:
	AbstractHistoryLayer();
	virtual ~AbstractHistoryLayer();
	static AbstractHistoryLayer &instance();
	void loadProfile(const QString &profile_name);
	bool saveHistoryMessage(const HistoryItem &item);
	bool saveSystemMessage(const HistoryItem &item);
	void loadHistoryMessage(const TreeModelItem &item, const QDateTime &date_last);
	void closeHistoyWindow(const QString &identification);
	void openHistoryWindow(const TreeModelItem &item);
	void setContactHistoryName(const TreeModelItem &item);
        QString getContactHistoryName(const TreeModelItem &item);
	void loadSettings();        
private:
	HistoryEngine *m_engine;
	QHash <QString, HistoryWindow *> m_user_history_list;
	QHash <QString, QString> m_contact_history;
	QString m_profile_name;
	bool m_save_history;
	bool m_show_recent_messages;
	quint16 m_recent_messages_count;
};

#endif /*ABSTRACTHISTORYLAYER_H_*/
