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
//#include <QUiLoader>
#include <QFile>
#include <QVBoxLayout>
#include <QDateTime>
#include <QtXml>
#include <QRegExp>
#include <QTimer>
#include <QSoftMenuBar>

bool ChatEventEater::eventFilter(QObject *obj, QEvent *event)
{
	if ( event->type() == QEvent::KeyPress )
	{
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
        if ( obj->objectName() == "chatInputEdit" ||
        		obj->objectName() == "conferenceTextEdit" )
        {
        	if ( keyEvent->key() == Qt::Key_Return && m_send_on_enter && keyEvent->modifiers() == Qt::NoModifier)
        	{
        		emit sendMessage();
        		return true;
        	}
        	else if (keyEvent->key() == Qt::Key_Return && !m_send_on_enter
        			&& keyEvent->modifiers() == Qt::ControlModifier)
        	{
        		emit sendMessage();
        		return true;
        	}
	}                 else if (keyEvent->key() == Qt::Key_Escape)
	{
	      emit close();
	      return true;
        } /*else if (obj->metaObject()->className() == QWebView::staticMetaObject.className() )
        {
        	if ( keyEvent->matches(QKeySequence::Copy) )
        	{
        		QWebView *temp_web = qobject_cast<QWebView*>(obj);
        		temp_web->triggerPageAction(QWebPage::Copy);
        		return true;
        	}
        }
        }*/ else if ( event->type() == QEvent::FocusIn )
	{
		emit focusedIn();
	}
	return QObject::eventFilter(obj, event);
        }
}

SeparateChatWindow::SeparateChatWindow(const QString &protocol_name,
		const QString &account_name,
		const QString &item_name,
                /*bool webkit_mode,*/
		const QString &emoticon_path,
/*		const QString &chat_style,*/
		const QString &webkit_style,
		const QString &webkit_variant,
		QWidget *parent)
    : QWidget(parent)
    , m_abstract_chat_layer(AbstractChatLayer::instance())
    , m_protocol_name(protocol_name)
    , m_account_name(account_name)
    , m_item_name(item_name)
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
	m_emoticon_widget = 0;
	m_emotic_menu = 0;
	m_font_button = 0;
	m_color_button = 0;
	m_send_picture_button = 0;
	m_send_file_button = 0;
	m_webkit_header_offset = 0;
	m_on_enter_button = 0;
	m_send_typing_button = 0;
	m_typing_label = 0;
	m_client_label = 0;
//	m_main_splitter = 0;
	ui = 0;
	m_last_message_sender = true;
	m_lock_for_first_message = false;
	m_close_after_send = false;
	m_text_changed = false;
	m_last_message_icon_position = 0;
	m_last_message_position = 0;
	
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
            file.close();
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
			m_now_html = m_style_output->makeSkeleton(m_contact_name, 
					m_own_name, 
					m_contact_name,
					m_own_avatar_path,
					m_contact_avatar_path,
					QDateTime::currentDateTime().toString());
			m_webkit_header_offset = m_now_html.size();
			m_web_view->setHtml(m_now_html);

	    }
            else
	    {
	        m_text_browser = qFindChild<QTextBrowser*>(this, "chatViewBrowser"); 
	    }
	    QMetaObject::connectSlotsByName(this);
	    QVBoxLayout *layout = new QVBoxLayout;
	    layout->setMargin(0);
	    layout->addWidget(chat_widget);
	    setLayout(layout);
	}*/
    setEmoticonsToMenu();
	readContactInfo();
	readOwnerInfo();
	focusTextEdit();
    if ( m_send_button )
    {
    	m_send_button->setEnabled(false);
    }
    if ( m_text_browser )
    {
    	m_text_browser->setOpenExternalLinks(true);
    }
    m_event_eater = new ChatEventEater(this);
    connect(m_event_eater, SIGNAL(sendMessage()),
    		this, SLOT(on_sendButton_clicked()));
    connect(m_event_eater, SIGNAL(focusedIn()),
    		this, SLOT(windowFocused()));
    connect(m_event_eater, SIGNAL(close()),
		this, SLOT(close()));
    installEventEater();
	setAttribute(Qt::WA_QuitOnClose, false);
	setAttribute(Qt::WA_DeleteOnClose, true);
	connect(this, SIGNAL(customContextMenuRequested(const QPoint &)),
			SLOT(onCustomContextMenuRequested(const QPoint &)));
	setIconsToButtons();
