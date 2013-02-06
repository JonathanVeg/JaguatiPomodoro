#include "configuration.h"

Configuration::Configuration(QWidget *parent) :
    QWidget(parent)
{
    createComponent();

    input();

    connections();
}

void Configuration::createComponent()
{
    lbg = new QLabel(this);

    lBreakDuration = new QLabel(tr("Break duration"),this);
    sbBreakDuration = new QSpinBox(this);

    lLongBreakDuration = new QLabel(tr("Long break duration"),this);
    sbLongBreakDuration = new QSpinBox(this);

    lPomodoroDuration = new QLabel(tr("Pomodoro duration"),this);
    sbPomodoroDuration = new QSpinBox(this);

    pbSave = new QPushButton(tr("Save"),this);
}

void Configuration::input()
{
    lbg->setPixmap(QPixmap(QString("://imagens/bg")));

    QFont f;

    f.setPointSize(12);

    lPomodoroDuration->setFont(f);
    lPomodoroDuration->setStyleSheet("color: white;");

    lBreakDuration->setFont(f);
    lBreakDuration->setStyleSheet("color: white;");

    lLongBreakDuration->setFont(f);
    lLongBreakDuration->setStyleSheet("color: white;");

    QString button = "QPushButton"
            "{"
            "background-color: rgba(255,255,255,20);"
            "color:white;"
            "}";

    pbSave->setStyleSheet(button);

    sbPomodoroDuration->setSuffix(" min");
    sbPomodoroDuration->setCursor(Qt::PointingHandCursor);

    sbLongBreakDuration->setSuffix(" min");
    sbLongBreakDuration->setCursor(Qt::PointingHandCursor);

    sbBreakDuration->setSuffix(" min");
    sbBreakDuration->setCursor(Qt::PointingHandCursor);

    pbSave->setCursor(Qt::PointingHandCursor);
}

void Configuration::resizeEvent(QResizeEvent *)
{
    adjustLayout();
}

void Configuration::adjustLayout()
{
    lbg->setGeometry(0,0,this->width(),this->height());

    lPomodoroDuration->setGeometry(40,70,200,18);
    sbPomodoroDuration->setGeometry(40,lPomodoroDuration->y() + lPomodoroDuration->height() + 10,120,28);

    lBreakDuration->setGeometry(40,sbPomodoroDuration->y() + sbPomodoroDuration->height() + 60,200,18);
    sbBreakDuration->setGeometry(40,lBreakDuration->y() + lBreakDuration->height() + 10,120,28);

    lLongBreakDuration->setGeometry(40,sbBreakDuration->y() + sbBreakDuration->height() + 60,300,18);
    sbLongBreakDuration->setGeometry(40,lLongBreakDuration->y() + lLongBreakDuration->height() + 10,120,28);

    pbSave->setGeometry(this->width()/2 - 40,440,80,30);
}

void Configuration::connections()
{
    connect(pbSave,SIGNAL(clicked()),SLOT(onClick_pbSave()));
}

void Configuration::onClick_pbSave()
{
    newConfigData(sbPomodoroDuration->value(), sbBreakDuration->value(), sbLongBreakDuration->value());
}

void Configuration::updateData(int pomodoroDuration, int breakDuration, int longBreakDuration)
{
    sbPomodoroDuration->setValue(pomodoroDuration);

    sbBreakDuration->setValue(breakDuration);

    sbLongBreakDuration->setValue(longBreakDuration);
}
