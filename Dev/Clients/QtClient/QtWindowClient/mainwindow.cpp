#include "mainwindow.h"

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
#include "gamecinematic.h"
#include "windowshakeanimation.h"

MainWindow::MainWindow(QApplication* application, QWidget *parent)
    : QMainWindow(parent)
{
    this->qApplication = application;
    this->SetupUi();

    this->introScreenWidget = new IntroScreenWidget(this->mainWidget, this->bottomBox);
    this->mainHorizontalLayout->addWidget(this->introScreenWidget);
}

void MainWindow::SetupUi()
{
    this->setWindowTitle("Shitty Wizzard Installer");
    this->setFixedSize(AutoClicker::WindowWidth, AutoClicker::WindowHeight);

    this->mainWidget = new QWidget(this);
    this->setCentralWidget(this->mainWidget);

    this->mainLayout = new QVBoxLayout(this->mainWidget);
    this->mainHorizontalLayout = new QHBoxLayout();

    QPixmap image("Ressources/Gradient.png");
    QLabel* imageHolder = new QLabel(this->mainWidget);
    imageHolder->setPixmap(image);
    imageHolder->setStyleSheet("background: red");
    imageHolder->setScaledContents(true);
    imageHolder->setMaximumWidth(200);
    imageHolder->setMinimumWidth(200);
    this->mainHorizontalLayout->addWidget(imageHolder);

    this->mainLayout->addItem(this->mainHorizontalLayout);

    this->bottomBox = new QDialogButtonBox();

    this->mainLayout->addWidget(this->bottomBox);

//    QObject::connect(startButton, &QPushButton::clicked, this, &MainWindow::StartPressed);
}

void MainWindow::StartPressed()
{
    this->mainHorizontalLayout->removeWidget(this->introScreenWidget);
    this->introScreenWidget->hide();
    delete this->introScreenWidget;

/*
    this->mainGameWidget = new MainGameWidget(this->mainWidget, this->qApplication);
    this->mainHorizontalLayout->addWidget(this->mainGameWidget);
    this->mainGameWidget->show();
*/

    this->gameCinematic = new GameCinematic(this->mainWidget, this);
    this->mainHorizontalLayout->addWidget(this->gameCinematic);
    this->gameCinematic->show();
    this->gameCinematic->StartCinematic();
}

