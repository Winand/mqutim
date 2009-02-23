#ifndef CONTACTLISTITEMDELEGATE_QTOPIA_H_
#define CONTACTLISTITEMDELEGATE_QTOPIA_H_

#include <QtopiaItemDelegate>
class QTreeView;

class QtopiaCLItemDelegate: public QtopiaItemDelegate
{
  Q_OBJECT
  public:
    QtopiaCLItemDelegate(QObject *parent = NULL);
    virtual ~QtopiaCLItemDelegate();

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const;
  private:
    void tuneStyleOption(QStyleOptionViewItem &option, const QModelIndex &index) const;
    QFont accountFont;
    QFont groupFont;
    QFont firstLineFont;
    QFont secondLineFont;
    QFont separatorFont;

    QTreeView *m_tree_view;
};

#endif // CONTACTLISTITEMDELEGATE_QTOPIA_H_
