#include "introductionscreen.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QWidget>
#include <QPixmap>
#include <QSpacerItem>
#include <QDialogButtonBox>

#include "mainwindow.h"

IntroductionScreen::IntroductionScreen(QApplication* application, QWidget *parent)
    : QMainWindow(parent)
{
    this->qApplication = application;
    this->SetupUi();
}

void IntroductionScreen::SetupUi()
{
    this->setWindowTitle("Shitty Wizzard Installer");
    this->setFixedSize(500,600);

    QWidget* centralWidget = new QWidget(this);
    this->setCentralWidget(centralWidget);

    QVBoxLayout* mainLayout = new QVBoxLayout(centralWidget);
    QHBoxLayout* horizontalLayout = new QHBoxLayout();

    QPixmap image("Ressources/Gradient.png");
    QLabel* imageHolder = new QLabel(centralWidget);
    imageHolder->setPixmap(image);
    imageHolder->setStyleSheet("background: red");
    imageHolder->setScaledContents(true);
    imageHolder->setMaximumWidth(200);
    horizontalLayout->addWidget(imageHolder);

    QVBoxLayout* vBoxLayout = new QVBoxLayout(centralWidget);
    vBoxLayout->setMargin(15);

    horizontalLayout->addItem(vBoxLayout);

    QLabel* titleLabel = new QLabel("Welcome to the Shitty Wizzard Installer(SWI).", centralWidget);
    titleLabel->setAlignment(Qt::AlignLeft);
    vBoxLayout->addWidget(titleLabel);

    QLabel* subTitleLabel = new QLabel("We will guide you through the installation of DotGobbler.exe", centralWidget);
    vBoxLayout->addWidget(subTitleLabel);
    mainLayout->addItem(horizontalLayout);

    vBoxLayout->addStretch(0);

    QPushButton* startButton = new QPushButton(centralWidget);
    startButton->setText("Start Installation!");
    QDialogButtonBox* buttonBox = new QDialogButtonBox();
    buttonBox->addButton(startButton, QDialogButtonBox::ButtonRole::YesRole);

    mainLayout->addWidget(buttonBox);

    this->setLayout(mainLayout);

    QObject::connect(startButton, &QPushButton::clicked, this, &IntroductionScreen::StartPressed);
}

void IntroductionScreen::StartPressed()
{
    qDebug() << "Coucou";
    MainWindow* mainWindow = new MainWindow(this->qApplication);
    mainWindow->show();
    this->close();
}
