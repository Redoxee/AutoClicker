#ifndef PRESENTATIONWIDGET_H
#define PRESENTATIONWIDGET_H

#include <QObject>
#include <QWidget>
#include <QPushButton>
#include <QLabel>

#include <QThread>

#include "windows.h"

#include "gamewindow.h"
#include "corechecker.h"

class PresentationWidget : public QWidget
{
    Q_OBJECT
public:
    PresentationWidget(QWidget* parent, GameWindow* gameWindow);
private:
    GameWindow* gameWindow;
    QPushButton* bottomButton;
    QThread* checkerThread;
    CoreChecker* coreChecker;

    void SetupUI();

private slots:
    void StartButtonClicked();
    void CoreCheckerReply();
};

#endif // PRESENTATIONWIDGET_H
