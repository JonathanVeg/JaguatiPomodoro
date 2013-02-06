#include "topmenu.h"
#include <QFont>

TopMenu::TopMenu(QWidget *parent) :
    QWidget(parent)
{
    createComponent();

    input();

    connections();
}

void TopMenu::createComponent()
{
    lbg = new QLabel(this);

    pbHome = new QPushButton(tr("Home"),this);
    pbConf = new QPushButton(tr("Configuration"),this);
    pbAbout = new QPushButton(tr("About"),this);
    pbHide = new QPushButton(tr("Hide"),this);

}

void TopMenu::input()
{
    lbg->setStyleSheet("background-color: rgba(0,0,0,200);");

    this->setStyleSheet("QPushButton"
                        "{"
                            "background-color: rgba(0,0,0,0);"
                            "color: rgba(240,240,240,255);"
                        "}"
                        "QPushButton:hover, QPushButton:default:hover"
                        "{"
                            "background-color: rgba(255,255,255,25);"
                        "}");
    QFont f;
    f.setBold(true);
    pbHome->setFont(f);
    pbConf->setFont(f);
    pbHide->setFont(f);
    pbAbout->setFont(f);

    pbHome->setCursor(Qt::PointingHandCursor);
    pbConf->setCursor(Qt::PointingHandCursor);
    pbHide->setCursor(Qt::PointingHandCursor);
    pbAbout->setCursor(Qt::PointingHandCursor);
}

void TopMenu::adjustLayout()
{
    lbg->setGeometry(0,0,this->width(),this->height());

    pbHome->setGeometry(10,4,70,22);

    pbConf->setGeometry(pbHome->width() + pbHome->x() + 10,4,100,22);

    pbAbout->setGeometry(pbConf->width() + pbConf->x() + 10,4,70,22);

    pbHide->setGeometry(this->width()-80,4,70,22);
}

void TopMenu::resizeEvent(QResizeEvent *)
{
    adjustLayout();
}

void TopMenu::connections()
{
    connect(pbHome,SIGNAL(clicked()),SLOT(onClick_pbHome()));

    connect(pbConf,SIGNAL(clicked()),SLOT(onClick_pbConf()));

    connect(pbAbout,SIGNAL(clicked()),SLOT(onClick_pbAbout()));

    connect(pbHide,SIGNAL(clicked()),SLOT(onClick_pbHide()));
}

void TopMenu::onClick_pbHome()
{
    emit go(0);
}

void TopMenu::onClick_pbConf()
{
    emit go(1);
}

void TopMenu::onClick_pbAbout()
{
    emit go(2);
}

void TopMenu::onClick_pbHide()
{
    emit hidden();
}
