#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QPushButton>
#include <QLabel>

#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>

#include <QJsonDocument>
#include <QJsonObject>

#include <QThread>

#include "refresherworker.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void handleClick();
    void handleHttpRequest(QNetworkReply *reply);

    void refreshData(int data);

private:
    Ui::MainWindow *ui;

    QNetworkAccessManager *manager;
    QNetworkRequest request;

    QLabel* scoreValueLabel;
    QLabel* tickValueLabel;

    RefresherWorker* refreshWorker;
    QThread* workerThread;
};
#endif // MAINWINDOW_H
