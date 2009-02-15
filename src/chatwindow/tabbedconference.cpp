/*
    TabbedConference

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

#include "tabbedconference.h"
#include "../abstractchatlayer.h"
#include "../iconmanager.h"
#include <QScrollBar>
#include <QtXml>

TabbedConference::TabbedConference(
		const QString &emoticon_path,
/*		bool webkit_mode,*/
		const QString &style_path,
		const QString &style_variant,
		QWidget *parent)
		: QWidget(parent)    
		, m_emoticons_path(emoticon_path)
                /*, m_webkit_mode(webkit_mode)*/
		, m_webkit_style_path(style_path)
		, m_webkit_variant(style_variant)
		, m_abstract_chat_layer(AbstractChatLayer::instance())
{
	ui.setupUi(this);
	setAttribute(Qt::WA_QuitOnClose, false);
	setAttribute(Qt::WA_DeleteOnClose, true);
	m_close_tab_button = 0;
	m_tab_layout = 0;
//	m_web_view = 0;
	m_text_browser = 0;
	m_previous_tab = -1;
	m_current_scroll_position = 0;
	m_scroll_at_max = false;

	m_chat_tab_bar = new GeneralTabBar;
	m_chat_tab_bar->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
	connect(m_chat_tab_bar, SIGNAL(currentChanged(int)), this, SLOT(currentTabChanged(int)));
	connect(m_chat_tab_bar, SIGNAL(closeTab( int )), this, SLOT(closeTabAt(int)));
	m_close_tab_button = new QToolButton;
	m_close_tab_button->resize(22,22);
	m_close_tab_button->setAutoRaise(true);
	connect(m_close_tab_button, SIGNAL(clicked()), this, SLOT(closeCurrentTab()));
	m_tab_layout = new QHBoxLayout;
	m_tab_layout->setMargin(0);
	m_tab_layout->addWidget(m_chat_tab_bar);
	m_tab_layout->addWidget(m_close_tab_button);
        ui.vboxLayout->insertLayout(0,m_tab_layout);
/*	if ( m_webkit_mode )
	{
		QFrame *frame = new QFrame(this);
	    QGridLayout *layout = new QGridLayout(frame);
	    m_web_view = new QWebView(this);
            layout->addWidget(m_web_view);
	    frame->setLayout(layout);
	    layout->setMargin(0);
	    frame->setFrameShape(QFrame::StyledPanel);
	    frame->setFrameShadow(QFrame::Sunken);
	    ui.chatSplitter->insertWidget(0, frame);
		m_style_output = new ChatWindowStyleOutput(m_webkit_style_path,
					m_webkit_variant);

	}
        else*/
	{
		m_text_browser = new QTextBrowser(this);
            ui.vboxLayout->insertWidget(2, m_text_browser);
	}
	
	setIconsToButtons();
	setEmoticonsToMenu();
    m_event_eater = new ChatEventEater(this);
    connect(m_event_eater, SIGNAL(sendMessage()),
    		this, SLOT(on_sendButton_clicked()));
    connect(m_event_eater, SIGNAL(focusedIn()),
    		this, SLOT(windowFocused()));
    ui.conferenceTextEdit->installEventFilter(m_event_eater);
    installEventFilter(m_event_eater);
/*	if ( m_webkit_mode && m_web_view )
	{
		m_web_view->page()->setLinkDelegationPolicy(QWebPage::DelegateAllLinks);
		connect(m_web_view, SIGNAL(linkClicked(const QUrl &)),
				this, SLOT(newsOnLinkClicked(const QUrl &)));
    		m_web_view->installEventFilter(m_event_eater);
	}
        else*/ if ( m_text_browser )
    {
    	m_text_browser->setOpenExternalLinks(true);
    }
	//m_already_hred = false;
	//m_contact_list = new ConferenceContactList(protocol_name,conference_name,account_name,ui.conferenceList);

	m_item_delegate = new ContactListItemDelegate();
	m_item_delegate->setTreeView(ui.conferenceList);
	ui.conferenceList->setItemDelegate(m_item_delegate);
	ui.conferenceList->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	ui.conferenceList->setSelectionMode(QAbstractItemView::SingleSelection);
	ui.conferenceList->setSelectionBehavior(QAbstractItemView::SelectItems);
	m_cl_event_eater = new ConferenceContactListEventEater();
        ui.conferenceList->findChild<QObject *>("qt_scrollarea_viewport")->installEventFilter(m_cl_event_eater);
	ui.conferenceList->installEventFilter(m_cl_event_eater);	
	
	//m_scroll_at_max = true;
	//m_current_scroll_position = 0;
/*	QList<int> sizes;
	sizes.append(400);
	sizes.append(20);
	ui.chatSplitter->setSizes(sizes);
	sizes.clear();
	sizes.append(400);
	sizes.append(100);
        ui.listSplitter->setSizes(sizes);*/
	//m_last_history=true;
	ui.conferenceTextEdit->installEventFilter(this);
}

