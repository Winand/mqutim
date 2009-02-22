/*
    SeparateConference

    Copyright (c) 2008 by Rustam Chakin <qutim.develop@gmail.com>
                                        Nigmatullin Ruslan <euroelessar@gmail.com>

 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************
*/

#include "separateconference.h"
#include "../abstractchatlayer.h"
#include "../iconmanager.h"
#include <QScrollBar>
#include <QtXml>
#include <QSoftMenuBar>

SeparateConference::SeparateConference(
		const QString &protocol_name,
		const QString &conference_name,
		const QString &account_name,
		const QString &emoticon_path,
                /*bool webkit_mode,*/
		const QString &style_path,
		const QString &style_variant,
		QWidget *parent)
    : QWidget(parent)
    , m_abstract_chat_layer(AbstractChatLayer::instance())
    , m_conference_name(conference_name)
    , m_account_name(account_name)
    , m_protocol_name(protocol_name)
    , m_emoticons_path(emoticon_path)
/*    , m_webkit_mode(webkit_mode)*/
    , m_webkit_style_path(style_path)
    , m_webkit_variant(style_variant)
{
	ui.setupUi(this);
	conferenceTextEdit = ui.t_Edit;
	m_text_browser = ui.t_Browser;
	setAttribute(Qt::WA_QuitOnClose, false);
	setAttribute(Qt::WA_DeleteOnClose, true);
	m_last_message_position = 0;
        if ( m_text_browser )
	    {
    		m_text_browser->setOpenExternalLinks(true);
	    }
	m_already_hred = false;

	QSoftMenuBar::setLabel(this, Qt::Key_Back, QSoftMenuBar::NoLabel);
	QSoftMenuBar::setLabel(m_text_browser, Qt::Key_Back, QSoftMenuBar::NoLabel);
	QSoftMenuBar::setLabel(conferenceTextEdit, Qt::Key_Back, QString::null, tr("Send"));
	QMenu *menu = QSoftMenuBar::menuFor(this);
	menu->clear();
	menu->addAction(tr("Clear Chat"), this, SLOT(on_clearButton_clicked()));
	menu->addSeparator();
	menu->addAction(tr("Close Chat"), this, SLOT(deleteLater()));

//	m_contact_list = new ConferenceContactList(protocol_name,conference_name,account_name,ui.conferenceList);
//	m_item_delegate = new ContactListItemDelegate();
//	m_item_delegate->setTreeView(ui.conferenceList);
//	ui.conferenceList->setItemDelegate(m_item_delegate);
//	ui.conferenceList->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
//	ui.conferenceList->setSelectionMode(QAbstractItemView::SingleSelection);
//	ui.conferenceList->setSelectionBehavior(QAbstractItemView::SelectItems);
//	m_cl_event_eater = new ConferenceContactListEventEater();
//        m_cl_event_eater->m_contact_list = m_contact_list;
//        ui.conferenceList->findChild<QObject *>("qt_scrollarea_viewport")->installEventFilter(m_cl_event_eater);
//        ui.conferenceList->installEventFilter(m_cl_event_eater);
//	m_contact_list->nowActive();

//	m_scroll_at_max = true;
//	m_current_scroll_position = 0;
/*	QList<int> sizes;
        sizes.append(400);
	sizes.append(20);
        ui.chatSplitter->setSizes(sizes);
	sizes.clear();
	sizes.append(400);
	sizes.append(100);
        ui.listSplitter->setSizes(sizes);*/
	m_last_history=true;
	ui.t_Edit->installEventFilter(this);
	m_tab_compl = new TabCompletion(this);
	m_tab_compl->setTextEdit(ui.t_Edit);
	m_tab_compl->setContactList(m_contact_list);
	qDebug() << "ui done...";
}

bool SeparateConference::eventFilter(QObject *obj, QEvent *event)
{
	if (obj == ui.t_Edit && event->type() == QEvent::KeyPress)
	{
		QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);		
		if ( keyEvent->key() == Qt::Key_Tab ) 
		{
			m_tab_compl->tryComplete();
			return true;
		}

		m_tab_compl->reset();
		return false;
	}
	return QObject::eventFilter( obj, event );
}

SeparateConference::~SeparateConference()
{
	m_abstract_chat_layer.removeSeparateConferenceFromList(
			QString("%1.%2.%3")
			.arg(m_protocol_name)
			.arg(m_conference_name)
			.arg(m_account_name));
	PluginSystem::instance().leaveConference(m_protocol_name, m_conference_name,
			m_account_name);
/*	m_abstract_chat_layer.saveConferenceSizeAndPosition(pos(), size(),
                        ui.chatSplitter->saveState(), ui.listSplitter->saveState());*/
	//delete m_item_delegate;
}

void SeparateConference::addMessage(const QString &from,
		const QString &message, const QString &date, bool history)
{
	if ( !history )
		checkForActive(message);
	if( m_text_browser )
	{	
		quint64 tmp_position = m_text_browser->textCursor().position();
		QString add_text;
		add_text.append(m_mine_nick != from
				?QString("<b><font color=#0000FF>"):QString("<b><font color=#FF0000>"));
		add_text.append(QString("[%1]").arg(date));
		
		if ( m_show_names )
		{
			add_text.append(QString(" %1").arg(Qt::escape(from)));
		}
		add_text.append(": </font></b>");
		add_text.append(QString("%1<br>").arg(checkForEmoticons(message)));
		m_current_scroll_position = m_text_browser->verticalScrollBar()->value();
		m_scroll_at_max = m_text_browser->verticalScrollBar()->maximum()
			== m_current_scroll_position; 
		m_text_browser->moveCursor(QTextCursor::End);
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
			}
			m_message_positions.append(m_text_browser->textCursor().position() - tmp_position);
		}
	}
	if(m_mine_nick != from && message.contains(m_mine_nick)){
		m_tab_compl->setLastReferrer(from);
	}
}

