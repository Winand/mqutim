/*
jTransport.cpp

Copyright (c) 2008 by Kostin Dmitrij <kostindima@gmail.com>

***************************************************************************
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
*                                                                         *
***************************************************************************
*/

#ifndef JTRANSPORT_H_
#define JTRANSPORT_H_

#include <QDebug>
#include <QWidget>
#include <QLabel>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QGridLayout>
#include <QSpacerItem>
#include <QPushButton>
#include <QMessageBox>

#include <gloox/registration.h>
#include <gloox/registrationhandler.h>
#include <gloox/client.h>
#include <gloox/jid.h>
#include <gloox/dataform.h>
#include <gloox/dataformfield.h>
#include <gloox/dataformfieldcontainer.h>

#include "jProtocol.h"
#include "jDataForm.h"

using namespace gloox;

class jDataForm;

class jTransport : public QWidget, RegistrationHandler
{
	Q_OBJECT

public:
        jTransport(Client *jClient, QString jid, QWidget *parent = 0);
	virtual ~jTransport();

	void addRegistration();
	void removeRegistration();

private:
	void handleRegistrationFields (const JID & from, int fields, std::string instructions);
	void handleAlreadyRegistered(const gloox::JID&);
	void handleRegistrationResult(const gloox::JID& jid, gloox::RegistrationResult res);
	void handleDataForm(const gloox::JID&, const gloox::DataForm&);
	void handleOOB(const gloox::JID&, const gloox::OOB&) {}

	void newLineEdit(QString name, QString label);
	Registration *transportreg;

	QWidget *add_transport_form;
	QLabel *information;
	QGridLayout *gridLayout;
	jDataForm* d_form;

	DataForm *forma;
	int wfields;
	bool haveDataform;
	QPushButton* pushRegister;
        QList<QLineEdit *> line_edits;
        QList<QLabel *> labels;
        QList<QHBoxLayout *> laouts;

signals:
	void registrationResult(QString , RegistrationResult);

private slots:
	void registerClicked();

};

#endif /* JADDTRANSPORT_H_ */
