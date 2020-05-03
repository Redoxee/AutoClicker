#ifndef INTROSCREENWIDGET_H
#define INTROSCREENWIDGET_H

#include <QObject>
#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QMainWindow>
#include <QDialogButtonBox>
#include <QPushButton>

class GameWindow;

class IntroScreenWidget : public QWidget
{
    Q_OBJECT
public:
    IntroScreenWidget(QWidget* parent, GameWindow* mainWindow);

public slots:
    void StartButtonPressed();

private:
    void SetupUI();
    GameWindow* mainWindow;

    QPushButton* startButton;
};

#endif // INTROSCREENWIDGET_H
