/*
    ContactChatUserSpace
    ChatEventEater
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

#ifndef TABBEDCHATWINDOW_H
#define TABBEDCHATWINDOW_H

#include <QtGui/QWidget>
#include <QTabBar>
#include <QMouseEvent>
#include <QList>
#include <QPushButton>
#include <QTextEdit>
#include <QTextBrowser>
//#include <QWebView>
//#include <QWebFrame>
#include "../abstractchatlayer.h"
#include "chatwindowstyleoutput.h"
#include "chatemoticonmenu.h"
#include "../pluginsystem.h"
#include "ui_chatwindowform.h"
#include "separatechatwindow.h"
#include <QPixmap>

class GeneralTabBar : public QTabBar
{
	Q_OBJECT
public:
	GeneralTabBar (QWidget *parent = 0);
	~GeneralTabBar ();
protected:
	void mouseDoubleClickEvent ( QMouseEvent * event );
signals:
	void closeTab(int);
};

struct UnreadMessage
{
	QString m_message;
	bool m_same;
};

struct ContactChatUserSpace
{
	QString m_protocol_name;
	QString m_account_name;
	QString m_contact_name;
	QString m_chat_history;
	QString m_contact_nick;
	QString m_account_nick;
	QString m_contact_avatar;
	QString m_account_avatar;
	QString m_client_id;
	QString m_additional_data;
	bool m_last_message_sender;
	int m_last_message_icon_position;
	int m_last_message_position;
    QList<int> m_message_positions;
    bool m_lock_for_first_message;
    bool m_waiting_for_activation;
    bool m_typing;
    bool m_unreaded_message;
    QPixmap m_client_pixmap;
    QIcon m_status_icon;
    QString m_send_text;
    QList<UnreadMessage> m_unreaded_messages;
    bool m_last_history;
};

class TabbedChatWindow : public QWidget
{
    Q_OBJECT

public:
    TabbedChatWindow(/*bool webkit_mode, */
    		const QString &emoticon_path,
                /*const QString &chat_style,*/
    		const QString &webkit_style,
    		const QString &webkit_variant,
    		QWidget *parent = 0);
    ~TabbedChatWindow();
    static bool lengthLessThan (const QString &s1, const QString &s2);
    void createTab(const QString &protocol_name,
    		const QString &account_name,
    		const QString &item_name,
    		const QString &identification,
    		const QIcon &icon,
    		bool set_as_current = true);
    void addMessage(const QString &identification, const QString &message, bool in, 
    		const QString &message_date, bool unreaded_message, bool history);
    void addServiceMessage(const QString &idenitifcation, const QString &message);
    void messageDelievered(const QString &identification, int position);
    bool contactTabOpened(const QString &identification)
    {
    	return m_tab_chat_index_list.contains(identification);
    }
    void setOptions(bool remove_after, quint16 remove_count,
    		bool close_after, bool show_names, bool on_enter,
    		bool typing_notifications);
    void contactTyping(const QString &identification, bool typing);
	QStringList getTabList();
	void setContactStatusIcon(const QString &identification, const QIcon &icon);
//	void restoreSplitter(const QByteArray &state);
	void setContactClientIcon(const QString &identification);
	void addSeparator(const QString &idfentification);
	int getCursorPosition(const QString &identification);
        bool changeTabID(const QString &old_id, const QString &new_id, const QString &id);
protected:
	void showEvent(QShowEvent * event); 
	void keyPressEvent ( QKeyEvent *event );
private slots:
	void closeTabAt(int index);
	void on_sendButton_clicked();
	void currentTabChanged(int index);
	void insertEmoticon(const QString &emoticon_text);
	void on_chatInputEdit_textChanged();
	void windowFocused();
	void on_infoButton_clicked();
//	void on_sendPictureButton_clicked();
	void on_sendFileButton_clicked();
	void on_onEnterButton_clicked();
	void on_typingButton_clicked();
	void typingNow();
	void on_translitButton_clicked();
	void on_historyButton_clicked();
	void on_clearChatButton_clicked();
	void on_quoteButton_clicked();
	void closeCurrentTab();
//	void scrollWebViewToEnd();
	void onCustomContextMenuRequested(const QPoint & pos);
	void onCustomTabContextMenuRequested(const QPoint & pos);
	void newsOnLinkClicked(const QUrl &url);
private:
//	void appendMessage(QString message, bool same_from);
	void setIconsToButtons();
	QString invertMessage(QString &message);
    void windowActivatedByUser();
	void checkForActiveState(ContactChatUserSpace *user_space, 
			const QString &message, bool unread_content, bool in);
	void moveCursorToEnd();
	void setDefaultForm();
	void installEventEater();
	void setEmoticonsToMenu();
	void setEmoticonPath(const QString &path);
	void focusTextEdit();
	void readContactInfo(ContactChatUserSpace *user_space);
	void readOwnerInfo(ContactChatUserSpace *user_space);
	QString checkForEmoticons(const QString &message);
	void setMessageTabIcon(int index);
	Ui::Form *ui;
	PluginSystem &m_plugin_system;
	ChatEventEater *m_event_eater;
	GeneralTabBar  *m_chat_tab_bar;
	AbstractChatLayer &m_abstract_chat_layer;
	QList<QString> m_tab_chat_index_list;
	QList<ContactChatUserSpace *> m_user_spaces_chat_list;
	QPushButton *m_send_button;
	QTextEdit *m_plain_text_edit;
	QTextBrowser *m_text_browser;
//	QWebView *m_web_view;
	ChatWindowStyleOutput *m_style_output;
	QLabel *m_contact_avatar_label;
	QLabel *m_name_label;
	QLabel *m_id_label;
	QLabel *m_cliend_id_label;
	QLabel *m_owner_avatar_label;
	QLabel *m_additional_label;
	QLabel *m_typing_label;
	QLabel *m_client_label;
	QToolButton *m_info_button;
	QToolButton *m_emoticons_button;
	QToolButton *m_history_button;
	QToolButton *m_font_button;
	QToolButton *m_color_button;
	QToolButton *m_send_picture_button;
	QToolButton *m_send_file_button;
	QToolButton *m_on_enter_button;
	QToolButton *m_send_typing_button;
	QToolButton *m_close_tab_button;
	QToolButton *m_translate_button;
	QToolButton *m_quote_button;
	QToolButton *m_clear_button;
	QHBoxLayout *m_tab_layout;
//	QSplitter *m_main_splitter;
	QString m_header_html;
//	bool m_webkit_mode;
	QString m_emoticons_path;
	QString m_chat_form_path;
	QString m_webkit_style_path;
	QString m_webkit_variant;
    QMenu *m_emotic_menu;
    QWidgetAction *m_emoticon_action;
    ChatEmoticonMenu *m_emoticon_widget;
    QHash<QString, QString> m_emoticon_list;
    QHash<QString, QString> m_urls;
    QStringList m_emoticon_keys;
    QAction *m_change_font_action;
    QAction *m_change_font_color_action;
    QMenu *m_font_menu;
    QFont m_text_font;
    QColor m_text_color;
    QColor m_background_color;
    bool m_send_typing_notifications;
    bool m_remove_message_after;
    quint16 m_remove_count;
    bool m_close_after_send;
    bool m_show_names;
    bool m_text_changed;
    bool m_typing_changed;
    int m_previous_tab;
    bool m_scroll_at_max;
    quint64 m_current_scroll_position;
};

#endif // TABBEDCHATWINDOW_H
