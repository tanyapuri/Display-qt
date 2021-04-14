#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "juldate.h"
#include "calculate.h"
#include "memo_save.h"
#include "memo_c.h"
#include "tv.h"
#include "tv_input.h"
#include "popup.h"
#include "settings.h"
#include "code.h"

#include <QMainWindow>
#include <QPushButton>
#include <string>
#include <QtMultimedia/QMediaPlayer>
#include <QUrl>
#include <QStackedWidget>
/***********************************************/
/*Weather*/
/**********************************************/
#include <QWidget>
#include "QtNetwork/QNetworkAccessManager"
#include "QtNetwork/QNetworkRequest"

/***********************************************/
/*Alarm System*/
/**********************************************/

#include <QWidget>
#include <QTimer>
#include <QTime>
#include <QRect>
#include <QDesktopWidget>
#include <QDebug>
#include <QScreen>
#include "mthread.h"
#include <QtMultimedia/QMediaPlayer>
#include <QtMultimedia/QSound>

/***********************************************/
/*Music Player*/
/**********************************************/

#include <QMediaPlayer>
#include <QAudioProbe>
#include <QMediaMetaData>

#include <QMessageBox>
#include <QAudioBuffer>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QMediaPlaylist>
#include <QFileDialog>

#include <QtCore>
#include <QtGui>

#include "initialize.h"

#include "qpositionwidget.h"
#include "qvolumecontrol.h"
#include "qvolumeview.h"


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow , public JulDate
{
    Q_OBJECT

    /****************************************************/
    /*Music Player */
    /****************************************************/

    Initialize ini;

    QMediaPlayer *player;

    QMediaPlaylist *playlist;

    QVolumeControl  *volumeControl;
    QVolumeView     *volumeView;

    QDir wndPath;

    QPositionWidget *position;

    QStringList fileExtensionsList = {"mp3", "ogg", "wma"};

    void loadFileList(QStringList &fileList);

    void loadFileListFromFile();

public:

    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    QString s2q(const std::string &s);

    std::string q2s(const QString &s);

    void set_button_show(QPushButton* button, int a, int b);

    void textedit_show();

    void memo_show();

    int get_today(int,int);

    void analyze_memo(int&,int&,int&,std::string);

    void sort_memo();

    void reset_memo();

    void tv_show();

    void analyze_tv(int& season, int& chapter, std::string content);

    void reset_tv();

    void get_popup();

    void set_wallpaper();

    void set_checked();

    void set_music();

    /****************************************/
    /*AlarmSystem*/
    /****************************************/
    QString text2 ;
    QString alarm_time  ;
    QString minutes ="00";
    QString hours ="00";
    bool flag = false;
    bool isSetFlag = false ;
    mthread *m_thread ;
    bool f_fired = false ;

public slots:
    void onChange(QMediaPlayer::State state);

    void onPositionChange(qint64 position);
    void onDurationChange(qint64 duration);
private slots:

    void on_pushButton_now_clicked();

    void refresh_allbutton();

    void on_next_month_clicked();

    void reset_flag();

    void on_next_year_clicked();

    void on_last_month_clicked();

    void on_last_year_clicked();

    void on_spinBox_year_editingFinished();

    void on_spinBox_month_editingFinished();

    void on_action_triggered();

    void on_pushButton_0_0_clicked();

    void on_text_save_clicked();

    void on_memo_save_clicked();

    void memo_doubleclicked(int row, int col);

    void tv_doubleclicked(int row,int col);

    void on_TV_save_clicked();

    void on_actionTixing_triggered();

    void on_pushButton_4_2_clicked();

    void on_pushButton_0_1_clicked();

    void on_pushButton_0_2_clicked();

    void on_pushButton_0_3_clicked();

    void on_pushButton_0_4_clicked();

    void on_pushButton_0_5_clicked();

    void on_pushButton_0_6_clicked();

    void on_pushButton_1_0_clicked();

    void on_pushButton_1_1_clicked();

    void on_pushButton_1_2_clicked();

    void on_pushButton_1_3_clicked();

    void on_pushButton_1_4_clicked();

    void on_pushButton_1_5_clicked();

    void on_pushButton_1_6_clicked();

    void on_pushButton_2_0_clicked();

    void on_pushButton_2_1_clicked();

    void on_pushButton_2_2_clicked();

    void on_pushButton_2_3_clicked();

    void on_pushButton_2_4_clicked();

    void on_pushButton_2_5_clicked();

    void on_pushButton_2_6_clicked();

    void on_pushButton_3_0_clicked();

    void on_pushButton_3_1_clicked();

    void on_pushButton_3_2_clicked();

    void on_pushButton_3_3_clicked();

    void on_pushButton_3_4_clicked();

    void on_pushButton_3_5_clicked();

    void on_pushButton_3_6_clicked();

    void on_pushButton_4_0_clicked();

    void on_pushButton_4_1_clicked();

    void on_pushButton_4_3_clicked();

    void on_pushButton_4_4_clicked();

    void on_pushButton_4_5_clicked();

    void on_pushButton_4_6_clicked();

    void on_pushButton_5_0_clicked();

    void on_pushButton_5_1_clicked();

    void on_pushButton_5_2_clicked();

    void on_pushButton_5_3_clicked();

    void on_pushButton_5_4_clicked();

    void on_pushButton_5_5_clicked();

    void on_pushButton_5_6_clicked();

    void on_pushButton_play_clicked();

    void on_pushButton_pause_clicked();

    void on_pushButton_stop_clicked();

    void on_action_2_triggered();

    void on_action_code_triggered();


    /***********************************/
    /*DATE AND TIME*/
    /***********************************/
    void showTime();



    /***********************************/
    /*weather*/
    /***********************************/
     void on_queryButton_2_clicked();



     /***********************************/
     /*alarm system*/
     /***********************************/
    void showTime_2();
    void updateAlarm();
    void setalarmtime();
    void setAlarmStatus();
    void on_btnStopAlarm_clicked();
    void soundAlarm();


    /***********************************************/
    /*Music player*/
    /***********************************************/

    void on_duration_windowIconTextChanged(const QString &iconText);

    void on_play_clicked(bool checked);

    void on_play_clicked();

    void on_next_clicked();

    void on_previous_clicked();

    void on_forward5s_clicked();

    void on_back5s_clicked();

    void on_tracksList_doubleClicked(const QModelIndex &index);

    void onCurrentIndexChanged(int position);

    void on_loop_clicked(bool checked);

    void on_o_triggered();

    void on_actionAuthor_triggered();


    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

    void on_pushButton_3_clicked();


    /***********************************************/
    /*stacked widget*/
    /***********************************************/


private:

    Ui::MainWindow *ui;
    QString location;
    //JulDate date=JulDate(2001,10,1,0,0,0);
    int year=1993;
    int month=12;
    int day=1;
    int date_gonna_print=0;
    int date_different=0;
    int date_startdate=0;
    int date_finishrow=0;
    int date_finishdate=0;
    //
    QString dialyname;

    int memo_size=0;
    int memo_num[100][2]={0};
    QString memo_qs[100][2];

    QString tv_name[300];
    int tv_num[300][2]={0};
    int tv_size=0;

    int popup_flag=-1;
    int music_flag=-1;



    /*********************************/
    /*Weather*/
    /*********************************/

    QNetworkAccessManager *manager;
    QNetworkRequest request;

    void displayWeatherData(QString jsonString);
    QString direction(int deg);
    void Display();
};

#endif // MAINWINDOW_H
