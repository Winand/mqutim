/*
    flapPacket

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


#ifndef FLAP_H_
#define FLAP_H_

class icqBuffer;
class QByteArray;

class flapPacket
{
public:
	flapPacket();
	~flapPacket();
	inline quint8 getChannel() const { return channel; }
	inline quint16 getLength() const { return length; }
	bool readFromSocket(icqBuffer *);
private:
	quint8 byteArrayToInt8(const QByteArray &) const;
	quint16 byteArrayToInt16(const QByteArray &) const;
	quint8 channel;
	quint16 seqNumber;
	quint16 length;
};

#endif /*FLAP_H_*/
