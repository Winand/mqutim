#ifndef jConnection_H
#define jConnection_H
#include <gloox/gloox.h>
#include <gloox/connectionbase.h>
#include <gloox/connectiondatahandler.h>
#include <QTcpSocket>
#include <QMutex>
#include <QMutexLocker>
#include <QHostAddress>

using namespace gloox;

class jConnection : public QObject, public ConnectionBase
{
	Q_OBJECT
	
public:
        jConnection( ConnectionDataHandler* cdh, const QString &profile_name, const QString &account_name );
	virtual ~jConnection();
	
	void createSocket();
	void setServer(const std::string& server, int port = -1);
        void loadSettings();
	
	virtual ConnectionError connect();
	virtual ConnectionError recv( int timeout = -1 );
	virtual bool send( const std::string& data );
	virtual ConnectionError receive();
	virtual void disconnect();
	virtual ConnectionBase* newInstance() const;
	virtual void getStatistics( int &totalIn, int &totalOut ) { totalIn=m_total_bytes_in; totalOut=m_total_bytes_out; }
	virtual void cleanup() {}
	QTcpSocket *getSocket() { return m_socket; }

signals:
        void startConnection();

public slots:
	void connected ();
	void disconnected ();
	void error ( QAbstractSocket::SocketError socketError );
	void hostFound ();
	void stateChanged ( QAbstractSocket::SocketState socketState );
        void read ();
        void atStartConnection();

protected:		
	int m_total_bytes_in;
	int m_total_bytes_out;
	QTcpSocket *m_socket;
	QMutex m_mutex;
	ConnectionError m_error;
	QMutexLocker *m_mutex_locker;
	QList<QPair<QString, int> > m_hosts;
        int m_current_host;
        QString m_profile_name;
        QString m_account_name;
};
#endif // CONNECTIONHTTPPROXY_H__