TabbedConference::~TabbedConference()
{
	m_abstract_chat_layer.removeTabbedConference();
	int count_tab = m_chat_tab_bar->count();
	for( int i = 0; i < count_tab; i++)
	{
		closeTabAt(0);
	}
/*	m_abstract_chat_layer.saveConferenceSizeAndPosition(pos(), size(),
                        ui.chatSplitter->saveState(), ui.listSplitter->saveState());*/
}

void TabbedConference::setOptions(bool remove_after, quint16 remove_count,
		bool close_after, bool show_names, bool on_enter)
{
	m_remove_message_after = remove_after;
	m_remove_count = remove_count;
	m_close_after_send = close_after;
	m_show_names = show_names;
	m_event_eater->m_send_on_enter = on_enter;
	ui.onEnterButton->setChecked(on_enter);
}

void TabbedConference::setIconsToButtons()
{
	ui.sendButton->setIcon(IconManager::instance().getIcon("message"));
	ui.historyButton->setIcon(IconManager::instance().getIcon("history"));
	ui.emoticonButton->setIcon(IconManager::instance().getIcon("emoticon"));
	ui.translitButton->setIcon(IconManager::instance().getIcon("translate"));
	ui.onEnterButton->setIcon(IconManager::instance().getIcon("key_enter"));
	ui.quoteButton->setIcon(IconManager::instance().getIcon("quote"));
	ui.clearButton->setIcon(IconManager::instance().getIcon("clear"));
	m_close_tab_button->setIcon(IconManager::instance().getIcon("deletetab"));
}

void TabbedConference::setEmoticonsToMenu()
{
	m_emotic_menu = new QMenu(ui.emoticonButton);
	m_emoticon_action = new QWidgetAction(m_emotic_menu);
	m_emoticon_widget = new ChatEmoticonMenu(m_emotic_menu);
	m_emoticon_action->setDefaultWidget(m_emoticon_widget);
	m_emotic_menu->addAction(m_emoticon_action);
	ui.emoticonButton->setMenu(m_emotic_menu);
	setEmoticonPath(m_emoticons_path);
	connect(m_emoticon_widget, SIGNAL(insertSmile(const QString &)), this,
			SLOT(insertEmoticon(const QString &)));
}

void TabbedConference::setEmoticonPath(const QString &path)
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
		}
		m_emoticon_widget->setEmoticons(m_emoticon_list, dirPath);
	}
}

void TabbedConference::insertEmoticon(const QString &emoticon_text)
{
		ui.conferenceTextEdit->insertPlainText(" " + emoticon_text + " ");
}

/*void TabbedConference::restoreSplitters(const QByteArray &state, const QByteArray &state2)
{
	ui.chatSplitter->restoreState(state);
	ui.listSplitter->restoreState(state2);
}*/

