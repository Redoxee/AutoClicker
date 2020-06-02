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
    explicit UpgradeButton(QString mainLabel, QWidget *parent = nullptr);

    QPushButton* MainButton = nullptr;
    QPushButton* SecondaryButton = nullptr;

    QMenu* secondaryMenu = nullptr;
    QAction* secondaryAction = nullptr;

private:
signals:
public slots:
};

#endif // UPGRADEBUTTON_H
