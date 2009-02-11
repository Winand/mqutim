#include <QtDebug>
#include <QTextDocument>
#include <QStringList>
#include "chatwindowstyle.h"
#include "chatwindowstyleoutput.h"

ChatWindowStyleOutput::ChatWindowStyleOutput(const QString &_styleName, const QString &_variantName)
{

	styleUsed = new ChatWindowStyle(_styleName);
	variantUsedName = _variantName;

}

ChatWindowStyleOutput::~ChatWindowStyleOutput()
{
	delete styleUsed;
}

ChatWindowStyle::StyleVariants ChatWindowStyleOutput::getVariants() const
{
	return styleUsed->getVariants();
}

QString ChatWindowStyleOutput::getMainCSS()
{
	return styleUsed->getMainCSS();
}

QStringList ChatWindowStyleOutput::getPaths()
{
	QStringList paths;

	paths << styleUsed->getStyleBaseHref();
	paths << styleUsed->getStyleBaseHref() + "Variants/";
	paths << styleUsed->getStyleBaseHref() + "Images/";
	paths << styleUsed->getStyleBaseHref() + "Incoming/";
	paths << styleUsed->getStyleBaseHref() + "Outgoing/";
	paths << styleUsed->getStyleBaseHref() + "styles/";

	return paths;
}

QString ChatWindowStyleOutput::makeSkeleton(const QString &_chatName, const QString &_ownerName, 
											const QString &_partnerName, const QString &_ownerIconPath,
											const QString &_partnerIconPath, const QString &_time)
{
	QString headerHTML = styleUsed->getHeaderHtml();
	QString footerHTML = styleUsed->getFooterHtml();


	QString path_to_base;
//#if defined(Q_OS_WIN32)
       path_to_base = styleUsed->getStyleBaseHref();
//#else
//	path_to_base = "file://"+ styleUsed->getStyleBaseHref();
//#endif
	QString generalSkeleton = styleUsed->getTemplateHtml();
	generalSkeleton.replace(generalSkeleton.indexOf("%@"),2,path_to_base);
	generalSkeleton.replace(generalSkeleton.lastIndexOf("%@"),2,styleUsed->getFooterHtml());
	generalSkeleton.replace(generalSkeleton.lastIndexOf("%@"),2,styleUsed->getHeaderHtml());
	if(!variantUsedName.isEmpty())
		generalSkeleton.replace(generalSkeleton.lastIndexOf("%@"),2,"Variants/" + variantUsedName + ".css");
	else
		generalSkeleton.replace(generalSkeleton.lastIndexOf("%@"),2,"main.css");
	if(generalSkeleton.contains("%@"))
		generalSkeleton.replace(generalSkeleton.indexOf("%@"),2,"@import url( \"main.css\" );");
//	generalSkeleton.replace("%rep2%", "main.css");
//	generalSkeleton.replace("%rep3%", "Variants/" + variantUsedName + ".css");
//	generalSkeleton.replace("%rep4%", headerHTML);

	generalSkeleton = generalSkeleton.replace("%chatName%", makeHTML(_chatName));
	generalSkeleton = generalSkeleton.replace("%sourceName%", makeHTML(_ownerName));
	generalSkeleton = generalSkeleton.replace("%destinationName%", makeHTML(_partnerName));
	generalSkeleton = generalSkeleton.replace("%timeOpened%", makeHTML(_time));
	
	QRegExp timeRegExp("%timeOpened\\{([^}]*)\\}%");
	int pos=0;
	while((pos=timeRegExp.indexIn(generalSkeleton, pos)) != -1)
	{
		generalSkeleton = generalSkeleton.replace(pos, timeRegExp.cap(0).length(), _time);
	}
	
	if(_ownerIconPath == "")
		generalSkeleton = generalSkeleton.replace("%outgoingIconPath%", "outgoing_icon.png");
	else
#if defined(Q_OS_WIN32)
		generalSkeleton = generalSkeleton.replace("%outgoingIconPath%", _ownerIconPath);
#else
		generalSkeleton = generalSkeleton.replace("%outgoingIconPath%", "file://" + _ownerIconPath);
#endif

	if(_partnerIconPath == "")
		generalSkeleton = generalSkeleton.replace("%incomingIconPath%", "incoming_icon.png");
	else
#if defined(Q_OS_WIN32)
		generalSkeleton = generalSkeleton.replace("%incomingIconPath%", _partnerIconPath);
#else
		generalSkeleton = generalSkeleton.replace("%incomingIconPath%", "file://" + _partnerIconPath);
#endif
	return generalSkeleton;
}

