#include "introscreenwidget.h"
#include "gamewindow.h"

IntroScreenWidget::IntroScreenWidget(QWidget* parent, GameWindow* mainWindow) : QWidget(parent)
{
    this->mainWindow = mainWindow;
    this->SetupUI();
}

void IntroScreenWidget::SetupUI()
{

    QVBoxLayout* vLayout = new QVBoxLayout(this);
    vLayout->setMargin(0);
    vLayout->setSpacing(0);

    vLayout->addSpacing(0);

    QLabel* titleLabel = new QLabel("Welcome to the Shitty Wizzard Installer.", this);
    titleLabel->setAlignment(Qt::AlignmentFlag::AlignLeft);
    titleLabel->setMargin(10);
    vLayout->addWidget(titleLabel);

    QSpacerItem* spacer = new QSpacerItem(0,0,QSizePolicy::Fixed, QSizePolicy::Expanding);
    vLayout->addSpacerItem(spacer);

    QLabel* subLabel = new QLabel("you are about to install :");
    subLabel->setMargin(10);
    vLayout->addWidget(subLabel);
    vLayout->addSpacing(0);

    QLabel* illusHolder = new QLabel(this);
    QPixmap* illus = new QPixmap("Ressources/DotGobbler.png", "png", Qt::ImageConversionFlag::AutoDither);
    illusHolder->setPixmap(*illus);
    illusHolder->setFixedWidth(294);
    illusHolder->setFixedHeight(450);
    illusHolder->setScaledContents(true);
    vLayout->addWidget(illusHolder);

    this->startButton = new QPushButton(this);
    this->startButton->setText("Next");
    this->mainWindow->BottomBox->addWidget(startButton);

    connect(this->startButton, SIGNAL(clicked()), this, SLOT(StartButtonPressed()));
}

void IntroScreenWidget::StartButtonPressed()
{
    this->mainWindow->BottomBox->removeWidget(this->startButton);
    delete this->startButton;

    this->mainWindow->GotToScreen(Screens::PresentationScreen);
}
