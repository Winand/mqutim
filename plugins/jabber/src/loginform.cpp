#include "loginform.h"
#include "jTransport.h"

LoginForm::LoginForm(QWidget *parent) : QWidget(parent)
{
	ui.setupUi(this); // <- BullSHIT
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
