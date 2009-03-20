/*
    jPluginSystem.cpp

    Copyright (c) 2008-2009 by Nikita Belov <zodiac.nv@gmail.com>

 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************
*/

#include "jPluginSystem.h"


jPluginSystem::jPluginSystem()
{
}

jPluginSystem &jPluginSystem::instance()
{
	static jPluginSystem jps;
	return jps;
}

void jPluginSystem::setJabberLayer(jLayer *jabber_layer)
{
	m_parent_layer = jabber_layer;
}

void jPluginSystem::init()
{
        m_mood_tr.insert("afraid",QObject::tr("Afraid"));
        m_mood_tr.insert("amazed",QObject::tr("Amazed"));
        m_mood_tr.insert("amorous",QObject::tr("Amorous"));
        m_mood_tr.insert("angry",QObject::tr("Angry"));
        m_mood_tr.insert("annoyed",QObject::tr("Annoyed"));
        m_mood_tr.insert("anxious",QObject::tr("Anxious"));
        m_mood_tr.insert("aroused",QObject::tr("Aroused"));
        m_mood_tr.insert("ashamed",QObject::tr("Ashamed"));
        m_mood_tr.insert("bored",QObject::tr("Bored"));
        m_mood_tr.insert("brave",QObject::tr("Brave"));
        m_mood_tr.insert("calm",QObject::tr("Calm"));
        m_mood_tr.insert("cautious",QObject::tr("Cautious"));
        m_mood_tr.insert("cold",QObject::tr("Cold"));
        m_mood_tr.insert("confident",QObject::tr("Confident"));
        m_mood_tr.insert("confused",QObject::tr("Confused"));
        m_mood_tr.insert("contemplative",QObject::tr("Contemplative"));
        m_mood_tr.insert("contented",QObject::tr("Contented"));
        m_mood_tr.insert("cranky",QObject::tr("Cranky"));
        m_mood_tr.insert("crazy",QObject::tr("Crazy"));
        m_mood_tr.insert("creative",QObject::tr("Creative"));
        m_mood_tr.insert("curious",QObject::tr("Curious"));
        m_mood_tr.insert("dejected",QObject::tr("Dejected"));
        m_mood_tr.insert("depressed",QObject::tr("Depressed"));
        m_mood_tr.insert("disappointed",QObject::tr("Disappointed"));
        m_mood_tr.insert("disgusted",QObject::tr("Disgusted"));
        m_mood_tr.insert("dismayed",QObject::tr("Dismayed"));
        m_mood_tr.insert("distracted",QObject::tr("Distracted"));
        m_mood_tr.insert("embarrassed",QObject::tr("Embarrassed"));
        m_mood_tr.insert("envious",QObject::tr("Envious"));
        m_mood_tr.insert("excited",QObject::tr("Excited"));
        m_mood_tr.insert("flirtatious",QObject::tr("Flirtatious"));
        m_mood_tr.insert("frustrated",QObject::tr("Frustrated"));
        m_mood_tr.insert("grateful",QObject::tr("Grateful"));
        m_mood_tr.insert("grieving",QObject::tr("Grieving"));
        m_mood_tr.insert("grumpy",QObject::tr("Grumpy"));
        m_mood_tr.insert("guilty",QObject::tr("Guilty"));
        m_mood_tr.insert("happy",QObject::tr("Happy"));
        m_mood_tr.insert("hopeful",QObject::tr("Hopeful"));
        m_mood_tr.insert("hot",QObject::tr("Hot"));
        m_mood_tr.insert("humbled",QObject::tr("Humbled"));
        m_mood_tr.insert("humiliated",QObject::tr("Humiliated"));
        m_mood_tr.insert("hungry",QObject::tr("Hungry"));
        m_mood_tr.insert("hurt",QObject::tr("Hurt"));
        m_mood_tr.insert("impressed",QObject::tr("Impressed"));
        m_mood_tr.insert("in_awe",QObject::tr("In awe"));
        m_mood_tr.insert("in_love",QObject::tr("In love"));
        m_mood_tr.insert("indignant",QObject::tr("Indignant"));
        m_mood_tr.insert("interested",QObject::tr("Interested"));
        m_mood_tr.insert("intoxicated",QObject::tr("Intoxicated"));
        m_mood_tr.insert("invincible",QObject::tr("Invincible"));
        m_mood_tr.insert("jealous",QObject::tr("Jealous"));
        m_mood_tr.insert("lonely",QObject::tr("Lonely"));
        m_mood_tr.insert("lost",QObject::tr("Lost"));
        m_mood_tr.insert("lucky",QObject::tr("Lucky"));
        m_mood_tr.insert("mean",QObject::tr("Mean"));
        m_mood_tr.insert("moody",QObject::tr("Moody"));
        m_mood_tr.insert("nervous",QObject::tr("Nervous"));
        m_mood_tr.insert("neutral",QObject::tr("Neutral"));
        m_mood_tr.insert("offended",QObject::tr("Offended"));
        m_mood_tr.insert("outraged",QObject::tr("Outraged"));
        m_mood_tr.insert("playful",QObject::tr("Playful"));
        m_mood_tr.insert("proud",QObject::tr("Proud"));
        m_mood_tr.insert("relaxed",QObject::tr("Relaxed"));
        m_mood_tr.insert("relieved",QObject::tr("Relieved"));
        m_mood_tr.insert("remorseful",QObject::tr("Remorseful"));
        m_mood_tr.insert("restless",QObject::tr("Restless"));
        m_mood_tr.insert("sad",QObject::tr("Sad"));
        m_mood_tr.insert("sarcastic",QObject::tr("Sarcastic"));
        m_mood_tr.insert("satisfied",QObject::tr("Satisfied"));
        m_mood_tr.insert("serious",QObject::tr("Serious"));
        m_mood_tr.insert("shocked",QObject::tr("Shocked"));
        m_mood_tr.insert("shy",QObject::tr("Shy"));
        m_mood_tr.insert("sick",QObject::tr("Sick"));
        m_mood_tr.insert("sleepy",QObject::tr("Sleepy"));
        m_mood_tr.insert("spontaneous",QObject::tr("Spontaneous"));
        m_mood_tr.insert("stressed",QObject::tr("Stressed"));
        m_mood_tr.insert("strong",QObject::tr("Strong"));
        m_mood_tr.insert("surprised",QObject::tr("Surprised"));
        m_mood_tr.insert("thankful",QObject::tr("Thankful"));
        m_mood_tr.insert("thirsty",QObject::tr("Thirsty"));
        m_mood_tr.insert("tired",QObject::tr("Tired"));
        m_mood_tr.insert("undefined",QObject::tr("Undefined"));
        m_mood_tr.insert("weak",QObject::tr("Weak"));
        m_mood_tr.insert("worried",QObject::tr("Worried"));
        m_activity_general_tr.insert("unknown",QObject::tr("Unknown"));
        m_activity_general_tr.insert("doing_chores",QObject::tr("Doing chores"));
        m_activity_tr["doing_chores"].insert("buying_groceries",QObject::tr("buying groceries"));
        m_activity_tr["doing_chores"].insert("cleaning",QObject::tr("cleaning"));
        m_activity_tr["doing_chores"].insert("cooking",QObject::tr("cooking"));
        m_activity_tr["doing_chores"].insert("doing_maintenance",QObject::tr("doing maintenance"));
        m_activity_tr["doing_chores"].insert("doing_the_dishes",QObject::tr("doing the dishes"));
        m_activity_tr["doing_chores"].insert("doing_the_laundry",QObject::tr("doing the laundry"));
        m_activity_tr["doing_chores"].insert("gardening",QObject::tr("gardening"));
        m_activity_tr["doing_chores"].insert("running_an_errand",QObject::tr("running an errand"));
        m_activity_tr["doing_chores"].insert("walking_the_dog",QObject::tr("walking the dog"));
        m_activity_general_tr.insert("drinking",QObject::tr("Drinking"));
        m_activity_tr["drinking"].insert("having_a_beer",QObject::tr("having a beer"));
        m_activity_tr["drinking"].insert("having_coffee",QObject::tr("having coffee"));
        m_activity_tr["drinking"].insert("having_tea",QObject::tr("having tea"));
        m_activity_general_tr.insert("eating",QObject::tr("Eating"));
        m_activity_tr["eating"].insert("having_a_snack",QObject::tr("having a snack"));
        m_activity_tr["eating"].insert("having_breakfast",QObject::tr("having breakfast"));
        m_activity_tr["eating"].insert("having_dinner",QObject::tr("having dinner"));
        m_activity_tr["eating"].insert("having_lunch",QObject::tr("having lunch"));
        m_activity_general_tr.insert("exercising",QObject::tr("Exercising"));
        m_activity_tr["exercising"].insert("cycling",QObject::tr("cycling"));
        m_activity_tr["exercising"].insert("dancing",QObject::tr("dancing"));
        m_activity_tr["exercising"].insert("hiking",QObject::tr("hiking"));
        m_activity_tr["exercising"].insert("jogging",QObject::tr("jogging"));
        m_activity_tr["exercising"].insert("playing_sports",QObject::tr("playing sports"));
        m_activity_tr["exercising"].insert("running",QObject::tr("running"));
        m_activity_tr["exercising"].insert("skiing",QObject::tr("skiing"));
        m_activity_tr["exercising"].insert("swimming",QObject::tr("swimming"));
        m_activity_tr["exercising"].insert("working_out",QObject::tr("working out"));
        m_activity_general_tr.insert("grooming",QObject::tr("Grooming"));
        m_activity_tr["grooming"].insert("at_the_spa",QObject::tr("at the spa"));
        m_activity_tr["grooming"].insert("brushing_teeth",QObject::tr("brushing teeth"));
        m_activity_tr["grooming"].insert("getting_a_haircut",QObject::tr("getting a haircut"));
        m_activity_tr["grooming"].insert("shaving",QObject::tr("shaving"));
        m_activity_tr["grooming"].insert("taking_a_bath",QObject::tr("taking a bath"));
        m_activity_tr["grooming"].insert("taking_a_shower",QObject::tr("taking a shower"));
        m_activity_general_tr.insert("having_appointment",QObject::tr("Having appointment"));
        m_activity_general_tr.insert("inactive",QObject::tr("Inactive"));
        m_activity_tr["inactive"].insert("day_off",QObject::tr("day off"));
        m_activity_tr["inactive"].insert("hanging_out",QObject::tr("hanging out"));
        m_activity_tr["inactive"].insert("hiding",QObject::tr("hiding"));
        m_activity_tr["inactive"].insert("on_vacation",QObject::tr("on vacation"));
        m_activity_tr["inactive"].insert("praying",QObject::tr("praying"));
        m_activity_tr["inactive"].insert("scheduled_holiday",QObject::tr("scheduled holiday"));
        m_activity_tr["inactive"].insert("sleeping",QObject::tr("sleeping"));
        m_activity_tr["inactive"].insert("thinking",QObject::tr("thinking"));
        m_activity_general_tr.insert("relaxing",QObject::tr("Relaxing"));
        m_activity_tr["relaxing"].insert("fishing",QObject::tr("fishing"));
        m_activity_tr["relaxing"].insert("gaming",QObject::tr("gaming"));
        m_activity_tr["relaxing"].insert("going_out",QObject::tr("going out"));
        m_activity_tr["relaxing"].insert("partying",QObject::tr("partying"));
        m_activity_tr["relaxing"].insert("reading",QObject::tr("reading"));
        m_activity_tr["relaxing"].insert("rehearsing",QObject::tr("rehearsing"));
        m_activity_tr["relaxing"].insert("shopping",QObject::tr("shopping"));
        m_activity_tr["relaxing"].insert("smoking",QObject::tr("smoking"));
        m_activity_tr["relaxing"].insert("socializing",QObject::tr("socializing"));
        m_activity_tr["relaxing"].insert("sunbathing",QObject::tr("sunbathing"));
        m_activity_tr["relaxing"].insert("watching_tv",QObject::tr("watching TV"));
        m_activity_tr["relaxing"].insert("watching_a_movie",QObject::tr("watching a movie"));
        m_activity_general_tr.insert("talking",QObject::tr("Talking"));
        m_activity_tr["talking"].insert("in_real_life",QObject::tr("in real life"));
        m_activity_tr["talking"].insert("on_the_phone",QObject::tr("on the phone"));
        m_activity_tr["talking"].insert("on_video_phone",QObject::tr("on video phone"));
        m_activity_general_tr.insert("traveling",QObject::tr("Traveling"));
        m_activity_tr["traveling"].insert("commuting",QObject::tr("commuting"));
        m_activity_tr["traveling"].insert("cycling",QObject::tr("cycling"));
        m_activity_tr["traveling"].insert("driving",QObject::tr("driving"));
        m_activity_tr["traveling"].insert("in_a_car",QObject::tr("in a car"));
        m_activity_tr["traveling"].insert("on_a_bus",QObject::tr("on a bus"));
        m_activity_tr["traveling"].insert("on_a_plane",QObject::tr("on a plane"));
        m_activity_tr["traveling"].insert("on_a_train",QObject::tr("on a train"));
        m_activity_tr["traveling"].insert("on_a_trip",QObject::tr("on a trip"));
        m_activity_tr["traveling"].insert("walking",QObject::tr("walking"));
        m_activity_general_tr.insert("working",QObject::tr("Working"));
        m_activity_tr["working"].insert("coding",QObject::tr("coding"));
        m_activity_tr["working"].insert("in_a_meeting",QObject::tr("in a meeting"));
        m_activity_tr["working"].insert("studying",QObject::tr("studying"));
        m_activity_tr["working"].insert("writing",QObject::tr("writing"));
}

