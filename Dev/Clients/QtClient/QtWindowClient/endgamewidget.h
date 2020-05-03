#ifndef ENDGAMEWIDGET_H
#define ENDGAMEWIDGET_H

#include <QObject>
#include <QWidget>
#include <QLabel>
#include <QSpacerItem>
#include <QGraphicsScene>
#include <QGraphicsProxyWidget>
#include <QGraphicsView>
#include <QBoxLayout>
#include <QSequentialAnimationGroup>

class GameWindow;
class GridProgressBar;
class SpiralProgressBar;
class TiledProgressBar;
class UpdateWorker;
class CrissCrossProgressBar;
class DoorStyleProgressBar;
class EndScoreWidget;
class FancyProgressBarWrapper;

class EndGameWidget : public QWidget
{
    Q_OBJECT
public:
    explicit EndGameWidget(GameWindow* parent = nullptr);

signals:

private slots:
    void Update(float dt);
    void FirstSequenceFinished();

private:
    GameWindow* gameWindow = nullptr;
    UpdateWorker* updateWorker = nullptr;

    GridProgressBar* gridProgressBar = nullptr;
    SpiralProgressBar* spiralProgressBar = nullptr;
    TiledProgressBar* tiledProgressBar = nullptr;
    CrissCrossProgressBar* crissCrossProgressBar = nullptr;
    DoorStyleProgressBar* doorStyleProgressBar = nullptr;
    EndScoreWidget* endScoreWidget = nullptr;

    FancyProgressBarWrapper* testWrapper = nullptr;

    void SetupUI();

    float time = 0;
};

#endif // ENDGAMEWIDGET_H
