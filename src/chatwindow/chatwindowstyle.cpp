#include <QFile>
#include <QDir>
#include <QHash>
#include <QStringList>
#include <QTextCodec>
#include <QTextStream>
#include <QtDebug>
#include <QSettings>
#include <QResource>
#include "chatwindowstyle.h"

class ChatWindowStyle::Private
{
public:
	QString styleName;
	StyleVariants variantsList;
	QString baseHref;
	QString currentVariantPath;

	QString templateHtml;
	QString headerHtml;
	QString footerHtml;
	QString incomingHtml;
	QString nextIncomingHtml;
	QString outgoingHtml;
	QString nextOutgoingHtml;
	QString incomingHistoryHtml;
	QString nextIncomingHistoryHtml;
	QString outgoingHistoryHtml;
	QString nextOutgoingHistoryHtml;
	QString incomingActionHtml;
	QString outgoingActionHtml;
	QString statusHtml;
	QString mainCSS;
	bool defaultStyle;
};

class Resource: public QResource
{
	public:
	using QResource::children;
	using QResource::isFile;
	using QResource::isDir;
	Resource() : QResource(){}
};

ChatWindowStyle::ChatWindowStyle(const QString &styleName, StyleBuildMode styleBuildMode)
	: d(new Private)
{
	init(styleName, styleBuildMode);
}

ChatWindowStyle::ChatWindowStyle(const QString &styleName, const QString &variantPath, StyleBuildMode styleBuildMode)
	: d(new Private)
{
	d->currentVariantPath = variantPath;
	init(styleName, styleBuildMode);
}

void ChatWindowStyle::init(const QString &styleName, StyleBuildMode styleBuildMode)
{
//	QSettings settings(QSettings::defaultFormat(), QSettings::UserScope, "qutim", "account");
//	QString styleDir = QString(settings.fileName().section('/', 0, -2) + "/skins/%1/Contents/Resources/").arg(styleName);
	QString styleDir = styleName;
	if(QDir::isAbsolutePath(styleName) && QDir(styleName).exists())
	{
		d->styleName = styleName;
		d->baseHref = styleDir;
		d->defaultStyle = false;
	}
	else
	{
		d->styleName = "Default";
		d->baseHref = "webkit/Contents/Resources/";
		d->defaultStyle = true;
	}

	readStyleFiles();
	if(styleBuildMode & StyleBuildNormal)
	{
		listVariants();
	}
}

ChatWindowStyle::~ChatWindowStyle()
{
	delete d;
}

ChatWindowStyle::StyleVariants ChatWindowStyle::getVariants()
{
	// If the variantList is empty, list available variants.
	if( d->variantsList.isEmpty() )
	{
		listVariants();
	}
	return d->variantsList;
}

QString ChatWindowStyle::getStyleName() const
{
	return d->styleName;
}

QString ChatWindowStyle::getStyleBaseHref() const
{
	if(d->defaultStyle)
		return "qrc:/"+d->baseHref;
#if defined(Q_OS_WIN32)
	return "file:///"+d->baseHref;
#else
	return "file://"+d->baseHref;
#endif
}

QString ChatWindowStyle::getTemplateHtml() const
{
	return d->templateHtml;
}

QString ChatWindowStyle::getHeaderHtml() const
{
	return d->headerHtml;
}

QString ChatWindowStyle::getFooterHtml() const
{
	return d->footerHtml;
}

QString ChatWindowStyle::getIncomingHtml() const
{
	return d->incomingHtml;
}

QString ChatWindowStyle::getNextIncomingHtml() const
{
	return d->nextIncomingHtml;
}

QString ChatWindowStyle::getOutgoingHtml() const
{
	return d->outgoingHtml;
}

QString ChatWindowStyle::getNextOutgoingHtml() const
{
	return d->nextOutgoingHtml;
}

QString ChatWindowStyle::getIncomingHistoryHtml() const
{
	return d->incomingHistoryHtml;
}

QString ChatWindowStyle::getNextIncomingHistoryHtml() const
{
	return d->nextIncomingHistoryHtml;
}

