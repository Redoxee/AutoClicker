#ifndef INTROSCREENWIDGET_H
#define INTROSCREENWIDGET_H

#include <QObject>
#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QMainWindow>

class IntroScreenWidget : public QWidget
{
    Q_OBJECT
public:
    IntroScreenWidget(QWidget* parent);

private:
    void SetupUI();
};

#endif // INTROSCREENWIDGET_H
