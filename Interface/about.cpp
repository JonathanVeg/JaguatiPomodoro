#include "about.h"
#include <QDesktopServices>
#include <QUrl>

About::About(QWidget *parent) :
    QWidget(parent)
{
    createComponent();

    connections();

    input();
}

void About::createComponent()
{
    view = new QWebView(this);
}

void About::input()
{     
    view->show();
    view->page()->setLinkDelegationPolicy(QWebPage::DelegateAllLinks);
}

void About::connections()
{
    connect(view,SIGNAL(linkClicked(QUrl)),SLOT(click(QUrl)));
    connect(view,SIGNAL(loadFinished(bool)),SLOT(check(bool)));
}

void About::click(QUrl url)
{
    QDesktopServices::openUrl(url);
}

bool About::isConnected()
{
    return _isConnected;
}

void About::check(bool b)
{
    _isConnected = b;
}

void About::resizeEvent(QResizeEvent *)
{
    view->setGeometry(0,0,this->width(),this->height());
}

void About::load()
{
    if(!_load)
    {
        view->load(QUrl("http://www.jaguati.com.br/ads/pomodoro/pomodoro.php"));
        _load = true;
    }
}
