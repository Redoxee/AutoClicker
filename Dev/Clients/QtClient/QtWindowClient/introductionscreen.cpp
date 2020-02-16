#include "introductionscreen.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QWidget>
#include <QPixmap>
#include <QSpacerItem>
#include <QDialogButtonBox>
#include "autoclickerconfig.h"
#include "mainwindow.h"
#include "introanimationwindow.h"

IntroductionScreen::IntroductionScreen(QApplication* application, QWidget *parent)
    : QMainWindow(parent)
{
    this->qApplication = application;
    this->SetupUi();
}

void IntroductionScreen::SetupUi()
{
    this->setWindowTitle("Shitty Wizzard Installer");
    this->setFixedSize(AutoClicker::WindowWidth, AutoClicker::WindowHeight);

    QWidget* centralWidget = new QWidget(this);
    this->setCentralWidget(centralWidget);

    this->mainLayout = new QVBoxLayout(centralWidget);
    this->mainHorizontalLayout = new QHBoxLayout();

    QPixmap image("Ressources/Gradient.png");
    QLabel* imageHolder = new QLabel(centralWidget);
    imageHolder->setPixmap(image);
    imageHolder->setStyleSheet("background: red");
    imageHolder->setScaledContents(true);
    imageHolder->setMaximumWidth(200);
    this->mainHorizontalLayout->addWidget(imageHolder);

    this->introScreenWidget = new IntroScreenWidget(this);
    this->mainHorizontalLayout->addWidget(this->introScreenWidget);

    this->mainLayout->addItem(this->mainHorizontalLayout);

    QPushButton* startButton = new QPushButton(centralWidget);
    startButton->setText("Start Installation!");
    QDialogButtonBox* buttonBox = new QDialogButtonBox();
    buttonBox->addButton(startButton, QDialogButtonBox::ButtonRole::YesRole);

    this->mainLayout->addWidget(buttonBox);

    QObject::connect(startButton, &QPushButton::clicked, this, &IntroductionScreen::StartPressed);
}

void IntroductionScreen::StartPressed()
{
    this->mainHorizontalLayout->removeWidget(this->introScreenWidget);
    delete this->introScreenWidget;
}