void TabbedConference::currentTabChanged(int index)
{
	if ( m_chat_tab_bar->count() && ( m_previous_tab > -1 ) 
			&& ( m_previous_tab < m_conf_spaces_chat_list.count()))
	{
		m_conf_spaces_chat_list.at(m_previous_tab)->m_send_text = ui.conferenceTextEdit->toPlainText();
	}

/*	if( m_webkit_mode && m_web_view && m_previous_tab>-1 && m_previous_tab != index && index>-1 && m_previous_tab < m_conf_spaces_chat_list.count())
	{
		m_conf_spaces_chat_list.at(m_previous_tab)->m_chat_history = m_web_view->page()->mainFrame()->toHtml();		
        }*/
	
	m_previous_tab = index;

	if ( m_chat_tab_bar->count() && m_conf_spaces_chat_list.count() )
	{
		ConferenceUserSpace *tmp_conf_space = m_conf_spaces_chat_list.at(index);
                tmp_conf_space->m_waiting_for_activation = false;
		tmp_conf_space->m_contact_list->nowActive();
		m_cl_event_eater->m_contact_list = tmp_conf_space->m_contact_list;
/*		if (m_webkit_mode && m_web_view)
		{
			m_web_view->setHtml(tmp_conf_space->m_chat_history);
			foreach(UnreadConfMessage message_struct,tmp_conf_space->m_unreaded_messages)
			{
				appendMessage(message_struct.m_message,message_struct.m_sender);
			}
			tmp_conf_space->m_unreaded_messages.clear();
		}
                else*/ if ( m_text_browser )
		{
			m_text_browser->setHtml(tmp_conf_space->m_chat_history);
		}	
		moveCursorToEnd();

		
		if ( tmp_conf_space->m_unreaded_message )
		{
			
			tmp_conf_space->m_unreaded_message = false;
			m_chat_tab_bar->setTabTextColor(index, QColor());
			m_chat_tab_bar->setTabText(index, tmp_conf_space->m_conference_name);
			/*tmp_user_space->m_unreaded_message = false;
			m_chat_tab_bar->setTabIcon(index, tmp_user_space->m_status_icon);
			m_chat_tab_bar->setTabTextColor(index, QColor());
			m_abstract_chat_layer.windowActivated(tmp_user_space->m_protocol_name,
					tmp_user_space->m_account_name,
					tmp_user_space->m_contact_name);*/
		}

			ui.conferenceTextEdit->setPlainText(tmp_conf_space->m_send_text);
			ui.topicLineEdit->setText(tmp_conf_space->m_topic);
			setWindowTitle(tmp_conf_space->m_conference_name);
		
		//setWindowTitle(QString("%1(%2)").arg(tmp_user_space->m_contact_nick)
		//		.arg(tmp_user_space->m_account_name));
		//setWindowIcon(m_chat_tab_bar->tabIcon(index));
		//m_scroll_at_max = true;
	}
}
void TabbedConference::closeTabAt(int index)
{
	if ( index < 0 )
		return;
        if ( m_conf_spaces_chat_list.size() <= index )
            return;
	ConferenceUserSpace *current_conf_space = m_conf_spaces_chat_list.at(index);

	if ( current_conf_space )
	{

		PluginSystem::instance().leaveConference(current_conf_space->m_protocol_name, current_conf_space->m_conference_name,
			current_conf_space->m_account_name);
	}
	m_tab_chat_index_list.removeAt(index);
	delete m_conf_spaces_chat_list.at(index)->m_contact_list;
	delete m_conf_spaces_chat_list.at(index)->m_tab_compl;
	delete m_conf_spaces_chat_list.at(index);
	m_conf_spaces_chat_list.removeAt(index);
	m_chat_tab_bar->removeTab(index);

	if ( !m_chat_tab_bar->count() )
	{
		close();
	}
}
void TabbedConference::closeCurrentTab()
{
	closeTabAt(m_chat_tab_bar->currentIndex());
}
void TabbedConference::on_sendButton_clicked()
{
	
	ConferenceUserSpace *current_conf_space = m_conf_spaces_chat_list.at(m_chat_tab_bar->currentIndex());
	if ( current_conf_space )
	{
		m_abstract_chat_layer.sendMessageToConference(current_conf_space->m_protocol_name,
				current_conf_space->m_conference_name, current_conf_space->m_account_name,
				ui.conferenceTextEdit->toPlainText());
		ui.conferenceTextEdit->clear();
		if ( m_close_after_send )
			close();
	}
	ui.conferenceTextEdit->moveCursor(QTextCursor::Start);
	ui.conferenceTextEdit->setFocus();
}
void TabbedConference::windowFocused()
{
	ui.conferenceTextEdit->setFocus();
	if ( m_chat_tab_bar->count() && m_conf_spaces_chat_list.count() )
	{
	ConferenceUserSpace *current_conf_space = m_conf_spaces_chat_list.at(m_chat_tab_bar->currentIndex());

	if ( current_conf_space )
	{
			current_conf_space->m_unreaded_message = false;
			m_chat_tab_bar->setTabTextColor(m_chat_tab_bar->currentIndex(), QColor());
			m_chat_tab_bar->setTabText(m_chat_tab_bar->currentIndex(), current_conf_space->m_conference_name);
			setWindowTitle(current_conf_space->m_conference_name);
	}
	}
	//m_already_hred = false;
}
void TabbedConference::newsOnLinkClicked(const QUrl &url)
{
        QDesktopServices::openUrl(url);
}

