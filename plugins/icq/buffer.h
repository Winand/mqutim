/*
    icqBuffer

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


#ifndef BUFFER_H_
#define BUFFER_H_

#include <QBuffer>

class icqBuffer : public QBuffer
{
	Q_OBJECT
public:
	icqBuffer(QObject *parent = 0);
	~icqBuffer();
	QByteArray readAll();
	QByteArray read(qint64);
	qint64 bytesAvailable() const;
	qint64 write ( const QByteArray &);
};

#endif /*BUFFER_H_*/
