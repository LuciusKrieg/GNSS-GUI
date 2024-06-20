#include "form.h"
#include "ui_form.h"

Form::Form(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Form)
{
    ui->setupUi(this);


    // Инициализируем вертикальную линию

    verticalLine = new QCPCurve(ui->plot_0->xAxis, ui->plot_0->yAxis);
    horisontalLine = new QCPCurve(ui->plot_0->yAxis, ui->plot_0->xAxis);
    // Подключаем сигналы событий мыши от полотна графика к слотам для их обработки
    connect(ui->plot_0, &QCustomPlot::mousePress, this, &Form::slotMousePress);
    connect(ui->plot_0, &QCustomPlot::mouseMove, this, &Form::slotMouseMove);

    // configure scroll bars:
    // Since scroll bars only support integer values, we'll set a high default range of -500..500 and
    // divide scroll bar position values by 100 to provide a scroll range -5..5 in floating point
    // axis coordinates. if you want to dynamically grow the range accessible with the scroll bar,
    // just increase the minimum/maximum values of the scroll bars as needed.

    connect(ui->horizontalScrollBar, SIGNAL(valueChanged(int)), this, SLOT(horzScrollBarChanged(int)));
    connect(ui->verticalScrollBar, SIGNAL(valueChanged(int)), this, SLOT(vertScrollBarChanged(int)));
    connect(ui->plot_0->xAxis, SIGNAL(rangeChanged(QCPRange)), this, SLOT(xAxisChanged(QCPRange)));
    connect(ui->plot_0->yAxis, SIGNAL(rangeChanged(QCPRange)), this, SLOT(yAxisChanged(QCPRange)));

    //connect(ui->plot, &QCustomPlot::mouseWheel, this, &Form::keyPressEvent);
    connect(ui->plot_0, &QCustomPlot::mouseWheel, this, &Form::wheelEvent);
    // initialize axis range (and scroll bar positions via signals we just connected):
    tracer = new QCPItemTracer(ui->plot_0);



}
void Form::slotMousePress(QMouseEvent *event)
{
    // Определяем координату X на графике, где был произведён клик мышью
    double coordX = ui->plot_0->xAxis->pixelToCoord(event->pos().x());
    double coordY = ui->plot_0->yAxis->pixelToCoord(event->pos().y());
    //std::cout<<"a";
    // Подготавливаем координаты по оси X для переноса вертикальной линии
    QVector<double> x(2), y(2);
    x[0] = coordX;
    y[0] = coordY;
    x[1] = coordX;
    y[1] = coordY;

    // Устанавливаем новые координаты
    verticalLine->setData(x, y);
    horisontalLine->setData(y,x);
    // По координате X клика мыши определим ближайшие координаты для трассировщика
    tracer->setGraphKey(coordX);

    // Выводим координаты точки графика, где установился трассировщик, в lineEdit
    ui->label->setText("x: " + QString::number(tracer->position->key()));
    ui->label_2->setText("y: " + QString::number(tracer->position->value()));

    ui->plot_0->replot(); // Перерисовываем содержимое полотна графика
}

void Form::slotMouseMove(QMouseEvent *event)
{
    /* Если при передвижении мыши, ей кнопки нажаты,
     * то вызываем отработку координат мыши
     * через слот клика
     * */
    if(QApplication::mouseButtons() && ui->checkBox_2->isChecked()) slotMousePress(event);
}