bool TabbedConference::conferenceTabOpened(const QString &identification)
{
	return m_tab_chat_index_list.contains(identification);
}

void TabbedConference::createTab(const QString &protocol_name,
   	const QString &account_name,
   	const QString &conference_name,
   	const QString &identification)
{
	if ( !m_tab_chat_index_list.contains(identification) )
	{
		m_tab_chat_index_list.append(identification);
		int index = m_chat_tab_bar->addTab(conference_name);
		m_chat_tab_bar->setTabIcon(index, IconManager::instance().getIcon("chat"));
		ConferenceUserSpace *conference_space = new ConferenceUserSpace;
		conference_space->m_account_name = account_name;
		conference_space->m_protocol_name = protocol_name;
		conference_space->m_conference_name = conference_name;
		conference_space->m_last_message_sender = false;
		conference_space->m_lock_for_first_message = false;
                conference_space->m_waiting_for_activation = false;
		conference_space->m_unreaded_message = false;
		conference_space->m_last_message_icon_position = 0;
		conference_space->m_last_message_position = 0;
		conference_space->m_last_history = true;
		conference_space->m_contact_list = new ConferenceContactList(protocol_name,conference_name,account_name,ui.conferenceList);
		conference_space->m_tab_compl = new TabCompletion(this);
		conference_space->m_tab_compl->setTextEdit(ui.conferenceTextEdit);
		conference_space->m_tab_compl->setContactList(conference_space->m_contact_list);
		m_conf_spaces_chat_list.append(conference_space);
/*		if ( m_webkit_mode )
		{
			conference_space->m_chat_history = m_style_output->makeSkeleton("", 
					"",
					"", 
					"",
					"",
					QDateTime::currentDateTime().toString());
                }*/
		if ( index == m_chat_tab_bar->currentIndex() )
		{
			currentTabChanged(index);
		}
		m_chat_tab_bar->setCurrentIndex(index);
	}
	else
	{
		m_chat_tab_bar->setCurrentIndex(m_tab_chat_index_list.indexOf(identification));
		windowActivatedByUser();
	}	
}

void TabbedConference::windowActivatedByUser()
{
	if ( isMinimized() )
	{
                setWindowState(windowState() & ~Qt::WindowMinimized | Qt::WindowActive);
	}
	show();
	activateWindow();
	raise();
	setFocus(Qt::OtherFocusReason);

	ui.conferenceTextEdit->setFocus();
}

void TabbedConference::setOwnerNickName(const QString &identification, const QString &nickname)
{
	int conf_space_index = m_tab_chat_index_list.indexOf(identification);
	if ( conf_space_index > -1 )
	{
		ConferenceUserSpace *tmp_conf_space = m_conf_spaces_chat_list.at(conf_space_index);
		tmp_conf_space->m_mine_nick = nickname;
	}
}

