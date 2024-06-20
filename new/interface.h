#ifndef INTERFACE_H
#define INTERFACE_H
#include <cstring>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <iomanip>
#include <iterator>
#include <stdlib.h>
#include <malloc.h>


class Interface{
public:
    Interface(std::string);
    Interface();
    ~Interface();
    void out(double*);
    void out(double**);
    int rows;
    int cols;
    int heap;
protected:
    double** Calibration_Data;
    std::string filename;
    int find_pos(double*, double);
private:
    std::string devider(std::string);
    void get_size();
};

class spectrum : public Interface{
public:
    spectrum(std::string s);
    ~spectrum();
    void file_writting(double**, std::string);
    void file_writting(double**, double*, std::string s);
    void writting(std::string);
    double* F;
    int RBW;
    double** P;
    double** Power;
    double* T;
    double* calculate(double, double);

    //connect(s, SIGNAL(sendData(QString)), this, SLOT(recieveData2(QString)));

};

class sun : public Interface{
public:
    sun(std::string s);
    void on_Calibration_B_clicked();
    ~sun();
    //void file_writting() override;
    double** L;
    double** G; //коэф усилени антенны
};

class power : public Interface{
public:
    power(std::string);
    ~power();
    //void file_writting() override;
    double*** L;
    double** D;
    int** S;

};


#endif // INTERFACE_H
