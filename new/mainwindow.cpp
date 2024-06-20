#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "form.h"
#include "range.h"
#include "spec.h"


//CONSTRUCTORS

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    flag = 0;
    Range* r = new Range();
    range = new short*[3];
    for(int i = 0; i < 3; i++){
        range[i] = new short[3];
        for(int j = 0; j < 3; j++){
            range[i][j] = r->l[i][j];
        }
    }

    ui->setupUi(this);

    Form* f = new Form();
    Spec* s = new Spec();



    connect(ui->Calculate_B, SIGNAL(clicked()), this, SLOT(proc()));
    connect(ui->action, SIGNAL(triggered()), r, SLOT(show()));
    connect(ui->action_4, SIGNAL(triggered()), s, SLOT(show()));

    connect(r, SIGNAL(sendData(QString)), this, SLOT(recieveData1(QString)));
    connect(s, SIGNAL(sendData(QString)), this, SLOT(recieveData2(QString)));

    connect(ui->action_5, SIGNAL(triggered()), f, SLOT(show()));
    connect(ui->action_5, SIGNAL(triggered()), f, SLOT(setupPlot()));
    connect(ui->action_4, SIGNAL(triggered()), s, SLOT(setupPlot()));
    connect(this, SIGNAL(sendSpectrum(spectrum*)), f, SLOT(recieveSpectrum(spectrum*)));
    connect(this, SIGNAL(sendSpectrum(spectrum*)), s, SLOT(recieveSpectrum(spectrum*)));
    connect(this, SIGNAL(sendSun(sun*)), f, SLOT(recieveSun(sun*)));
    connect(this, SIGNAL(sendPower(power*)), f, SLOT(recievePower(power*)));
    //connect(, SIGNAL(sendFTP(double*, double*, double**), f, SLOT(recieveFTP(double*,double*,double**))));
    ui->progressBar->hide();



}

//DESTRUCTORS

MainWindow::~MainWindow()
{
    delete ui;
}

//FUNCTIONS
void MainWindow::on_Calibration_B_clicked()
{
    ui->Calibration_L->setText(QFileDialog::getOpenFileName(0, "Выбор файла калибровки по солнцу", ""));
}

void MainWindow::on_Spectrum_B_clicked()
{
    ui->Spectrum_L->setText(QFileDialog::getOpenFileName(0, "Выбор файла спектра", ""));
}

void MainWindow::on_Power_B_clicked()
{
    ui->Power_L->setText(QFileDialog::getOpenFileName(0, "Выбор файла мощности", ""));
}

void MainWindow::dif(double** C){
    C = 0;
}

