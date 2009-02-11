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

#include "buffer.h"

icqBuffer::icqBuffer(QObject *parent) : QBuffer(parent)
{
	
}

icqBuffer::~icqBuffer()
{
	
}
QByteArray icqBuffer::readAll()
{
	QBuffer::seek(0);
	QByteArray array = QBuffer::readAll();
	QBuffer::buffer().clear();
	return array;
}

QByteArray icqBuffer::read(qint64 size)
{
	QBuffer::seek(0);
	QByteArray array = QBuffer::read(size);
	QBuffer::buffer().remove(0,size);
	return array;
}

qint64 icqBuffer::bytesAvailable() const
{
	return QBuffer::buffer().size();
}

qint64 icqBuffer::write ( const QByteArray & byteArray )
{
	QBuffer::seek(QBuffer::buffer().size());
	return QBuffer::write(byteArray);
}
