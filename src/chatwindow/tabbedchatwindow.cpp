/*
    ContactChatUserSpace
    CustomTabBar
    TabbedChatWindow

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

#include "tabbedchatwindow.h"
#include "../notifications/abstractnotificationlayer.h"
#include "../abstracthistorylayer.h"
//#include <QUiLoader>
//#include <QFile>
#include <QVBoxLayout>
#include <QDateTime>
#include <QtXml>

GeneralTabBar::GeneralTabBar(QWidget *parent)
: QTabBar(parent)
{

}

GeneralTabBar::~GeneralTabBar()
{

}


void GeneralTabBar::mouseDoubleClickEvent(QMouseEvent *e)
{

	if ( e->button() == Qt::LeftButton )
	{
		emit closeTab(tabAt(e->pos()));
	}
}

TabbedChatWindow::TabbedChatWindow(/*bool webkit_mode, */
		const QString &emoticon_path,
                /*const QString &chat_style,*/
		const QString &webkit_style,
		const QString &webkit_variant,
		QWidget *parent)
    : QWidget(parent)
    , m_abstract_chat_layer(AbstractChatLayer::instance())
/*    , m_webkit_mode(webkit_mode)*/
    , m_emoticons_path(emoticon_path)
/*    , m_chat_form_path(chat_style)*/
    , m_webkit_style_path(webkit_style)
    , m_webkit_variant(webkit_variant)
    , m_plugin_system(PluginSystem::instance())
{
	m_send_button = 0;
	m_plain_text_edit = 0;
	m_text_browser = 0;
//	m_web_view = 0;
	m_style_output = 0;
	m_contact_avatar_label = 0;
	m_name_label = 0;
	m_id_label = 0;
	m_cliend_id_label = 0;
	m_owner_avatar_label = 0;
	m_additional_label = 0;
	m_info_button = 0;
	m_emoticons_button = 0;
	m_history_button = 0;
	m_font_button = 0;
	m_color_button = 0;
	m_send_picture_button = 0;
	m_send_file_button = 0;
	m_on_enter_button = 0;
	m_send_typing_button = 0;
	m_typing_label = 0;
	m_client_label = 0;
//	m_main_splitter = 0;
	m_text_changed = false;
	m_typing_changed = false;
	m_previous_tab = -1;
//resize(200,150);
	m_chat_tab_bar = new GeneralTabBar;
	m_chat_tab_bar->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
	connect(m_chat_tab_bar, SIGNAL(currentChanged(int)), this, SLOT(currentTabChanged(int)));
	connect(m_chat_tab_bar, SIGNAL(closeTab( int )), this, SLOT(closeTabAt(int)));
	m_close_tab_button = new QToolButton;
	m_close_tab_button->resize(22,22);
	m_close_tab_button->setIcon(QIcon(":/icons/crystal_project/deletetab"));
	m_close_tab_button->setAutoRaise(true);
	connect(m_close_tab_button, SIGNAL(clicked()), this, SLOT(closeCurrentTab()));
	m_tab_layout = new QHBoxLayout;
	m_tab_layout->setMargin(0);
	m_tab_layout->addWidget(m_chat_tab_bar);
	m_tab_layout->addWidget(m_close_tab_button);
	
//	if ( m_chat_form_path.isEmpty() )
	{
		setDefaultForm();
	}
/*	else
	{
                QString window_file_path;
                if ( webkit_mode )
		{
			window_file_path = m_chat_form_path + "/webchatwindow.ui";
		}
                else
		{
			window_file_path = m_chat_form_path + "/textchatwindow.ui";
		}
                QUiLoader loader;
	
	    QFile file(window_file_path);
	    file.open(QFile::ReadOnly);
	    QWidget *chat_widget = loader.load(&file, this);
            file.close();x
	    m_send_button = qFindChild<QPushButton*>(this, "sendButton");
	    m_plain_text_edit = qFindChild<QTextEdit*>(this, "chatInputEdit");
	    m_contact_avatar_label = qFindChild<QLabel*>(this, "contactAvatarLabel");
	    m_name_label = qFindChild<QLabel*>(this, "nickNameLabel");
	    m_id_label = qFindChild<QLabel*>(this, "idLabel");
	    m_typing_label = qFindChild<QLabel*>(this, "typingLabel");
	    m_cliend_id_label = qFindChild<QLabel*>(this, "typingClientLabel");
	    m_owner_avatar_label = qFindChild<QLabel*>(this, "ownAvatarLabel");
	    m_additional_label = qFindChild<QLabel*>(this, "additionalLabel");
	    m_client_label = qFindChild<QLabel*>(this, "clientLabel");
	    m_info_button = qFindChild<QToolButton*>(this, "infoButton");
	    m_history_button = qFindChild<QToolButton*>(this, "historyButton");
	    m_emoticons_button = qFindChild<QToolButton*>(this, "emoticonButton");
	    m_font_button = qFindChild<QToolButton*>(this, "fontButton");
	    m_color_button = qFindChild<QToolButton*>(this, "colorButton");
	    m_send_picture_button = qFindChild<QToolButton*>(this, "sendPictureButton");
	    m_send_file_button = qFindChild<QToolButton*>(this, "sendFileButton");
	    m_on_enter_button = qFindChild<QToolButton*>(this, "onEnterButton");
	    m_send_typing_button = qFindChild<QToolButton*>(this, "typingButton");
	    m_translate_button = qFindChild<QToolButton*>(this, "translitButton");
		m_quote_button = qFindChild<QToolButton*>(this, "quoteButton");
		m_clear_button = qFindChild<QToolButton*>(this, "clearChatButton");
//	    m_main_splitter= qFindChild<QSplitter*>(this, "splitter");
	    
            if ( webkit_mode )
	    {
	        m_web_view = qFindChild<QWebView*>(this, "webView"); 
			m_style_output = new ChatWindowStyleOutput(m_webkit_style_path,
					m_webkit_variant);
	    }
            else
	    {
	        m_text_browser = qFindChild<QTextBrowser*>(this, "chatViewBrowser"); 
	    }
	    QMetaObject::connectSlotsByName(this);
	    QVBoxLayout *layout = new QVBoxLayout;
	    layout->setMargin(0);
	    layout->addLayout(m_tab_layout);
	    layout->addWidget(chat_widget);
	    setLayout(layout);
        }*/
    setEmoticonsToMenu();
	focusTextEdit();
    if ( m_send_button )
    {
    	m_send_button->setEnabled(false);
    }
    if ( m_text_browser )
    {
    	m_text_browser->setOpenExternalLinks(true);
    }
/*    if ( m_web_view )
    {
    	connect(m_web_view, SIGNAL(loadFinished(bool)),
    			this, SLOT(scrollWebViewToEnd()));
    }*/
    m_event_eater = new ChatEventEater(this);
    connect(m_event_eater, SIGNAL(sendMessage()),
    		this, SLOT(on_sendButton_clicked()));
    connect(m_event_eater, SIGNAL(focusedIn()),
    		this, SLOT(windowFocused()));
    installEventEater();
	setAttribute(Qt::WA_QuitOnClose, false);
	setAttribute(Qt::WA_DeleteOnClose, true);
	connect(this, SIGNAL(customContextMenuRequested(const QPoint &)),
			SLOT(onCustomContextMenuRequested(const QPoint &)));
	m_chat_tab_bar->setContextMenuPolicy(Qt::CustomContextMenu);
	connect(m_chat_tab_bar, SIGNAL(customContextMenuRequested(const QPoint &)),
			SLOT(onCustomTabContextMenuRequested(const QPoint &)));
	setIconsToButtons();
/*	if ( m_webkit_mode && m_web_view )
	{
		m_web_view->page()->setLinkDelegationPolicy(QWebPage::DelegateAllLinks);
		connect(m_web_view, SIGNAL(linkClicked(const QUrl &)),
				this, SLOT(newsOnLinkClicked(const QUrl &)));
        }*/
	m_scroll_at_max = true;
	m_current_scroll_position = 0;
}

