/*
    metaInformation

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

#include <QTcpSocket>

#include "snac.h"
#include "buffer.h"
#include "tlv.h"

#include "metainformation.h"

metaInformation::metaInformation(const QString &uin) : myUin(uin), setBirth(false)
{

}

metaInformation::~metaInformation()
{
	
}

void metaInformation::sendShortInfoReq(QTcpSocket *tcpSocket, const quint16 &flapReq, 
		const quint32 &snacReq, const quint16 &metaReq, const QString &uin)
{
	QByteArray packet;
	packet[0] = 0x2a;
	packet[1] = 0x02;
	packet.append(convertToByteArray((quint16)flapReq));
	packet.append(convertToByteArray((quint16)30));
	snac snac1502;
	snac1502.setFamily(0x0015);
	snac1502.setSubType(0x0002);
	snac1502.setReqId(snacReq);
	packet.append(snac1502.getData());
	
	packet.append(convertToByteArray((quint16)0x0001));
	packet.append(convertToByteArray((quint16)0x0010));
	packet.append(convertToByteArray((quint16)0x0e00));
	packet.append(convertUinToArray(myUin));
	packet.append(convertToByteArray((quint16)0xd007));
	packet.append(convertToLEByteArray((quint16)metaReq));
	packet.append(convertToByteArray((quint16)0xba04));
	packet.append(convertUinToArray(uin));
	tcpSocket->write(packet);
}

QByteArray metaInformation::convertToByteArray(const quint16 &d)
{
	QByteArray packet;
	packet[0] = (d / 0x100);
	packet[1] = (d % 0x100);
	return packet;
}

QByteArray metaInformation::convertToByteArray(const quint8 &d)
{
	QByteArray packet;
	packet[0] = d;
	return packet;
}

QByteArray metaInformation::convertToLEByteArray(const quint16 &d)
{
	QByteArray packet;
	packet[1] = (d / 0x100);
	packet[0] = (d % 0x100);
	return packet;
}

QByteArray metaInformation::convertToByteArray(const quint32 &d)
{
	QByteArray packet;
	packet[0] = (d / 0x1000000);
	packet[1] = (d / 0x10000);
	packet[2] = (d / 0x100);
	packet[3] = (d % 0x100);
	return packet;
}

QByteArray metaInformation::convertUinToArray(const QString &uin)
{
	 quint32 tmpUin = uin.toUInt();
	 QByteArray packet;
	 packet[0] = tmpUin % 0x100;
	 packet[1] = tmpUin % 0x10000 / 0x100;
	 packet[2] = tmpUin % 0x1000000 / 0x10000;
	 packet[3] = tmpUin / 0x1000000;
	 return packet;
}

quint16 metaInformation::byteArrayToLEInt16(const QByteArray &array)
{
	bool ok;
	quint16 tmp = array.toHex().toUInt(&ok,16);
	return ((tmp % 0x100) * 0x100 + (tmp)/ 0x100);
}

quint32 metaInformation::byteArrayToLEInt32(const QByteArray &array)
{
	bool ok;
	quint32 tmp = array.toHex().toUInt(&ok,16);
	return ((tmp % 0x100) * 0x1000000 + 
			(tmp % 0x10000 / 0x100) * 0x10000 +
			(tmp % 0x1000000 / 0x10000) * 0x100 +
			(tmp)/0x1000000);
}


quint16 metaInformation::readShortInfo(icqBuffer *socket)
{
	quint16 length = 3;
	socket->read(1);
	quint16 nickLength = byteArrayToLEInt16(socket->read(2));
	nick = socket->read(nickLength - 1);
	socket->read(1);
	length += nickLength;
	quint16 firstNameLength = byteArrayToLEInt16(socket->read(2));
	socket->read(firstNameLength);
	
	length = length + firstNameLength + 2;
	
	quint16 lastNameLength = byteArrayToLEInt16(socket->read(2));
	socket->read(lastNameLength);
	
	length = length + lastNameLength + 2;
	quint16 emailLength = byteArrayToLEInt16(socket->read(2));
	socket->read(emailLength);
	
	length = length + emailLength  + 2 + 3;
	socket->read(3);
	return length;
}



void metaInformation::searchByUin(QTcpSocket *tcpSocket, const quint16 &flapReq, 
		const quint32 &snacReq, const quint16 &metaReq, const QString &uin)
{
	QByteArray packet;
		packet[0] = 0x2a;
		packet[1] = 0x02;
		packet.append(convertToByteArray((quint16)flapReq));
		packet.append(convertToByteArray((quint16)34));
		snac snac1502;
		snac1502.setFamily(0x0015);
		snac1502.setSubType(0x0002);
		snac1502.setReqId(snacReq);
		packet.append(snac1502.getData());
		
		packet.append(convertToByteArray((quint16)0x0001));
		packet.append(convertToByteArray((quint16)0x0014));
		packet.append(convertToByteArray((quint16)0x1200));
		packet.append(convertUinToArray(myUin));
		packet.append(convertToByteArray((quint16)0xd007));
		packet.append(convertToLEByteArray((quint16)metaReq));
		packet.append(convertToByteArray((quint16)0x6905));
		packet.append(convertToByteArray((quint16)0x3601));
		packet.append(convertToByteArray((quint16)0x0400));
		packet.append(convertUinToArray(uin));
		tcpSocket->write(packet);

}

quint16 metaInformation::readSearchResult(icqBuffer *socket, bool last)
{
	quint16 length = 1;
	founded = true;
	
	quint8 successByte = convertToInt8(socket->read(1));
	
	if ( successByte != 0x0a)
	{
		founded = false;
		return length;
	}
	
	socket->read(2);
	length += 2;
	quint32 userUin = byteArrayToLEInt32(socket->read(4));
	length += 4;
	foundedUin = QString::number(userUin);
	
	
	quint16 nickLength = byteArrayToLEInt16(socket->read(2));
	
	length += 2;
	foundedNick = socket->read(nickLength - 1);
	socket->read(1);
	length += nickLength;
	
	quint16 firstLength = byteArrayToLEInt16(socket->read(2));
		
	length += 2;
	foundedFirst = socket->read(firstLength - 1);
	socket->read(1);
	length += firstLength;
		
	quint16 lastLength = byteArrayToLEInt16(socket->read(2));
		
	length += 2;
	foundedLast = socket->read(lastLength - 1);
	socket->read(1);
	length += lastLength;
	
	quint16 emailLength = byteArrayToLEInt16(socket->read(2));
		
	length += 2;
	foundedEmail = socket->read(emailLength - 1);
	socket->read(1);
	length += emailLength;
	
	
	authFlag = convertToInt8(socket->read(1));
	
	length += 1;
	
	foundedStatus  = byteArrayToLEInt16(socket->read(2));
	
	length += 2;
	
	foundedGender = convertToInt8(socket->read(1));
	length += 1;
	
	foundedAge = byteArrayToLEInt16(socket->read(2));
	
	length += 2;
	
	
	if ( last )
	{
		socket->read(4);
		length += 4;
	}
	
	return length;
}

quint8 metaInformation::convertToInt8(const QByteArray &packet)
{
	bool ok;
	return packet.toHex().toUInt(&ok,16);
}

void metaInformation::searchByEmail(QTcpSocket *tcpSocket, const quint16 &flapReq, 
		const quint32 &snacReq, const quint16 &metaReq, QString email)
{
	QByteArray packet;
		packet[0] = 0x2a;
		packet[1] = 0x02;
		packet.append(convertToByteArray((quint16)flapReq));

		quint16 metaLength = 19 + email.length();
		
		packet.append(convertToByteArray((quint16)(14 + metaLength)));
		snac snac1502;
		snac1502.setFamily(0x0015);
		snac1502.setSubType(0x0002);
		snac1502.setReqId(snacReq);
		packet.append(snac1502.getData());
		
		packet.append(convertToByteArray((quint16)0x0001));
		packet.append(convertToByteArray((quint16)metaLength));
		packet.append(convertToLEByteArray((quint16)(metaLength - 2)));
		packet.append(convertUinToArray(myUin));
		packet.append(convertToByteArray((quint16)0xd007));
		packet.append(convertToLEByteArray((quint16)metaReq));
		packet.append(convertToByteArray((quint16)0x7305));
		packet.append(convertToByteArray((quint16)0x5e01));
		packet.append(convertToLEByteArray((quint16)(email.length() + 3)));
		packet.append(convertToByteArray((quint16)(email.length() + 1)));
		email.append(QChar(0x00));
		packet.append(email);
		
		tcpSocket->write(packet);

}

void metaInformation::searchByOther(QTcpSocket *tcpSocket, const quint16 &flapReq, 
			const quint32 &snacReq, const quint16 &metaReq,bool onlineOnly, QString nick,
			QString firstName, QString lastName, quint8 gender, quint16 minAge, quint16 maxAge,
			quint16 country, QString city, quint16 interests, quint16 language, quint16 occupation,
			QString keyWords)
{
			QByteArray packet;
			packet[0] = 0x2a;
			packet[1] = 0x02;
			packet.append(convertToByteArray((quint16)flapReq));

			quint16 metaLength = 0;
			
			QByteArray metaArray;
			
			metaLength += 5;
							
			metaArray.append(convertToByteArray((quint16)0x3002));
			metaArray.append(convertToByteArray((quint16)0x0100));
			
			if ( onlineOnly )
			{
				
				metaArray.append(convertToByteArray((quint8)0x01));
			} else
				metaArray.append(convertToByteArray((quint8)0x00));
			
			if ( !nick.isEmpty() )
			{
				metaArray.append(convertToByteArray((quint16)0x5401));
				metaArray.append(convertToLEByteArray((quint16)(3 + nick.length())));
				metaArray.append(convertToLEByteArray((quint16)(1 + nick.length())));
				metaLength += 6;
				
				nick.append(QChar(0x00));
				metaArray.append(nick);
				
				metaLength += nick.length();
			}
			
			if ( !firstName.isEmpty() )
			{
				metaArray.append(convertToByteArray((quint16)0x4001));
				metaArray.append(convertToLEByteArray((quint16)(3 + firstName.length())));
				metaArray.append(convertToLEByteArray((quint16)(1 + firstName.length())));
				metaLength += 6;		
				
				firstName.append(QChar(0x00));
				metaArray.append(firstName);
				metaLength += firstName.length();
			}
			
			if ( !lastName.isEmpty() )
			{
				metaArray.append(convertToByteArray((quint16)0x4a01));
				metaArray.append(convertToLEByteArray((quint16)(3 + lastName.length())));
				metaArray.append(convertToLEByteArray((quint16)(1 + lastName.length())));
				metaLength += 6;
				
				lastName.append(QChar(0x00));
				metaArray.append(lastName);
				metaLength += lastName.length();
			}
			
			if ( gender )
			{
				metaArray.append(convertToByteArray((quint16)0x7c01));
				metaArray.append(convertToByteArray((quint16)0x0100));
				metaArray.append(convertToByteArray((quint8)gender));
				metaLength += 5;
			}
			
			if ( minAge && maxAge)
			{
				metaArray.append(convertToByteArray((quint16)0x6801));
				metaArray.append(convertToByteArray((quint16)0x0400));
				metaArray.append(convertToLEByteArray((quint16)minAge));
				metaArray.append(convertToLEByteArray((quint16)maxAge));
				metaLength += 8;
			}
			
			if ( country )
			{
				metaArray.append(convertToByteArray((quint16)0xa401));
				metaArray.append(convertToByteArray((quint16)0x0200));
				metaArray.append(convertToLEByteArray((quint16)country));
				metaLength += 6;
			}
			
			if ( !city.isEmpty() )
			{
				metaArray.append(convertToByteArray((quint16)0x9001));
				metaArray.append(convertToLEByteArray((quint16)(3 + city.length())));
				metaArray.append(convertToLEByteArray((quint16)(1 + city.length())));
				metaLength += 6;
				
				city.append(QChar(0x00));
				metaArray.append(city);
				metaLength += city.length();
			}
			
			if ( interests )
			{
				metaArray.append(convertToByteArray((quint16)0xea01));
				metaArray.append(convertToByteArray((quint16)0x0400));
				metaArray.append(convertToLEByteArray((quint16)interests));
				metaArray.append(convertToLEByteArray((quint16)0x0000));
				metaLength += 8;
			}
			
			
			if ( language )
			{
				metaArray.append(convertToByteArray((quint16)0x8601));
				metaArray.append(convertToByteArray((quint16)0x0200));
				metaArray.append(convertToLEByteArray((quint16)language));
				metaLength += 6;
			}
			
			if ( occupation )
			{
				metaArray.append(convertToByteArray((quint16)0xcc01));
				metaArray.append(convertToByteArray((quint16)0x0200));
				metaArray.append(convertToLEByteArray((quint16)occupation));
				metaLength += 6;
			}
			
			if ( !keyWords.isEmpty() )
			{
				metaArray.append(convertToByteArray((quint16)0x2602));
				metaArray.append(convertToLEByteArray((quint16)(3 + keyWords.length())));
				metaArray.append(convertToLEByteArray((quint16)(1 + keyWords.length())));
				metaLength += 6;
				
				keyWords.append(QChar(0x00));
				metaArray.append(keyWords);
				metaLength += keyWords.length();
			}
			
			packet.append(convertToByteArray((quint16)(26 + metaLength)));
			snac snac1502;
			snac1502.setFamily(0x0015);
			snac1502.setSubType(0x0002);
			snac1502.setReqId(snacReq);
			packet.append(snac1502.getData());
			
			packet.append(convertToByteArray((quint16)0x0001));
			packet.append(convertToByteArray((quint16)(12 + metaLength)));
			packet.append(convertToLEByteArray((quint16)(metaLength + 10)));
			packet.append(convertUinToArray(myUin));
			packet.append(convertToByteArray((quint16)0xd007));
			packet.append(convertToLEByteArray((quint16)metaReq));
			packet.append(convertToByteArray((quint16)0x5f05));
			packet.append(metaArray);
			
			tcpSocket->write(packet);
}

void metaInformation::getFullUserInfo(QTcpSocket *tcpSocket, const quint16 &flapReq, 
		const quint32 &snacReq, const quint16 &metaReq, const QString &uin)
{
	QByteArray packet;
	packet[0] = 0x2a;
	packet[1] = 0x02;
	packet.append(convertToByteArray((quint16)flapReq));
	packet.append(convertToByteArray((quint16)30));
	snac snac1502;
	snac1502.setFamily(0x0015);
	snac1502.setSubType(0x0002);
	snac1502.setReqId(snacReq);
	packet.append(snac1502.getData());
	
	packet.append(convertToByteArray((quint16)0x0001));
	packet.append(convertToByteArray((quint16)0x0010));
	packet.append(convertToByteArray((quint16)0x0e00));
	packet.append(convertUinToArray(myUin));
	packet.append(convertToByteArray((quint16)0xd007));
	packet.append(convertToLEByteArray((quint16)metaReq));
	if ( uin == myUin )
		packet.append(convertToByteArray((quint16)0xb204));
	else
		packet.append(convertToByteArray((quint16)0xd004));
	packet.append(convertUinToArray(uin));
	tcpSocket->write(packet);
}


quint16 metaInformation::readBasicUserInfo(icqBuffer *socket)
{
	quint16 length = 1;
	basicInfoSuccess = true;
		
	quint8 successByte = convertToInt8(socket->read(1));
		
	if ( successByte != 0x0a)
	{
			basicInfoSuccess = false;
			return length;
	}
	
	
	
	
	quint16 nickLength = byteArrayToLEInt16(socket->read(2));
	length += 2;
	
	basicNick = socket->read(nickLength - 1);
	socket->read(1);
	length += nickLength;
	
	
	quint16 firstNameLength = byteArrayToLEInt16(socket->read(2));
	basicFirst = socket->read(firstNameLength - 1);
	socket->read(1);
	
	length = length + firstNameLength + 2;
	
	quint16 lastNameLength = byteArrayToLEInt16(socket->read(2));
	basicLast = socket->read(lastNameLength - 1);
	socket->read(1);
	length = length + lastNameLength + 2;
	
	
	quint16 emailLength = byteArrayToLEInt16(socket->read(2));
	basicEmail = socket->read(emailLength - 1);
	socket->read(1);
	length = length + emailLength  + 2;
	
	quint16 cityLength = byteArrayToLEInt16(socket->read(2));
	basicCity = socket->read(cityLength - 1);
	socket->read(1);
	length = length + cityLength  + 2;
	
	quint16 stateLength = byteArrayToLEInt16(socket->read(2));
	basicState = socket->read(stateLength - 1);
	socket->read(1);
	length = length + stateLength  + 2;
	
	quint16 phoneLength = byteArrayToLEInt16(socket->read(2));
	basicPhone = socket->read(phoneLength - 1);
	socket->read(1);
	length = length +phoneLength  + 2;
	
	quint16 faxLength = byteArrayToLEInt16(socket->read(2));
	basicFax= socket->read(faxLength - 1);
	socket->read(1);
	length = length + faxLength  + 2;
	
	quint16 addressLength = byteArrayToLEInt16(socket->read(2));
	basicAddress= socket->read(addressLength - 1);
	socket->read(1);
	length = length + addressLength  + 2;
	
	quint16 cellLength = byteArrayToLEInt16(socket->read(2));
	basicCell= socket->read(cellLength - 1);
	socket->read(1);
	length = length + cellLength  + 2;
	
	quint16 zipLength = byteArrayToLEInt16(socket->read(2));
	basicZip= socket->read(zipLength - 1);
	socket->read(1);
	length = length + zipLength  + 2;
	
	country = byteArrayToLEInt16(socket->read(2));

	length  += 2;
	
	socket->read(1);
	
	basicAuthFlag = convertToInt8(socket->read(1));
	webAware = convertToInt8(socket->read(1));
	socket->read(1);
	publishEmail = convertToInt8(socket->read(1));
	
	
	length += 5;
	
	return length;	
}

quint16 metaInformation::readMoreUserInfo(icqBuffer *socket)
{
	quint16 length = 1;
	moreInfoSuccess = true;
		
	quint8 successByte = convertToInt8(socket->read(1));
		
	if ( successByte != 0x0a)
	{
			moreInfoSuccess = false;
			return length;
	}
	
	
	
	
	moreAge = byteArrayToLEInt16(socket->read(2));
	length += 2;
	
	moreGender = convertToInt8(socket->read(1));
	
	length ++;
	
	
	
	
	quint16 homePageLength = byteArrayToLEInt16(socket->read(2));
	homepage = socket->read(homePageLength - 1);
	socket->read(1);
	
	length = length + homePageLength + 2;
	
	moreBirthYear = byteArrayToLEInt16(socket->read(2));
	
	length += 2;
	
	moreBirthMonth = convertToInt8(socket->read(1));
	
	length ++;
	
	moreBirthDay = convertToInt8(socket->read(1));
	
	length ++;
	
	
	moreLang1 = convertToInt8(socket->read(1));
	
	length ++;
	
	moreLang2 = convertToInt8(socket->read(1));
	
	length ++;
	
	moreLang3 = convertToInt8(socket->read(1));
	
	length ++;
	
	
	socket->read(2);
	
	length += 2;
	
	
	
	quint16 cityLength = byteArrayToLEInt16(socket->read(2));
	moreCity = socket->read(cityLength - 1);
	socket->read(1);
	length = length + cityLength + 2;

	
	quint16 stateLength = byteArrayToLEInt16(socket->read(2));
	moreState = socket->read(stateLength - 1);
	socket->read(1);
	length = length + stateLength  + 2;
	
	
	moreCountry  = byteArrayToLEInt16(socket->read(2));
	
	length += 2;
	
	
	socket->read(1);
	
	length++;
	
	return length;	
}

quint16 metaInformation::readWorkUserInfo(icqBuffer *socket)
{
	quint16 length = 1;
	workInfoSuccess = true;
		
	quint8 successByte = convertToInt8(socket->read(1));
		
	if ( successByte != 0x0a)
	{
			workInfoSuccess = false;
			return length;
	}
	
	
	

	
	
	
	
	quint16 cityLength = byteArrayToLEInt16(socket->read(2));
	workCity = socket->read(cityLength - 1);
	socket->read(1);
	
	length = length + cityLength + 2;
	
	
	
	
	quint16 stateLength = byteArrayToLEInt16(socket->read(2));
	workState = socket->read(stateLength - 1);
	socket->read(1);
	length = length + stateLength + 2;
	
	
	quint16 phoneLength = byteArrayToLEInt16(socket->read(2));
	workPhone = socket->read(phoneLength - 1);
	socket->read(1);
	length = length + phoneLength  + 2;
	
	quint16 faxLength = byteArrayToLEInt16(socket->read(2));
	workFax = socket->read(faxLength - 1);
	socket->read(1);
	length = length + faxLength  + 2;
	
	quint16 addressLength = byteArrayToLEInt16(socket->read(2));
	workAddress = socket->read(addressLength - 1);
	socket->read(1);
	length = length + addressLength  + 2;
	
	quint16 zipLength = byteArrayToLEInt16(socket->read(2));
	workZip = socket->read(zipLength - 1);
	socket->read(1);
	length = length + zipLength  + 2;

	workCountry = byteArrayToLEInt16(socket->read(2));
	length += 2;
	
	quint16 companyLength = byteArrayToLEInt16(socket->read(2));
	workCompany = socket->read(companyLength - 1);
	socket->read(1);
	length = length + companyLength  + 2;
	
	quint16 departmentLength = byteArrayToLEInt16(socket->read(2));
	workDepartment = socket->read(departmentLength - 1);
	socket->read(1);
	length = length + departmentLength  + 2;
	
	quint16 positionLength = byteArrayToLEInt16(socket->read(2));
	workPosition = socket->read(positionLength - 1);
	socket->read(1);
	length = length + positionLength  + 2;
	
	workOccupation = byteArrayToLEInt16(socket->read(2));
	length += 2;
	
	quint16 webpageLength = byteArrayToLEInt16(socket->read(2));
	workWebPage = socket->read(webpageLength - 1);
	socket->read(1);
	length = length + webpageLength  + 2;
	
	
	return length;	
}

quint16 metaInformation::readInterestsUserInfo(icqBuffer *socket)
{
	quint16 length = 1;
	interestsInfoSuccess = true;
		
	quint8 successByte = convertToInt8(socket->read(1));
		
	if ( successByte != 0x0a)
	{
			interestsInfoSuccess = false;
			return length;
	}
	
	
	

	
	quint8 interestsCount = convertToInt8(socket->read(1));
	length++;
	
	for(int i = 0; i < interestsCount; i++)
	{
		
	if(i == 0)
	{
		interCode1 = byteArrayToLEInt16(socket->read(2));
		length += 2;
		
		
		quint16 interLength1 = byteArrayToLEInt16(socket->read(2));
		interKeyWords1 = socket->read(interLength1  - 1);
		socket->read(1);
		
		length = length + interLength1  + 2;
	}	
	else if ( i == 1)
	{
		interCode2 = byteArrayToLEInt16(socket->read(2));
		length += 2;
		
		
		quint16 interLength2 = byteArrayToLEInt16(socket->read(2));
		interKeyWords2 = socket->read(interLength2  - 1);
		socket->read(1);
		
		length = length + interLength2  + 2;
	}
	else if ( i == 2)
	{
		interCode3 = byteArrayToLEInt16(socket->read(2));
		length += 2;
		
		
		quint16 interLength3 = byteArrayToLEInt16(socket->read(2));
		interKeyWords3 = socket->read(interLength3  - 1);
		socket->read(1);
		
		length = length + interLength3  + 2;
	}
	else if ( i == 3 )
	{
		interCode4 = byteArrayToLEInt16(socket->read(2));
		length += 2;
		
		
		quint16 interLength4 = byteArrayToLEInt16(socket->read(2));
		interKeyWords4 = socket->read(interLength4  - 1);
		socket->read(1);
		
		length = length + interLength4  + 2;
	} else {
		socket->read(2);
		length += 2;
		quint16 interLength= byteArrayToLEInt16(socket->read(2));
		socket->read(interLength);
		length = length + interLength  + 2;
	}
	
	}
	
	
	return length;	
}

quint16 metaInformation::readAboutUserInfo(icqBuffer *socket)
{
	quint16 length = 1;
	aboutInfoSuccess = true;
		
	quint8 successByte = convertToInt8(socket->read(1));
		
	if ( successByte != 0x0a)
	{
			aboutInfoSuccess = false;
			return length;
	}
	
	quint16 aboutLength = byteArrayToLEInt16(socket->read(2));
	about = socket->read(aboutLength- 1);
	socket->read(1);
	length = length + aboutLength  + 2;
	

	
	
	
	
	return length;	
}


void metaInformation::saveOwnerInfo(QTcpSocket *tcpSocket, const quint16 &flapReq, 
		const quint32 &snacReq, const quint16 &metaReq)
{
	QByteArray packet;
	packet[0] = 0x2a;
	packet[1] = 0x02;
	packet.append(convertToByteArray((quint16)flapReq));
	
	
	QByteArray metaData;
	tlv nickTlv;
	nickTlv.setLe(true);
	nickTlv.setType(0x0154);
	QByteArray nickArray;
	nickArray.append(convertToLEByteArray((quint16)(basicNick.length() + 1)));
	nickArray.append(basicNick);
	nickArray.append(QChar(0x00));
	nickTlv.setData(nickArray);
	
	metaData.append(nickTlv.getData());
	
	

	tlv firstTlv;
	firstTlv.setLe(true);
	firstTlv.setType(0x0140);
	
	QByteArray firstArray;
	firstArray.append(convertToLEByteArray((quint16)(basicFirst.length() + 1)));
	firstArray.append(basicFirst);
	firstArray.append(QChar(0x00));
	
	firstTlv.setData(firstArray);
	
	metaData.append(firstTlv.getData());
	

	tlv lastTlv;
	lastTlv.setLe(true);
	lastTlv.setType(0x014a);
	
	QByteArray lastArray;
	lastArray.append(convertToLEByteArray((quint16)(basicLast.length() + 1)));
	lastArray.append(basicLast);
	lastArray.append(QChar(0x00));
	
	
	lastTlv.setData(lastArray);
	
	metaData.append(lastTlv.getData());
	

	tlv emailTlv;
	emailTlv.setType(0x015e);
	emailTlv.setLe(true);
	QByteArray mailArray;
	
	mailArray.append(convertToLEByteArray((quint16)(basicEmail.length() + 2)));
	mailArray.append(basicEmail);
	mailArray.append(QChar(0x00));
	mailArray.append(publishEmail);
	
	emailTlv.setData(mailArray);
	
	metaData.append(emailTlv.getData());
	
	
	tlv countryTlv;
	countryTlv.setType(0x01a4);
	countryTlv.setLe(true);
	countryTlv.setData(convertToLEByteArray(country));
	
	metaData.append(countryTlv.getData());
	
	
	tlv cityTlv;
	cityTlv.setLe(true);
	cityTlv.setType(0x0190);
		
	QByteArray cityArray;
	cityArray.append(convertToLEByteArray((quint16)(basicCity.length() + 1)));
	cityArray.append(basicCity);
	cityArray.append(QChar(0x00));
		
		
	cityTlv.setData(cityArray);
	
	metaData.append(cityTlv.getData());
	
	tlv stateTlv;
	stateTlv.setLe(true);
	stateTlv.setType(0x019a);
		
	QByteArray stateArray;
	stateArray.append(convertToLEByteArray((quint16)(basicState.length() + 1)));
	stateArray.append(basicState);
	stateArray.append(QChar(0x00));
		
		
	stateTlv.setData(stateArray);
	
	metaData.append(stateTlv.getData());
	
	
	tlv zipTlv;
	zipTlv.setType(0x026c);
	zipTlv.setLe(true);
	
	zipTlv.setData(convertUinToArray(zip));
	
	metaData.append(zipTlv.getData());
	
	
	tlv phoneTlv;
	phoneTlv.setLe(true);
	phoneTlv.setType(0x0276);
		
	QByteArray phoneArray;
	phoneArray.append(convertToLEByteArray((quint16)(basicPhone.length() + 1)));
	phoneArray.append(basicPhone);
	phoneArray.append(QChar(0x00));
		
		
	phoneTlv.setData(phoneArray);
	
	metaData.append(phoneTlv.getData());
	
	tlv faxTlv;
	faxTlv.setLe(true);
	faxTlv.setType(0x0280);
		
	QByteArray faxArray;
	faxArray.append(convertToLEByteArray((quint16)(basicFax.length() + 1)));
	faxArray.append(basicFax);
	faxArray.append(QChar(0x00));
		
		
	faxTlv.setData(faxArray);
	
	metaData.append(faxTlv.getData());
	
	tlv cellTlv;
	cellTlv.setLe(true);
	cellTlv.setType(0x028a);
		
	QByteArray cellArray;
	cellArray.append(convertToLEByteArray((quint16)(basicCell.length() + 1)));
	cellArray.append(basicCell);
	cellArray.append(QChar(0x00));
		
		
	cellTlv.setData(cellArray);
	
	metaData.append(cellTlv.getData());
	
	
	tlv streetTlv;
	streetTlv.setLe(true);
	streetTlv.setType(0x0262);
		
	QByteArray streetArray;
	streetArray.append(convertToLEByteArray((quint16)(basicAddress.length() + 1)));
	streetArray.append(basicAddress);
	streetArray.append(QChar(0x00));
		
		
	streetTlv.setData(streetArray);
	
	metaData.append(streetTlv.getData());
	
	
	
	
	tlv mcountryTlv;
	mcountryTlv.setType(0x0334);
	mcountryTlv.setLe(true);
	mcountryTlv.setData(convertToLEByteArray(moreCountry));
	
	metaData.append(mcountryTlv.getData());
	
	
	tlv ocityTlv;
	ocityTlv.setLe(true);
	ocityTlv.setType(0x0320);
		
	QByteArray ocityArray;
	ocityArray.append(convertToLEByteArray((quint16)(moreCity.length() + 1)));
	ocityArray.append(moreCity);
	ocityArray.append(QChar(0x00));
		
		
	ocityTlv.setData(ocityArray);
	
	metaData.append(ocityTlv.getData());
	
	
	tlv ostateTlv;
	ostateTlv.setLe(true);
	ostateTlv.setType(0x032a);
		
	QByteArray ostateArray;
	ostateArray.append(convertToLEByteArray((quint16)(moreState.length() + 1)));
	ostateArray.append(moreState);
	ostateArray.append(QChar(0x00));
		
		
	ostateTlv.setData(ostateArray);
	
	metaData.append(ostateTlv.getData());
	
	
	tlv wcountryTlv;
	wcountryTlv.setType(0x02b2);
	wcountryTlv.setLe(true);
	wcountryTlv.setData(convertToLEByteArray(workCountry));
	
	metaData.append(wcountryTlv.getData());
	
	
	tlv wcityTlv;
	wcityTlv.setLe(true);
	wcityTlv.setType(0x029e);
		
	QByteArray wcityArray;
	wcityArray.append(convertToLEByteArray((quint16)(workCity.length() + 1)));
	wcityArray.append(workCity);
	wcityArray.append(QChar(0x00));
			
			
	wcityTlv.setData(wcityArray);
		
	metaData.append(wcityTlv.getData());
		
	tlv wstateTlv;
	wstateTlv.setLe(true);
	wstateTlv.setType(0x02a8);
			
	QByteArray wstateArray;
	wstateArray.append(convertToLEByteArray((quint16)(workState.length() + 1)));
	wstateArray.append(workState);
	wstateArray.append(QChar(0x00));
			
			
	wstateTlv.setData(wstateArray);
		
	metaData.append(wstateTlv.getData());
		
		
		tlv wzipTlv;
		wzipTlv.setType(0x02bc);
		wzipTlv.setLe(true);
		
		wzipTlv.setData(convertUinToArray(wzip));
		
		metaData.append(wzipTlv.getData());
		
		
		tlv wphoneTlv;
		wphoneTlv.setLe(true);
		wphoneTlv.setType(0x02c6);
			
		QByteArray wphoneArray;
		wphoneArray.append(convertToLEByteArray((quint16)(workPhone.length() + 1)));
		wphoneArray.append(workPhone);
		wphoneArray.append(QChar(0x00));
			
			
		wphoneTlv.setData(wphoneArray);
		
		metaData.append(wphoneTlv.getData());
		
		tlv wfaxTlv;
		wfaxTlv.setLe(true);
		wfaxTlv.setType(0x02d0);
			
		QByteArray wfaxArray;
		wfaxArray.append(convertToLEByteArray((quint16)(workFax.length() + 1)));
		wfaxArray.append(workFax);
		wfaxArray.append(QChar(0x00));
			
			
		wfaxTlv.setData(wfaxArray);
		
		metaData.append(wfaxTlv.getData());
	
	
		tlv wstreetTlv;
		wstreetTlv.setLe(true);
		wstreetTlv.setType(0x0294);
			
		QByteArray wstreetArray;
		wstreetArray.append(convertToLEByteArray((quint16)(workAddress.length() + 1)));
		wstreetArray.append(workAddress);
		wstreetArray.append(QChar(0x00));
			
			
		wstreetTlv.setData(wstreetArray);
		
		metaData.append(wstreetTlv.getData());
	
		tlv companyTlv;
		companyTlv.setLe(true);
		companyTlv.setType(0x01ae);
			
		QByteArray companyArray;
		companyArray.append(convertToLEByteArray((quint16)(workCompany.length() + 1)));
		companyArray.append(workCompany);
		companyArray.append(QChar(0x00));
			
			
		companyTlv.setData(companyArray);
		
		metaData.append(companyTlv.getData());
	
	
	
		
		tlv occuptionTlv;
		occuptionTlv.setType(0x01cc);
		occuptionTlv.setLe(true);
		occuptionTlv.setData(convertToLEByteArray(workOccupation));
		
		metaData.append(occuptionTlv.getData());
		
		
		tlv depTlv;
		depTlv.setLe(true);
		depTlv.setType(0x01b8);
			
		QByteArray depArray;
		depArray.append(convertToLEByteArray((quint16)(workDepartment.length() + 1)));
		depArray.append(workDepartment);
		depArray.append(QChar(0x00));
			
			
		depTlv.setData(depArray);
		
		metaData.append(depTlv.getData());
		
		
		tlv positionTlv;
		positionTlv.setLe(true);
		positionTlv.setType(0x01c2);
			
		QByteArray positionArray;
		positionArray.append(convertToLEByteArray((quint16)(workPosition.length() + 1)));
		positionArray.append(workPosition);
		positionArray.append(QChar(0x00));
			
			
		positionTlv.setData(positionArray);
		
		metaData.append(positionTlv.getData());
		
		tlv websiteTlv;
		websiteTlv.setLe(true);
		websiteTlv.setType(0x02da);
			
		QByteArray websiteArray;
		websiteArray.append(convertToLEByteArray((quint16)(workWebPage.length() + 1)));
		websiteArray.append(workWebPage);
		websiteArray.append(QChar(0x00));
			
			
		websiteTlv.setData(websiteArray);
		
		metaData.append(websiteTlv.getData());
		
	
		
		tlv genderTlv;
		genderTlv.setType(0x017c);
		genderTlv.setLe(true);
		genderTlv.setData(convertToByteArray((quint8)foundedGender));
		metaData.append(genderTlv.getData());
		
		tlv homePageTlv;
		homePageTlv.setLe(true);
		homePageTlv.setType(0x0213);
			
		QByteArray homePageArray;
		homePageArray.append(convertToLEByteArray((quint16)(homepage.length() + 1)));
		homePageArray.append(homepage);
		homePageArray.append(QChar(0x00));
			
			
		homePageTlv.setData(homePageArray);
		
		metaData.append(homePageTlv.getData());
		
		tlv birthTlv;
		birthTlv.setType(0x023a);
		birthTlv.setLe(true);
		QByteArray birthDay;
		
		if ( setBirth )
		{
			birthDay.append(convertToLEByteArray((quint16)moreBirthYear));
			birthDay.append(convertToLEByteArray((quint16)moreBirthMonth));
			birthDay.append(convertToLEByteArray((quint16)moreBirthDay));
		} else
		{
			birthDay.append(convertToByteArray((quint16)0x0000));
			birthDay.append(convertToByteArray((quint16)0x0000));
			birthDay.append(convertToByteArray((quint16)0x0000));
		}
		
		birthTlv.setData(birthDay);
		metaData.append(birthTlv.getData());
		
		tlv langTlv;
		langTlv.setType(0x0186);
		langTlv.setLe(true);
		
		langTlv.setData(convertToLEByteArray(moreLang1));
		
		
		metaData.append(langTlv.getData());
		
		tlv langTlv2;
		langTlv2.setType(0x0186);
		langTlv2.setLe(true);
		
		langTlv2.setData(convertToLEByteArray(moreLang2));

		
		metaData.append(langTlv2.getData());
		
		tlv langTlv3;
		langTlv3.setType(0x0186);
		langTlv3.setLe(true);
		
		langTlv3.setData(convertToLEByteArray(moreLang3));
		
		
		metaData.append(langTlv3.getData());
		
		
		tlv interest1Tlv;
		interest1Tlv.setLe(true);
		interest1Tlv.setType(0x01ea);
			
		QByteArray interst1Array;
		interst1Array.append(convertToLEByteArray((quint16)interCode1));
		interst1Array.append(convertToLEByteArray((quint16)(interKeyWords1.length() + 1)));
		interst1Array.append(interKeyWords1);
		interst1Array.append(QChar(0x00));
			
			
		interest1Tlv.setData(interst1Array);
		
		metaData.append(interest1Tlv.getData());
		
		tlv interest2Tlv;
		interest2Tlv.setLe(true);
		interest2Tlv.setType(0x01ea);
			
		QByteArray interst2Array;
		interst2Array.append(convertToLEByteArray((quint16)interCode2));
		interst2Array.append(convertToLEByteArray((quint16)(interKeyWords2.length() + 1)));
		interst2Array.append(interKeyWords2);
		interst2Array.append(QChar(0x00));
			
			
		interest2Tlv.setData(interst2Array);
		
		metaData.append(interest2Tlv.getData());
		
		tlv interest3Tlv;
		interest3Tlv.setLe(true);
		interest3Tlv.setType(0x01ea);
			
		QByteArray interst3Array;
		interst3Array.append(convertToLEByteArray((quint16)interCode3));
		interst3Array.append(convertToLEByteArray((quint16)(interKeyWords3.length() + 1)));
		interst3Array.append(interKeyWords3);
		interst3Array.append(QChar(0x00));
			
			
		interest3Tlv.setData(interst3Array);
		
		metaData.append(interest3Tlv.getData());
		
		tlv interest4Tlv;
		interest4Tlv.setLe(true);
		interest4Tlv.setType(0x01ea);
			
		QByteArray interst4Array;
		interst4Array.append(convertToLEByteArray((quint16)interCode4));
		interst4Array.append(convertToLEByteArray((quint16)(interKeyWords4.length() + 1)));
		interst4Array.append(interKeyWords4);
		interst4Array.append(QChar(0x00));
			
			
		interest4Tlv.setData(interst4Array);
		
		metaData.append(interest4Tlv.getData());
		

		tlv aboutTlv;
		aboutTlv.setLe(true);
		aboutTlv.setType(0x0258);
			
		QByteArray aboutArray;
		aboutArray.append(convertToLEByteArray((quint16)(about.length() + 1)));
		aboutArray.append(about);
		aboutArray.append(QChar(0x00));
			
			
		aboutTlv.setData(aboutArray);
		
		metaData.append(aboutTlv.getData());
		
		tlv authTlv;
		authTlv.setType(0x030c);
		authTlv.setLe(true);
		authTlv.setData(convertToByteArray((quint8)webAware));
		metaData.append(authTlv.getData());
		
		tlv webTlv;
		webTlv.setType(0x02f8);
		webTlv.setLe(true);
		webTlv.setData(convertToByteArray((quint8)!authFlag));
		metaData.append(webTlv.getData());
		
	packet.append(convertToByteArray((quint16)(26 + metaData.length())));
	snac snac1502;
	snac1502.setFamily(0x0015);
	snac1502.setSubType(0x0002);
	snac1502.setReqId(snacReq);
	packet.append(snac1502.getData());
	packet.append(convertToByteArray((quint16)0x0001));
	packet.append(convertToByteArray((quint16)(12 +metaData.length())));
	
	packet.append(convertToLEByteArray((quint16)(metaData.length() + 10 )));
	packet.append(convertUinToArray(myUin));
	packet.append(convertToByteArray((quint16)0xd007));
	packet.append(convertToLEByteArray((quint16)metaReq));
	packet.append(convertToByteArray((quint16)0x3a0c));
	
	packet.append(metaData);

	tcpSocket->write(packet);
}

void metaInformation::sendMoreInfo(QTcpSocket *tcpSocket, const quint16 &flapReq, 
		const quint32 &snacReq, const quint16 &metaReq)
{
	QByteArray packet;
	packet[0] = 0x2a;
	packet[1] = 0x02;
	quint16 metaLength = 13 + homepage.length();
	packet.append(convertToByteArray((quint16)flapReq));
	packet.append(convertToByteArray((quint16)(metaLength + 26)));
	snac snac1502;
	snac1502.setFamily(0x0015);
	snac1502.setSubType(0x0002);
	snac1502.setReqId(snacReq);
	packet.append(snac1502.getData());
	
	packet.append(convertToByteArray((quint16)0x0001));
	packet.append(convertToByteArray((quint16)(12 + metaLength)));
	packet.append(convertToLEByteArray((quint16)(10 + metaLength)));
	packet.append(convertUinToArray(myUin));
	packet.append(convertToByteArray((quint16)0xd007));
	packet.append(convertToLEByteArray((quint16)metaReq));

	packet.append(convertToByteArray((quint16)0xfd03));
	
	packet.append(convertToByteArray((quint16)0x1200));
	packet.append(convertToByteArray((quint8)foundedGender));
	
			QByteArray homePageArray;
			homePageArray.append(convertToLEByteArray((quint16)(homepage.length() + 1)));
			homePageArray.append(homepage);
			homePageArray.append(QChar(0x00));
	
		packet.append(homePageArray);
	
		
				if ( setBirth )
				{
					packet.append(convertToLEByteArray((quint16)moreBirthYear));
					packet.append(convertToByteArray((quint8)moreBirthMonth));
					packet.append(convertToByteArray((quint8)moreBirthDay));
				} else
				{
					packet.append(convertToByteArray((quint16)0x0000));
					packet.append(convertToByteArray((quint8)0x00));
					packet.append(convertToByteArray((quint8)0x00));
				}
		
		
				packet.append(convertToByteArray((quint8)moreLang1));
				packet.append(convertToByteArray((quint8)moreLang2));
				packet.append(convertToByteArray((quint8)moreLang3));
				
	tcpSocket->write(packet);
}

void metaInformation::changePassword(QTcpSocket *tcpSocket, const quint16 &flapReq, 
		const quint32 &snacReq, const quint16 &metaReq, const QString &password)
{
	QByteArray packet;
	packet[0] = 0x2a;
	packet[1] = 0x02;
	packet.append(convertToByteArray((quint16)flapReq));
	packet.append(convertToByteArray((quint16)(29 + password.length())));
	snac snac1502;
	snac1502.setFamily(0x0015);
	snac1502.setSubType(0x0002);
	snac1502.setReqId(snacReq);
	packet.append(snac1502.getData());
	
	packet.append(convertToByteArray((quint16)0x0001));
	packet.append(convertToByteArray((quint16)(15 + password.length())));
	packet.append(convertToLEByteArray((quint16)(13 + password.length())));
	packet.append(convertUinToArray(myUin));
	packet.append(convertToByteArray((quint16)0xd007));
	packet.append(convertToLEByteArray((quint16)metaReq));
	packet.append(convertToByteArray((quint16)0x2e04));
	
	packet.append(convertToLEByteArray(password.length()));
	packet.append(password);
	packet.append(QChar(0x00));

	tcpSocket->write(packet);
}
