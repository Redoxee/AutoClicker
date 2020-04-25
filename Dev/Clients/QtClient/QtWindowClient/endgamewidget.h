#ifndef ENDGAMEWIDGET_H
#define ENDGAMEWIDGET_H

// TODO Remove the full module include to speed up compilation (but that mean finding the recursive include shenanigan).
#include <QtCharts>

#include <QObject>
#include <QWidget>
#include <QLabel>
#include <QSpacerItem>
#include <QGraphicsScene>
#include <QGraphicsProxyWidget>
#include <QGraphicsView>

#include "math.h"

#include "gamewindow.h"
#include "updateworker.h"
#include "gridprogressbar.h"
#include "spiralprogressbar.h"
#include "tiledprogressbar.h"
#include "crisscrossprogressbar.h"
#include "doorstyleprogressbar.h"

class EndGameWidget : public QWidget
{
    Q_OBJECT
public:
    explicit EndGameWidget(GameWindow* parent = nullptr);

signals:

private slots:
    void Update(float dt);

private:
    GameWindow* gameWindow = nullptr;
    UpdateWorker* updateWorker = nullptr;

    GridProgressBar* gridProgressBar = nullptr;
    SpiralProgressBar* spiralProgressBar = nullptr;
    TiledProgressBar* tiledProgressBar = nullptr;
    CrissCrossProgressBar* crissCrossProgressBar = nullptr;
    DoorStyleProgressBar* doorStyleProgressBar = nullptr;

    void SetupUI();

    float time = 0;
};

#endif // ENDGAMEWIDGET_H