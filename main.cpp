#include "cryptotarlib.hpp"
#include <QApplication>
#include <QPushButton>
#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QFrame>
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
#include <QMessageBox>
#include <QLineEdit>
#include <QInputDialog>
#include <string>
using namespace std;
string special = "";


void customProgress(size_t bytesRead,size_t fileSize){

    special = "Bytes read: " + std::to_string(bytesRead);
}
int main(int argc, char *argv[])
{

    int flag = 0;

	QApplication app(argc, argv);
    QWidget window;
    window.setFixedSize(642,540);
    QString homed = "/home";
    QString name = qgetenv("USER");
    QString path = homed + "/" + name + "/";
    QString filenames;
    QFrame frame(&window);
    frame.setStyleSheet("background-color: rgb(255,255,255)");
    QListWidget *list = new QListWidget(&frame);
    list->setGeometry(0,108,642,432);
    QFrame hline(&frame);
    hline.setGeometry(0,107,642,1);
    hline.setStyleSheet("background-color: rgb(0,0,0)");
    int count = 0;

    //ADDBUTTON

    QPushButton buttonadd("Add file", &frame);
    buttonadd.setGeometry(0,0,107,107);

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
    buttonclear.setGeometry(214,0,107,107);

    QObject::connect(&buttonclear,&QPushButton::clicked,[&](){
    list->clear();
    });

    //UNtar
    QPushButton buttonuntar("UnTAR",&frame);
    buttonuntar.setGeometry(428,0,107,107);

    QObject::connect(&buttonuntar,&QPushButton::clicked,[&](){
        QString archname1 = QFileDialog::getOpenFileName(
                    nullptr,
                    "Open File",
                    path,
                    "cTAR files (*.ctar*)"
                    );
        if(archname1 != NULL){
           // cryptotar target;
            QString targetpath = QFileDialog::getExistingDirectory(nullptr,"Choose Dir",QDir::homePath());

            if(targetpath != NULL){
                targetpath += "/";
            qDebug() << targetpath;
            cryptotar tarEx(archname1.toStdString(), targetpath.toStdString());

            QMessageBox msgCom;
            msgCom.setText("Untared!");
            msgCom.addButton(QMessageBox::Ok);
            msgCom.setDefaultButton(QMessageBox::Ok);
            msgCom.setFixedWidth(900);
            msgCom.exec();
            list->clear();
            }
            else{
            QMessageBox msgError;
            msgError.setText("No path!");
            msgError.addButton(QMessageBox::Ok);
            msgError.setDefaultButton(QMessageBox::Ok);
            msgError.setFixedWidth(300);
            msgError.exec();
            }
        }
        else{
            QMessageBox msgError;
            msgError.setText("No archieve name!");
            msgError.addButton(QMessageBox::Ok);
            msgError.setDefaultButton(QMessageBox::Ok);
            msgError.setFixedWidth(300);
            msgError.exec();

        }


    });

    QPushButton buttonexit("Exit",&frame);
    buttonexit.setGeometry(535,0,107,107);
    QObject::connect(&buttonexit,&QPushButton::clicked,[&](){
        app.exit();
    });


    QPushButton buttondelete("Delete", &frame);
    buttondelete.setGeometry(107,0,107,107);
    QObject::connect(&buttondelete,&QPushButton::clicked,[&](){
        flag++;
        if(flag == 1){
            buttondelete.setStyleSheet("background-color: rgb(180,180,180)");
    QObject::connect(list, &QListWidget::itemClicked, [&](QListWidgetItem *item){
            delete item;
        });
        }
        else if(flag == 2){
            buttondelete.setStyleSheet("background-color: rgb(248,233,215)");
            flag = 0;
            QObject::disconnect(list, &QListWidget::itemClicked,0,0);
        }
    });

    QPushButton buttontar("TAR", &frame);
    buttontar.setGeometry(321,0,107,107);
    QObject::connect(&buttontar,&QPushButton::clicked,[&](){
        if(list->count() == 0){
            QMessageBox msgError;
            msgError.setText("Nothing to archive");
            msgError.addButton(QMessageBox::Ok);
            msgError.setDefaultButton(QMessageBox::Ok);
            msgError.setFixedWidth(300);
            msgError.exec();

        }
        else{
        std::vector<std::string> paths;
        QStringList stringList;
       for(int i =0; i< list->count();i++){
           stringList << list->item(i)->text();
       }

      foreach(const QString &str, stringList){
          paths.push_back(str.toStdString());

      }

      QString archname = QFileDialog::getExistingDirectory(nullptr,"Choose Dir",QDir::homePath());
      QString arname = QInputDialog::getText(nullptr, "Enter the name of tar archieve", "Archname:");

      cryptotar tar(archname.toStdString()+"/"+arname.toStdString().append(".ctar"), paths);

        QMessageBox msgCom;
        msgCom.setText("Tar created!");
        msgCom.addButton(QMessageBox::Ok);
        msgCom.setDefaultButton(QMessageBox::Ok);
        msgCom.setFixedWidth(300);
        msgCom.exec();
        list->clear();

       }


    });


    frame.resize(642,540);
    frame.move(0,0);



    window.show();


    return app.exec();
}
