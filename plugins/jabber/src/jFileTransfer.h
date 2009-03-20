/*****************************************************************************
    jFileTransfer

    Copyright (c) 2009 by Nigmatullin Ruslan <euroelessar@gmail.com>

 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************
*****************************************************************************/

#ifndef JFILETRANSFER_H
#define JFILETRANSFER_H

/*#include <jFileTransferWidget.h>
#include <QObject>
#include <QHash>
#include <gloox/client.h>
#include <gloox/siprofileft.h>
#include <gloox/siprofilefthandler.h>
#include <gloox/simanager.h>
#include <gloox/socks5bytestreammanager.h>
#include <gloox/iqhandler.h>


using namespace gloox;

class jFileTransfer : public QObject, SIProfileFTHandler
{
public:

    class StreamHostQuery : public StanzaExtension, IqHandler
    {
    public:
        StreamHostQuery(Tag *tag=0);
        const std::string &filterString () const;
        StanzaExtension *newInstance (const Tag *tag) const;
        Tag* tag() const;
        StanzaExtension* clone() const
        {
            StreamHostQuery *query = new StreamHostQuery();
            query->m_jid = m_jid;
            query->m_host = m_host;
            query->m_port = m_port;
            query->m_zeroconf = m_zeroconf;
            return query;
        }
        StreamHost getStreamHost()
        {
            StreamHost stream_host;
            stream_host.jid = m_jid;
            stream_host.host = host;
            stream_host.port = m_port;
            return stream_host;
        }
    private:
        JID m_jid;
        std::string m_host;
        int m_port;
        std::string m_zeroconf;
    };

    jFileTransfer(Client *client);
    void handleFTRequest (const JID &from, const std::string &sid, const std::string &name, long size, const std::string &hash, const std::string &date, const std::string &mimetype, const std::string &desc, int stypes, long offset, long length);
    void handleFTRequestError (const IQ &iq, const std::string &sid);
    void handleFTBytestream (Bytestream *bs);
    bool handleIq (const IQ &iq);
    void handleIqID (const IQ &iq, int context);
    const std::string handleOOBRequestResult (const JID &from, const std::string &sid);
    void searchSocks5Proxy(const JID& jid);
    void appendStreamHost(const StreamHost &host);
    void prependStreamHost(const StreamHost &host);
    void replaceStreamHost(const StreamHost &host);

private:

    Client *m_client;
    SIProfileFT *m_ft;
//  SIManager *m_manager;
//  SOCKS5BytestreamManager *m_socks5_manager;
    QHash<QString, jFileTransferWidget *> m_ft_widgets;
    QList<StreamHost> m_hosts;
};
*/
#endif // JFILETRANSFER_H
