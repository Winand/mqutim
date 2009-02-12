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

#include <QTextCodec>
#include "treebuddyitem.h"

#include "buddycaps.h"
#include "clientidentify.h"

//=============================================================================
clientIdentify::clientIdentify()
 : client_caps(), client_shorts()
{

}

//=============================================================================
clientIdentify::~clientIdentify()
{

}

//=============================================================================
void clientIdentify::addContactClientId(treeBuddyItem *buddy)
{
  client_caps = buddy->capabilitiesList;
  client_shorts = buddy->shortCaps;
  client_proto = buddy->protocolVersion;
  lastInfo = buddy->lastInfoUpdate;
  lastExtInfo = buddy->lastExtInfoUpdate;
  lastExtStatusInfo = buddy->lastExtStatusInfoUpdate;
  buddy->clientId = "";

  capsize = 0x10 * client_caps.size();
  capsbuf = (char *)malloc(sizeof(char) * (capsize + 1));
  for (int i = 0; i < client_caps.size(); ++i) {
    memcpy(capsbuf + i * 0x10, (const char *)client_caps[i], 0x10);
  }
  capsbuf[capsize] = '\0';

  // There may be some x-statuses info here.. remove all of them.
  removeXstatus();
  if ( !identify_by_Caps(buddy) ) {
  // Can't determine a buddy's client version using its capabilities.
    if ( !identify_by_ProtoVersion(buddy) ) {
    // Can't determine a buddy's client using its protocol version.
      if ( !identify_by_DCInfo(buddy) ) {
      // Can't determine a buddy's client even using direct connection info.
        // Well.. this is bad, I'm a loser.. :(
        buddy->clientId = "-";
        buddy->setClientIcon(QIcon(":/icons/clients/unknown.png"));
        buddy->updateBuddyText();
      }
    }
  }

  free(capsbuf);
}

//=============================================================================
void clientIdentify::removeXstatus()
{
  client_caps.removeAll(QByteArray::fromHex("01d8d7eeac3b492aa58dd3d877e66b92"));
  client_caps.removeAll(QByteArray::fromHex("5a581ea1e580430ca06f612298b7e4c7"));
  client_caps.removeAll(QByteArray::fromHex("83c9b78e77e74378b2c5fb6cfcc35bec"));
  client_caps.removeAll(QByteArray::fromHex("e601e41c33734bd1bc06811d6c323d81"));
  client_caps.removeAll(QByteArray::fromHex("8c50dbae81ed4786acca16cc3213c7b7"));
  client_caps.removeAll(QByteArray::fromHex("3fb0bd36af3b4a609eefcf190f6a5a7f"));
  client_caps.removeAll(QByteArray::fromHex("f8e8d7b282c4414290f810c6ce0a89a6"));
  client_caps.removeAll(QByteArray::fromHex("80537de2a4674a76b3546dfd075f5ec6"));
  client_caps.removeAll(QByteArray::fromHex("f18ab52edc57491d99dc6444502457af"));
  client_caps.removeAll(QByteArray::fromHex("1b78ae31fa0b4d3893d1997eeeafb218"));
  client_caps.removeAll(QByteArray::fromHex("61bee0dd8bdd475d8dee5f4baacf19a7"));
  client_caps.removeAll(QByteArray::fromHex("488e14898aca4a0882aa77ce7a165208"));
  client_caps.removeAll(QByteArray::fromHex("107a9a1812324da4b6cd0879db780f09"));
  client_caps.removeAll(QByteArray::fromHex("6f4930984f7c4affa27634a03bceaea7"));
  client_caps.removeAll(QByteArray::fromHex("1292e5501b644f66b206b29af378e48d"));
  client_caps.removeAll(QByteArray::fromHex("d4a611d08f014ec09223c5b6bec6ccf0"));
  client_caps.removeAll(QByteArray::fromHex("609d52f8a29a49a6b2a02524c5e9d260"));
  client_caps.removeAll(QByteArray::fromHex("63627337a03f49ff80e5f709cde0a4ee"));
  client_caps.removeAll(QByteArray::fromHex("1f7a4071bf3b4e60bc324c5787b04cf1"));
  client_caps.removeAll(QByteArray::fromHex("785e8c4840d34c65886f04cf3f3f43df"));
  client_caps.removeAll(QByteArray::fromHex("a6ed557e6bf744d4a5d4d2e7d95ce81f"));
  client_caps.removeAll(QByteArray::fromHex("12d07e3ef885489e8e97a72a6551e58d"));
  client_caps.removeAll(QByteArray::fromHex("ba74db3e9e24434b87b62f6b8dfee50f"));
  client_caps.removeAll(QByteArray::fromHex("634f6bd8add24aa1aab9115bc26d05a1"));
  client_caps.removeAll(QByteArray::fromHex("2ce0e4e57c6443709c3a7a1ce878a7dc"));
  client_caps.removeAll(QByteArray::fromHex("101117c9a3b040f981ac49e159fbd5d4"));
  client_caps.removeAll(QByteArray::fromHex("160c60bbdd4443f39140050f00e6c009"));
  client_caps.removeAll(QByteArray::fromHex("6443c6af22604517b58cd7df8e290352"));
  client_caps.removeAll(QByteArray::fromHex("16f5b76fa9d240358cc5c084703c98fa"));
  client_caps.removeAll(QByteArray::fromHex("631436ff3f8a40d0a5cb7b66e051b364"));
  client_caps.removeAll(QByteArray::fromHex("b70867f538254327a1ffcf4cc1939797"));
  client_caps.removeAll(QByteArray::fromHex("ddcf0ea971954048a9c6413206d6f280"));
  client_caps.removeAll(QByteArray::fromHex("d4e2b0ba334e4fa598d0117dbf4d3cc8"));
  client_caps.removeAll(QByteArray::fromHex("0072d9084ad143dd91996f026966026f"));
}

