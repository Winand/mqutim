/*
    HistoryWindow

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

#include "historywindow.h"
#include <QPoint>
#include <QDesktopWidget>
#include <QFileInfo>
#include <QDir>
#include "../abstracthistorylayer.h"
#include "../iconmanager.h"
#include "../pluginsystem.h"
#include <QScrollBar>
#include <QHeaderView>

HistoryWindow::HistoryWindow(const QString &protocol_name,
                             const QString &account_name,
                             const QString &item_name,
                             const QString &profile_name,
                             QWidget *parent)
    : QWidget(parent),
    m_protocol_name(protocol_name),
    m_account_name(account_name),
    m_profile_name(profile_name)
{
  TreeModelItem item;
  item.m_account_name = account_name;
  item.m_protocol_name = protocol_name;
  item.m_item_name = item_name;
  m_item_name = AbstractHistoryLayer::instance().getContactHistoryName(item);
  ui.setupUi(this);
  ui.dateTreeWidget->header()->hide();
  setWindowState(Qt::WindowMaximized);

  ui.historyLog->setHtml("<p align='center'><span style='font-size:36pt;'>"
                         + tr("No History") + "</span></p>");

  codec = QTextCodec::codecForName("UTF-8");
  setAttribute(Qt::WA_QuitOnClose, false);
  setAttribute(Qt::WA_DeleteOnClose, true);
/*  QList<int> sizes;
  sizes.append(80);
  sizes.append(250);
  	ui.splitter->setSizes(sizes);
          ui.splitter->setCollapsible(1,false);*/
  QSettings settings(QSettings::NativeFormat, QSettings::UserScope, "qutim/qutim."+profile_name, "profilesettings");
  m_history_path = QFileInfo(settings.fileName()).absolutePath()+"/history/";
  setIcons();
  fillAccountComboBox();

  connect(ui.accountComboBox, SIGNAL(currentIndexChanged(int)),
          this, SLOT(fillContactComboBox(int)));

  int account_index = ui.accountComboBox->findData(
                        protocol_name + "." + toHex(account_name));
  if ( !account_index )
    fillContactComboBox(0);
  else
    ui.accountComboBox->setCurrentIndex(account_index);

  connect(ui.fromComboBox, SIGNAL(currentIndexChanged(int)),
          this, SLOT(fillDateTreeWidget(int)));

  int from_index = ui.fromComboBox->findData(m_item_name);
  if ( !from_index )
    fillDateTreeWidget(0);
  else
    ui.fromComboBox->setCurrentIndex(from_index);
}

HistoryWindow::~HistoryWindow()
{
  AbstractHistoryLayer::instance().closeHistoyWindow(QString("%1.%2.%3").arg(m_protocol_name)
      .arg(m_account_name).arg(m_item_name));
}

void HistoryWindow::setIcons()
{
  IconManager &im = IconManager::instance();
  setWindowIcon(im.getIcon("history"));
  ui.searchButton->setIcon(im.getIcon("search"));
}

void HistoryWindow::fillAccountComboBox()
{
  QDir history_dir(m_history_path);
  QStringList accounts = history_dir.entryList(QDir::AllDirs |
                         QDir::NoDotAndDotDot);
  PluginSystem &ps = PluginSystem::instance();
  PluginInfoList protocols = ps.getPluginsByType("protocol");
  foreach(const QString &account, accounts)
  {
    QString protocol = account.section(".",0,0);
    QString account_name = fromHex(account.section(".",1,1));
    QIcon protocol_icon;
    foreach( PluginInfo plugin_info, protocols )
    {
      if ( plugin_info.name == protocol )
        protocol_icon = plugin_info.icon;
    }
    TreeModelItem item;
    item.m_protocol_name = protocol;
    item.m_account_name = account_name;
    item.m_item_name = account_name;
    item.m_item_type = TreeModelItem::Buddy;
    QStringList account_info = ps.getAdditionalInfoAboutContact(item);
    QString account_nickname = account_name;
    if ( account_info.count() > 0 )
    {
      account_nickname = account_info.at(0);
    }
    ui.accountComboBox->addItem(protocol_icon,
                                account_nickname, account);
  }
}