void jPluginSystem::updateStatusIcons()
{
	m_parent_layer->getMainPluginSystemPointer()->updateStatusIcons();	
}

bool jPluginSystem::addItemToContactList(TreeModelItem Item, QString name)
{
	bool result = m_parent_layer->getMainPluginSystemPointer()->addItemToContactList(Item, name);
	return result;
}

bool jPluginSystem::removeItemFromContactList(TreeModelItem Item)
{
	return m_parent_layer->getMainPluginSystemPointer()->removeItemFromContactList(Item);
}

bool jPluginSystem::moveItemInContactList(TreeModelItem OldItem, TreeModelItem NewItem)
{
	return m_parent_layer->getMainPluginSystemPointer()->moveItemInContactList(OldItem, NewItem);
}

bool jPluginSystem::setContactItemName(TreeModelItem Item, QString name)
{
	return m_parent_layer->getMainPluginSystemPointer()->setContactItemName(Item, name);
}

bool jPluginSystem::setContactItemIcon(TreeModelItem Item, QIcon icon, int position)
{
	return m_parent_layer->getMainPluginSystemPointer()->setContactItemIcon(Item, icon, position);
}

bool jPluginSystem::setContactItemRow(TreeModelItem Item, QList<QVariant> row, int position)
{
	return m_parent_layer->getMainPluginSystemPointer()->setContactItemRow(Item, row, position);
}