/*	if ( m_webkit_mode && m_web_view )
	{
		m_web_view->page()->setLinkDelegationPolicy(QWebPage::DelegateAllLinks);
		connect(m_web_view, SIGNAL(linkClicked(const QUrl &)),
				this, SLOT(newsOnLinkClicked(const QUrl &)));
        }*/
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
/*	m_abstract_chat_layer.saveWindowSizeAndPosition(pos(), size(),
                        m_main_splitter ? m_main_splitter->saveState():QByteArray() );*/
	TreeModelItem item;
	item.m_protocol_name = m_protocol_name;
	item.m_account_name = m_account_name;
	item.m_item_name = m_item_name;
	item.m_item_type = 0;
	PluginSystem::instance().chatWindowClosed(item);
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
	if ( m_emoticon_widget )
		delete m_emoticon_widget;
	if ( m_emotic_menu )
		delete m_emotic_menu;
	delete m_emoticons_button;
	
	delete m_font_button;
	delete m_color_button;
	delete m_send_file_button;
	delete m_send_picture_button;
//	delete m_main_splitter;
	delete ui;
}

int SeparateChatWindow::getCursorPosition()
{
	int cursor_position = 0;
/*	if (m_webkit_mode && m_web_view)
	{
		
	}
        else*/ if( m_text_browser )
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

bool SeparateChatWindow::lengthLessThan(const QString &s1, const QString &s2)
{
	return s1.size() > s2.size();
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
		if ( m_close_after_send )
			close();
		m_plain_text_edit->setFocus();
	}
	m_send_button->setEnabled(false);
}