//=============================================================================
bool clientIdentify::identify_by_DCInfo(treeBuddyItem *buddy)
{
  char extra[256] = { 0 };
  if ((lastInfo & 0xff000000) == 0x7d000000) {
    unsigned long ver = lastInfo & 0xffff;
    snprintf(extra,
             sizeof(extra) - 1,
             "%lu.%lu",
             ver / 1000,
             (ver / 10) % 100);
    if(ver % 10) {
      sprintf(extra,
              "%s.%lu",
              extra,
              ver % 10);
    }

    buddy->clientId = "Licq v" + QString(extra);
    if ((lastInfo & 0x00ff0000) == 0x00800000)
      buddy->clientId += "/SSL";

    buddy->setClientIcon(QIcon(":/icons/clients/licq.png"));
    buddy->updateBuddyText();
  } else if (lastInfo == 0xffffffff) {
    if (lastExtStatusInfo == 0xffffffff) {
      buddy->clientId = "Gaim";
      buddy->setClientIcon(QIcon(":/icons/clients/gaim.png"));
      buddy->updateBuddyText();
    } else if ((!lastExtStatusInfo) && (client_proto == 7)) {
      buddy->clientId = "WebICQ";
      buddy->setClientIcon(QIcon(":/icons/clients/webicq.png"));
      buddy->updateBuddyText();
    } else if ((!lastExtStatusInfo) && (lastExtInfo == 0x3b7248ed)) {
      buddy->clientId = "Spam Bot";
      buddy->setClientIcon(QIcon(":/icons/clients/bot.png"));
      buddy->updateBuddyText();
    } else {
      unsigned ver1 = (lastExtStatusInfo >> 0x18) & 0xFF;
      unsigned ver2 = (lastExtStatusInfo >> 0x10) & 0xFF;
      unsigned ver3 = (lastExtStatusInfo >> 0x08) & 0xFF;
      unsigned ver4 = lastExtStatusInfo & 0xFF;

      if(ver1 & 0x80) {
      	snprintf(extra,
                 sizeof(extra) - 1,
                 "Miranda IM (ICQ 0.%hu.%hu.%hu alpha)",
                 ver2,
                 ver3,
                 ver4);
      } else {
      	snprintf(extra,
                 sizeof(extra) - 1,
                 "Miranda IM (ICQ %hu.%hu.%hu.%hu)",
                 ver1,
                 ver2,
                 ver3,
                 ver4);
      }

      buddy->clientId = QString(extra);
      if (lastExtInfo == 0x5AFEC0DE)
      	buddy->clientId += " (SecureIM)";
      else if ((lastExtInfo >> 0x18) == 0x80)
      	buddy->clientId += " (Unicode)";
      buddy->setClientIcon(QIcon(":/icons/clients/miranda.png"));
      buddy->updateBuddyText();
    }
  } else if (lastInfo == 0x7fffffff) {
    unsigned ver1 = (lastExtStatusInfo >> 0x18) & 0xFF;
    unsigned ver2 = (lastExtStatusInfo >> 0x10) & 0xFF;
    unsigned ver3 = (lastExtStatusInfo >> 0x08) & 0xFF;
    unsigned ver4 = lastExtStatusInfo & 0xFF;
    if(ver1 & 0x80) {
      snprintf(extra,
               sizeof(extra) - 1,
               "Miranda IM (ICQ 0.%hu.%hu.%hu alpha)",
               ver2,
               ver3,
               ver4);
    } else {
      snprintf(extra,
               sizeof(extra) - 1,
               "Miranda IM (ICQ %hu.%hu.%hu.%hu)",
               ver1,
               ver2,
               ver3,
               ver4);
    }

    buddy->clientId = QString(extra);
    if (lastExtInfo == 0x5AFEC0DE)
      buddy->clientId += " (SecureIM)";
    else if ((lastExtInfo >> 0x18) == 0x80)
      buddy->clientId += " (Unicode)";
    buddy->setClientIcon(QIcon(":/icons/clients/miranda.png"));
    buddy->updateBuddyText();
  } else if (lastInfo == 0x3b75ac09) {
    buddy->clientId = "Trillian";
    buddy->setClientIcon(QIcon(":/icons/clients/trillian.png"));
    buddy->updateBuddyText();
  } else if (lastInfo == 0xffffff42) {
    buddy->clientId = "mICQ";
    buddy->setClientIcon(QIcon(":/icons/clients/micq.png"));
    buddy->updateBuddyText();
  } else if (lastInfo == 0xffffff8f) {
    buddy->clientId = "StrICQ";
    buddy->setClientIcon(QIcon(":/icons/clients/unknown.png"));
    buddy->updateBuddyText();
  } else if (lastInfo == 0xffffff7f) {
    buddy->clientId = "&RQ";
    buddy->setClientIcon(QIcon(":/icons/clients/RQ.png"));
    buddy->updateBuddyText();
  } else if (lastInfo == 0xffffffab) {
    buddy->clientId = "YSM";
    buddy->setClientIcon(QIcon(":/icons/clients/unknown.png"));
    buddy->updateBuddyText();
  } else if (lastInfo == 0xffffffbe) {
    unsigned ver1 = (lastExtStatusInfo >> 0x18) & 0xff;
    unsigned ver2 = (lastExtStatusInfo >> 0x10) & 0xff;
    unsigned ver3 = (lastExtStatusInfo >> 0x08) & 0xff;
    snprintf(extra,
	     sizeof(extra) - 1,
	     "Alicq %d.%d.%d",
	     ver1,
	     ver2,
	     ver3);
    buddy->clientId = QString(extra);
    buddy->setClientIcon(QIcon(":/icons/clients/unknown.png"));
    buddy->updateBuddyText();
  } else if (lastInfo == 0x04031980) {
    buddy->clientId = "vICQ";
    buddy->setClientIcon(QIcon(":/icons/clients/unknown.png"));
    buddy->updateBuddyText();
  } else if ((lastInfo == 0x3aa773ee) && (lastExtStatusInfo == 0x3aa66380)) {
    buddy->clientId = "libicq2000";
    buddy->setClientIcon(QIcon(":/icons/clients/icq2000.png"));
    if (MatchBuddyCaps(capsbuf, capsize, ICQ_CAPABILITY_RTFxMSGS, 0x10)) {
      buddy->clientId = "Centericq";
      buddy->setClientIcon(QIcon(":/icons/clients/centericq.png"));
    } else if (MatchBuddyCaps(capsbuf,
                              capsize,
                              ICQ_CAPABILITY_UTF8,
                              0x10) ||
               MatchShortCaps(client_shorts, ICQ_SHORTCAP_UTF)) {
      buddy->clientId += " (Unicode)";
    }
    buddy->updateBuddyText();
  } else if ((lastInfo == 0x3ba8dbaf) && (client_proto == 2)) {
    buddy->clientId = "stICQ";
    buddy->setClientIcon(QIcon(":/icons/clients/sticq.png"));
    buddy->updateBuddyText();
  } else if ((lastInfo == 0xfffffffe) && (lastExtInfo == 0xfffffffe)) {
    buddy->clientId = "Jimm";
    buddy->setClientIcon(QIcon(":/icons/clients/jimm.png"));
    buddy->updateBuddyText();
  } else if ((lastInfo == 0x3ff19beb) && (lastExtInfo == 0x3ff19beb)) {
    buddy->clientId = "IM2";
    buddy->setClientIcon(QIcon(":/icons/clients/im2.png"));
    buddy->updateBuddyText();
  } else if ((lastInfo == 0xddddeeff) &&
             (!lastExtStatusInfo) &&
             (!lastExtInfo)) {
    buddy->clientId = "SmartICQ";
    buddy->setClientIcon(QIcon(":/icons/clients/unknown.png"));
    buddy->updateBuddyText();
  } else if (((lastInfo & 0xfffffff0) == 0x494d2b00) &&
             (!lastExtStatusInfo) &&
             (!lastExtInfo)) {
    buddy->clientId = "IM+";
    buddy->setClientIcon(QIcon(":/icons/clients/implus.png"));
    buddy->updateBuddyText();
  } else if ((lastInfo == 0x3b4c4c0c) &&
             (!lastExtStatusInfo) &&
             (lastExtInfo == 0x3b7248ed)) {
    buddy->clientId = "KXicq2";
    buddy->setClientIcon(QIcon(":/icons/clients/kxicq.png"));
    buddy->updateBuddyText();
  } else if ((lastInfo == 0xfffff666)/* && (!lastExtInfo)*/) {
    snprintf(extra, sizeof(extra) - 1, "R&Q %u", (unsigned)lastExtStatusInfo);
    buddy->clientId = QString(extra);
    buddy->setClientIcon(QIcon(":/icons/clients/rnq.png"));
    buddy->updateBuddyText();
  } else if ((lastInfo == 0x66666666) && (lastExtInfo == 0x66666666)) {
    buddy->clientId = "D[i]Chat";
    buddy->setClientIcon(QIcon(":/icons/clients/di_chat.png"));
    buddy->updateBuddyText();
    if (lastExtStatusInfo == 0x10000)
      buddy->clientId += " v.0.1a";
    else if (lastExtStatusInfo == 0x22)
      buddy->clientId += " v.0.2b";
  } else if ((lastInfo == 0x44F523B0) &&
             (lastExtStatusInfo == 0x44F523A6) &&
             (lastExtInfo == 0x44F523A6) &&
             (client_proto == 8)) {
    buddy->clientId = "Virus";
    buddy->setClientIcon(QIcon(":/icons/clients/unknown.png"));
    buddy->updateBuddyText();
  }

  if ( buddy->clientId.compare("") ) return true;
  else return false;
}