void TabbedConference::addMessage(const QString &identification, const QString &from,
    	const QString &message, const QString &date, bool history)
{
	int conf_space_index = m_tab_chat_index_list.indexOf(identification);
	if ( conf_space_index > -1 )
	{
	ConferenceUserSpace *tmp_conf_space = m_conf_spaces_chat_list.at(conf_space_index);


	if ( !history )
		checkForActive(tmp_conf_space, message);
	
/*	if (m_webkit_mode && m_web_view)
	{
		QString add_new_message = QString("%1")
					.arg(checkForEmoticons(message));
                bool same_from = (tmp_conf_space->m_last_sender == from) && tmp_conf_space->m_lock_for_first_message;
		QString newMessage;
		if(add_new_message.startsWith("/me "))
                {
			newMessage = m_style_output->makeAction(from, 
			add_new_message.section(" ",1), from == tmp_conf_space->m_mine_nick, date, 
                                        "", true,
					from, tmp_conf_space->m_protocol_name);
                        tmp_conf_space->m_lock_for_first_message = false;
                        same_from = false;
                }
		else
                {
			newMessage = m_style_output->makeMessage(from, 
					add_new_message, from == tmp_conf_space->m_mine_nick, date, 
                                        "", true,
					from, tmp_conf_space->m_protocol_name, 
                                        tmp_conf_space->m_lock_for_first_message && same_from && tmp_conf_space->m_last_history == history,history);
                        tmp_conf_space->m_lock_for_first_message = true;
                }
			if(conf_space_index != m_chat_tab_bar->currentIndex())
			{
				UnreadConfMessage message_struct;
				message_struct.m_message = newMessage;
                                message_struct.m_sender = same_from;
                                tmp_conf_space->m_unreaded_messages.append(message_struct);
                                tmp_conf_space->m_unreaded_message = true;
			} else
			{

				appendMessage(newMessage,same_from);

				//QString js_message = QString("append%2Message(\"%1\");").arg(newMessage.replace("\"","\\\"").replace("\n","\\n")).arg(m_last_history==history&&same_from&&!add_new_message.startsWith("/me ")?"Next":"");
				//m_web_view->page()->mainFrame()->evaluateJavaScript(js_message);
			
			}


		tmp_conf_space->m_last_sender = add_new_message.startsWith("/me ")?"":from;

                tmp_conf_space->m_last_history = history;
	}
        else*/ if( m_text_browser )
	{
		QString add_text;
		add_text.append(QString("<msgdiv id='%1'>").arg(tmp_conf_space->m_last_message_position));
		add_text.append(tmp_conf_space->m_mine_nick != from
				?QString("<b><font color=#0000FF>"):QString("<b><font color=#FF0000>"));
		add_text.append(QString("[%1]").arg(date));
		
		if ( m_show_names )
		{
			add_text.append(QString(" %1").arg(Qt::escape(from)));
		}
		add_text.append(": </font></b>");
		add_text.append(QString("%1<br>").arg(checkForEmoticons(message)));
		//m_current_scroll_position = m_text_browser->verticalScrollBar()->value();
		//tmp_conf_space->m_scroll_at_max = tmp_conf_space->m_text_browser->verticalScrollBar()->maximum()
		//	== m_current_scroll_position; 

		//m_text_browser->moveCursor(QTextCursor::End);
		//m_text_browser->insertHtml(add_text);
		//moveCursorToEnd();
		if ( m_remove_message_after )
		{
			if ( tmp_conf_space->m_message_positions.count() >= m_remove_count )
			{
					int temp_position = tmp_conf_space->m_message_positions.at(0);
					QRegExp rx(QString("<msgdiv id='%1'>*</msgdiv>").arg(temp_position));
					rx.setMinimal(true);
					rx.setPatternSyntax(QRegExp::Wildcard);
					tmp_conf_space->m_chat_history.remove(rx);
					tmp_conf_space->m_message_positions.removeAt(0);
			}
				tmp_conf_space->m_message_positions.append(tmp_conf_space->m_last_message_position);
				tmp_conf_space->m_last_message_position++;
		}
		tmp_conf_space->m_chat_history.append(add_text + "</msgdiv>");

		if ( conf_space_index == m_chat_tab_bar->currentIndex() )
			{
				m_current_scroll_position = m_text_browser->verticalScrollBar()->value();
				m_scroll_at_max = m_text_browser->verticalScrollBar()->maximum()
					== m_current_scroll_position; 
				m_text_browser->setHtml(tmp_conf_space->m_chat_history);
				moveCursorToEnd();
			}
			else
			{
				//setMessageTabIcon(user_space_index);
				tmp_conf_space->m_unreaded_message = true;
			}

	}
	if(tmp_conf_space->m_mine_nick != from && message.contains(tmp_conf_space->m_mine_nick)){
		tmp_conf_space->m_tab_compl->setLastReferrer(from);
	}
}
}