QString ChatWindowStyle::getOutgoingHistoryHtml() const
{
	return d->outgoingHistoryHtml;
}

QString ChatWindowStyle::getNextOutgoingHistoryHtml() const
{
	return d->nextOutgoingHistoryHtml;
}

QString ChatWindowStyle::getIncomingActionHtml() const
{
	return d->incomingActionHtml;
}

QString ChatWindowStyle::getOutgoingActionHtml() const
{
	return d->outgoingActionHtml;
}

QString ChatWindowStyle::getStatusHtml() const
{
	return d->statusHtml;
}

QString ChatWindowStyle::getMainCSS() const
{
	return d->mainCSS;
}

void ChatWindowStyle::listVariants()
{
	QString variantDirPath = d->baseHref + QString::fromUtf8("Variants/");
	
	if(d->defaultStyle)
	{
		Resource variantDir;
		variantDir.setFileName(variantDirPath);
		if(variantDir.isDir())
		{
			QStringList variantList = variantDir.children();
			foreach(QString variantName, variantList)
			{
				if(variantName.endsWith(".css"))
				{
					QString variantPath = "Variants/"+variantName;
					variantName = variantName.left(variantName.lastIndexOf("."));	
					d->variantsList.insert(variantName, variantPath);
				}
			}
		}
	}
	else
	{
		QDir variantDir(variantDirPath);
		variantDir.makeAbsolute();

		QStringList variantList = variantDir.entryList(QStringList("*.css"));
		QStringList::ConstIterator it, itEnd = variantList.constEnd();
		QString compactVersionPrefix("_compact_");
		for(it = variantList.constBegin(); it != itEnd; ++it)
		{
			QString variantName = *it, variantPath;
			// Retrieve only the file name.
			variantName = variantName.left(variantName.lastIndexOf("."));
			QString compactVersionFilename = *it;
			QString compactVersionPath = variantDirPath + compactVersionFilename.prepend( compactVersionPrefix );
			// variantPath is relative to baseHref.
			variantPath = QString("Variants/%1").arg(*it);
			d->variantsList.insert(variantName, variantPath);
		}
	}
}

