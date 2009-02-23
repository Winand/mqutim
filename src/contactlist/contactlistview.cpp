#include <QHeaderView>

#include "contactlistview.h"
#include "contactlistitemdelegate_qtopia.h"

ContactListView::ContactListView(QWidget *parent)
  : QTreeView(parent)
{
  header()->hide();
  setIndentation(0);
  setRootIsDecorated(false);
  setItemDelegate(new QtopiaCLItemDelegate(this));
}

ContactListView::~ContactListView()
{
}

void ContactListView::drawBranches(QPainter *painter,
                                   const QRect &rect,
                                   const QModelIndex &index) const
{
}

void ContactListView::drawRow(QPainter *painter,
                              const QStyleOptionViewItem &option,
                              const QModelIndex &index) const
{
  QStyleOptionViewItem mod_option = option;
  mod_option.showDecorationSelected = false;
  QTreeView::drawRow(painter, mod_option, index);
}
