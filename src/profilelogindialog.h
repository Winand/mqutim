/*
    ProfileLoginDialog

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

#ifndef PROFILELOGINDIALOG_H
#define PROFILELOGINDIALOG_H

#include <QtGui/QDialog>
#include <QSettings>
#include <QDir>
#include <QMessageBox>
#include "ui_profilelogindialog.h"

class ProfileLoginDialog : public QDialog
{
    Q_OBJECT

public:
    ProfileLoginDialog(QWidget *parent = 0);
    ~ProfileLoginDialog();
    bool isNewProfile() const { return m_new_registered_profile; }
    QString getProfileName() const { return m_current_profile; }
    
protected:
  void keyPressEvent(QKeyEvent *ev);  

private slots:
	void on_passwordEdit_textChanged(const QString &) { enableOrDisableSignIn(); }
	void on_nameComboBox_editTextChanged(const QString &);
	void signIn();
	void on_deleteButton_clicked();
private:
	void saveData();
	void loadData();
	void saveProfileSettings(const QString &);
	void loadProfileSettings(const QString &);
	void enableOrDisableSignIn();
	void removeProfile(const QString &);
	void removeProfileDir(const QString &);
	
  Ui::ProfileLoginDialogClass ui;
  bool m_new_registered_profile;
  QString m_current_profile;
};

#endif // PROFILELOGINDIALOG_H
