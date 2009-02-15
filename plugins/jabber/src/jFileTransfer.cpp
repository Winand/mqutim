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
/*
#include "jFileTransfer.h"
#include <gloox/util.h>
#include <stdlib.h>


jFileTransfer::StreamHostQuery::StreamHostQuery(Tag *tag) : StanzaExtension(ExtUser+3)
{
    if(!tag)
        return;
    Tag *streamhost = tag->findChild("streamhost");
    if(!streamhost)
        return;
    m_jid = JID(streamhost->findAttribute("jid"));
    m_host = streamhost->findAttribute("host");
    m_port = atoi(streamhost->findAttribute("port").c_str());
    m_zeroconf = streamhost->findAttribute("zeroconf");
}

const std::string &jFileTransfer::StreamHostQuery::filterString () const
{
        static const std::string filter = "/iq/query[@xmlns='" + XMLNS_BYTESTREAMS + "']";
        return filter;
}

StanzaExtension *jFileTransfer::StreamHostQuery::newInstance (const Tag *tag) const
{
    return new StreamHostQuery(tag);
}

Tag *jFileTransfer::StreamHostQuery::tag() const
{
    Tag *tag = new Tag("query");
    tag->setXmlns(XMLNS_BYTESTREAMS);
    if(!m_jid)
        return tag;
    Tag *streamhost = new Tag(tag, "streamhost");
    streamhost->addAttribute("jid", m_jid.full());
    if(!m_host.empty())
        streamhost->addAttribute("host", m_host);
    if(m_port>0)
        streamhost->addAttribute("port", util::int2string(m_port));
    if(!m_zeroconf.empty())
        streamhost->addAttribute("zeroconf", m_zeroconf);
    return tag;
}

jFileTransfer::jFileTransfer(Client *client)
{
//  m_manager = new SIManager(client);
//  m_socks5_manager = new SOCKS5BytestreamManager( m_parent, this );
    m_client = client;
    m_ft = new SIProfileFT(m_client, this);
}

void jFileTransfer::handleFTRequest (const JID &from, const std::string &sid, const std::string &name, long size, const std::string &hash, const std::string &date, const std::string &mimetype, const std::string &desc, int stypes, long offset, long length)
{
}

void jFileTransfer::handleFTRequestError (const IQ &iq, const std::string &sid)
{
}

void jFileTransfer::handleFTBytestream (Bytestream *bs)
{
}

const std::string jFileTransfer::handleOOBRequestResult (const JID &from, const std::string &sid)
{
    return "";
}

bool jFileTransfer::handleIq (const IQ &)
{
    return false;
}

void jFileTransfer::handleIqID (const IQ &iq, int )
{
    StreamHostQuery *stream_host = iq.findExtension<StreamHostQuery>(ExtUser+3);
    if(stream_host)
    {
        StreamHost new_host = stream_host->getStreamHost();
        appendStreamHost(new_host);
        if(new_host.jid)
        {
            bool has_jid = false;
            foreach(StreamHost &host, m_hosts)
            {
                if(host.jid == new_host.jid && host.host == new_host.host && host.port == new_host.port)
                {
                    has_jid = true;
                }
            }
            if(!has_jid)
            {
                m_hosts.append(new_host);
            }
        }
    }
}

void jFileTransfer::searchSocks5Proxy(const JID& jid)
{
    IQ iq(IQ::Get, jid, m_client->getID());
    iq.addExtension(new StreamHostQuery());
    m_client->send(iq, this, 0);
}

void jFileTransfer::appendStreamHost(const StreamHost &host)
{
    if(!host.jid)
        return;
    foreach(StreamHost &h, m_hosts)
    {
        if(host.jid == h.jid && host.host == h.host && host.port == h.port)
            return;
    }
    m_hosts.append(host);
    m_ft->setStreamHosts(m_hosts.toStdList());
}

void jFileTransfer::prependStreamHost(const StreamHost &host)
{
    if(!host.jid)
        return;
    foreach(StreamHost &h, m_hosts)
    {
        if(host.jid == h.jid && host.host == h.host && host.port == h.port)
            return;
    }
    m_hosts.prepend(host);
    m_ft->setStreamHosts(m_hosts.toStdList());
}

void jFileTransfer::replaceStreamHost(const StreamHost &host)
{
    if(!host.jid)
        return;
    bool has_host = false;
    foreach(StreamHost &h, m_hosts)
    {
        if(host.jid == h.jid && host.host == h.host && host.port == h.port)
            return;
        else if(host.jid == h.jid)
        {
            if(has_host)
                m_hosts.removeOne(h);
            else
            {
                h.host = host.host;
                h.port = host.port;
                has_host = true;
            }
        }
    }
    if(has_host)
        m_ft->setStreamHosts(m_hosts.toStdList());
}
*/