bool jPluginSystem::setContactItemStatus(TreeModelItem Item, QIcon icon, QString text, int mass)
{
	return m_parent_layer->getMainPluginSystemPointer()->setContactItemStatus(Item, icon, text, mass);
}

bool jPluginSystem::setStatusMessage(QString &status_message, bool &dshow)
{
	return m_parent_layer->getMainPluginSystemPointer()->setStatusMessage(status_message, dshow);
}

void jPluginSystem::addMessageFromContact(const TreeModelItem &item, const QString &message, const QDateTime &message_date)
{
	m_parent_layer->getMainPluginSystemPointer()->addMessageFromContact(item, message, message_date);
}

void jPluginSystem::addServiceMessage(const TreeModelItem &item, const QString &message)
{
	m_parent_layer->getMainPluginSystemPointer()->addServiceMessage(item, message);
}

void jPluginSystem::addImage(const TreeModelItem &item, const QByteArray &image_raw)
{
	m_parent_layer->getMainPluginSystemPointer()->addImage(item, image_raw);
}

void jPluginSystem::contactTyping(const TreeModelItem &item, bool typing)
{
	m_parent_layer->getMainPluginSystemPointer()->contactTyping(item, typing);
}

void jPluginSystem::messageDelievered(const TreeModelItem &item, int position)
{
	m_parent_layer->getMainPluginSystemPointer()->messageDelievered(item, position);
}

