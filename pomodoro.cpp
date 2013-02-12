#include "pomodoro.h"
#include "ui_pomodoro.h"
#include <QDebug>
#include <QFile>
#include <QMessageBox>
#include <windows.h>
#include <QSound>

Pomodoro::Pomodoro(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Pomodoro)
{
    ui->setupUi(this);

    createComponent();

    adjustLayout();

    input();

    connections();
}

Pomodoro::~Pomodoro()
{
    delete trayIcon;
    delete ui;
}

void Pomodoro::createComponent(){

    timer = new QTimer();

    trayIcon = new QSystemTrayIcon();

    config = new Config();

    wTopMenu = new TopMenu(this);

    swPage = new QStackedWidget(this);
    {
        wHome = new Home(this);

        wConfiguration = new Configuration(this);

        wAbout = new About(this);
    }
}

void Pomodoro::adjustLayout(){
    wTopMenu->setGeometry(0,0,this->width(),30);

    swPage->setGeometry(0,wTopMenu->height(),this->width(),this->height()-wTopMenu->height());

    wHome->setGeometry(0,0,swPage->width(),swPage->height());

    wConfiguration->setGeometry(0,0,swPage->width(),swPage->height());

    wAbout->setGeometry(0,0,swPage->width(),swPage->height());
}

void Pomodoro::input()
{
    QFile file(":/QSS/style");
    file.open(QFile::ReadOnly);

    setStyleSheet(QLatin1String(file.readAll()));

    trayIcon->setIcon(QIcon(":/imagens/icone"));

    trayIcon->show();

    bd.start("bdpomodoro.db3", "mainConnection");

    QSqlDatabase *sdb = &bd.sdb;

    config->start(sdb);

    _occurrence = Break;

    _currentPomodoro = 0;

    setMaximumSize(500,550);
    setMinimumSize(500,550);

    swPage->addWidget(wHome);
    swPage->addWidget(wConfiguration);
    swPage->addWidget(wAbout);

    _currentTime = 59;

    timer->setInterval(1000);

    setTotalDuration(1);

    wHome->setTotalLongDuration(4 * config->pomodoroDuration() + 3 * config->breakDuration());

    wHome->setCurrentLongTime(0);
}

void Pomodoro::connections(){
    connect(timer, SIGNAL(timeout()), SLOT(timer_timeout()));

    connect(wTopMenu,SIGNAL(go(int)),swPage,SLOT(setCurrentIndex(int)));

    connect(wTopMenu,SIGNAL(go(int)),SLOT(updateData(int)));

    connect(wTopMenu,SIGNAL(hidden()),this,SLOT(hide()));

    connect(wConfiguration, SIGNAL(newConfigData(int,int,int)), SLOT(newConfigData(int,int,int)));

    connect(wHome, SIGNAL(play()), SLOT(play()));

    connect(wHome, SIGNAL(pause()), SLOT(pause()));

    connect(wHome, SIGNAL(stop()), SLOT(stop()));

    connect(wHome, SIGNAL(restart()), SLOT(restart()));

    connect(wHome,SIGNAL(newTitle(QString)),SLOT(setWindowTitle(QString)));

    connect(wTopMenu->pbAbout,SIGNAL(clicked()),wAbout,SLOT(load()));

    connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(trayIconClicked(QSystemTrayIcon::ActivationReason)));
}

void Pomodoro::timer_timeout(){
    setCurrentTime(currentTime() + 1);

    if (_occurrence != LongBreak)
        wHome->appendCurrentLongTime();

    if (currentTime() == totalDuration())
    {
        QSound::play("pomodoro_beep.wav");

        // aqui eu preciso mudar o modo.
        timer->stop();

        //Beep(523, 500);

        if (_occurrence == Pomo){
            if (_currentPomodoro % 4 == 0){
                // long break
                _occurrence = LongBreak;
                wHome->setTitle("Long break");
                setTotalDuration(config->longBreakDuration());
                trayIcon->showMessage("Long break started", "You have " + QString::number(config->longBreakDuration()) + " minutes to break. Close your work during this time.");
            }else{
                // normal break
                trayIcon->showMessage("Break started", "You have " + QString::number(config->breakDuration()) + " minutes to break. Close your work during this time.");
                _occurrence = Break;
                wHome->setTitle("Break");
                setTotalDuration(config->breakDuration());
            }
        }else{
            // pomodoro
            _occurrence = Pomo;
            wHome->setTitle("Pomodoro");
            setTotalDuration(config->pomodoroDuration());
            trayIcon->showMessage("Pomodoro started", "You have " + QString::number(config->pomodoroDuration()) + " minutes to work. Focus on the job during this time.");

            _currentPomodoro ++;
        }

        setCurrentTime(0);
        timer->start();
    }

}

void Pomodoro::resizeEvent(QResizeEvent *)
{
    adjustLayout();
}

void Pomodoro::updateData(int index)
{
    if (index == 1){
        wConfiguration->updateData(config->pomodoroDuration(), config->breakDuration(), config->longBreakDuration());
    }
}

void Pomodoro::newConfigData(int pomodoroDuration, int breakDuration, int longBreakDuration)
{
    config->setPomodoroDuration(pomodoroDuration);
    config->setBreakDuration(breakDuration);
    config->setLongBreakDuration(longBreakDuration);

    if (config->save())
        QMessageBox::information(this, "Saved data", "The data was correctly saved.", QMessageBox::Ok);
    else
        QMessageBox::warning(this, "Unsaved data", "The data couldn't be saved.", QMessageBox::Ok);
}

void Pomodoro::stop(){
    timer->stop();

    setTotalDuration(config->pomodoroDuration());

    trayIcon->showMessage("Pomodoro was stopped", "Your pomodoro was stopped for you");

    wHome->setTotalDuration(config->pomodoroDuration());

    wHome->setCurrentTime(0);

    wHome->setTotalLongDuration(4 * config->pomodoroDuration() + 3 * config->breakDuration());

    wHome->setCurrentLongTime(0);
}

void Pomodoro::setTotalDuration(usint totalDuration)
{
    wHome->setTotalDuration(totalDuration);

    _totalDuration = totalDuration * 60;
}

void Pomodoro::setTotalLongDuration(usint totalLongDuration){
    wHome->setTotalLongDuration(totalLongDuration);
}

void Pomodoro::setCurrentTime(usint currentTime)
{
    _currentTime = currentTime;
    wHome->setCurrentTime(_currentTime);
}

void Pomodoro::play(){
    timer->start();
}

void Pomodoro::pause()
{
    timer->stop();
}

void Pomodoro::restart()
{
    setCurrentTime(0);
}

void Pomodoro::trayIconClicked(QSystemTrayIcon::ActivationReason)
{
    this->setVisible(!this->isVisible());
}

void Pomodoro::keyPressEvent(QKeyEvent *event){
    if (event->modifiers() == Qt::ControlModifier)
    {
        if (event->key() == Qt::Key_P)
            wHome->onClick_pbPlay();

        if (event->key() == Qt::Key_S)
            wHome->onClick_pbStop();

        if (event->key() == Qt::Key_R)
            wHome->onClick_pbRestart();

        return;
    }

    if (event->key() == Qt::Key_F2)
        wHome->onClick_pbPlay();
}
