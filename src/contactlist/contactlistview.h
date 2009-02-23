#ifndef CONTACTLISTVIEW_H_
#define CONTACTLISTVIEW_H_

#include <QTreeView>

class ContactListView: public QTreeView
{
  Q_OBJECT
  public:
    ContactListView(QWidget *parent = NULL);
    virtual ~ContactListView();
  protected:
    void drawBranches(QPainter *painter,
                      const QRect &rect,
                      const QModelIndex &index) const;
    void drawRow(QPainter *painter,
                 const QStyleOptionViewItem &option,
                 const QModelIndex &index) const;
};

#endif // CONTACTLISTVIEW_H_