TabbedChatWindow::~TabbedChatWindow()
{
	windowFocused();
	m_abstract_chat_layer.removeTabbedWindow();
	for( int i = 0; i < m_chat_tab_bar->count(); i++)
	{
		closeTabAt(i);
	}
	delete m_chat_tab_bar;
	delete m_close_tab_button;
	delete m_tab_layout;
	qDeleteAll(m_user_spaces_chat_list);
/*	m_abstract_chat_layer.saveWindowSizeAndPosition(pos(), size(),
                        m_main_splitter ? m_main_splitter->saveState():QByteArray());*/
	delete m_send_button;
	delete m_plain_text_edit;
	delete m_text_browser;
//	delete m_web_view;
	delete m_style_output;
	delete m_contact_avatar_label;
	delete m_name_label;
	delete m_id_label;
	delete m_cliend_id_label;
	delete m_owner_avatar_label;
	delete m_additional_label;
	delete m_info_button;
	delete m_history_button;
	delete m_emoticons_button;
	
	delete m_font_button;
	delete m_color_button;
	delete m_send_file_button;
	delete m_send_picture_button;
}

bool TabbedChatWindow::lengthLessThan(const QString &s1, const QString &s2)
{
		return s1.size() > s2.size();
}

void TabbedChatWindow::createTab(const QString &protocol_name,
		const QString &account_name, const QString &item_name,
		const QString &identification, const QIcon &icon, bool set_as_current)
{
	if ( !m_tab_chat_index_list.contains(identification) )
	{
		m_tab_chat_index_list.append(identification);
		int index = m_chat_tab_bar->addTab(item_name);
		m_chat_tab_bar->setTabIcon(index, icon);
		ContactChatUserSpace *user_space = new ContactChatUserSpace;
		user_space->m_account_name = account_name;
		user_space->m_protocol_name = protocol_name;
		user_space->m_contact_name = item_name;
		user_space->m_last_message_sender = false;
		user_space->m_lock_for_first_message = false;
		user_space->m_waiting_for_activation = false;
		user_space->m_typing = false;
		user_space->m_unreaded_message = false;
		user_space->m_last_message_icon_position = 0;
		user_space->m_last_message_position = 0;
		user_space->m_status_icon = icon;
		user_space->m_last_history = true;
		m_user_spaces_chat_list.append(user_space);
		readContactInfo(user_space);
		readOwnerInfo(user_space);
/*		if ( m_webkit_mode )
		{
			user_space->m_chat_history = m_style_output->makeSkeleton(user_space->m_contact_nick, 
					user_space->m_account_nick,
					user_space->m_contact_nick, 
					user_space->m_account_avatar,
					user_space->m_contact_avatar,
					QDateTime::currentDateTime().toString());
                }*/
		if ( index == m_chat_tab_bar->currentIndex() )
		{
			currentTabChanged(index);
		}
		if ( set_as_current )
			m_chat_tab_bar->setCurrentIndex(index);
	}
	else if (set_as_current)
	{
		m_chat_tab_bar->setCurrentIndex(m_tab_chat_index_list.indexOf(identification));
	}
	if ( set_as_current )
		windowActivatedByUser();
}

void TabbedChatWindow::closeTabAt(int index)
{
	if ( index < 0 )
		return;
	ContactChatUserSpace *current_user_space = m_user_spaces_chat_list.at(index);
	TreeModelItem item;
	item.m_protocol_name = current_user_space->m_protocol_name;
	item.m_account_name = current_user_space->m_account_name;
	item.m_item_name = current_user_space->m_contact_name;
	item.m_item_type = 0;	
	if ( current_user_space )
	{
        m_abstract_chat_layer.sendTypingNotification(current_user_space->m_protocol_name,
			current_user_space->m_account_name, 
			current_user_space->m_contact_name, 0);
	}
	m_tab_chat_index_list.removeAt(index);
	delete m_user_spaces_chat_list.at(index);
	m_user_spaces_chat_list.removeAt(index);
	m_chat_tab_bar->removeTab(index);
	PluginSystem::instance().chatWindowClosed(item);

	if ( !m_chat_tab_bar->count() )
	{
		close();
	}
}