//=============================================================================
bool clientIdentify::identify_by_Caps(treeBuddyItem *buddy)
{
  char *tmp = 0;
  if ((tmp = identify_qutIM())) {
    buddy->setClientIcon(QIcon(":/icons/clients/qutim.png"));
  } else if ((tmp = identify_k8qutIM())) {
    buddy->setClientIcon(QIcon(":/icons/clients/k8qutim.png"));
  } else if ((tmp = identify_Miranda())) {
    buddy->setClientIcon(QIcon(":/icons/clients/miranda.png"));
  } else if ((tmp = identify_Qip())) {
    buddy->setClientIcon(QIcon(":/icons/clients/qip.png"));
  } else if ((tmp = identify_QipInfium())) {
    buddy->setClientIcon(QIcon(":/icons/clients/qipinf.png"));
  } else if ((tmp = identify_QipPDA())) {
    buddy->setClientIcon(QIcon(":/icons/clients/qippda.png"));
  } else if ((tmp = identify_QipMobile())) {
    buddy->setClientIcon(QIcon(":/icons/clients/qipsymb.png"));
  } else if ((tmp = identify_SimRnQ())) {
    buddy->setClientIcon(QIcon(":/icons/clients/rnq.png"));
  } else if ((tmp = identify_Sim())) {
    buddy->setClientIcon(QIcon(":/icons/clients/sim.png"));
  } else if ((tmp = identify_Licq())) {
    buddy->setClientIcon(QIcon(":/icons/clients/licq.png"));
  } else if ((tmp = identify_Kopete())) {
    buddy->setClientIcon(QIcon(":/icons/clients/kopete.png"));
  } else if ((tmp = identify_Micq())) {
    buddy->setClientIcon(QIcon(":/icons/clients/micq.png"));
  } else if ((tmp = identify_LibGaim())) {
    if ( !strcmp(tmp, "Pidgin/AdiumX") )
      buddy->setClientIcon(QIcon(":/icons/clients/pidgin.png"));
    else if ( !strcmp(tmp, "Meebo") )
      buddy->setClientIcon(QIcon(":/icons/clients/meebo.png"));
    else
      buddy->setClientIcon(QIcon(":/icons/clients/gaim.png"));
  } else if ((tmp = identify_Jimm())) {
    buddy->setClientIcon(QIcon(":/icons/clients/jimm.png"));
  } else if ((tmp = identify_Mip())) {
    buddy->setClientIcon(QIcon(":/icons/clients/mip.png"));
  } else if ((tmp = identify_Trillian())) {
    buddy->setClientIcon(QIcon(":/icons/clients/trillian.png"));
  } else if ((tmp = identify_Climm())) {
    buddy->setClientIcon(QIcon(":/icons/clients/unknown.png"));
  } else if ((tmp = identify_Im2())) {
    buddy->setClientIcon(QIcon(":/icons/clients/im2.png"));
  } else if ((tmp = identify_AndRQ())) {
    buddy->setClientIcon(QIcon(":/icons/clients/RQ.png"));
  } else if ((tmp = identify_RandQ())) {
    buddy->setClientIcon(QIcon(":/icons/clients/rnq.png"));
  } else if ((tmp = identify_Imadering())) {
    buddy->setClientIcon(QIcon(":/icons/clients/unknown.png"));
  } else if ((tmp = identify_Mchat())) {
    buddy->setClientIcon(QIcon(":/icons/clients/mchat.png"));
  } else if ((tmp = identify_CorePager())) {
    buddy->setClientIcon(QIcon(":/icons/clients/corepager.png"));
  } else if ((tmp = identify_DiChat())) {
    buddy->setClientIcon(QIcon(":/icons/clients/di_chat.png"));
  } else if ((tmp = identify_Macicq())) {
    buddy->setClientIcon(QIcon(":/icons/clients/icq_mac.png"));
  } else if ((tmp = identify_Anastasia())) {
    buddy->setClientIcon(QIcon(":/icons/clients/anastasia.png"));
  } else if ((tmp = identify_Jicq())) {
    buddy->setClientIcon(QIcon(":/icons/clients/jicq.png"));
  } else if ((tmp = identify_Inlux())) {
    buddy->setClientIcon(QIcon(":/icons/clients/inlux.png"));
  } else if ((tmp = identify_Vmicq())) {
    buddy->setClientIcon(QIcon(":/icons/clients/vmicq.png"));
  } else if ((tmp = identify_Smaper())) {
    buddy->setClientIcon(QIcon(":/icons/clients/smaper.png"));
  } else if ((tmp = identify_Yapp())) {
    buddy->setClientIcon(QIcon(":/icons/clients/yapp.png"));
  } else if ((tmp = identify_Pigeon())) {
    buddy->setClientIcon(QIcon(":/icons/clients/pigeon.png"));
  } else if ((tmp = identify_NatIcq())) {
    buddy->setClientIcon(QIcon(":/icons/clients/naticq.png"));
  } else if ((tmp = identify_WebIcqPro())) {
    buddy->setClientIcon(QIcon(":/icons/clients/webicq.png"));
  } else if ((tmp = identify_BayanIcq())) {
    buddy->setClientIcon(QIcon(":/icons/clients/bayanicq.png"));
  }

  if (tmp != 0) {
    buddy->clientId = QTextCodec::codecForName("CP1251")->makeDecoder()->toUnicode(tmp);
    buddy->updateBuddyText();
    free(tmp);

    return true;
  }

  return false;
}