Form::~Form()
{
    delete ui;
}
/*
void Form::newPlot(){

    customPlot = new QCustomPlot(); // Инициализируем графическое полотно
    //ui->gridLayout->addWidget(customPlot,0,0,1,1);  // Устанавливаем customPlot в окно проложения

    customPlot->setInteraction(QCP::iRangeZoom,true);   // Включаем взаимодействие удаления/приближения
    customPlot->setInteraction(QCP::iRangeDrag, true);  // Включаем взаимодействие перетаскивания графика
    customPlot->axisRect()->setRangeDrag(Qt::Horizontal);   // Включаем перетаскивание только по горизонтальной оси
    customPlot->axisRect()->setRangeZoom(Qt::Horizontal);   // Включаем удаление/приближение только по горизонтальной оси
    customPlot->xAxis->setLabel(QString("Time"));  // Подпись координат по Оси X в качестве Даты и Времени
    //customPlot->xAxis->setNumberFormat("g");  // Устанавливаем формат даты и времени

    // Настраиваем шрифт по осям координат
    customPlot->xAxis->setTickLabelFont(QFont(QFont().family(), 8));
    customPlot->yAxis->setTickLabelFont(QFont(QFont().family(), 8));

    // Автоматическое масштабирование тиков по Оси X
    customPlot->xAxis->setTicks(true);

    // Делаем видимыми оси X и Y по верхней и правой границам графика,
    // но отключаем на них тики и подписи координат

    customPlot->xAxis2->setVisible(true);
    customPlot->yAxis2->setVisible(true);
    customPlot->xAxis2->setTicks(false);
    customPlot->yAxis2->setTicks(false);
    customPlot->xAxis2->setTickLabels(false);
    customPlot->yAxis2->setTickLabels(false);

    customPlot->yAxis->setTickLabelColor(QColor(Qt::red)); // Красный цвет подписей тиков по Оси Y
    customPlot->legend->setVisible(true);   //Включаем Легенду графика
    // Устанавливаем Легенду в левый верхний угол графика
    customPlot->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignLeft|Qt::AlignTop);

    // Инициализируем график и привязываем его к Осям
    //graphic = new QCPGraph(customPlot->xAxis, customPlot->yAxis);
    customPlot->addGraph(customPlot->xAxis,customPlot->yAxis);    // Устанавливаем график на полотно
    graphic->setName("Доход, Р");       // Устанавливаем
    graphic->setPen(QPen(QColor(Qt::red))); // Устанавливаем цвет графика
    graphic->setAntialiased(false);         // Отключаем сглаживание, по умолчанию включено
    graphic->setLineStyle(QCPGraph::lsImpulse); // График в виде импульсных тиков

    // Подключаем сигнал от Оси X об изменении видимого диапазона координат
    // к СЛОТу для переустановки формата времени оси.

    connect(customPlot->xAxis, SIGNAL(rangeChanged(QCPRange)),
            this, SLOT(slotRangeChanged(QCPRange)));

    // Будем строить график с сегодняшнего дни и текущей секунды в будущее
    double now = QDateTime::currentDateTime().toTime_t();
    // Объявляем вектора времени и доходов
    QVector <double> time(400), income(400);

    srand(15); // Инициализируем генератор псевдослучайных чисел

    // Заполняем график значениями
    for (int i=0; i<400; ++i)
    {
        time[i] = now + 3600*i;
        income[i] = qFabs(income[i-1]) + (i/50.0+1)*(rand()/(double)RAND_MAX-0.5);
    }

    graphic->setData(time, income); // Устанавливаем данные
    customPlot->rescaleAxes();      // Масштабируем график по данным
    customPlot->replot();           // Отрисовываем график

}
*/
void Form::setupPlot()
{

    QVector<double> X(2) , Y(2);
    X[0] = 0;
    Y[0] = 0;
    X[1] = 0;
    Y[1] = 0;
    //std::cout<<"stdUped"<<std::endl;

        // Добавляем линию на полотно
    verticalLine->setName("Vertical");      // Устанавливаем ей наименование
    verticalLine->setData(X, Y);
    horisontalLine->setName("Horisontal");      // Устанавливаем ей наименование
    horisontalLine->setData(Y, X);



    // The following plot setup is mostly taken from the plot demos:

    //std::cout<<"\n\n\n\t\t\tCOLS"<<I[0]->cols<<"\n\n\n";
    ui->plot_0->addGraph();
    ui->plot_1->addGraph();
    ui->plot_0->graph()->setPen(QPen(Qt::blue));
    ui->plot_0->graph()->setBrush(QBrush(QColor(0, 0, 255, 20)));
    ui->plot_1->graph()->setPen(QPen(Qt::red));
    ui->plot_1->graph()->setBrush(QBrush(QColor(0, 0, 255, 20)));
    int current = ui->comboBox->currentIndex();
    //std::cout<<current<<std::endl;
    int size = spectrum__->cols;
        QVector<double> x(size), y0(size), y1(size);
        for (int i=0; i<size; ++i)
        {
            x[i] = (int)((power__->S[i][1] + power__->S[i][0])/2);
            y1[i] = spectrum__->Power[current][i];
            y0[i] = spectrum__->Power[(current+6)%9][i];
           // exponential envelope
        }
        double min[2] = {*std::min_element(y0.begin(), y0.end()),*std::min_element(y1.begin(), y1.end())};
        double max[2] = {*std::max_element(y0.begin(), y0.end()),*std::max_element(y1.begin(), y1.end())};
    //ui->plot->graph(0)->setData(x, y0);
        ui->plot_0->xAxis->setRange(0, x[size-1]);
        ui->plot_0->yAxis->setRange(min[0] ,max[0]);
        ui->plot_1->xAxis->setRange(0, x[size-1]);
        ui->plot_1->yAxis->setRange(min[1] ,max[1]);
    ui->plot_0->graph(0)->setData(x, y0);
    ui->plot_0->addGraph();
    ui->plot_0->graph(1)->setData(x, y1);
    ui->plot_0->graph(1)->rescaleAxes();
    ui->plot_0->graph(0)->rescaleAxes(true);
    //ui->plot->graph(1)->setData(x, y0);

    //ui->horizontalScrollBar->setRange(0, x[size-1]);
    //ui->verticalScrollBar->setRange(min, max);
    //std::cout<<"min:"<<min<<"\n max:"<<max<<std::endl;
    //tracer = new QCPItemTracer(ui->plot);
    //tracer->setGraph(ui->plot->graph(0));
    //customPlot->graph(0)->rescaleAxes();
    //customPlot->graph(1)->rescaleAxes(true);
    //ui->plot->axisRect()->setupFullAxesBox(true);
    ui->plot_0->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);

    ui->plot_0->replot();
    ui->plot_1->replot();
}



