#ifndef PRESENTATIONWIDGET_H
#define PRESENTATIONWIDGET_H

#include <QObject>
#include <QWidget>
#include <QPushButton>
#include <QLabel>

#include <QThread>

#include "windows.h"
#include "serverworker.h"

#include "gamewindow.h"

class PresentationWidget : public QWidget
{
    Q_OBJECT
public:
    PresentationWidget(QWidget* parent, GameWindow* gameWindow);
private:
    GameWindow* gameWindow;
    QPushButton* bottomButton;

    void SetupUI();

private slots:
    void StartButtonClicked();
    void serverWorkerReply();
    void serverReady();

signals:
    void OrderSignal();
};

#endif // PRESENTATIONWIDGET_H
