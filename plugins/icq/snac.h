/*
    snac

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


#ifndef SNAC_H_
#define SNAC_H_

#include <QByteArray>

class QTcpSocket;
class icqBuffer;

class snac
{
public:
	snac();
	~snac();
	void setNotAlone() { snacFlags |= 0x0001; }
	void setUnknowInfo() { snacFlags |= 0x8000; }
	void setFamily(const quint16 family) { familyId = family; }
	void setSubType(const quint16 subtype) { subTypeId = subtype; }
	void setReqId(const quint32 id) { reqId = id; }
	quint16 getFamily() { return familyId; }
	quint16 getSubType() { return subTypeId; }
	quint16 getFlags() { return snacFlags; }
	bool aloneSnac();
	QByteArray getData();
	void readData(icqBuffer *);
private:
	QByteArray convertToByteArray(const quint16 &);
	QByteArray convertToByteArray(const quint32 &);
	quint16 byteArrayToInt16(const QByteArray &);
	quint32 byteArrayToInt32(const QByteArray &);
	quint16 familyId;
	quint16 subTypeId;
	quint16 snacFlags;
	quint32 reqId;
};

#endif /*SNAC_H_*/
