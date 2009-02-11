/*
    tlv

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

#ifndef TLV_H_
#define TLV_H_

#include <QString>

class QTcpSocket;
class icqBuffer;

class tlv
{
public:
	tlv();
	~tlv();
	void setLe(bool f){ LE = f;}
	void setType(quint16 t) { type = t; }
	void setData(const QString &);
	void setData(const QByteArray &);
	void setData(const quint8 &);
	void setData(const quint16 &);
	void setData(const quint32 &);
	quint16 getLength() const;
	QByteArray getData() const;
	inline QByteArray getTlvData() const { return data; }
	inline quint16 getTlvLength() const { return length; }
	inline quint16 getTlvType() const { return type; }
	void readData(icqBuffer *);
private:
	quint16 byteArrayToInt16(const QByteArray &) const;
	quint16 type;
	quint16 length;
	QByteArray data;
	bool LE;
};

#endif /*TLV_H_*/
