/*
    clientIdentify

    Copyright (c) 2008 by Alexey Ignatiev <twosev@gmail.com>

 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************
*/


#ifndef CLIENTIDENTIFY_H_
#define CLIENTIDENTIFY_H_

#if defined(Q_OS_WIN32)
  #define snprintf _snprintf
#endif

#include <QList>
#include <QByteArray>

typedef QList<QByteArray> ByteArrayList;

class treeBuddyItem;

class clientIdentify
{
 public:
  clientIdentify();
  ~clientIdentify();
  
  void addContactClientId(treeBuddyItem *);
 private:
  ByteArrayList client_caps;
  QList<quint16> client_shorts;
  char *capsbuf;
  unsigned capsize;
  quint8  client_proto;
  quint32 lastInfo;
  quint32 lastExtStatusInfo;
  quint32 lastExtInfo;

  void removeXstatus();
  bool identify_by_DCInfo(treeBuddyItem *);
  bool identify_by_Caps(treeBuddyItem *);
  bool identify_by_ProtoVersion(treeBuddyItem *);
  char *MatchBuddyCaps(char *, unsigned, const char *, unsigned short);
  bool MatchShortCaps(const QList<quint16> &, const quint16 &);

  char *identify_qutIM();
  char *identify_k8qutIM();
  char *identify_Miranda();
  char *identify_Qip();
  char *identify_QipInfium();
  char *identify_QipPDA();
  char *identify_QipMobile();
  char *identify_Sim();
  char *identify_SimRnQ();
  char *identify_Licq();
  char *identify_Kopete();
  char *identify_Micq();
  char *identify_LibGaim();
  char *identify_Jimm();
  char *identify_Mip();
  char *identify_Trillian();
  char *identify_Climm();
  char *identify_Im2();
  char *identify_AndRQ();
  char *identify_RandQ();
  char *identify_Imadering();
  char *identify_Mchat();
  char *identify_CorePager();
  char *identify_DiChat();
  char *identify_Macicq();
  char *identify_Anastasia();
  char *identify_Jicq();
  char *identify_Inlux();
  char *identify_Vmicq();
  char *identify_Smaper();
  char *identify_Yapp();
  char *identify_Pigeon();
  char *identify_NatIcq();
  char *identify_WebIcqPro();
  char *identify_BayanIcq();
};

#endif /*CLIENTIDENTIFY_H_*/
