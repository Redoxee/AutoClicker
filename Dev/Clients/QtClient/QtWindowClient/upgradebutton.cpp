#include "upgradebutton.h"
#include "SWIUtils.h"

UpgradeButton::UpgradeButton(QWidget *parent) : QWidget(parent)
{
    QHBoxLayout* hLayout = new QHBoxLayout(this);
    hLayout->setMargin(0);
    hLayout->setSpacing(0);
    this->MainButton = new QPushButton(this);
    hLayout->addWidget(this->MainButton);
    this->SecondaryButton = new QPushButton(this);
    hLayout->addWidget(this->SecondaryButton);

    this->SecondaryButton->setText("+");

    this->MainButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
    this->SecondaryButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
    this->MainButton->setFixedWidth(70);
    this->SecondaryButton->setFixedWidth(25);

    this->secondaryMenu = new QMenu(this);
    this->secondaryAction = new QAction(this);
    this->secondaryAction->setIconVisibleInMenu(false);
    this->secondaryMenu->addAction(this->secondaryAction);
    this->secondaryMenu->setToolTipsVisible(true);

    connect(this->SecondaryButton, &QPushButton::clicked, this,[this]()
    {
        QSize menuSize = this->secondaryMenu->sizeHint();
        QPoint pos = QWidget::mapToGlobal(this->SecondaryButton->pos());
        pos.setY(pos.y() - menuSize.height() + 5);
        pos.setX(pos.x() - menuSize.width() + this->SecondaryButton->width());
        this->secondaryMenu->popup(pos);
    });
}

void UpgradeButton::SetMainButtonValue(int64_t value)
{
    QString valueLabel = SWIUtils::FormatDownQuantity(value);
    this->MainButton->setText(this->mainButtonPattern.arg(valueLabel));
}

void UpgradeButton::SetSecondaryButtonValue(int64_t value)
{
    QString valueLabel = SWIUtils::FormatDownQuantity(value);
    this->secondaryAction->setText(this->secondButtonPattern.arg(valueLabel));
}

void UpgradeButton::SetSecondaryButtonValue(int64_t v1, int64_t v2)
{
    QString v1Label = SWIUtils::FormatDownQuantity(v1);
    QString v2Label = SWIUtils::FormatDownQuantity(v2);
    this->secondaryAction->setText(this->secondButtonPattern.arg(v1Label, v2Label));
}

void UpgradeButton::SetSecondaryTooltipValue(int64_t value)
{
    QString valueLabel = SWIUtils::FormatDownQuantity(value);
    this->secondaryAction->setToolTip(this->secondButtonTooltipPattern.arg(valueLabel));
}

void UpgradeButton::SetSecondaryTooltipValue(int64_t v1, int64_t v2)
{
    QString v1Label = SWIUtils::FormatDownQuantity(v1);
    QString v2Label = SWIUtils::FormatDownQuantity(v2);
    this->secondaryAction->setToolTip(this->secondButtonTooltipPattern.arg(v1Label, v2Label));
}
