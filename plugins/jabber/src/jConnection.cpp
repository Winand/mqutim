#include "jConnection.h"
#include "jProtocol.h"
#include <stdlib.h>
#include <gloox/dns.h>

jConnection::jConnection( ConnectionDataHandler* cdh, const QString &profile_name, const QString &account_name )
	: QObject(0), ConnectionBase(cdh)
{
    m_profile_name = profile_name;
    m_account_name = account_name;
	m_error = ConnNotConnected;
	m_current_host = 0;
        m_socket = 0;
        QObject::connect(this, SIGNAL(startConnection()), this, SLOT(atStartConnection()));
}

jConnection::~jConnection()
{
	if(m_socket) delete m_socket;
}

void jConnection::createSocket()
{
	if(m_socket)
		delete m_socket;
        m_socket = new QTcpSocket();
        QObject::connect(m_socket,SIGNAL(connected()),this,SLOT(connected()));
	QObject::connect(m_socket,SIGNAL(disconnected()),this,SLOT(disconnected()));
        QObject::connect(m_socket,SIGNAL(hostFound()),this,SLOT(hostFound()));
        QObject::connect(m_socket,SIGNAL(readyRead()),this,SLOT(read()));
	QObject::connect(m_socket,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(error(QAbstractSocket::SocketError)));
	QObject::connect(m_socket,SIGNAL(stateChanged(QAbstractSocket::SocketState)),this,SLOT(stateChanged(QAbstractSocket::SocketState)));
}

void jConnection::setServer(const std::string& server, int port)
{
	m_server = server;
	m_port = port;
}

void jConnection::loadSettings()
{
    m_hosts.clear();
    createSocket();
    QSettings settings(QSettings::NativeFormat, QSettings::UserScope, "qutim/qutim."+m_profile_name+"/jabber."+m_account_name, "accountsettings");

    settings.beginGroup("main");
    JID jid(jProtocol::toStd(m_account_name));
    ClientBase *jClient = static_cast<ClientBase*>(m_handler);

    QString server = jProtocol::fromStd(jid.server());
    int port = settings.value("port",5222).toInt();
    bool use_dns_srv = settings.value("usedns", true).toBool();
    TLSPolicy tls_policy = TLSOptional;
    if(server=="qutim.org")
    {
        server="jabber.qutim.org";
        port = 5222;
        use_dns_srv = false;
    }
    if(server=="udaff.com")
    {
        server="jabber.udaff.com";
        port = 5222;
        use_dns_srv = true;
    }
    bool use_sasl = settings.value("usesasl", true).toBool();
    bool compress = settings.value("compress", true).toBool();
    server = settings.value("server",server).toString();
    m_server = jProtocol::toStd(server);
    m_port = port;
    if(!use_dns_srv)
    {
        if(port==-1)
            port = 5222;
        QPair<QString, int> host;
        host.first = server;
        host.second = port;
        m_hosts.append(host);
    }
    else
    {
        DNS::HostMap hosts = DNS::resolve(m_server, ((ClientBase *)m_handler)->logInstance());
        DNS::HostMap::iterator h = hosts.begin();
        for(;h!=hosts.end();h++)
        {
            QPair<QString, int> host;
            host.first = jProtocol::fromStd(h->first);
            host.second = h->second;
            m_hosts.append(host);
        }
    }
    switch(settings.value("tlspolicy",1).toInt())
    {
        case 0:
            tls_policy = TLSDisabled;
            break;
        case 1:
            tls_policy = TLSOptional;
            break;
        case 2:
            tls_policy = TLSRequired;
            break;
    }

    jClient->setTls(tls_policy);
    jClient->setSasl(use_sasl);
    jClient->setCompression(compress);
    settings.endGroup();
}

ConnectionError jConnection::connect()
{
    if( !m_handler )
        return ConnNotConnected;
    emit startConnection();
    return m_error;
}

