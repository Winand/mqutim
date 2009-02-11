/*
Copyright (c) 2008 by Denis Daschenko <daschenko@gmail.com>
***************************************************************************
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
*                                                                         *
***************************************************************************
*/

#ifndef VCARDCONST_H
#define VCARDCONST_H

#include <QObject>

//class VCardRole

class VCardConst
{
	public:
		VCardConst() {};
		~VCardConst();
		static QString emptyString() {return QObject::tr("<font color='#808080'>%1</font>").arg("empty");}
		static QString personalMailStatus() {return QObject::tr("%1").arg("personal e-mail");}
		static QString workMailStatus() {return QObject::tr("%1").arg("work e-mail");}
		static QString emptyMailStatus() {return QObject::tr("%1").arg("uncertain e-mail");}
		static QString homePhoneStatus() {return QObject::tr("%1").arg("home phone");}
		static QString workPhoneStatus() {return QObject::tr("%1").arg("work phone");}
		static QString celluarPhoneStatus() {return QObject::tr("%1").arg("celluar phone");}
		static QString emptyPhoneStatus() {return QObject::tr("%1").arg("uncertain phone");}

};

#endif // VCARDCONST_H
