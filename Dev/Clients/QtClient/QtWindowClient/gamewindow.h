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
class ServerWorker;

enum class Screens
{
    IntroScreen,
    PresentationScreen,
    CinematicScreen,
    GameScreen,
    EndGameScreen,
};

class GameWindow : public QMainWindow
{
    Q_OBJECT
public:
    GameWindow(QApplication* application, QWidget *parent = nullptr);
    void GotToScreen(Screens nextScreen);
    QDialogButtonBox* BottomBox;

    ServerWorker* ServerWorker() { return this->serverWorker; };
    QLayout* LeftLayout = nullptr;
    QGridLayout* LeftGLayout = nullptr;

    int currentFrame = -1;
public slots:
private :
    void SetupUi();
    QApplication* qApplication = nullptr;

    QLayout* mainLayout = nullptr;
    QLayout* mainHorizontalLayout = nullptr;
    QWidget* mainWidget = nullptr;

    QWidget* leftWidget = nullptr;

    QWidget* CurrentDisplayedScreen = nullptr;
    class::ServerWorker* serverWorker = nullptr;
};
#endif // GAMEWINDOW_H
