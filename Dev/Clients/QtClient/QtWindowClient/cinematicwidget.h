#ifndef GAMECINEMATIC_H
#define GAMECINEMATIC_H

#include <QObject>
#include <QWidget>
#include <QSequentialAnimationGroup>
#include <QPropertyAnimation>

#include <QDialogButtonBox>
#include <QProgressBar>
#include <QVBoxLayout>

#include "gamewindow.h"
#include "windowshakeanimation.h"

class CinematicWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CinematicWidget(QWidget *parent, GameWindow* window);

    void StartCinematic();
public slots:
    void MainSequenceFinished();
    void GameStartPressed();

private:
    GameWindow* gameWindow;

    QSequentialAnimationGroup* animationSequence;
    WindowShakeAnimation* windowShakeAnimation;

    QProgressBar* progressBar;
    QPushButton* bottomButton;

    void SetupUI();
};

#endif // GAMECINEMATIC_H
