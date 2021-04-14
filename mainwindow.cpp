#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QApplication>
#include <QPushButton>
#include <string>
#include <QDebug>
#include <QDialog>
#include <QInputDialog>
#include <time.h>
#include <QFrame>
#include <QFile>
#include <string>
#include <QTextStream>
#include <QDir>
#include <QTableWidget>
#include <QTableView>
#include <QDir>
#include <QMessageBox>
#include <string>
#include <algorithm>
#include <QtMultimedia/QMediaPlayer>
#include <QUrl>
#include <QTime>
#include<QTimer>
#include<QDate>


#include "QtNetwork/QNetworkReply"
#include "QUrl"
#include "QUrlQuery"
#include "QtNetwork/QNetworkAccessManager"
#include "QtNetwork/QNetworkRequest"
#include "QJsonDocument"
#include "QJsonArray"
#include "QJsonObject"

#include <QPixmap>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    qDebug()<<0;
    ui->setupUi(this);

    time_t timenow=time(NULL);
    tm* Jtime;
    Jtime=localtime(&timenow);
    year=Jtime->tm_year+1900;
    month=Jtime->tm_mon+1;
    day=Jtime->tm_mday;
    set_checked();
    ui->textEdit->setStyleSheet("background-color:rgba(128,128,128,0.3);");
    ui->memo->setShowGrid(false);
    ui->tv->setShowGrid(false);
    ui->memo->setStyleSheet("background-color:rgba(128,128,128,0.3);");
    ui->tv->setStyleSheet("background-color:rgba(128,128,128,0.3);");
    this->setWindowFlags(Qt::Dialog | Qt::WindowMinimizeButtonHint | Qt::WindowCloseButtonHint);
    refresh_allbutton();
    ui->memo->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tv->setSelectionBehavior(QAbstractItemView::SelectRows);
    connect(ui->memo,SIGNAL(cellDoubleClicked(int,int)),this,SLOT(memo_doubleclicked(int,int)));
    connect(ui->tv,SIGNAL(cellDoubleClicked(int,int)),this,SLOT(tv_doubleclicked(int,int)));
    ui->spinBox_year->setRange(1900,2999);
    ui->spinBox_month->setRange(1,12);
    ui->spinBox_year->setValue(year);
    ui->spinBox_month->setValue(month);
    location = QDir::currentPath();
    QDir temp;
    if(!temp.exists(location+"/dialy")){
        temp.mkdir(location+"/dialy");
    }
    if(!temp.exists(location+"/memo")){
        temp.mkdir(location+"/memo");
    }
    if(!temp.exists(location+"/memo_content")){
        temp.mkdir(location+"/memo_content");
    }
    if(!temp.exists(location+"/tv")){
        temp.mkdir(location+"/tv");
    }
    if(!temp.exists(location+"/music")){
        temp.mkdir(location+"/music");
    }
    if(!temp.exists(location+"/code")){
        temp.mkdir(location+"/code");
    }
    memo_size=0;
    reset_memo();
    reset_tv();
    memo_show();
    tv_show();
    textedit_show();
    get_popup();
    if(popup_flag==1){
        popup a;
        a.exec();
    }

    set_wallpaper();
    set_music();
    QTimer *timer=new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(showTime()));    //signals and slots     (Object1,Signal1 , Object2,Slot1)
    timer->start();
/*****************************************************/
/*Date and time*/
/****************************************************/
    QDate date=QDate::currentDate();   // show  current date
    QString datetext=date.toString();
    ui->date->setText(datetext);



/*****************************************************/
/*weather*/
/*****************************************************/


    manager = new QNetworkAccessManager();
    QObject::connect(manager, &QNetworkAccessManager::finished,
        this, [=](QNetworkReply *reply) {
            if (reply->error()) {
                qDebug() << reply->errorString();
                Display();
                ui->location_2->setText("Not Found");
                 ui->location_3->setText("Not Found");
                return;
            }

            QString jsonString = reply->readAll();

            qDebug() << jsonString;

            displayWeatherData(jsonString);
        }
    );
/*****************************************************/
/*Alarm System*/
/*****************************************************/
    QScreen *screen = QGuiApplication::primaryScreen();
    QRect screenGeometry = screen->geometry();
    int x = (screenGeometry.width()-this->width()) / 2;
    int y = (screenGeometry.height()-this->height()) / 2;
    this->move(x,y);



    // set  up signals and slots

    connect(timer, &QTimer::timeout, this, &MainWindow::showTime_2);
    connect(ui->btnSetAlarm, &QPushButton::clicked, this, &MainWindow::setalarmtime);
    connect(ui->btnEnableAlarm,&QPushButton::clicked,this,&MainWindow::setAlarmStatus);
    connect(ui->btnStopAlarm,&QPushButton::clicked,this,&MainWindow::on_btnStopAlarm_clicked);


    QTime time = QTime::currentTime();
    QString text = time.toString("hh:mm");

    ui->timeLCD->display(text);

    QTime alarmtime = QTime::currentTime();
    text2 = alarmtime.toString("hh:mm");

    ui->alarmLCD->display(text2);
    m_thread = new mthread(this);

    // connect the thread
    connect(m_thread, &mthread::AlarmFired,this,&MainWindow::soundAlarm) ;


    timer->start(1000);

    /***********************************************************/
    /*Music Player*/
    /***********************************************************/


    QPixmap pix("/home/virender/Downloads/ab67706f00000003aa93fe4e8c2d24fc62556cba.jpeg");
        ui->label_pic->setPixmap(pix.scaled(400,350,Qt::KeepAspectRatio));
    ini.loadData();



        wndPath = QDir::current();

        QDir::setCurrent(QDir::homePath());

        playlist = new QMediaPlaylist;

        playlist->setPlaybackMode(QMediaPlaylist::Loop);

        QObject::connect(playlist, SIGNAL(currentIndexChanged(int)), this, SLOT(onCurrentIndexChanged(int)));

        player = new QMediaPlayer;

        player->setPlaylist(playlist);

        QObject::connect(player, SIGNAL(stateChanged(QMediaPlayer::State)), this, SLOT(onChange(QMediaPlayer::State)));
        QObject::connect(player, SIGNAL(positionChanged(qint64)), this, SLOT(onPositionChange(qint64)));
        QObject::connect(player, SIGNAL(durationChanged(qint64)), this, SLOT(onDurationChange(qint64)));

        ui->tracksList->setColumnCount(2);
        ui->tracksList->setRowCount(playlist->mediaCount());

        for(int i = 0; i < playlist->mediaCount(); i++){
            QTableWidgetItem *item = new QTableWidgetItem(playlist->media(i).canonicalUrl().fileName());
            ui->tracksList->setItem(i, 0, item);
        }

        QStringList header;

        header << "Track name" << "duration";

        ui->tracksList->setHorizontalHeaderLabels(header);
        ui->tracksList->setColumnWidth(0, 246);
        ui->tracksList->setEditTriggers(QTableWidget::NoEditTriggers);
        ui->tracksList->setSelectionBehavior(QAbstractItemView::SelectRows);

        ui->loop->setCheckable(true);
        ui->loop->setChecked(true);

        loadFileListFromFile();

        position = new QPositionWidget(player, this->ui->page_2);

        position->setGeometry(60,460,481,10);


        volumeControl   = new QVolumeControl(player, this->ui->page_2);
        volumeControl->setGeometry(280, 10, 40, 80);

        volumeView = new QVolumeView(player, volumeControl, ui->page_2);
        volumeView->setGeometry(10, 10, 202, 72);

        volumeControl->setVolume(ini.getOptionInt("volume"));

        QObject::connect(ui->open, SIGNAL(clicked(bool)), this, SLOT(on_o_triggered()));




}