void TabbedChatWindow::addMessage(const QString &identification, const QString &message, bool in,
		const QString &message_date, bool unread_message, bool history)
{
	int user_space_index = m_tab_chat_index_list.indexOf(identification);
	if ( user_space_index > -1 )
	{
/*		if (m_webkit_mode && m_web_view)
		{
			ContactChatUserSpace *tmp_user_space = m_user_spaces_chat_list.at(user_space_index);
			
			QString add_new_message = checkForEmoticons(message);
			QString newMessage;
			if(add_new_message.startsWith("/me "))			
				newMessage = m_style_output->makeAction(in?tmp_user_space->m_contact_nick:tmp_user_space->m_account_nick, 
						add_new_message.section(" ",1), !in, message_date, 
						in?tmp_user_space->m_contact_avatar:tmp_user_space->m_account_avatar, 
						true, in?tmp_user_space->m_contact_name:tmp_user_space->m_account_name, tmp_user_space->m_protocol_name);
			else
				newMessage = m_style_output->makeMessage(in?tmp_user_space->m_contact_nick:tmp_user_space->m_account_nick, 
						add_new_message, !in, message_date, 
						in?tmp_user_space->m_contact_avatar:tmp_user_space->m_account_avatar, 
						true, in?tmp_user_space->m_contact_name:tmp_user_space->m_account_name, tmp_user_space->m_protocol_name, (tmp_user_space->m_last_message_sender == in)
						&tmp_user_space->m_lock_for_first_message&(tmp_user_space->m_last_history==history), history);
			if(user_space_index != m_chat_tab_bar->currentIndex())
			{
				UnreadMessage message_struct;
				message_struct.m_message = newMessage;
				message_struct.m_same = (tmp_user_space->m_last_message_sender == in)
						&& tmp_user_space->m_lock_for_first_message && (tmp_user_space->m_last_history==history)
						&& !add_new_message.startsWith("/me ");
				tmp_user_space->m_unreaded_messages.append(message_struct);
				setMessageTabIcon(user_space_index);
				tmp_user_space->m_unreaded_message = true;
			}
			else
			{
				bool same_from = (tmp_user_space->m_last_message_sender == in) 
						&& tmp_user_space->m_lock_for_first_message && (tmp_user_space->m_last_history==history)
						&& !add_new_message.startsWith("/me ");
				appendMessage(newMessage,same_from);
			}
			tmp_user_space->m_last_message_sender = in;
			tmp_user_space->m_lock_for_first_message = !add_new_message.startsWith("/me ");
			tmp_user_space->m_last_history=history;
			checkForActiveState(tmp_user_space, message, unread_message, in);
		}
                else*/ if ( m_text_browser )
		{
			ContactChatUserSpace *tmp_user_space = m_user_spaces_chat_list.at(user_space_index);
			QString add_text;
			add_text.append(QString("<msgdiv id='%1'>").arg(tmp_user_space->m_last_message_position));
			add_text.append(QString("<img id='%1' src=':/icons/crystal_project/message.png'> ")
					.arg(tmp_user_space->m_last_message_icon_position));
			tmp_user_space->m_last_message_icon_position++;
	
			add_text.append(in?QString("<b><font color=#0000FF>"):QString("<b><font color=#FF0000>"));
			if ( m_show_names )
			{
				add_text.append(in?tmp_user_space->m_contact_nick:tmp_user_space->m_account_nick);
			}
			add_text.append(QString(" ( %1)</font></b><br>").arg(message_date));
			add_text.append(QString("%1<br>").arg(checkForEmoticons(message)));
			if ( m_remove_message_after )
			{
				if ( tmp_user_space->m_message_positions.count() >= m_remove_count )
				{
					int temp_position = tmp_user_space->m_message_positions.at(0);
					QRegExp rx(QString("<msgdiv id='%1'>*</msgdiv>").arg(temp_position));
					rx.setMinimal(true);
					rx.setPatternSyntax(QRegExp::Wildcard);
					tmp_user_space->m_chat_history.remove(rx);
					tmp_user_space->m_message_positions.removeAt(0);
				}
				tmp_user_space->m_message_positions.append(tmp_user_space->m_last_message_position);
				tmp_user_space->m_last_message_position++;
			}
	
			tmp_user_space->m_chat_history.append(add_text + "</msgdiv>");
			if ( user_space_index == m_chat_tab_bar->currentIndex() )
			{
				m_current_scroll_position = m_text_browser->verticalScrollBar()->value();
				m_scroll_at_max = m_text_browser->verticalScrollBar()->maximum()
					== m_current_scroll_position; 
				m_text_browser->setHtml(tmp_user_space->m_chat_history);
				moveCursorToEnd();
			}
			else
			{
				setMessageTabIcon(user_space_index);
				tmp_user_space->m_unreaded_message = true;
			}
			checkForActiveState(tmp_user_space, message, unread_message, in);
		}
	}

}

/*void TabbedChatWindow::appendMessage(QString message, bool same_from)
{
	QString js_message = QString("append%2Message(\"%1\");").arg(message.replace("\"","\\\"").replace("\n","\\n")).arg(same_from?"Next":"");
	m_web_view->page()->mainFrame()->evaluateJavaScript(js_message);	
}*/

void TabbedChatWindow::addServiceMessage(const QString &identification, const QString &message)
{
/*	if (m_webkit_mode && m_web_view)
	{
		int user_space_index = m_tab_chat_index_list.indexOf(identification);
		ContactChatUserSpace *tmp_user_space = m_user_spaces_chat_list.at(user_space_index);
		QString newMessage = m_style_output->makeStatus(message, QDateTime::currentDateTime().toString());
		
		if ( user_space_index == m_chat_tab_bar->currentIndex() )
		{
			appendMessage(newMessage,false);
			tmp_user_space->m_lock_for_first_message = false;
		}
		else
		{
			UnreadMessage message_struct;
			message_struct.m_message = newMessage;
			message_struct.m_same = false;
			tmp_user_space->m_lock_for_first_message = false;
			tmp_user_space->m_unreaded_messages.append(message_struct);
		}
	}
        else*/ if ( m_text_browser )
	{
		int user_space_index = m_tab_chat_index_list.indexOf(identification);
		ContactChatUserSpace *tmp_user_space = m_user_spaces_chat_list.at(user_space_index);
		tmp_user_space->m_chat_history.append(QString("<font size='-1' color=#808080>%1</font><br>").arg(message));
		if ( user_space_index == m_chat_tab_bar->currentIndex() )
			m_text_browser->append(QString("<font size='-1' color=#808080>%1</font>").arg(message));
	}
}

int TabbedChatWindow::getCursorPosition(const QString &identification)
{
	int user_space_index = m_tab_chat_index_list.indexOf(identification);
	if ( user_space_index > -1 )
	{
		ContactChatUserSpace *current_user_space = m_user_spaces_chat_list.at(user_space_index);
		current_user_space->m_last_message_icon_position++;
		return current_user_space->m_last_message_icon_position - 1;
	}
	return 0;
}

