#include "jServiceBrowser.h"

jServiceBrowser::jServiceBrowser(jAccount *jabber_account, jServiceBrowserType type, QWidget *parent)
    : QWidget(parent)
{
	ui.setupUi(this);
	m_jabber_account = jabber_account;

	switch (type)
	{
	case TRANSPORTS_BROWSER:
		this->setWindowTitle(tr("Transports on") + " " +m_jabber_account->getAccountName());
		break;
	case OTHER_SERVICES_BROWSER:
		this->setWindowTitle(tr("Services on") + " " +m_jabber_account->getAccountName());
		break;
	}
	ui.serviceServer->installEventFilter(this);
	ui.goButton->setIcon(m_jabber_account->getPluginSystem().getIcon("search"));
	ui.closeButton->setIcon(m_jabber_account->getPluginSystem().getIcon("cancel"));
	connect(ui.servicesWidget,SIGNAL(itemDoubleClicked(QTreeWidgetItem*, int)), this, SLOT(joinConference(QTreeWidgetItem*, int)));
	connect(ui.servicesWidget,SIGNAL(itemExpanded(QTreeWidgetItem*)), this, SLOT(joinConference(QTreeWidgetItem*)));
	QString server = jProtocol::fromStd(JID(jProtocol::toStd(m_jabber_account->getAccountName())).server());
	ui.serviceServer->addItem(server);
	on_goButton_clicked();
}

jServiceBrowser::~jServiceBrowser()
{

}

bool jServiceBrowser::eventFilter(QObject *obj, QEvent *event)
{
	if (event->type() == QEvent::KeyPress) {
		QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
		if (keyEvent->key() == Qt::Key_Return || keyEvent->key() == Qt::Key_Enter)
			on_goButton_clicked();
		return QObject::eventFilter(obj, event);
	} else {
		return QObject::eventFilter(obj, event);
	}
}

void jServiceBrowser::setConferencesList(QTreeWidgetItem *parent_item, const QStringList &entityList)
{
	if (entityList[0].isEmpty())
	{
		return;
	}
	QTreeWidgetItem *item = new QTreeWidgetItem(parent_item);
	item->setText(0, entityList[0]);
	item->setText(1, entityList[1]);
	item->setText(2, entityList[2]);
	if (entityList[1] == "server")
	{
		item->setChildIndicatorPolicy(QTreeWidgetItem::ShowIndicator);
	}
	parent_item->setExpanded(true);
	item->setExpanded(false);
}

void jServiceBrowser::on_goButton_clicked()
{
	/*ui.servicesWidget->clear();
	QTreeWidgetItem *root_item = new QTreeWidgetItem();
	root_item->setText(0, ui.serviceServer->currentText());
	ui.servicesWidget->addTopLevelItem(root_item);
	jServiceDiscovery *handler = new jServiceDiscovery(root_item, "gateway");
	connect(handler, SIGNAL(getDiscoItem(QString, QString, DiscoHandler*)), this, SLOT(getDiscoItem(QString, QString, DiscoHandler*)));
	connect(handler, SIGNAL(getDiscoInfo(QString, QString, DiscoHandler*)), this, SLOT(getDiscoInfo(QString, QString, DiscoHandler*)));
	connect(handler, SIGNAL(finishSearch(QTreeWidgetItem*, QStringList)), this, SLOT(setConferencesList(QTreeWidgetItem*, QStringList)));
	handler->search(ui.serviceServer->currentText());*/
}

void jServiceBrowser::getDiscoItem(const QString &server, const QString &node, DiscoHandler *dh)
{
	m_jabber_account->getDiskoItem(server, node, dh);
}

void jServiceBrowser::getDiscoInfo(const QString &server, const QString &node, DiscoHandler *dh)
{
	m_jabber_account->getDiskoInfo(server, node, dh);
}

void jServiceBrowser::joinConference(QTreeWidgetItem *item, int) //double clicked
{
	if (item->text(1) == "gateway")
	{
	    m_jabber_account->getProtocol()->showTransportReg(item->text(0));
	}
	if (item->text(1) != "server")
	{
            qDebug() << "dbl";
           // execCommand(item->text(0), item->text(2));
		//on_selectButton_clicked();
	}

}

void jServiceBrowser::execCommand(QString server, QString node)
{
  //  m_jabber_account->getDiskoInfo(server, node, this);
    jAdhoc *m_adhoc = new jAdhoc(m_jabber_account, server, node);

}

void jServiceBrowser::joinConference(QTreeWidgetItem *item) //expanded
{
	//on_selectButton_clicked();
	if (!item->childCount() && item->text(1) == "server")
	{
		/*jServiceDiscovery *handler = new jServiceDiscovery(item, "gateway");
		connect(handler, SIGNAL(getDiscoItem(QString, QString, DiscoHandler*)), this, SLOT(getDiscoItem(QString, QString, DiscoHandler*)));
		connect(handler, SIGNAL(getDiscoInfo(QString, QString, DiscoHandler*)), this, SLOT(getDiscoInfo(QString, QString, DiscoHandler*)));
		connect(handler, SIGNAL(finishSearch(QTreeWidgetItem*, QStringList)), this, SLOT(setConferencesList(QTreeWidgetItem*, QStringList)));
		handler->search(item->text(0), item->text(1), item->text(2));*/
	}
}

