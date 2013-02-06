#ifndef CONFIG_H
#define CONFIG_H

#include <QObject>
#include <Abstract/bdtools.h>

class Config : public QObject
{
    Q_OBJECT
public:
    explicit Config(QObject *parent = 0);

    void start(QSqlDatabase * sdb);

private:
    void loadConfig();

public:
    int pomodoroDuration() const {return _pomodoroDuration;}
    int breakDuration() const {return _breakDuration;}
    int longBreakDuration() const {return _longBreakDuration;}

    void setPomodoroDuration(int pomodoroDuration);
    void setBreakDuration(int breakDuration);
    void setLongBreakDuration(int longBreakDuration);

    bool save();

private:
    BDTools bd;

    int _pomodoroDuration;
    int _breakDuration;
    int _longBreakDuration;

signals:

public slots:

};

#endif // CONFIG_H
