#include "xmlconsole.h"
#include <QPalette>
#include <QDebug>

XmlConsole::XmlConsole(const QString &title, QWidget *parent) : QWidget(parent) 
{
    ui.setupUi(this);
    setWindowTitle(title);
    setAttribute(Qt::WA_QuitOnClose, false);
    setAttribute(Qt::WA_DeleteOnClose, true);
}

void XmlConsole::appendTag(const QString &xml, bool in)
{
    if(xml.size()==1)
        return;
    QString html = QString("<font color=\"%1\">%2</font><br/><br/>").arg(in?"yellow":"red").arg(Qt::escape(xml).replace("\n","<br/>").replace("&gt;&lt;","&gt;<br/>&lt;"));
    ui.textBrowser->append(html);
}
