#include "MainWindow/mainwindow.h"
#include <QApplication>
#include <QTextCodec>

int main(int argc, char *argv[])
{
	QTextCodec *codec = QTextCodec::codecForName("UTF-8");
	QTextCodec::setCodecForLocale(codec);
    QApplication a(argc, argv);
    MainWindow w;
	QFile fileCss(QString("./Css/widgetstylesheet.css"));
	QString t = fileCss.fileName();
	if (fileCss.exists()){
		fileCss.open(QFile::ReadOnly);
		QString qss = fileCss.readAll();
		qApp->setStyleSheet(qss);
		qApp->setPalette(QPalette(QColor("#F0F0F0")));
	}
	w.show();
    return a.exec();
}
