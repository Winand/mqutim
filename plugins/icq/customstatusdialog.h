/*
    customStatusDialog

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


#ifndef CUSTOMSTATUSDIALOG_H
#define CUSTOMSTATUSDIALOG_H

#include <QtGui>
#include "ui_customstatusdialog.h"


class customStatusDialog : public QDialog
{
  Q_OBJECT
public:
  customStatusDialog(const QString &, const QString &profile_name,QWidget *parent = 0);
  ~customStatusDialog();
  void setStatuses(int, const QList<QString> &);
  int status;
  QString statusCaption;
  QString statusMessage;
  void setCaption(const QString &t){ui.captionEdit->setText(t);}
  void setMessage(const QString &t){ui.textEdit->setPlainText(t);}
private slots:
  void on_iconList_currentItemChanged ( QListWidgetItem * current, QListWidgetItem * previous );
  void commitStatus();
  void on_textEdit_textChanged();
protected:
  void keyPressEvent(QKeyEvent *ev);
private:
  Ui::customStatusDialogClass ui;
  QList<QListWidgetItem *> itemList;
  QString getToolTip(int) const;
  int statusIndex;
  QString mineUin;
  QString m_profile_name;
};

#endif // CUSTOMSTATUSDIALOG_H
