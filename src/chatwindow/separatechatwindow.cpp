/*
    SeparateChatWindow

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

#include "separatechatwindow.h"
#include "../abstracthistorylayer.h"
#include "qutim.h"

#include <QDateTime>
#include <QSoftMenuBar>

SeparateChatWindow::SeparateChatWindow(const QString &protocol_name,
                                       const QString &account_name, const QString &item_name,
                                       QWidget *parent)
    : QWidget(parent)
    , m_abstract_chat_layer(AbstractChatLayer::instance())
    , m_protocol_name(protocol_name)
    , m_account_name(account_name)
    , m_item_name(item_name)
    , m_plugin_system(PluginSystem::instance())
    , m_last_message_sender(true)
    , m_lock_for_first_message(false)
    , m_text_changed(false)
    , m_last_message_icon_position(0)
    , m_last_message_position(0)
{
  ui.setupUi(this);
  m_edit = ui.chatInputEdit;
  m_log = ui.chatLog;
  m_log->setOpenExternalLinks(true);
  m_edit->installEventFilter(this);

  QSoftMenuBar::setLabel(this, Qt::Key_Back, QSoftMenuBar::NoLabel);
  QSoftMenuBar::setLabel(m_log, Qt::Key_Back, QSoftMenuBar::NoLabel);
  QSoftMenuBar::setLabel(m_edit, Qt::Key_Back, QString::null, tr("Send"));
  QMenu *menu = QSoftMenuBar::menuFor(this);
  menu->clear();
  menu->addAction(tr("Clear Chat"), this, SLOT(clearChat()));
  menu->addSeparator();
  menu->addAction(tr("Close Chat"), this, SLOT(deleteLater()));

  m_contact_name = m_item_name;
  m_own_name = m_account_name;

  updateInfo();

  setWindowTitle(QString("%1").arg(m_contact_name));
  m_edit->setFocus();

  connect(this, SIGNAL(customContextMenuRequested(const QPoint &)),
          SLOT(onCustomContextMenuRequested(const QPoint &)));
  //setIconsToButtons();

  m_scroll_at_max = true;
  m_current_scroll_position = 0;
  m_last_history = true;
}

SeparateChatWindow::~SeparateChatWindow()
{
  m_abstract_chat_layer.windowActivated(m_protocol_name, m_account_name, m_item_name);
  m_abstract_chat_layer.sendTypingNotification(m_protocol_name,
      m_account_name, m_item_name, 0);
  m_abstract_chat_layer.removeSeparateWindowFromList(QString("%1.%2.%3").arg(m_protocol_name)
      .arg(m_account_name).arg(m_item_name));

  TreeModelItem item;
  item.m_protocol_name = m_protocol_name;
  item.m_account_name = m_account_name;
  item.m_item_name = m_item_name;
  item.m_item_type = TreeModelItem::Buddy;
  PluginSystem::instance().chatWindowClosed(item);
}

int SeparateChatWindow::getCursorPosition()
{
  int cursor_position = 0;
  if ( m_remove_message_after )
  {
    cursor_position = m_last_message_icon_position;
    m_message_position_offset.insert(m_last_message_icon_position, m_log->textCursor().position());
    m_last_message_icon_position++;
  }
  else
  {
    cursor_position = m_log->textCursor().position();
  }
  return cursor_position;
}

void SeparateChatWindow::sendMessage()
{
  if (m_edit->toPlainText().trimmed().isEmpty())
    return; // Empty message

  int cursor_position = getCursorPosition();
  m_abstract_chat_layer.sendMessageTo(m_protocol_name, m_account_name,
                                      m_item_name, m_edit->toPlainText(),
                                      cursor_position);
  m_edit->clear();
  m_edit->moveCursor(QTextCursor::Start);
  m_edit->setFocus();
}

void SeparateChatWindow::addMessage(const QString &message_raw, bool in, const QDateTime &message_date, bool history)
{
  if ( m_log )
  {
    quint64 tmp_position = m_log->textCursor().position();

    QString color_from = "#FF0000";
    QString color_to = "#0000FF";
    QString color_timestamp = "#808080";
    int timestamp_font_size = -2;

    QString message_header_format = "&nbsp;<b><font color=\"%1\">%2</font></b><br>";
    QString message_body_format = "<font color=\"%1\" size=\"%2\">%3</font> %4<br>";
    QString message_timestamp_format = "h:mm";

    QString message_html;
    message_html += message_header_format
                    .arg(in?color_from:color_to) // header font color
                    .arg(in?m_contact_name:m_own_name); // name
    message_html += message_body_format
                    .arg(color_timestamp) // timestamp font color
                    .arg(timestamp_font_size) // timestamp font size
                    .arg(message_date.toString(message_timestamp_format)) // timestamp
                    .arg(message_raw); // message body

    m_current_scroll_position = m_log->verticalScrollBar()->value();
    m_scroll_at_max = m_log->verticalScrollBar()->maximum() == m_current_scroll_position;
    moveCursorToEnd();
    m_log->textCursor().insertImage(":/icons/crystal_project/message.png");
    m_log->insertHtml(message_html);
    moveCursorToEnd();
    if ( m_remove_message_after )
    {
      if ( m_message_positions.count() >= m_remove_count )
      {
        int message_length = m_message_positions.at(0);
        QTextCursor cursor = m_log->textCursor();
        cursor.clearSelection();
        cursor.setPosition(0, QTextCursor::MoveAnchor);
        cursor.setPosition(message_length, QTextCursor::KeepAnchor);
        cursor.removeSelectedText();
        m_message_positions.removeAt(0);
        foreach( int icon_number, m_message_position_offset.keys() )
        {
          int old_position = m_message_position_offset.value(icon_number);
          m_message_position_offset.remove(icon_number);
          m_message_position_offset.insert(icon_number, old_position - message_length);
        }
      }
      m_message_positions.append(m_log->textCursor().position() - tmp_position);
    }
  }
}

void SeparateChatWindow::addServiceMessage(const QString &message)
{
  if ( m_log )
  {
    m_log->append(QString("<font size='-1' color='grey'>%1</font><br>").arg(message));
  }
  moveCursorToEnd();
}

void SeparateChatWindow::setOptions(bool remove_after, quint16 remove_count,
                                    bool close_after, bool show_names, bool on_enter, bool typing_notifications)
{
  m_remove_message_after = remove_after;
  m_remove_count = remove_count;
  m_show_names = show_names;
  m_send_typing_notifications = typing_notifications;
}

void SeparateChatWindow::on_chatInputEdit_textChanged()
{
  m_text_changed = true;
  if ( m_send_typing_notifications )
  {
    if ( !m_typing_changed )
    {
      m_typing_changed = true;
      m_abstract_chat_layer.sendTypingNotification(m_protocol_name,
          m_account_name, m_item_name, 2);
      QTimer::singleShot(5000, this, SLOT(typingNow()));
    }

    if ( m_edit->toPlainText().isEmpty() )
    {
      m_typing_changed = false;
      m_abstract_chat_layer.sendTypingNotification(m_protocol_name,
          m_account_name, m_item_name, 0);
    }
  }
}

void SeparateChatWindow::typingNow()
{
  if ( m_send_typing_notifications )
  {
    if ( m_text_changed )
    {
      m_text_changed= false;
      m_abstract_chat_layer.sendTypingNotification(m_protocol_name,
          m_account_name, m_item_name, 1);
      QTimer::singleShot(5000, this, SLOT(typingNow()));
    }
    else
    {
      m_typing_changed = false;
      m_abstract_chat_layer.sendTypingNotification(m_protocol_name,
          m_account_name, m_item_name, 0);
    }
  }
}

void SeparateChatWindow::contactTyping(bool typing)
{
}

void SeparateChatWindow::clearChat()
{
  m_log->clear();
  m_message_positions.clear();
  m_message_position_offset.clear();
}

void SeparateChatWindow::quoteText()
{
  QString selected_text;

  selected_text = m_log->textCursor().selectedText();
  if ( !selected_text.isEmpty() )
  {
    // Replace paragraph separators
    selected_text.replace(QChar::ParagraphSeparator, "\n>  ");
    // Qt internally uses U+FDD0 and U+FDD1 to mark the beginning and the end of frames.
    // They should be seen as non-printable characters, as trying to display them leads
    // to a crash caused by a Qt "noBlockInString" assertion.
    selected_text.replace(QChar(0xFDD0), " ");
    selected_text.replace(QChar(0xFDD1), " ");
    // Prepend text with ">" tag
    QStringList list = selected_text.split(QChar(0x2028));
    foreach (const QString &mes, list)
    m_edit->insertPlainText("> " + mes + "\n");
    m_edit->moveCursor(QTextCursor::End);
    m_edit->ensureCursorVisible();
    m_edit->setFocus();
  }
  m_edit->setFocus();
}

void SeparateChatWindow::messageDelievered(int position)
{
  if ( m_remove_message_after )
  {
    QTextCursor cursor = m_log->textCursor();
    cursor.setPosition(m_message_position_offset.value(position), QTextCursor::MoveAnchor);
    cursor.deleteChar();
    m_log->setTextCursor(cursor);
    m_log->textCursor().insertImage(":/icons/crystal_project/message_accept.png");
  }
  else
  {
    QTextCursor cursor = m_log->textCursor();
    cursor.setPosition(position, QTextCursor::MoveAnchor);
    cursor.deleteChar();
    m_log->setTextCursor(cursor);
    m_log->textCursor().insertImage(":/icons/crystal_project/message_accept.png");
  }
  moveCursorToEnd();
}

void SeparateChatWindow::moveCursorToEnd()
{
  m_log->moveCursor(QTextCursor::End);
  m_log->ensureCursorVisible();
  m_log->setLineWrapColumnOrWidth(m_log->lineWrapColumnOrWidth());
  int scroll_maximum = m_log->verticalScrollBar()->maximum();
  if ( m_scroll_at_max )
    m_log->verticalScrollBar()->setValue( scroll_maximum );
  else
    m_log->verticalScrollBar()->setValue(m_current_scroll_position);
}

void SeparateChatWindow::windowActivatedByUser()
{
  qutIM *qutim = qutIM::getInstance();
  qutim->setCurrentWidget(this);
  m_edit->setFocus();
}

void SeparateChatWindow::windowFocused()
{
  if ( m_waiting_for_activation )
  {
    m_waiting_for_activation = false;
    m_abstract_chat_layer.windowActivated(m_protocol_name, m_account_name, m_item_name);
    m_edit->setFocus();
  }
}

void SeparateChatWindow::showHistory()
{
  TreeModelItem item;
  item.m_protocol_name = m_protocol_name;
  item.m_item_type = TreeModelItem::Buddy;
  item.m_account_name = m_account_name;
  item.m_item_name = m_item_name;
  AbstractHistoryLayer::instance().openHistoryWindow(item);
  m_edit->setFocus();
}

void SeparateChatWindow::onCustomContextMenuRequested(const QPoint &pos)
{
  m_abstract_chat_layer.askForContactMenu(m_protocol_name, m_account_name,
                                          m_item_name, mapToGlobal(pos));
}

void SeparateChatWindow::setIconsToButtons()
{
}

void SeparateChatWindow::newsOnLinkClicked(const QUrl &url)
{
  QDesktopServices::openUrl(url);
}

void SeparateChatWindow::setContactClientIcon()
{
  updateInfo();
}

void SeparateChatWindow::addSeparator()
{
  m_log->insertHtml("<hr><br>");
  moveCursorToEnd();
}

bool SeparateChatWindow::event(QEvent *event)
{
  if ( event->type() == QEvent::Show )
  {
    m_edit->setFocus();
  }
  return QWidget::event(event);
}

void SeparateChatWindow::setID(const QString &id)
{
  m_item_name = id;
  updateInfo();
}

void SeparateChatWindow::updateInfo()
{
  TreeModelItem info_item;
  info_item.m_protocol_name = m_protocol_name;
  info_item.m_item_type = TreeModelItem::Buddy;
  info_item.m_account_name = m_account_name;

  info_item.m_item_name = m_item_name;
  QStringList contact_info = m_plugin_system.getAdditionalInfoAboutContact(info_item);
  if (contact_info.size()>0)
    m_contact_name = contact_info.at(0);

  info_item.m_item_name = m_account_name;
  QStringList own_info = m_plugin_system.getAdditionalInfoAboutContact(info_item);
  if (own_info.size()>0)
    m_own_name = own_info.at(0);
}

bool SeparateChatWindow::eventFilter(QObject *obj, QEvent *evt)
{
  if (obj==m_edit && evt->type()==QEvent::KeyPress)
  {
    QKeyEvent *ke = static_cast<QKeyEvent *>(evt);
    if (ke->key()==Qt::Key_Back)
    {
      sendMessage();
      return true;
    }
  }
  return false;
}

void SeparateChatWindow::keyPressEvent(QKeyEvent *ev)
{
  if (ev->key()==Qt::Key_Back)
    return;
  QWidget::keyPressEvent(ev);
}
