#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include <QObject>
#include <QMainWindow>
#include <QCloseEvent>
#include <QPushButton>
#include <QProgressBar>
#include <QLabel>
#include <QGridLayout>
#include <QDialogButtonBox>

#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>

#include <QJsonDocument>
#include <QJsonObject>

#include <QThread>

#include <vector>

class GameWindow;

#include "introscreenwidget.h"
#include "presentationwidget.h"
#include "cinematicwidget.h"
#include "maingamewidget.h"
#include "serverworker.h"

enum Screens
{
    IntroScreen,
    PresentationScreen,
    CinematicScreen,
    GameScreen,
};

class GameWindow : public QMainWindow
{
    Q_OBJECT
public:
    GameWindow(QApplication* application, QWidget *parent = nullptr);
    void GotToScreen(Screens nextScreen);
    QDialogButtonBox* BottomBox;

    ServerWorker* ServerWorker() { return this->serverWorker; };

public slots:
private :
    void SetupUi();
    QApplication* qApplication = nullptr;

    QLayout* mainLayout = nullptr;
    QLayout* mainHorizontalLayout = nullptr;
    QWidget* mainWidget = nullptr;

    QWidget* CurrentDisplayedScreen = nullptr;
    class::ServerWorker* serverWorker = nullptr;
};
#endif // GAMEWINDOW_H
