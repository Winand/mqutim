/*
    jBuddy.cpp

    Copyright (c) 2008-2009 by Nikita Belov <zodiac.nv@gmail.com>
					Nigmatullin Ruslan <euroelessar@gmail.com>

 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************
*/

#include "jBuddy.h"

jBuddy::jBuddy(const QString &jid, const QString &group, const QString &path_to_avatars) 
	: m_plugin_system(jPluginSystem::instance()),
	m_path_to_avatars(path_to_avatars)
{
	m_jid = jid;
	m_group = group;
	count_resources = 0;
	max_priority_resource_value = -128;
	m_chat_state = ChatStateInactive;
}

jBuddy::~jBuddy()
{
}

QString jBuddy::getBaseToolTip(const jBuddy::ResourceInfo &info)
{
	QString html = "";
	QString status;
	
	if (!info.m_status_message.isEmpty())
		status = info.m_status_message;
	
	if (!status.isEmpty())
	{
		if (!info.m_xstatus.m_icon.isEmpty())
			html.append("<img src='" + jPluginSystem::instance().getIconFileName(info.m_xstatus.m_icon) + "'> ");
		html.append(QObject::tr("<font size='2'><b>Status text:</b> %1</font>").arg(status)+"<br/>");
	}

	QString client;
	if(info.m_client_name.isNull())
		client = "-";
	else
	{
		client = info.m_client_name+" "+info.m_client_version;
		if(!info.m_client_os.isNull() && !info.m_client_os.isEmpty())
			client+=" / "+info.m_client_os;
	}
	html.append(QObject::tr("<font size='2'><b>Possible client:</b> %1</font>").arg(client));
	return html;
}

void jBuddy::addResource(const QString & resource, int priority, Presence::PresenceType presence)
{
	if (!m_resources_info.contains(resource))
	{
		ResourceInfo info;
		info.m_name = resource;
		m_resources_info.insert(resource, info);
		count_resources++;
	}
	m_resources_info[resource].m_priority = priority;
	m_resources_info[resource].m_presence = presence;
	m_resources_info[resource].m_chat_state = ChatStateInactive;
	m_resources_info[resource].m_in_cl = false;
	                 
	if (priority >= max_priority_resource_value)
	{
		max_priority_resource = resource;
		max_priority_resource_value = priority;
	}
}

void jBuddy::delResource(const QString & resource)
{
	if(m_resources_info[resource].m_in_cl)
        {
		m_resources_info[resource].m_presence = Presence::Unavailable;
                m_resources_info[resource].m_priority = -129;
        }
	else
	{
		m_resources_info.remove(resource);
		count_resources--;
	}	

	if (resource == getMaxPriorityResource())
	{
		max_priority_resource = "";
		max_priority_resource_value = -128;
		newMaxPriorityResource();
	}
}

bool jBuddy::resourceExist(const QString & resource, bool hard)
{
	if(m_resources_info.contains(resource))
	{
		if(hard)
			return true;
		return m_resources_info[resource].m_presence != Presence::Unavailable;
	}
	return false;
}

void jBuddy::newMaxPriorityResource()
{
	ResourceInfo info;
	QHashIterator<QString, ResourceInfo> i(m_resources_info);
	while (i.hasNext())
	{
		i.next();
		info = i.value();
		if (info.m_priority >= max_priority_resource_value && info.m_presence != Presence::Unavailable)
		{
			max_priority_resource = i.key();
			max_priority_resource_value = info.m_priority;
		}
	}
}

QString jBuddy::getToolTip(const QString &resource)
{
	QString html="<table><tr><td>";
	if(m_name.isEmpty())
		if (resource.isEmpty())
			html.append(m_jid);
		else
			html.append("<b><i>My connection</i></b><br/>(resource: "+resource+")");
	else
		html.append("<b>"+m_name+"</b> ("+m_jid+")");
	QList<QString> resources;
	if (resource.isEmpty())
	{
		resources = m_resources_info.keys();
		switch(m_subscription_type)
		{
			case S10nNone:
			case S10nNoneOut:
			case S10nNoneIn:
			case S10nNoneOutIn:
				html.append("<br/>"+QObject::tr("<font size='2'><b>Authorization:</b> <i>None</i></font>"));
				break;
			case S10nTo:
			case S10nToIn:
				html.append("<br/>"+QObject::tr("<font size='2'><b>Authorization:</b> <i>To</i></font>"));
				break;
			case S10nFrom:
			case S10nFromOut:
				html.append("<br/>"+QObject::tr("<font size='2'><b>Authorization:</b> <i>From</i></font>"));
				break;
			case S10nBoth:
//				html.append("<br/>"+QObject::tr("<font size='2'><b>Authorization:</b> <i>Both</i></font>"));
				break;
		}
	}
	else
		resources << resource;
	foreach(const SimpleInfo &info, m_simple_info.values())
		if(!info.m_html.isEmpty())
			html.append("<br/>"+info.m_html);
	html.append("</td></tr>");
	for(int i=0;i<resources.size();i++)
	{
		ResourceInfo info = m_resources_info[resources[i]];
		if(info.m_presence==Presence::Unavailable || info.m_presence==Presence::Error
				|| info.m_presence==Presence::Invalid || info.m_presence==Presence::Probe)
			continue;
                html.append("<tr><td>");
		html.append("<img src='" + m_plugin_system.getStatusIconName(info.m_presence) + "'>");
		html.append("<b>"+info.m_name+"</b> ("+QString::number(info.m_priority)+")<br/>");
		html.append(getBaseToolTip(info));
		html.append("</td></tr>");
	}
	html.append("</table>");
	if(!m_avatar_hash.isEmpty())
	{
		html = "<table><tr><td>"+html+"</td><td><img src=\"";
		html.append(m_path_to_avatars+"/"+m_avatar_hash);
		html.append("\"/></td></tr></table>");
	}
	return html;
}

jBuddy::ResourceInfo *jBuddy::getResourceInfo(const QString &name)
{
	if(m_resources_info.contains(name))
		return &(m_resources_info[name]);
	else
		return 0;
}

QString jBuddy::getMaxPriorityResource()
{
	return max_priority_resource;
}

void jBuddy::setSimpleInfo(const QString &name, const SimpleInfo &info)
{
	m_simple_info[name] = info;
}

QString jBuddy::getGroup()
{
	return m_group;
}

int jBuddy::getCountResources()
{
	return count_resources;
}

Presence::PresenceType jBuddy::getMaxPriorityStatus()
{
	return (m_resources_info.contains(max_priority_resource)) ? m_resources_info[max_priority_resource].m_presence : Presence::Unavailable;
}