//=============================================================================
bool clientIdentify::identify_by_ProtoVersion(treeBuddyItem *buddy)
{
  bool bRTF      = MatchBuddyCaps(capsbuf, capsize, ICQ_CAPABILITY_RTFxMSGS, 0x10);
  bool bTYPING   = MatchBuddyCaps(capsbuf, capsize, ICQ_CAPABILITY_TYPING, 0x10);
  bool bAIMCHAT  = MatchBuddyCaps(capsbuf, capsize, ICQ_CAPABILITY_AIMCHAT, 0x10);
  bool bXTRAZ    = MatchBuddyCaps(capsbuf, capsize, ICQ_CAPABILITY_XTRAZ, 0x10);

  bool bUTF8     = MatchBuddyCaps(capsbuf, capsize, ICQ_CAPABILITY_UTF8, 0x10) ||
    MatchShortCaps(client_shorts, ICQ_SHORTCAP_UTF);
  bool bSENDFILE = MatchBuddyCaps(capsbuf, capsize, ICQ_CAPABILITY_AIMSENDFILE, 0x10) ||
    MatchShortCaps(client_shorts, ICQ_SHORTCAP_SENDFILE);
  bool bDIRECT   = MatchBuddyCaps(capsbuf, capsize, ICQ_CAPABILITY_DIRECT, 0x10) ||
    MatchShortCaps(client_shorts, ICQ_SHORTCAP_DIRECT);
  bool bAIMICON  = MatchBuddyCaps(capsbuf, capsize, ICQ_CAPABILITY_AIMICON, 0x10) ||
    MatchShortCaps(client_shorts, ICQ_SHORTCAP_BUDDYCON);
  bool bGETFILE  = MatchBuddyCaps(capsbuf, capsize, ICQ_CAPABILITY_AIMGETFILE, 0x10) ||
    MatchShortCaps(client_shorts, ICQ_SHORTCAP_GETFILE);
  bool bSRVRELAY = MatchBuddyCaps(capsbuf, capsize, ICQ_CAPABILITY_SRVxRELAY, 0x10) ||
    MatchShortCaps(client_shorts, ICQ_SHORTCAP_RELAY);
  bool bAVATAR   = MatchBuddyCaps(capsbuf, capsize, ICQ_CAPABILITY_AVATAR, 0x10) ||
    MatchShortCaps(client_shorts, ICQ_SHORTCAP_AVATAR);

  int capsnum = client_caps.size() + client_shorts.size();

  // VERSION = 0
  if (client_proto == 0) {
    if (!lastInfo &&
        !lastExtStatusInfo &&
        !lastExtInfo &&
        !(buddy->userPort) &&
        !(buddy->Cookie)) {
      if (bTYPING &&
          (MatchBuddyCaps(capsbuf,
                          capsize,
                          ICQ_CAPABILITY_IS2001,
                          0x10)) &&
          (MatchBuddyCaps(capsbuf,
                          capsize,
                          ICQ_CAPABILITY_IS2002,
                          0x10)) &&
          (MatchBuddyCaps(capsbuf,
                          capsize,
                          ICQ_CAPABILITY_COMM20012,
                          0x10))) {
        buddy->clientId = "Spam Bot";
        buddy->setClientIcon(QIcon(":/icons/clients/bot.png"));
      } else if (bAIMICON) {
        if ((bUTF8 && (capsnum == 2)) ||
            (MatchBuddyCaps(capsbuf,
                            capsize,
                            ICQ_CAPABILITY_ICQLITExVER,
                            0x10) &&
             bXTRAZ)) {
          buddy->clientId = "PyICQ-t Jabber Transport";
          buddy->setClientIcon(QIcon(":/icons/clients/pyicq.png"));
        } else if (bUTF8 &&
                   bSRVRELAY &&
                   bSENDFILE &&
                   (MatchShortCaps(client_shorts, ICQ_SHORTCAP_AIMBUDDYLIST)) &&
                   (MatchShortCaps(client_shorts, 0x0002))) {
          buddy->clientId = "Digsby";
          buddy->setClientIcon(QIcon(":/icons/clients/digsby.png"));
        }
      } else if (bAIMCHAT) {
        if (bSENDFILE) {
          if (capsnum == 2) {
            buddy->clientId = "Easy Message";
            buddy->setClientIcon(QIcon(":/icons/clients/unknown.png"));
          } else if (bAIMICON &&
                     MatchShortCaps(client_shorts, ICQ_SHORTCAP_AIMIMAGE)) {
            buddy->clientId = "AIM";
            buddy->setClientIcon(QIcon(":/icons/clients/aim.png"));
          }
        } else if (bUTF8 && (capsnum == 2)) {
          buddy->clientId = "BeejiveIM";
          buddy->setClientIcon(QIcon(":/icons/clients/beejive.png"));
        }
      } else if (bUTF8 && bSRVRELAY && bDIRECT) {
        if ((bTYPING && (capsnum == 4)) ||
            (bAIMICON && bAVATAR && (capsnum = 5))) {
          buddy->clientId = "Agile Messenger";
          buddy->setClientIcon(QIcon(":/icons/clients/agile.png"));
        } else if (bSENDFILE && bGETFILE) {
          buddy->clientId = "Slick";
          buddy->setClientIcon(QIcon(":/icons/clients/slick.png"));
        }
      } else if(bDIRECT && bRTF && bTYPING && bUTF8) {
        buddy->clientId = "GlICQ";
        buddy->setClientIcon(QIcon(":/icons/clients/glicq.png"));
      }
    }
  }

  // VERSION = 7
  else if (client_proto == 7) {
    if (bRTF) {
      if (bSRVRELAY && bDIRECT &&
          (lastInfo == 0x3aa773ee) &&
          (lastExtStatusInfo == 0x3aa66380) &&
          (lastExtInfo == 0x3a877a42)) {
        buddy->clientId = "centerim";
        buddy->setClientIcon(QIcon(":/icons/clients/centericq.png"));
      } else {
        buddy->clientId = "GnomeICU";
        buddy->setClientIcon(QIcon(":/icons/clients/unknown.png"));
      }
    } else if (bSRVRELAY) {
      if (!lastInfo && !lastExtStatusInfo && !lastExtInfo) {
        buddy->clientId = "&RQ";
        buddy->setClientIcon(QIcon(":/icons/clients/RQ.png"));
      } else {
        buddy->clientId = "ICQ 2000";
        buddy->setClientIcon(QIcon(":/icons/clients/icq2000.png"));
      }
    } else if (bUTF8) {
      if (bTYPING)
        buddy->clientId = "Icq2Go! (Java)";
      else
        buddy->clientId = "Icq2Go! (Flash)";
      buddy->setClientIcon(QIcon(":/icons/clients/icq2go.png"));
    }
  }

  // VERSION = 8
  else if (client_proto == 8) {
    if (bXTRAZ) {
      if (bDIRECT && bUTF8 && bSRVRELAY && bAVATAR) {
        buddy->clientId = "IM Gate";
        buddy->setClientIcon(QIcon(":/icons/clients/imgate.png"));
      } else if ((MatchBuddyCaps(capsbuf,
                                 capsize,
                                 ICQ_CAPABILITY_IMSECKEY1,
                                 0x06)) &&
                 (MatchBuddyCaps(capsbuf,
                                 capsize,
                                 ICQ_CAPABILITY_IMSECKEY2,
                                 0x06))) {
        client_proto = 9;
        return identify_by_ProtoVersion(buddy);
      }
    } else if ((MatchBuddyCaps(capsbuf,
                               capsize,
                               ICQ_CAPABILITY_COMM20012,
                               0x10)) || (bSRVRELAY)) {
      if (MatchBuddyCaps(capsbuf,
                         capsize,
                         ICQ_CAPABILITY_IS2001,
                         0x10)) {
        if (!lastInfo && !lastExtStatusInfo && !lastExtInfo) {
          if (bRTF) {
            buddy->clientId = "TICQClient"; // possibly also older GnomeICU
            buddy->setClientIcon(QIcon(":/icons/clients/unknown.png"));
          } else {
            buddy->clientId = "ICQ for Pocket PC";
            buddy->setClientIcon(QIcon(":/icons/clients/unknown.png"));
          }
        } else {
          buddy->clientId = "ICQ 2001";
          buddy->setClientIcon(QIcon(":/icons/clients/icq2001.png"));
        }
      } else if (MatchBuddyCaps(capsbuf,
                                capsize,
                                ICQ_CAPABILITY_IS2002,
                                0x10)) {
        buddy->clientId = "ICQ 2002";
        buddy->setClientIcon(QIcon(":/icons/clients/icq.png"));
      } else if (bSRVRELAY && bUTF8 && bRTF &&
                 (!MatchBuddyCaps(capsbuf,
                                  capsize,
                                  ICQ_CAPABILITY_ICQJS7xVER,
                                  0x04)) &&
                 (!MatchBuddyCaps(capsbuf,
                                  capsize,
                                  ICQ_CAPABILITY_ICQJSINxVER,
                                  0x04))) {
        if (!lastInfo && !lastExtStatusInfo && !lastExtInfo) {
          if (!(buddy->userPort)) {
            buddy->clientId = "GnomeICU 0.99.5+";
            buddy->setClientIcon(QIcon(":/icons/clients/unknown.png"));
          } else {
            buddy->clientId = "IC@";
            buddy->setClientIcon(QIcon(":/icons/clients/unknown.png"));
          }
        } else {
          buddy->clientId = "ICQ 2002/2003a";
          buddy->setClientIcon(QIcon(":/icons/clients/icq.png"));
        }
      } else if (bSRVRELAY && bUTF8 && bTYPING) {
        if (!lastInfo && !lastExtStatusInfo && !lastExtInfo) {
          buddy->clientId = "PreludeICQ";
          buddy->setClientIcon(QIcon(":/icons/clients/unknown.png"));
        }
      }
    }
  }

  // VERSION = 9
  else if (client_proto == 9) {
    if (bXTRAZ) {
      if (bSENDFILE) {
        if (MatchBuddyCaps(capsbuf,
                           capsize,
                           ICQ_CAPABILITY_TZERS,
                           0x10)) {
          if (MatchBuddyCaps(capsbuf,
                             capsize,
                             ICQ_CAPABILITY_HTMLMSGS,
                             0x10)) {
            if (bRTF) {
              buddy->clientId = "MDC";
              buddy->setClientIcon(QIcon(":/icons/clients/unknown.png"));
            } else {
              buddy->clientId = "ICQ 6";
              buddy->setClientIcon(QIcon(":/icons/clients/icq60.png"));
            }
          } else {
            buddy->clientId = "ICQ 5.1";
            buddy->setClientIcon(QIcon(":/icons/clients/icq51.png"));
          }
        } else {
          buddy->clientId = "ICQ 5";
          buddy->setClientIcon(QIcon(":/icons/clients/icq50.png"));
        }
        if (MatchBuddyCaps(capsbuf,
                           capsize,
                           ICQ_CAPABILITY_RAMBLER,
                           0x10))
          buddy->clientId += " (Rambler)";
        else if (MatchBuddyCaps(capsbuf,
                                capsize,
                                ICQ_CAPABILITY_ABV,
                                0x10))
          buddy->clientId += " (Abv)";
        else if (MatchBuddyCaps(capsbuf,
                                capsize,
                                ICQ_CAPABILITY_NETVIGATOR,
                                0x10))
          buddy->clientId += " (Netvigator)";
      } else if (!bDIRECT) {
        if (bRTF) {
          buddy->clientId = "QNext";
          buddy->setClientIcon(QIcon(":/icons/clients/unknown.png"));
        } else {
          buddy->clientId = "PyICQ-t Jabber Transport";
          buddy->setClientIcon(QIcon(":/icons/clients/pyicq.png"));
        }
      } else {
        buddy->clientId = "ICQ Lite v4";
        buddy->setClientIcon(QIcon(":/icons/clients/icq4lite.png"));
      }
    } else if (!bDIRECT && bUTF8 && !bRTF) {
      buddy->clientId = "PyICQ-t Jabber Transport";
      buddy->setClientIcon(QIcon(":/icons/clients/pyicq.png"));
    }
  }

  // VERSION = 10
  else if (client_proto == 0xA) {
    if (bRTF && bUTF8 && bTYPING && bDIRECT && bSRVRELAY) {
      buddy->clientId = "ICQ 2003b Pro";
      buddy->setClientIcon(QIcon(":/icons/clients/icq2003pro.png"));
    } else if (!bRTF && !bUTF8) {
      buddy->clientId = "QNext";
      buddy->setClientIcon(QIcon(":/icons/clients/unknown.png"));
    } else if (!bRTF &&
               bUTF8 &&
               !lastInfo &&
               !lastExtStatusInfo &&
               !lastExtInfo) {
      buddy->clientId = "NanoICQ";
      buddy->setClientIcon(QIcon(":/icons/clients/unknown.png"));
    }
  }

  if ( buddy->clientId.compare("") ) {
    buddy->updateBuddyText();
    return true;
  }
  return false;
}

//=============================================================================
char *clientIdentify::MatchBuddyCaps(
  char *buf,
  unsigned bufsize,
  const char *cap,
  unsigned short capsize
)
{
  while (bufsize > 0) {
    if (!memcmp(buf, cap, capsize)) {
      // the capability is found
      return (char *)buf;
    } else {
      buf += 0x10;
      bufsize -= 0x10;
    }
  }
  // there is not such a capability.
  return 0;
}

//=============================================================================
bool clientIdentify::MatchShortCaps(
  const QList<quint16> &buf,
  const quint16 &cap
)
{
  if (buf.contains(cap)) {
    return true;
  } else
    return false;
}

