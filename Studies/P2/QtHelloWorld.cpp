/*
1) qmake -project
2) aggiungere "QT += widgets" al file .pro
3) qmake
4) make
*/

#include <QApplication>
#include <QPushButton>

int main (int argc, char* argv[]) {
	QApplication app(argc, argv);
	QPushButton hello("Hello world!");
	hello.show();
	return QApplication::exec();
}
