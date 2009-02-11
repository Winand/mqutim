#ifndef AcceptAuthDialog_H
#define AcceptAuthDialog_H

#include "ui_acceptautdialog.h"
#include <QtGui>
#include "jProtocol.h"

class AcceptAuthDialog : public QDialog 
{
    Q_OBJECT
public:
	AcceptAuthDialog(const QString &text, const JID &jid, Client *jabber_client, QWidget *parent=0);
public slots:
	void on_authorizeButton_clicked();
	void on_denyButton_clicked();
private:
	Ui::AcceptAuthDialog ui;
	QPoint desktopCenter();
	JID m_jid;
	Client *m_jabber_client;
};

#endif
