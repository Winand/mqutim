/*****************************************************************************
    Contact List Item Delegate

    Copyright (c) 2008 by Nigmatullin Ruslan <euroelessar@gmail.com>

 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************
*****************************************************************************/

#include "contactlistitemdelegate.h"
#include "proxymodelitem.h"
#include <QDir>
#include <QFile>
#include <QScrollBar>
#include <QBrush>
#include <QPalette>
#include <QBitmap>
#include <QLinearGradient>
#include "src/qutim.h"
#include "abstractlayer.h"

ContactListItemDelegate::ContactListItemDelegate(QObject *parent) : QtopiaItemDelegate(parent)
{
  m_show_icons.fill(true, 13);
}

ContactListItemDelegate::~ContactListItemDelegate() {
    //
}

void ContactListItemDelegate::setSettings(QVector<bool> show_icons) {
  m_show_icons = show_icons;
}

void ContactListItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option2, const QModelIndex &index) const
{
  QStyleOptionViewItem option = option2;

  QPalette::ColorGroup cg = option.state & QStyle::State_Enabled
                            ? QPalette::Normal : QPalette::Disabled;
  if (cg == QPalette::Normal && !(option.state & QStyle::State_Active))
    cg = QPalette::Inactive;

  QBrush selected = option.palette.brush(cg, QPalette::Highlight);

  int type = index.data(AbstractContactList::ContactTypeRole).toInt();
  QFont font=option.font;
  QVariant var_font = index.data(Qt::FontRole);
  if (var_font.type() == QVariant::Font)
    font=qvariant_cast<QFont>(var_font);
  QColor font_color=painter->pen().color();
  QVariant var_color = index.data(AbstractContactList::ContactColorRole);
  if (var_color.type() == QVariant::Color)
    font_color=qvariant_cast<QColor>(var_color);
  QFont status_font = font;;
  QColor status_color = font_color;
  // prepare
  painter->save();
  QtopiaItemDelegate::drawBackground(painter, option, index);
  QFontMetrics font_metrics(font);

  // draw

  QPoint point = option.rect.topLeft();

  int height=0;
  QList<QVariant> &list = *reinterpret_cast<QList<QVariant> *>(index.data(AbstractContactList::ContactIconsRole).value<qptrdiff>());
  switch (type)
  {
  case 1:
    if (static_cast<QTreeView *>(m_tree_view)->isExpanded(index))
      list[0]=IconManager::instance().getIcon("expanded");
    else
      list[0]=IconManager::instance().getIcon("collapsed");
    break;
  default:
    break;
  }
  if (m_show_icons[0])
  {
    QVariant icon_variant = index.data(AbstractContactList::ContactStatusIconRole);
    if (icon_variant.isValid()&&icon_variant.type()==QVariant::Icon || type==1)
    {
      QIcon icon = qvariant_cast<QIcon>(type==1?list[0]:icon_variant);
      QSize icon_size = size(option, index, type==1?list[0]:icon_variant);
      painter->drawPixmap(QRect(point, icon_size),icon.pixmap(icon_size,QIcon::Normal,QIcon::On));
      point+=QPoint(icon_size.width(),0);
      if (icon_size.height()>height)
        height = icon_size.height();
    }
  }
  for (int i=1;i<3;i++)
  {
    if (m_show_icons[i])
      if (list[i].isValid()&&list[i].type()==QVariant::Icon)
      {
        QIcon icon = qvariant_cast<QIcon>(list[i]);
        QSize icon_size = size(option, index, list[i]);
        painter->drawPixmap(QRect(point, icon_size),icon.pixmap(icon_size,QIcon::Normal,QIcon::On));
        point+=QPoint(icon_size.width(),0);
        if (icon_size.height()>height)
          height = icon_size.height();
      }
  }
  QPoint point_r(option.rect.right(),option.rect.top());
  for (int i=12;i>2;i--)
  {
    if (m_show_icons[i])
      if (list[i].isValid()&&list[i].type()==QVariant::Icon)
      {
        QIcon icon = qvariant_cast<QIcon>(list[i]);
        QSize icon_size = size(option, index, list[i]);
        point_r-=QPoint(icon_size.width(),0);
        painter->drawPixmap(QRect(point_r, icon_size),icon.pixmap(icon_size,QIcon::Normal,QIcon::On));
        if (icon_size.height()>height)
          height = icon_size.height();
      }
  }
  QString text=index.data(Qt::DisplayRole).toString();
  QSize item_size=QSize(point_r.x()-point.x(),font_metrics.height());
  if (font_metrics.height()>height)
    height = font_metrics.height();
  int delta_width = item_size.width() - font_metrics.width(text);
  if (type==-1 && delta_width>0)
  {
    item_size.setWidth(item_size.width()-delta_width);
    delta_width/=2;
    point+=QPoint(delta_width,0);
  }
  painter->setFont(font);
  painter->setPen(font_color);
  painter->drawText(QRect(point,QSize(item_size.width(),font_metrics.height())),font_metrics.elidedText(text,Qt::ElideRight,item_size.width()));
  QList<QVariant> &row_list = *reinterpret_cast<QList<QVariant> *>(index.data(AbstractContactList::ContactTextRole).value<qptrdiff>());
  painter->setFont(status_font);
  painter->setPen(status_color);
  font_metrics = QFontMetrics(status_font);
  option.fontMetrics = font_metrics;
  option.font = status_font;
  foreach(const QVariant &row_variant, row_list)
  {
    QList<QVariant> row = row_variant.toList();
    point.setX(option.rect.left());
    point+=QPoint(0,height);
    height=0;
    for (int j=0;j<row.size();j++)
    {
      QRect rect = QRect(point,size(option,index,row[j]));
      draw(painter, option, rect, row[j]);
      point+=QPoint(rect.width(),0);
      if (rect.height()>height)
        height = rect.height();
    }
  }

  // done
  painter->restore();
}

