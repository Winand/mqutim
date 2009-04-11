#include <QDebug>
#include <QTextDocument>
#include <QTextFrame>
#include <QScrollBar>

#include "chatview.h"

ChatView::ChatView(QWidget *parent)
  : QTextEdit(parent),
    m_model(NULL)
{
  setReadOnly(true);
  setTextInteractionFlags(Qt::TextBrowserInteraction);
  
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
  
  smartScrollBegin();
  
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
  
  smartScrollEnd();
}

void ChatView::modelRowsAboutToBeRemoved(const QModelIndex & parent, int start, int end)
{
  qDebug() << "ChatView::modelRowsAboutToBeRemoved(" << start << "," << end << ")";
  
  smartScrollBegin();
  
  QTextDocument *doc = document();
  QTextCursor cursor = textCursor();
  QTextFrame *frame = qobject_cast<QTextFrame *>(doc->object(m_cache[start].frameObject));
  cursor.setPosition(frame->firstPosition()-1);
  frame = qobject_cast<QTextFrame *>(doc->object(m_cache[end].frameObject));
  cursor.setPosition(frame->lastPosition()+1, QTextCursor::KeepAnchor);
  cursor.removeSelectedText();
  
  m_cache.erase(m_cache.begin()+start, m_cache.begin()+end+1);
  
  if (m_cache.count())
    updateMessageText(0); // If messages were grouped, first message should be updated
    
  smartScrollEnd();
}

void ChatView::modelReset()
{
  qDebug() << "ChatView: resetting model...";
  m_cache.clear();
  clear();
  
  smartScrollBegin();
  
  int n_rows = m_model->rowCount();
  QTextCursor cursor = textCursor();
  MessageCache current_cache;
  for (int row = 0; row<n_rows; row++)
  {
    QTextFrame *frame = cursor.insertFrame(m_frameformat);
    current_cache.frameObject = frame->objectIndex();
    current_cache.imagePos = -1;
    m_cache.append(current_cache);
    
    updateMessageText(row);
    
    cursor.setPosition(frame->lastPosition()+1);
  }
  
  smartScrollEnd();
}

void ChatView::modelDataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight)
{ 
  for (int row=topLeft.row(); row<=bottomRight.row(); row++)
    updateMessageText(row);
}

void ChatView::updateMessageText(int messageRow)
{
  QTextFrame *frame = qobject_cast<QTextFrame *>(document()->object(m_cache[messageRow].frameObject));
  Q_ASSERT(frame != NULL);
  
  Message message = m_model->message(m_model->index(messageRow, 0));
  Message prev_message;
  if (messageRow>0)
    prev_message = m_model->message(m_model->index(messageRow-1, 0));
  
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
  
  // Format strings
  static const QString dialog_nick_color_in = "#FF0000";
  static const QString dialog_nick_color_out = "#00FF00";
  static const QString dialog_nick_format = "<font color=\"%1\"><b>%2</b></font><br>";
  static const QString time_format = "'<font color=\"#999999\" size=\"-2\">'h:mm'</font> '";  
  
  // Insert text
  QString html_text;
  bool append_timestamp = true;
  switch (message.type())
  {
    case Message::IncomingDialog:
      if (prev_message.type()!=message.type())
        html_text += dialog_nick_format.arg(dialog_nick_color_in).arg(message.nick());
      if (append_timestamp)
        html_text += message.time().toString(time_format);
      html_text += Qt::escape(message.text());
      break;
      
    case Message::OutgoingDialog:
      if (prev_message.type()!=message.type())
        html_text += dialog_nick_format.arg(dialog_nick_color_out).arg(message.nick());
      if (append_timestamp)
        html_text += message.time().toString(time_format);
      html_text += Qt::escape(message.text());
      break;
      
    default:
      break;
  }
  
  cursor.insertHtml(html_text);
  
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

#define SMART_SCROLL_THRESHOLD 3
void ChatView::smartScrollBegin()
{
  m_needToScroll = (verticalScrollBar()->maximum()-verticalScrollBar()->sliderPosition())<SMART_SCROLL_THRESHOLD;
}

void ChatView::smartScrollEnd()
{
  if (m_needToScroll)
    verticalScrollBar()->setSliderPosition(verticalScrollBar()->maximum());
}