QString MainWindow::s2q(const std::string &s)
{
    return QString(QString::fromLocal8Bit(s.c_str()));
}

std::string MainWindow::q2s(const QString &s)
{
    return std::string((const char *)s.toLocal8Bit());
}

void MainWindow::set_button_show(QPushButton* button,int a,int b){
    int num=0;
    if (date_different==0 && date_finishdate==0 && date_finishrow==0 && date_startdate==0){
        JulDate test(year,month,1);
        int week = test.getweek();

        int days=JulDate::dayofmonth(year,month);
        if(week==7){
            date_different=1;
            date_startdate=0;
        }
        else {
            date_different=1-week;
            date_startdate=week;
        }
        int gap;
        if(week==7){
            gap=0;
        }
        else gap=week;
        if(days%7-1+gap>6){
            date_finishrow=days/7+1;
            date_finishdate=days%7-8+gap;
        }
        else if(days%7-1+gap<=6){
            date_finishrow=days/7;
            date_finishdate=days%7-1+gap;
        }
    }

    if(a>0 && a<date_finishrow){
        num=a*7+b+date_different;
        button->show();
        button->setText(s2q(std::to_string(num)));
    }
    else if(a==0 && b>=date_startdate){
        num=a*7+b+date_different;
        button->setText(s2q(std::to_string(num)));
        button->show();
    }
    else if(a==date_finishrow && b<=date_finishdate){
        num=a*7+b+date_different;
        button->setText(s2q(std::to_string(num)));
        button->show();
    }
    else {
        button->setText(" ");
        button->hide();
    }
    if(day==num){
        button->setChecked(true);
    }
    else button->setChecked(false);

    if(month==5 && num==6){
        button->setStyleSheet("background-color: rgba(192,192,192,0.8);border:0px;");
    }
    else if(month==6 && num==14){
        button->setStyleSheet("background-color: rgba(192,192,192,0.8);border:0px;");
    }
    else if(month==8 && num==1){
        button->setStyleSheet("background-color: rgba(192,192,192,0.8);border:0px;");
    }
    else if(month==5 && num==23){
        button->setStyleSheet("background-color: rgba(192,192,192,0.8);border:0px;");
    }
    else if(month==4 && num==24){
        button->setStyleSheet("background-color: rgba(192,192,192,0.8);border:0px;");
    }
    else if(month==12 && num==28){
        button->setStyleSheet("background-color:rgba(192,192,192,0.8);border:0px;");
    }
    else{
        button->setStyleSheet("background-color:rgba(192,192,192,0.8);border:0px;");
    }
}

void MainWindow::refresh_allbutton()
{
    MainWindow::set_button_show(ui->pushButton_0_0,0,0);
    MainWindow::set_button_show(ui->pushButton_0_1,0,1);
    MainWindow::set_button_show(ui->pushButton_0_2,0,2);
    MainWindow::set_button_show(ui->pushButton_0_3,0,3);
    MainWindow::set_button_show(ui->pushButton_0_4,0,4);
    MainWindow::set_button_show(ui->pushButton_0_5,0,5);
    MainWindow::set_button_show(ui->pushButton_0_6,0,6);
    MainWindow::set_button_show(ui->pushButton_1_0,1,0);
    MainWindow::set_button_show(ui->pushButton_1_1,1,1);
    MainWindow::set_button_show(ui->pushButton_1_2,1,2);
    MainWindow::set_button_show(ui->pushButton_1_3,1,3);
    MainWindow::set_button_show(ui->pushButton_1_4,1,4);
    MainWindow::set_button_show(ui->pushButton_1_5,1,5);
    MainWindow::set_button_show(ui->pushButton_1_6,1,6);
    MainWindow::set_button_show(ui->pushButton_2_0,2,0);
    MainWindow::set_button_show(ui->pushButton_2_1,2,1);
    MainWindow::set_button_show(ui->pushButton_2_2,2,2);
    MainWindow::set_button_show(ui->pushButton_2_3,2,3);
    MainWindow::set_button_show(ui->pushButton_2_4,2,4);
    MainWindow::set_button_show(ui->pushButton_2_5,2,5);
    MainWindow::set_button_show(ui->pushButton_2_6,2,6);
    MainWindow::set_button_show(ui->pushButton_3_0,3,0);
    MainWindow::set_button_show(ui->pushButton_3_1,3,1);
    MainWindow::set_button_show(ui->pushButton_3_2,3,2);
    MainWindow::set_button_show(ui->pushButton_3_3,3,3);
    MainWindow::set_button_show(ui->pushButton_3_4,3,4);
    MainWindow::set_button_show(ui->pushButton_3_5,3,5);
    MainWindow::set_button_show(ui->pushButton_3_6,3,6);
    MainWindow::set_button_show(ui->pushButton_4_0,4,0);
    MainWindow::set_button_show(ui->pushButton_4_1,4,1);
    MainWindow::set_button_show(ui->pushButton_4_2,4,2);
    MainWindow::set_button_show(ui->pushButton_4_3,4,3);
    MainWindow::set_button_show(ui->pushButton_4_4,4,4);
    MainWindow::set_button_show(ui->pushButton_4_5,4,5);
    MainWindow::set_button_show(ui->pushButton_4_6,4,6);
    MainWindow::set_button_show(ui->pushButton_5_0,5,0);
    MainWindow::set_button_show(ui->pushButton_5_1,5,1);
    MainWindow::set_button_show(ui->pushButton_5_2,5,2);
    MainWindow::set_button_show(ui->pushButton_5_3,5,3);
    MainWindow::set_button_show(ui->pushButton_5_4,5,4);
    MainWindow::set_button_show(ui->pushButton_5_5,5,5);
    MainWindow::set_button_show(ui->pushButton_5_6,5,6);
    ui->spinBox_year->setValue(year);
    ui->spinBox_month->setValue(month);
}

void MainWindow::on_next_month_clicked()
{
    if(year==2999 && month==12){
        month=12;
    }
    else month++;

    if(month>12){
        year++;
        month=1;
    }

    reset_flag();

    set_wallpaper();
    reset_memo();
    textedit_show();
    memo_show();
    refresh_allbutton();
}

void MainWindow::reset_flag()
{
    date_different=0;
    date_finishdate=0;
    date_finishrow=0;
    date_startdate=0;
}

