#ifndef CONTACTLISTITEMDELEGATE_QTOPIA_H_
#define CONTACTLISTITEMDELEGATE_QTOPIA_H_

#include <QtopiaItemDelegate>

class QtopiaCLItemDelegate: public QtopiaItemDelegate
{
  Q_OBJECT
  public:
    QtopiaCLItemDelegate(QObject *parent = NULL);
    virtual ~QtopiaCLItemDelegate();
};

#endif // CONTACTLISTITEMDELEGATE_QTOPIA_H_
