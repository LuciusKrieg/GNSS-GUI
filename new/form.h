#ifndef FORM_H
#define FORM_H

#include <QWidget>
#include "qcustomplot.h"
#include "interface.h"
#include <QKeyEvent>
#include <QMouseEvent>
#include <QEvent>
#include <iostream>


namespace Ui {
class Form;
}

class Form : public QWidget
{
    Q_OBJECT

public:
    explicit Form(QWidget *parent = nullptr);

    ~Form();

    short flag = 0;
    short count = 0;
private slots:
    void recievePower(power *x);
    void recieveSun(sun *x);
    void setupPlot();
    void horzScrollBarChanged(int value);
    void vertScrollBarChanged(int value);
    void recieveSpectrum(spectrum *x);
    void xAxisChanged(QCPRange range);
    void yAxisChanged(QCPRange range);
    void wheelEvent(QWheelEvent *w);
    void on_checkBox_toggled(bool checked);
    bool event(QEvent *event);

    void slotMousePress(QMouseEvent * event);
    void slotMouseMove(QMouseEvent * event);

    void on_comboBox_currentIndexChanged(int index);

private:
    QCPCurve *verticalLine;
    QCPCurve *horisontalLine;    // Объявляем объект для вертикальной линии
    QCPItemTracer *tracer;      // Трасировщик по точкам графика

    Ui::Form *ui;
    spectrum *spectrum__;
    spectrum *spectr_;
    power *power__;
    sun *sun__;
};

#endif // FORM_H