void Form::horzScrollBarChanged(int value)
{
    if (qAbs(ui->plot_0->xAxis->range().center()-value/100.0) > 0.01) // if user is dragging plot, we don't want to replot twice
    {
        ui->plot_0->xAxis->setRange(value/100.0, ui->plot_0->xAxis->range().size(), Qt::AlignCenter);
        ui->plot_0->replot();
    }
}

void Form::vertScrollBarChanged(int value)
{
    if (qAbs(ui->plot_0->yAxis->range().center()+value/100.0) > 0.01) // if user is dragging plot, we don't want to replot twice
    {
        ui->plot_0->yAxis->setRange(-value/100.0, ui->plot_0->yAxis->range().size(), Qt::AlignCenter);
        ui->plot_0->replot();
    }
}

void Form::xAxisChanged(QCPRange range)
{
    ui->horizontalScrollBar->setValue(qRound(range.center()*100.0));
    ui->horizontalScrollBar->setPageStep(qRound(range.size()*100.0));
    // adjust position of scroll bar slider
 // adjust size of scroll bar slider
}

void Form::yAxisChanged(QCPRange range)
{
    ui->verticalScrollBar->setValue(qRound(-range.center()*100.0));
    ui->verticalScrollBar->setPageStep(qRound(range.size()*100.0));


    // adjust position of scroll bar slider
    // adjust size of scroll bar slider
}





void Form::wheelEvent(QWheelEvent *w)
{
    w = nullptr;
    if(flag == 1){
        ui->plot_0->axisRect(0)->setRangeZoom(Qt::Horizontal);
        //ui->plot->xAxis->scaleRange(0.1);
        //ui->plot->xAxis->rescale(true);

    }
    else if(flag == 2 ){
        ui->plot_0->axisRect(0)->setRangeZoom(Qt::Vertical);
        //ui->plot->yAxis->scaleRange(0.1);
        //ui->plot->yAxis->rescale(true);
    }
    else
        ui->plot_0->axisRect(0)->setRangeZoom(Qt::Horizontal | Qt::Vertical);

        /*
    short b = (k->key() == Qt::Key_Shift) ?
                  1 : (k->key() == Qt::Key_Control) ?
                                                2 : 0;

    switch(b){
    case 1:


        break;
    case 2:

        break;
    case 0:
        ui->plot->yAxis->rescale();
        ui->plot->xAxis->rescale();
    default:
        break;
    }
*/
}

void Form::on_checkBox_toggled(bool checked)
{
    if(checked){
        ui->horizontalScrollBar->show();
        ui->verticalScrollBar->show();
    }
    else{
        ui->horizontalScrollBar->hide();
        ui->verticalScrollBar->hide();
    }
}

bool Form::event(QEvent *event)
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

void Form::recieveSpectrum(spectrum *x)
{
    spectrum__ = x;
}
void Form::recievePower(power *x)
{
    power__ = x;
    ui->plot_0->replot();
}
void Form::recieveSun(sun *x)
{
    sun__ = x;
}



void Form::on_comboBox_currentIndexChanged(int index)
{
    setupPlot();
    ui->plot_0->replot();
}

