#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include <QWidget>
#include <QLabel>
#include <QSpinBox>
#include <Component/yesno.h>
#include <QPushButton>

class Configuration : public QWidget
{
    Q_OBJECT
public:
    explicit Configuration(QWidget *parent = 0);

    void createComponent();

    void input();

    void adjustLayout();

    void resizeEvent(QResizeEvent *);

    void connections();

    void updateData(int pomodoroDuration, int breakDuration, int longBreakDuration);

signals:
    void newConfigData(int pomodoroDuration, int breakDuration, int longBreakDuration);

public slots:

    void onClick_pbSave();

private:

    QLabel *lbg;

    QLabel *lPomodoroDuration;
    QSpinBox *sbPomodoroDuration;

    QLabel *lBreakDuration;
    QSpinBox *sbBreakDuration;

    QLabel *lLongBreakDuration;
    QSpinBox *sbLongBreakDuration;

    QPushButton *pbSave;



};

#endif // CONFIGURATION_H
