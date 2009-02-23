/*****************************************************************************
    Abstract Sound Layer

    Copyright (c) 2008 by Nigmatullin Ruslan <euroelessar@gmail.com>

 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************
*****************************************************************************/

#include "abstractsoundlayer.h"
#include "pluginsystem.h"
#include <QSettings>
#include <QDebug>

AbstractSoundLayer::AbstractSoundLayer() : m_sound_path(static_cast<int>(EVENT_COUNT))
{
  m_engine=qutim_sdk_0_2::NoSound;
  m_cmd = "play \"%1\"";
  m_enable_sound=true;
  m_active_window_income_sound=false;
#ifdef HAVE_PHONON
  for (int i=0;i<10;i++)
  {
    Phonon::AudioOutput *audio = new Phonon::AudioOutput(Phonon::MusicCategory);
    m_audio.append(audio);
    Phonon::MediaObject *media = new Phonon::MediaObject();
    m_media.append(media);
    Phonon::createPath(media, audio);
  }
#endif
}

AbstractSoundLayer::~AbstractSoundLayer()
{
}
AbstractSoundLayer &AbstractSoundLayer::instance()
{
  static AbstractSoundLayer asl;
  return asl;
}
void AbstractSoundLayer::loadProfile(const QString &profile_name)
{
  m_profile_name  = profile_name;
  loadSettings();
}
void AbstractSoundLayer::enableSound(bool enable)
{
  m_enable_sound=enable;
}
void AbstractSoundLayer::playSound(NotificationType event)
{
  QString file_name=m_sound_path.value(static_cast<int>(event),"");//.value(event,"");
  if (file_name.isEmpty())
    return;
  playSound(file_name);
}
void AbstractSoundLayer::playSound(const QString &file_name)
{
  if (m_enable_sound)
    switch (m_engine)
    {
    case qutim_sdk_0_2::LibSound:
#if defined(Q_WS_WIN) || defined(Q_WS_MAC)
      QSound::play(file_name);
#endif
      break;
    case qutim_sdk_0_2::UserCommand:
      QProcess::startDetached(m_cmd.arg(file_name));
      break;
    case qutim_sdk_0_2::LibPhonon:
    {
#ifdef HAVE_PHONON
      int num = 0;
      int min_time = 0;
      for (int i=0;i<10;i++)
      {
        int time = m_media[i]->remainingTime()==0?0:1;
        time += m_media[i]->queue().size();
        if (i==0 || time<min_time)
        {
          num = i;
          min_time = time;
        }
      }
      if (m_media[num]->state()==Phonon::ErrorState || min_time<=0)
        m_media[num]->clearQueue();
      if (min_time>0)
        m_media[num]->enqueue(file_name);
      else
      {
        m_media[num]->setCurrentSource(file_name);
        m_media[num]->play();
      }
#endif
      break;
    }
    case qutim_sdk_0_2::PluginEngine:
      PluginSystem::instance().playSoundByPlugin(file_name);
      break;
    }
}
void AbstractSoundLayer::testPlaySound(SoundEngineSystem sys, const QString &argument)
{
  switch (sys)
  {
  case qutim_sdk_0_2::LibSound:
#if defined(Q_WS_WIN) || defined(Q_WS_MAC)
    QSound::play(argument);
#endif
    break;
  case qutim_sdk_0_2::UserCommand:
    QProcess::startDetached(argument);
    break;
  case qutim_sdk_0_2::LibPhonon:
  {
#ifdef HAVE_PHONON
    int num = 0;
    int min_time = 0;
    for (int i=0;i<10;i++)
    {
      int time = m_media[i]->remainingTime()==0?0:1;
      time += m_media[i]->queue().size();
      if (i==0 || time<min_time)
      {
        num = i;
        min_time = time;
      }
    }
    /*if(m_media[num]->state()==Phonon::ErrorState || min_time<=0)
    	m_media[num]->clearQueue();
    if(min_time>0)*/
    m_media[num]->enqueue(argument);
    m_media[num]->play();
    /*else
    {
    	m_media[num]->setCurrentSource(argument);
    	m_media[num]->play();
    }*/
#endif
    break;
  }
  case qutim_sdk_0_2::PluginEngine:
    PluginSystem::instance().playSoundByPlugin(argument);
    break;
  }
}
void AbstractSoundLayer::loadSettings()
{
  QSettings settings(QSettings::NativeFormat, QSettings::UserScope, "qutim/qutim."+m_profile_name, "profilesettings");
  settings.beginGroup("sounds");
  m_enable_sound = settings.value("enable",true).toBool();
  m_active_window_income_sound = settings.value("actwndincome", false).toBool();
  m_engine = static_cast<qutim_sdk_0_2::SoundEngineSystem>(settings.value("soundengine", 0).toInt());
  QString file_name;
  for (int i=0;i<qutim_sdk_0_2::EVENT_COUNT;i++)
  {
    file_name = settings.value("event"+QString::number(i), "-").toString();
    m_sound_path[i] = (file_name.at(0) == '+') ? file_name.mid(1) : "";
  }
  m_cmd = settings.value("command", "play \"%1\"").toString();
  if (m_cmd.indexOf("%1")<0)
    m_cmd+=" \"%1\"";
  settings.endGroup();
}

bool AbstractSoundLayer::isActiveWindowIncomeSound()
{
  return m_active_window_income_sound;
}