void MainWindow::proc(){
    ui->progressBar->show();
    double* mean = new double[9];
    double L[3] = {L1, L2, L3};
    double** C = new double*[9];

    sun_      = new sun(ui->Calibration_L->text().toStdString());     ui->progressBar->setValue(10);
    power_    = new power(ui->Power_L->text().toStdString());         ui->progressBar->setValue(30);
    spectrum_ = new spectrum((ui->Spectrum_L->text().toStdString())); ui->progressBar->setValue(50);
    if(flag){
        spectrum_->writting(spectrum_file.toStdString());
    }

    double* t = new double[spectrum_->cols];
    double* k = new double[spectrum_->cols];

    for(int j = 0; j < 3; j++){
        for(int i = 0; i < 3; i++){

            std::cout<<"Calculating in range: "
                     <<range[j][i]
                     <<" = "
                     <<(L[i] - range[j][i]) * 1000000
                     <<" - "
                     <<(L[i] + range[j][i]) * 1000000
                     <<"\t for C["<<j*3+i<<"]"
                     <<'\n';

            C[j*3+i] = new double[spectrum_->rows];
            C[j*3+i] = spectrum_->calculate((L[i]-range[j][i])*1000000,(L[i]+range[j][i])*1000000);
        }
    }
    ui->progressBar->setValue(70);

    for(int j = 0; j < 9; j++){
        double y1, y2, t1, t2;
        for(int i = 0; i < spectrum_->cols; i++){
            y1 = power_->D[i][j%3];
            y2 = power_->D[i][(j%3+3)];
            t1 = power_->S[i][0];
            t2 = power_->S[i][1];
            t[i] = (int)((power_->S[i][1] + power_->S[i][0])/2);
            k[i] = y2+((y1-y2)/(t1-t2))*((t2+t1)/2 - t2);
            C[j][i] = C[j][i] - k[i] + sun_->L[(j%3+1)][0] - 30 - sun_->G[(j%3+1)][0];
        }
    }
    std::cout<<"Normalized\n";
    double** d = new double*[3];
        for(int i = 0; i < 3; d[i] = new double[spectrum_->cols], i++);
    double* dt = new double[spectrum_->cols];

    for(int j = 0; j < 3; j++){
        double y1, y2, t1, t2;
        for(int i = 0; i < spectrum_->cols-1; i++){
            y1 = C[j][i];
            y2 = C[j][i+1];
            t1 = t[i];
            t2 = t[i+1];
            d[j][i] = (y2-y1)/(t2-t1);
            dt[i] = t2 - t1;
        }
    }
    std::cout<<"Deiffed\n";
    for(int j = 0; j < 9; j++){
        double param = 0;
        for(int i = 0; i < spectrum_->cols; i++){

            param = C[j][i] + param;
        }
        mean[j] = param / spectrum_->cols;
        std::cout << "Mean["
                  << j
                  << "] = "
                  << mean[j]
                  << "\n";
    }
    double K = ui->doubleSpinBox->value();
    for(int j = 0; j < 3; j++){
        for(int i = 0; i < spectrum_->cols-1; i++){
            if(d[j][i] >= K/dt[i]){
                double u[2] = {C[j][i],C[j+6][i]};
                while(abs(d[j][i]) >= K/dt[i]){
                    std::cout<<"C["<<j<<"]["<<i<<"] = " << C[j][i] << " -> "<<"C["<<j+6<<"]["<<i<<"] = " << C[j+6][i] << "\t";
                    C[j][i] = C[6+j][i]+1*abs(u[0]-u[1]);
                    std::cout << "ABS: "
                              << abs(d[j][i])
                              << "\tTime: "
                              << t[i]
                              << "\n";
                    i++;
                }
            }
        }
        std::cout<<"\nExit\n";
    }

    spectrum_->Power = C;
    {
        std::string s = "F:\\Files\\dif.dat";
        std::ofstream wfile(s.c_str());
        for(int i = 0; i < spectrum_->cols-1; i++){
            wfile << std::setprecision(10)
                  << t[i]
                  << ",\t";
            for(int j = 0; j < 3; j++){
                wfile << std::setprecision(10)
                      << d[j][i]
                      << ",\t";
            }
            wfile << "\n";
        }
        wfile.close();
    }
    ui->progressBar->setValue(90);

    //spectrum_->file_writting(d, t, ui->Save_L->text().toStdString());
    spectrum_->file_writting(spectrum_->Power, t, ui->Save_L->text().toStdString());
    ui->progressBar->setValue(100);
    ui->progressBar->hide();

}

void MainWindow::on_action_2_triggered()
{

}

void MainWindow::on_Save_B_clicked()
{
    ui->Save_L->setText(QFileDialog::getOpenFileName(nullptr, "Выбор файла сохранения", ""));
}

void MainWindow::on_action_3_triggered()
{
    this->~MainWindow();
}

void MainWindow::recieveData1(QString str){
    char *s = new char[27];
    strcpy_s(s,54,str.toStdString().c_str());
    char *p = new char[9];
    p = strtok(s," ");
    for(int i = 0; p != 0; i++){
        for(int j = 0; j < 3; j++) {
            std::string st(p);
            range[i][j] = atoi(st.c_str());
            //std::cout<<"Range["<<i<<"]["<<j<<"] = "<<range[i][j]<<std::endl;
            p = strtok(0, " ");
        }
    }
}

void MainWindow::recieveData2(QString str){
    spectrum_file = str;
    flag = 1;
}

void MainWindow::on_action_5_triggered()
{
    emit sendSun(sun_);
    emit sendSpectrum(spectrum_);
    emit sendPower(power_);

}

void MainWindow::addTimer()
{

}
/*
void MainWindow::on_checkBox1_toggled(bool checked)
{

}



void MainWindow::on_checkBox_triggered(bool checked)
{
    ui->Save_L->setEnabled(checked);
    ui->label->setEnabled(checked);
    ui->Save_B->setEnabled(checked);
}
*/

void MainWindow::on_action_4_triggered()
{
    emit sendSpectrum(spectrum_);
}

