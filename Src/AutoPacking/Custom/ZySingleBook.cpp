#include <QJsonParseError>
#include <QJsonObject>
#include <QImage>
#include <QPixmap>
#include <QPainter>
#include <QBitmap>
#include "ZySingleBook.h"
#include "Model/PathManager.h"
#include "Network/DownLoad.h"

ZySingleBook::ZySingleBook(QObject *parent, QString &bookId, QString &save) :QObject(parent),
mbookId(bookId),
msave(save),
mcoverUrl(""),
misMakeIcon(false),
mnewBookId("")
{
}

ZySingleBook::~ZySingleBook()
{

}

bool ZySingleBook::GetCover()
{
	DownLoad down(this, mcoverUrl);
	QString saveFile = msave + "/assets/covers/" + mnewBookId + ".jpg";
	if (!down.DownFile(saveFile)){
		return false;
	}
	return true;
}

bool ZySingleBook::GetBookInfo()
{
	QString url = QString("%1/api/api.php?act=bookinfo&id=%2&p3=809133").arg(PathManager::GetUrl()).arg(mbookId);
	DownLoad down(this, url);
	QByteArray json;
	if (!down.Get(json)){
		return false;
	}

	QJsonParseError jsonError;
	QJsonDocument parseDoucment = QJsonDocument::fromJson(json, &jsonError);
	if (jsonError.error != QJsonParseError::NoError)
	{
		return false;
	}
	if (parseDoucment.isObject())
	{
		return false;
	}
	QJsonObject obj = parseDoucment.object();
	if (!obj.contains("cover"))
	{
		return false;
	}
	QJsonValue nameValue = obj.take("cover");
	if (!nameValue.isString())
	{
		return false;
	}
	mcoverUrl = nameValue.toString();

	if (!obj.contains("id"))
	{
		return false;
	}
	nameValue = obj.take("id");
	if (!nameValue.isString())
	{
		return false;
	}
	mnewBookId = nameValue.toString();


	QFile file(msave + "/assets/preset_books/stuffed_" + mnewBookId);
	if (!file.open(QIODevice::WriteOnly | QIODevice::Text)){
		return false;
	}
	QTextStream txtOutput(&file);
	txtOutput.setCodec("UTF-8");
	txtOutput << json;
	file.close();
	return true;
}

bool ZySingleBook::GetIcon()
{
	QString resPath = PathManager::GetResPath();
	QString resIcon = resPath + "/" + mbookId + ".png";
	QString bookIcon = msave + "/res/drawable-hdpi/icon.png";
	QFile icon(resIcon);
	if (icon.exists()){
		misMakeIcon = false;
		if (!PathManager::CopyFile(resIcon, bookIcon, true)){
			return false;
		}
		return true;
	}

	misMakeIcon = true;
	QImage image;
	image.load(msave + "/assets/covers/" + mnewBookId + ".jpg");
	QPixmap pixmapToShow = QPixmap::fromImage(image.scaled(QSize(72, 72), Qt::IgnoreAspectRatio, Qt::SmoothTransformation));


	QImage frame(PathManager::GetFramePng());
	QBitmap mask(PathManager::GetMaskPng());
	pixmapToShow.setMask(mask);

	QPainter paint(&pixmapToShow);
	QPoint pos(0, 0);
	paint.drawImage(pos, frame);
	if (!pixmapToShow.save(bookIcon, "PNG", 90)){
		return false;
	}
	return true;
}

bool ZySingleBook::MakeBook(bool &isMakeIcon)
{
	if (!GetBookInfo()){
		return false;
	}
	if (!GetCover()){
		return false;
	}
	if (!GetIcon()){
		return false;
	}
	isMakeIcon = misMakeIcon;
	return true;
}