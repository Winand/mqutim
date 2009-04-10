#include <QDebug>
#include <QTextDocument>
#include <QTextFrame>

#include "chatview.h"

ChatView::ChatView(QWidget *parent)
  : QTextEdit(parent),
    m_model(NULL)
{
  setReadOnly(true);
  setTextInteractionFlags(Qt::TextBrowserInteraction);
  
  m_frameformat.setBorderStyle(QTextFrameFormat::BorderStyle_Solid);
  m_frameformat.setBorder(1);
  
  // FIXME: Workaround for empty lines between frames. Simply hide them.
  QFontMetrics fm(font());
  m_frameformat.setTopMargin(-fm.height()+1);
}

ChatView::~ChatView()
{
}

void ChatView::setModel(MessageListModel *model)
{
  disconnectModel();
  m_model = model;
  connectModel();
  modelReset();
}

void ChatView::modelRowsInserted(const QModelIndex &parent, int start, int end)
{
  qDebug() << "ChatView::modelRowsInserted(" << start << "," << end << ")";
  
  QTextDocument *doc = document();
  QTextCursor cursor = textCursor();
  if (start>=m_cache.count())
  {
    qDebug() << "will append message";
    cursor.movePosition(QTextCursor::End);
  }
  else
  {
    qDebug() << "will insert message";
    QTextFrame *frame = qobject_cast<QTextFrame *>(doc->object(m_cache[start].frameObject));
    cursor.setPosition(frame->firstPosition()-1);
  }
  MessageCache current_cache;
  for (int row = start; row<=end; row++)
  {
    QTextFrame *frame = cursor.insertFrame(m_frameformat);
    
    current_cache.frameObject = frame->objectIndex();
    current_cache.imagePos = -1;
    m_cache.insert(row, current_cache);
    
    cursor.setPosition(frame->lastPosition()+1);
  }
}

void ChatView::modelRowsAboutToBeRemoved(const QModelIndex & parent, int start, int end)
{
  qDebug() << "ChatView::modelRowsAboutToBeRemoved(" << start << "," << end << ")";
  
  QTextDocument *doc = document();
  QTextCursor cursor = textCursor();
  QTextFrame *frame = qobject_cast<QTextFrame *>(doc->object(m_cache[start].frameObject));
  cursor.setPosition(frame->firstPosition()-1);
  frame = qobject_cast<QTextFrame *>(doc->object(m_cache[end].frameObject));
  cursor.setPosition(frame->lastPosition()+1, QTextCursor::KeepAnchor);
  cursor.removeSelectedText();
  
  m_cache.erase(m_cache.begin()+start, m_cache.begin()+end+1);
}

void ChatView::modelReset()
{
  qDebug() << "ChatView: resetting model...";
  m_cache.clear();
  clear();
  
  int n_rows = m_model->rowCount();
  QTextCursor cursor = textCursor();
  MessageCache current_cache;
  for (int row = 0; row<n_rows; row++)
  {
    QModelIndex index = m_model->index(row, 0);
    Message message = m_model->message(index);
    
    QTextFrame *frame = cursor.insertFrame(m_frameformat);
    current_cache.frameObject = frame->objectIndex();
    current_cache.imagePos = -1;
    insertMessage(message, frame, current_cache);
    m_cache.append(current_cache);
    
    cursor.setPosition(frame->lastPosition()+1);
  }
}

void ChatView::modelDataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight)
{ 
  QTextDocument *doc = document();
  for (int row=topLeft.row(); row<=bottomRight.row(); row++)
  {
    QModelIndex index = m_model->index(row, 0);
    QTextFrame *frame = qobject_cast<QTextFrame *>(doc->object(m_cache[row].frameObject));
    insertMessage(m_model->message(index), frame, m_cache[row]);
  }
}

void ChatView::insertMessage(const Message &message, QTextFrame *frame, MessageCache &mcache)
{
  qDebug() << "Appending message";
  qDebug() << "From:" << message.author().m_item_name;
  qDebug() << "Text:" << Qt::escape(message.text());
  qDebug();
  
  QTextCursor cursor = frame->firstCursorPosition();
  cursor.beginEditBlock();
  
  // Clear this frame
  cursor.clearSelection();
  cursor.setPosition(frame->lastPosition(), QTextCursor::KeepAnchor);
  cursor.removeSelectedText();
  cursor.setPosition(frame->firstPosition());
  
  // Insert text
  mcache.imagePos = -1;
  cursor.insertHtml(QString("<b>%1:</b><br>%2").arg(message.author().m_item_name).arg(Qt::escape(message.text())));
  
  cursor.endEditBlock();
}

void ChatView::connectModel()
{
  if (!m_model)
    return;
  
  connect(m_model, SIGNAL(modelReset()), SLOT(modelReset()));
  connect(m_model, SIGNAL(rowsInserted(QModelIndex, int, int)), SLOT(modelRowsInserted(QModelIndex, int, int)));
  connect(m_model, SIGNAL(rowsAboutToBeRemoved(QModelIndex, int, int)), SLOT(modelRowsAboutToBeRemoved(QModelIndex, int, int)));
  connect(m_model, SIGNAL(dataChanged(QModelIndex, QModelIndex)), SLOT(modelDataChanged(QModelIndex, QModelIndex)));
}

void ChatView::disconnectModel()
{
  if (!m_model)
    return;
  
  m_model->disconnect(this);
}
