#include "range.h"
#include "ui_range.h"

Range::Range(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Range)
{
    l = std::vector<std::vector<short>>(3, std::vector <short> (3) );
    for(size_t i = 0; i < 3; i++){
        l[i] = {10,10,15};
    }
    ui->setupUi(this);
    ui->L11_SB->setValue(l[0][0]);
    ui->L21_SB->setValue(l[1][0]);
    ui->L31_SB->setValue(l[2][0]);

    ui->L12_SB->setValue(l[0][1]);
    ui->L22_SB->setValue(l[1][1]);
    ui->L32_SB->setValue(l[2][1]);

    ui->L13_SB->setValue(l[0][2]);
    ui->L23_SB->setValue(l[1][2]);
    ui->L33_SB->setValue(l[2][2]);
   //l[0][0] = ui->L11_SB->value();
   //l[1][0] = ui->L21_SB->value();
   //l[2][0] = ui->L31_SB->value();

   //l[0][1] = ui->L12_SB->value();
   //l[1][1] = ui->L22_SB->value();
   //l[2][1] = ui->L32_SB->value();

   //l[0][2] = ui->L13_SB->value();
   //l[1][2] = ui->L23_SB->value();
   //l[2][2] = ui->L33_SB->value();
    //connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(on_pushButton_clicked()));

}
void Range::getValue(const QList <QWidget*> &widgets) {
    size_t i = 0;
    uint u[9];
    foreach (QWidget* w, widgets) {
        u[i] = w->property("value").toUInt();
        i++;

    }
    for(i = 0; i < 3; i++){
        for(size_t j = 0; j <3; j++){
            l[i][j] = u[3*i+j];
        }
        std::cout<<u[i]<<std::endl;
    }
}

Range::~Range()
{
    delete ui;
}

void Range::on_pushButton_clicked()
{
    emit sendData(ui->L11_SB->text()+' '+ui->L21_SB->text()+' '+ui->L31_SB->text() + ' ' +
                  ui->L12_SB->text()+' '+ui->L22_SB->text()+' '+ui->L32_SB->text() + ' ' +
                  ui->L13_SB->text()+' '+ui->L23_SB->text()+' '+ui->L33_SB->text());
    Range::hide();
    emit

}
