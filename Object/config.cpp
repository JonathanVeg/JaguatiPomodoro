#include "config.h"

Config::Config(QObject *parent) :
    QObject(parent)
{
}

void Config::start(QSqlDatabase *sdb)
{
    bd.sdb = *sdb;

    loadConfig();
}

void Config::setPomodoroDuration(int pomodoroDuration){
    _pomodoroDuration = pomodoroDuration;
}

void Config::setBreakDuration(int breakDuration){
    _breakDuration = breakDuration;
}

void Config::setLongBreakDuration(int longBreakDuration){
    _longBreakDuration = longBreakDuration;
}

void Config::loadConfig()
{
    if (bd.search("select * from config") > 0){
        setPomodoroDuration(bd.loadSimple(0).toInt());
        setBreakDuration(bd.loadSimple(1).toInt());
        setLongBreakDuration(bd.loadSimple(2).toInt());
    }else{
        QList<QString> fields, values;
        fields << "time_work";
        fields << "time_break";
        fields << "time_long_break";

        values << "25";
        values << "5";
        values << "30";

        bd.insert("config", fields, values);

    }
}

bool Config::save()
{
    QList<QString> fields, values;
    fields << "time_work";
    fields << "time_break";
    fields << "time_long_break";

    values << QString::number(pomodoroDuration());
    values << QString::number(breakDuration());
    values << QString::number(longBreakDuration());

    if (bd.search("select * from config"))
        bd.exec("delete from config");

    return bd.insert("config", fields, values);
}
