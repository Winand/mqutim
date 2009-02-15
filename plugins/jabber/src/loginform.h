/*
    loginform.h

    Copyright (c) 2008 by zodiac <zodiac.nv@gmail.com>

 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************
*/

#ifndef LOGINFORM_H
#define LOGINFORM_H

#include <QtGui/QWidget>
#include "ui_loginform.h"

#include "gloox/client.h"
#include "gloox/jid.h"
#include "gloox/connectionlistener.h"
#include "gloox/registrationhandler.h"

#include "jConnection.h"

using namespace gloox;

class LoginForm : public QWidget, ConnectionListener
{
    Q_OBJECT

public:
	LoginForm(QWidget *parent = 0);
	~LoginForm();
        QString getJid() const {return ui.jidEdit->text().toLower();}
	QString getPass() const {return ui.passowrdEdit->text();}
	void 	onConnect ();
	void 	onDisconnect (ConnectionError e);
	inline void 	onResourceBind (const std::string &resource){};
	inline void 	onResourceBindError (const Error *error){};
	void 	onSessionCreateError (const Error *error);
	inline bool 	onTLSConnect (const CertInfo &info){};
	inline void 	onStreamEvent (StreamEvent event){};

private:
    Ui::LoginFormClass ui;
    Client* r_client;
    QString serverToRegister;
    jConnection *r_connection;

private slots:
    void btnRegisterClick();
    void registrationResult(QString jid, RegistrationResult result);
};

#endif
