#include <QApplication>
#include <QFont>
#include <QFontMetrics>
#include <QTreeView>
#include <QStyle>
#include <QPainter>

#include "contactlistitemdelegate_qtopia.h"
#include "abstractcontactlist.h"
#include <qutim/plugininterface.h>

#define BUDDY_INDENT 10

QtopiaCLItemDelegate::QtopiaCLItemDelegate(QObject *parent)
    : QtopiaItemDelegate(parent),
    m_min_buddy_height(0), m_vmargin(0), m_hspace(0)
{
  m_tree_view = qobject_cast<QTreeView *>(parent);
  
  if (m_tree_view)
  {
    QStyle *st = m_tree_view->style();
    m_vmargin = st->pixelMetric(QStyle::PM_FocusFrameVMargin);
    m_min_buddy_height = st->pixelMetric(QStyle::PM_ListViewIconSize) + 2*m_vmargin;
    m_hspace = st->pixelMetric(QStyle::PM_LayoutHorizontalSpacing);
  }

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
  QList<QVariant> &icons = *reinterpret_cast<QList<QVariant> *>(index.data(AbstractContactList::ContactIconsRole).value<qptrdiff>());
  drawRightIconGroup(painter, option, icons);
}

QSize QtopiaCLItemDelegate::sizeHint(const QStyleOptionViewItem &option,
                                     const QModelIndex &index) const
{
  QStyleOptionViewItem mod_option = option;
  tuneStyleOption(mod_option, index);
  QSize size = QtopiaItemDelegate::sizeHint(mod_option, index); // Get initial sizeHint
  
  int h = size.height();
  
  // Fit additional icons'size
  QList<QVariant> &icons = *reinterpret_cast<QList<QVariant> *>(index.data(AbstractContactList::ContactIconsRole).value<qptrdiff>());
  for (int i=qMin(icons.size(), int(TreeModelItem::RightIconGroupEnd)); i>=TreeModelItem::RightIconGroupStart; i--)
    h = qMax(h, variantSize(m_tree_view, option, icons[i]).height()+2*m_vmargin); 
  
  size.setHeight(h);
  return size;
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
#if 0 // this thing causes crashes
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

void QtopiaCLItemDelegate::drawRightIconGroup(QPainter *painter, 
                                              const QStyleOptionViewItem &option,
                                              const QList<QVariant> &list) const
{
  QPoint right = option.rect.topRight();
  right.ry() += m_vmargin;
  for (int i=qMin(list.size(), int(TreeModelItem::RightIconGroupEnd)); i>=TreeModelItem::RightIconGroupStart; i--)
  {
    QSize iconSize = variantSize(painter->device(), option, list[i]);
    if (iconSize.isEmpty()) // No icon here
      continue;
    right.rx() -= iconSize.width()+m_hspace;
    drawDecoration(painter, option, QRect(right, iconSize), list[i]);
  }
}

QSize QtopiaCLItemDelegate::variantSize(QPaintDevice *device,
                                        const QStyleOptionViewItem &option, 
                                        const QVariant &value) const
{
  QSize maxsize(65535,65535);
  if (!value.isNull())
  {
    switch (value.type())
    {
    case QVariant::Invalid:
      break;
    case QVariant::Pixmap:
      return qvariant_cast<QPixmap>(value).size();
    case QVariant::Image:
      return qvariant_cast<QImage>(value).size();
    case QVariant::Icon:
    {
      QIcon icon = qvariant_cast<QIcon>(value);
      return icon.actualSize(maxsize);
    }
    case QVariant::List:
    {
      int maxh=0;
      int width=0;
      foreach(const QVariant &item, value.toList())
      {
        QSize item_size = variantSize(device, option, item);
        width += item_size.width();
        maxh = qMax(maxh, item_size.height());
      }
      return QSize(width,maxh);
    }
    case QVariant::String:
    {
      QFontMetrics fm(option.font, device);
      return QSize(fm.width(value.toString()), fm.height());
    }
    default:
      break;
    }
  }
  return QSize(0,0);
}
