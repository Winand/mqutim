#include "plugineventeater.h"
#include "icqlayer.h"

using namespace qutim_sdk_0_2;

PluginEventEater::PluginEventEater()
{
}

PluginEventEater::~PluginEventEater()
{
}

PluginEventEater &PluginEventEater::instance()
{
	static PluginEventEater pee;
	return pee;
}

void PluginEventEater::getEvent(const QList<void*> &event)
{
	if ( event.count() )
	{
		MessageToProtocolEventType type = *(MessageToProtocolEventType*)event.at(0);
		switch(type)
		{
		case SetStatus:
			setStatus(event);
			break;
		case RestoreStatus:
			restoreStatus(event);
			break;
		default:;
		}
	}
}

void PluginEventEater::setStatus(const QList<void *> &event)
{
	if ( event.count() > 3)
	{
		QStringList account_list = *(QStringList*)event.at(1);
		if ( !account_list.count() )
		{
			foreach (icqAccount *account, m_icq_list)
			{
				account->setStatusFromPlugin(*(accountStatus*)event.at(2), *(QString*)event.at(3));
				if ( event.count() > 6)
				{
					account->setXstatusFromPlugin(*(int*)event.at(4), *(QString*)event.at(5), *(QString*)event.at(6));
				}
			}
		}
		else{
			foreach(const QString &account, account_list)
			{
				icqAccount *paccount = m_icq_list.value(account);
				if ( paccount )
				{
					paccount->setStatusFromPlugin(*(accountStatus*)event.at(2), *(QString*)event.at(3));
					if ( event.count() > 6)
					{
						paccount->setXstatusFromPlugin(*(int*)event.at(4), *(QString*)event.at(5), *(QString*)event.at(6));
					}
				}
			}
		}
	}
}

void PluginEventEater::restoreStatus(const QList<void *> &event)
{
	if ( event.count() > 3)
	{
		QStringList account_list = *(QStringList*)event.at(1);
		if ( !account_list.count() )
		{
			foreach (icqAccount *account, m_icq_list)
			{
				account->restoreStatusFromPlugin();
				account->restoreXstatusFromPlugin();
			}
		}
		else{
			foreach(const QString &account, account_list)
			{
				icqAccount *paccount = m_icq_list.value(account);
				if ( paccount )
				{
					paccount->restoreStatusFromPlugin();
					paccount->restoreXstatusFromPlugin();
				}
			}
		}
	}
}