bool jPluginSystem::checkForMessageValidation(const TreeModelItem &item, const QString &message, int message_type, bool special_status)
{
	return m_parent_layer->getMainPluginSystemPointer()->checkForMessageValidation(item, message, message_type, special_status);
}

QString jPluginSystem::getIconFileName(const QString &icon_name)
{
	return m_parent_layer->getMainPluginSystemPointer()->getIconFileName(icon_name);
}

QIcon jPluginSystem::getIcon(const QString &icon_name)
{
	return m_parent_layer->getMainPluginSystemPointer()->getIcon(icon_name);
}

QString jPluginSystem::getStatusIconFileName(const QString &icon_name, const QString &default_path)
{
	return m_parent_layer->getMainPluginSystemPointer()->getStatusIconFileName(icon_name, default_path);
}

QIcon jPluginSystem::getStatusIcon(const QString & icon_name, const QString & default_path)
{
	return m_parent_layer->getMainPluginSystemPointer()->getStatusIcon(icon_name, default_path);	
}

void jPluginSystem::setAccountIsOnline(const TreeModelItem &Item, bool online)
{
	m_parent_layer->getMainPluginSystemPointer()->setAccountIsOnline(Item, online);	
}

void jPluginSystem::createChat(const TreeModelItem &item)
{
	m_parent_layer->getMainPluginSystemPointer()->createChat(item);	
}

