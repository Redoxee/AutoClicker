#include "presentationwidget.h"

PresentationWidget::PresentationWidget(QWidget* parent, GameWindow* gameWindow) : QWidget(parent)
{
    this->gameWindow = gameWindow;

    this->SetupUI();

    QString coreGameProcessPath = QDir::currentPath() + "/CoreGame/ClickerServer.exe";
    qDebug() << coreGameProcessPath;
    if(QProcess::startDetached(coreGameProcessPath))
    {
        qDebug() << "Server process started.";
    }
    else
    {
        qDebug() << "Server process failed!";
    }
}

void PresentationWidget::SetupUI()
{
    this->bottomButton = new QPushButton(this);
    this->bottomButton->setText("Begin installation");
    this->gameWindow->BottomBox->addButton(this->bottomButton, QDialogButtonBox::ButtonRole::YesRole);

    connect(this->bottomButton, SIGNAL(clicked()), this, SLOT(startButtonClicked()));
}

void PresentationWidget::startButtonClicked()
{
    this->gameWindow->BottomBox->removeButton(this->bottomButton);
    delete this->bottomButton;

    this->gameWindow->GotToScreen(Screens::CinematicScreen);
}