void MainWindow::on_next_year_clicked()
{
    if(year<2999){
        year++;
    }
    else if(year>=2999){
        year=2999;
    }

    reset_flag();
    refresh_allbutton();
    set_wallpaper();
    reset_memo();
    textedit_show();
    memo_show();
}

void MainWindow::on_last_month_clicked()
{
    if(year==1900 && month==1){
        month=1;
    }
    else month--;

    if(month==0){
        year--;
        month=12;
    }

    reset_flag();
    refresh_allbutton();
    set_wallpaper();
    reset_memo();
    textedit_show();
    memo_show();
}

void MainWindow::on_last_year_clicked()
{
    if(year>1900){
        year--;
    }
    else if(year<=1900){
        year=1900;
    }

    reset_flag();
    refresh_allbutton();
    set_wallpaper();
    reset_memo();
    textedit_show();
    memo_show();
}

void MainWindow::on_spinBox_year_editingFinished()
{
    year = ui->spinBox_year->value();
    reset_flag();
    refresh_allbutton();
    set_wallpaper();
    reset_memo();
    textedit_show();
    memo_show();
}


void MainWindow::on_spinBox_month_editingFinished()
{
    month = ui->spinBox_month->value();
    reset_flag();
    refresh_allbutton();
    set_wallpaper();
    reset_memo();
    textedit_show();
    memo_show();
}


void MainWindow::on_pushButton_now_clicked()
{
    time_t timenow=time(NULL);
    tm* Jtime;
    Jtime=localtime(&timenow);
    year=Jtime->tm_year+1900;
    month=Jtime->tm_mon+1;
    day=Jtime->tm_mday;
    reset_flag();
    refresh_allbutton();
    textedit_show();
    memo_show();
}

void MainWindow::textedit_show()
{
    QDir temp;
    if(!temp.exists(location+"/dialy")){
        QMessageBox::about(this,"warning","Can't find the destination folder");
        return;
    }
    if(day<0){
        return;
    }
    date_gonna_print=day;
    QString filename = s2q(std::to_string(year)+"."+std::to_string(month)+"."
                           +std::to_string(date_gonna_print));
    dialyname=filename+".txt";
    QString start_print=s2q(std::to_string(year)+"."+std::to_string(month)+"."
                            +std::to_string(date_gonna_print))+"\n"+"---------------------------------------\n";
    QFile file(location+"/dialy"+"/"+dialyname);
    QString text="";
    if(file.open(QIODevice::ReadOnly|QIODevice::Text)){
        QTextStream text_TS(&file);
        text = text_TS.readAll();
    }
    else text = "";
    file.close();
    QString today_print = start_print + text;

    ui->textEdit->setText(today_print);
}

void MainWindow::memo_show()
{
    QDir temp;
    if(!temp.exists(location+"/memo") || !temp.exists(location+"/memo_content")){
        QMessageBox::about(this,"warning","Can't find the destination folder");
        return;
    }
    if(day<0){
        return;
    }

    date_gonna_print=day;
    QString filename = s2q(std::to_string(year)+"."+std::to_string(month)+"."
                           +std::to_string(date_gonna_print)+".memo");
    QString memoname = filename + ".txt";
    QFile file(location+"/memo"+"/"+memoname);
    int count=0;
    if(file.open(QIODevice::ReadOnly|QIODevice::Text)){
        for(int i=0;!file.atEnd();i++){
            QString str=file.readLine();
            count++;
        }
    }
    file.close();
    memo_size=count/3;
    ui->memo->setRowCount(count/3);
    ui->memo->setColumnCount(3);
    ui->memo->setColumnWidth(0,48);
    ui->memo->setColumnWidth(1,45);
    ui->memo->setColumnWidth(2,110);
    for(int i=0;i<count;i++){
        ui->memo->setRowHeight(i,22);
    }
    ui->memo->verticalHeader()->setVisible(false);
    QStringList horizontitle;
    horizontitle << "time" << "types of" << "information";
    ui->memo->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->memo->setHorizontalHeaderLabels(horizontitle);
    ui->memo->horizontalHeader()->setDefaultAlignment(Qt::AlignCenter);

    if(file.open(QIODevice::ReadOnly|QIODevice::Text)){
        QTextStream read(&file);
        for(int i=0;i<memo_size;i++){
            QString content=read.readLine();
            int hour=0;
            int minute=0;
            int type=0;
            analyze_memo(hour,minute,type,q2s(content));

            memo_num[i][0]=(hour*100)+minute;
            memo_num[i][1]=type;

            QString info_q = read.readLine();

            QString path=read.readLine();
            memo_qs[i][0]=info_q;
            memo_qs[i][1]=path;
            file.close();
        }
    }
    for(int i=0;i<memo_size;i++){
        qDebug()<<memo_num[i][0]<<"||||"<<memo_num[i][1];
    }
    sort_memo();

    for(int i=0;i<memo_size;i++){
        qDebug()<<memo_num[i][0]<<"||||"<<memo_num[i][1];
    }
    for(int i=0;i<memo_size;i++){
        int hour_n=memo_num[i][0]/100;
        int minute_n=memo_num[i][0]-hour_n*100;
        QString time=s2q(std::to_string(hour_n)+":"+std::to_string(minute_n));
        ui->memo->setItem(i,0,new QTableWidgetItem(time));

        QString type_q="";
        if(memo_num[i][1]==0) type_q="daily";
        if(memo_num[i][1]==1) type_q="learn";
        if(memo_num[i][1]==2) type_q="entertainment";
        if(memo_num[i][1]==3) type_q="other";

        ui->memo->setItem(i,1,new QTableWidgetItem(type_q));
        ui->memo->setItem(i,2,new QTableWidgetItem(memo_qs[i][0]));
    }
    ui->memo->show();
}

int MainWindow::get_today(int a, int b)
{
    int num;
    if(a>0 && a<date_finishrow){
        return num=a*7+b+date_different;
    }
    else if(a==0 && b>=date_startdate){
        return num=a*7+b+date_different;
    }
    else if(a==date_finishrow && b<=date_finishdate){
        return num=a*7+b+date_different;
    }
    else return -1;
}

void MainWindow::analyze_memo(int& hour , int& minute , int& type,std::string content)
{
    int hour_flag=0;
    int minute_flag=0;
    int receving=0;
    int type_flag=0;
    std::string hour_s="";
    std::string minute_s="";
    std::string type_s="";

    for(int i=0;content[i]!='\0';i++){
        if(receving==0 && content[i]==' '){
            continue;
        }

        if(hour_flag==0 && (content[i]>='0' && content[i]<='9')){
            hour_s=hour_s+content[i];
            receving=1;
            continue;
        }
        if(hour_flag==0 && content[i]==' ' && receving==1){
            hour_s=hour_s+'\0';
            hour=atoi(hour_s.c_str());
            receving=0;
            hour_flag=1;
            continue;
        }

        if(hour_flag==1 && minute_flag==0 && (content[i]>='0' && content[i]<='9')){
            minute_s=minute_s+content[i];
            receving=1;
            continue;
        }
        if(hour_flag==1 && minute_flag==0 && content[i]==' ' && receving==1){
            minute_s=minute_s+'\0';
            minute=atoi(minute_s.c_str());
            receving=0;
            minute_flag=1;
            continue;
        }

        if(hour_flag==1 && minute_flag==1 && type_flag==0 && content[i]>='0' && content[i]<='3' && receving==0){
            type_s = type_s + content[i] +'\0';
            type=atoi(type_s.c_str());
            type_flag=1;
        }
    }
}

