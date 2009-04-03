#include <QEvent>
#include <QDebug>

#include "eventprofiler.h"

EventProfiler::EventProfiler(QObject *parent)
  : QObject(parent),
    m_count(0)
{
  if (parent)
    parent->installEventFilter(this);
  startTime.start();
}

bool EventProfiler::eventFilter(QObject *watched, QEvent *event)
{
//  qDebug() << watched->metaObject()->className() << watched->objectName() << event->type();
  m_count++;
  stats[event->type()]++;
  if (m_count%1000==0)
    dumpStats();
  return QObject::eventFilter(watched, event);
}

void EventProfiler::dumpStats()
{
//  qDebug() << "Events profiling statistics:";
//  double sec = startTime.elapsed()/100.0;
//  for (QMap<int, uint>::iterator it = stats.begin(); it!=stats.end(); it++)
//    qDebug() << "Type:" << it.key() << "total:" << it.value() << "ev; rate:" << it.value()/sec << "ev/sec";
//  qDebug();
}