void SeparateConference::moveCursorToEnd()
{
	if( m_text_browser )
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

void SeparateConference::on_clearButton_clicked()
{
    if ( m_text_browser )
	{
		m_text_browser->clear();
		m_message_positions.clear();
	}
	focusTextEdit();
}
/*
void SeparateConference::on_onEnterButton_clicked()
{
	ui.conferenceTextEdit->setFocus();
}
*/
/*
void SeparateConference::on_quoteButton_clicked()
{
	QString selected_text;

	if ( m_text_browser )
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

void SeparateConference::on_translitButton_clicked()
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
*/
QString SeparateConference::invertMessage(QString &text)
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

void SeparateConference::focusTextEdit()
{
    	ui.t_Edit->setFocus();
}
/*
void SeparateConference::setEmoticonsToMenu()
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
*/
/*
void SeparateConference::setEmoticonPath(const QString &path)
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
*/
/*
void SeparateConference::insertEmoticon(const QString &emoticon_text)
{
		ui.conferenceTextEdit->insertPlainText(" " + emoticon_text + " ");
}
*/
/*
void SeparateConference::setOptions(bool remove_after, quint16 remove_count,
		bool close_after, bool show_names, bool on_enter)
{
	m_remove_message_after = remove_after;
	m_remove_count = remove_count;
	m_close_after_send = close_after;
	m_show_names = show_names;
//	ui.onEnterButton->setChecked(on_enter);
}
*/
/*
void SeparateConference::setConferenceTopic(const QString &topic)
{
	ui.topicLineEdit->setText(topic);
}
*/	
QString SeparateConference::checkForEmoticons(const QString &message)
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

/*void SeparateConference::scrollWebViewToEnd()
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

void SeparateConference::newsOnLinkClicked(const QUrl &url)
{
	QDesktopServices::openUrl(url);
}

void SeparateConference::addServiceMessage(const QString &message, const QString &date)
{
    if ( m_text_browser )
	{
		m_current_scroll_position = m_text_browser->verticalScrollBar()->value();
		m_scroll_at_max = m_text_browser->verticalScrollBar()->maximum()
			== m_current_scroll_position; 
		QString modified_message;
		modified_message= AbstractChatLayer::instance().findUrls(Qt::escape(message)).replace("\n", "<br>");
		m_text_browser->moveCursor(QTextCursor::End);
		m_text_browser->insertHtml(QString("<font size='-1' color=#808080>[%1] %2</font><br>")
				.arg(date)
				.arg(modified_message));
		moveCursorToEnd();
	}
	moveCursorToEnd();
}

void SeparateConference::windowFocused()
{
	ui.t_Edit->setFocus();
	setWindowTitle(m_conference_name);
	m_already_hred = false;
}

void SeparateConference::checkForActive(const QString &message)
{
	if ( (!isActiveWindow() || isMinimized()) && !m_already_hred )
	{
	if ( m_text_browser )
		{
                        m_text_browser->append("<hr><br>");
			moveCursorToEnd();
		}
		setWindowTitle("*" + m_conference_name);
		m_already_hred = true;
	}
	if ( message.contains(m_mine_nick))
	{
		qApp->alert(this, 0);
	}
}
void SeparateConference::addConferenceItem(const QString &nickname)
{
	m_contact_list->addConferenceItem(nickname);
}

void SeparateConference::removeConferenceItem(const QString &nickname)
{
	m_contact_list->removeConferenceItem(nickname);
}

void SeparateConference::renameConferenceItem(const QString &nickname, const QString &new_nickname)
{
	if(m_mine_nick==nickname)
		m_mine_nick = new_nickname;
	m_contact_list->renameConferenceItem(nickname, new_nickname);
}

void SeparateConference::setConferenceItemStatus(const QString &nickname, const QIcon &icon, const QString &status, int mass)
{
	m_contact_list->setConferenceItemStatus(nickname,icon,status,mass);
}

void SeparateConference::setConferenceItemIcon(const QString &nickname,	const QIcon &icon, int position)
{
	m_contact_list->setConferenceItemIcon(nickname, icon, position);
}

void SeparateConference::setConferenceItemRole(const QString &nickname, const QIcon &icon, const QString &role, int mass)
{
	m_contact_list->setConferenceItemRole(nickname,icon,role,mass);
}

/*void SeparateConference::restoreSplitters(const QByteArray &state, const QByteArray &state2)
{
	ui.chatSplitter->restoreState(state);
	ui.listSplitter->restoreState(state2);
}*/
/*
void SeparateConference::on_topicButton_clicked()
{
	AbstractChatLayer::instance().showConferenceTopicSettings(m_protocol_name, m_account_name, m_conference_name);
}
*/
/*
void SeparateConference::on_configButton_clicked()
{
	PluginSystem::instance().showConferenceMenu(m_protocol_name, m_account_name, m_conference_name,
			mapToGlobal(ui.configButton->pos()));
}
*/
void SeparateConference::showEvent(QShowEvent * event)
{
	//windowActivatedByUser();
	windowFocused();
}

void SeparateConference::keyPressEvent(QKeyEvent *ev)
{
  if (ev->key()==Qt::Key_Back)
    return;
  QWidget::keyPressEvent(ev);
}
