#ifndef POMODORO_H
#define POMODORO_H

#include <QMainWindow>
#include <QTimer>
#include <QSystemTrayIcon>
#include <QIcon>
#include <Abstract/bdtools.h>
#include <Object/config.h>
#include <Component/topmenu.h>
#include <Interface/home.h>
#include <Interface/configuration.h>
#include <Interface/about.h>
#include <QStackedWidget>
#include <QKeyEvent>

namespace Ui {
    class Pomodoro;
}

typedef unsigned short int usint;

class Pomodoro : public QMainWindow
{
    Q_OBJECT

public:
    explicit Pomodoro(QWidget *parent = 0);
    ~Pomodoro();

private:
    void createComponent();
    void adjustLayout();
    void input();
    void connections();
    void resizeEvent(QResizeEvent *);
    void loadConfig();
    void loadLanguage(QString file);
    void setTotalDuration(usint totalDuration);
    void setTotalLongDuration(usint totalLongDuration);
    void setCurrentTime(usint currentTime);

    inline usint totalDuration() const {return _totalDuration;}
    inline usint currentTime() const {return _currentTime;}

private slots:
    void timer_timeout();

    void updateData(int index);

    void newConfigData(int pomodoroDuration, int breakDuration, int longBreakDuration);

    void play();

    void pause();

    void stop();

    void restart();

    void trayIconClicked(QSystemTrayIcon::ActivationReason);

    void keyPressEvent(QKeyEvent *event);

private:

    enum Occurrence{
        Pomo, // ia usar Pomodoro, mas é o nome da classe, ai da erro, poderia por minúsculo, mas parece que a padronização dos enuns é por maiúsculo
        Break,
        LongBreak
    };

    Ui::Pomodoro *ui;

    QTimer *timer;

    QSystemTrayIcon *trayIcon;

    BDTools bd;

    Config *config;

    Occurrence _occurrence;

    TopMenu *wTopMenu;

    QStackedWidget *swPage;

    Home *wHome;

    Configuration *wConfiguration;

    About *wAbout;

    usint _currentPomodoro;

    unsigned int _currentTime;

    usint _totalDuration;
};

#endif // POMODORO_H