//=============================================================================
char *clientIdentify::identify_qutIM()
{
  char *capID;
  if ((capID = MatchBuddyCaps(capsbuf,
                              capsize,
                              ICQ_CAPABILITY_QUTIMxVER,
                              strlen(ICQ_CAPABILITY_QUTIMxVER)))) {
    char *res = (char *)malloc(sizeof(char) * 256);
    char *verStr = capID + 5;
    if (verStr[1] == 46) {
      // old qutim id
      int ver1 = verStr[0] - 48;
      int ver2 = verStr[2] - 48;
      snprintf(res, 255, "qutIM v%u.%u", ver1, ver2);
    } else {
      // new qutim id
      char os[13] = { 0 };
      switch (verStr[0]) {
      case 'l':
        strcpy(os, "(Linux)");
        break;
      case 'w':
        strcpy(os, "(Windows)");
        break;
      case 'm':
        strcpy(os, "(MacOS)");
        break;
      case 'u':
      default:
        strcpy(os, "(Unknown OS)");
        break;
      }
      int ver1 = verStr[1];
      int ver2 = verStr[2];
      int ver3 = verStr[3];
      unsigned char svn1 = verStr[4];
      unsigned char svn2 = verStr[5];
      if (ver1 == 0x42) {
        snprintf(res,
                 255,
                 "oldk8 v%i.%i (%u)",
                 ver2,
                 ver3,
                 (unsigned int)((svn1 << 0x08) | svn2));
      } else {
        snprintf(res, 255, "qutIM v%i.%i.%i", ver1, ver2, ver3);
        if (svn1 | svn2) {
          sprintf(res,
                  "%s svn%u",
                  res,
                  (unsigned int)((svn1 << 0x08) | svn2));
        }
      }
      sprintf(res,"%s %s", res, os);
    }
    return res;
  }
  return 0;
}

//=============================================================================
char *clientIdentify::identify_k8qutIM()
{
  char *capID;
  if ((capID = MatchBuddyCaps(capsbuf,
                              capsize,
                              ICQ_CAPABILITY_K8QUTIMxVER,
                              strlen(ICQ_CAPABILITY_K8QUTIMxVER)))) {
    char *res = (char *)malloc(sizeof(char) * 256);
    char *verStr = capID + 7;
    char os[10] = { 0 };

    if (verStr[0] == 'l')
      strcpy(os, "");
    else
      snprintf(os, 8, " (%c)", verStr[0]);

    verStr += 2;
    int ver0 = verStr[0];
    int ver1 = verStr[1];
    int ver2 = verStr[2];
    unsigned char rel0 = verStr[3];
    unsigned char rel1 = verStr[4];
    snprintf(res,
             255,
             "k8qutIM v%i.%i.%i.%u",
             ver0,
             ver1,
             ver2,
             (unsigned int)((rel0 << 8) | rel1));
    strcat(res, os);
    return res;
  }
  return 0;
}

//=============================================================================
char *clientIdentify::identify_Miranda()
{
  char *capID;
  char tmpVer[256] = { 0 };
  if ((capID = MatchBuddyCaps(capsbuf,
                              capsize,
                              ICQ_CAPABILITY_ICQJSINxVER,
                              0x04)) ||
      (capID = MatchBuddyCaps(capsbuf,
                              capsize,
                              ICQ_CAPABILITY_ICQJS7xVER,
                              0x04)) ||
      (capID = MatchBuddyCaps(capsbuf,
                              capsize,
                              ICQ_CAPABILITY_ICQJPxVER,
                              0x04)) ||
      (capID = MatchBuddyCaps(capsbuf,
                              capsize,
                              ICQ_CAPABILITY_ICQJENxVER,
                              0x04))) {
    unsigned mver0   = capID[0x4] & 0xff;
    unsigned mver1   = capID[0x5] & 0xff;
    unsigned mver2   = capID[0x6] & 0xff;
    unsigned mver3   = capID[0x7] & 0xff;
    unsigned iver0   = capID[0x8] & 0xff;
    unsigned iver1   = capID[0x9] & 0xff;
    unsigned iver2   = capID[0xa] & 0xff;
    unsigned iver3   = capID[0xb] & 0xff;
    unsigned secure  = capID[0xc] & 0xff;
    unsigned ModVer  = capID[0x3] & 0xff;
    char *res = (char *)malloc(sizeof(char) * 256);
    unsigned Unicode = (lastExtInfo >> 24) & 0xff;
    if ((mver1 < 20) && (iver1 < 20)) {
      strcpy(res, "Miranda IM ");
      if (mver0 == 0x80) {
        if (mver2 == 0x00) {
          snprintf(tmpVer,
                   sizeof(tmpVer) - 1,
                   "0.%u alpha build #%u",
                   mver1,
                   mver3);
        } else {
          snprintf(tmpVer,
                   sizeof(tmpVer) - 1,
                   "0.%u.%u alpha build #%u",
                   mver1,
                   mver2,
                   mver3);
        }
        strcat(res, tmpVer);
      } else {
        if (mver2 == 0x00) {
          snprintf(tmpVer,
                   sizeof(tmpVer) - 1,
                   "%u.%u",
                   mver0,
                   mver1);
        } else {
          snprintf(tmpVer,
                   sizeof(tmpVer) - 1,
                   "%u.%u.%u",
                   mver0,
                   mver1,
                   mver2);
        }
        strcat(res, tmpVer);
        if ((mver3 != 0x00) && (mver3 != 0x64)) {
          snprintf(tmpVer,
                   sizeof(tmpVer) - 1,
                   " alpha build #%u",
                   mver3);
          strcat(res, tmpVer);
        }
      }
      if ((Unicode == 0x80) || (lastInfo == 0x7fffffff))
        strcat(res, " Unicode");
      if (ModVer == 'p')
        strcat(res, " (ICQ Plus");
      else if (capID[0x0] == 's')
        strcat(res, " (ICQ S!N");
      else if (capID[0x0] == 'e')
        strcat(res, " (ICQ eternity/PlusPlus++");
      else if (ModVer == 'j')
        strcat(res, " (ICQ S7 & SSS");
      if (iver0 == 0x80) {
        snprintf(tmpVer,
                 sizeof(tmpVer) - 1,
                 " 0.%u.%u.%u alpha)",
                 iver1,
                 iver2,
                 iver3);
        strcat(res, tmpVer);
      } else {
        snprintf(tmpVer,
                 sizeof(tmpVer) - 1,
                 " %u.%u.%u.%u)",
                 iver0,
                 iver1,
                 iver2,
                 iver3);
        strcat(res, tmpVer);
      }
      if (((secure != 0) && (secure != 20)) || (lastExtInfo == 0x5AFEC0DE))
        strcat(res, " (SecureIM)");
      else if (MatchBuddyCaps(capsbuf,
                              capsize,
                              ICQ_CAPABILITY_ICQJS7SxVER,
                              0x10))
        strcpy(res, "Miranda IM (ICQ SSS & S7)(SecureIM)");
      else if (MatchBuddyCaps(capsbuf,
                              capsize,
                              ICQ_CAPABILITY_ICQJS7OxVER,
                              0x10))
        strcpy(res, "Miranda IM (ICQ SSS & S7)");
    }
    return res;
  } else if ((capID = MatchBuddyCaps(capsbuf,
				     capsize,
				     ICQ_CAPABILITY_MIRANDAxVER,
				     0x08))) {
    unsigned mver0 = capID[0x8] & 0xff;
    unsigned mver1 = capID[0x9] & 0xff;
    unsigned mver2 = capID[0xA] & 0xff;
    unsigned mver3 = capID[0xB] & 0xff;
    unsigned iver0 = capID[0xC] & 0xff;
    unsigned iver1 = capID[0xD] & 0xff;
    unsigned iver2 = capID[0xE] & 0xff;
    unsigned iver3 = capID[0xF] & 0xff;
    char *res = (char *)malloc(sizeof(char) * 256);
    strcpy(res, "Miranda IM ");
    if (MatchBuddyCaps(capsbuf,
                       capsize,
                       ICQ_CAPABILITY_MIRMOBxVER,
                       0x0d))
      strcat(res, "Mobile ");
    if (mver0 == 0x80) {
      if (mver2 == 0x00) {
        snprintf(tmpVer,
                 sizeof(tmpVer) - 1,
                 "0.%u alpha build #%u",
                 mver1,
                 mver3);
      } else {
        snprintf(tmpVer,
                 sizeof(tmpVer),
                 "0.%u.%u preview #%u",
                 mver1,
                 mver2,
                 mver3);
      }
      strcat(res, tmpVer);
    } else {
      if (mver2 == 0x00) {
        snprintf(tmpVer,
                 sizeof(tmpVer) - 1,
                 "%u.%u",
                 mver0,
                 mver1);
      } else {
        snprintf(tmpVer,
                 sizeof(tmpVer) - 1,
                 "%u.%u.%u",
                 mver0,
                 mver1,
                 mver2);
      }
      strcat(res, tmpVer);
      if ((mver3 != 0x00) && (mver3 != 0x64)) {
        snprintf(tmpVer,
                 sizeof(tmpVer) - 1,
                 " alpha build #%u",
                 mver3);
        strcat(res, tmpVer);
      }
    }
    if ((lastInfo == 0x7fffffff) ||
        ((unsigned)((lastExtInfo >> 24) & 0xFF) == 0x80))
      strcat(res, " Unicode");
    strcat(res, " (ICQ ");
    if (MatchBuddyCaps(capsbuf,
                       capsize,
                       ICQ_CAPABILITY_ICQJS7OxVER,
                       0x10) ||
        MatchBuddyCaps(capsbuf,
                       capsize,
                       ICQ_CAPABILITY_ICQJS7SxVER,
                       0x10)) {
      strcat(res, " S7 & SSS (old)");
    } else {
      switch (iver0) {
      case 0x81:
        strcat(res, " BM");
        break;
      case 0x88:
        strcat(res, " eternity (old)");
      default:
        break;
      }
    }
    switch (iver2) {
    case 0x58:
      strcat(res, " eternity/PlusPlus++ Mod");
      break;
    default:
      break;
    }
    strcat(res, " ");
    switch (iver0) {
    case 0x88:
    case 0x81:
    case 0x80:
      snprintf(tmpVer,
               sizeof(tmpVer) - 1,
               "0.%u.%u.%u alpha)",
               iver1,
               iver2,
               iver3);
      break;
    default:
      snprintf(tmpVer,
               sizeof(tmpVer) - 1,
               "%u.%u.%u.%u)",
               iver0,
               iver1,
               iver2,
               iver3);
      break;
    }
    strcat(res, tmpVer);
    if ((lastExtInfo == 0x5AFEC0DE) ||
        MatchBuddyCaps(capsbuf,
                       capsize,
                       ICQ_CAPABILITY_ICQJS7SxVER,
                       0x10))
      strcat(res, " (SecureIM)");
    return res;
  }
  return 0;
}