/*void TabbedConference::appendMessage(QString message, bool same_from)
{
	
	QString js_message = QString("append%2Message(\"%1\");").arg(message.replace("\"","\\\"").replace("\n","\\n")).arg(same_from?"Next":"");
	m_web_view->page()->mainFrame()->evaluateJavaScript(js_message);	
}*/

QString TabbedConference::checkForEmoticons(const QString &message)
{
	QString new_message = message;
	foreach(QString emoticon_text, m_urls.keys())
	{
#if defined(Q_OS_WIN32)
		new_message.replace(emoticon_text, "<img src='file:///" + m_urls.value(emoticon_text) + "'>");
#else
		new_message.replace(emoticon_text, "<img src='file://" + m_urls.value(emoticon_text) + "'>");
#endif
	}
	return new_message;
}

void TabbedConference::moveCursorToEnd()
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

void TabbedConference::addServiceMessage(const QString &identification, const QString &message, const QString &date)
{
        int conf_space_index = m_tab_chat_index_list.indexOf(identification);
        if(conf_space_index<0)
            return;
        ConferenceUserSpace *tmp_conf_space = m_conf_spaces_chat_list.at(conf_space_index);
	
/*	if (m_webkit_mode && m_web_view)
	{

		QString newMessage = m_style_output->makeStatus(message, date);
		
		if ( conf_space_index == m_chat_tab_bar->currentIndex() )
		{
			appendMessage(newMessage,false);
			tmp_conf_space->m_lock_for_first_message = false;
			tmp_conf_space->m_last_sender = "";
		}
		else
		{
			UnreadConfMessage message_struct;
			message_struct.m_message = newMessage;
			message_struct.m_sender = false;
			tmp_conf_space->m_lock_for_first_message = false;
			tmp_conf_space->m_unreaded_messages.append(message_struct);
		}
		
		
	}
        else*/ if ( m_text_browser )
        {
		tmp_conf_space->m_chat_history.append(QString("<font size='-1' color=#808080>%1</font><br>").arg(message));
		if ( conf_space_index == m_chat_tab_bar->currentIndex() )
			m_text_browser->append(QString("<font size='-1' color=#808080>%1</font>").arg(message));
		
	}
	moveCursorToEnd();
}

void TabbedConference::addConferenceItem(const QString &identification, const QString &nickname)
{
	int conf_space_index = m_tab_chat_index_list.indexOf(identification);
	if(conf_space_index==-1)
		return;
	m_conf_spaces_chat_list.at(conf_space_index)->m_contact_list->addConferenceItem(nickname);
}

void TabbedConference::removeConferenceItem(const QString &identification, const QString &nickname)
{
	int conf_space_index = m_tab_chat_index_list.indexOf(identification);
	if(conf_space_index==-1)
		return;
	m_conf_spaces_chat_list.at(conf_space_index)->m_contact_list->removeConferenceItem(nickname);
}