void MainWindow::sort_memo()
{
    for (int j=0;j<memo_size-1;j++){
        for (int i=0;i<memo_size-1-j;i++){
            if (memo_num[i][0]>memo_num[i+1][0]){
                int temp=0;
                temp=memo_num[i][0];
                memo_num[i][0]=memo_num[i+1][0];
                memo_num[i+1][0]=temp;
                temp=memo_num[i][1];
                memo_num[i][1]=memo_num[i+1][1];
                memo_num[i+1][1]=temp;

                swap(memo_qs[i][0],memo_qs[i+1][0]);
                swap(memo_qs[i][1],memo_qs[i+1][1]);
            }
        }
    }
}

void MainWindow::reset_memo()
{
    for(int i=0;i<100;i++){
        for(int j=0;j<2;j++){
            memo_qs[i][j]="";
            memo_num[i][j]=0;
        }
    }
    memo_size=0;
}

void MainWindow::tv_show()
{
    reset_tv();
    QDir temp;
    if(!temp.exists(location+"/tv")){
        QMessageBox::about(this,"warning","Can't find the destination folder");
        return;
    }
    QFile file(location+"/tv"+"/tv.txt");
    int count=0;
    if(file.open(QIODevice::ReadOnly|QIODevice::Text)){
        for(int i=0;!file.atEnd();i++){
            QString str = file.readLine();
            count++;
        }
        file.close();
    }
    tv_size=count/2;
    if(file.open(QIODevice::ReadOnly|QIODevice::Text)){
        QTextStream read(&file);
        for(int i=0;i<tv_size;i++){
            QString content=read.readLine();
            analyze_tv(tv_num[i][0],tv_num[i][1],q2s(content));
            tv_name[i]=read.readLine();
        }
        file.close();
    }
    ui->tv->setRowCount(tv_size);
    ui->tv->setColumnCount(3);
    ui->tv->verticalHeader()->setVisible(false);
    QStringList horizontitle;
    horizontitle << "Play title" << "Season number" << "Episode";
    ui->tv->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tv->setHorizontalHeaderLabels(horizontitle);
    ui->tv->horizontalHeader()->setDefaultAlignment(Qt::AlignCenter);

    ui->tv->setColumnWidth(0,110);
    ui->tv->setColumnWidth(1,48);
    ui->tv->setColumnWidth(2,48);
    for(int i=0;i<tv_size;i++){
        ui->tv->setRowHeight(i,22);
    }
    for(int i=0;i<tv_size;i++){
        ui->tv->setItem(i,0,new QTableWidgetItem(tv_name[i]));
        QString season="";
        QString chapter="";
        if(tv_num[i][1]<0){
            season = "end";
            chapter = "end";
        }
        else {
            season=s2q(std::to_string(tv_num[i][0]));
            chapter=s2q(std::to_string(tv_num[i][1]));
        };
        ui->tv->setItem(i,1,new QTableWidgetItem(season));
        ui->tv->setItem(i,2,new QTableWidgetItem(chapter));
    }
}

void MainWindow::analyze_tv(int& season, int& chapter, std::string content)
{
    int season_flag=0;
    int chapter_flag=0;
    int receving=0;
    std::string season_s="";
    std::string chapter_s="";
    for(int i=0;content[i]!='\0';i++){
        if(receving==0 && content[i]==' '){
            continue;
        }
        if(season_flag==0 && content[i]>='0' && content[i]<='9'){
            season_s=season_s+content[i];
            receving=1;
            continue;
        }
        if(season_flag==0 && content[i]==' ' && receving==1){
            season_s=season_s+'\0';
            season=atoi(season_s.c_str());
            season_flag=1;
            receving=0;
            continue;
        }

        if(season_flag==1 && chapter_flag==0 && content[i]>='0' && content[i]<='9'){
            chapter_s=chapter_s+content[i];
            receving=1;
            continue;
        }
        if(season_flag==1 && chapter_flag==0 && content[i]=='-'){
            chapter=-1;
            return;
        }
    }
    chapter_s=chapter_s+'\0';
    chapter=atoi(chapter_s.c_str());
}

void MainWindow::reset_tv()
{
    for(int i=0;i<300;i++){
        tv_name[i]="";
        for(int j=0;j<2;j++){
            tv_num[i][j]=0;
        }
    }
    tv_size=0;
}

void MainWindow::get_popup()
{
    QFile file(location+"/memo/settings.txt");
    if(file.open(QIODevice::ReadOnly|QIODevice::Text)){
        QTextStream read(&file);
        QString content=read.readLine();
        if(content[0]=='0'){
            popup_flag=0;
        }
        else if(content[0]=='1'){
            popup_flag=1;
        }
        QString content2=read.readLine();
        if(content2[0]=='0'){
            music_flag=0;
        }
        else if(content2[0]=='1'){
            music_flag=1;
        }
        file.close();
    }
}

void MainWindow::set_wallpaper()
{
    if(month==12 && day==28){
        this->setStyleSheet("#MainWindow { border-image: url(:/image/image/77.jpg);}#MainWindow * { border-image:url(); };");
    }
    else if(month==5 && day==6){
        this->setStyleSheet("#MainWindow { border-image: url(:/image/image/66.jpg);}#MainWindow * { border-image:url(); };");
    }
    else if(month==6 && day==14){
        this->setStyleSheet("#MainWindow { border-image: url(:/image/image/66.jpg);}#MainWindow * { border-image:url(); };");
    }
    else if(month==8 && day==1){
        this->setStyleSheet("#MainWindow { border-image: url(:/image/image/66.jpg);}#MainWindow * { border-image:url(); };");
    }
    else if(month==5 && day==23){
        this->setStyleSheet("#MainWindow { border-image: url(:/image/image/66.jpg);}#MainWindow * { border-image:url(); };");
    }
    else if(month==4 && day==24){
        this->setStyleSheet("#MainWindow { border-image: url(:/image/sanchuimg/images.jpg);}#MainWindow * { border-image:url(); };");
    }
    else{
        QTime t;
        t= QTime::currentTime();
        qsrand(t.msec()+t.second()*1000);
        int n = qrand()%10;
        if(n==0){
            this->setStyleSheet("#MainWindow { border-image: url(:/image/image/10.jpg);}#MainWindow * { border-image:url(); };");
        }
        else if(n==1){
            this->setStyleSheet("#MainWindow { border-image: url(:/image/image/1.jpg);}#MainWindow * { border-image:url(); };");
        }
        else if(n==2){
            this->setStyleSheet("#MainWindow { border-image: url(:/image/image/2.jpg);}#MainWindow * { border-image:url(); };");
        }
        else if(n==3){
            this->setStyleSheet("#MainWindow { border-image: url(:/image/image/3.jpg);}#MainWindow * { border-image:url(); };");
        }
        else if(n==4){
            this->setStyleSheet("#MainWindow { border-image: url(:/image/image/4.jpg);}#MainWindow * { border-image:url(); };");
        }
        else if(n==5){
            this->setStyleSheet("#MainWindow { border-image: url(:/image/image/5.jpg);}#MainWindow * { border-image:url(); };");
        }
        else if(n==6){
            this->setStyleSheet("#MainWindow { border-image: url(:/image/image/6.jpg);}#MainWindow * { border-image:url(); };");
        }
        else if(n==7){
            this->setStyleSheet("#MainWindow { border-image: url(:/image/image/7.jpg);}#MainWindow * { border-image:url(); };");
        }
        else if(n==8){
            this->setStyleSheet("#MainWindow { border-image: url(:/image/image/8.jpg);}#MainWindow * { border-image:url(); };");
        }
        else if(n==9){
            this->setStyleSheet("#MainWindow { border-image: url(:/image/image/9.jpg);}#MainWindow * { border-image:url(); };");
        }
    }
}

