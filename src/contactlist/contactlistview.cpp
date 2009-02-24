#include <QHeaderView>
#include <QtopiaApplication>
#include <QMouseEvent>
#include <QKeyEvent>

#include "contactlistview.h"
#include "contactlistitemdelegate_qtopia.h"
#include "abstractcontactlist.h"

ContactListView::ContactListView(QWidget *parent)
    : QTreeView(parent)
{
  header()->hide();
  setIndentation(0);
  setRootIsDecorated(false);
  setItemDelegate(new QtopiaCLItemDelegate(this));
  QtopiaApplication::setStylusOperation(viewport(), QtopiaApplication::RightOnHold);
  setContextMenuPolicy(Qt::DefaultContextMenu);

  connect(this, SIGNAL(doubleClicked(QModelIndex)), SLOT(onDoubleClicked(QModelIndex)));
  connect(this, SIGNAL(collapsed(QModelIndex)), SLOT(onCollapsed(QModelIndex)));
  connect(this, SIGNAL(expanded(QModelIndex)), SLOT(onExpanded(QModelIndex)));
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

void ContactListView::mousePressEvent(QMouseEvent *event)
{
  if (event->button()==Qt::RightButton)
  {
    QModelIndex index = indexAt(event->pos());
    if (index.isValid())
      AbstractContactList::instance().sendEventClicked(index, event->globalPos());
      // Show context menu
      // TODO: very bad method naming in ACL ^
    return;
  }
  QTreeView::mousePressEvent(event);
}

void ContactListView::keyPressEvent(QKeyEvent *event)
{
  if (event->isAutoRepeat())
    return;
  if (event->key()==Qt::Key_Select)
  {
    QModelIndex index = currentIndex();
    if (index.isValid())
      emit doubleClicked(index);
    return;
  }
  QTreeView::keyPressEvent(event);
}

void ContactListView::onDoubleClicked(const QModelIndex &index)
{
  AbstractContactList::instance().sendEventActivated(index);
}

void ContactListView::onCollapsed(const QModelIndex &index)
{
  AbstractContactList::instance().setExpanded(index, false);
}

void ContactListView::onExpanded(const QModelIndex &index)
{
  AbstractContactList::instance().setExpanded(index, true);
}

