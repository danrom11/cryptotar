#include <QApplication>
#include <QPushButton>
#include <QWidget>
#include <QLabel>
#include<QVBoxLayout>
#include<QFrame>
#include <QBuffer>
#include <QPushButton>
#include <QDebug>
#include <QString>
#include <QFileDialog>
#include <QDir>
#include <QStandardPaths>
#include <QListWidget>
#include <QPainter>
#include <QPen>
#include <QMouseEvent>
#include <QPixmap>



int main(int argc, char *argv[])
{

    int flag = 0;
	QApplication app(argc, argv);
    QWidget window;
    QString homed = "/home";
    QString name = qgetenv("USER");
    QString path = homed + "/" + name + "/";
    QString filenames;
    QFrame frame(&window);
    frame.setStyleSheet("background-color: rgb(248,233,215)");
    QListWidget *list = new QListWidget(&frame);
    list->setGeometry(0,120,1920,960);
    QFrame hline(&frame);
    hline.setGeometry(0,120,1920,2);
    hline.setStyleSheet("background-color: rgb(0,0,0)");
    int count = 0;

    //ADDBUTTON
    QPushButton buttonadd("Add file", &frame);
    buttonadd.setGeometry(0,0,384,120);

    QObject::connect(&buttonadd,&QPushButton::clicked,[&](){
    qDebug()<< "kek";
        QString filename = QFileDialog::getOpenFileName(
                    nullptr,
                    "Open File",
                    path,
                    "All files (*.*)"
                    );
        list->addItem(filename);
        count++;
        list->show();
    });

    //CLEARBUTTON
    QPushButton buttonclear("Clear", &frame);
    buttonclear.setGeometry(768,0,384,120);

    QObject::connect(&buttonclear,&QPushButton::clicked,[&](){
    list->clear();
    });

    //HELPBUTTON
    QPushButton buttonhelp("Help",&frame);
    buttonhelp.setGeometry(1536,0,192,120);

    QObject::connect(&buttonhelp,&QPushButton::clicked,[&](){
    QFrame *helpframe =  new QLabel("1st button adds elements\n3rd button removes all\n2nd button delete elements");
    helpframe->setGeometry(0,0,200,200);
    helpframe->show();
    });

    QPushButton buttonexit("Exit",&frame);
    buttonexit.setGeometry(1728,0,192,120);
    QObject::connect(&buttonexit,&QPushButton::clicked,[&](){
        app.exit();
    });


    QPushButton buttondelete("Delete", &frame);
    buttondelete.setGeometry(384,0,384,120);
    QObject::connect(&buttondelete,&QPushButton::clicked,[&](){
        flag++;
        if(flag == 1){
    QObject::connect(list, &QListWidget::itemClicked, [&](QListWidgetItem *item){
            delete item;
        });
        }
        else if(flag == 2){
            flag = 0;
            QObject::disconnect(list, &QListWidget::itemClicked,0,0);
        }
    });

    QPushButton buttontar("TAR", &frame);
    buttontar.setGeometry(1152,0,384,120);


    frame.resize(1920,1080);
    frame.move(0,0);



    window.showFullScreen();


    return app.exec();
}
