
#include "jClientIdentification.h"
#include <QSettings>
#include <QFileInfo>

jClientIdentification::jClientIdentification()
{

}

jClientIdentification::~jClientIdentification()
{
}

jClientIdentification &jClientIdentification::instance()
{
	static jClientIdentification jci;
	return jci;
}

void jClientIdentification::loadSettings()
{
    QSettings settings(QSettings::NativeFormat, QSettings::UserScope, "qutim/qutim."+m_profile_name, "jabbersettings");
    m_auto_request = settings.value("contacts/requestversion", true).toBool();
}

void jClientIdentification::init(const QString profile_name)
{
    m_profile_name = profile_name;
    loadSettings();
	QSettings settings(QSettings::NativeFormat, QSettings::UserScope, "qutim/qutim."+profile_name, "hash");
	QFileInfo info(settings.fileName());
	m_hash_path = info.absoluteDir().filePath("jabberhash");
	m_clients_info.clear();
	m_client_nodes.clear();
	QFile file(m_hash_path);
	if (file.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		QTextStream in(&file);
		in.setAutoDetectUnicode(false);
		in.setCodec("UTF-8");
		QString line;
		while(!in.atEnd())
		{
			line = in.readLine();
			if(line=="[client version]")
			{
				QString node_ver_str = in.readLine();
				int index = node_ver_str.lastIndexOf('#');
				QPair<QString,QString> node_ver(node_ver_str.left(index),node_ver_str.mid(index+1));
				if(!m_clients_info.contains(node_ver))
				{
					ClientInfo client_info;
					m_clients_info.insert(node_ver,client_info);
				}
				m_clients_info[node_ver].m_name = in.readLine();
				m_clients_info[node_ver].m_version = in.readLine();
				if(ifBase64(node_ver.second))
					m_clients_info[node_ver].m_os = in.readLine();
				m_client_nodes.insert(node_ver.first,m_clients_info[node_ver].m_name);

			}
			else if(line=="[client disco]")
			{
				QString node_ver_str = in.readLine();
				int index = node_ver_str.lastIndexOf('#');
				QPair<QString,QString> node_ver(node_ver_str.left(index),node_ver_str.mid(index+1));
				if(!m_clients_info.contains(node_ver))
				{
					ClientInfo client_info;
					m_clients_info.insert(node_ver,client_info);
				}
				line=in.readLine();
				while((line=in.readLine())!="[client disco/features/end]")
				{
					QString feature = line;
					m_clients_info[node_ver].m_features.append(feature);
				}
			}
		}
	}
}

bool jClientIdentification::ifBase64(const QString &ver)
{
	if(ver.length()%4!=0)
		return false;
	QRegExp regexp("^[a-zA-Z0-9+/]+={0,3}$");
	return ver.contains(regexp);
}

void jClientIdentification::setClient(void *info_v, const JID& jid, Client *jClient, DiscoHandler *disco_handler)
{
	jBuddy::ResourceInfo *info = (jBuddy::ResourceInfo *)info_v;
	if(!info)
		return;
	QPair<QString,QString> node_ver(info->m_caps_node,info->m_caps_ver);
	bool get_version=true, get_disco=true;

	if(m_clients_info.contains(node_ver))
	{
		get_version = m_clients_info[node_ver].m_name.isNull() || m_clients_info[node_ver].m_name.isEmpty();
		if(!get_version)
		{
			info->m_client_name = m_clients_info[node_ver].m_name;
			info->m_client_version = m_clients_info[node_ver].m_version;
			info->m_client_os = m_clients_info[node_ver].m_os;
		}
		get_disco = m_clients_info[node_ver].m_features.size()==0;
		if(!get_disco)
		{
			info->m_features = m_clients_info[node_ver].m_features;
		}
	}
        if(!m_auto_request)
            return;
	if(get_version)
	{
		if(m_client_nodes.contains(info->m_caps_node))
			info->m_client_name = m_client_nodes[info->m_caps_node];
		else
			info->m_client_name = info->m_caps_node;
		if(!ifBase64(info->m_caps_ver))
			info->m_client_version = info->m_caps_ver;
		IQ iq(IQ::Get, jid, jClient->getID());
		iq.addExtension(new VersionExtension());
		jClient->send(iq);
	}
	if(get_disco)
	{
		jClient->disco()->getDiscoInfo(jid,jProtocol::toStd(info->m_caps_node+"#"+info->m_caps_ver),disco_handler,0);
	}
}

