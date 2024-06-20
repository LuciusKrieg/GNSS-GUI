#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>


#include "interface.h"

#define L1 1602+0.5625
#define L2 1246+0.4375
#define L3 1202.25


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    QString sun_file;
    QString power_file;
    QString spectrum_file;
    short** range;
    bool flag;

signals:
    void sendSun(sun*);
    void sendPower(power*);
    void sendSpectrum(spectrum*);

private slots:
    void on_Calibration_B_clicked();

    void on_Spectrum_B_clicked();

    void on_Power_B_clicked();

    void proc();

    int find_pos(double* M, double f, int size){
        if(size < 0){
            std::cout<<"size is below null";
            return 0;
        }
        int i = 0;
        if((M[(int)(size/2)])<f){
            while(f>(M[(int)(size/2)+i])){
                i = i + 1;
                //cout<<info->Frequency[(info->lines/2)+i]<<endl;
            }
            return (size/2)+i;
        }
        else if((M[(int)(size/2)])>f){
            while(f<M[(int)(size/2)-i]){
                i = i + 1;
            }
            return (size/2)-i;
        }
        else return (size)/2;
    }

    void dif(double**);

    void on_action_2_triggered();

    void on_Save_B_clicked();

    void on_action_3_triggered();

    void recieveData1(QString str);

    void recieveData2(QString str);

    void on_action_5_triggered();

    void addTimer();

    void on_action_4_triggered();


private:
    Ui::MainWindow *ui;
    sun* sun_ = nullptr;
    power* power_ = nullptr;
    spectrum* spectrum_ = nullptr;
};

#endif // MAINWINDOW_H
