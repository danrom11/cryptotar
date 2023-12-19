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
#include <QThread>
#include <QFileInfo>


qint64 bytesbts = 0;
using namespace std;
int c = -1;
int progress = 0;
int progress2 = 0;
qint64 filesize;
qint64 alreadyread = 0;

qint64 alreadyread2 = 0;
long prom = 0;
int progressprom = 0;;
qint64 archsize1;
double currentprogress;
qint64 bytestotar = 0;
int progressspec = 0;
QProgressBar *globalbar;
QProgressBar *globalbartar;
QString unarchname;
double bytesprom = 0;
double bytesspec = 0;
double bytesprom2 = 0;
double bytesspec2 = 0;
string cryptokey;
QString cryptopath;
string cryptopathstr;
string cryptokeystr;

void customprogress(size_t bytesRead,size_t fileSize){
    alreadyread = bytesRead;
    if(alreadyread != bytesprom){
        bytesspec = bytesspec + 4096;
    }
    bytesprom = alreadyread;
    progress = bytesspec /(static_cast<double>(archsize1)) * 100;
    if(progress == 99){
        globalbar->setVisible(false);
        alreadyread = 0;

    }
        globalbar->setValue(progress);

}

void customprogress2(size_t bytesRead,size_t fileSize){
    alreadyread2 = bytesRead;
    if(alreadyread2 != bytesprom2){
        bytesspec2 = bytesspec2 + 4096;
    }
    bytesprom2 = alreadyread2;
    progress2 = bytesspec2 /(static_cast<double>(bytestotar)) * 100;
    if(progress2 == 100){
        globalbartar->setVisible(false);
        alreadyread = 0;

    }
        globalbartar->setValue(progress2);
}

class Worker : public QThread{

//Q_OBJECT
public:
    Worker(const string& arg1,const string& arg2)
        : m_arg1(arg1),m_arg2(arg2)
    {}

    void run() {
       cryptotar TarEx;

       if(!cryptokeystr.empty()){
           TarEx.setCryptoModule(cryptopathstr, cryptokeystr, cryptokeystr.length());
       }
       TarEx.globalProgressCallback = customprogress;
            TarEx.unpackTar(m_arg1, m_arg2);
    }
private:
string m_arg1;
string m_arg2;

};



class Worker2 : public QThread{

//Q_OBJECT
public:
    Worker2(const string& arg1,const std::vector<std::string>& arg2)
        : m_arg1(arg1),m_arg2(arg2)
    {}

    void run() {
         cryptotar mytar(m_arg1);

         if(!cryptokeystr.empty()){
             mytar.setCryptoModule(cryptopathstr, cryptokeystr, cryptokeystr.length());
         }
         mytar.globalProgressCallback = customprogress2;
         for(string path : m_arg2){
             mytar.addPath(path);
         }
            mytar.closeTar();
    }
private:
string m_arg1;
std::vector<std::string> m_arg2;

};




string special = "";

int specbytes = 0;