QSize ContactListItemDelegate::sizeHint ( const QStyleOptionViewItem & option, const QModelIndex & index ) const
{
  int type = index.data(AbstractContactList::ContactTypeRole).toInt();
  QVariant value = index.data(Qt::SizeHintRole);
  if (value.isValid())
    return qvariant_cast<QSize>(value);
  QStyleOptionViewItem opt = option;
  QVariant font = index.data(Qt::FontRole);
  if (font.type() == QVariant::Font)
    opt.font=qvariant_cast<QFont>(font);

  int height=size(opt, index, index.data(Qt::DisplayRole)).height();
  QList<QVariant> &list = *reinterpret_cast<QList<QVariant> *>(index.data(AbstractContactList::ContactIconsRole).value<qptrdiff>());
  int icons_height = 0;
  for (int i=0;i<13;i++)
    if (m_show_icons[i])
    {
      icons_height = size(option, index, list[i]).height();
      if (icons_height > height)
        height = icons_height;
    }
  //height+=m_margin*2;
  QList<QVariant> &row_list = *reinterpret_cast<QList<QVariant> *>(index.data(AbstractContactList::ContactTextRole).value<qptrdiff>());
  foreach(const QVariant &list, row_list)
  {
    int h = size(option, index, list).height();
    if (h>0)
      height+=h;
  }
  int width = m_tree_view->maximumViewportSize().width();
  return QSize(width, height);
}

QSize ContactListItemDelegate::size(const QStyleOptionViewItem &option, const QModelIndex & index, const QVariant &value) const
{
  QSize maxsize(65535,65535);
  if (value.isValid() && !value.isNull())
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
      QIcon::Mode mode = QIcon::Normal;//d->iconMode(option.state);
      QIcon::State state = QIcon::On;//option.state&QStyle::State_On==QStyle::State_On?QIcon::On:QIcon::Off;
      QIcon icon = qvariant_cast<QIcon>(value);
      QSize size = icon.actualSize(maxsize);//, mode, state);
      return size;
    }
    case QVariant::Color:
      return option.decorationSize;
    case QVariant::List:
    {
      int maxh=0;
      int width=0;
      foreach(QVariant item, value.toList())
      {
        QSize item_size = size(option, index, item);
        width+=item_size.width();
        if (item_size.height()>maxh)
          maxh=item_size.height();
      }
      return QSize(width,maxh);
    }
    case QVariant::String:
    {
      QFont fnt = option.font;
      QFontMetrics fm(fnt);
      return QSize(fm.width(value.toString()),fm.height());
    }
    default:
      break;
    }
  }
  return QSize(0,0);
}

void ContactListItemDelegate::draw(QPainter *painter, const QStyleOptionViewItem &option, const QRect &rect, const QVariant &value) const
{
  if (!value.isValid() || value.isNull())
    return;

  switch (value.type())
  {
  case QVariant::Invalid:
    break;
  case QVariant::Pixmap:
    painter->drawPixmap(rect,qvariant_cast<QPixmap>(value));
    break;
  case QVariant::Image:
    painter->drawImage(rect,qvariant_cast<QImage>(value));
    break;
  case QVariant::Icon:
  {
    //QIcon::Mode mode = QIcon::Normal;//d->iconMode(option.state);
    //QIcon::State state = QIcon::On;//option.state&QStyle::State_On==QStyle::State_On?QIcon::On:QIcon::Off;
    QIcon icon = qvariant_cast<QIcon>(value);
    painter->drawPixmap(rect,icon.pixmap(icon.actualSize(QSize(65535,65535)),QIcon::Normal,QIcon::On));
    break;
  }
  case QVariant::String:
  {
    QFont fnt = option.font;
    QFontMetrics font_metrics(fnt);
    painter->drawText(rect,font_metrics.elidedText(value.toString(),Qt::ElideRight,rect.width()));
    break;
  }
  default:
    break;
  }
}

void ContactListItemDelegate::setTreeView(QAbstractItemView *tree)
{
  m_tree_view = tree;
}