void MainWindow::set_checked()
{
    ui->pushButton_0_0->setCheckable(true);
    ui->pushButton_0_1->setCheckable(true);
    ui->pushButton_0_2->setCheckable(true);
    ui->pushButton_0_3->setCheckable(true);
    ui->pushButton_0_4->setCheckable(true);
    ui->pushButton_0_5->setCheckable(true);
    ui->pushButton_0_6->setCheckable(true);
    ui->pushButton_1_0->setCheckable(true);
    ui->pushButton_1_1->setCheckable(true);
    ui->pushButton_1_2->setCheckable(true);
    ui->pushButton_1_3->setCheckable(true);
    ui->pushButton_1_4->setCheckable(true);
    ui->pushButton_1_5->setCheckable(true);
    ui->pushButton_1_6->setCheckable(true);
    ui->pushButton_2_0->setCheckable(true);
    ui->pushButton_2_1->setCheckable(true);
    ui->pushButton_2_2->setCheckable(true);
    ui->pushButton_2_3->setCheckable(true);
    ui->pushButton_2_4->setCheckable(true);
    ui->pushButton_2_5->setCheckable(true);
    ui->pushButton_2_6->setCheckable(true);
    ui->pushButton_3_0->setCheckable(true);
    ui->pushButton_3_1->setCheckable(true);
    ui->pushButton_3_2->setCheckable(true);
    ui->pushButton_3_3->setCheckable(true);
    ui->pushButton_3_4->setCheckable(true);
    ui->pushButton_3_5->setCheckable(true);
    ui->pushButton_3_6->setCheckable(true);
    ui->pushButton_4_0->setCheckable(true);
    ui->pushButton_4_1->setCheckable(true);
    ui->pushButton_4_2->setCheckable(true);
    ui->pushButton_4_3->setCheckable(true);
    ui->pushButton_4_4->setCheckable(true);
    ui->pushButton_4_5->setCheckable(true);
    ui->pushButton_4_6->setCheckable(true);
    ui->pushButton_5_0->setCheckable(true);
    ui->pushButton_5_1->setCheckable(true);
    ui->pushButton_5_2->setCheckable(true);
    ui->pushButton_5_3->setCheckable(true);
    ui->pushButton_5_4->setCheckable(true);
    ui->pushButton_5_5->setCheckable(true);
    ui->pushButton_5_6->setCheckable(true);
}

void MainWindow::set_music()
{
    player = new QMediaPlayer();
    player->setMedia(QUrl::fromLocalFile(location+"My Fellas - Arjan Dhillon (DjPunjab.Com).mp3"));
    player->setVolume(100);
    if(music_flag==1){
        player->play();
    }
}

void MainWindow::on_action_triggered()
{
    calculate a;
    a.exec();
}

void MainWindow::on_text_save_clicked()
{
    QFile file(location+"/dialy"+"/"+dialyname);
    if(file.open(QIODevice::WriteOnly)){
        QTextStream out(&file);
        QString content = ui->textEdit->toPlainText();
        content.replace(QString(s2q(std::to_string(year)+"."+std::to_string(month)+"."
                                +std::to_string(date_gonna_print))+"\n"
                                +"---------------------------------------\n"),QString(""));
        content.replace(QString("\n"), QString("\r\n"));
        out<<content;
    }
    file.close();
}

void MainWindow::on_memo_save_clicked()
{
    memo_save a(this,year,month,day);
    a.exec();
    textedit_show();
    memo_show();
}

void MainWindow::memo_doubleclicked(int row, int col)
{
    QString location_memo_c=location+"/memo/"+s2q(std::to_string(year)+"."+std::to_string(month)+"."
                                         +std::to_string(date_gonna_print)+".memo")+".txt";
    int num=year*10000+month*100+day;
    memo_c a(this,memo_num[row][0],memo_num[row][1],memo_qs[row][0],memo_qs[row][1],location_memo_c,num);
    a.exec();
    memo_show();
}

void MainWindow::tv_doubleclicked(int row, int col)
{
    tv a(this,tv_name[row],tv_num[row][0],tv_num[row][1],location+"/tv/tv.txt");
    a.exec();
    tv_show();
}

void MainWindow::on_TV_save_clicked()
{
    tv_input a(this,location+"/tv/tv.txt");
    a.exec();
    tv_show();
}

void MainWindow::on_actionTixing_triggered()
{
    popup a(this);
    a.exec();
    memo_show();
}

void MainWindow::on_pushButton_0_0_clicked()
{
    reset_memo();
    day=get_today(0,0);
    textedit_show();
    memo_show();
    set_wallpaper();
    refresh_allbutton();
}

void MainWindow::on_pushButton_0_1_clicked()
{
    reset_memo();
    day=get_today(0,1);
    textedit_show();
    memo_show();
    set_wallpaper();
    refresh_allbutton();
}

void MainWindow::on_pushButton_0_2_clicked()
{
    reset_memo();
    day=get_today(0,2);
    textedit_show();
    memo_show();
    set_wallpaper();
    refresh_allbutton();
}

void MainWindow::on_pushButton_0_3_clicked()
{
    reset_memo();
    day=get_today(0,3);
    textedit_show();
    memo_show();
    set_wallpaper();
    refresh_allbutton();
}

void MainWindow::on_pushButton_0_4_clicked()
{
    reset_memo();
    day=get_today(0,4);
    textedit_show();
    memo_show();
    set_wallpaper();
    refresh_allbutton();
}

void MainWindow::on_pushButton_0_5_clicked()
{
    reset_memo();
    day=get_today(0,5);
    textedit_show();
    memo_show();
    set_wallpaper();
    refresh_allbutton();
}

