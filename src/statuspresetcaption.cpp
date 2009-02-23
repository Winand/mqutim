/*
    StatusPresetCaption

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

#include "statuspresetcaption.h"

StatusPresetCaption::StatusPresetCaption(QWidget *parent)
    : QDialog(parent)
{
  ui.setupUi(this);
  setFixedSize(size());
  move(desktopCenter());
}

StatusPresetCaption::~StatusPresetCaption()
{

}

QPoint StatusPresetCaption::desktopCenter()
{
  QDesktopWidget desktop;
  return QPoint(desktop.width() / 2 - size().width() / 2, desktop.height() / 2 - size().height() / 2);
}

void StatusPresetCaption::on_presetEdit_textChanged(const QString &text)
{
  ui.okButton->setEnabled(!text.isEmpty());
}