void TabbedConference::renameConferenceItem(const QString &identification, const QString &nickname, const QString &new_nickname)
{
	int conf_space_index = m_tab_chat_index_list.indexOf(identification);
	if(conf_space_index==-1)
		return;
	m_conf_spaces_chat_list.at(conf_space_index)->m_contact_list->renameConferenceItem(nickname, new_nickname);
}

void TabbedConference::setConferenceItemStatus(const QString &identification, const QString &nickname, const QIcon &icon, const QString &status, int mass)
{
	int conf_space_index = m_tab_chat_index_list.indexOf(identification);
	if(conf_space_index==-1)
		return;
	m_conf_spaces_chat_list.at(conf_space_index)->m_contact_list->setConferenceItemStatus(nickname,icon,status,mass);
}

void TabbedConference::setConferenceItemIcon(const QString &identification, const QString &nickname,	const QIcon &icon, int position)
{
	int conf_space_index = m_tab_chat_index_list.indexOf(identification);
	if(conf_space_index==-1)
		return;
	m_conf_spaces_chat_list.at(conf_space_index)->m_contact_list->setConferenceItemIcon(nickname, icon, position);
}

void TabbedConference::setConferenceItemRole(const QString &identification, const QString &nickname, const QIcon &icon, const QString &role, int mass)
{
	int conf_space_index = m_tab_chat_index_list.indexOf(identification);
	if(conf_space_index==-1)
		return;
	m_conf_spaces_chat_list.at(conf_space_index)->m_contact_list->setConferenceItemRole(nickname,icon,role,mass);
}

bool TabbedConference::eventFilter(QObject *obj, QEvent *event)
{
	if (obj == ui.conferenceTextEdit && event->type() == QEvent::KeyPress)
	{
		int index = m_chat_tab_bar->currentIndex();
		if(index<0 || index>=m_conf_spaces_chat_list.size())
			return QObject::eventFilter( obj, event );
		TabCompletion *tab_compl = m_conf_spaces_chat_list.at(index)->m_tab_compl;
		QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);		
		if ( keyEvent->key() == Qt::Key_Tab ) 
		{
			tab_compl->tryComplete();
			return true;
		}

		tab_compl->reset();
		return false;
	}
	return QObject::eventFilter( obj, event );
}

void TabbedConference::on_historyButton_clicked()
{

}

void TabbedConference::on_translitButton_clicked()
{
		QString txt = ui.conferenceTextEdit->toPlainText();
		if ( ui.conferenceTextEdit->textCursor().hasSelection() )
		{
			QString sel_text = ui.conferenceTextEdit->textCursor().selectedText();
			sel_text = invertMessage(sel_text);
			txt.replace(ui.conferenceTextEdit->textCursor().selectionStart(), sel_text.length(), sel_text);
		} 
		else 
		{
			txt = invertMessage(txt);
		}
		ui.conferenceTextEdit->clear();
		ui.conferenceTextEdit->insertPlainText(txt);
	focusTextEdit();
}

void TabbedConference::on_onEnterButton_clicked()
{
	m_event_eater->m_send_on_enter = ui.onEnterButton->isChecked();
	ui.conferenceTextEdit->setFocus();
}

void TabbedConference::on_quoteButton_clicked()
{
	QString selected_text;

/*		if (m_webkit_mode && m_web_view)
		{
			selected_text = m_web_view->selectedText();
		}
                else*/ if ( m_text_browser )
		{
			selected_text = m_text_browser->textCursor().selectedText();
		}
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
				ui.conferenceTextEdit->insertPlainText(mes.prepend("> ") + "\n");
				ui.conferenceTextEdit->moveCursor(QTextCursor::End);
				ui.conferenceTextEdit->ensureCursorVisible();
				ui.conferenceTextEdit->setFocus();
			}		
		focusTextEdit();
}
void TabbedConference::on_clearButton_clicked()
{
	ConferenceUserSpace *current_conf_space = m_conf_spaces_chat_list.at(m_chat_tab_bar->currentIndex());
/*	if (m_webkit_mode && m_web_view)
	{
		current_conf_space->m_chat_history = 
			m_style_output->makeSkeleton("", 
				"", 
				"",
				"",
				"",
				QDateTime::currentDateTime().toString());
		m_web_view->setHtml(current_conf_space->m_chat_history);
		current_conf_space->m_message_positions.clear();
		current_conf_space->m_lock_for_first_message = false;
		current_conf_space->m_last_sender = "";
	}
        else*/ if ( m_text_browser )
	{
		current_conf_space->m_chat_history.clear();
		m_text_browser->clear();
		current_conf_space->m_message_positions.clear();
	}
	focusTextEdit();
}