void MainWindow::on_pushButton_0_6_clicked()
{
    reset_memo();
    day=get_today(0,6);
    textedit_show();
    memo_show();
    set_wallpaper();
    refresh_allbutton();
}

void MainWindow::on_pushButton_1_0_clicked()
{
    reset_memo();
    day=get_today(1,0);
    textedit_show();
    memo_show();
    set_wallpaper();
    refresh_allbutton();
}

void MainWindow::on_pushButton_1_1_clicked()
{
    reset_memo();
    day=get_today(1,1);
    textedit_show();
    memo_show();
    set_wallpaper();
    refresh_allbutton();
}

void MainWindow::on_pushButton_1_2_clicked()
{
    reset_memo();
    day=get_today(1,2);
    textedit_show();
    memo_show();
    set_wallpaper();
    refresh_allbutton();
}

void MainWindow::on_pushButton_1_3_clicked()
{
    reset_memo();
    day=get_today(1,3);
    textedit_show();
    memo_show();
    set_wallpaper();
    refresh_allbutton();
}

void MainWindow::on_pushButton_1_4_clicked()
{
    reset_memo();
    day=get_today(1,4);
    textedit_show();
    memo_show();
    set_wallpaper();
    refresh_allbutton();
}

void MainWindow::on_pushButton_1_5_clicked()
{
    reset_memo();
    day=get_today(1,5);
    textedit_show();
    memo_show();
    set_wallpaper();
    refresh_allbutton();
}

void MainWindow::on_pushButton_1_6_clicked()
{
    reset_memo();
    day=get_today(1,6);
    textedit_show();
    memo_show();
    set_wallpaper();
    refresh_allbutton();
}

void MainWindow::on_pushButton_2_0_clicked()
{
    reset_memo();
    day=get_today(2,0);
    textedit_show();
    memo_show();
    set_wallpaper();
    refresh_allbutton();
}

void MainWindow::on_pushButton_2_1_clicked()
{
    reset_memo();
    day=get_today(2,1);
    textedit_show();
    memo_show();
    set_wallpaper();
    refresh_allbutton();
}

void MainWindow::on_pushButton_2_2_clicked()
{
    reset_memo();
    day=get_today(2,2);
    textedit_show();
    memo_show();
    set_wallpaper();
    refresh_allbutton();
}

void MainWindow::on_pushButton_2_3_clicked()
{
    reset_memo();
    day=get_today(2,3);
    textedit_show();
    memo_show();
    set_wallpaper();
    refresh_allbutton();
}

void MainWindow::on_pushButton_2_4_clicked()
{
    reset_memo();
    day=get_today(2,4);
    textedit_show();
    memo_show();
    set_wallpaper();
    refresh_allbutton();
}

void MainWindow::on_pushButton_2_5_clicked()
{
    reset_memo();
    day=get_today(2,5);
    textedit_show();
    memo_show();
    set_wallpaper();
    refresh_allbutton();
}

void MainWindow::on_pushButton_2_6_clicked()
{
    reset_memo();
    day=get_today(2,6);
    textedit_show();
    memo_show();
    set_wallpaper();
    refresh_allbutton();
}

void MainWindow::on_pushButton_3_0_clicked()
{
    reset_memo();
    day=get_today(3,0);
    textedit_show();
    memo_show();
    set_wallpaper();
    refresh_allbutton();
}

void MainWindow::on_pushButton_3_1_clicked()
{
    reset_memo();
    day=get_today(3,1);
    textedit_show();
    memo_show();
    set_wallpaper();
    refresh_allbutton();
}

void MainWindow::on_pushButton_3_2_clicked()
{
    reset_memo();
    day=get_today(3,2);
    textedit_show();
    memo_show();
    set_wallpaper();
    refresh_allbutton();
}

void MainWindow::on_pushButton_3_3_clicked()
{
    reset_memo();
    day=get_today(3,3);
    textedit_show();
    memo_show();
    set_wallpaper();
    refresh_allbutton();
}

void MainWindow::on_pushButton_3_4_clicked()
{
    reset_memo();
    day=get_today(3,4);
    textedit_show();
    memo_show();
    set_wallpaper();
    refresh_allbutton();
}

void MainWindow::on_pushButton_3_5_clicked()
{
    reset_memo();
    day=get_today(3,5);
    textedit_show();
    memo_show();
    set_wallpaper();
    refresh_allbutton();
}

void MainWindow::on_pushButton_3_6_clicked()
{
    reset_memo();
    day=get_today(3,6);
    textedit_show();
    memo_show();
    set_wallpaper();
    refresh_allbutton();
}

void MainWindow::on_pushButton_4_0_clicked()
{
    reset_memo();
    day=get_today(4,0);
    textedit_show();
    memo_show();
    set_wallpaper();
    refresh_allbutton();
}

void MainWindow::on_pushButton_4_1_clicked()
{
    reset_memo();
    day=get_today(4,1);
    textedit_show();
    memo_show();
    set_wallpaper();
    refresh_allbutton();
}

void MainWindow::on_pushButton_4_2_clicked()
{
    reset_memo();
    day=get_today(4,2);
    textedit_show();
    memo_show();
    set_wallpaper();
    refresh_allbutton();
}

void MainWindow::on_pushButton_4_3_clicked()
{
    reset_memo();
    day=get_today(4,3);
    textedit_show();
    memo_show();
    set_wallpaper();
    refresh_allbutton();
}

void MainWindow::on_pushButton_4_4_clicked()
{
    reset_memo();
    day=get_today(4,4);
    textedit_show();
    memo_show();
    set_wallpaper();
    refresh_allbutton();
}

void MainWindow::on_pushButton_4_5_clicked()
{
    reset_memo();
    day=get_today(4,5);
    textedit_show();
    memo_show();
    set_wallpaper();
    refresh_allbutton();
}

void MainWindow::on_pushButton_4_6_clicked()
{
    reset_memo();
    day=get_today(4,6);
    textedit_show();
    memo_show();
    set_wallpaper();
    refresh_allbutton();
}

void MainWindow::on_pushButton_5_0_clicked()
{
    reset_memo();
    day=get_today(5,0);
    textedit_show();
    memo_show();
    set_wallpaper();
    refresh_allbutton();
}

void MainWindow::on_pushButton_5_1_clicked()
{
    reset_memo();
    day=get_today(5,1);
    textedit_show();
    memo_show();
    set_wallpaper();
    refresh_allbutton();
}

void MainWindow::on_pushButton_5_2_clicked()
{
    reset_memo();
    day=get_today(5,2);
    textedit_show();
    memo_show();
    set_wallpaper();
    refresh_allbutton();
}

void MainWindow::on_pushButton_5_3_clicked()
{
    reset_memo();
    day=get_today(5,3);
    textedit_show();
    memo_show();
    set_wallpaper();
    refresh_allbutton();
}

void MainWindow::on_pushButton_5_4_clicked()
{
    reset_memo();
    day=get_today(5,4);
    textedit_show();
    memo_show();
    set_wallpaper();
    refresh_allbutton();
}