void SeparateChatWindow::addMessage(const QString &message, bool in, const QString &message_date, bool history)
{
    qDebug()<<message;
/*	if (m_webkit_mode && m_web_view)
	{
		QString add_new_message = QString("%1")
				.arg(checkForEmoticons(message));
		QString newMessage;
		if(add_new_message.startsWith("/me "))
			newMessage = m_style_output->makeAction(in?m_contact_name:m_own_name, 
					add_new_message.section(" ",1), !in, message_date, 
					in?m_contact_avatar_path:m_own_avatar_path, true, in?m_item_name:m_account_name, m_protocol_name);
		else
			newMessage = m_style_output->makeMessage(in?m_contact_name:m_own_name, 
					add_new_message, !in, message_date, 
					in?m_contact_avatar_path:m_own_avatar_path, true, in?m_item_name:m_account_name, m_protocol_name,
					(m_last_message_sender == in)&m_lock_for_first_message&(m_last_history==history), history);

		QString js_message = QString("append%2Message(\"%1\");").arg(newMessage.replace("\"","\\\"").replace("\n","\\n")).arg(((m_last_message_sender == in)&m_lock_for_first_message&(m_last_history==history)&!add_new_message.startsWith("/me "))?"Next":"");
		m_web_view->page()->mainFrame()->evaluateJavaScript(js_message);
		m_last_message_sender = in;
		m_lock_for_first_message = !add_new_message.startsWith("/me ");
		m_last_history = history;
	}
        else*/ if ( m_text_browser )
	{
		quint64 tmp_position = m_text_browser->textCursor().position();
		m_text_browser->textCursor().insertImage(":/icons/crystal_project/message.png");
		m_text_browser->insertHtml("&nbsp;");
		QString add_text;
		add_text.append(in?QString("<b><font color='blue'>"):QString("<b><font color='red'>"));
		if ( m_show_names )
		{
			add_text.append(in?m_contact_name:m_own_name);
		}
		add_text.append(QString(" ( %1)</font></b><br>").arg(message_date));
		add_text.append(QString("%1<br>").arg(checkForEmoticons(message)));
		m_current_scroll_position = m_text_browser->verticalScrollBar()->value();
		m_scroll_at_max = m_text_browser->verticalScrollBar()->maximum()
			== m_current_scroll_position; 
		m_text_browser->insertHtml(add_text);
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
	checkForActiveState();
}

void SeparateChatWindow::addServiceMessage(const QString &message)
{
/*	if (m_webkit_mode && m_web_view)
	{
		QString newMessage = m_style_output->makeStatus(message, QDateTime::currentDateTime().toString());
		QString js_message = QString("appendMessage(\"%1\");").arg(newMessage.replace("\"","\\\"").replace("\n","\\n"));
		m_web_view->page()->mainFrame()->evaluateJavaScript(js_message);
		m_lock_for_first_message = false;
	}
        else*/ if ( m_text_browser )
	{
		m_text_browser->append(QString("<font size='-1' color='grey'>%1</font>").arg(message));
	}
	moveCursorToEnd();
}

void SeparateChatWindow::readContactInfo()
{
	TreeModelItem item;
	item.m_protocol_name = m_protocol_name;
	item.m_item_type = 0;
	item.m_account_name = m_account_name;
	item.m_item_name = m_item_name;
	
	m_contact_name = m_item_name;
	
	QStringList addition_contact_info = m_plugin_system.getAdditionalInfoAboutContact(item);
	if ( addition_contact_info.count() > 0)
	{
		m_contact_name = addition_contact_info.at(0);
		if ( m_name_label )
			m_name_label->setText(m_contact_name);
	}
	if ( addition_contact_info.count() > 1 )
	{
		m_contact_avatar_path = addition_contact_info.at(1);
		if ( !m_contact_avatar_path.isEmpty() && m_contact_avatar_label)
			m_contact_avatar_label->setPixmap(QPixmap(m_contact_avatar_path));
	}
	if ( addition_contact_info.count() > 2 )
	{
		m_client_id = addition_contact_info.at(2);
		if ( m_cliend_id_label )
			m_cliend_id_label->setText(m_client_id);
	}
	if ( addition_contact_info.count() > 3 )
	{
		QString additional_data = addition_contact_info.at(3);
		if ( m_additional_label )
			m_additional_label->setText(additional_data.remove('\n'));
	}
	if ( m_id_label )
	{
		m_id_label->setText(m_item_name);
	}
	
	if ( m_client_label )
	{
		m_client_label->setPixmap(AbstractContactList::instance().getItemClientIcon(item));
                m_client_label->setWhatsThis(m_client_id);
	}
	
	setWindowTitle(QString("%1(%2)").arg(m_contact_name).arg(m_account_name));
}

void SeparateChatWindow::readOwnerInfo()
{
	TreeModelItem item;
	item.m_protocol_name = m_protocol_name;
	item.m_item_type = 0;
	item.m_account_name = m_account_name;
	item.m_item_name = m_account_name;
	m_own_name = m_account_name;
	QStringList addition_own_info = m_plugin_system.getAdditionalInfoAboutContact(item);
	if ( addition_own_info.count() > 0 )
	{
		m_own_name = addition_own_info.at(0);
	}
	if ( addition_own_info.count() > 1 )
	{
		m_own_avatar_path = addition_own_info.at(1);
		if ( !m_own_avatar_path.isEmpty() && m_owner_avatar_label)
			m_owner_avatar_label->setPixmap(QPixmap(m_own_avatar_path));
	}
}

void SeparateChatWindow::on_infoButton_clicked()
{
	m_abstract_chat_layer.showContactInformation(m_protocol_name, m_account_name, m_item_name);
	focusTextEdit();
}

void SeparateChatWindow::setEmoticonsToMenu()
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

void SeparateChatWindow::setEmoticonPath(const QString &path)
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

void SeparateChatWindow::insertEmoticon(const QString &emoticon_text)
{
	if ( m_plain_text_edit )
	{
		m_plain_text_edit->insertHtml(" " + emoticon_text + " ");
	}
}

QString SeparateChatWindow::changeEmoticonsPathsToText(const QString &message)
{
	QString new_message = message;
	foreach(QString path, m_urls)
	{
		new_message.replace("<img src='" + path + "'>", m_urls.key(path));
	}
	return new_message;
}

QString SeparateChatWindow::checkForEmoticons(const QString &message)
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

void SeparateChatWindow::createFontMenu()
{
	
}

/*void SeparateChatWindow::on_sendFileButton_clicked()
{
	m_abstract_chat_layer.sendFileTo(m_protocol_name, m_account_name, m_item_name);
	focusTextEdit();
}*/

/*void SeparateChatWindow::on_sendPictureButton_clicked()
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
				m_abstract_chat_layer.sendImageTo(m_protocol_name, m_account_name, m_item_name, iconFile.readAll());
				iconFile.close();
			}
		}
	}
        focusTextEdit();
}*/

void SeparateChatWindow::installEventEater()
{
	if ( m_plain_text_edit )
		m_plain_text_edit->installEventFilter(m_event_eater);
/*	if ( m_web_view )
                m_web_view->installEventFilter(m_event_eater);*/
	installEventFilter(m_event_eater);
}

void SeparateChatWindow::setOptions(bool remove_after, quint16 remove_count,
		bool close_after, bool show_names, bool on_enter, bool typing_notifications)
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

/*void SeparateChatWindow::on_onEnterButton_clicked()
{
	m_event_eater->m_send_on_enter = m_on_enter_button->isChecked();
	focusTextEdit();
}*/

void SeparateChatWindow::on_typingButton_clicked()
{
	m_send_typing_notifications = m_send_typing_button->isChecked();
	focusTextEdit();
}
void SeparateChatWindow::on_chatInputEdit_textChanged()
{
	m_text_changed = true;
	if ( m_send_typing_notifications )
	{
		if ( ! m_typing_changed )
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
	if ( m_cliend_id_label )
	{
		m_cliend_id_label->setText(typing?tr("<font color='green'>Typing...</font>"):
			m_client_id);
	}
	if ( m_typing_label )
	{
		m_typing_label->setText(typing?QString("<img src=':/icons/typing.png'>"):"");
	}
}

/*void SeparateChatWindow::on_translitButton_clicked()
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

void SeparateChatWindow::on_clearChatButton_clicked()
{
/*	if (m_webkit_mode && m_web_view)
	{
		m_now_html = m_style_output->makeSkeleton(m_contact_name, 
				m_own_name, 
				m_contact_name,
				m_own_avatar_path,
				m_contact_avatar_path,
				QDateTime::currentDateTime().toString());
		m_web_view->setHtml(m_now_html);
		m_message_positions.clear();
		m_lock_for_first_message = false;
	}
        else*/ if ( m_text_browser )
	{
		m_text_browser->clear();
		m_message_positions.clear();
	}
	focusTextEdit();
}

void SeparateChatWindow::on_quoteButton_clicked()
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

void SeparateChatWindow::messageDelievered(int position)
{
/*	if (m_webkit_mode && m_web_view)
	{
		
	}
        else*/ if( m_text_browser )
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

/*void SeparateChatWindow::scrollWebViewToEnd()
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

void SeparateChatWindow::windowActivatedByUser()
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

void SeparateChatWindow::checkForActiveState()
{
	if ( !isActiveWindow() || isMinimized() )
	{
		qApp->alert(this, 0);
		m_abstract_chat_layer.animateTrayIconMessage();
		m_waiting_for_activation = true;
	}
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

void SeparateChatWindow::on_historyButton_clicked()
{
	TreeModelItem item;
	item.m_protocol_name = m_protocol_name;
	item.m_item_type = 0;
	item.m_account_name = m_account_name;
	item.m_item_name = m_item_name;
	AbstractHistoryLayer::instance().openHistoryWindow(item);
	focusTextEdit();
}

void SeparateChatWindow::setDefaultForm()
{
	ui = new Ui::Form;
	ui->setupUi(this);
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
		m_now_html = m_style_output->makeSkeleton(m_contact_name, 
				m_own_name, 
				m_contact_name,
				m_own_avatar_path,
				m_contact_avatar_path,
				QDateTime::currentDateTime().toString());
		m_webkit_header_offset = m_now_html.size();
		m_web_view->setHtml(m_now_html);
    }
    else*/
    {
        m_text_browser = new QTextBrowser(this);
        ui->vboxLayout->insertWidget(1, m_text_browser);
    }
    
}

void SeparateChatWindow::focusTextEdit()
{
    if ( m_plain_text_edit )
    {
    	m_plain_text_edit->setFocus();
    }
}

/*void SeparateChatWindow::restoreSplitter(const QByteArray &state)
{
	if ( m_main_splitter )
	{
		m_main_splitter->restoreState(state);
	}
}*/

void SeparateChatWindow::onCustomContextMenuRequested(const QPoint &pos)
{
	m_abstract_chat_layer.askForContactMenu(m_protocol_name, m_account_name,
			m_item_name, mapToGlobal(pos));

}

void SeparateChatWindow::setIconsToButtons()
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

void SeparateChatWindow::newsOnLinkClicked(const QUrl &url)
{
	QDesktopServices::openUrl(url);
}

void SeparateChatWindow::setContactClientIcon()
{
	readContactInfo();
}

void SeparateChatWindow::addSeparator()
{
/*	if (m_webkit_mode && m_web_view)
	{
		QString js_message = QString("appendMessage(\"<hr><div id=\\\"insert\\\"></div>\");");
		m_web_view->page()->mainFrame()->evaluateJavaScript(js_message);
		m_lock_for_first_message = false;
//		m_web_view->page()->mainFrame()->evaluateJavaScript("appendMessage(\"<hr><div id=\\\"insert\\\"></div>\",\"new\");");
//		qDebug() << m_web_view->page()->mainFrame()->toHtml();
		//m_now_html.append("<hr><br>");
		//m_web_view->setHtml(m_now_html);
	}
        else*/ if( m_text_browser )
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
    m_id_label->setText(id);
}