//=============================================================================
char *clientIdentify::identify_Qip()
{
  char *capID;
  if ((capID = MatchBuddyCaps(capsbuf,
                              capsize,
                              ICQ_CAPABILITY_QIPxVER,
                              0x0e))) {
    char *res = (char *)malloc(sizeof(char) * 256);
    char tmpVer[256] = { 0 };
    if (lastExtInfo == 0x0F)
      strcpy(tmpVer, "2005");
    else {
      strncpy(tmpVer, capID + 11, 5);
      tmpVer[5] = '\0';
    }
    snprintf(res, 255, "QIP %s", tmpVer);
    if ((lastExtStatusInfo == 0x0000000e) && (lastExtInfo == 0x0000000f)) {
      snprintf(tmpVer,
               sizeof(tmpVer) - 1,
               " (Build %u%u%u%u)",
               lastInfo >> 0x18,
               (lastInfo >> 0x10) & 0xFF,
               (lastInfo >> 0x08) & 0xFF,
               lastInfo & 0xFF);
      strcat(res, tmpVer);
    }
    return res;
  }
  return 0;
}

//=============================================================================
char *clientIdentify::identify_QipInfium()
{
  if (MatchBuddyCaps(capsbuf,
                     capsize,
                     ICQ_CAPABILITY_QIPINFxVER,
                     0x10)) {
    char *res = (char *)malloc(sizeof(char) * 256);
    char tmpVer[256] = { 0 };
    strcpy(res, "QIP Infium");
    if (lastInfo) {
      snprintf(tmpVer, sizeof(tmpVer) - 1, " (Build %u)", lastInfo);
      strcat(res, tmpVer);
    }
    if (lastExtStatusInfo == 0x0000000B)
      strcat(res, " Beta");
    return res;
  }
  return 0;
}

//=============================================================================
char *clientIdentify::identify_QipPDA()
{
  if (MatchBuddyCaps(capsbuf,
                     capsize,
                     ICQ_CAPABILITY_QIPPDAxVER,
                     0x10)) {
    char *res = (char *)malloc(sizeof(char) * 256);
    strcpy(res, "QIP PDA (Windows)");
    return res;
  }
  return 0;
}

//=============================================================================
char *clientIdentify::identify_QipMobile()
{
  if (MatchBuddyCaps(capsbuf,
                     capsize,
                     ICQ_CAPABILITY_QIPMOBxVER,
                     0x10)) {
    char *res = (char *)malloc(sizeof(char) * 256);
    strcpy(res, "QIP Mobile (Java)");
    return res;
  } else if (MatchBuddyCaps(capsbuf,
                            capsize,
                            ICQ_CAPABILITY_QIPSYMBxVER,
                            0x10)) {
    char *res = (char *)malloc(sizeof(char) * 256);
    strcpy(res, "QIP Mobile (Symbian)");
    return res;
  }
  return 0;
}

//=============================================================================
char *clientIdentify::identify_Sim()
{
  char *capID;
  if ((capID = MatchBuddyCaps(capsbuf,
                              capsize,
                              ICQ_CAPABILITY_SIMxVER,
                              strlen(ICQ_CAPABILITY_SIMxVER)))) {
    char *res = (char *)malloc(sizeof(char) * 256);
    char tmpVer[256] = { 0 };
    char *verStr = capID + 12;
    unsigned ver1 = verStr[0];
    unsigned ver2 = verStr[1];
    unsigned ver3 = verStr[2];
    unsigned ver4 = verStr[3] & 0x0f;
    if (ver4) {
      snprintf(tmpVer,
               sizeof(tmpVer) - 1,
               "%u.%u.%u.%u",
               ver1,
               ver2,
               ver3,
               ver4);
    } else if (ver3) {
      snprintf(tmpVer,
               sizeof(tmpVer) - 1,
               "%u.%u.%u",
               ver1,
               ver2,
               ver3);
    } else {
      snprintf(tmpVer,
               sizeof(tmpVer) - 1,
               "%u.%u",
               ver1,
               ver2);
    }
    if (verStr[3] & 0x80)
      strcat(tmpVer,"/Win32");
    else if (verStr[3] & 0x40)
      strcat(tmpVer,"/MacOS X");

    snprintf(res, 255, "SIM v%s", tmpVer);
    return res;
  }
  return 0;
}

//=============================================================================
char *clientIdentify::identify_SimRnQ()
{
  char *capID;
  if ((capID = MatchBuddyCaps(capsbuf,
                              capsize,
                              ICQ_CAPABILITY_SIMxVER,
                              strlen(ICQ_CAPABILITY_SIMxVER)))) {
    char *verStr = capID + 12;
    if (verStr[0] || verStr[1] || verStr[2] || (verStr[3] & 0x0f)) return 0;
  } else {
    capID = MatchBuddyCaps(capsbuf, capsize, ICQ_CAPABILITY_SIMxVER, 10);
    if ( !capID ) return 0;
  }

  char *res = (char *)malloc(sizeof(char) * 256);
  snprintf(res, 255, "R&Q-masked (SIM)");
  return res;
}

//=============================================================================
char *clientIdentify::identify_Licq()
{
  char *capID;
  if ((capID = MatchBuddyCaps(capsbuf,
                              capsize,
                              ICQ_CAPABILITY_LICQxVER,
                              strlen(ICQ_CAPABILITY_LICQxVER)))) {
    char *res = (char *)malloc(sizeof(char) * 256);
    char *verStr = capID + 12;
    unsigned ver1 = verStr[0];
    unsigned ver2 = verStr[1]%100;
    unsigned ver3 = verStr[2];
    snprintf(res, 255, "Licq v%u.%u.%u", ver1, ver2, ver3);
    if (verStr[3] == 1) strcat(res, "/SSL");
    return res;
  }
  return 0;
}

//=============================================================================
char *clientIdentify::identify_Kopete()
{
  char *capID;
  if ((capID = MatchBuddyCaps(capsbuf,
                              capsize,
                              ICQ_CAPABILITY_KOPETExVER,
                              strlen(ICQ_CAPABILITY_KOPETExVER)))) {
    char *res = (char *)malloc(sizeof(char) * 256);
    char tmpVer[256] = { 0 };
    char *verStr = capID + 12;
    unsigned ver1 = verStr[0];
    unsigned ver2 = verStr[1];
    unsigned ver3 = verStr[2]*100;
    ver3 += verStr[3];
    snprintf(tmpVer, sizeof(tmpVer) - 1, "%u.%u.%u", ver1, ver2, ver3);
    snprintf(res, 255, "Kopete v%s", tmpVer);
    return res;
  }
  return 0;
}

