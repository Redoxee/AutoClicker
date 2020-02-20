#ifndef GAMECINEMATIC_H
#define GAMECINEMATIC_H

#include <QObject>
#include <QWidget>

#include "windowshakeanimation.h"

class GameCinematic : public QWidget
{
    Q_OBJECT
public:
    explicit GameCinematic(QWidget *parent = nullptr, QMainWindow* window = nullptr);

private:
    WindowShakeAnimation* windowShakeAnimation;

};

#endif // GAMECINEMATIC_H
