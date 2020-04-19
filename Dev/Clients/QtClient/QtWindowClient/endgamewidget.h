#ifndef ENDGAMEWIDGET_H
#define ENDGAMEWIDGET_H

#include "math.h"

#include <QObject>
#include <QWidget>
#include <QLabel>
#include <QSpacerItem>

#include "gamewindow.h"
#include "updateworker.h"
#include "gridprogressbar.h"
#include "spiralprogressbar.h"

class EndGameWidget : public QWidget
{
    Q_OBJECT
public:
    explicit EndGameWidget(GameWindow* parent = nullptr);

signals:

private slots:
    void Update(float dt);

private:
    GameWindow* gameWindow;
    UpdateWorker* updateWorker;

    GridProgressBar* gridProgressBar;
    SpiralProgressBar* spiralProgressBar;

    void SetupUI();

    float time = 0;
};

#endif // ENDGAMEWIDGET_H