//=============================================================================
char *clientIdentify::identify_Micq()
{
  char *capID;
  if ((capID = MatchBuddyCaps(capsbuf,
                              capsize,
                              ICQ_CAPABILITY_MICQxVER,
                              0x0c))) {
    char *res = (char *)malloc(sizeof(char) * 256);
    char tmpVer[256] = { 0 };
    char *verStr = capID + 12;
    unsigned ver1 = verStr[0];
    unsigned ver2 = verStr[1];
    unsigned ver3 = verStr[2];
    unsigned ver4 = verStr[3];
    snprintf(tmpVer,
             sizeof(tmpVer) - 1,
             "%u.%u.%u.%u",
             ver1,
             ver2,
             ver3,
             ver4);
    if ((ver1 & 0x80) == 0x80) strcat(tmpVer, " alpha");

    snprintf(res, 255, "mICQ v%s", tmpVer);
    return res;
  }
  return 0;
}

//=============================================================================
char *clientIdentify::identify_LibGaim()
{
  int newver = 0;
  if (MatchBuddyCaps(capsbuf,
                     capsize,
                     ICQ_CAPABILITY_AIMCHAT,
                     0x10)) {
    newver = 1;
  }

  if ((MatchBuddyCaps(capsbuf,
                      capsize,
                      ICQ_CAPABILITY_AIMSENDFILE,
                      0x10) ||
       MatchShortCaps(client_shorts, ICQ_SHORTCAP_SENDFILE)) &&
      (MatchBuddyCaps(capsbuf,
                      capsize,
                      ICQ_CAPABILITY_AIMIMAGE,
                      0x10) ||
       MatchShortCaps(client_shorts, ICQ_SHORTCAP_AIMIMAGE)) &&
      (MatchBuddyCaps(capsbuf,
                      capsize,
                      ICQ_CAPABILITY_AIMICON,
                      0x10) ||
       MatchShortCaps(client_shorts, ICQ_SHORTCAP_BUDDYCON)) &&
      (MatchBuddyCaps(capsbuf,
                      capsize,
                      ICQ_CAPABILITY_UTF8,
                      0x10) ||
       MatchShortCaps(client_shorts, ICQ_SHORTCAP_UTF)) &&
      ((client_caps.size() + client_shorts.size()) == (4 + newver))) {
    char *res = (char *)malloc(sizeof(char) * 256);
    if (newver == 1) {
      strcpy(res, "Pidgin/AdiumX");
    } else {
      strcpy(res, "Gaim/AdiumX");
    }
    return res;
  } else if ((newver == 1) && (client_proto == 0)) {
    if ((MatchBuddyCaps(capsbuf,
                        capsize,
                        ICQ_CAPABILITY_AIMICON,
                        0x10) ||
         MatchShortCaps(client_shorts, ICQ_SHORTCAP_BUDDYCON)) &&
        (MatchBuddyCaps(capsbuf,
                        capsize,
                        ICQ_CAPABILITY_UTF8,
                        0x10) ||
         MatchShortCaps(client_shorts, ICQ_SHORTCAP_UTF))) {
      char *res = (char *)malloc(sizeof(char) * 256);
      strcpy(res, "Meebo");
      return res;
    }
  }

  return 0;
}

//=============================================================================
char *clientIdentify::identify_Jimm()
{
  char *capID;
  if ((capID = MatchBuddyCaps(capsbuf,
                              capsize,
                              ICQ_CAPABILITY_JIMMxVER,
                              0x5))) {
    char *res = (char *)malloc(sizeof(char) * 256);
    char tmpVer[256] = { 0 };

    strncpy(tmpVer, capID + 0x5, 11);

    snprintf(res, 255, "Jimm %s", tmpVer);
    return res;
  }
  return 0;
}

//=============================================================================
char *clientIdentify::identify_Mip()
{
  char *capID;
  if ((capID = MatchBuddyCaps(capsbuf,
                              capsize,
                              ICQ_CAPABILITY_MIPCLIENT,
                              0xC))) {
    char *res = (char *)malloc(sizeof(char) * 256);
    //char tmpVer[256];
    //strncpy(tmpVer, capID + 0x4, 12);
    //snprintf(res, 255, "MIP %s", tmpVer);

    unsigned ver1 = capID[0xC];
    unsigned ver2 = capID[0xD];
    unsigned ver3 = capID[0xE];
    unsigned ver4 = capID[0xF];

    if (ver1 < 30) {
      snprintf(res,
               255,
               "MIP %u.%u.%u.%u",
               ver1,
               ver2,
               ver3,
               ver4);
    } else {
      char tmpVer[256] = { 0 };
      strncpy(tmpVer, capID + 0xb, 5);
      snprintf(res, 255, "MIP %s", tmpVer);
    }

    return res;
  } else if ((capID = MatchBuddyCaps(capsbuf,
                                     capsize,
                                     ICQ_CAPABILITY_MIPCLIENT,
                                     0x4))) {
    char *res = (char *)malloc(sizeof(char) * 256);
    char tmpVer[256] = { 0 };
    strncpy(tmpVer, capID + 0x4, 12);
    snprintf(res, 255, "MIP %s", tmpVer);
    return res;
  }
  return 0;
}

//=============================================================================
char *clientIdentify::identify_Trillian()
{
  if ((MatchBuddyCaps(capsbuf,
                      capsize,
                      ICQ_CAPABILITY_TRILLIANxVER,
                      0x10)) ||
      (MatchBuddyCaps(capsbuf,
                      capsize,
                      ICQ_CAPABILITY_TRILCRPTxVER,
                      0x10))) {
    char *res = (char *)malloc(sizeof(char) * 256);
    strcpy(res, "Trillian");
    if (MatchBuddyCaps(capsbuf, capsize, ICQ_CAPABILITY_RTFxMSGS, 0x10)) {
      if (MatchBuddyCaps(capsbuf,
                         capsize,
                         ICQ_CAPABILITY_AIMSENDFILE,
                         0x10) ||
          MatchShortCaps(client_shorts, ICQ_SHORTCAP_SENDFILE)) {
        strcat(res, " Astra");
      } else {
        strcat(res, " v3");
      }
    }
    return res;
  }
  return 0;
}

//=============================================================================
char *clientIdentify::identify_Climm()
{
  char *capID;
  if ((capID = MatchBuddyCaps(capsbuf,
                              capsize,
                              ICQ_CAPABILITY_CLIMMxVER,
                              0x0C))) {
    char *res = (char *)malloc(sizeof(char) * 256);
    char tmpVer[256] = { 0 };
    unsigned ver1 = capID[0xC];
    unsigned ver2 = capID[0xD];
    unsigned ver3 = capID[0xE];
    unsigned ver4 = capID[0xF];

    snprintf(tmpVer,
             sizeof(tmpVer) - 1,
             "%u.%u.%u.%u",
             ver1,
             ver2,
             ver3,
             ver4);
    snprintf(res, 255, "climm %s", tmpVer);
    if ((ver1 & 0x80) == 0x80)
      strcat(res, " alpha");
    if (lastExtInfo == 0x02000020)
      strcat(res, "/Win32");
    else if (lastExtInfo == 0x03000800)
      strcat(res, "/MacOS X");
    return res;
  }
  return 0;
}

//=============================================================================
char *clientIdentify::identify_Im2()
{
  if (MatchBuddyCaps(capsbuf,
                     capsize,
                     ICQ_CAPABILITY_IM2xVER,
                     0x10)) {
    char *res = (char *)malloc(sizeof(char) * 256);
    strcpy(res, "IM2");
    return res;
  }
  return 0;
}

//=============================================================================
char *clientIdentify::identify_AndRQ()
{
  char *capID;
  if ((capID = MatchBuddyCaps(capsbuf,
                              capsize,
                              ICQ_CAPABILITY_ANDRQxVER,
                              0x09))) {
    char *res = (char *)malloc(sizeof(char) * 256);
    char tmpVer[256] = { 0 };
    unsigned ver1 = capID[0xC];
    unsigned ver2 = capID[0xB];
    unsigned ver3 = capID[0xA];
    unsigned ver4 = capID[9];
    snprintf(tmpVer,
             sizeof(tmpVer) - 1,
             "%u.%u.%u.%u",
             ver1,
             ver2,
             ver3,
             ver4);
    snprintf(res, 255, "&RQ %s", tmpVer);
    return res;
  }
  return 0;
}

