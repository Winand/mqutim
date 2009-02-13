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

#ifndef SEPARATECHATWINDOW_H
#define SEPARATECHATWINDOW_H

#include <QtGui/QWidget>
#include <QPushButton>
#include <QToolButton>
#include <QTextEdit>
#include <QTextBrowser>
//#include <QWebView>
//#include <QWebFrame>
#include <QLabel>
#include <QMenu>
#include <QWidgetAction>
#include <QSplitter>
#include "../abstractchatlayer.h"
#include "chatwindowstyleoutput.h"
#include "../pluginsystem.h"
#include "chatemoticonmenu.h"
#include <QTextCursor>
#include "ui_chatwindowform.h"

class ChatEventEater : public QObject
{
	Q_OBJECT

public:
	ChatEventEater(QWidget *parent = 0) : QObject(parent){ }
	bool m_send_on_enter;
	
signals:
	void sendMessage();
	void insertNewLine();
	void focusedIn();
	void close();
protected:
	bool eventFilter(QObject *obj, QEvent *event);

};

class SeparateChatWindow : public QStackedWidget
{
    Q_OBJECT

public:
    SeparateChatWindow(const QString &protocol_name,
    		const QString &account_name,
    		const QString &item_name,
                /*bool webkit_mode,*/
    		const QString &emoticon_path,
/*    		const QString &chat_style,*/
    		const QString &webkit_style,
    		const QString &webkit_variant,
    		QWidget *parent = 0);
    ~SeparateChatWindow();
    static bool lengthLessThan (const QString &s1, const QString &s2);
    void addMessage(const QString &message, bool in
    		, const QString &message_date, bool history);
    void addServiceMessage(const QString &message);
    void setOptions(bool remove_after, quint16 remove_count,
    		bool close_after, bool show_names, bool on_enter,
    		bool typing_notifications);
    void contactTyping(bool typing);
    void messageDelievered(int position);
    void windowActivatedByUser();
    QString getProtocolName()
    {
    	return m_protocol_name;
    }
    QString getAccountName()
    {
    	return m_account_name;
    }
    QString getItemName()
    {
    	return m_item_name;
    }
//	void restoreSplitter(const QByteArray &state);
	void setContactClientIcon();
	void addSeparator();
	int getCursorPosition();
        void setID(const QString &id);

protected:
	bool event(QEvent *event);  

private slots:
	void on_sendButton_clicked();
	void on_infoButton_clicked();
	void insertEmoticon(const QString &emoticon_text);
/*	void on_sendPictureButton_clicked();
	void on_sendFileButton_clicked();
        void on_onEnterButton_clicked();*/
	void on_typingButton_clicked();
	void on_chatInputEdit_textChanged();
/*	void on_translitButton_clicked();*/
	void on_historyButton_clicked();
	void typingNow();
	void on_clearChatButton_clicked();
	void on_quoteButton_clicked();
	void windowFocused();
//	void scrollWebViewToEnd();
	void onCustomContextMenuRequested(const QPoint & pos);
	void newsOnLinkClicked(const QUrl &url);

private:
	Ui::Form *ui;
	void setIconsToButtons();
	QString invertMessage(QString &message);
	void readContactInfo();
	void readOwnerInfo();
	void setEmoticonsToMenu();
	void setEmoticonPath(const QString &path);
	void installEventEater();
	QString changeEmoticonsPathsToText(const QString &message);
	QString checkForEmoticons(const QString &message);
	void createFontMenu();
	void moveCursorToEnd();
	void setDefaultForm();
	void focusTextEdit();
	AbstractChatLayer &m_abstract_chat_layer;
	QString m_protocol_name;
	QString m_account_name;
	QString m_item_name;
	QString m_client_id;
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
	QToolButton *m_translate_button;
	QToolButton *m_quote_button;
	QToolButton *m_clear_button;
//	QSplitter *m_main_splitter;
	QString m_own_name;
	QString m_contact_name;
	QString m_own_avatar_path;
	QString m_contact_avatar_path;
//	bool m_webkit_mode;
	bool m_last_message_sender;
	bool m_lock_for_first_message;
	PluginSystem &m_plugin_system;
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
    QString m_now_html;
    bool m_remove_message_after;
    quint16 m_remove_count;
    QList<quint64> m_message_positions;
    quint32 m_webkit_header_offset;
    bool m_close_after_send;
    bool m_show_names;
    ChatEventEater *m_event_eater;
    bool m_send_typing_notifications;
    bool m_text_changed;
    bool m_typing_changed;
    QHash<int,int> m_message_position_offset;
    int m_last_message_icon_position;
    int m_last_message_position;
    void checkForActiveState();
    bool m_waiting_for_activation;
	QString m_emoticons_path;
	QString m_chat_form_path;
	QString m_webkit_style_path;
	QString m_webkit_variant;
    bool m_scroll_at_max;
    quint64 m_current_scroll_position;
    bool m_last_history;
};

#endif // SEPARATECHATWINDOW_H
