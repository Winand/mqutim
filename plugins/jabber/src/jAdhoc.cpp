/*
jAdhoc.cpp

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

#include "jAdhoc.h"

jAdhoc::jAdhoc(jAccount *jabber_account, QString server, QString node, QWidget* parent) : QWidget(parent)
{
    m_adhoc = new Adhoc(jabber_account->getProtocol()->getClient());
    m_node = node;
    m_server = server;
    m_adhoc->execute(JID(server.toAscii().data()), new Adhoc::Command (node.toAscii().data(), Adhoc::Command::Execute), this );
//    m_adhoc->execute(JID(server.toStdString()), new Adhoc::Command (node.toStdString(), Adhoc::Command::Execute), this );
}

void jAdhoc::handleAdhocExecutionResult (const JID &remote, const Adhoc::Command &command)
{
        if (command.status() == Adhoc::Command::Completed) return;
        setObjectName("adhoc_form");
       // setAttribute(Qt::WA_DeleteOnClose, true);
        session_id = command.sessionID();
        gridLayout = new QGridLayout(this);
        //Instruction
        StringList lst = command.form()->instructions();
        StringList::iterator it;
        for ( it=lst.begin() ; it != lst.end(); it++ ){
            gridLayout->addWidget(new QLabel(jProtocol::fromStd(*it), this),0,0);
        }
        //Form
        d_form = new jDataForm(const_cast<DataForm *>(command.form()), this);
        gridLayout->addWidget(d_form,1,0);
        //Buttons
        QSpacerItem *horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
        QHBoxLayout* tmplaout = new QHBoxLayout();
        tmplaout->addItem(horizontalSpacer);
        if(command.actions() & Adhoc::Command::Execute){
            pushFinish = new QPushButton(tr("Finish"), this);
            tmplaout->addWidget(pushFinish);
            connect(pushFinish, SIGNAL(clicked()), this, SLOT(doExecute()));
        }
        if(command.actions() & Adhoc::Command::Cancel){
            pushCancel = new QPushButton(tr("Cancel"), this);
            tmplaout->addWidget(pushCancel);
            connect(pushCancel, SIGNAL(clicked()), this, SLOT(doCancel()));
        }
        if(command.actions() & Adhoc::Command::Previous){
            pushPrev = new QPushButton(tr("Previous"), this);
            tmplaout->addWidget(pushPrev);
            connect(pushPrev, SIGNAL(clicked()), this, SLOT(doPrev()));
        }
        if(command.actions() & Adhoc::Command::Next){
            pushNext = new QPushButton(tr("Next"), this);
            tmplaout->addWidget(pushNext);
            connect(pushNext, SIGNAL(clicked()), this, SLOT(doNext()));
        }
        if(command.actions() & Adhoc::Command::Complete){
            pushComplete = new QPushButton(tr("Ok"), this);
            tmplaout->addWidget(pushComplete);
            connect(pushComplete, SIGNAL(clicked()), this, SLOT(doComplete()));
        }
        if (command.actions() == 0){
            pushFinish = new QPushButton(tr("Finish"), this);
            tmplaout->addWidget(pushFinish);
            connect(pushFinish, SIGNAL(clicked()), this, SLOT(doExecute()));
        }


        gridLayout->addLayout(tmplaout, 31, 0, Qt::AlignBottom);

        show();
}

void jAdhoc::doExecute(){
m_adhoc->execute(JID(m_server.toAscii().data()), new Adhoc::Command (m_node.toAscii().data(), session_id, Adhoc::Command::Executing, d_form->getDataForm()), this );
close();
}
void jAdhoc::doCancel(){
    m_adhoc->execute(JID(m_server.toAscii().data()), new Adhoc::Command (m_node.toAscii().data(), session_id, Adhoc::Command::Cancel), this );
    close();
}
void jAdhoc::doNext(){
m_adhoc->execute(JID(m_server.toAscii().data()), new Adhoc::Command (m_node.toAscii().data(), session_id, Adhoc::Command::Executing, d_form->getDataForm()), this );
close();
}
void jAdhoc::doPrev(){
m_adhoc->execute(JID(m_server.toAscii().data()), new Adhoc::Command (m_node.toAscii().data(), session_id, Adhoc::Command::Previous), this );
close();
}
void jAdhoc::doComplete(){
m_adhoc->execute(JID(m_server.toAscii().data()), new Adhoc::Command (m_node.toAscii().data(), session_id, Adhoc::Command::Complete), this );
close();
}

