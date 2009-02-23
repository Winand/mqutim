/*
    emoticonMenu

    Copyright (c) 2008 by Rustam Chakin <qutim.develop@gmail.com>

 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************
*/


#include "chatemoticonmenu.h"

ChatEmoticonMenu::ChatEmoticonMenu(QWidget *parent)
    : QWidget(parent)
{

  m_grid_layout = new QGridLayout(this);
  setLayout(m_grid_layout);
}

ChatEmoticonMenu::~ChatEmoticonMenu()
{
  clearList();
}

void ChatEmoticonMenu::setEmoticons(const QHash<QString, QString> &list, const QString &path)
{
  emotList = list;
  emotPath = path;
  clearList();
  int sq  = std::ceil(std::sqrt((float)list.count()));



  int i = 0, j = 0;

  QStringList emotList = list.keys();
  emotList.sort();

  foreach(const QString &name, emotList)
  {
    movieLabel *l = new movieLabel;
//		QMovie *movie = new QMovie(path + "/" + list.key(name));
    QMovie *movie = new QMovie(path + "/" + name);
    movieList.append(movie);
    l->setMovie(movie);
    movie->setCacheMode(QMovie::CacheAll);
    movie->start();
    labelList.append(l);
    l->setWhatsThis(list.value(name));
    connect(l, SIGNAL(sendMovieTip(const QString &)), this, SIGNAL(insertSmile(const QString &)));
    m_grid_layout->addWidget(l,i,j);



    if ( j < sq )
      j++;
    else
    {
      i++;
      j = 0;
    }
    movie->stop();
  }


}

void ChatEmoticonMenu::clearList()
{
//	foreach(movieLabel *l, labelList)
//		delete l;


  qDeleteAll(labelList);
  labelList.clear();

//	foreach(QMovie *m, movieList)
//		delete m;
  qDeleteAll(movieList);
  movieList.clear();
}


void ChatEmoticonMenu::hideEvent(QHideEvent *e)
{
  foreach(QMovie *m, movieList)
  m->stop();
//	clearList();
  QWidget::hideEvent(e);
}

void ChatEmoticonMenu::showEvent(QShowEvent *e)
{
//	setEmoticons(emotList, emotPath);
  foreach(QMovie *m, movieList)
  {
    m->setCacheMode(QMovie::CacheAll);
    m->start();
  }
  QWidget::showEvent(e);
}
