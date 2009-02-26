#ifndef CONTACTLISTITEMDELEGATE_QTOPIA_H_
#define CONTACTLISTITEMDELEGATE_QTOPIA_H_

#include <QtopiaItemDelegate>
#include <QList>
#include <QVariant>
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
    void drawRightIconGroup(QPainter *painter, const QStyleOptionViewItem &option, const QList<QVariant> &list) const;
    QSize variantSize(QPaintDevice *device, const QStyleOptionViewItem &option, const QVariant &value) const;
    
    QFont accountFont;
    QFont groupFont;
    QFont firstLineFont;
    QFont secondLineFont;
    QFont separatorFont;

    QTreeView *m_tree_view;
    
    // Caching pixel metrics. Theoretically, if widget style changes at runtime, these values become invalid
    // Though, it shouldn't happen
    int m_min_buddy_height;
    int m_vmargin;
    int m_hspace;
};

#endif // CONTACTLISTITEMDELEGATE_QTOPIA_H_
