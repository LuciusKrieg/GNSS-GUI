#include "spec.h"
#include "ui_spec.h"

Spec::Spec(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Spec)
{
    spec_file = "spectrum.dat";
    ui->setupUi(this);

    connect(ui->plot, &QCustomPlot::mousePress, this, &Spec::slotMousePress);
    connect(ui->plot, &QCustomPlot::mouseMove, this, &Spec::slotMouseMove);
    //connect(ui->plot->xAxis, SIGNAL(rangeChanged(QCPRange)), this, SLOT(xAxisChanged(QCPRange)));
    //connect(ui->plot->yAxis, SIGNAL(rangeChanged(QCPRange)), this, SLOT(yAxisChanged(QCPRange)));
    connect(ui->plot, &QCustomPlot::mouseWheel, this, &Spec::wheelEvent);
    tracer = new QCPItemTracer(ui->plot);
}

Spec::~Spec()
{
    delete ui;
}
void Spec::slotMousePress(QMouseEvent *event)
{
    // Определяем координату X на графике, где был произведён клик мышью
    double coordX = ui->plot->xAxis->pixelToCoord(event->pos().x());
    double coordY = ui->plot->yAxis->pixelToCoord(event->pos().y());
    //std::cout<<"a";
    // Подготавливаем координаты по оси X для переноса вертикальной линии
    QVector<double> x(2), y(2);
    x[0] = coordX;
    y[0] = coordY;
    x[1] = coordX;
    y[1] = coordY;

    // Устанавливаем новые координаты

    // По координате X клика мыши определим ближайшие координаты для трассировщика

    tracer->setGraphKey(coordX);

    // Выводим координаты точки графика, где установился трассировщик, в lineEdit
    ui->label_2->setText("X: " + QString::number(tracer->position->key()));
    ui->label_3->setText("Y: " + QString::number(tracer->position->value()));
    ui->plot->replot(); // Перерисовываем содержимое полотна графика
}

void Spec::slotMouseMove(QMouseEvent *event)
{
    /* Если при передвижении мыши, ей кнопки нажаты,
     * то вызываем отработку координат мыши
     * через слот клика
     * */
    if(QApplication::mouseButtons() && ui->checkBox_2->isChecked()) slotMousePress(event);
}

void Spec::adding(){

}

void Spec::setupPlot()
{
    std::cout<<"stdUped"<<std::endl;

        // Добавляем линию на полотно

    ui->plot->xAxis->setRange(0, 6, Qt::AlignCenter);
    ui->plot->yAxis->setRange(0, 10, Qt::AlignCenter);
    // The following plot setup is mostly taken from the plot demos:
    //std::cout<<"\n\n\n\t\t\tCOLS"<<I[0]->cols<<"\n\n\n";
    ui->plot->addGraph();
    ui->plot->graph()->setPen(QPen(Qt::blue));
    ui->plot->graph()->setBrush(QBrush(QColor(0, 0, 255, 20)));
    time = ui->spinBox->value();
    int size = spectrum___->rows;
        QVector<double> x(size), y0(size), y1(size);
        for (int i=0; i<size; ++i)
        {
            x[i] = spectrum___->F[i];
            y1[i] = spectrum___->P[i][time];

            std::cout<<"y("<<x[i]<<") = "<<y1[i]<<std::endl;// exponentially decaying cosine             // exponential envelope
        }
    //ui->plot->graph(0)->setData(x, y0);
    ui->plot->graph(0)->setData(x, y1);



    ui->plot->rescaleAxes();
    //customPlot->graph(1)->rescaleAxes(true);
    ui->plot->axisRect()->setupFullAxesBox(true);
    ui->plot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
    ui->plot->xAxis->setLabel("Frequency, Hz");
    ui->plot->yAxis->setLabel("Power, dBm");

    ui->plot->replot();
}

void Spec::on_checkBox_clicked(bool checked)
{
    if(checked){
        ui->label->setEnabled(1);
        ui->lineEdit->setEnabled(1);
        ui->pushButton->setEnabled(1);
    }
    else{
        ui->lineEdit->setText(spec_file);
        ui->label->setEnabled(0);
        ui->lineEdit->setEnabled(0);
        ui->pushButton->setEnabled(0);
    }
}

void Spec::on_pushButton_clicked()
{
    ui->lineEdit->setText(QFileDialog::getOpenFileName(0, "Выбор файла сохранения спектра", ""));
}

void Spec::on_pushButton_2_clicked()
{
    if(ui->checkBox->isChecked()){
        emit sendData(ui->lineEdit->text());
    }
    Spec::hide();
}
void Spec::wheelEvent(QWheelEvent *w)
{
    w = nullptr;
    if(flag == 1){
        ui->plot->axisRect(0)->setRangeZoom(Qt::Horizontal);
        //ui->plot->xAxis->scaleRange(0.1);
        //ui->plot->xAxis->rescale(true);

    }
    else if(flag == 2 ){
        ui->plot->axisRect(0)->setRangeZoom(Qt::Vertical);
        //ui->plot->yAxis->scaleRange(0.1);
        //ui->plot->yAxis->rescale(true);
    }
    else
        ui->plot->axisRect(0)->setRangeZoom(Qt::Horizontal | Qt::Vertical);
}

bool Spec::event(QEvent *event)
{
    QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
    if(keyEvent->key() == Qt::Key_Shift){
        if(event->type() == QEvent::KeyPress){
            flag = 1;
        }
        else if(event->type() == QEvent::KeyRelease)
            flag = 0;
    }
    else if(keyEvent->key() == Qt::Key_Control){
        if(event->type() == QEvent::KeyPress){
            flag = 2;
        }
        else if(event->type() == QEvent::KeyRelease)
            flag = 0;
    }

    return QWidget::event(event);
}
void Spec::xAxisChanged(QCPRange range)
{
    // adjust position of scroll bar slider
 // adjust size of scroll bar slider
}

void Spec::yAxisChanged(QCPRange range)
{


    // adjust position of scroll bar slider
    // adjust size of scroll bar slider
}

void Spec::recieveSpectrum(spectrum *x)
{
    spectrum___ = x;
}

void Spec::on_spinBox_valueChanged(int arg1)
{
    time = arg1;
    ui->plot->clearGraphs();
    this->setupPlot();
}

void Spec::on_checkBox_2_clicked(bool checked)
{
    if(checked){
        tracer = new QCPItemTracer(ui->plot);
        tracer->setGraph(ui->plot->graph(0));
    }
    else{
        tracer->deleteLater();
    }
}