void MainWindow::on_pushButton_5_5_clicked()
{
    reset_memo();
    day=get_today(5,5);
    textedit_show();
    memo_show();
    set_wallpaper();
    refresh_allbutton();
}

void MainWindow::on_pushButton_5_6_clicked()
{
    day=get_today(5,6);
    reset_memo();

    textedit_show();
    memo_show();
    set_wallpaper();
    refresh_allbutton();
}
void MainWindow::on_pushButton_play_clicked()
{
    player->play();
}

void MainWindow::on_pushButton_pause_clicked()
{
    player->pause();
}

void MainWindow::on_pushButton_stop_clicked()
{
    player->stop();
}

void MainWindow::on_action_2_triggered()
{
    settings a(this);
    a.exec();
}

void MainWindow::on_action_code_triggered()
{
    code a(this);
    a.exec();
}


/***************************************************************/
/*DATE AND TIME*/
/***************************************************************/
void MainWindow:: showTime()
{

QTime time=QTime::currentTime();

QString time_text=time.toString("hh : mm : ss");   // time

ui->Digital_clock->setText(time_text);

}


/***************************************************************/
/*Weather*/
/***************************************************************/
void MainWindow::on_queryButton_2_clicked()
{
    QString query = ui->query_2->text();
    QUrl url("http://api.openweathermap.org/data/2.5/weather");
    QUrlQuery attributes;
    attributes.addQueryItem("q", query);
    attributes.addQueryItem("appid", "3130fe9c95bcd429a94c61a79c4f7c32");
    attributes.addQueryItem("units", "metric");
    url.setQuery(attributes);
    request.setUrl(url);
    manager->get(request);
}

void MainWindow::displayWeatherData(QString jsonString){
    QJsonDocument jsonResponse = QJsonDocument::fromJson(jsonString.toUtf8());
    QJsonObject data = jsonResponse.object();
    QJsonObject dataMain = data["main"].toObject();
    QJsonObject dataWeather = data["weather"].toArray().at(0).toObject();
    QJsonObject dataWind = data["wind"].toObject();
    QJsonObject dataSys = data["sys"].toObject();

    QString city = data["name"].toString();
    QString countryCode = dataSys["country"].toString();
    QString location = city.append(", ").append(countryCode);

    QString weatherCondition = dataWeather["main"].toString();

    int temp = dataMain["temp"].toDouble();
    int feelsLike = dataMain["feels_like"].toDouble();
    int tempMax = dataMain["temp_max"].toDouble();
    int tempMin = dataMain["temp_min"].toDouble();

    int pressure = dataMain["pressure"].toDouble();
    int humidity = dataMain["humidity"].toDouble();

    int visibility = data["visibility"].toDouble();

    int windSpeed = dataWind["speed"].toDouble();
    int windDirDegree = dataWind["deg"].toDouble();

    int sunriseTimestamp = dataSys["sunrise"].toInt();
    int sunsetTimestamp = dataSys["sunset"].toInt();

    QDateTime sunriseDateTime;
    sunriseDateTime.setTime_t(sunriseTimestamp);
    QDateTime sunsetDateTime;
    sunsetDateTime.setTime_t(sunsetTimestamp);

    QDate date = sunriseDateTime.date();
    QTime sunriseTime = sunriseDateTime.time();
    QTime sunsetTime = sunsetDateTime.time();

    ui->query_2->setText(city);

    ui->location_2->setText(location);
    ui->location_3->setText(location);
    ui->date->setText(date.toString());
    ui->weatherlabel->setText(weatherCondition);
    ui->weatherlabel_2->setText(weatherCondition);
    ui->temperature->setText(QString::number(temp));
     ui->temperature_2->setText(QString::number(temp));
    ui->feelsLikeTempValue->setText(QString::number(feelsLike));
    ui->maxTempValue->setText(QString::number(tempMax));
    ui->minTempValue->setText(QString::number(tempMin));
    ui->pressurevalue_2->setText(QString::number(pressure).append(" hPa"));
    ui->humidityValue_2->setText(QString::number(humidity).append("%"));
    ui->visibilityValue_2->setText(QString::number(visibility).append(" m"));
    ui->windSpeedValue_2->setText(QString::number(windSpeed).append(" m/s"));
    ui->windDirValue_2->setText(direction(windDirDegree));
    ui->sunriseValue_2->setText(sunriseTime.toString().remove(5, 3));
    ui->sunsetValue_2->setText(sunsetTime.toString().remove(5, 3));
}

QString MainWindow::direction(int deg){
    QString direction;
    deg += 23;
    deg %= 360;
    if(deg <= 45){
        direction = "North";
    }else if(deg < 90){
        direction = "North-East";
    }else if(deg <= 135){
        direction = "West";
    }else if(deg < 180){
        direction = "South-West";
    }else if(deg <= 225){
        direction = "South";
    }else if(deg < 270){
        direction = "South-West";
    }else if(deg < 315){
        direction = "West";
    }else{
        direction = "North-West";
    }
    return direction;
}

void MainWindow::Display(){
    ui->location_2->setText("WeatherApp");
    ui->location_3->setText("WeatherApp");
    ui->date->setText("");
    ui->weatherlabel->setText("Partly sunny");
    ui->weatherlabel_2->setText("Partly sunny");
    ui->temperature->setText(QString::number(0));
     ui->temperature_2->setText(QString::number(0));
    ui->feelsLikeTempValue->setText(QString::number(0));
    ui->maxTempValue->setText(QString::number(0));
    ui->minTempValue->setText(QString::number(0));
    ui->pressurevalue_2->setText("");
    ui->humidityValue_2->setText("");
    ui->visibilityValue_2->setText("");
    ui->windSpeedValue_2->setText(QString::number(0).append(" m/s"));
    ui->windDirValue_2->setText(direction(0));
    ui->sunriseValue_2->setText("00:00");
    ui->sunsetValue_2->setText("00:00");
}

/********************************************************/
/*Alarm System*/
/*******************************************************/

void MainWindow::  showTime_2(){

    QTime time = QTime::currentTime();
    QString text = time.toString("hh:mm");
    if ((time.second() % 2) == 0)
    {
        text[2] = ' ';
        if(flag==true){
         alarm_time[2]  = ' ';

        }
    }
    else
    {
       alarm_time[2]  = ':';

    }


     if(flag==true){
     alarm_time = hours + alarm_time[2] + minutes ;

     ui->alarmLCD->display(alarm_time);
     updateAlarm();

     }


    ui->timeLCD->display(text);



}
void MainWindow:: updateAlarm()
{
 QTime time = QTime::currentTime();
 QString _now = time.toString("hh:mm");

 if((_now == alarm_time)&&(flag== true))
 {




    if(f_fired==false){
    m_thread->start() ;
    f_fired= true ;
    }


 }


}

