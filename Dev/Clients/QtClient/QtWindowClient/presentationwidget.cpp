#include "presentationwidget.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QTextBrowser>
#include <QFile>
#include <QTextStream>
#include <QStringList>
#include <QUrl>
#include <QCheckBox>

#include "gamewindow.h"
#include "serverworker.h"

PresentationWidget::PresentationWidget(QWidget* parent, GameWindow* gameWindow) : QWidget(parent)
{
    this->gameWindow = gameWindow;

    this->SetupUI();

    // This might not work with thread, need lock of some kind.
    ServerWorker::State currentState = gameWindow->ServerWorker()->CurrentState();
    if(currentState == ServerWorker::State::WaitingForGame)
    {
        qDebug() << "Server ready";
        this->serverReady();
    }
    else if(currentState == ServerWorker::NoServerFound)
    {
        connect(gameWindow->ServerWorker(), &ServerWorker::ServerStarted, this, &PresentationWidget::serverWorkerReply);
        gameWindow->ServerWorker()->RequestOrder(ServerWorker::OrderStartNewServer);
    }
    else
    {
        connect(gameWindow->ServerWorker(), &ServerWorker::InitialServerResponse, this, &PresentationWidget::serverWorkerReply);
        qDebug() << "Waiting for response";
    }
}

void PresentationWidget::SetupUI()
{
    this->layout = new QVBoxLayout(this);
    this->layout->setContentsMargins(QMargins(7, 5, 5, 7));

    QLabel* introLabel = new QLabel(this);
    introLabel->setText("You will soon starting the instalation");
    QFont font = introLabel->font();
    font.setBold(true);
    font.setPointSize(10);
    introLabel->setFont(font);
    this->layout->addWidget(introLabel);

    QStringList presentationList;
    QFile file("Ressources/Presentation.md");
    if(!file.open(QIODevice::ReadOnly)) {
        return;
    }

    QTextStream in(&file);

    while(!in.atEnd()) {
        QString line = in.readLine();
        presentationList << line;
    }

    file.close();

    QTextBrowser* textBrowser = new QTextBrowser(this);
    textBrowser->setMarkdown(presentationList.join("\n"));
    textBrowser->setOpenLinks(false);
    textBrowser->setOpenExternalLinks(false);
    this->layout->addWidget(textBrowser);

    QCheckBox* checkBox = new QCheckBox(this);
    checkBox->setText("I don't care.");
    checkBox->setToolTip("Me neither.");
    checkBox->setLayoutDirection(Qt::RightToLeft);
    this->layout->addWidget(checkBox);

    this->bottomButton = new QPushButton(this);
    this->bottomButton->setText("Begin installation");
    this->gameWindow->BottomBox->addWidget(this->bottomButton);
    this->bottomButton->hide();
    QSizePolicy policy = this->bottomButton->sizePolicy();
    policy.setRetainSizeWhenHidden(true);
    this->bottomButton->setSizePolicy(policy);

    connect(this->bottomButton, SIGNAL(clicked()), this, SLOT(StartButtonClicked()));
}

void PresentationWidget::StartButtonClicked()
{
    this->gameWindow->BottomBox->removeWidget(this->bottomButton);
    delete this->bottomButton;


    if(!GameSettings::Instance()->SkipIntroCinematic)
    {
        this->gameWindow->GotToScreen(Screens::CinematicScreen);
    }
    else
    {
        this->gameWindow->GotToScreen(Screens::GameScreen);
    }
}


void PresentationWidget::serverWorkerReply()
{
    // This might not work with thread, need lock of some kind.
    ServerWorker::State currentState = gameWindow->ServerWorker()->CurrentState();
    if(currentState == ServerWorker::State::WaitingForGame)
    {
        disconnect(gameWindow->ServerWorker(), &ServerWorker::ServerStarted, this, &PresentationWidget::serverWorkerReply);
        this->serverReady();
    }
    else if(currentState == ServerWorker::NoServerFound)
    {
        disconnect(gameWindow->ServerWorker(), &ServerWorker::InitialServerResponse, this, &PresentationWidget::serverWorkerReply);
        connect(gameWindow->ServerWorker(), &ServerWorker::ServerStarted, this, &PresentationWidget::serverWorkerReply);
        this->gameWindow->ServerWorker()->RequestOrder(ServerWorker::Order::OrderStartNewServer);
    }
    else
    {
        qDebug() << "Something's wrong.";
    }
}

void PresentationWidget::serverReady()
{
    this->bottomButton->show();
}