//=============================================================================
char *clientIdentify::identify_RandQ()
{
  char *capID;
  if ((capID = MatchBuddyCaps(capsbuf,
                              capsize,
                              ICQ_CAPABILITY_RANDQxVER,
                              0x09))) {
    char *res = (char *)malloc(sizeof(char) * 256);
    char tmpVer[256] = { 0 };
    unsigned ver1 = capID[0xC];
    unsigned ver2 = capID[0xB];
    unsigned ver3 = capID[0xA];
    unsigned ver4 = capID[9];
    snprintf(tmpVer,
             sizeof(tmpVer) - 1,
             "%u.%u.%u.%u",
             ver1,
             ver2,
             ver3,
             ver4);
    snprintf(res, 255, "R&Q %s", tmpVer);
    return res;
  }
  return 0;
}

//=============================================================================
char *clientIdentify::identify_Imadering()
{
  if (MatchBuddyCaps(capsbuf,
                     capsize,
                     ICQ_CAPABILITY_IMADERING,
                     0x10)) {
    char *res = (char *)malloc(sizeof(char) * 256);
    strcpy(res, "IMadering");
    return res;
  }
  return 0;
}

//=============================================================================
char *clientIdentify::identify_Mchat()
{
  char *capID;
  if ((capID = MatchBuddyCaps(capsbuf,
                              capsize,
                              ICQ_CAPABILITY_MCHATxVER,
                              0xA))) {
    char *res = (char *)malloc(sizeof(char) * 256);
    char tmpVer[256] = { 0 };
    strncpy(tmpVer, capID + 0xA, 6);
    snprintf(res, 255, "mChat %s", tmpVer);
    return res;
  }
  return 0;
}

//=============================================================================
char *clientIdentify::identify_CorePager()
{
  char *capID;
  if ((capID = MatchBuddyCaps(capsbuf,
                              capsize,
                              ICQ_CAPABILITY_COREPGRxVER,
                              0xA))) {
    char *res = (char *)malloc(sizeof(char) * 256);
    char tmpVer[256] = { 0 };
    strcpy(res, "CORE Pager");
    if ((lastExtStatusInfo == 0x0FFFF0011) &&
        (lastExtInfo == 0x1100FFFF) &&
        (lastInfo >> 0x18)) {
      snprintf(tmpVer,
               sizeof(tmpVer) - 1,
               " %u.%u",
               lastInfo >> 0x18,
               (lastInfo >> 0x10) & 0xFF);
      if ((lastInfo & 0xFF) == 0x0B)
        strcat(tmpVer, " Beta");
      strcat(res, tmpVer);
    }
    return res;
  }
  return 0;
}

//=============================================================================
char *clientIdentify::identify_DiChat()
{
  char *capID;
  if ((capID = MatchBuddyCaps(capsbuf,
                              capsize,
                              ICQ_CAPABILITY_DICHATxVER,
                              0x9))) {
    char *res = (char *)malloc(sizeof(char) * 256);
    char tmpVer[256] = { 0 };
    strncpy(tmpVer, capID + 8, 8);
    snprintf(res, 255, "D[i]Chat %s", tmpVer);
    return res;
  }
  return 0;
}

//=============================================================================
char *clientIdentify::identify_Macicq()
{
  if (MatchBuddyCaps(capsbuf,
                     capsize,
                     ICQ_CAPABILITY_MACICQxVER,
                     0x10)) {
    char *res = (char *)malloc(sizeof(char) * 256);
    strcpy(res, "ICQ for Mac");
    return res;
  }
  return 0;
}

//=============================================================================
char *clientIdentify::identify_Anastasia()
{
  if (MatchBuddyCaps(capsbuf,
                     capsize,
                     ICQ_CAPABILITY_ANSTxVER,
                     0x10)) {
    char *res = (char *)malloc(sizeof(char) * 256);
    strcpy(res, "Anastasia");
    return res;
  }
  return 0;
}

//=============================================================================
char *clientIdentify::identify_Jicq()
{
  char *capID;
  if ((capID = MatchBuddyCaps(capsbuf,
                              capsize,
                              ICQ_CAPABILITY_PALMJICQ,
                              0xc))) {
    char *res = (char *)malloc(sizeof(char) * 256);
    char tmpVer[256] = { 0 };
    unsigned ver1 = capID[0xC];
    unsigned ver2 = capID[0xD];
    unsigned ver3 = capID[0xE];
    unsigned ver4 = capID[0xF];
    snprintf(tmpVer,
             sizeof(tmpVer) - 1,
             "%u.%u.%u.%u",
             ver1,
             ver2,
             ver3,
             ver4);
    snprintf(res, 255, "JICQ %s", tmpVer);
    return res;
  }
  return 0;
}

//=============================================================================
char *clientIdentify::identify_Inlux()
{
  if (MatchBuddyCaps(capsbuf,
                     capsize,
                     ICQ_CAPABILITY_INLUXMSGR,
                     0x10)) {
    char *res = (char *)malloc(sizeof(char) * 256);
    strcpy(res, "Inlux Messenger");
    return res;
  }
  return 0;
}

//=============================================================================
char *clientIdentify::identify_Vmicq()
{
  char *capID;
  if ((capID = MatchBuddyCaps(capsbuf,
                              capsize,
                              ICQ_CAPABILITY_VMICQxVER,
                              0x5))) {
    char *res = (char *)malloc(sizeof(char) * 256);
    char tmpVer[256] = { 0 };
    strncpy(tmpVer, capID + 5, 11);
    snprintf(res, 255, "VmICQ %s", tmpVer);
    return res;
  }
  return 0;
}

//=============================================================================
char *clientIdentify::identify_Smaper()
{
  char *capID;
  if ((capID = MatchBuddyCaps(capsbuf,
                              capsize,
                              ICQ_CAPABILITY_SMAPERxVER,
                              0x7))) {
    char *res = (char *)malloc(sizeof(char) * 256);
    char tmpVer[256] = { 0 };
    strncpy(tmpVer, capID + 6, 10);
    snprintf(res, 255, "SmapeR %s", tmpVer);
    return res;
  }
  return 0;
}

//=============================================================================
char *clientIdentify::identify_Yapp()
{
  char *capID;
  if ((capID = MatchBuddyCaps(capsbuf,
                              capsize,
                              ICQ_CAPABILITY_YAPPxVER,
                              0x4))) {
    char *res = (char *)malloc(sizeof(char) * 256);
    char tmpVer[256] = { 0 };
    strncpy(tmpVer, capID + 8, 5);
    snprintf(res, 255, "Yapp! v%s", tmpVer);
    return res;
  }
  return 0;
}

//=============================================================================
char *clientIdentify::identify_Pigeon()
{
  char *capID;
  if ((capID = MatchBuddyCaps(capsbuf,
                              capsize,
                              ICQ_CAPABILITY_PIGEONxVER,
                              0x7))) {
    char *res = (char *)malloc(sizeof(char) * 256);
    strcpy(res, "Pigeon");
    return res;
  }
  return 0;
}

//=============================================================================
char *clientIdentify::identify_NatIcq()
{
  char *capID;
  if ((capID = MatchBuddyCaps(capsbuf,
                              capsize,
                              ICQ_CAPABILITY_NATICQxVER,
                              0x6))) {
    char *res = (char *)malloc(sizeof(char) * 256);
    char rev[256] = { 0 };
    strncpy(rev, capID + 0xc, 4);
    snprintf(res, 255, "NatICQ Siemens (revision %s)", rev);
    return res;
  }
  return 0;
}

//=============================================================================
char *clientIdentify::identify_WebIcqPro()
{
  char *capID;
  if ((capID = MatchBuddyCaps(capsbuf,
                              capsize,
                              ICQ_CAPABILITY_WEBICQPRO,
                              0x9))) {
    char *res = (char *)malloc(sizeof(char) * 256);
    unsigned ver1 = capID[0xA];
    unsigned ver2 = capID[0xB];
    unsigned ver3 = capID[0xC];
    snprintf(res, 255, "WebIcqPro %u.%u.%u", ver1, ver2, ver3);
    if (capID[0xF] == 'b')
      strcat(res, "b");
    return res;
  }
  return 0;
}

//=============================================================================
char *clientIdentify::identify_BayanIcq()
{
  char *capID;
  if ((capID = MatchBuddyCaps(capsbuf,
                              capsize,
                              ICQ_CAPABILITY_BAYANICQxVER,
                              0x8))) {
    char *res = (char *)malloc(sizeof(char) * 256);
    char tmpVer[256] = { 0 };
    strncpy(tmpVer, capID + 8, 8);
    snprintf(res, 255, "bayanICQ v%s", tmpVer);
    return res;
  }
  return 0;
}
