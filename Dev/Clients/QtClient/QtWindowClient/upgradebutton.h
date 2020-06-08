#ifndef UPGRADEBUTTON_H
#define UPGRADEBUTTON_H

#include <QWidget>
#include <QPushButton>
#include <QAction>
#include <QMenu>
#include <QHBoxLayout>

class UpgradeButton : public QWidget
{
    Q_OBJECT
public:
    explicit UpgradeButton(QWidget *parent = nullptr);

    void SetMainButtonValue(int64_t value);
    void SetSecondaryButtonValue(int64_t value);
    void SetSecondaryButtonValue(int64_t v1, int64_t v2);
    void SetSecondaryTooltipValue(int64_t value);
    void SetSecondaryTooltipValue(int64_t v1, int64_t v2);

    QPushButton* MainButton = nullptr;
    QPushButton* SecondaryButton = nullptr;

    QMenu* secondaryMenu = nullptr;
    QAction* secondaryAction = nullptr;
    QString mainButtonPattern;
    QString secondButtonPattern;
    QString secondButtonTooltipPattern;

private:
signals:
public slots:
};

#endif // UPGRADEBUTTON_H
