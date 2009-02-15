#include "loginform.h"

#include "jTransport.h"

#include <QThread>

class ClientThread : public QThread, ConnectionListener
{
public:
    ClientThread(QString server);
    void run();
    void 	onConnect ();
    inline void 	onDisconnect (ConnectionError e){};
    inline void 	onResourceBind (const std::string &resource){qDebug() << "onResourceBind";};
    inline void 	onResourceBindError (const Error *error){qDebug() << "onResourceBindError";};
    inline void 	onSessionCreateError (const Error *error){qDebug() << "onSessionCreateError";};
    inline bool 	onTLSConnect (const CertInfo &info){};
    inline void 	onStreamEvent (StreamEvent event){};

private:
    Client *r_client;
    jConnection *r_connection;

private slots:
    void startConnection();
};

 ClientThread::ClientThread(QString server)
 {
     qDebug() << server;
	r_client = new Client( jProtocol::toStd(server), "" );
	r_client->disableRoster();
	r_client->registerConnectionListener( this );
	r_connection = new jConnection(r_client, "", server);
	r_client->setConnectionImpl(r_connection);
	//r_client->setCompressionImpl(0);
	//r_client->setEncryptionImpl(0);
	qDebug() << "connect";
	//r_connection = new jConnection(r_client, serverToRegister, serverToRegister);
	//r_client->setConnectionImpl(r_connection);
 }
void  ClientThread::run(){
    r_client->connect(false);
}

void ClientThread::startConnection(){
    qDebug() << "startconn";
}
 void ClientThread::onConnect ()
{
    qDebug() << "onconnect";
   /* jTransport *tran = new jTransport(r_client, server, this);
    connect(tran, SIGNAL(registrationResult(QString , RegistrationResult)), SLOT(registrationResult(QString , RegistrationResult)));
    tran->addRegistration();*/
}

LoginForm::LoginForm(QWidget *parent) : QWidget(parent)
{
	ui.setupUi(this);
}

LoginForm::~LoginForm()
{
}


void LoginForm::btnRegisterClick()
{
//    bool ok;
//    QStringList knownServers;
//    knownServers << "jabber.org" << "jabber.ru" << "jabbus.org";
//    serverToRegister = QInputDialog::getItem(0, tr("Enter Jabber Server, where you want register"), tr("Server:"), knownServers, 0, true, &ok);
//    if (ok){
	/*r_client = new Client( serverToRegister.toStdString() );
	r_client->disableRoster();
	r_client->registerConnectionListener( this );
	//r_client->setCompressionImpl(0);
	//r_client->setEncryptionImpl(0);
	qDebug() << "connect";
	//r_connection = new jConnection(r_client, serverToRegister, serverToRegister);
	//r_client->setConnectionImpl(r_connection);
	//r_client->connect(false);*/
//	ClientThread *cltrd = new ClientThread(serverToRegister);
//	cltrd->run();
//    }
}

void LoginForm::onConnect ()
{
    qDebug() << "onconnect";
  /*  jTransport *tran = new jTransport(r_client, serverToRegister, this);
    connect(tran, SIGNAL(registrationResult(QString , RegistrationResult)), SLOT(registrationResult(QString , RegistrationResult)));
    tran->addRegistration();*/
}

void LoginForm::registrationResult(QString jid, RegistrationResult result)
{
    ui.jidEdit->setText(jid);
}

void 	LoginForm::onDisconnect (ConnectionError e){
    qDebug() << "ondisconnect";
}

void 	LoginForm::onSessionCreateError (const Error *error)
{
    qDebug() << "onSessionCreateError";
}

