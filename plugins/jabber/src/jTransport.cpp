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

jTransport::jTransport(Client *jClient, QString jid, QObject *parent) : QObject(parent)
{
	transportreg = new Registration(jClient, JID (jid.toStdString()));
	transportreg->registerRegistrationHandler(this);
	add_transport_form = new QWidget(0, Qt::Dialog);
}

void jTransport::addRegistration()
{
	add_transport_form->setObjectName("add_transport_form");
	add_transport_form->setAttribute(Qt::WA_DeleteOnClose, true);
	information = new QLabel("Receiving form data...", add_transport_form);
	information->setObjectName("information");
	add_transport_form->setWindowTitle(tr("Register transport"));
	gridLayout = new QGridLayout(add_transport_form);
	gridLayout->addWidget(information,0,0);
	add_transport_form->show();
	transportreg->fetchRegistrationFields();

	QSpacerItem *horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
	QHBoxLayout* tmplaout = new QHBoxLayout();
	pushRegister = new QPushButton("Register", add_transport_form);
	QPushButton* pushCancel = new QPushButton("Cancel", add_transport_form);
	tmplaout->addItem(horizontalSpacer);
	tmplaout->addWidget(pushRegister);
	tmplaout->addWidget(pushCancel);
	gridLayout->addLayout(tmplaout, 31, 0, Qt::AlignBottom);
	connect(pushCancel, SIGNAL(clicked()), add_transport_form, SLOT(close()));
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
	labels.append(new QLabel(add_transport_form));
	line_edits.append(new QLineEdit(add_transport_form));
	labels.last()->setText(label);
	line_edits.last()->setObjectName(name);
	laouts.last()->addWidget(labels.last(), 0);
	laouts.last()->addWidget(line_edits.last(), 1);
	gridLayout->addLayout(laouts.last(), laouts.count()+1, 0);
}
void jTransport::handleRegistrationResult(const gloox::JID&, gloox::RegistrationResult res)
{
	switch(res)	{
	case RegistrationSuccess:
		QMessageBox::information(add_transport_form, tr("Transport registration"), tr("Registration success!"), QMessageBox::Ok);
	//default:
	//	QMessageBox::critical(add_transport_form, tr("Transport registration"), tr("Error registration"), QMessageBox::Ok);
	}
	//emit addContact(ui.jidName->text(), ui.nickName->text(), group, ui.authorizationCheck->isChecked());

	add_transport_form->close();
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
        d_form = new jDataForm(new gloox::DataForm(form), add_transport_form);
	gridLayout->addWidget(d_form,1,0);
	pushRegister->setVisible(true);
}

void jTransport::registerClicked()
{
	if (haveDataform) {
		transportreg->createAccount(d_form->getDataForm());
	} else {
		RegistrationFields regfields;
		for (int i = 0; i < line_edits.size(); ++i) {

			QString name = line_edits[i]->objectName();
		      if( name == "name" )
		      {
		    	  regfields.username = line_edits[i]->text().toStdString();
		      }
		      else if( name == "nick" )
		      {
		    	  regfields.nick = line_edits[i]->text().toStdString();
		      }
		      else if( name == "password" )
		      {
		    	  regfields.password = line_edits[i]->text().toStdString();
		      }
		      else if( name == "name" )
		      {
		    	  regfields.name = line_edits[i]->text().toStdString();
		      }
		      else if( name == "first" )
		      {
		    	  regfields.first = line_edits[i]->text().toStdString();
		      }
		      else if( name == "last" )
		      {
		    	  regfields.last = line_edits[i]->text().toStdString();
		      }
		      else if( name == "email" )
		      {
		    	  regfields.email = line_edits[i]->text().toStdString();
		      }
		      else if( name == "address" )
		      {
		    	  regfields.address = line_edits[i]->text().toStdString();
		      }
		      else if( name == "city" )
		      {
		    	  regfields.city = line_edits[i]->text().toStdString();
		      }
		      else if( name == "state" )
		      {
		    	  regfields.state = line_edits[i]->text().toStdString();
		      }
		      else if( name == "zip" )
		      {
		    	  regfields.zip = line_edits[i]->text().toStdString();
		      }
		      else if( name == "phone" )
		      {
		    	  regfields.phone = line_edits[i]->text().toStdString();
		      }
		      else if( name == "url" )
		      {
		    	  regfields.url = line_edits[i]->text().toStdString();
		      }
		      else if( name == "date" )
		      {
		    	  regfields.date = line_edits[i]->text().toStdString();
		      }
		      else if( name == "misc" )
		      {
		    	  regfields.misc = line_edits[i]->text().toStdString();
		      }
		      else if( name == "text" )
		      {
		    	  regfields.text = line_edits[i]->text().toStdString();
		      }
		}
		transportreg->createAccount(wfields, regfields);
	}
}
jTransport::~jTransport() {
	// TODO Auto-generated destructor stub
}
