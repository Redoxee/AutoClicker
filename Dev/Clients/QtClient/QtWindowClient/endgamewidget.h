#ifndef ENDGAMEWIDGET_H
#define ENDGAMEWIDGET_H

#include <QObject>
#include <QWidget>
#include <QLabel>
#include <QSpacerItem>
#include <QGraphicsScene>
#include <QGraphicsProxyWidget>
#include <QGraphicsView>
#include <QGridLayout>
#include <QBoxLayout>
#include <QSequentialAnimationGroup>
#include <QFrame>
#include <QPushButton>
#include <QPropertyAnimation>

class QProgressBar;

class GameWindow;
class GridProgressBar;
class SpiralProgressBar;
class TiledProgressBar;
class UpdateWorker;
class CrissCrossProgressBar;
class DoorStyleProgressBar;
class EndScoreWidget;
class FancyProgressBarWrapper;
class ScaledProgressBar;

class EndGameModuleWidget;
class PropertyTextAnimation;

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

    ScaledProgressBar* BigBar = nullptr;
    GridProgressBar* gridProgressBar = nullptr;
    SpiralProgressBar* spiralProgressBar = nullptr;
    TiledProgressBar* tiledProgressBar = nullptr;
    CrissCrossProgressBar* crissCrossProgressBar = nullptr;
    DoorStyleProgressBar* doorStyleProgressBar = nullptr;
    EndScoreWidget* endScoreWidget = nullptr;
    EndGameModuleWidget* endGameModule = nullptr;

    void SetupUI();

    float time = 0;
};

class EndGameModuleWidget : public QFrame
{
    Q_OBJECT
public:
    EndGameModuleWidget(QWidget* parent, GameWindow* gameWindow);
    QAbstractAnimation* GetRevealAnimation();

private :
    GameWindow* gamewindow = nullptr;

    QLabel* titleLabel = nullptr;
    QLabel* subtitleLabel = nullptr;
    QLabel* finalScoreLabel = nullptr;
    QProgressBar* scoreBar = nullptr;
    QLabel* scoreLabel = nullptr;
    QPushButton* sendMessage = nullptr;
    QPushButton* exit = nullptr;
};


class PropertyTextAnimation : public QPropertyAnimation
{
    Q_OBJECT
public:
    PropertyTextAnimation(QProgressBar* progressbar, QLabel* label, int endValue, QWidget* parent);

protected:
    void updateCurrentValue(const QVariant &value) override;
    void updateState(QAbstractAnimation::State newState, QAbstractAnimation::State oldState) override;

private:
    QLabel* label = nullptr;
    int endValue;
};

#endif // ENDGAMEWIDGET_H
