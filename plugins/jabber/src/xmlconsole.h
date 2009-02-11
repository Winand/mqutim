#ifndef XmlConsole_H
#define XmlConsole_H

#include "ui_xmlconsole.h"

class XmlConsole : public QWidget
{
    Q_OBJECT
    
private:
	Ui::XmlConsole ui;
public:
	XmlConsole(const QString &title, QWidget *parent=0);
	void appendTag(const QString &xml, bool in);
};

#endif