void ChatWindowStyleOutput::setVariant(const QString &_variantName)
{
	variantUsedName = _variantName;
}

QString ChatWindowStyleOutput::makeMessage(const QString &_name, const QString &_message,
						const bool &_direction, const QString &_time, const QString &_avatarPath,
						const bool &_aligment, const QString &_senderID, const QString &_service,
						const bool &_sameSender, bool _history)
{
	// prepare values, so they could be inserted to html code
	QString html;
//	if(_sameSender)
//	{
//		if(_direction)
//		{
//			html = styleUsed->getNextOutgoingHtml();
//			if(html == "")
//				html = styleUsed->getOutgoingHtml();
//
//		}
//		else
//		{
//			html = styleUsed->getNextIncomingHtml();
//			if(html == "")
//				html = styleUsed->getIncomingHtml();
//		}
//	}
//	else
//	{
//		if(_direction)
//			html = styleUsed->getOutgoingHtml();
//		else
//			html = styleUsed->getIncomingHtml();
//	}
	if(_history)
	{
		if ( _direction )
			html = _sameSender ? styleUsed->getNextOutgoingHistoryHtml() : styleUsed->getOutgoingHistoryHtml();
		else
			html = _sameSender ? styleUsed->getNextIncomingHistoryHtml() : styleUsed->getIncomingHistoryHtml();
	}
	else
	{
		if ( _direction )
			html = _sameSender ? styleUsed->getNextOutgoingHtml() : styleUsed->getOutgoingHtml();
		else
			html = _sameSender ? styleUsed->getNextIncomingHtml() : styleUsed->getIncomingHtml();
	}
	
	QString avatarPath = _avatarPath;

	// Replace %sender% to name
	html = html.replace("%sender%", makeHTML(_name));
	// Replace %senderScreenName% to name
	html = html.replace("%senderScreenName%", makeHTML(_senderID));
	// Replace %time% to time
	html = html.replace("%time%", makeHTML(_time));
	// Replace %time{X}%
	QRegExp timeRegExp("%time\\{([^}]*)\\}%");
	int pos=0;
	while((pos=timeRegExp.indexIn(html, pos)) != -1)
	{
		html = html.replace(pos, timeRegExp.cap(0).length(), _time);
	}
	// Replace %service% to protocol name
	// TODO: have to get protocol global value somehow
	html = html.replace("%service%", _service);
	// Replace %protocolIcon% to sender statusIcon path
	// TODO: find icon to add here
	html = html.replace("%senderStatusIcon%", "");
	// Replace userIconPath
	if(avatarPath == "")
	{
		if(_direction)
			avatarPath = (styleUsed->getStyleBaseHref() + "Outgoing/buddy_icon.png");
		else
			avatarPath = (styleUsed->getStyleBaseHref() + "Incoming/buddy_icon.png");
		
	}
//#if defined(Q_OS_WIN32)
       html = html.replace("%userIconPath%", avatarPath);
//#else
//	html = html.replace("%userIconPath%", "file://" + avatarPath);
//#endif
	

	// search for background colors and change them, so CSS would stay clean
	QString bgColor = "inherit";
	QRegExp textBackgroundRegExp("%textbackgroundcolor\\{([^}]*)\\}%");
	int textPos=0;
	while((textPos=textBackgroundRegExp.indexIn(html, textPos)) != -1)
	{
		html = html.replace(textPos, textBackgroundRegExp.cap(0).length(), bgColor);
	}

	// Replace %messageDirection% with "rtl"(Right-To-Left) or "ltr"(Left-to-right)
	html = html.replace("%messageDirection%", _aligment ? "ltr" : "rtl" );

	// Replace %messages%, replacing last to avoid errors if messages contains tags
	QString message = _message;
	html = html.replace("%message%", message.replace("\\","\\\\").remove('\r').replace("%","&#37;")+"&nbsp;");
	return html;
}

