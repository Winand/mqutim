/*****************************************************************************
    jFileTransfer

    Copyright (c) 2009 by Nigmatullin Ruslan <euroelessar@gmail.com>

 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************
*****************************************************************************/

#include "jFileTransferWidget.h"
#include "ui_jFileTransferWidget.h"

jFileTransferWidget::jFileTransferWidget(QWidget *parent) :
    QWidget(parent),
    m_ui(new Ui::jFileTransferWidget)
{
    m_ui->setupUi(this);
}

jFileTransferWidget::~jFileTransferWidget()
{
    delete m_ui;
}

void jFileTransferWidget::changeEvent(QEvent *e)
{
    switch (e->type()) {
    case QEvent::LanguageChange:
        m_ui->retranslateUi(this);
        break;
    default:
        break;
    }
}