void jPluginSystem::notifyAboutBirthDay(const TreeModelItem &item)
{
	m_parent_layer->getMainPluginSystemPointer()->notifyAboutBirthDay(item);	
}

void jPluginSystem::systemNotifiacation(const TreeModelItem &item, const QString &message)
{
	m_parent_layer->getMainPluginSystemPointer()->systemNotifiacation(item, message);	
}

void jPluginSystem::customNotifiacation(const TreeModelItem &item, const QString &message)
{
	m_parent_layer->getMainPluginSystemPointer()->customNotifiacation(item, message);	
}

void jPluginSystem::getQutimVersion(quint8 &major, quint8 &minor, quint8 &secminor, quint16 &svn)
{
	m_parent_layer->getMainPluginSystemPointer()->getQutimVersion(major, minor, secminor, svn);	
}

void jPluginSystem::createConference(const QString &protocol_name, const QString &conference_name, const QString &account_name)
{
	m_parent_layer->getMainPluginSystemPointer()->createConference(protocol_name, conference_name, account_name);	
}

void jPluginSystem::addMessageToConference(const QString &protocol_name, const QString &conference_name, const QString &account_name, const QString &from, const QString &message, const QDateTime &date, bool history)
{
	m_parent_layer->getMainPluginSystemPointer()->addMessageToConference(protocol_name, conference_name, account_name, from, message, date, history);	
}

void jPluginSystem::changeOwnConferenceNickName(const QString &protocol_name, const QString &conference_name, const QString &account_name, const QString &nickname)
{
	m_parent_layer->getMainPluginSystemPointer()->changeOwnConferenceNickName(protocol_name, conference_name, account_name, nickname);	
}

void jPluginSystem::addConferenceItem(const QString &protocol_name, const QString &conference_name, const QString &account_name, const QString &nickname)
{
        m_parent_layer->getMainPluginSystemPointer()->addConferenceItem(protocol_name, conference_name, account_name, nickname);
}

void jPluginSystem::removeConferenceItem(const QString &protocol_name, const QString &conference_name, const QString &account_name, const QString &nickname)
{
	m_parent_layer->getMainPluginSystemPointer()->removeConferenceItem(protocol_name, conference_name, account_name, nickname);	
}