void TabbedChatWindow::on_sendButton_clicked()
{

	if ( m_plain_text_edit  && m_send_button->isEnabled())
	{
		ContactChatUserSpace *current_user_space = m_user_spaces_chat_list.at(m_chat_tab_bar->currentIndex());
		m_abstract_chat_layer.sendMessageTo(current_user_space->m_protocol_name,
				current_user_space->m_account_name,
				current_user_space->m_contact_name, m_plain_text_edit->toPlainText(),
				current_user_space->m_last_message_icon_position);
		current_user_space->m_last_message_icon_position++;
		m_plain_text_edit->clear();
		m_plain_text_edit->moveCursor(QTextCursor::Start);
		if ( m_close_after_send )
			close();
		m_plain_text_edit->setFocus();
	}
}

/*void TabbedChatWindow::on_sendPictureButton_clicked()
{
        QString fileName =   QFileDialog::getOpenFileName(this, tr("Open File"),
			 "",
	                                                 tr("Images (*.gif *.png *.bmp *.jpg *.jpeg)"));

	if ( !fileName.isEmpty())
	{
		QFile iconFile(fileName);
		if ( iconFile.size() > (7 * 1024 + 600))
		{
			QMessageBox::warning(this, tr("Open error"),
			                   tr("Image size is too big"));
		}
		else
		{
			if (iconFile.open(QIODevice::ReadOnly))
			{
				ContactChatUserSpace *current_user_space = m_user_spaces_chat_list.at(m_chat_tab_bar->currentIndex());
				m_abstract_chat_layer.sendImageTo(current_user_space->m_protocol_name,
						current_user_space->m_account_name,
						current_user_space->m_contact_name, iconFile.readAll());
				iconFile.close();
			}
		}
	}
        focusTextEdit();
}*/

/*void TabbedChatWindow::on_sendFileButton_clicked()
{
	ContactChatUserSpace *current_user_space = m_user_spaces_chat_list.at(m_chat_tab_bar->currentIndex());
	m_abstract_chat_layer.sendFileTo(current_user_space->m_protocol_name,
			current_user_space->m_account_name,
			current_user_space->m_contact_name);
	focusTextEdit();
}

void TabbedChatWindow::on_onEnterButton_clicked()
{
	m_event_eater->m_send_on_enter = m_on_enter_button->isChecked();
	focusTextEdit();
}*/

void TabbedChatWindow::on_typingButton_clicked()
{
	m_send_typing_notifications = m_send_typing_button->isChecked();
	focusTextEdit();
}

void TabbedChatWindow::on_infoButton_clicked()
{
	ContactChatUserSpace *current_user_space = m_user_spaces_chat_list.at(m_chat_tab_bar->currentIndex());
	m_abstract_chat_layer.showContactInformation(current_user_space->m_protocol_name, 
			current_user_space->m_account_name, current_user_space->m_contact_name);
	focusTextEdit();
}

void TabbedChatWindow::currentTabChanged(int index)
{
	if( m_typing_changed && m_previous_tab>-1 && m_previous_tab < m_user_spaces_chat_list.count() && m_previous_tab != index )
	{
		ContactChatUserSpace *current_user_space = m_user_spaces_chat_list.at(m_previous_tab);
		m_typing_changed = false;
		m_abstract_chat_layer.sendTypingNotification(current_user_space->m_protocol_name,
			current_user_space->m_account_name, 
			current_user_space->m_contact_name, 0);
	}
	
	if ( m_chat_tab_bar->count() && ( m_previous_tab > -1 ) 
			&& ( m_previous_tab < m_user_spaces_chat_list.count()) 
			&& m_plain_text_edit )
	{
		m_user_spaces_chat_list.at(m_previous_tab)->m_send_text = m_plain_text_edit->toPlainText();
	}
/*	if( m_webkit_mode && m_web_view && m_previous_tab>-1 && m_previous_tab != index && index>-1 && m_previous_tab < m_user_spaces_chat_list.count())
	{
		m_user_spaces_chat_list.at(m_previous_tab)->m_chat_history = m_web_view->page()->mainFrame()->toHtml();		
        }*/
	m_previous_tab = index;
	
	if ( m_chat_tab_bar->count() && m_user_spaces_chat_list.count() )
	{
		ContactChatUserSpace *tmp_user_space = m_user_spaces_chat_list.at(index);
/*		if (m_webkit_mode && m_web_view)
		{
			m_web_view->setHtml(tmp_user_space->m_chat_history);
			foreach(UnreadMessage message_struct,tmp_user_space->m_unreaded_messages)
			{
				appendMessage(message_struct.m_message,message_struct.m_same);
			}
			tmp_user_space->m_unreaded_messages.clear();
		}
                else*/ if ( m_text_browser )
		{
			m_text_browser->setHtml(tmp_user_space->m_chat_history);
		}	
		moveCursorToEnd();
		if ( m_name_label )
			m_name_label->setText(tmp_user_space->m_contact_nick);
		if ( !tmp_user_space->m_contact_avatar.isEmpty() && m_contact_avatar_label)
			m_contact_avatar_label->setPixmap(QPixmap(tmp_user_space->m_contact_avatar));
		if ( m_cliend_id_label )
			m_cliend_id_label->setText(tmp_user_space->m_client_id);
		if ( m_additional_label )
			m_additional_label->setText(tmp_user_space->m_additional_data.remove('\n'));
		if ( m_id_label )
		{
			m_id_label->setText(tmp_user_space->m_contact_name);
		}
		
		if ( !tmp_user_space->m_account_avatar.isEmpty() && m_owner_avatar_label)
			m_owner_avatar_label->setPixmap(QPixmap(tmp_user_space->m_account_avatar));
		
		if ( m_cliend_id_label )
		{
			m_cliend_id_label->setText(tmp_user_space->m_typing?tr("<font color=#00FF00>Typing...</font>"):
				tmp_user_space->m_client_id);
		}
		if ( m_typing_label )
		{
			m_typing_label->setText(tmp_user_space->m_typing?QString("<img src=':/icons/typing.png'>"):"");
		}
		
		if ( tmp_user_space->m_unreaded_message )
		{
			tmp_user_space->m_unreaded_message = false;
			m_chat_tab_bar->setTabIcon(index, tmp_user_space->m_status_icon);
			m_chat_tab_bar->setTabTextColor(index, QColor());
			m_abstract_chat_layer.windowActivated(tmp_user_space->m_protocol_name,
					tmp_user_space->m_account_name,
					tmp_user_space->m_contact_name);
		}
		if ( m_client_label )
		{
			m_client_label->setPixmap(tmp_user_space->m_client_pixmap);
                        m_client_label->setWhatsThis(tmp_user_space->m_client_id);
		}
		if ( m_plain_text_edit )
		{
			m_plain_text_edit->setPlainText(tmp_user_space->m_send_text);
                        m_plain_text_edit->moveCursor(QTextCursor::End);
		}
                if( m_send_button )
                {
                    m_send_button->setEnabled(!tmp_user_space->m_send_text.isEmpty());
                }
		setWindowTitle(QString("%1(%2)").arg(tmp_user_space->m_contact_nick)
				.arg(tmp_user_space->m_account_name));
		setWindowIcon(m_chat_tab_bar->tabIcon(index));
		m_scroll_at_max = true;
	}
}

