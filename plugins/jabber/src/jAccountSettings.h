#ifndef JACCOUNTSETTINGS_H
#define JACCOUNTSETTINGS_H

#include <QtGui/QWidget>
#include <QSettings>
#include "ui_jAccountSettings.h"
#include "jAccount.h"

class jAccountSettings : public QDialog
{
    Q_OBJECT

public:
    jAccountSettings(const QString &profile_name, const QString &account_name, jAccount *account, QWidget *parent = 0);
    ~jAccountSettings();
    
signals:
    void settingsSaved();
        
  protected:
    void keyPressEvent(QKeyEvent *ev);

private:
    void loadSettings();
    void saveSettings();
    Ui::jAccountSettingsClass ui;
    QString m_profile_name;
    QString m_account_name;
    jAccount *m_account;
};

#endif // JACCOUNTSETTINGS_H