void jPluginSystem::renameConferenceItem(const QString &protocol_name, const QString &conference_name, const QString &account_name, const QString &nickname, const QString &new_nickname)
{
	m_parent_layer->getMainPluginSystemPointer()->renameConferenceItem(protocol_name, conference_name, account_name, nickname, new_nickname);	
}

void jPluginSystem::setConferenceItemStatus(const QString &protocol_name, const QString &conference_name, const QString &account_name, const QString &nickname, const QIcon &icon, const QString &status, int mass)
{
	m_parent_layer->getMainPluginSystemPointer()->setConferenceItemStatus(protocol_name, conference_name, account_name, nickname, icon, status, mass);	
}

void jPluginSystem::setConferenceItemIcon(const QString &protocol_name, const QString &conference_name, const QString &account_name, const QString &nickname, const QIcon &icon, int position)
{
	m_parent_layer->getMainPluginSystemPointer()->setConferenceItemIcon(protocol_name, conference_name, account_name, nickname, icon, position);	
}

void jPluginSystem::setConferenceItemRole(const QString &protocol_name, const QString &conference_name, const QString &account_name, const QString &nickname, const QIcon &icon, const QString &role, int mass)
{
	m_parent_layer->getMainPluginSystemPointer()->setConferenceItemRole(protocol_name, conference_name, account_name, nickname, icon, role, mass);	
}

void jPluginSystem::setItemVisible(const TreeModelItem &item, bool visible)
{
	m_parent_layer->getMainPluginSystemPointer()->setItemVisible(item, visible);	
}

void jPluginSystem::setItemInvisible(const TreeModelItem &item, bool invisible)
{
	m_parent_layer->getMainPluginSystemPointer()->setItemInvisible(item, invisible);	
}

void jPluginSystem::setConferenceTopic(const QString &protocol_name, const QString &conference_name, const QString &account_name, const QString &topic)
{
	m_parent_layer->getMainPluginSystemPointer()->setConferenceTopic(protocol_name, conference_name, account_name, topic);
}

void jPluginSystem::addSystemMessageToConference(const QString &protocol_name, const QString &conference_name, const QString &account_name, const QString &message, const QDateTime &date, bool history)
{
	m_parent_layer->getMainPluginSystemPointer()->addSystemMessageToConference(protocol_name, conference_name, account_name, message, date, history);
}

bool jPluginSystem::changeChatWindowID(const TreeModelItem &item, const QString &id)
{
    return m_parent_layer->getMainPluginSystemPointer()->changeChatWindowID(item, id);
}

QIcon jPluginSystem::getStatusIcon(int presence)
{
	QString name="connecting";
	switch(presence)
	{
	case Presence::Available:
		name = "online";
		break;
	case Presence::Chat:
		name = "ffc";
		break;
	case Presence::Away:
		name = "away";
		break;
	case Presence::DND:
		name = "dnd";
		break;
	case Presence::XA:
		name = "na";
		break;
	case Presence::Probe:
	case Presence::Error:
	case Presence::Invalid:
	case Presence::Unavailable:
		name = "offline";
		break;
	}	
	if(!m_icons_hash.contains(name))
		m_icons_hash.insert(name,m_parent_layer->getMainPluginSystemPointer()->getStatusIcon(name,"jabber"));
	return m_icons_hash.value(name);
}

QString jPluginSystem::getStatusIconName(int presence)
{
	QString name="connecting";
	switch(presence)
	{
	case Presence::Available:
		name = "online";
		break;
	case Presence::Chat:
		name = "ffc";
		break;
	case Presence::Away:
		name = "away";
		break;
	case Presence::DND:
		name = "dnd";
		break;
	case Presence::XA:
		name = "na";
		break;
	case Presence::Probe:
	case Presence::Error:
	case Presence::Invalid:
	case Presence::Unavailable:
		name = "offline";
		break;
	}
	if(!m_icons_name_hash.contains(name))
		m_icons_name_hash.insert(name,m_parent_layer->getMainPluginSystemPointer()->getStatusIconFileName(name,"jabber"));
	return m_icons_name_hash.value(name);
}