void TabbedConference::focusTextEdit()
{
    	ui.conferenceTextEdit->setFocus();
}

QString TabbedConference::invertMessage(QString &text)
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

void TabbedConference::on_topicButton_clicked()
{
	ConferenceUserSpace *current_conf_space = m_conf_spaces_chat_list.at(m_chat_tab_bar->currentIndex());

	if ( current_conf_space )
		AbstractChatLayer::instance().showConferenceTopicSettings(current_conf_space->m_protocol_name,
		current_conf_space->m_account_name, current_conf_space->m_conference_name);
}

void TabbedConference::on_configButton_clicked()
{
	ConferenceUserSpace *current_conf_space = m_conf_spaces_chat_list.at(m_chat_tab_bar->currentIndex());

	if ( current_conf_space )
		PluginSystem::instance().showConferenceMenu(current_conf_space->m_protocol_name,
		current_conf_space->m_account_name, current_conf_space->m_conference_name, mapToGlobal(ui.configButton->pos()));
}

void TabbedConference::setConferenceTopic(const QString &identification, const QString &topic)
{
	int conf_space_index = m_tab_chat_index_list.indexOf(identification);
	if ( conf_space_index > -1 )
	{
	ConferenceUserSpace *tmp_conf_space = m_conf_spaces_chat_list.at(conf_space_index);
	tmp_conf_space->m_topic = topic;
	if ( m_chat_tab_bar->currentIndex() == conf_space_index )
		ui.topicLineEdit->setText(topic);
	}
}

void TabbedConference::checkForActive(ConferenceUserSpace *conf_space, const QString &message)
{
	int index = m_conf_spaces_chat_list.indexOf(conf_space);
        if ( (!isActiveWindow() || isMinimized()) )
			//&& !m_already_hred )
        {
            if(!conf_space->m_waiting_for_activation)
            {
/*                    if (m_webkit_mode && m_web_view)
                    {
                        if (m_chat_tab_bar->currentIndex()==index)
                            appendMessage("<hr id=\"separator\"><div id=\"insert\"></div>",false);
                        else
                        {
                            UnreadConfMessage message_struct;
                            message_struct.m_message = "<hr id=\"separator\"><div id=\"insert\"></div>";
                            message_struct.m_sender = false;
                            conf_space->m_unreaded_messages.append(message_struct);
                        }
                        conf_space->m_lock_for_first_message = false;
                    }
                    else*/ if ( m_text_browser )
                    {
                        conf_space->m_chat_history.append("<hr><br></div>");
                        if (index== m_chat_tab_bar->currentIndex())
                            m_text_browser->append(QString("<hr><br></div>").arg(message));
                    }
                    conf_space->m_waiting_for_activation = true;
            }
            m_chat_tab_bar->setTabText(index, "*" + conf_space->m_conference_name);

            conf_space->m_unreaded_message = true;
            m_chat_tab_bar->setTabTextColor(index, Qt::red);
            if ( index == m_chat_tab_bar->currentIndex())
            {
                setWindowTitle("*" + conf_space->m_conference_name);
            }
		//m_already_hred = true;
	}
	if ( message.contains(conf_space->m_mine_nick))
	{
		qApp->alert(this, 0);
	}
}

void TabbedConference::showEvent(QShowEvent * event)
{
	//windowActivatedByUser();
	windowFocused();
}
