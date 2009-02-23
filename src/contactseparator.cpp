/*
    contactSeparator

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


#include "contactseparator.h"

contactSeparator::contactSeparator(QTreeWidget *parent) : QTreeWidgetItem(parent, 3)
{
  setFlags(flags() & (~Qt::ItemIsSelectable));
}

contactSeparator::contactSeparator(QTreeWidgetItem *parent) :  QTreeWidgetItem(parent, 3)
{
  setFlags(flags() & (~Qt::ItemIsSelectable));
}

contactSeparator::~contactSeparator()
{

}

void contactSeparator::hideSeparator(bool hide)
{
  if ( hide )
  {
//		QTreeWidgetItem::setText(1, "");
//		if (this->columnCount() >= 1)
    setSizeHint(0, QSize(sizeHint(0).width(), 0));

//		if (this->columnCount() >= 2)
    setSizeHint(1, QSize(sizeHint(1).width(), 0));

//		if (this->columnCount() >= 3)
    setSizeHint(2, QSize(sizeHint(2).width(), 0));

//		if (this->columnCount() >= 4)
    setSizeHint(3, QSize(sizeHint(3).width(), 0));
  }
  else
  {
//		QTreeWidgetItem::setText(1, currentText);
    if (this->columnCount() >= 1)
      setSizeHint(0, QSize(sizeHint(0).width(), 16));

    if (this->columnCount() >= 2)
      setSizeHint(1, QSize(sizeHint(1).width(), 16));

    if (this->columnCount() >= 3)
      setSizeHint(2, QSize(sizeHint(2).width(), 16));

    if (this->columnCount() >= 4)
      setSizeHint(3, QSize(sizeHint(3).width(), 16));

  }
}

void contactSeparator::setText(int column, const QString &text)
{
  currentText = text;
  QTreeWidgetItem::setText(column, text);
}
