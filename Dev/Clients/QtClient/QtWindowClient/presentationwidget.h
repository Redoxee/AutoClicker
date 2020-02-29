#ifndef PRESENTATIONWIDGET_H
#define PRESENTATIONWIDGET_H

#include <QObject>
#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QProcess>
#include <QDir>
#include <QString>

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
    void startButtonClicked();
};

#endif // PRESENTATIONWIDGET_H
