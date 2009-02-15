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
#include <QFile>
#include <QVBoxLayout>
#include <QDateTime>
#include <QtXml>
#include <QRegExp>
#include <QTimer>
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
{
	m_last_message_sender = true;
	m_lock_for_first_message = false;
	m_close_after_send = false;
	m_text_changed = false;
	m_last_message_icon_position = 0;
	m_last_message_position = 0;
	
  ui.setupUi(this);
  m_send_button = ui.sendButton;
  m_plain_text_edit = ui.chatInputEdit;
  m_text_browser = ui.chatLog;
  m_send_button->setEnabled(false);
  m_text_browser->setOpenExternalLinks(true);
  
  m_contact_name = m_item_name;
  m_own_name = m_account_name;
  
  updateInfo();
    
	setWindowTitle(QString("%1").arg(m_contact_name));
	m_plain_text_edit->setFocus();
  
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
	item.m_item_type = 0;
	PluginSystem::instance().chatWindowClosed(item);
}

int SeparateChatWindow::getCursorPosition()
{
	int cursor_position = 0;
  if( m_text_browser )
	{
		if ( m_remove_message_after )
		{
			cursor_position = m_last_message_icon_position;
			m_message_position_offset.insert(m_last_message_icon_position, m_text_browser->textCursor().position());
			m_last_message_icon_position++;
		}
		else
		{
			cursor_position = m_text_browser->textCursor().position();
		}
	}
	return cursor_position;
}

void SeparateChatWindow::on_sendButton_clicked()
{
	if ( m_plain_text_edit )
	{
		int cursor_position = getCursorPosition();
		if ( !m_plain_text_edit->toPlainText().isEmpty() )
		{
			m_abstract_chat_layer.sendMessageTo(m_protocol_name, m_account_name,
					m_item_name, m_plain_text_edit->toPlainText(),
					cursor_position);
		}
		m_plain_text_edit->clear();
		m_plain_text_edit->moveCursor(QTextCursor::Start);
		m_plain_text_edit->setFocus();
	}
	m_send_button->setEnabled(false);
}

void SeparateChatWindow::addMessage(const QString &message_raw, bool in, const QDateTime &message_date, bool history)
{
  if ( m_text_browser )
	{
		quint64 tmp_position = m_text_browser->textCursor().position();
    
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
    
		m_current_scroll_position = m_text_browser->verticalScrollBar()->value();
		m_scroll_at_max = m_text_browser->verticalScrollBar()->maximum() == m_current_scroll_position; 
    moveCursorToEnd();
    m_text_browser->textCursor().insertImage(":/icons/crystal_project/message.png");
		m_text_browser->insertHtml(message_html);
    moveCursorToEnd();
		if ( m_remove_message_after )
		{
			if ( m_message_positions.count() >= m_remove_count )
			{
				int message_length = m_message_positions.at(0);
				QTextCursor cursor = m_text_browser->textCursor();
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
			m_message_positions.append(m_text_browser->textCursor().position() - tmp_position);
		}
	}
}

void SeparateChatWindow::addServiceMessage(const QString &message)
{
  if ( m_text_browser )
	{
		m_text_browser->append(QString("<font size='-1' color='grey'>%1</font>").arg(message));
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

		if ( m_plain_text_edit->toPlainText().isEmpty() )
		{
			m_typing_changed = false;
			m_abstract_chat_layer.sendTypingNotification(m_protocol_name,
					m_account_name, m_item_name, 0);
		}
	}
	if ( m_plain_text_edit->toPlainText().isEmpty() )
	{
		if ( m_send_button )
			m_send_button->setEnabled(false);
	}
	else
	{
		if ( m_send_button )
			m_send_button->setEnabled(true);
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
		} else {
			m_typing_changed = false;
			m_abstract_chat_layer.sendTypingNotification(m_protocol_name,
					m_account_name, m_item_name, 0);
		}
	}
}

void SeparateChatWindow::contactTyping(bool typing)
{
  /*
	if ( m_cliend_id_label )
	{
		ui.typingLabel->setText(typing?tr("<font color='green'>Typing...</font>"):
			m_client_id);
	}
  */
  ui.typingLabel->setText(typing?QString("<img src=':/icons/typing.png'>"):"");
}

