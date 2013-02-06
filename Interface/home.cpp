#include "home.h"
#include <QMessageBox>
#include <QDebug>

Home::Home(QWidget *parent) :
    QWidget(parent)
{
    createComponent();

    connections();

    input();
}

void Home::createComponent()
{
    lbg = new QLabel(this);

    pbPlay = new QPushButton(tr("Play"),this);
    pbStop = new QPushButton(tr("Stop"),this);
    pbRestart = new QPushButton(tr("Restart"),this);

    lTime = new QLabel(tr("00:00"),this);
    lPercent = new QLabel(tr("0%"),this);

    lTextStatus = new QLabel(tr("Pomodoro"),this);

    progress = new QProgressBar(this);

    longProgress = new QProgressBar(this);
}

void Home::connections(){
    connect(pbPlay, SIGNAL(clicked()), SLOT(onClick_pbPlay()));

    connect(pbStop, SIGNAL(clicked()), SLOT(onClick_pbStop()));

    connect(pbRestart, SIGNAL(clicked()), SLOT(onClick_pbRestart()));
}

void Home::input()
{
    lbg->setPixmap(QPixmap(QString("://imagens/bg")));

    QFont f;
    f.setPointSize(50);
    lTime->setAlignment(Qt::AlignCenter);
    lTime->setFont(f);
    lTime->setStyleSheet("color:white;");

    f.setPointSize(14);

    lPercent->setAlignment(Qt::AlignCenter);
    lPercent->setFont(f);
    lPercent->setStyleSheet("color:white;");    

    f.setPointSize(18);

    lTextStatus->setAlignment(Qt::AlignCenter);
    lTextStatus->setFont(f);
    lTextStatus->setStyleSheet("color:white;");

    f.setPointSize(10);

    pbPlay->setFont(f);
    pbStop->setFont(f);
    pbRestart->setFont(f);

    QString button = "QPushButton"
            "{"
            "background-color: rgba(255,255,255,20);"
            "color:white;"
            "}";

    pbPlay->setStyleSheet(button);
    pbStop->setStyleSheet(button);
    pbRestart->setStyleSheet(button);


    progress->setTextVisible(false);

    progress->setMaximum(100);
    progress->setMinimum(0);
    progress->setValue(0);
    progress->setTextVisible(false);
    progress->setStyleSheet("QProgressBar"
                            " {"
                            "border: 1px solid rgba(0,0,0,25);"
                            "border-radius: 5px;"
                            "background-color: rgba(240, 240, 240, 255);"
                            "}"

                            "QProgressBar::chunk "
                            "{"
                            "border: 1px solid rgba(0,0,0,25);"
                            "border-radius: 5px;"
                            "background-color:rgba(0,0,0,200);"
                            "}");

    pbPlay->setCursor(Qt::PointingHandCursor);
    pbStop->setCursor(Qt::PointingHandCursor);
    pbRestart->setCursor(Qt::PointingHandCursor);

    setTotalDuration(0);
    setCurrentTime(0);
}

void Home::adjustLayout()
{
    lbg->setGeometry(0,0,this->width(),this->height());

    longProgress->setGeometry(0,0,536,2);

    lTextStatus->setGeometry(this->width()/2 - 60,45,120,34);

    lTime->setGeometry(this->width()/2 - 100,160,200,80);

    lPercent->setGeometry(this->width()/2 - 50,lTime->y() + lTime->height() + 50,100,20);
    progress->setGeometry(50,lPercent->y() + lPercent->height() + 20,400,10);    

    pbPlay->setGeometry(this->width()/2 - 40,440,80,30);
    pbStop->setGeometry(pbPlay->x() - 100,pbPlay->y(),80,30);
    pbRestart->setGeometry(pbPlay->x() + pbPlay->width() + 20,pbPlay->y(),80,30);

}

void Home::resizeEvent(QResizeEvent *)
{
    adjustLayout();
}

void Home::onClick_pbPlay(){
    if (pbPlay->text() == "Play"){
        play();

        pbPlay->setText("Pause");
    }else{
        pause();

        pbPlay->setText("Play");
    }
}

void Home::onClick_pbStop(){
    int op = QMessageBox::question(this, "Stop operation", "Do you really want to stop this process?", "Yes", "No");

    if (op == 0){
        stop();

        pbPlay->setText("Play");
    }
}

void Home::onClick_pbRestart(){

    int op = QMessageBox::question(this, "Restart timer", "Do you really want to restart the timer?\nIsn't recommended do it.", "Yes", "No");
    if (op == 0){
        restart();
    }
}

void Home::setTotalDuration(int duration)
{
    _totalDuration = duration * 60;

    progress->setMaximum(duration * 60);
}

void Home::setTotalLongDuration(int duration){
    qDebug() << "setTotalLongDuration" << duration;
    _totalLongDuration = duration;

    longProgress->setMaximum(duration * 60);
}

void Home::setCurrentTime(int time)
{
    QString timeString; // time no formato "00:00" para o label

    int remainingTime = totalDuration() - time;

    if (remainingTime != 0){

        int minutes = remainingTime / 60;
        int seconds = remainingTime % 60;

        QString minutesString = minutes < 10? "0" + QString::number(minutes): QString::number(minutes);

        QString secondsString = seconds < 10? "0" + QString::number(seconds): QString::number(seconds);

        timeString = minutesString + ":" + secondsString;
    }else{
        timeString = "00:00";
    }

    lTime->setText(timeString);

    if (progress->maximum() != 0){
        _currentTime = time;

        progress->setValue(time);

        lPercent->setText(QString::number((float)(progress->value()) / progress->maximum() * 100, 'f', 2) + "%");
    }else
    {
        lPercent->setText("0%");
    }
}

void Home::setCurrentLongTime(int time){
    if (longProgress->maximum() != 0){
        _currentLongTime = time;

        longProgress->setValue(time);
    }

    if (longProgress->value() < time){
        _currentLongTime = 0;

        longProgress->setValue(0);
    }
}

void Home::appendCurrentTime()
{
    setCurrentTime(currentTime() + 1);
}

void Home::appendCurrentLongTime(){
    setCurrentLongTime(currentLongTime() + 1);
}

void Home::setTitle(QString title)
{
    emit newTitle(title);
    lTextStatus->setText(title);
}