void TabbedChatWindow::setDefaultForm()
{
	ui = new Ui::Form;
    QWidget *temp_widget = new QWidget(this);
    ui->setupUi(temp_widget);
//	ui->setupUi(this);
    m_send_button = ui->sendButton;
    m_plain_text_edit = ui->chatInputEdit;
    m_typing_label = ui->typingLabel;
    m_client_label = ui->clientLabel;
    m_info_button = ui->infoButton;
    m_history_button = ui->historyButton;
    m_emoticons_button = ui->emoticonButton;
/*    m_send_picture_button = ui->sendPictureButton;
    m_send_file_button = ui->sendFileButton;
    m_on_enter_button = ui->onEnterButton;*/
    m_send_typing_button = ui->typingButton;
/*    m_translate_button = ui->translitButton;*/
	m_quote_button = ui->quoteButton;
	m_clear_button = ui->clearChatButton;
//    m_main_splitter= ui->splitter;
/*    if ( m_webkit_mode )
    {
        QFrame *frame = new QFrame(this);
        QGridLayout *layout = new QGridLayout(frame);
        m_web_view = new QWebView(this);
        layout->addWidget(m_web_view);
        frame->setLayout(layout);
        layout->setMargin(0);
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Sunken);
        ui->splitter->insertWidget(0, frame);
		m_style_output = new ChatWindowStyleOutput(m_webkit_style_path,
				m_webkit_variant);
    }
    else*/
    {
        m_text_browser = new QTextBrowser(this);
        ui->vboxLayout->insertWidget(0, m_text_browser);
    }
    QVBoxLayout *layout = new QVBoxLayout;
    layout->setMargin(4);
    layout->addLayout(m_tab_layout);
    layout->addWidget(temp_widget);
    setLayout(layout);
    temp_widget->setContextMenuPolicy(Qt::CustomContextMenu);
	connect(temp_widget, SIGNAL(customContextMenuRequested(const QPoint &)),
			SLOT(onCustomContextMenuRequested(const QPoint &)));
    setContextMenuPolicy(Qt::CustomContextMenu);
    QMetaObject::connectSlotsByName(this);
}

void TabbedChatWindow::installEventEater()
{
	if ( m_plain_text_edit )
		m_plain_text_edit->installEventFilter(m_event_eater);
/*	if ( m_web_view )
                m_web_view->installEventFilter(m_event_eater);*/
	installEventFilter(m_event_eater);
}

void TabbedChatWindow::setEmoticonsToMenu()
{
	if ( m_emoticons_button )
	{
		m_emotic_menu = new QMenu(m_emoticons_button);
		m_emoticon_action = new QWidgetAction(m_emotic_menu);
		m_emoticon_widget = new ChatEmoticonMenu(m_emotic_menu);
		m_emoticon_action->setDefaultWidget(m_emoticon_widget);
		m_emotic_menu->addAction(m_emoticon_action);
		m_emoticons_button->setMenu(m_emotic_menu);
		setEmoticonPath(m_emoticons_path);
		connect(m_emoticon_widget, SIGNAL(insertSmile(const QString &)), this,
				SLOT(insertEmoticon(const QString &)));
	}
}

void TabbedChatWindow::setEmoticonPath(const QString &path)
{
	m_emoticon_list.clear();
	QFile file(path);
	QString dirPath = path.section('/', 0, -2);
	QDir dir ( dirPath );
	QStringList fileList = dir.entryList(QDir::Files);
	if (file.exists() && file.open(QIODevice::ReadOnly) )
	{
		QDomDocument doc;
		if ( doc.setContent(&file) )
		{
			QDomElement rootElement = doc.documentElement();
			int emoticonCount = rootElement.childNodes().count();
			QDomElement emoticon = rootElement.firstChild().toElement();
			for ( int i = 0; i < emoticonCount ; i++ )
			{
				if ( emoticon.tagName() == "emoticon")
				{
					QStringList fileName = fileList.filter(emoticon.attribute("file"));
					if ( !fileName.isEmpty())
					{
						m_emoticon_list.insert(fileName.at(0),emoticon.firstChild().toElement().text());
						int stringCount = emoticon.childNodes().count();
						QDomElement emoticonString = emoticon.firstChild().toElement();
						for(int j = 0; j < stringCount; j++)
						{
							if ( emoticonString.tagName() == "string")
							{
								m_urls.insert(Qt::escape(emoticonString.text()), dirPath + "/" + fileName.at(0));
							}

							emoticonString = emoticonString.nextSibling().toElement();
						}
					}
				}
				emoticon = emoticon.nextSibling().toElement();
			}
			m_emoticon_keys = m_urls.keys();
			qSort(m_emoticon_keys.begin(), m_emoticon_keys.end(), lengthLessThan);
		}
		m_emoticon_widget->setEmoticons(m_emoticon_list, dirPath);
	}
}

void TabbedChatWindow::insertEmoticon(const QString &emoticon_text)
{
	if ( m_plain_text_edit )
	{
		m_plain_text_edit->insertHtml(" " + emoticon_text + " ");
	}
}

void TabbedChatWindow::focusTextEdit()
{
    if ( m_plain_text_edit )
    {
    	m_plain_text_edit->setFocus();
    }
}

