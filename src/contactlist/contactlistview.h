#ifndef CONTACTLISTVIEW_H_
#define CONTACTLISTVIEW_H_

#include <QTreeView>

class ContactListView: public QTreeView
{
  Q_OBJECT
  public:
    ContactListView(QWidget *parent = NULL);
    virtual ~ContactListView();
  private slots:
    void onDoubleClicked(const QModelIndex &index);
    void onCollapsed(const QModelIndex &index);
    void onExpanded(const QModelIndex &index);
  protected:
    void drawBranches(QPainter *painter,
                      const QRect &rect,
                      const QModelIndex &index) const;
    void drawRow(QPainter *painter,
                 const QStyleOptionViewItem &option,
                 const QModelIndex &index) const;

    void mousePressEvent(QMouseEvent *event);
    void keyPressEvent(QKeyEvent *event);
};

#endif // CONTACTLISTVIEW_H_
