#include "QWidget"
#include "QMainWindow.h"
#include <QApplication>
#include <QFrame>
#include "QPushButton"
#include "QtGui"
#include "QListWidget"
#include "QFileDialog"
#include "iostream"
#include "QMessageBox"
#include "QFile"
#include "cryptotarlib.hpp"
#include "QInputDialog"
#include "string.h"

using namespace std;
int flag = 0;
qint64 totalsize = 0;
int countt = 0;
string pathtotar;
string paths;
int main(int argc, char *argv[])
{

    //presets for window
    QApplication a(argc, argv);
    QWidget window;
    window.show();
    window.setFixedSize(600,500);
    QFrame framemain(&window);
    framemain.setFixedSize(600,500);

    QPoint center;
    center.setX(0);
    center.setY(0);
    window.move(center);
    QListWidget listfiles(&framemain);
    listfiles.setGeometry(0,100,600,400);

    //button "ADD"
    QPushButton button1(&framemain);
    QString path;
    QString name1 = "Add";
    button1.setText(name1);
    button1.setGeometry(0,0,100,100);
    QObject::connect(&button1,&QPushButton::clicked,[&](){
        QString fileaddname = QFileDialog::getOpenFileName(
            nullptr,"Add file",path,"All files (*.*)");
        if(fileaddname.isEmpty() == true){
            QMessageBox emptyfile;
            emptyfile.setText("Nothing added!");
            emptyfile.setDefaultButton(QMessageBox::Ok);
            emptyfile.setFixedSize(400,100);
            emptyfile.exec();
            QObject::disconnect(&listfiles,&QListWidget::itemClicked,0,0);
        }
        else{
        QFile* filetoadd = new QFile(fileaddname);
        totalsize = totalsize + filetoadd->size();
        filetoadd->close();
        listfiles.addItem(fileaddname);
        countt++;
        }
        listfiles.show();

    });

    //button "DELETE"
    QPushButton button2(&framemain);
    QString name2 = "Delete";
    button2.setText(name2);
    button2.setGeometry(100,0,100,100);
    QObject::connect(&button2,&QPushButton::clicked,[&](){
        flag++;
        if(flag % 2 != 0){
            QMessageBox deleteon;
            deleteon.setText("Delete mod is on!");
            deleteon.setDefaultButton(QMessageBox::Ok);
            deleteon.setFixedSize(400,100);
            deleteon.exec();
            QObject::connect(&listfiles, &QListWidget::itemClicked, [&](QListWidgetItem *item){
                QString filenamedelete = item->text();
                QFile* filedelete = new QFile(filenamedelete);
                totalsize = totalsize - filedelete->size();
                filedelete->close();
                delete item;
                countt --;
            });
        }
            if(flag % 2 == 0){
                QMessageBox deleteoff;
                deleteoff.setText("Delete mod is off!");
                deleteoff.setDefaultButton(QMessageBox::Ok);
                deleteoff.setFixedSize(400,100);
                deleteoff.exec();
                QObject::disconnect(&listfiles,&QListWidget::itemClicked,0,0);

            }
    });

    //button "Clear"
    QPushButton button3(&framemain);
    QString name3 = "Clear";
    button3.setText(name3);
    button3.setGeometry(200,0,100,100);
    QObject::connect(&button3,&QPushButton::clicked,[&](){
        listfiles.clear();
        totalsize = 0;
        QMessageBox listcleared;
        listcleared.setText("List has been cleared");
        listcleared.setDefaultButton(QMessageBox::Ok);
        listcleared.setFixedSize(400,100);
        countt = 0;
        listcleared.exec();
    });

   //button "Tar"
    QPushButton button4(&framemain);
    QString name4 = "Tar";
    button4.setText(name4);
    button4.setGeometry(300,0,100,100);
     QObject::connect(&button4,&QPushButton::clicked,[&](){
        if(countt == 0){
            QMessageBox nta;
            nta.setText("Nothing to archive!");
            nta.setDefaultButton(QMessageBox::Ok);
            nta.setFixedSize(400,100);
            nta.exec();
        }
        else {
            QString tarpath = QFileDialog::getExistingDirectory(nullptr,"Choose Dir",QDir::homePath());
            QString tarname = QInputDialog::getText(nullptr, "Enter the name of tar archieve", "Archname:");
            string totaltar = tarpath.toStdString() + tarname.toStdString().append(".ctar");
            cryptotar myTar(totaltar);
            for(int i = 0; i < countt; i++){
                QListWidgetItem* specitem = listfiles.item(i);
                paths = specitem->text().toStdString();
                myTar.addPath(paths);
            }
            myTar.closeTar();
        }
    });

    //button "Untar"
    QPushButton button5(&framemain);
    QString name5 = "Untar";
    button5.setText(name5);
    button5.setGeometry(400,0,100,100);



    //buuton "Exit"
    QPushButton button6(&framemain);
    QString name6 = "Exit";
    button6.setText(name6);
    button6.setGeometry(500,0,100,100);
    QObject::connect(&button6,&QPushButton::clicked,[&](){
        a.exit();
    });

    framemain.show();
    return a.exec();
}