void HistoryWindow::fillContactComboBox(int index)
{
  if ( ui.accountComboBox->count() > 0 )
  {
    QString item_data = ui.accountComboBox->itemData(index).toString();
    QString protocol = item_data.section(".",0,0);
    QString account_name = fromHex(item_data.section(".",1,-1));
    QDir account_history_dir(m_history_path + "/" + item_data);
    QStringList log_list = account_history_dir.entryList(QDir::Files | QDir::NoDotAndDotDot);
    ui.fromComboBox->clear();
    foreach(const QString &contact, log_list)
    {
      QString contact_name = fromHex(contact.section(".",0,0));
      if ( ui.fromComboBox->findData(contact_name) != -1 )
        continue;
      TreeModelItem item;
      item.m_protocol_name = protocol;
      item.m_account_name = account_name;
      item.m_item_name = contact_name;
      item.m_item_type = TreeModelItem::Buddy;
      QStringList contact_info =
        PluginSystem::instance().getAdditionalInfoAboutContact(item);
      QString contact_nickname = contact_name;
      if ( contact_info.count() > 0 )
      {
        contact_nickname = contact_info.at(0);
      }
      ui.fromComboBox->addItem(contact_nickname,
                               contact_name);
    }
  }
}

void HistoryWindow::fillDateTreeWidget(int index, const QString &search_word)
{
  if ( ui.fromComboBox->count() > 0 )
  {
    int account_index = ui.accountComboBox->currentIndex();
    if ( account_index < 0 )
      return;
    QString item_data = ui.accountComboBox->itemData(account_index).toString();
    QDir account_history_dir(m_history_path + "/" + item_data);
    QStringList filters;
    QString from_name = ui.fromComboBox->itemData(index).toString();
    filters<<toHex(from_name) + ".*";
    QStringList from_files = account_history_dir.entryList(filters,
                             QDir::Files | QDir::NoDotAndDotDot);
    int year = 0, month = 0, day = 0;
    ui.dateTreeWidget->clear();
    IconManager &im = IconManager::instance();
    QTreeWidgetItem *last_item = 0;
    foreach(const QString &history_file, from_files)
    {
      QFile file(account_history_dir.absoluteFilePath(history_file));
      if (file.open(QIODevice::ReadOnly))
      {
        QList<QDateTime> days_list;
        QDataStream in(&file);
        while (!file.atEnd())
        {
          QDateTime history_date_time;
          qint8 history_type;
          bool history_in;
          QString history_message;
          in >> history_date_time >>
          history_type >>
          history_in >>
          history_message;
          if ( ( search_word.isEmpty()
                 && !days_list.contains(history_date_time) )
               ||
               ( !search_word.isEmpty()
                 && !days_list.contains(history_date_time)
                 && ( history_message.indexOf(search_word, Qt::CaseInsensitive) != -1 ) ))
          {
            days_list.append(history_date_time);
          }
        }
        if ( days_list.count() )
        {
          int tmp_year = days_list.at(0).date().year();
          QTreeWidgetItem *year_item;
          if ( tmp_year != year )
          {
            year = tmp_year;
            month = 0;
            year_item = new QTreeWidgetItem(ui.dateTreeWidget);
            year_item->setText(0, QString::number(year));
            year_item->setIcon(0, im.getIcon("year"));
          }
          else
          {
            QList<QTreeWidgetItem *> items_list =
              ui.dateTreeWidget->findItems(QString::number(year), Qt::MatchExactly);
            if ( items_list.count() )
              year_item = items_list.at(0);
          }
          QTreeWidgetItem *month_item = new QTreeWidgetItem(year_item);
          month = days_list.at(0).date().month();
          month_item->setText(0, QDate::longMonthName(month));
          if ( month <= 2 || month == 12 )
            month_item->setIcon(0, im.getIcon("winter"));
          else if ( month >= 3 && month <=5 )
            month_item->setIcon(0, im.getIcon("spring"));
          else if ( month >= 6 && month <=8 )
            month_item->setIcon(0, im.getIcon("summer"));
          else if ( month >= 9 && month <=11 )
            month_item->setIcon(0, im.getIcon("autumn"));
          foreach(QDateTime history_date, days_list)
          {
            int tmp_day = history_date.date().day();
            if ( tmp_day != day )
            {
              day = tmp_day;
              QTreeWidgetItem *day_item = new QTreeWidgetItem(month_item);
              day_item->setText(0, QString::number(day) +
                                history_date.time().toString("(hh:mm)"));
              day_item->setIcon(0, im.getIcon("day"));
              day_item->setData(0, Qt::UserRole, history_date);
              last_item = day_item;
            }
          }

        }
      }
    }
    if ( last_item )
      ui.dateTreeWidget->setCurrentItem(last_item);
    ui.dateTreeWidget->expandAll();
    setWindowTitle(QString("%1(%2)").arg(ui.fromComboBox->currentText())
                   .arg(ui.accountComboBox->currentText()));
  }
}

