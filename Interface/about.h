#ifndef ABOUT_H
#define ABOUT_H

#include <QWidget>
#include <QtWebKit/QWebView>

class About : public QWidget
{
    Q_OBJECT
public:
    explicit About(QWidget *parent = 0);

    void createComponent();

    void input();

    void connections();

    void resizeEvent(QResizeEvent *);

signals:

public slots:

    void click(QUrl url);

    bool isConnected();

    void check(bool b);

    void load();

private:

    QWebView * view;
    bool _load;

public:
    bool _isConnected;

};

#endif // ABOUT_H