void MainWindow::setalarmtime(){

// get the int value from the spingbox


int ihours = ui->spinBoxHrs->value() ;
int iminutes =ui->spinBoxMinutes->value() ;

 if (ihours<10)
 {
   hours = "0"+ QString::number(ihours);
 }

 else
 {
    hours=QString::number(ihours);

 }

 if(iminutes <10 )
 {
   minutes ="0"+ QString::number(iminutes);

 }
 else
 {
    minutes = QString::number(iminutes);
 }


    alarm_time = hours + ':' + minutes ;




ui->alarmLCD->display(alarm_time);
isSetFlag = true ;

}

void MainWindow::setAlarmStatus()
{
   if(isSetFlag){

    QString labeltext = ui->lblalarmstatus->text();

    if(labeltext=="Alarm Switched Off")
    {

    ui->lblalarmstatus->setText("Alarm Switched On");
    QPalette *p = new QPalette;  // change lcd color

    p->setColor(QPalette::WindowText, QColor(255, 0, 0));
    ui->lblalarmstatus->setPalette(*p);
    flag = true ;
    }


    if(labeltext=="Alarm Switched On")
    {

    ui->lblalarmstatus->setText("Alarm Switched Off");
    QPalette *p = new QPalette;  // change lcd color

    p->setColor(QPalette::WindowText, QColor(0, 0, 255));
    ui->lblalarmstatus->setPalette(*p);
    alarm_time = hours + ':' + minutes ;
    ui->alarmLCD->display(alarm_time);
    flag = false ;
    }

 }
   else
   {


       QMessageBox *msgBox = new QMessageBox(this);
       msgBox->setText("You havent set an alarm time ");
       msgBox->exec();



   }


}



void MainWindow::on_btnStopAlarm_clicked()
{
 alarm_time ="00:00" ;
 ui->lblalarmstatus->setText("Alarm Switched Off");
 QPalette *p = new QPalette;  // change lcd color

 p->setColor(QPalette::WindowText, QColor(0, 0, 255));
 ui->lblalarmstatus->setPalette(*p);
 ui->alarmLCD->display(alarm_time);
 flag =false ;
 isSetFlag =false ;
 m_thread->stop= true ;
 f_fired= false ;

}

void MainWindow::soundAlarm(){

    QSound::play(":/bells.wav");
}

/**********************************************************/
/*Music player*/
/**********************************************************/


void MainWindow::loadFileListFromFile(){
    QString filePath = this->wndPath.filePath("tracklist");

    QFile file(filePath);

    if(file.open(QIODevice::ReadOnly | QIODevice::Text)){
        QTextStream readList(&file);

        QStringList files;

        while(!readList.atEnd()){
            QString mp3path = readList.readLine();

            if(QFile::exists(mp3path)){
                QFileInfo fi(mp3path);

                if(fileExtensionsList.indexOf(QRegularExpression(fi.suffix())) != -1){
                    files.push_back(mp3path);
                }
            }
        }
        if(files.count()){
            loadFileList(files);
        }
    }
}

void MainWindow::on_duration_windowIconTextChanged(const QString &iconText)
{

}

void MainWindow::onPositionChange(qint64 position){

    this->ui->position->setText(" " + getStringTime(position));
    this->ui->timeToEnd->setText("-" + getStringTime(player->duration() - position));
}

void MainWindow::onDurationChange(qint64 duration){
    this->ui->duration->setText(" " + getStringTime(duration));

    QTableWidgetItem *item = new QTableWidgetItem(getStringTime(duration));
    ui->tracksList->setItem(playlist->currentIndex(), 1, item);
}

void MainWindow::on_play_clicked(bool checked)
{

}

void MainWindow::onChange(QMediaPlayer::State state){
    switch(state){
        case QMediaPlayer::StoppedState:
            {
                ui->play->setText(QString("▶"));
            }
            break;
    }
}


void MainWindow::on_play_clicked()
{
    if(!playlist->mediaCount())
        return ;
    if(player->state() == QMediaPlayer::PlayingState){
        player->pause();
        ui->play->setText(QString("▶"));
    }else{
        player->play();
        ui->play->setText(QString("⏸"));
    }
}


void MainWindow::on_next_clicked()
{
    playlist->next();
}

void MainWindow::on_previous_clicked()
{
    playlist->previous();
}

void MainWindow::on_forward5s_clicked()
{
    player->setPosition(player->position() + 5000);
}

void MainWindow::on_back5s_clicked()
{
    player->setPosition(player->position() > 5000 ? player->position() - 5000 : 0);
}

void MainWindow::loadFileList(QStringList &fileList){
    if(player->state() == QMediaPlayer::PlayingState)
        on_play_clicked();
    this->playlist->clear();

    ui->tracksList->setRowCount(fileList.count());

    for(int i = 0; i < fileList.count(); i++){
        QTableWidgetItem *item = new QTableWidgetItem(QFileInfo(fileList.at(i)).baseName());
        ui->tracksList->setItem(i, 0, item);
        item = new QTableWidgetItem("--:--:--");
        ui->tracksList->setItem(i, 1, item);
    }

    foreach(QString pathToFile, fileList){
        playlist->addMedia(QUrl::fromLocalFile(pathToFile));
    }

    for(int i = 0; i < playlist->mediaCount() + 1; i++){
        playlist->next();
    }

    on_play_clicked();
}

void MainWindow::onCurrentIndexChanged(int position){
    ui->tracksList->selectRow(position);
}

void MainWindow::on_tracksList_doubleClicked(const QModelIndex &index)
{
    playlist->setCurrentIndex(index.row());
    ui->tracksList->selectRow(index.row());
}



void MainWindow::on_loop_clicked(bool checked)
{
    ui->loop->setChecked(checked);

    if(checked){
        playlist->setPlaybackMode(QMediaPlaylist::Loop);
    }else{
        playlist->setPlaybackMode(QMediaPlaylist::Sequential);
    }
}

void MainWindow::on_o_triggered()
{
    QString path = ini.getOption("currentPath");
    QStringList list = QFileDialog::getOpenFileNames(this, "Open mp3 files", path, "Audio file (*." + fileExtensionsList.join(" *.").trimmed() + ")" );
    if(list.count()){
        QFileInfo fi(list[0]);
        ini.setOption("currentPath", fi.absolutePath());

        loadFileList(list);
    }
}

void MainWindow::on_actionAuthor_triggered()
{
    QMessageBox msg(this);

    msg.setText("Author: Paras Dhand \nPage: http://www.obliczeniowo.com.pl/896");

    msg.exec();
}



MainWindow::~MainWindow()
{
    QString filePath = this->wndPath.filePath("tracklist");

    if(playlist->mediaCount()){
        QFile file(filePath);
        QTextStream write(&file);
        write.setCodec("UTF-8");

        if(file.open(QIODevice::WriteOnly | QIODevice::Text)){
            for(int i = 0; i < playlist->mediaCount(); i++){
                write << (playlist->media(i).canonicalUrl().path() + "\n");
            }
        }
    }

    ini.setOption("volume", volumeControl->volume());
    ini.saveData();

    delete playlist;
    delete player;
    delete ui;
}


void MainWindow::on_pushButton_2_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}

void MainWindow::on_pushButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
}

void MainWindow::on_pushButton_3_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}