void HistoryWindow::on_dateTreeWidget_currentItemChanged( QTreeWidgetItem* current, QTreeWidgetItem* previous )
{
  if ( current )
  {
    QDateTime item_date_time = current->data(0, Qt::UserRole).toDateTime();
    if ( !item_date_time.isNull() )
    {
      QString search_word = ui.searchEdit->text();
      int account_index = ui.accountComboBox->currentIndex();
      int from_index = ui.fromComboBox->currentIndex();
      if ( account_index < 0 || from_index < 0)
        return;
      QDir account_history_dir(m_history_path + "/" +
                               ui.accountComboBox->itemData(account_index).toString());
      QFile file(account_history_dir.absoluteFilePath(
                   toHex(ui.fromComboBox->itemData(from_index).toString()) +
                   "." + item_date_time.date().toString("yyyyMM") + ".log"));
      if (file.open(QIODevice::ReadOnly))
      {
        int day = item_date_time.date().day();
        QDataStream in(&file);
        QString account_nickname = ui.accountComboBox->currentText();
        QString from_nickname = ui.fromComboBox->currentText();
        QString history_html;
        while (!file.atEnd())
        {
          QDateTime history_date_time;
          qint8 history_type;
          bool history_in;
          QString history_message;
          in >> history_date_time >>
          history_type >>
          history_in >>
          history_message;
          if ( history_date_time.date().day() == day )
          {
            QString history_html_2="";
            history_html_2.append(
              history_in?QString("<b><font color=#0000FF>")
              :QString("<b><font color=#FF0000>"));
            history_html_2.append(history_in?from_nickname:account_nickname);
            history_html_2.append(QString(" ( %1)</font></b><br>").arg(
                                    history_date_time.time().toString()));
            if ( search_word.isEmpty() )
            {
              history_html_2.append(QString("%1<br>").arg(history_message));
            }
            else
            {
              history_html_2.append("<font style=background-color:white>" /*#FFFFFF=white*/
                                    +history_message.replace(search_word,
                                                             "<font style=background-color:#A0A000>" /*#FFFF00=yellow*/
                                                             +search_word+"</font>", Qt::CaseInsensitive)+"</font><br>");
            }
            history_html.append(history_html_2);
          }
        }
        ui.historyLog->setHtml(history_html);
        ui.historyLog->moveCursor(QTextCursor::End);
        ui.historyLog->ensureCursorVisible();
        ui.historyLog->setLineWrapColumnOrWidth(ui.historyLog->lineWrapColumnOrWidth());
        ui.historyLog->verticalScrollBar()->setValue( ui.historyLog->verticalScrollBar()->maximum() );
      }
    }
  }
}

void HistoryWindow::on_searchButton_clicked()
{
  if ( ui.accountComboBox->count() && ui.fromComboBox->count() )
  {
    fillDateTreeWidget(ui.fromComboBox->currentIndex(), ui.searchEdit->text());
  }
}

void HistoryWindow::on_cmdsh_clicked()
{
  if(ui.dateTreeWidget->isVisible()) ui.dateTreeWidget->hide();
  else ui.dateTreeWidget->show();
}