void jClientIdentification::newInfo(const VersionExtension *version, void *info_v)
{
	jBuddy::ResourceInfo *info = (jBuddy::ResourceInfo *)info_v;
	if(!version)
		return;
	if(!info)
		return;
	QPair<QString,QString> node_ver(info->m_caps_node,info->m_caps_ver);
	if(!m_clients_info.contains(node_ver))
	{
		ClientInfo client_info;
		m_clients_info.insert(node_ver,client_info);
	}
	else if(!m_clients_info[node_ver].m_name.isNull() && !m_clients_info[node_ver].m_name.isEmpty())
	{
		info->m_client_name = version->name();
		info->m_client_version = version->version();
		info->m_client_os = version->os();
		return;
	}
	info->m_client_name = version->name();
	info->m_client_version = version->version();
	info->m_client_os = version->os();
	if(info->m_caps_node.isEmpty())
		return;
	if(!m_client_nodes.contains(info->m_caps_node))
		m_client_nodes.insert(info->m_caps_node,version->name());
	if(info->m_caps_ver.isEmpty())
		return;
	m_clients_info[node_ver].m_name = version->name();
	m_clients_info[node_ver].m_version = version->version();
	if(ifBase64(info->m_caps_ver))
		m_clients_info[node_ver].m_os = version->os();
	QFile file(m_hash_path);
	if (file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append))
	{
		QTextStream out(&file);
		out.setAutoDetectUnicode(false);
		out.setCodec("UTF-8");
		out << "[client version]\n";
		out << info->m_caps_node << "#" << info->m_caps_ver << "\n";
		out << version->name() << "\n";
		out << version->version() << "\n";
		if(ifBase64(info->m_caps_ver))
			out << version->os() << "\n";
		out << "\n";
	}
}

void jClientIdentification::newInfo(const Disco::Info &disco_info, void *info_v)
{
	jBuddy::ResourceInfo *info = (jBuddy::ResourceInfo *)info_v;
	if(!info)
		return;
	QPair<QString,QString> node_ver(info->m_caps_node,info->m_caps_ver);
	if(!m_clients_info.contains(node_ver))
	{
		ClientInfo client_info;
		m_clients_info.insert(node_ver,client_info);
	}
	else if(m_clients_info[node_ver].m_features.size()>0)
	{
		info->m_features = m_clients_info[node_ver].m_features;
		return;
	}
	QStringList features;
	const StringList &sl = disco_info.features();
	StringList::const_iterator it = sl.begin();
	for( ; it != sl.end(); ++it )
		features << jProtocol::fromStd((*it));
	qSort(features);
	info->m_features.clear();
	QString last="";
	for(int i=0;i<features.size();i++)
	{
		if(last!=features[i])
		{
			last=features[i];
			info->m_features << last;
		}
	}
	features = info->m_features;
	if(info->m_caps_node.isEmpty() || info->m_caps_ver.isEmpty())
		return;
	m_clients_info[node_ver].m_features = info->m_features;
	QFile file(m_hash_path);
	if (file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append))
	{
		QTextStream out(&file);
		out.setAutoDetectUnicode(false);
		out.setCodec("UTF-8");
		out << "[client disco]\n";
		out << info->m_caps_node << "#" << info->m_caps_ver << "\n\n";
		out << "[client disco/features/begin]\n";
		foreach(const QString &feature, info->m_features)
			out << feature << "\n";
		out << "[client disco/features/end]\n\n";
	}
}

QIcon jClientIdentification::clientIcon(const QString &name)
{
	QString icon_name = name.toLower();
	if(icon_name.isEmpty())
		return QIcon();
	else if(icon_name.startsWith("qip infium"))
		icon_name="qipinf";
	else if(icon_name.startsWith("mchat"))
                icon_name="mchat";
        else if(icon_name.startsWith("miranda"))
                icon_name="miranda";
        else if(icon_name.startsWith("hotcoffee"))
                icon_name="mirandahotcoffee";
	else if(icon_name.startsWith("psi"))
                icon_name="psi";
	else if(icon_name.startsWith("bombusmod"))
		icon_name="bombusmod";
	else if(icon_name.startsWith("bombus-ng"))
		icon_name="bombusng";
	else if(icon_name.startsWith("ichat"))
		icon_name="apple";
	else if(icon_name.startsWith("bombus.pl"))
		icon_name="bombuspl";
	else if(icon_name.startsWith("bombus+"))
		icon_name="bombusp";
	else if(icon_name.startsWith("exodus"))
		icon_name="exodus";
	else if(icon_name.startsWith("jabbim"))
		icon_name="jabbim";
        else if(icon_name.startsWith("jajc"))
		icon_name="jajc";
	else if(icon_name.startsWith("just another jabber client"))
		icon_name="jajc";
	else if(icon_name.startsWith("coccinella"))
		icon_name="coccinella";
	else if(icon_name.startsWith("bitlbee"))
		icon_name="bitlbee";
	else if(icon_name.startsWith("siemens native jabber client"))
		icon_name="siejc";
	else if(icon_name.startsWith("tkabber"))
		icon_name="tkabber";
        else if(icon_name.startsWith("talkonaut"))
		icon_name="talkonaut";
	else if(icon_name.startsWith("jabber.el"))
		icon_name="jabber.el";
	else if(icon_name.contains("bombus"))
		icon_name="bombus";
	else if(icon_name==QString::fromUtf8(QByteArray::fromHex("d18f2ed0bed0bdd0bbd0b0d0b9d0bd")))
		icon_name="yachat";
	QIcon icon;
	icon = QIcon(":/icons/clients/"+icon_name);
	if(icon.isNull() || icon.actualSize(QSize(16,16)).width()<0)
		icon = QIcon(":/icons/clients/unknown");
	return icon;
}
