#include "acceptautdialog.h"

AcceptAuthDialog::AcceptAuthDialog(const QString &text, const JID &jid, Client *jabber_client, QWidget *parent) :
	QDialog(parent),
	m_jid(jid)
{
	ui.setupUi(this);
	ui.authorizeText->setText(text);
	setFixedSize(size());
	move(desktopCenter());
	setAttribute(Qt::WA_QuitOnClose, false);
	setAttribute(Qt::WA_DeleteOnClose, true);
	m_jabber_client = jabber_client;
	setWindowTitle(jProtocol::fromStd(jid.full()));
}

QPoint AcceptAuthDialog::desktopCenter()
{
	QDesktopWidget desktop;
	return QPoint(desktop.width() / 2 - size().width() / 2, desktop.height() / 2 - size().height() / 2);
}

void AcceptAuthDialog::on_authorizeButton_clicked()
{
	m_jabber_client->rosterManager()->ackSubscriptionRequest(m_jid,true);
	close();
}

void AcceptAuthDialog::on_denyButton_clicked()
{
	m_jabber_client->rosterManager()->ackSubscriptionRequest(m_jid,false);
	close();
}
