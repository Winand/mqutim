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


#ifndef METAINFORMATION_H_
#define METAINFORMATION_H_

#include <QString>

class QTcpSocket;
class icqBuffer;

class metaInformation
{
public:
	metaInformation(const QString &);
	~metaInformation();
	void sendShortInfoReq(QTcpSocket *, const quint16 &, 
			const quint32 &, const quint16 &, const QString &);
	void searchByUin(QTcpSocket *, const quint16 &, 
				const quint32 &, const quint16 &, const QString &);
	void searchByEmail(QTcpSocket *, const quint16 &, 
					const quint32 &, const quint16 &, QString);
	void searchByOther(QTcpSocket *, const quint16 &, 
						const quint32 &, const quint16 &, 
						bool, QString, QString, QString,
						quint8, quint16, quint16, quint16, QString,
						quint16, quint16, quint16, QString);
	
	void getFullUserInfo(QTcpSocket *, const quint16 &, 
			const quint32 &, const quint16 &, const QString &);
	
	void saveOwnerInfo(QTcpSocket *, const quint16 &, 
			const quint32 &, const quint16 &);
	
	void sendMoreInfo(QTcpSocket *, const quint16 &, 
	const quint32 &, const quint16 &);
	
	void changePassword(QTcpSocket *, const quint16 &, 
	const quint32 &, const quint16 &, const QString &);
	
	QString myUin;
	quint16 readShortInfo(icqBuffer *);
	quint16 readSearchResult(icqBuffer *,bool);
	quint16 readBasicUserInfo(icqBuffer *);
	quint16 readMoreUserInfo(icqBuffer *);
	quint16 readWorkUserInfo(icqBuffer *);
	quint16 readInterestsUserInfo(icqBuffer *);
	quint16 readAboutUserInfo(icqBuffer *);
	QByteArray nick;
	
	
	
	QString foundedUin;
	QString foundedNick;
	QString foundedFirst;
	QString foundedLast;
	QString foundedEmail;
	quint8 authFlag;
	quint16 foundedStatus;
	quint8 foundedGender;
	quint16 foundedAge;
	bool founded;
	bool basicInfoSuccess;
	bool moreInfoSuccess;
	bool workInfoSuccess;
	bool interestsInfoSuccess;
	bool aboutInfoSuccess;
	
	QByteArray basicNick;
	QByteArray basicFirst;
	QByteArray basicLast;
	QByteArray basicEmail;
	QByteArray basicCity;
	QByteArray basicState;
	QByteArray basicPhone;
	QByteArray basicFax;
	QByteArray basicAddress;
	QByteArray basicCell;
	QByteArray basicZip;
	QString zip;
	QString wzip;
	quint16 country;
	
	quint16 moreAge;
	quint8 moreGender;
	QByteArray homepage;
	quint16 moreBirthYear;
	quint8 moreBirthDay;
	quint8 moreBirthMonth;
	quint8 moreLang1;
	quint8 moreLang2;
	quint8 moreLang3;
	QByteArray moreCity;
	QByteArray moreState;
	quint16 moreCountry;
	
	QByteArray workCity;
	QByteArray workState;
	QByteArray workPhone;
	QByteArray workFax;
	QByteArray workAddress;

	QByteArray workZip;
	quint16 workCountry;
	QByteArray workCompany;
	QByteArray workDepartment;
	QByteArray workPosition;
	
	quint16 workOccupation;
	
	QByteArray workWebPage;
	
	
	quint16 interCode1;
	QByteArray interKeyWords1;
	quint16 interCode2;
	QByteArray interKeyWords2;
	quint16 interCode3;
	QByteArray interKeyWords3;
	quint16 interCode4;
	QByteArray interKeyWords4;
	
	QByteArray about;
	quint8 basicAuthFlag;
	quint8 webAware;
	quint8 publishEmail;
	bool setBirth;
	
private:
	QByteArray convertToByteArray(const quint8 &);
	QByteArray convertToByteArray(const quint16 &);
	QByteArray convertToLEByteArray(const quint16 &);
	QByteArray convertToByteArray(const quint32 &);
	QByteArray convertUinToArray(const QString &);
	quint16 byteArrayToLEInt16(const QByteArray &);
	quint32 byteArrayToLEInt32(const QByteArray &);
	quint8 convertToInt8(const QByteArray &);
};

#endif /*METAINFORMATION_H_*/