void TabbedChatWindow::readContactInfo(ContactChatUserSpace *user_space)
{
	TreeModelItem item;
	item.m_protocol_name = user_space->m_protocol_name;
	item.m_item_type = 0;
	item.m_account_name = user_space->m_account_name;
	item.m_item_name = user_space->m_contact_name;
	
	user_space->m_contact_nick = user_space->m_contact_name;
	QStringList addition_contact_info = m_plugin_system.getAdditionalInfoAboutContact(item);
	if ( addition_contact_info.count() > 0)
	{
		user_space->m_contact_nick = addition_contact_info.at(0);
	}
	if ( addition_contact_info.count() > 1 )
	{
		user_space->m_contact_avatar = addition_contact_info.at(1);
	}
	if ( addition_contact_info.count() > 2 )
	{
		user_space->m_client_id = addition_contact_info.at(2);
	}
	if ( addition_contact_info.count() > 3 )
	{
		user_space->m_additional_data = addition_contact_info.at(3);
	}
	
	user_space->m_client_pixmap = AbstractContactList::instance().getItemClientIcon(item);
	
	m_chat_tab_bar->setTabText(m_user_spaces_chat_list.indexOf(user_space), user_space->m_contact_nick);
}

void TabbedChatWindow::readOwnerInfo(ContactChatUserSpace *user_space)
{
	TreeModelItem item;
	item.m_protocol_name = user_space->m_protocol_name;
	item.m_item_type = 0;
	item.m_account_name = user_space->m_account_name;
	item.m_item_name = user_space->m_account_name;
	
	user_space->m_account_nick = user_space->m_account_name;
	QStringList addition_own_info = m_plugin_system.getAdditionalInfoAboutContact(item);
	if ( addition_own_info.count() > 0 )
	{
		user_space->m_account_nick = addition_own_info.at(0);
	}
	if ( addition_own_info.count() > 1 )
	{
		user_space->m_account_avatar = addition_own_info.at(1);
	}
}

void TabbedChatWindow::setOptions(bool remove_after, quint16 remove_count,
		bool close_after, bool show_names, bool on_enter,
		bool typing_notifications)
{
	m_remove_message_after = remove_after;
	m_remove_count = remove_count;
	m_close_after_send = close_after;
	m_show_names = show_names;
	m_event_eater->m_send_on_enter = on_enter;
	if ( m_on_enter_button )
	{
		m_on_enter_button->setChecked(on_enter);
	}
	if ( m_send_typing_button )
	{
		m_send_typing_button->setChecked(typing_notifications);
	}
	m_send_typing_notifications = typing_notifications;
}

