#include "introscreenwidget.h"

IntroScreenWidget::IntroScreenWidget(QWidget* parent, GameWindow* mainWindow) : QWidget(parent)
{
    this->mainWindow = mainWindow;
    this->SetupUI();
}

void IntroScreenWidget::SetupUI()
{

    QVBoxLayout* vBoxLayout = new QVBoxLayout(this);
    vBoxLayout->setMargin(15);

    QLabel* titleLabel = new QLabel("Welcome to the Shitty Wizzard Installer(SWI).", this);
    titleLabel->setAlignment(Qt::AlignLeft);
    vBoxLayout->addWidget(titleLabel);

    QLabel* subTitleLabel = new QLabel("We will guide you through the installation of DotGobbler.exe", this);
    vBoxLayout->addWidget(subTitleLabel);

    this->startButton = new QPushButton(this);
    this->startButton->setText("Next");
    this->mainWindow->BottomBox->addButton(startButton, QDialogButtonBox::ButtonRole::YesRole);

    connect(this->startButton, SIGNAL(clicked()), this, SLOT(StartButtonPressed()));
}

void IntroScreenWidget::StartButtonPressed()
{
    this->mainWindow->BottomBox->removeButton(this->startButton);
    delete this->startButton;

    this->mainWindow->GotToScreen(Screens::PresentationScreen);
}
