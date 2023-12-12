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
#include <QIcon>
#include <QCoreApplication>
#include <QFile>
#include <QProgressBar>
#include <QTimer>
using namespace std;
string special = "";
qint64 bytesbts = 0;
void customprogress(size_t bytesRead,size_t filesize){
    bytesbts = bytesbts + static_cast<qint64>(bytesRead);
    qDebug()<< bytesRead;
}

int main(int argc, char *argv[])
{
//QCoreApplication::setAttribute(Qt::AA_X11InitThreads);
    int flag = 0;
    qint64 totalsum = 0;
    qint64 unsum = 0;
    setenv("QT_X11_NO_MITSHM","1",1);

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
   QString corepath =QCoreApplication::applicationDirPath();
    corepath.resize(corepath.length() - 29);
    //ADDBUTTON

    QPushButton buttonadd( &frame);
    QPixmap pixmap1(corepath + "icons/addicon.jpg");
    qDebug()<< corepath + "icons/addicon.jpg";
    QIcon icon(pixmap1);
    buttonadd.setIcon(icon);
    buttonadd.setIconSize(QSize(107,107));
    buttonadd.setToolTip("Add File");
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
        QFile* file = new QFile(filename);
        totalsum = totalsum + file->size();
        file->close();
        count++;
        list->show();
    });

    //CLEARBUTTON
    QPushButton buttonclear(&frame);
    QPixmap pixmap3(corepath + "icons/clearicon.jpg");
    qDebug()<< corepath + "icons/deleteicon.jpg";
    QIcon icon3(pixmap3);
    buttonclear.setIcon(icon3);
    buttonclear.setIconSize(QSize(107,107));
    buttonclear.setToolTip("Clear all");
    buttonclear.setGeometry(214,0,107,107);

    QObject::connect(&buttonclear,&QPushButton::clicked,[&](){
    list->clear();
    });

    //UNtar
    QPushButton buttonuntar(&frame);
    QPixmap pixmap6(corepath + "icons/untaricon.jpg");
   // qDebug()<< corepath + "icons/deleteicon.jpg";
    QIcon icon6(pixmap6);
    buttonuntar.setIcon(icon6);
    buttonuntar.setIconSize(QSize(107,107));
    buttonuntar.setToolTip("Untar Files");
    buttonuntar.setGeometry(428,0,107,107);

    QObject::connect(&buttonuntar,&QPushButton::clicked,[&](){
        QString archname1 = QFileDialog::getOpenFileName(
                    nullptr,
                    "Open file",
                    path,
                    "ctar files (*.ctar*)"
                    );
        if(archname1 != NULL){
           // cryptotar target;
            QFileDialog dirpath;
           // dirpath.setAcceptMode(QFileDialog::AcceptOpen);
            setenv("QT_X11_NO_MITSHM","1",1);

            QString targetpath = dirpath.getExistingDirectory(nullptr,"Choose Dir",QDir::homePath());

            if(targetpath != NULL){
                targetpath += "/";
            qDebug() << targetpath;
            cryptotar tarEx;
            QProgressBar bar;
            bar.setRange(0,archname1.size());
                        bar.setValue(bytesbts);
                        bar.show();
            tarEx.globalProgressCallback = customprogress;
            tarEx.unpackTar(archname1.toStdString(), targetpath.toStdString());



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

    QPushButton buttonexit(&frame);
    QPixmap pixmap7(corepath + "icons/exiticon.jpg");
   // qDebug()<< corepath + "icons/deleteicon.jpg";
    QIcon icon7(pixmap7);
    buttonexit.setIcon(icon7);
    buttonexit.setIconSize(QSize(107,107));
    buttonexit.setToolTip("Exit");
    buttonexit.setGeometry(535,0,107,107);
    QObject::connect(&buttonexit,&QPushButton::clicked,[&](){
        app.exit();
    });


    QPushButton buttondelete( &frame);
    QPixmap pixmap2(corepath + "icons/deleteicon.jpg");
    qDebug()<< corepath + "icons/deleteicon.jpg";
    QIcon icon2(pixmap2);
    buttondelete.setIcon(icon2);
    buttondelete.setIconSize(QSize(107,107));
    buttondelete.setToolTip("Delete File");
    buttondelete.setGeometry(107,0,107,107);
    QObject::connect(&buttondelete,&QPushButton::clicked,[&](){
        flag++;
        if(flag == 1){
            buttondelete.setStyleSheet("background-color: rgb(180,180,180)");
    QObject::connect(list, &QListWidget::itemClicked, [&](QListWidgetItem *item){
        QString filenamedelete = item->text();
        QFile* filedelete = new QFile(filenamedelete);
        totalsum = totalsum - filedelete->size();
        filedelete->close();
            delete item;
        });
        }
        else if(flag == 2){
            buttondelete.setStyleSheet("background-color: rgb(248,233,215)");
            flag = 0;
            QObject::disconnect(list, &QListWidget::itemClicked,0,0);
        }
    });

    QPushButton buttontar(&frame);
    QPixmap pixmap5(corepath + "icons/taricon.jpg");
    qDebug()<< corepath + "icons/deleteicon.jpg";
    QIcon icon5(pixmap5);
    buttontar.setIcon(icon5);
    buttontar.setIconSize(QSize(107,107));
    buttontar.setToolTip("Tar Files");
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
