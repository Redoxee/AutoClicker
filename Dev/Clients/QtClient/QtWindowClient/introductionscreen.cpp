#include "introductionscreen.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QWidget>
#include <QPixmap>

IntroductionScreen::IntroductionScreen(QApplication* application, QWidget *parent)
    : QMainWindow(parent)
{
    this->qApplication = application;
    this->SetupUi();
}

void IntroductionScreen::SetupUi()
{
    this->setWindowTitle("Auto Clicker");
    this->setFixedSize(600,800);

    QWidget* centralWidget = new QWidget(this);
    this->setCentralWidget(centralWidget);

    QHBoxLayout* mainLayout = new QHBoxLayout(centralWidget);

    QPixmap image("Ressources/Gradient.png");
    image.scaled(300,800,Qt::IgnoreAspectRatio);

    QLabel* imageHolder = new QLabel(centralWidget);
    imageHolder->setPixmap(image);

    mainLayout->addWidget(imageHolder);

    QVBoxLayout* vBoxLayout = new QVBoxLayout(centralWidget);
    mainLayout->addItem(vBoxLayout);

    QLabel* introLabel = new QLabel("Welcome to the Shitty Wizzard Installer.", centralWidget);
    introLabel->setAlignment(Qt::AlignCenter);
    vBoxLayout->addWidget(introLabel);

    QPushButton* startButton = new QPushButton(centralWidget);
    startButton->setText("Start Installation");

    vBoxLayout->addWidget(startButton);
}
