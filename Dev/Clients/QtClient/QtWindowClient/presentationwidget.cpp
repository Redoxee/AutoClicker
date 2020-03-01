#include "presentationwidget.h"

PresentationWidget::PresentationWidget(QWidget* parent, GameWindow* gameWindow) : QWidget(parent)
{
    this->gameWindow = gameWindow;

    this->SetupUI();

    this->checkerThread = new QThread();
    this->coreChecker = new CoreChecker(this->checkerThread);
    connect(this->coreChecker, SIGNAL(Reply()), this, SLOT(CoreCheckerReply()));
    this->checkerThread->start();

    this->bottomButton->hide();
}

void PresentationWidget::SetupUI()
{
    this->bottomButton = new QPushButton(this);
    this->bottomButton->setText("Begin installation");
    this->gameWindow->BottomBox->addButton(this->bottomButton, QDialogButtonBox::ButtonRole::YesRole);

    connect(this->bottomButton, SIGNAL(clicked()), this, SLOT(StartButtonClicked()));
}

void PresentationWidget::StartButtonClicked()
{
    this->gameWindow->BottomBox->removeButton(this->bottomButton);
    delete this->bottomButton;

    this->gameWindow->GotToScreen(Screens::CinematicScreen);
}

void PresentationWidget::CoreCheckerReply()
{
    this->bottomButton->show();
    if(this->coreChecker->CurrentState() != CoreCheckerState::Sucess)
    {
        this->bottomButton->setDisabled(true);
    }
}
