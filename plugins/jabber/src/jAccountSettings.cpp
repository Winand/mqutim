#include "jAccountSettings.h"
#include <QSoftMenuBar>

jAccountSettings::jAccountSettings(const QString &profile_name, const QString &account_name, jAccount *account, QWidget *parent)
    : QDialog(parent)
{
	qDebug("setupUi");
	ui.setupUi(this);
  m_account = account;
  m_profile_name = profile_name;
  m_account_name = account_name;
  loadSettings();
  setAttribute(Qt::WA_QuitOnClose, false);
  setAttribute(Qt::WA_DeleteOnClose, true);
  QSoftMenuBar::setLabel(this, Qt::Key_Back, QSoftMenuBar::Ok);
//	m_priority = settings.value("priority", 5).toInt();
//	QString password = settings.value("password","").toString();
//	m_my_nick = settings.value("nickname",m_account_name).toString();
}

jAccountSettings::~jAccountSettings()
{

}

void jAccountSettings::loadSettings()
{
    QSettings settings(QSettings::NativeFormat, QSettings::UserScope, "qutim/qutim."+m_profile_name+"/jabber."+m_account_name, "accountsettings");
    settings.beginGroup("main");
    ui.paswdEdit->setText(settings.value("password","").toString());
    ui.jidEdit->setText(m_account_name);
    ui.autologinCheck->setChecked(settings.value("autoconnect",false).toBool());
    ui.resourceEdit->setText(settings.value("resource","qutIM").toString());
    ui.prioritySpinBox->setValue(settings.value("priority",5).toInt());
    ui.tlsComboBox->setCurrentIndex(settings.value("tlspolicy",1).toInt());
    ui.compressCheckBox->setChecked(settings.value("compress",true).toBool());
//    ui.manualCheckBox->setChecked(!settings.value("usedns",true).toBool());
//    ui.hostEdit->setEnabled(ui.manualCheckBox->isChecked());
//    ui.portSpinBox->setEnabled(ui.manualCheckBox->isChecked());
    QString server = m_account_name.section("@",1);
    if(server=="qutim.org")
        server = "jabber.qutim.org";
    if(server=="udaff.com")
        server = "jabber.udaff.com";
    qDebug() << "Using " << server << "server";
//    ui.hostEdit->setText(settings.value("server",server).toString());
//    ui.portSpinBox->setValue(settings.value("port",5222).toInt());
    settings.endGroup();
//    settings.beginGroup("proxy");
//    ui.proxyTypeComboBox->setCurrentIndex(settings.value("type",3).toInt());
//    ui.proxyHostEdit->setText(settings.value("server","").toString());
//    ui.proxyPortSpinBox->setValue(settings.value("port",3128).toInt());
//    settings.endGroup();
}

void jAccountSettings::saveSettings()
{
    QSettings settings(QSettings::NativeFormat, QSettings::UserScope, "qutim/qutim."+m_profile_name+"/jabber."+m_account_name, "accountsettings");
    settings.beginGroup("main");
    settings.setValue("password", ui.paswdEdit->text());
    settings.setValue("autoconnect", ui.autologinCheck->isChecked());
    settings.setValue("resource", ui.resourceEdit->text());
    settings.setValue("priority", ui.prioritySpinBox->value());
    settings.setValue("tlspolicy", ui.tlsComboBox->currentIndex());
    settings.setValue("compress", ui.compressCheckBox->isChecked());
//    settings.setValue("usedns", !ui.manualCheckBox->isChecked());
//    settings.setValue("server", server);
//    settings.setValue("port", ui.portSpinBox->value());
    settings.endGroup();
    emit settingsSaved();
    m_account->getProtocol()->loadSettings();
}

void jAccountSettings::keyPressEvent(QKeyEvent *ev)
{
  if (ev->key()==Qt::Key_Back)
  {
    saveSettings();
    close();
    return;
  }
  QWidget::keyPressEvent(ev);
}

