#include "gamewindow.h"

#include "autoclickerconfig.h"
#include "introscreenwidget.h"
#include "presentationwidget.h"
#include "cinematicwidget.h"
#include "maingamewidget.h"
#include "serverworker.h"
#include "endgamewidget.h"

GameWindow::GameWindow(QApplication* application, QWidget *parent)
    : QMainWindow(parent)
{
    this->qApplication = application;
    this->Settings = new GameSettings();

    this->Settings->SkipUpgradeReveal = true;
    this->Settings->SkipIntroCinematic= true;

    this->SetupUi();

    this->serverWorker = new class::ServerWorker(application);

    this->GotToScreen(Screens::IntroScreen);
//    this->GotToScreen(Screens::CinematicScreen);
//    this->GotToScreen(Screens::GameScreen);
//    this->GotToScreen(Screens::EndGameScreen);
}

GameWindow::~GameWindow()
{
    delete this->Settings;
}

void GameWindow::SetupUi()
{
    this->setWindowTitle("Shitty Wizard Installer");
    this->setFixedSize(500, 600);

    this->mainWidget = new QWidget(this);
    this->mainWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    this->setCentralWidget(this->mainWidget);

    this->mainLayout = new QVBoxLayout(this->mainWidget);
    this->mainLayout->setMargin(2);

    this->mainHorizontalLayout = new QHBoxLayout();
    this->mainHorizontalLayout->setSpacing(2);
    this->mainHorizontalLayout->setMargin(0);

    this->leftWidget = new QWidget(this);
    this->leftWidget->setObjectName("LeftWidget");
    this->leftWidget->setMaximumWidth(200);
    this->leftWidget->setMinimumWidth(200);
    this->leftWidget->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Expanding);
    this->leftWidget->setStyleSheet("QWidget#LeftWidget {border-image: url(Ressources/Gradient.png)} .QLabel {color : white}");

    this->LeftGLayout = new QGridLayout(this->leftWidget);
    this->LeftGLayout->setMargin(0);
    this->LeftLayout = new QVBoxLayout();
    this->LeftGLayout->addLayout(this->LeftLayout, 0, 0);
    this->LeftLayout->setMargin(0);
    this->logoHolder = new QLabel(this);
    QPixmap* logo = new QPixmap("Ressources/SWI_logo.png");
    this->logoHolder->setPixmap(*logo);
    this->LeftLayout->addWidget(this->logoHolder);
    QSpacerItem* spacer = new QSpacerItem(0,0,QSizePolicy::Fixed, QSizePolicy::Expanding);
    this->LeftLayout->addItem(spacer);

    this->mainHorizontalLayout->addWidget(this->leftWidget);
    this->mainLayout->addItem(this->mainHorizontalLayout);

    this->BottomBox = new QHBoxLayout();
    this->mainLayout->addLayout(this->BottomBox);

    QLabel* contactLabel = new QLabel();
    contactLabel->setText("contact : antonmakesgames@gmail.com");
    contactLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    contactLabel->setTextInteractionFlags(Qt::TextSelectableByMouse);
    contactLabel->setAlignment(Qt::AlignLeft);
    this->BottomBox->addWidget(contactLabel);
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
    case Screens::IntroScreen:
        this->CurrentDisplayedScreen = new IntroScreenWidget(this->mainWidget, this);
        break;
    case Screens::PresentationScreen:
        this->CurrentDisplayedScreen = new PresentationWidget(this->mainWidget, this);
        break;
    case Screens::CinematicScreen:
        this->CurrentDisplayedScreen = new CinematicWidget(this, this);
        break;
    case Screens::GameScreen:
        this->CurrentDisplayedScreen = new MainGameWidget(this);
        break;
    case Screens::EndGameScreen:
        this->CurrentDisplayedScreen = new EndGameWidget(this);
    }

    this->mainHorizontalLayout->addWidget(this->CurrentDisplayedScreen);
    this->CurrentDisplayedScreen->show();
}

