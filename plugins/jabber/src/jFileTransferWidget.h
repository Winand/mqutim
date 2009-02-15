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

#ifndef JFILETRANSFERWIDGET_H
#define JFILETRANSFERWIDGET_H

#include <QtGui/QWidget>
#include <gloox/siprofileft.h>
#include <gloox/siprofilefthandler.h>

using namespace gloox;

namespace Ui {
    class jFileTransferWidget;
}

class jFileTransferWidget : public QWidget {
    Q_OBJECT
    Q_DISABLE_COPY(jFileTransferWidget)
public:
    explicit jFileTransferWidget(QWidget *parent = 0);
    virtual ~jFileTransferWidget();

protected:
    virtual void changeEvent(QEvent *e);

private:
    Ui::jFileTransferWidget *m_ui;
};

#endif // JFILETRANSFERWIDGET_H
