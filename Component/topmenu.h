#ifndef TOPMENU_H
#define TOPMENU_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>

class TopMenu : public QWidget
{
    Q_OBJECT
public:
    explicit TopMenu(QWidget *parent = 0);

    void createComponent();

    void input();

    void adjustLayout();

    void resizeEvent(QResizeEvent *);

    void connections();

signals:

    void go(int index);

    void hidden();

public slots:

    void onClick_pbHome();

    void onClick_pbConf();

    void onClick_pbAbout();

    void onClick_pbHide();


public:

    QLabel *lbg;
    QPushButton *pbHome;
    QPushButton *pbConf;
    QPushButton *pbAbout;
    QPushButton *pbHide;

};

#endif // TOPMENU_H
