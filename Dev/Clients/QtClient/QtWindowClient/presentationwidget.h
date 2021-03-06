#ifndef PRESENTATIONWIDGET_H
#define PRESENTATIONWIDGET_H

#include <QObject>
#include <QWidget>
#include <QPushButton>
#include <QLabel>

#include <QThread>

#include "windows.h"

class GameWindow;
class QBoxLayout;

class PresentationWidget : public QWidget
{
    Q_OBJECT
public:
    PresentationWidget(QWidget* parent, GameWindow* gameWindow);
private:
    GameWindow* gameWindow;
    QPushButton* bottomButton;
    QBoxLayout* layout = nullptr;

    void SetupUI();

private slots:
    void StartButtonClicked();
    void serverWorkerReply();
    void serverReady();
};

#endif // PRESENTATIONWIDGET_H
