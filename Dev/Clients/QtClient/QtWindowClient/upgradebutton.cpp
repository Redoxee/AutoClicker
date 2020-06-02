#include "upgradebutton.h"

UpgradeButton::UpgradeButton(QString mainLabel, QWidget *parent) : QWidget(parent)
{
    QHBoxLayout* hLayout = new QHBoxLayout(this);
    hLayout->setMargin(0);
    hLayout->setSpacing(0);
    this->MainButton = new QPushButton(this);
    hLayout->addWidget(this->MainButton);
    this->SecondaryButton = new QPushButton(this);
    hLayout->addWidget(this->SecondaryButton);

    this->MainButton->setText(mainLabel);
    this->SecondaryButton->setText("+");

    this->MainButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
    this->SecondaryButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
    this->MainButton->setFixedWidth(55);
    this->SecondaryButton->setFixedWidth(25);

    this->secondaryMenu = new QMenu(this);
    this->secondaryAction = new QAction(this);
    this->secondaryMenu->addAction(this->secondaryAction);
    this->secondaryMenu->setToolTipsVisible(true);

    connect(this->SecondaryButton, &QPushButton::clicked, this,[this]()
    {
        this->secondaryAction->setText("Buy upgrade for 8888888 bits\n Test");
        this->secondaryAction->setToolTip("Upgrade by x precent \n youhou.");

        QSize menuSize = this->secondaryMenu->sizeHint();
        QPoint pos = QWidget::mapToGlobal(this->SecondaryButton->pos());
        pos.setY(pos.y() - menuSize.height() + 5);
        pos.setX(pos.x() - menuSize.width() + this->SecondaryButton->width());

        this->secondaryMenu->exec(pos);
    });
}
