#ifndef INTROSCREENWIDGET_H
#define INTROSCREENWIDGET_H

#include <QObject>
#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QMainWindow>
#include <QDialogButtonBox>
#include <QPushButton>

class IntroScreenWidget : public QWidget
{
    Q_OBJECT
public:
    IntroScreenWidget(QWidget* parent, QDialogButtonBox* bottomButtonHolder);

private:
    void SetupUI();
    QDialogButtonBox* bottomBox;
};

#endif // INTROSCREENWIDGET_H
