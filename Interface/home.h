#ifndef HOME_H
#define HOME_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QProgressBar>

class Home : public QWidget
{
    Q_OBJECT
public:
    explicit Home(QWidget *parent = 0);

    void createComponent();

    void input();

    void connections();

    void adjustLayout();

    void resizeEvent(QResizeEvent *);

    void setCurrentTime(int time); // sec
    void setCurrentLongTime(int time); // sec
    void setTotalDuration(int duration); // min
    void setTotalLongDuration(int duration); // min

    void appendCurrentTime();
    void appendCurrentLongTime();

private:
    int currentTime() const {return _currentTime;}
    int currentLongTime() const {return _currentLongTime;}
    int totalDuration() const {return _totalDuration;}
    int totalLongDuration() const {return _totalLongDuration;}

signals:
    void play();
    void pause();
    void stop();
    void restart();
    void newTitle(QString title);

public slots:
    void onClick_pbPlay();
    void onClick_pbStop();
    void onClick_pbRestart();
    void setTitle(QString title);

private:

    QLabel *lbg;

    QLabel *lTime;
    QLabel *lPercent;
    QProgressBar *progress;
    QProgressBar *longProgress;

    QPushButton *pbPlay;
    QPushButton *pbStop;
    QPushButton *pbRestart;
    QLabel *lTextStatus;

    int _totalDuration;
    int _totalLongDuration;
    int _currentTime;
    int _currentLongTime;

};

#endif // HOME_H
