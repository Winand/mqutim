/*
    AbstractStatusLayer

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

#include <QtopiaApplication>

#include "abstractstatus.h"
#include "statusdialog.h"

AbstractStatusLayer::AbstractStatusLayer()
    : m_icon_manager(IconManager::instance())
{
}

AbstractStatusLayer::~AbstractStatusLayer()
{

}

AbstractStatusLayer &AbstractStatusLayer::instance()
{
  static AbstractStatusLayer asl;
  return asl;
}

bool AbstractStatusLayer::setStatusMessage(QString &status_message, bool &dshow)
{
  StatusDialog status_dialog(m_profile_name);
  status_dialog.setStatusMessage(status_message);
  if ( QtopiaApplication::execDialog(&status_dialog) )
  {
    status_message = status_dialog.getStatusMessage();
    dshow = status_dialog.getDshowFlag();
    return true;
  }
  else
  {
    return false;
  }
}