QString ChatWindowStyleOutput::makeAction(const QString &_name, const QString &_message,
						const bool &_direction, const QString &_time, const QString &_avatarPath,
						const bool &_aligment, const QString &_senderID, const QString &_service)
{
	QString html = _direction?styleUsed->getOutgoingActionHtml():styleUsed->getIncomingActionHtml();
	
	QString avatarPath = _avatarPath;

	// Replace %sender% to name
	html = html.replace("%sender%", makeHTML(_name));
	// Replace %senderScreenName% to name
	html = html.replace("%senderScreenName%", makeHTML(_senderID));
	// Replace %time% to time
	html = html.replace("%time%", makeHTML(_time));
	// Replace %time{X}%
	QRegExp timeRegExp("%time\\{([^}]*)\\}%");
	int pos=0;
	while((pos=timeRegExp.indexIn(html, pos)) != -1)
	{
		html = html.replace(pos, timeRegExp.cap(0).length(), _time);
	}
	// Replace %service% to protocol name
	// TODO: have to get protocol global value somehow
	html = html.replace("%service%", _service);
	// Replace %protocolIcon% to sender statusIcon path
	// TODO: find icon to add here
	html = html.replace("%senderStatusIcon%", "");
	// Replace userIconPath
	if(avatarPath == "")
	{
		if(_direction)
			avatarPath = (styleUsed->getStyleBaseHref() + "Outgoing/buddy_icon.png");
		else
			avatarPath = (styleUsed->getStyleBaseHref() + "Incoming/buddy_icon.png");
		
	}
//#if defined(Q_OS_WIN32)
       html = html.replace("%userIconPath%", avatarPath);
//#else
//	html = html.replace("%userIconPath%", "file://" + avatarPath);
//#endif
	

	// search for background colors and change them, so CSS would stay clean
	QString bgColor = "inherit";
	QRegExp textBackgroundRegExp("%textbackgroundcolor\\{([^}]*)\\}%");
	int textPos=0;
	while((textPos=textBackgroundRegExp.indexIn(html, textPos)) != -1)
	{
		html = html.replace(textPos, textBackgroundRegExp.cap(0).length(), bgColor);
	}

	// Replace %messageDirection% with "rtl"(Right-To-Left) or "ltr"(Left-to-right)
	html = html.replace("%messageDirection%", _aligment ? "ltr" : "rtl" );

	// Replace %messages%, replacing last to avoid errors if messages contains tags
	QString message = _message;
	html = html.replace("%message%", message.replace("\\","\\\\").remove('\r').replace("%","&#37;")+"&nbsp;");
	return html;
}
						
QString ChatWindowStyleOutput::makeStatus(const QString &_message, const QString &_time)
{
	QString html = styleUsed->getStatusHtml();
	/*if(!html.contains("id=\"insert\""))
		html.append("<div id=\"insert\"></div>");*/
	// Replace %time% to time
	html = html.replace("%time%", makeHTML(_time));
	// Replace %time{X}%
	QRegExp timeRegExp("%time\\{([^}]*)\\}%");
	int pos=0;
	while((pos=timeRegExp.indexIn(html, pos)) != -1)
	{
		html = html.replace(pos, timeRegExp.cap(0).length(), _time);
	}
	// Replace %messages%, replacing last to avoid errors if messages contains tags
	html = html.replace("%message%", makeHTML(_message).replace("\\","\\\\").remove('\r').replace("%","&#37;")+"&nbsp;");
	return html;
}

QString ChatWindowStyleOutput::makeHTML(const QString &_sourceText)
{
	QString htmlOut = _sourceText;
	// changes all " symbols to &quot;
	//htmlOut.replace('"',"&quot;");
	// change text to html
	htmlOut = Qt::escape(htmlOut);

	return htmlOut;
}

QString ChatWindowStyleOutput::findEmail(const QString &_sourceHTML)
{
	QString html = _sourceHTML;
	QRegExp emailRegExp("((?:\\w+\\.)*\\w+@(?:\\w+\\.)*\\w+)");
	emailRegExp.indexIn(html);
	for(int i=0;i<emailRegExp.numCaptures();i++)
	{
		QString email = emailRegExp.cap(i);
		email = "<a href=\"mailto:" + email + "\">" + email + "</a>";
		html.replace(emailRegExp.cap(i), email);		
	}
	return html;
}

QString ChatWindowStyleOutput::findWebAddress(const QString &_sourceHTML)
{
	QString html = _sourceHTML;
	QRegExp linkRegExp("(([a-z]+://|www\\d?\\.)[^\\s]+)");
	int pos = 0;
	while((pos=linkRegExp.indexIn(html, pos)) != -1)
	{
		QString link = linkRegExp.cap(0);
		link = "<a href='" + link + "' target='_blank'>" + link + "</a>";
		html.replace(linkRegExp.cap(0), link);
		pos += link.count();
	}
	return html;
}