void jConnection::atStartConnection()
{
    m_error = ConnNoError;
    loadSettings();
    m_current_host = 0;
    qDebug() << m_account_name << "try to use:" << m_hosts[m_current_host].first << m_hosts[m_current_host].second;
    m_socket->connectToHost( m_hosts[m_current_host].first, m_hosts[m_current_host].second);
}

void jConnection::processMessages( )
{
    recv(10);
    if (!ConnNoError)
	{
	}
}


ConnectionError jConnection::recv( int timeout )
{
        qint64 max_size = m_socket->bytesAvailable();
	if(max_size<=0)
		return m_error;
        char *data = (char *)malloc(max_size*sizeof(char));
	qint64 size = m_socket->read(data, max_size);
	m_total_bytes_in+=size;
	m_handler->handleReceivedData( this, std::string( data, size ) );
        free(data);
	return ConnNoError;
}

bool jConnection::send( const std::string& data )
{
	int size = m_socket->write(data.c_str(),data.length());
	if(size==-1)
		return false;
	m_total_bytes_out+=size;
	m_socket->flush();
	return true;	
}

ConnectionError jConnection::receive()
{
	return m_error;
}

void jConnection::disconnect()
{
        m_current_host = m_hosts.size();
	m_error = ConnUserDisconnected;
	m_socket->disconnectFromHost();
}

ConnectionBase* jConnection::newInstance() const
{
        return new jConnection(m_handler, m_profile_name, m_account_name);
}

void jConnection::connected ()
{
	m_error = ConnNoError;
	m_handler->handleConnect( this );
}

void jConnection::disconnected ()
{
	error(m_socket->error());
	m_handler->handleDisconnect( this, m_error );
        m_error = ConnNotConnected;
}

void jConnection::error ( QAbstractSocket::SocketError socketError )
{
	if(m_error == ConnUserDisconnected)
		return;
	switch(socketError)
	{
	case QAbstractSocket::ConnectionRefusedError:
		m_error = ConnConnectionRefused;
    qDebug() << "ConnConnectionRefused";
		break;
	case QAbstractSocket::RemoteHostClosedError:
		m_error = ConnUserDisconnected;
    qDebug() << "ConnUserDisconnected";
		break;
	case QAbstractSocket::HostNotFoundError:
		m_error = ConnDnsError;
    qDebug() << "ConnDnsError";
		break;
	case QAbstractSocket::ProxyAuthenticationRequiredError:
		m_error = ConnProxyAuthRequired;
    qDebug() << "ConnProxyAuthRequired";
		break;
	case QAbstractSocket::SocketAddressNotAvailableError:
	case QAbstractSocket::AddressInUseError:
	case QAbstractSocket::NetworkError:
	case QAbstractSocket::SocketTimeoutError:
	case QAbstractSocket::SocketResourceError:
	case QAbstractSocket::SocketAccessError:
	case QAbstractSocket::UnsupportedSocketOperationError:
	case QAbstractSocket::DatagramTooLargeError:
	case QAbstractSocket::UnknownSocketError:
	case QAbstractSocket::UnfinishedSocketOperationError:
	default:
		m_error = ConnIoError;
		break;
	}
}

void jConnection::hostFound ()
{
	m_error = ConnNoError;
}

void jConnection::stateChanged ( QAbstractSocket::SocketState socketState )
{
	 switch(socketState)
	 {
	 case QAbstractSocket::HostLookupState:
	 case QAbstractSocket::ConnectingState:
	 	m_state = StateConnecting;
	 	break;
	 case QAbstractSocket::ConnectedState:
	 	m_state = StateConnected;
	 	break;
	 case QAbstractSocket::ClosingState:
	 case QAbstractSocket::ListeningState:
	 case QAbstractSocket::UnconnectedState:
	 default:
	 	m_state = StateDisconnected;
	 	break;
	 }
}

void jConnection::read()
{
	recv();
}
