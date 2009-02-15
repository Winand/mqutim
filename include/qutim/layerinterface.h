/*!
 * Plugin Interface
 *
 * @author EuroElessar
 * Copyright (c) 2009 by Nigmatullin Ruslan «euroelessar@gmail.com»
 * encoding: UTF-8
 *
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************
 */

#ifndef QUTIM_LAYERINTERFACE_H
#define QUTIM_LAYERINTERFACE_H

class QString;
class QWidget;

#include <qutim/plugininterface.h>
#include <QIODevice>

namespace qutim_sdk_0_2 {

class SoundEngineLayerInterface : LayerInterface
{
public:
    virtual void playSound(QIODevice *device) = 0;
    virtual QIODevice *grabSound() = 0;
};

class VideoEngineLayerInterface : LayerInterface
{
public:
    virtual void playVideo(QIODevice *device) = 0;
    virtual QIODevice *grabVideo() = 0;
};

class HistoryLayerInterface : LayerInterface
{
public:
    virtual void openWindow(const TreeModelItem &item);
    virtual bool storeMessage(const HistoryItem &item) = 0;
    virtual QList<HistoryItem> getMessages(const TreeModelItem &item, int max_num, const QDateTime &date_last) = 0;
};

class StatusLayerInterface : LayerInterface
{
public:
    virtual bool setStatusMessage(QString &status_message, bool &dshow) = 0;
};

class NotificationLayerInterface : LayerInterface
{
public:
    virtual void systemMessage(const TreeModelItem &item, const QString &message) = 0;
    virtual void userMessage(const TreeModelItem &item, const QString &message, NotificationType type) = 0;
};

class AntispamLayerInterface : LayerInterface
{
public:
    virtual bool checkForMessageValidation(const TreeModelItem &item, const QString &message,
                                           int message_type, bool special_status) = 0;
};

} //end namespace qutim_sdk_0_2

#endif // QUTIM_LAYERINTERFACE_H
