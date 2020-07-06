#include "gamewindow.h"

#include <QStringBuilder>
#include <QFile>
#include <QTextStream>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonValue>

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

    QString val;
    QFile file;
    file.setFileName(QString::fromStdString(AutoClicker::InitFilePath()));
    bool loadingSuccessfull = true;
    if(file.exists())
    {
        file.open(QIODevice::ReadOnly | QIODevice::Text);
        val = file.readAll();
        file.close();

        QJsonDocument d = QJsonDocument::fromJson(val.toUtf8());
        QJsonObject root = d.object();

        if(root.contains(QString("SkipUpgradeReveal")))
        {
            this->Settings->SkipUpgradeReveal = root["SkipProgressiveReveal"].toBool();
        }

        if(root.contains(QString("SkipIntroCinematic")))
        {
            this->Settings->SkipIntroCinematic = root["SkipIntroCinematic"].toBool();
        }

        if(root.contains(QString("DontKillProcessOnClose")))
        {
            this->Settings->DontKillProcessOnClose = root["DontKillProcessOnClose"].toBool();
        }

        if(root.contains("LoadingQuips"))
        {
            this->Settings->LoadingQuips = root["LoadingQuips"].toObject();
        }
        else
        {
            loadingSuccessfull = false;
        }
    }
    else
    {
        loadingSuccessfull = false;
    }

    if(!loadingSuccessfull)
    {
        this->setFixedSize(500, 600);
        QLabel* errorLabel = new QLabel();
        errorLabel->setText(QString("Something whent wrong while loading the init file : ") % QString::fromStdString(AutoClicker::InitFilePath()));
        errorLabel->setTextInteractionFlags(Qt::TextSelectableByMouse);
        errorLabel->show();
        return;
    }

    this->SetupUi();

    this->serverWorker = new class::ServerWorker(application);

    this->GotToScreen(Screens::IntroScreen);
//    this->GotToScreen(Screens::CinematicScreen);
//    this->GotToScreen(Screens::GameScreen);
//    this->GotToScreen(Screens::EndGameScreen);
}

GameWindow::~GameWindow()
{
    delete this->serverWorker;
    this->serverWorker = nullptr;
    delete this->Settings;
    this->Settings = nullptr;
}

void GameWindow::SetupUi()
{
    this->setWindowTitle("SWInstaller");
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
    this->leftWidget->setStyleSheet("QWidget#LeftWidget {border-image: url(:/RcRessources/Gradient.png)} .QLabel {color : white}");

    this->LeftGLayout = new QGridLayout(this->leftWidget);
    this->LeftGLayout->setMargin(0);
    this->LeftLayout = new QVBoxLayout();
    this->LeftGLayout->addLayout(this->LeftLayout, 0, 0);
    this->LeftLayout->setMargin(0);
    this->logoHolder = new QLabel(this);
    QPixmap* logo = new QPixmap(":/RcRessources/SWI_Logo3.png");
    this->logoHolder->setPixmap(*logo);
    this->LeftLayout->addWidget(this->logoHolder);

    this->ProgressiveIllus = new QLabel*[3];
    QPixmap* hat1PixMap = new QPixmap(":/RcRessources/hat1.png");
    this->ProgressiveIllus[0] = new QLabel(this);
    this->ProgressiveIllus[0]->setPixmap(*hat1PixMap);
    this->ProgressiveIllus[0]->setAlignment(Qt::AlignLeft);

    QSizePolicy policy = this->ProgressiveIllus[0]->sizePolicy();
    policy.setHorizontalPolicy(QSizePolicy::Policy::Expanding);
    policy.setRetainSizeWhenHidden(true);
    this->ProgressiveIllus[0]->setSizePolicy(policy);
    this->LeftLayout->addWidget(this->ProgressiveIllus[0]);

    this->ProgressiveIllus[1] = new QLabel(this);
    QPixmap* hat2PixMap = new QPixmap(":/RcRessources/hat2.png");
    this->ProgressiveIllus[1]->setPixmap(*hat2PixMap);
    this->ProgressiveIllus[1]->setAlignment(Qt::AlignRight);

    policy = this->ProgressiveIllus[1]->sizePolicy();
    policy.setHorizontalPolicy(QSizePolicy::Policy::Expanding);
    policy.setRetainSizeWhenHidden(true);
    this->ProgressiveIllus[1]->setSizePolicy(policy);
    this->LeftLayout->addWidget(this->ProgressiveIllus[1]);

    this->ProgressiveIllus[2] = new QLabel(this);
    QPixmap* hat3PixMap = new QPixmap(":/RcRessources/hat3.png");
    this->ProgressiveIllus[2]->setPixmap(*hat3PixMap);
    this->ProgressiveIllus[2]->setAlignment(Qt::AlignCenter);

    policy = this->ProgressiveIllus[2]->sizePolicy();
    policy.setHorizontalPolicy(QSizePolicy::Policy::Expanding);
    policy.setRetainSizeWhenHidden(true);
    this->ProgressiveIllus[2]->setSizePolicy(policy);
    this->LeftLayout->addWidget(this->ProgressiveIllus[2]);

    this->ProgressiveIllus[0]->setVisible(false);
    this->ProgressiveIllus[1]->setVisible(false);
    this->ProgressiveIllus[2]->setVisible(false);

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

