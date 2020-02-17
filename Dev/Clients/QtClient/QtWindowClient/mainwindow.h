#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QObject>
#include <QMainWindow>
#include <QCloseEvent>
#include <QPushButton>
#include <QProgressBar>
#include <QLabel>
#include <QGridLayout>

#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>

#include <QJsonDocument>
#include <QJsonObject>

#include <QThread>

#include <vector>

#include "introscreenwidget.h"
#include "maingamewidget.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QApplication* application, QWidget *parent = nullptr);

public slots:
    void StartPressed();

private :
    void SetupUi();
    QApplication* qApplication;

    QLayout* mainLayout;
    QLayout* mainHorizontalLayout;
    QWidget* mainWidget;


    IntroScreenWidget* introScreenWidget;
    MainGameWidget* mainGameWidget;
};
#endif // MAINWINDOW_H
