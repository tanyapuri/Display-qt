#ifndef MTHREAD_H
#define MTHREAD_H

#include <QObject>
#include <QThread>
#include <QtCore>

class mthread :public QThread
{
    Q_OBJECT
public:
    explicit mthread (QObject *parent = nullptr);
    void run();
    bool stop=false ;
signals:
   void AlarmFired();

public slots:
};

#endif // MTHREAD_H
