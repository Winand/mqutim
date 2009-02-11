/*
    AbstractAntiSpamLayer

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

#ifndef ABSTRACTANTISPAMLAYER_H_
#define ABSTRACTANTISPAMLAYER_H_
#include <QString>
#include <../include/qutim/plugininterface.h>
using namespace qutim_sdk_0_2;

class AbstractAntiSpamLayer
{
public:
	static AbstractAntiSpamLayer &instance();
	void loadProfile(const QString &profile_name);
	void loadSettings();
	bool checkForMessageValidation(const TreeModelItem &item,
			const QString &message,
			int message_type,
			bool special_status);
private:
	void notifyAboutBlock(const TreeModelItem &item, const QString &message);
	void answerToContact(const TreeModelItem &item, QString &message);
	bool checkForUrls(const QString &message);
	AbstractAntiSpamLayer();
	virtual ~AbstractAntiSpamLayer();
	QString m_profile_name;
	bool m_accept_from_cl_only;
	bool m_notify_on_block;
	bool m_block_authorization_from_nil;
	bool m_block_nil_urls;
	bool m_anti_spam_bot_enabled;
	QString m_bot_question;
	QString m_bot_answer;
	QString m_bot_after_answer;
	bool m_do_not_answer_if_invisible;
	QStringList m_banned_contacts;
};

#endif /*ABSTRACTANTISPAMLAYER_H_*/
