#ifndef PLUGINEVENTEATER_H_
#define PLUGINEVENTEATER_H_

#include "icqaccount.h"

class PluginEventEater
{	
public:
	static PluginEventEater &instance();
	void getEvent(const QList<void*> &event);
	void setAccountList(const QHash<QString, icqAccount*> &account_list )
	{
		m_icq_list = account_list;
	}
private:
	PluginEventEater();
	virtual ~PluginEventEater();
	QHash<QString, icqAccount *> m_icq_list;
	void setStatus(const QList<void *> &event);
	void restoreStatus(const QList<void *> &event);
};

#endif /*PLUGINEVENTEATER_H_*/
