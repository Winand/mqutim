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

#ifndef ABSTRACTSOUNDLAYER_H_
#define ABSTRACTSOUNDLAYER_H_
//#include <AudioOutput>
#include <QString>
#include <QProcess>
#include <QSound>
#include <QVector>
#ifdef HAVE_PHONON
	#include <phonon/phonon_export.h>
	#include <phonon/audiooutput.h>
	#include <phonon/mediaobject.h>
#endif
//#include </usr/include/KDE/Phonon/>
//#include </usr/include/KDE/Phonon/AudioOutput>
//#include </usr/include/KDE/Phonon/MediaSound>
#include "include/qutim/plugininterface.h"

using namespace qutim_sdk_0_2;

class AbstractSoundLayer
{
public:
	AbstractSoundLayer();
	virtual ~AbstractSoundLayer();
	static AbstractSoundLayer &instance();
	void loadProfile(const QString &profile_name);
	void enableSound(bool enable);
        void playSound(NotificationType event);
	void playSound(const QString &file_name);
	void testPlaySound(SoundEngineSystem sys, const QString &argument);
	void loadSettings();
        bool isActiveWindowIncomeSound();
private:
	SoundEngineSystem m_engine;
	bool m_enable_sound;
        bool m_active_window_income_sound;
	QVector<QString> m_sound_path;
	QString m_cmd;
	QString m_profile_name;
#ifdef HAVE_PHONON
//	DefaultSoundEngine m_default;
	QVector<Phonon::AudioOutput *> m_audio;
	QVector<Phonon::MediaObject *>   m_media;
#endif	
};

#endif /*ABSTRACTSOUNDLAYER_H_*/