int main(int argc, char *argv[])
{
    int flag = 0;
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


   //ADDBUTTON

    QPushButton buttonadd( &frame);
    QPixmap pixmap1(corepath + "/icons/addicon.jpg");
    QIcon icon(pixmap1);
    buttonadd.setIcon(icon);
    buttonadd.setIconSize(QSize(107,107));
    buttonadd.setToolTip("Add File");
    buttonadd.setGeometry(0,0,107,107);
    QObject::connect(&buttonadd,&QPushButton::clicked,[&](){

        QString filename = QFileDialog::getOpenFileName(
                    nullptr,
                    "Open File",
                    path,
                    "All files (*.*)"
                    );
        list->addItem(filename);
        QFile* file = new QFile(filename);
        bytestotar = bytestotar + file->size();
        file->close();
        count++;
        list->show();
    });

    //CLEARBUTTON
    QPushButton buttonclear(&frame);
    QPixmap pixmap3(corepath + "/icons/clearicon.jpg");
    QIcon icon3(pixmap3);
    buttonclear.setIcon(icon3);
    buttonclear.setIconSize(QSize(107,107));
    buttonclear.setToolTip("Clear all");
    buttonclear.setGeometry(214,0,107,107);

    QObject::connect(&buttonclear,&QPushButton::clicked,[&](){
        bytestotar = 0;
    list->clear();
    });

    //UNtar
    QPushButton buttonuntar(&frame);
    QPixmap pixmap6(corepath + "/icons/untaricon.jpg");
   // qDebug()<< corepath + "icons/deleteicon.jpg";
    QIcon icon6(pixmap6);
    buttonuntar.setIcon(icon6);
    buttonuntar.setIconSize(QSize(107,107));
    buttonuntar.setToolTip("Untar Files");
    buttonuntar.setGeometry(428,0,107,107);
QFileDialog fdialog;
    QObject::connect(&buttonuntar,&QPushButton::clicked,[&](){
        QString archname1 = fdialog.getOpenFileName(
                    nullptr,
                    "Open file",
                    path,
                    "ctar files (*.ctar*)"
                    );
        if(archname1 != NULL){
            fdialog.close();
            QFileDialog dirpath;
            QString targetpath = dirpath.getExistingDirectory(nullptr,"Choose Dir",QDir::homePath());

            if(targetpath != NULL){
                targetpath += "/";
                dirpath.reject();
                string archnamestr = archname1.toStdString();
                string targetpathstr = targetpath.toStdString();

            unarchname = archname1;
            QFileInfo fileinf(unarchname);
            archsize1 = fileinf.size();
            QProgressBar bar;
            bar.setRange(0,100);
            bar.setValue(progress);
            globalbar = &bar;
            bar.show();
            cryptopath = QFileDialog::getOpenFileName(
                       nullptr,
                       "Choose crypto",
                       cryptopath,
                       "cryptomodule files (*.cryptomodule*)"
                       );
            if(cryptopath != NULL){
                cryptopathstr = cryptopath.toStdString();
                QString cryptokey = QInputDialog::getText(nullptr, "Enter key", "Key:");
                cryptokeystr = cryptokey.toStdString();

            }

            Worker* worker = new Worker(archnamestr,targetpathstr);
            worker->start();

            QMessageBox msgCom;
                            msgCom.setText("Untaring in progress!");
                            msgCom.setStandardButtons(QMessageBox::Ok);
                            msgCom.setButtonText(QMessageBox::Ok, "Stop");
                          //  msgCom.setFixedWidth(900);
                            msgCom.exec();


            list->clear();
             alreadyread = 0;
             app.exit();

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
    QPixmap pixmap7(corepath + "/icons/exiticon.jpg");
    QIcon icon7(pixmap7);
    buttonexit.setIcon(icon7);
    buttonexit.setIconSize(QSize(107,107));
    buttonexit.setToolTip("Exit");
    buttonexit.setGeometry(535,0,107,107);
    QObject::connect(&buttonexit,&QPushButton::clicked,[&](){
        app.exit();
    });


    QPushButton buttondelete( &frame);
    QPixmap pixmap2(corepath + "/icons/deleteicon.jpg");
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
        bytestotar = bytestotar - filedelete->size();
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
    QPixmap pixmap5(corepath + "/icons/taricon.jpg");
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
       cryptopath = QFileDialog::getOpenFileName(
                  nullptr,
                  "Choose crypto",
                  cryptopath,
                  "cryptomodule files (*.cryptomodule*)"
                  );
      QProgressBar bar2;
      bar2.setRange(0,100);
      bar2.setValue(progress);
      globalbartar = &bar2;
      bar2.show();

       string archnametar = archname.toStdString()+"/"+arname.toStdString().append(".ctar");

       if(cryptopath != NULL){
           cryptopathstr = cryptopath.toStdString();
           QString cryptokey = QInputDialog::getText(nullptr, "Enter key", "Key:");
           cryptokeystr = cryptokey.toStdString();

       }
      Worker2* workertar = new Worker2(archnametar, paths);
      workertar->start();

        QMessageBox msgCom;
        msgCom.setText("Taring in progress");
        msgCom.setStandardButtons(QMessageBox::Ok);
        msgCom.setButtonText(QMessageBox::Ok, "Stop");
        msgCom.exec();
        list->clear();
        app.exit();

       }


    });


    frame.resize(642,540);
    frame.move(0,0);



    window.show();


    return app.exec();
}

