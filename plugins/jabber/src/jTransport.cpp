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

#include "jTransport.h"

jTransport::jTransport(Client *jClient, QString jid, QWidget *parent) : QWidget(parent)
{
	transportreg = new Registration(jClient, JID (jid.toAscii().data()));
	transportreg->registerRegistrationHandler(this);
        //this = new QWidget(0, Qt::Dialog);
}

void jTransport::addRegistration()
{
        this->setObjectName("this");
        this->setAttribute(Qt::WA_DeleteOnClose, true);
        information = new QLabel("Receiving form data...", this);
	information->setObjectName("information");
        setWindowTitle(tr("Register"));
        gridLayout = new QGridLayout(this);
	gridLayout->addWidget(information,0,0);
        this->show();
	transportreg->fetchRegistrationFields();

	QSpacerItem *horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
	QHBoxLayout* tmplaout = new QHBoxLayout();
        pushRegister = new QPushButton("Register", this);
        QPushButton* pushCancel = new QPushButton("Cancel", this);
	tmplaout->addItem(horizontalSpacer);
	tmplaout->addWidget(pushRegister);
	tmplaout->addWidget(pushCancel);
	gridLayout->addLayout(tmplaout, 31, 0, Qt::AlignBottom);
        connect(pushCancel, SIGNAL(clicked()), this, SLOT(close()));
	connect(pushRegister, SIGNAL(clicked()), this, SLOT(registerClicked()));
	pushRegister->setVisible(false);

	haveDataform=false;
}
void jTransport::removeRegistration()
{
	transportreg->removeAccount();
}

void jTransport::handleAlreadyRegistered(const gloox::JID&)
{
        //qDebug() << jProtocol::fromStd(form.instructions().back());
	qDebug() << "AlreadyRegistered";
}
void jTransport::newLineEdit(QString name, QString label)
{
	laouts.append(new QHBoxLayout());
        labels.append(new QLabel(this));
        line_edits.append(new QLineEdit(this));
	labels.last()->setText(label);
	line_edits.last()->setObjectName(name);
	laouts.last()->addWidget(labels.last(), 0);
	laouts.last()->addWidget(line_edits.last(), 1);
	gridLayout->addLayout(laouts.last(), laouts.count()+1, 0);
}
void jTransport::handleRegistrationResult(const gloox::JID& jid, gloox::RegistrationResult res)
{
/*	switch(res)	{
	case RegistrationSuccess:	
		//QMessageBox::information(this, tr("Transport registration"), tr("Registration success!"), QMessageBox::Ok);
	    break;
	//default:
		//QMessageBox::critical(this, tr("Transport registration"), tr("Error registration"), QMessageBox::Ok);
	}*/
	emit registrationResult(jProtocol::fromStd(jid.bare()), res);
	//emit addContact(ui.jidName->text(), ui.nickName->text(), group, ui.authorizationCheck->isChecked());

        this->close();
}
void jTransport::handleRegistrationFields (const JID & from, int fields, std::string instructions){
information->setText(jProtocol::fromStd(instructions));

if (haveDataform) return;

wfields=fields;
if( fields & Registration::FieldUsername ){
	newLineEdit(QString("name"), tr("Name"));
}
if( fields & Registration::FieldNick ){
	newLineEdit(QString("nick"), tr("Nick"));
}
if( fields & Registration::FieldPassword ){
	newLineEdit(QString("password"), tr("Password"));
	line_edits.last()->setEchoMode(QLineEdit::Password);
}
if( fields & Registration::FieldName ){
	newLineEdit(QString("name"), tr("Name"));
}
if( fields & Registration::FieldFirst ){
	newLineEdit(QString("first"), tr("First"));
}
if( fields & Registration::FieldLast ){
	newLineEdit(QString("last"), tr("Last"));
}
if( fields & Registration::FieldEmail ){
	newLineEdit(QString("email"), tr("Email"));
}
if( fields & Registration::FieldAddress ){
	newLineEdit(QString("address"), tr("Address"));
}
if( fields & Registration::FieldCity ){
	newLineEdit(QString("city"), tr("City"));
}
if( fields & Registration::FieldState ){
	newLineEdit(QString("state"), tr("State"));
}
if( fields & Registration::FieldZip ){
	newLineEdit(QString("zip"), tr("Zip"));
}
if( fields & Registration::FieldPhone ){
	newLineEdit(QString("phone"), tr("Phone"));
}
if( fields & Registration::FieldUrl ){
	newLineEdit(QString("url"), tr("URL"));
}
if( fields & Registration::FieldDate ){
	newLineEdit(QString("date"), tr("Date"));
}
if( fields & Registration::FieldMisc ){
	newLineEdit(QString("misc"), tr("Misc"));
}
if( fields & Registration::FieldText ){
	newLineEdit(QString("text"), tr("Text"));
}
pushRegister->setVisible(true);
}
void jTransport::handleDataForm(const gloox::JID& from, const gloox::DataForm& form)
{
	haveDataform=true;
        d_form = new jDataForm(new gloox::DataForm(form), this);
	gridLayout->addWidget(d_form,1,0);
	pushRegister->setVisible(true);
}

void jTransport::registerClicked()
{
      pushRegister->setEnabled(false);
	if (haveDataform) {
		transportreg->createAccount(d_form->getDataForm());
	} else {
		RegistrationFields regfields;
		for (int i = 0; i < line_edits.size(); ++i) {

			QString name = line_edits[i]->objectName();
		      if( name == "name" )
		      {
		    	  regfields.username = line_edits[i]->text().toAscii().data();
		      }
		      else if( name == "nick" )
		      {
		    	  regfields.nick = line_edits[i]->text().toAscii().data();
		      }
		      else if( name == "password" )
		      {
		    	  regfields.password = line_edits[i]->text().toAscii().data();
		      }
		      else if( name == "name" )
		      {
		    	  regfields.name = line_edits[i]->text().toAscii().data();
		      }
		      else if( name == "first" )
		      {
		    	  regfields.first = line_edits[i]->text().toAscii().data();
		      }
		      else if( name == "last" )
		      {
		    	  regfields.last = line_edits[i]->text().toAscii().data();
		      }
		      else if( name == "email" )
		      {
		    	  regfields.email = line_edits[i]->text().toAscii().data();
		      }
		      else if( name == "address" )
		      {
		    	  regfields.address = line_edits[i]->text().toAscii().data();
		      }
		      else if( name == "city" )
		      {
		    	  regfields.city = line_edits[i]->text().toAscii().data();
		      }
		      else if( name == "state" )
		      {
		    	  regfields.state = line_edits[i]->text().toAscii().data();
		      }
		      else if( name == "zip" )
		      {
		    	  regfields.zip = line_edits[i]->text().toAscii().data();
		      }
		      else if( name == "phone" )
		      {
		    	  regfields.phone = line_edits[i]->text().toAscii().data();
		      }
		      else if( name == "url" )
		      {
		    	  regfields.url = line_edits[i]->text().toAscii().data();
		      }
		      else if( name == "date" )
		      {
		    	  regfields.date = line_edits[i]->text().toAscii().data();
		      }
		      else if( name == "misc" )
		      {
		    	  regfields.misc = line_edits[i]->text().toAscii().data();
		      }
		      else if( name == "text" )
		      {
		    	  regfields.text = line_edits[i]->text().toAscii().data();
		      }
		}
		transportreg->createAccount(wfields, regfields);
	}
}
jTransport::~jTransport() {
	// TODO Auto-generated destructor stub
}
