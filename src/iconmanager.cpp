/*
    iconManager

    Copyright (c) 2008 by Alexey Pereslavtsev <steel1.0@mail.ru>
    						Nigmatullin Ruslan <euroelessar@gmail.com>
    						Dmitri Arekhta <DaemonES@gmail.com>

 This file is a part of qutIM project.
 You can find more information at http://qutim.org


 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************
*/

#include "iconmanager.h"


IconManager::IconManager()
//: m_path_to_system_icons(":/icons/crystal_project"),//it tested for absolute path.
//m_path_to_status_icons(":/icons/crystal_project")//it tested for absolute path.
{
  m_default_path_to_system_icons = ":/icons/crystal_project";
  m_path_to_system_icons = m_default_path_to_system_icons;
  m_default_path_to_status_icons = ":/icons/";
  m_default_path_to_icons = ":/icons";
  m_path_to_status_icons = m_default_path_to_status_icons;
  m_from_adium.insert("Generic Available","online");
  m_from_adium.insert("Free for Chat","ffc");
  m_from_adium.insert("Generic Away","away");
  m_from_adium.insert("Not Available","na");
  m_from_adium.insert("Occupied","occupied");
  m_from_adium.insert("DND","dnd");
  m_from_adium.insert("Invisible","invisible");
  m_from_adium.insert("Offline","offline");
  m_from_adium.insert("Unknown","connecting");
  m_from_adium.insert("Available At Home","athome");
  m_from_adium.insert("Available At Work","atwork");
  m_from_adium.insert("Lunch","lunch");
  m_from_adium.insert("Available Evil","evil");
  m_from_adium.insert("Available Depression","depression");
  m_from_adium.insert("Without Authorization","noauth");
}
IconManager &IconManager::instance()
{
  static IconManager ims;
  return ims;
}

void IconManager::loadSettings()
{
  QSettings settings(QSettings::NativeFormat, QSettings::UserScope, "qutim/qutim."+m_profile_name, "profilesettings");
  settings.beginGroup("gui");
  m_path_to_system_icons = settings.value("systemicon","").toString();
  if (m_path_to_system_icons.isEmpty())
    m_path_to_system_icons = ":/icons/crystal_project";
  m_path_to_status_icons = settings.value("statusicon","").toString();
  if (m_path_to_status_icons.isEmpty())
    m_path_to_status_icons = ":/icons";
  else
    setStatusIconPackage(m_path_to_status_icons);
  settings.endGroup();
}

void IconManager::loadProfile(const QString & profile_name)
{
  m_profile_name = profile_name;
  loadSettings();
}

QString IconManager::getIconFileName(const QString & icon_name)
{
  if (m_system_icons_name.contains(icon_name))
    return m_system_icons_name.value(icon_name);
  QString path = m_path_to_system_icons + "/" + icon_name;
  QIcon icon(path);
  if (icon.actualSize(QSize(16,16)).width()<0 || icon.isNull())
  {
    path = m_default_path_to_system_icons+"/"+icon_name;
    icon = QIcon(path);
    if (icon.actualSize(QSize(16,16)).width()<0 || icon.isNull())
    {
      m_system_icons_name.insert(icon_name,m_default_path_to_icons+"/"+icon_name);
      return m_default_path_to_icons+"/"+icon_name;
    }
    m_system_icons_name.insert(icon_name,path);
    return path;
  }
  m_system_icons_name.insert(icon_name,path);
  return path;
}

QIcon IconManager::getIcon(const QString & icon_name)
{
  return QIcon(getIconFileName(icon_name));
}

QString IconManager::getStatusIconFileName(const QString & icon_name, const QString & default_path)
{
  if (m_path_to_status_icons.startsWith(":/icons"))
    return  getIconFileName(default_path+"/"+icon_name);
  else
    return m_path_to_status_icons + "/" + m_status_icons_name[icon_name];
}

