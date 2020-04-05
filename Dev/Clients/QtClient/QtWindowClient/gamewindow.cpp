#include "gamewindow.h"

GameWindow::GameWindow(QApplication* application, QWidget *parent)
    : QMainWindow(parent)
{
    this->qApplication = application;
    this->SetupUi();

    this->serverWorker = new class::ServerWorker(application);

    this->GotToScreen(Screens::IntroScreen);
}

void GameWindow::SetupUi()
{
    this->setWindowTitle("Shitty Wizzard Installer");
    this->setFixedSize(AutoClicker::WindowWidth, AutoClicker::WindowHeight);

    this->mainWidget = new QWidget(this);
    this->setCentralWidget(this->mainWidget);

    this->mainLayout = new QVBoxLayout(this->mainWidget);
    this->mainHorizontalLayout = new QHBoxLayout();

    this->leftWidget = new QWidget(this);
    this->leftWidget->setObjectName("LeftWidget");
    this->leftWidget->setMaximumWidth(200);
    this->leftWidget->setMinimumWidth(200);
    this->leftWidget->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Expanding);
    this->leftWidget->setStyleSheet("QWidget#LeftWidget {border-image: url(Ressources/Gradient.png)} .QLabel {color : white}");

    this->LeftLayout = new QVBoxLayout(this->leftWidget);
    this->LeftLayout->setParent(this->leftWidget);

    this->mainHorizontalLayout->addWidget(this->leftWidget);
    this->mainLayout->addItem(this->mainHorizontalLayout);

    this->BottomBox = new QDialogButtonBox();
    this->mainLayout->addWidget(this->BottomBox);
}

void GameWindow::GotToScreen(Screens nextScreen)
{
    if(this->CurrentDisplayedScreen != nullptr)
    {
        this->CurrentDisplayedScreen->hide();
        this->mainHorizontalLayout->removeWidget(this->CurrentDisplayedScreen);
        delete this->CurrentDisplayedScreen;
    }

    switch (nextScreen) {
    case IntroScreen:
        this->CurrentDisplayedScreen = new IntroScreenWidget(this->mainWidget, this);
        break;
    case PresentationScreen:
        this->CurrentDisplayedScreen = new PresentationWidget(this->mainWidget, this);
        break;
    case CinematicScreen:
        this->CurrentDisplayedScreen = new CinematicWidget(this, this);
        break;
    case GameScreen:
        this->CurrentDisplayedScreen = new MainGameWidget(this);
        break;
    }

    this->mainHorizontalLayout->addWidget(this->CurrentDisplayedScreen);
    this->CurrentDisplayedScreen->show();
}