void ChatWindowStyle::readStyleFiles()
{
	QString templateFile = d->baseHref + QString("Template.html");
	QString headerFile = d->baseHref + QString("Header.html");
	QString footerFile = d->baseHref + QString("Footer.html");
	QString incomingFile = d->baseHref + QString("Incoming/Content.html");
	QString nextIncomingFile = d->baseHref + QString("Incoming/NextContent.html");
	QString outgoingFile = d->baseHref + QString("Outgoing/Content.html");
	QString nextOutgoingFile = d->baseHref + QString("Outgoing/NextContent.html");
	QString incomingHistoryFile = d->baseHref + QString("Incoming/Context.html");
	QString nextIncomingHistoryFile = d->baseHref + QString("Incoming/NextContext.html");
	QString outgoingHistoryFile = d->baseHref + QString("Outgoing/Context.html");
	QString nextOutgoingHistoryFile = d->baseHref + QString("Outgoing/NextContext.html");
	QString incomingActionFile = d->baseHref + QString("Incoming/Action.html");
	QString outgoingActionFile = d->baseHref + QString("Outgoing/Action.html");
	QString statusFile = d->baseHref + QString("Status.html");
	QString mainCSSFile = d->baseHref + QString("main.css");
	
	if(d->defaultStyle)
	{
		Resource resourceAccess;
		// First load template file.
		resourceAccess.setFileName(templateFile);
		if(resourceAccess.isFile())
			d->templateHtml = QString::fromUtf8((char*)resourceAccess.data(),resourceAccess.size());
		// Load header file.
		resourceAccess.setFileName(headerFile);
		if(resourceAccess.isFile())
			d->headerHtml = QString::fromUtf8((char*)resourceAccess.data(),resourceAccess.size());
		// Load Footer file
		resourceAccess.setFileName(footerFile);
		if(resourceAccess.isFile())
			d->footerHtml = QString::fromUtf8((char*)resourceAccess.data(),resourceAccess.size());
		// Load incoming file
		resourceAccess.setFileName(incomingFile);
		if(resourceAccess.isFile())
			d->incomingHtml = QString::fromUtf8((char*)resourceAccess.data(),resourceAccess.size());
		// Load next Incoming file
		resourceAccess.setFileName(nextIncomingFile);
		if(resourceAccess.isFile())
			d->nextIncomingHtml = QString::fromUtf8((char*)resourceAccess.data(),resourceAccess.size());
		else
			d->nextIncomingHtml = d->incomingHtml;
		// Load outgoing file
		resourceAccess.setFileName(outgoingFile);
		if(resourceAccess.isFile())
			d->outgoingHtml = QString::fromUtf8((char*)resourceAccess.data(),resourceAccess.size());
		else
			d->outgoingHtml = d->incomingHtml;
		// Load next outgoing file
		resourceAccess.setFileName(nextOutgoingFile);
		if(resourceAccess.isFile())
			d->nextOutgoingHtml = QString::fromUtf8((char*)resourceAccess.data(),resourceAccess.size());
		else
			d->nextOutgoingHtml = d->outgoingHtml;
		// Load incoming history file
		resourceAccess.setFileName(incomingHistoryFile);
		if(resourceAccess.isFile())
			d->incomingHistoryHtml = QString::fromUtf8((char*)resourceAccess.data(),resourceAccess.size());
		else
			d->incomingHistoryHtml = d->incomingHtml;
		// Load next Incoming history file
		resourceAccess.setFileName(nextIncomingHistoryFile);
		if(resourceAccess.isFile())
			d->nextIncomingHistoryHtml = QString::fromUtf8((char*)resourceAccess.data(),resourceAccess.size());
		else
			d->nextIncomingHistoryHtml = d->nextIncomingHtml;
		// Load outgoing history file
		resourceAccess.setFileName(outgoingHistoryFile);
		if(resourceAccess.isFile())
			d->outgoingHistoryHtml = QString::fromUtf8((char*)resourceAccess.data(),resourceAccess.size());
		else
			d->outgoingHistoryHtml = d->outgoingHtml;
		// Load next outgoing history file
		resourceAccess.setFileName(nextOutgoingHistoryFile);
		if(resourceAccess.isFile())
			d->nextOutgoingHistoryHtml = QString::fromUtf8((char*)resourceAccess.data(),resourceAccess.size());
		else
			d->nextOutgoingHistoryHtml = d->nextOutgoingHtml;
		// Load status file
		resourceAccess.setFileName(statusFile);
		if(resourceAccess.isFile())
			d->statusHtml = QString::fromUtf8((char*)resourceAccess.data(),resourceAccess.size());
		// Load incoming action file
		resourceAccess.setFileName(incomingActionFile);
		if(resourceAccess.isFile())
			d->incomingActionHtml = QString::fromUtf8((char*)resourceAccess.data(),resourceAccess.size());
		else
		{
			d->incomingActionHtml = d->statusHtml;
			d->incomingActionHtml = d->incomingActionHtml.replace("%message%","%sender% %message%");
		}
		// Load outgoing action file
		resourceAccess.setFileName(outgoingActionFile);
		if(resourceAccess.isFile())
			d->outgoingActionHtml = QString::fromUtf8((char*)resourceAccess.data(),resourceAccess.size());
		else
			d->outgoingActionHtml = d->incomingActionHtml;
		// Load main.css file
		resourceAccess.setFileName(mainCSSFile);
		if(resourceAccess.isFile())
			d->mainCSS = QString::fromUtf8((char*)resourceAccess.data(),resourceAccess.size());
	}
	else
	{
		QFile fileAccess;
		// First load template file.
		if( QFile::exists(templateFile) )
		{
			fileAccess.setFileName(templateFile);
			fileAccess.open(QIODevice::ReadOnly);
			QTextStream templateStream(&fileAccess);
			templateStream.setCodec(QTextCodec::codecForName("UTF-8"));
			d->templateHtml = templateStream.readAll();
			fileAccess.close();
		}
		else
		{
			QResource resourceAccess("webkit/Contents/Resources/Template.html");
			d->templateHtml = QString::fromUtf8((char*)resourceAccess.data(),resourceAccess.size());
		}
		// Load header file.
		if( QFile::exists(headerFile) )
		{
			fileAccess.setFileName(headerFile);
			fileAccess.open(QIODevice::ReadOnly);
			QTextStream headerStream(&fileAccess);
			headerStream.setCodec(QTextCodec::codecForName("UTF-8"));
			d->headerHtml = headerStream.readAll();
			//qDebug() << "Header HTML: " << d->headerHtml;
			fileAccess.close();
		}
		// Load Footer file
		if( QFile::exists(footerFile) )
		{
			fileAccess.setFileName(footerFile);
			fileAccess.open(QIODevice::ReadOnly);	
			QTextStream headerStream(&fileAccess);
			headerStream.setCodec(QTextCodec::codecForName("UTF-8"));
			d->footerHtml = headerStream.readAll();
			//qDebug() << "Footer HTML: " << d->footerHtml;
			fileAccess.close();
		}
		// Load incoming file
		if( QFile::exists(incomingFile) )
		{
			fileAccess.setFileName(incomingFile);	
			fileAccess.open(QIODevice::ReadOnly);
			QTextStream headerStream(&fileAccess);
			headerStream.setCodec(QTextCodec::codecForName("UTF-8"));
			d->incomingHtml = headerStream.readAll();
//			qDebug() << "Incoming HTML: " << d->incomingHtml;
			fileAccess.close();
		}
		// Load next Incoming file
		if( QFile::exists(nextIncomingFile) )
		{
			fileAccess.setFileName(nextIncomingFile);
			fileAccess.open(QIODevice::ReadOnly);
			QTextStream headerStream(&fileAccess);
			headerStream.setCodec(QTextCodec::codecForName("UTF-8"));
			d->nextIncomingHtml = headerStream.readAll();
			//qDebug() << "NextIncoming HTML: " << d->nextIncomingHtml;
			fileAccess.close();
		}
		else
			d->nextIncomingHtml = d->incomingHtml;
		// Load outgoing file
		if( QFile::exists(outgoingFile) )
		{
			fileAccess.setFileName(outgoingFile);
			fileAccess.open(QIODevice::ReadOnly);
			QTextStream headerStream(&fileAccess);
			headerStream.setCodec(QTextCodec::codecForName("UTF-8"));
			d->outgoingHtml = headerStream.readAll();
			//qDebug() << "Outgoing HTML: " << d->outgoingHtml;
			fileAccess.close();
		}
		else
			d->outgoingHtml = d->incomingHtml;
		// Load next outgoing file
		if( QFile::exists(nextOutgoingFile) )
		{
			fileAccess.setFileName(nextOutgoingFile);
			fileAccess.open(QIODevice::ReadOnly);	
			QTextStream headerStream(&fileAccess);
			headerStream.setCodec(QTextCodec::codecForName("UTF-8"));
			d->nextOutgoingHtml = headerStream.readAll();
			//qDebug() << "NextOutgoing HTML: " << d->nextOutgoingHtml;
			fileAccess.close();
		}
		else
                        d->nextOutgoingHtml = d->outgoingHtml;
		// Load incoming history file
		if( QFile::exists(incomingHistoryFile) )
		{
			fileAccess.setFileName(incomingHistoryFile);	
			fileAccess.open(QIODevice::ReadOnly);
			QTextStream headerStream(&fileAccess);
			headerStream.setCodec(QTextCodec::codecForName("UTF-8"));
			d->incomingHistoryHtml = headerStream.readAll();
//			qDebug() << "Incoming HTML: " << d->incomingHtml;
			fileAccess.close();
		}
		else
			d->incomingHistoryHtml = d->incomingHtml;
		// Load next Incoming history file
		if( QFile::exists(nextIncomingHistoryFile) )
		{
			fileAccess.setFileName(nextIncomingHistoryFile);
			fileAccess.open(QIODevice::ReadOnly);
			QTextStream headerStream(&fileAccess);
			headerStream.setCodec(QTextCodec::codecForName("UTF-8"));
			d->nextIncomingHistoryHtml = headerStream.readAll();
			//qDebug() << "NextIncoming HTML: " << d->nextIncomingHtml;
			fileAccess.close();
		}
		else
			d->nextIncomingHistoryHtml = d->nextIncomingHtml;
		// Load outgoing history file
		if( QFile::exists(outgoingHistoryFile) )
		{
			fileAccess.setFileName(outgoingHistoryFile);
			fileAccess.open(QIODevice::ReadOnly);
			QTextStream headerStream(&fileAccess);
			headerStream.setCodec(QTextCodec::codecForName("UTF-8"));
			d->outgoingHistoryHtml = headerStream.readAll();
			//qDebug() << "Outgoing HTML: " << d->outgoingHtml;
			fileAccess.close();
		}
		else
			d->outgoingHistoryHtml = d->outgoingHtml;
		// Load next outgoing history file
		if( QFile::exists(nextOutgoingHistoryFile) )
		{
			fileAccess.setFileName(nextOutgoingHistoryFile);
			fileAccess.open(QIODevice::ReadOnly);	
			QTextStream headerStream(&fileAccess);
			headerStream.setCodec(QTextCodec::codecForName("UTF-8"));
			d->nextOutgoingHistoryHtml = headerStream.readAll();
			//qDebug() << "NextOutgoing HTML: " << d->nextOutgoingHtml;
			fileAccess.close();
		}
		else
			d->nextOutgoingHistoryHtml = d->nextOutgoingHtml;
		// Load status file
		if( QFile::exists(statusFile) )
		{
			fileAccess.setFileName(statusFile);
			fileAccess.open(QIODevice::ReadOnly);
			QTextStream headerStream(&fileAccess);
			headerStream.setCodec(QTextCodec::codecForName("UTF-8"));
			d->statusHtml = headerStream.readAll();
			//qDebug() << "Status HTML: " << d->statusHtml;
			fileAccess.close();
		}
		// Load incoming action file
		if( QFile::exists(incomingActionFile) )
		{
			fileAccess.setFileName(incomingActionFile);
			fileAccess.open(QIODevice::ReadOnly);
			QTextStream headerStream(&fileAccess);
			headerStream.setCodec(QTextCodec::codecForName("UTF-8"));
			d->incomingActionHtml = headerStream.readAll();
			fileAccess.close();
		}
		else
		{
			d->incomingActionHtml = d->statusHtml;
			d->incomingActionHtml = d->incomingActionHtml.replace("%message%","%sender% %message%");
		}
		// Load outgoing action file
		if( QFile::exists(outgoingActionFile) )
		{
			fileAccess.setFileName(outgoingActionFile);
			fileAccess.open(QIODevice::ReadOnly);
			QTextStream headerStream(&fileAccess);
			headerStream.setCodec(QTextCodec::codecForName("UTF-8"));
			d->outgoingActionHtml = headerStream.readAll();
			//qDebug() << "Action HTML: " << d->actionHtml;
			fileAccess.close();
		}
		else
			d->outgoingActionHtml = d->incomingActionHtml;
		// Load main.css file
		if( QFile::exists(mainCSSFile) )
		{
			fileAccess.setFileName(mainCSSFile);
			fileAccess.open(QIODevice::ReadOnly);
			QTextStream headerStream(&fileAccess);
			headerStream.setCodec(QTextCodec::codecForName("UTF-8"));
			d->mainCSS = headerStream.readAll();
			//qDebug() << "mainCSS: " << d->mainCSS;
			fileAccess.close();
		}
	}
}

void ChatWindowStyle::reload()
{
	d->variantsList.clear();
	readStyleFiles();
	listVariants();
}
