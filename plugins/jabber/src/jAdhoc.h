/*
jAdhoc.h

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

#ifndef JADHOC_H
#define JADHOC_H

#include <QWidget>
#include <QtDebug>
#include <QHBoxLayout>
#include <QLabel>
#include <QSpacerItem>
#include <QPushButton>
#include <QMessageBox>
#include <QGridLayout>

#include <gloox/adhoc.h>
#include <gloox/discohandler.h>
#include <gloox/disco.h>
#include <gloox/adhochandler.h>

#include "jAccount.h"
#include "jDataForm.h"

using namespace gloox;

class jDataForm;

class jAdhoc : public QWidget, AdhocHandler
{
    Q_OBJECT
public:
    jAdhoc(jAccount *jabber_account, QString server, QString node, QWidget* parent=0);

    inline void handleAdhocSupport (const JID &remote, bool support){ ;};
    inline void handleAdhocCommands (const JID &remote, const StringMap &commands){ ;};
    inline void handleAdhocError (const JID &remote, const Error *error){ ;};
    void handleAdhocExecutionResult (const JID &remote, const Adhoc::Command &command);

private:
    jAccount *m_jabber_account;
    Adhoc *m_adhoc;
    jDataForm* d_form;
    QPushButton *pushFinish, *pushNext, *pushPrev, *pushCancel, *pushComplete;
    QGridLayout *gridLayout;
    QString m_node, m_server;
    std::string session_id;

private slots:
    void doExecute();
    void doCancel();
    void doNext();
    void doPrev();
    void doComplete();
};

#endif // JADHOC_H
