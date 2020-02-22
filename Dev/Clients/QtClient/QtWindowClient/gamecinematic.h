#ifndef GAMECINEMATIC_H
#define GAMECINEMATIC_H

#include <QObject>
#include <QWidget>
#include <QSequentialAnimationGroup>
#include <QPropertyAnimation>

#include <QProgressBar>
#include <QVBoxLayout>

#include "windowshakeanimation.h"

class GameCinematic : public QWidget
{
    Q_OBJECT
public:
    explicit GameCinematic(QWidget *parent = nullptr, QMainWindow* window = nullptr);

    void StartCinematic();

private:
    QSequentialAnimationGroup* animationSequence;
    WindowShakeAnimation* windowShakeAnimation;

    QProgressBar* progressBar;

    void SetupUI();
};

#endif // GAMECINEMATIC_H