QIcon IconManager::getStatusIcon(const QString & icon_name, const QString & default_path)
{
  if (default_path.isEmpty())
    return QIcon(getStatusIconFileName(icon_name,m_path_to_status_icons));
  else
    return QIcon(getStatusIconFileName(icon_name,default_path));
}
bool IconManager::setStatusIconPackage(const QString & path)
{
  if (!QDir(path).exists())
    return false;
  if (path.endsWith(".AdiumStatusIcons",Qt::CaseInsensitive))
  {
    do
    {
#if defined(Q_OS_WIN)
      QFile xmlFile(path + "\\Icons.plist");
#else
      QFile xmlFile(path + "/Icons.plist");
#endif
      if (!xmlFile.exists())
      {
        break;
      }
      if (xmlFile.open(QIODevice::ReadOnly))
      {
        QDomDocument adiumIconsPref;

        if (adiumIconsPref.setContent(&xmlFile))
        {
          QDomElement rootElem = adiumIconsPref.documentElement();
          if (rootElem.isNull())
          {
            break;
          }

          for (QDomNode node = rootElem.firstChild(); !node.isNull(); node = node.nextSibling())
          {
            for (QDomNode dictNode = node.firstChild(); !dictNode.isNull(); dictNode = dictNode.nextSibling())
            {
              QDomElement dictElem = dictNode.toElement();
              if (!dictElem.isNull())
              {
                //QString tagName = dictElem.tagName();
                QString attrName = dictElem.text();
                if (attrName == "List" || attrName == "Tabs")
                {
                  dictNode = dictNode.nextSibling();
                  QDomElement childDict = dictNode.toElement();
                  if (!dictNode.isNull())
                  {
                    QDomElement childDict = dictNode.toElement();
                    if (!childDict.isNull() && childDict.tagName() == "dict")
                    {
                      QString iconType;
                      //	QString iconPath;

                      for (QDomNode iconNode = childDict.firstChild(); !iconNode.isNull(); iconNode = iconNode.nextSibling())
                      {
                        QDomElement iconElement = iconNode.toElement();
                        if (!iconElement.isNull())
                        {
                          if (iconElement.tagName() == "key")
                          {
                            iconType = iconElement.text();
                          }
                          else if (iconElement.tagName() == "string")
                          {
                            //	iconPath = fIconSettingsFile + "/" + iconElement.text();

                            if (attrName == "List")
                            {
                              m_status_icons_name.insert(m_from_adium.value(iconType,""), iconElement.text());
                              /*else if (attrName == "Tabs")
                              	fTabIconsMap.insert(iconType, iconPath);*/
                            }
                          }
                        }
                      }
                    }
                  }
                }
              }
            }
          }
        }

        xmlFile.close();
      }
    }
    while (false);
    if (m_status_icons_name.value("online","").isEmpty() || m_status_icons_name.value("offline","").isEmpty())
    {
      m_status_icons_name.clear();
      return false;
    }
    if (m_status_icons_name.value("ffc","")=="")
      m_status_icons_name["ffc"] = m_status_icons_name["online"];
    if (m_status_icons_name.value("na","")=="")
      m_status_icons_name["na"] = m_status_icons_name["away"];
    if (m_status_icons_name.value("occupied","")=="")
      m_status_icons_name["occupied"] = m_status_icons_name["away"];
    if (m_status_icons_name.value("dnd","")=="")
      m_status_icons_name["dnd"] = m_status_icons_name["away"];
    if (m_status_icons_name.value("athome","")=="")
      m_status_icons_name["athome"] = m_status_icons_name["online"];
    if (m_status_icons_name.value("atwork","")=="")
      m_status_icons_name["atwork"] = m_status_icons_name["online"];
    if (m_status_icons_name.value("lunch","")=="")
      m_status_icons_name["lunch"] = m_status_icons_name["online"];
    if (m_status_icons_name.value("evil","")=="")
      m_status_icons_name["evil"] = m_status_icons_name["online"];
    if (m_status_icons_name.value("depression","")=="")
      m_status_icons_name["depression"] = m_status_icons_name["online"];
    if (m_status_icons_name.value("noauth","")=="")
      m_status_icons_name["noauth"] = m_status_icons_name["offline"];
    m_path_to_status_icons = path;
    return true;
  }
  return false;
}
bool IconManager::setIconPackage(const QString & path)
{
  if (!QDir(path).exists())
    return false;
  m_path_to_system_icons = path;
  return true;
}
