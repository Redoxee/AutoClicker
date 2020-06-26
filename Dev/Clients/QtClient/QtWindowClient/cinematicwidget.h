#ifndef GAMECINEMATIC_H
#define GAMECINEMATIC_H

#include <QObject>
#include <QWidget>
#include <QSequentialAnimationGroup>
#include <QPropertyAnimation>
#include <QDialogButtonBox>
#include <QProgressBar>
#include <QVBoxLayout>
#include <QLabel>
#include <QMessageBox>

class GameWindow;
class WindowShakeAnimation;
class EventLogger;

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

    QSequentialAnimationGroup* animationSequence = nullptr;
    EventLogger* eventLogger = nullptr;

    QProgressBar* progressBar = nullptr;
    QPushButton* bottomButton = nullptr;

    int prevProgress = 0;

    void SetupUI();
    void ValueChanged(int value);
};

#endif // GAMECINEMATIC_H
