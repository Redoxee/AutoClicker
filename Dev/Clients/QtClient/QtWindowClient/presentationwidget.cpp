#include "presentationwidget.h"

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
        qDebug() << "No server found";
        connect(this, &PresentationWidget::OrderSignal, gameWindow->ServerWorker(), [=](){gameWindow->ServerWorker()->RequestOrder(ServerWorker::OrderStartNewServer);});
        emit this->OrderSignal();
    }
    else
    {
        qDebug() << "Waiting for response";
        connect(gameWindow->ServerWorker(), &ServerWorker::InitialServerResponse, this, &PresentationWidget::serverWorkerReply);
    }
}

void PresentationWidget::SetupUI()
{
    this->bottomButton = new QPushButton(this);
    this->bottomButton->setText("Begin installation");
    this->gameWindow->BottomBox->addButton(this->bottomButton, QDialogButtonBox::ButtonRole::YesRole);
    this->bottomButton->hide();

    connect(this->bottomButton, SIGNAL(clicked()), this, SLOT(StartButtonClicked()));
}

void PresentationWidget::StartButtonClicked()
{
    this->gameWindow->BottomBox->removeButton(this->bottomButton);
    delete this->bottomButton;

    this->gameWindow->GotToScreen(Screens::CinematicScreen);
}


void PresentationWidget::serverWorkerReply()
{
    // This might not work with thread, need lock of some kind.
    ServerWorker::State currentState = gameWindow->ServerWorker()->CurrentState();
    if(currentState == ServerWorker::State::WaitingForGame)
    {
        this->serverReady();
    }
    else if(currentState == ServerWorker::NoServerFound)
    {
        connect(this, &PresentationWidget::OrderSignal, gameWindow->ServerWorker(), [=](){gameWindow->ServerWorker()->RequestOrder(ServerWorker::OrderStartNewServer);});
        emit this->OrderSignal();
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
