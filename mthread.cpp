#include "mthread.h"

mthread::mthread(QObject *parent) :QThread(parent)
{

}

void mthread::run()
{
        QMutex mutex ;



        for(int i =0 ;i < 100 ;i++){

        mutex.lock() ;
        if (this->stop)
            break ;
        mutex.unlock();
        emit this->AlarmFired() ;
        this->sleep(10);
        }







}
