#include "introscreenwidget.h"

IntroScreenWidget::IntroScreenWidget(QWidget* parent) : QWidget(parent)
{
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
}
