#ifndef SPEC_H
#define SPEC_H

#include <QWidget>
#include "qcustomplot.h"
#include "interface.h"
#include <QKeyEvent>
#include <QMouseEvent>
#include <QEvent>
#include <iostream>
namespace Ui {
class Spec;
}

class Spec : public QWidget
{
    Q_OBJECT

public:
    explicit Spec(QWidget *parent = nullptr);
    ~Spec();
    QString spec_file;
    short flag;
    int time = 0;

signals:
    void sendData(QString);

private slots:
    void on_checkBox_clicked(bool checked);

    void on_pushButton_clicked();

    void adding();

    void on_pushButton_2_clicked();

    void setupPlot();

    void recieveSpectrum(spectrum *x);

    void xAxisChanged(QCPRange range);

    void yAxisChanged(QCPRange range);

    bool event(QEvent *event);

    void wheelEvent(QWheelEvent *w);

    void slotMousePress(QMouseEvent * event);
    void slotMouseMove(QMouseEvent * event);

    void on_spinBox_valueChanged(int arg1);

    void on_checkBox_2_clicked(bool checked);

private:
    Ui::Spec *ui;
    spectrum *spectrum___;
    QCPItemTracer *tracer;
};

#endif // SPEC_H