void TabbedChatWindow::on_chatInputEdit_textChanged()
{
	m_text_changed = true;
	if ( m_send_typing_notifications )
	{
		
		ContactChatUserSpace *current_user_space = m_user_spaces_chat_list.at(m_chat_tab_bar->currentIndex());
		
		if ( ! m_typing_changed )
		{
			if(current_user_space->m_send_text == m_plain_text_edit->toPlainText())
				return;
			m_typing_changed = true;
			m_abstract_chat_layer.sendTypingNotification(current_user_space->m_protocol_name,
					current_user_space->m_account_name, 
					current_user_space->m_contact_name, 2);
			QTimer::singleShot(5000, this, SLOT(typingNow()));
		}


		if ( m_plain_text_edit->toPlainText().isEmpty() )
		{
			m_typing_changed = false;
			m_abstract_chat_layer.sendTypingNotification(current_user_space->m_protocol_name,
					current_user_space->m_account_name, 
					current_user_space->m_contact_name, 0);
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

void TabbedChatWindow::typingNow()
{
	if ( m_send_typing_notifications )
	{
		ContactChatUserSpace *current_user_space = m_user_spaces_chat_list.at(m_chat_tab_bar->currentIndex());
		if ( m_text_changed )
		{
			m_text_changed= false;
			m_abstract_chat_layer.sendTypingNotification(current_user_space->m_protocol_name,
					current_user_space->m_account_name, 
					current_user_space->m_contact_name, 1);
			QTimer::singleShot(5000, this, SLOT(typingNow()));
		} else {
			m_typing_changed = false;
			m_abstract_chat_layer.sendTypingNotification(current_user_space->m_protocol_name,
					current_user_space->m_account_name, 
					current_user_space->m_contact_name, 0);
		}
	}
}

/*void TabbedChatWindow::on_translitButton_clicked()
{
	if (m_plain_text_edit )
	{
		QString txt = m_plain_text_edit->toPlainText();
		if ( m_plain_text_edit->textCursor().hasSelection() )
		{
			QString sel_text = m_plain_text_edit->textCursor().selectedText();
			sel_text = invertMessage(sel_text);
			txt.replace(m_plain_text_edit->textCursor().selectionStart(), sel_text.length(), sel_text);
		} 
		else 
		{
			txt = invertMessage(txt);
		}
		m_plain_text_edit->clear();
		m_plain_text_edit->insertPlainText(txt);
	}
	focusTextEdit();
}*/

QString TabbedChatWindow::checkForEmoticons(const QString &message)
{
	QString new_message = message;
	foreach(QString emoticon_text, m_emoticon_keys)
	{
#if defined(Q_OS_WIN32)
		new_message.replace(emoticon_text, "<img src='file:///" + m_urls.value(emoticon_text) + "'>");
#else
		new_message.replace(emoticon_text, "<img src='file://" + m_urls.value(emoticon_text) + "'>");
#endif
	}
	return new_message;
}

void TabbedChatWindow::moveCursorToEnd()
{
/*	if (m_webkit_mode && m_web_view)
	{

	}
        else*/ if( m_text_browser )
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
}

/*void TabbedChatWindow::scrollWebViewToEnd()
{
	if ( m_scroll_at_max )
	{
		int scroll_maximum = m_web_view->page()->currentFrame()->scrollBarMaximum(Qt::Vertical);
		m_web_view->page()->currentFrame()->setScrollBarValue(Qt::Vertical, scroll_maximum);
	}
	else
	{
		m_web_view->page()->currentFrame()->setScrollBarValue(Qt::Vertical, m_current_scroll_position);
	}
}*/

void TabbedChatWindow::messageDelievered(const QString &identification, int position)
{
/*	if (m_webkit_mode && m_web_view)
	{
		
	}
        else*/ if( m_text_browser )
	{
		int user_space_index = m_tab_chat_index_list.indexOf(identification);
		ContactChatUserSpace *tmp_user_space = m_user_spaces_chat_list.at(user_space_index);
		tmp_user_space->m_chat_history.replace(QString("<img id='%1' src=':/icons/crystal_project/message.png'>").arg(position),
				"<img src=':/icons/crystal_project/message_accept.png'>");
		if ( user_space_index == m_chat_tab_bar->currentIndex() )
		{
			m_text_browser->setHtml(tmp_user_space->m_chat_history);
			moveCursorToEnd();
		}
	}
}

void TabbedChatWindow::checkForActiveState(ContactChatUserSpace *user_space, 
		const QString &message,bool unread_message, bool in)
{
	int index = m_user_spaces_chat_list.indexOf(user_space);
	if ( ((!isActiveWindow() || isMinimized() )|| index != m_chat_tab_bar->currentIndex() )
			&& !unread_message && in)
	{
		qApp->alert(this, 0);
		m_abstract_chat_layer.animateTrayIconMessage();
		m_abstract_chat_layer.addItemToActivationList(user_space->m_protocol_name,
				user_space->m_account_name,
				user_space->m_contact_name, message);
		user_space->m_waiting_for_activation = true;
	}
}

void TabbedChatWindow::windowFocused()
{
	int index = m_chat_tab_bar->currentIndex();
	if ( index < 0 )
		return;
	ContactChatUserSpace *tmp_user_space = m_user_spaces_chat_list.at(index);
	if ( tmp_user_space->m_waiting_for_activation )
	{
		tmp_user_space->m_waiting_for_activation = false;
		m_abstract_chat_layer.windowActivated(tmp_user_space->m_protocol_name,
				tmp_user_space->m_account_name,
				tmp_user_space->m_contact_name);
		
		if ( m_plain_text_edit )
		{
			m_plain_text_edit->setFocus();
		}
	}
}

void TabbedChatWindow::contactTyping(const QString &identification, bool typing)
{
	int user_space_index = m_tab_chat_index_list.indexOf(identification);
	if ( user_space_index > -1 )
	{
		ContactChatUserSpace *tmp_user_space = m_user_spaces_chat_list.at(user_space_index);
		tmp_user_space->m_typing = typing;
		if ( m_chat_tab_bar->currentIndex() == user_space_index  && isActiveWindow())
		{
			if ( m_cliend_id_label )
			{
				m_cliend_id_label->setText(typing?tr("<font color=#00FF00>Typing...</font>"):
					tmp_user_space->m_client_id);
			}
			if ( m_typing_label )
			{
				m_typing_label->setText(typing?QString("<img src=':/icons/typing.png'>"):"");
			}
		}
		else if ( typing )
		{
			TreeModelItem item;
			item.m_protocol_name = tmp_user_space->m_protocol_name;
			item.m_item_type = 0;
			item.m_account_name = tmp_user_space->m_account_name;
			item.m_item_name = tmp_user_space->m_contact_name;
			AbstractNotificationLayer::instance().userMessage(item, "", 2);
		}
		if ( !tmp_user_space->m_unreaded_message || m_chat_tab_bar->currentIndex() == user_space_index )
		{
			m_chat_tab_bar->setTabTextColor(user_space_index,typing?Qt::darkGreen:QColor() );
		}
		else
		{
			m_chat_tab_bar->setTabTextColor(user_space_index,typing?Qt::darkGreen:Qt::red );
		}
	}
}

void TabbedChatWindow::windowActivatedByUser()
{
	if ( isMinimized() )
	{
			setWindowState(windowState() & ~Qt::WindowMinimized | Qt::WindowActive);
	}
	show();
	activateWindow();
	raise();
	setFocus(Qt::OtherFocusReason);
	if ( m_plain_text_edit )
	{
		m_plain_text_edit->setFocus();
	}
}

QStringList TabbedChatWindow::getTabList()
{
	QStringList user_spaces;
	foreach(ContactChatUserSpace *space, m_user_spaces_chat_list)
	{
		user_spaces<<space->m_protocol_name
					<<space->m_account_name
					<<space->m_contact_name;
	}
	return user_spaces;
}

void TabbedChatWindow::on_historyButton_clicked()
{
        ContactChatUserSpace *current_user_space = m_user_spaces_chat_list.at(m_chat_tab_bar->currentIndex());
	if ( current_user_space )
	{
		TreeModelItem item;
		item.m_protocol_name = current_user_space->m_protocol_name;
		item.m_item_type = 0;
		item.m_account_name = current_user_space->m_account_name;
		item.m_item_name = current_user_space->m_contact_name;
		AbstractHistoryLayer::instance().openHistoryWindow(item);
	}
        focusTextEdit();
}

QString TabbedChatWindow::invertMessage(QString &text)
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

void TabbedChatWindow::on_clearChatButton_clicked()
{
	ContactChatUserSpace *current_user_space = m_user_spaces_chat_list.at(m_chat_tab_bar->currentIndex());
/*	if (m_webkit_mode && m_web_view)
	{
		current_user_space->m_chat_history = 
			m_style_output->makeSkeleton(current_user_space->m_contact_nick, 
				current_user_space->m_account_nick, 
				current_user_space->m_contact_nick,
				current_user_space->m_account_avatar,
				current_user_space->m_contact_avatar,
				QDateTime::currentDateTime().toString());
		m_web_view->setHtml(current_user_space->m_chat_history);
		current_user_space->m_message_positions.clear();
		current_user_space->m_lock_for_first_message = false;
	}
        else*/ if ( m_text_browser )
	{
		current_user_space->m_chat_history.clear();
		m_text_browser->clear();
		current_user_space->m_message_positions.clear();
	}
	focusTextEdit();
}

void TabbedChatWindow::on_quoteButton_clicked()
{
	QString selected_text;

/*	if (m_webkit_mode && m_web_view)
	{
		selected_text = m_web_view->selectedText();
	}
        else*/ if ( m_text_browser )
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
	focusTextEdit();
}

void TabbedChatWindow::setMessageTabIcon(int index)
{
	m_chat_tab_bar->setTabIcon(index, QIcon(":/icons/crystal_project/message.png"));
	m_chat_tab_bar->setTabTextColor(index, Qt::red);
}

void TabbedChatWindow::closeCurrentTab()
{
	closeTabAt(m_chat_tab_bar->currentIndex());
}

void TabbedChatWindow::setContactStatusIcon(const QString &identification, const QIcon &icon)
{
	int user_space_index = m_tab_chat_index_list.indexOf(identification);
	if ( user_space_index > -1 )
	{
		ContactChatUserSpace *user_space = m_user_spaces_chat_list.at(user_space_index);
		user_space->m_status_icon = icon;
		m_chat_tab_bar->setTabIcon(user_space_index, icon);
		if ( m_chat_tab_bar->currentIndex() == user_space_index )
		{
			setWindowIcon(icon);
		}
	}
}

/*void TabbedChatWindow::restoreSplitter(const QByteArray &state)
{
	if ( m_main_splitter )
	{
		m_main_splitter->restoreState(state);
	}
}*/

void TabbedChatWindow::onCustomContextMenuRequested(const QPoint &pos)
{
	ContactChatUserSpace *current_user_space = m_user_spaces_chat_list.at(m_chat_tab_bar->currentIndex());
	m_abstract_chat_layer.askForContactMenu(current_user_space->m_protocol_name,
			current_user_space->m_account_name,
			current_user_space->m_contact_name, mapToGlobal(pos));
}

void TabbedChatWindow::onCustomTabContextMenuRequested(const QPoint &pos)
{
	int index = m_chat_tab_bar->tabAt(pos);
	if ( index < 0 )
		index = m_chat_tab_bar->currentIndex();
	ContactChatUserSpace *current_user_space = m_user_spaces_chat_list.at(index);
	m_abstract_chat_layer.askForContactMenu(current_user_space->m_protocol_name,
			current_user_space->m_account_name,
			current_user_space->m_contact_name, mapToGlobal(pos));
}

void TabbedChatWindow::setIconsToButtons()
{
    if ( m_send_button )
    {
    	m_send_button->setIcon(IconManager::instance().getIcon("message") );
    }
    if ( m_info_button )
    {
    	m_info_button->setIcon(IconManager::instance().getIcon("contactinfo") );
    }
    if ( m_history_button )
    {
    	m_history_button->setIcon(IconManager::instance().getIcon("history") );
    }
    if ( m_emoticons_button )
    {
    	m_emoticons_button->setIcon(IconManager::instance().getIcon("emoticon") );
    }
    if ( m_send_picture_button )
    {
    	m_send_picture_button->setIcon(IconManager::instance().getIcon("image") );
    }
    if ( m_send_file_button )
    {
    	m_send_file_button->setIcon(IconManager::instance().getIcon("save_all") );
    }
    if ( m_on_enter_button )
    {
    	m_on_enter_button->setIcon(IconManager::instance().getIcon("key_enter") );
    }
    if ( m_send_typing_button )
    {
    	m_send_typing_button->setIcon(IconManager::instance().getIcon("typing") );
    }
    if ( m_translate_button )
    {
    	m_translate_button->setIcon(IconManager::instance().getIcon("translate") );
    }
    if ( m_quote_button )
    {
    	m_quote_button->setIcon(IconManager::instance().getIcon("quote") );
    }
    if ( m_clear_button )
    {
    	m_clear_button->setIcon(IconManager::instance().getIcon("edituser") );
    }
}

void TabbedChatWindow::showEvent(QShowEvent * event)
{
	windowActivatedByUser();
	windowFocused();
}

void TabbedChatWindow::keyPressEvent ( QKeyEvent *event ) 
{	
        int cur_tab = m_chat_tab_bar->currentIndex ();
	if(event->key() == Qt::Key_Left || event->key() == Qt::Key_Right ||
			event->key() == Qt::Key_Up || event->key() == Qt::Key_Down) {
	    cur_tab+=(event->key() == Qt::Key_Left || event->key() == Qt::Key_Down?-1:1);
	    if(cur_tab<0) cur_tab = m_chat_tab_bar->count()-1;
	    if(cur_tab>=m_chat_tab_bar->count()) cur_tab = 0;
	    m_chat_tab_bar->setCurrentIndex(cur_tab);
	} else if((event->key() == Qt::Key_W) || (event->key() == Qt::Key_Escape)) {
	    closeCurrentTab();
	}

}

void TabbedChatWindow::newsOnLinkClicked(const QUrl &url)
{
	QDesktopServices::openUrl(url);
}

void TabbedChatWindow::setContactClientIcon(const QString &identification)
{
	int user_space_index = m_tab_chat_index_list.indexOf(identification);
	if ( user_space_index > -1 )
	{
		ContactChatUserSpace *user_space = m_user_spaces_chat_list.at(user_space_index);
		readContactInfo(user_space);
		if ( m_chat_tab_bar->currentIndex() == user_space_index  && m_client_label)
		{
				m_client_label->setPixmap(user_space->m_client_pixmap);
                                m_client_label->setWhatsThis(user_space->m_client_id);
		}
	}
}

void TabbedChatWindow::addSeparator(const QString &identification)
{
/*	if (m_webkit_mode && m_web_view)
	{
		int user_space_index = m_tab_chat_index_list.indexOf(identification);
		ContactChatUserSpace *tmp_user_space = m_user_spaces_chat_list.at(user_space_index);		
		if ( user_space_index == m_chat_tab_bar->currentIndex() )
		{
			appendMessage("<hr><div id=\"insert\"></div>",false);
			tmp_user_space->m_lock_for_first_message = false;
		}
		else
		{
			UnreadMessage message_struct;
			message_struct.m_message = "<hr><div id=\"insert\"></div>";
			message_struct.m_same = false;
			tmp_user_space->m_lock_for_first_message = false;
			tmp_user_space->m_unreaded_messages.append(message_struct);
		}
	}
        else*/ if ( m_text_browser )
	{
		int user_space_index = m_tab_chat_index_list.indexOf(identification);
		ContactChatUserSpace *tmp_user_space = m_user_spaces_chat_list.at(user_space_index);
		tmp_user_space->m_chat_history.append("<hr><br>");
		if ( user_space_index == m_chat_tab_bar->currentIndex() )
		{
			m_text_browser->insertHtml("<hr><br>");
			moveCursorToEnd();
		}
	}
}

bool TabbedChatWindow::changeTabID(const QString &old_id, const QString &new_id, const QString &id)
{
    int old_index = m_tab_chat_index_list.indexOf(old_id);
    if(old_index==-1)
        return false;
    int new_index = m_tab_chat_index_list.indexOf(new_id);
    if(new_index>-1)
        return false;
    m_tab_chat_index_list[old_index] = new_id;
    m_user_spaces_chat_list.at(old_index)->m_contact_name = id;
    if(m_chat_tab_bar->currentIndex()==old_index)
    {
        if(m_id_label)
            m_id_label->setText(id);
    }
    return true;
}