QString SeparateChatWindow::invertMessage(QString &text)
{
	QString tableR=tr("qwertyuiop[]asdfghjkl;'zxcvbnm,./QWERTYUIOP{}ASDFGHJKL:\"ZXCVBNM<>?");
	QString tableE="qwertyuiop[]asdfghjkl;'zxcvbnm,./QWERTYUIOP{}ASDFGHJKL:\"ZXCVBNM<>?";
	QString txt = text;
	for(int i = 0; i < txt.length(); i++)
	{
		if(txt.at(i) <= QChar('z'))
	    {
			int j = 0; bool b=true;
	        while((j < tableE.length()) && b)
	        {
	        	if(txt[i] == tableE[j])
	            {
	               b = false;
	               txt[i] = tableR[j];
	            }
	            j++;
	         }
	      }else{
	         int j = 0; bool b = true;
	         while((j < tableR.length()) && b)
	         {
	            if(txt[i] == tableR[j])
	            {
	               b = false;
	               txt[i] = tableE[j];
	            }
	            j++;
	         }
	      }
	   }
	   return txt;
}

void SeparateChatWindow::clearChat()
{
  if (m_text_browser)
	{
		m_text_browser->clear();
		m_message_positions.clear();
	}
}

void SeparateChatWindow::on_quoteButton_clicked()
{
	QString selected_text;

  if ( m_text_browser )
	{
		selected_text = m_text_browser->textCursor().selectedText();
	}
	if ( m_plain_text_edit)
	{
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
			foreach (QString mes, list)
				m_plain_text_edit->insertPlainText(mes.prepend("> ") + "\n");
			m_plain_text_edit->moveCursor(QTextCursor::End);
			m_plain_text_edit->ensureCursorVisible();
			m_plain_text_edit->setFocus();
		}		
	}
	m_plain_text_edit->setFocus();
}

void SeparateChatWindow::messageDelievered(int position)
{
  if( m_text_browser )
	{
		if ( m_remove_message_after )
		{
			QTextCursor cursor = m_text_browser->textCursor();
		    cursor.setPosition(m_message_position_offset.value(position), QTextCursor::MoveAnchor);
		    cursor.deleteChar();
		    m_text_browser->setTextCursor(cursor);
		    m_text_browser->textCursor().insertImage(":/icons/crystal_project/message_accept.png");	
		}
		else
		{
			QTextCursor cursor = m_text_browser->textCursor();
		    cursor.setPosition(position, QTextCursor::MoveAnchor);
		    cursor.deleteChar();
		    m_text_browser->setTextCursor(cursor);
		    m_text_browser->textCursor().insertImage(":/icons/crystal_project/message_accept.png");
		}
	    moveCursorToEnd();
	}
}

void SeparateChatWindow::moveCursorToEnd()
{
  m_text_browser->moveCursor(QTextCursor::End);
  m_text_browser->ensureCursorVisible();
  m_text_browser->setLineWrapColumnOrWidth(m_text_browser->lineWrapColumnOrWidth());
  int scroll_maximum = m_text_browser->verticalScrollBar()->maximum();
  if ( m_scroll_at_max )
    m_text_browser->verticalScrollBar()->setValue( scroll_maximum );	
  else
    m_text_browser->verticalScrollBar()->setValue(m_current_scroll_position);	
}

void SeparateChatWindow::windowActivatedByUser()
{
  qutIM *qutim = qutIM::getInstance();
  qutim->setCurrentWidget(this);
  m_plain_text_edit->setFocus();
}

void SeparateChatWindow::windowFocused()
{
	if ( m_waiting_for_activation )
	{
		m_waiting_for_activation = false;
		m_abstract_chat_layer.windowActivated(m_protocol_name, m_account_name, m_item_name);
				if ( m_plain_text_edit )
		{
			m_plain_text_edit->setFocus();
		}
	}
}

void SeparateChatWindow::showHistory()
{
	TreeModelItem item;
	item.m_protocol_name = m_protocol_name;
	item.m_item_type = 0;
	item.m_account_name = m_account_name;
	item.m_item_name = m_item_name;
	AbstractHistoryLayer::instance().openHistoryWindow(item);
	m_plain_text_edit->setFocus();
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
  if( m_text_browser )
	{
		m_text_browser->insertHtml("<hr><br>");
		moveCursorToEnd();
	}
}

bool SeparateChatWindow::event(QEvent *event)
{
	if ( event->type() == QEvent::Show )
	{
		if ( m_plain_text_edit )
		{
			m_plain_text_edit->setFocus();
		}
	}
	return QWidget::event(event);
}

void SeparateChatWindow::setID(const QString &id)
{
  m_item_name = id;
  updateInfo();
  //m_id_label->setText(id);
}

void SeparateChatWindow::updateInfo()
{
  TreeModelItem info_item;
  info_item.m_protocol_name = m_protocol_name;
  info_item.m_item_type = 0;
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
