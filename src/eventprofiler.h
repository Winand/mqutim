#ifndef _EVENT_PROFILER_H_
#define _EVENT_PROFILER_H_

#include <QObject>
#include <QTime>
#include <QMap>

class EventProfiler: public QObject
{
  Q_OBJECT
  public:
    EventProfiler(QObject *parent);
    virtual bool eventFilter(QObject *watched, QEvent *event);
  private:
    void dumpStats();
    uint m_count;
    QMap<int, uint> stats;
    QTime startTime;
};

#endif // _EVENT_PROFILER_H_

