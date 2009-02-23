#include <QApplication>
#include <QFont>
#include <QFontMetrics>
#include <QTreeView>

#include "contactlistitemdelegate_qtopia.h"
#include "abstractcontactlist.h"

#define BUDDY_INDENT 10

QtopiaCLItemDelegate::QtopiaCLItemDelegate(QObject *parent)
  : QtopiaItemDelegate(parent)
{
  m_tree_view = qobject_cast<QTreeView *>(parent);

  accountFont = QApplication::font();
  accountFont.setPointSize(accountFont.pointSize()+2);
  accountFont.setUnderline(true);
  accountFont.setBold(true);

  groupFont = QApplication::font();
  groupFont.setPointSize(groupFont.pointSize()+2);

  separatorFont = QApplication::font();
  separatorFont.setItalic(true);


  firstLineFont = QApplication::font();
  firstLineFont.setPointSize(firstLineFont.pointSize()+1);

  secondLineFont = QApplication::font();
  secondLineFont.setPointSize(secondLineFont.pointSize()-1);
}

QtopiaCLItemDelegate::~QtopiaCLItemDelegate()
{
}


void QtopiaCLItemDelegate::paint(QPainter *painter,
                                 const QStyleOptionViewItem &option,
                                 const QModelIndex &index) const
{
  QStyleOptionViewItem mod_option = option;
  tuneStyleOption(mod_option, index);
  QtopiaItemDelegate::paint(painter, mod_option, index);
}

QSize QtopiaCLItemDelegate::sizeHint(const QStyleOptionViewItem &option,
                                     const QModelIndex &index) const
{
  QStyleOptionViewItem mod_option = option;
  tuneStyleOption(mod_option, index);
  return QtopiaItemDelegate::sizeHint(mod_option, index);
}

void QtopiaCLItemDelegate::tuneStyleOption(QStyleOptionViewItem &option,
                                           const QModelIndex &index) const
{
  switch (index.data(AbstractContactList::ContactTypeRole).toInt())
  {
    case TreeModelItem::Account:
      option.displayAlignment = Qt::AlignCenter;
      option.font = accountFont;
      break;
    case TreeModelItem::Group:
      option.displayAlignment = Qt::AlignLeft | Qt::AlignVCenter;
      option.font = groupFont;
      if (index.model()->hasChildren(index))
      {
#if 0
        if (!m_tree_view || (m_tree_view && m_tree_view->isExpanded(index)))
#endif
          option.font.setBold(true);
      }
      else
        option.font.setItalic(true);
      break;
    case TreeModelItem::Separator:
      option.displayAlignment = Qt::AlignCenter;
      option.font = separatorFont;
      break;
    case TreeModelItem::Buddy:
      option.rect.setLeft(option.rect.left()+BUDDY_INDENT);
      option.displayAlignment = Qt::AlignLeft | Qt::AlignTop;
      option.font = firstLineFont;
      break;
    default:
      option.displayAlignment = Qt::AlignLeft | Qt::AlignTop;
  }
}
