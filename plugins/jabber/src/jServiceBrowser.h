#ifndef JSERVICEBROWSER_H
#define JSERVICEBROWSER_H

#include <QtGui/QWidget>
#include "ui_jServiceBrowser.h"
#include <gloox/jid.h>
#include "jProtocol.h"
#include "jAccount.h"
#include "jAdhoc.h"

enum jServiceBrowserType {TRANSPORTS_BROWSER, OTHER_SERVICES_BROWSER};

class jAdhoc;

class jServiceBrowser : public QWidget
{
    Q_OBJECT

public:
    jServiceBrowser(jAccount *jabber_account, jServiceBrowserType type, QWidget *parent = 0);
    ~jServiceBrowser();

protected:
    void on_goButton_clicked();
    bool eventFilter(QObject *obj, QEvent *event);

private slots:
    void setConferencesList(QTreeWidgetItem *parent_item, const QStringList &entityList);
    void getDiscoItem(const QString &server, const QString &node, DiscoHandler *dh);
    void getDiscoInfo(const QString &server, const QString &node, DiscoHandler *dh);
    void joinConference(QTreeWidgetItem *item, int);
    void joinConference(QTreeWidgetItem *item);

private:
    Ui::jServiceBrowserClass ui;
    jAccount *m_jabber_account;
    void execCommand(QString server, QString node=0);
};

#endif // JSERVICEBROWSER_H
