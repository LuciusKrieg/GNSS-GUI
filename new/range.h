#ifndef RANGE_H
#define RANGE_H

#include <QWidget>
#include <iostream>
#include <vector>
namespace Ui {
class Range;
}

class Range : public QWidget
{
    Q_OBJECT

public:
    explicit Range(QWidget *parent = 0);
    ~Range();

    void getValue(const QList <QWidget*> &widgets);
    std::vector < std::vector<short> > l;


signals:
    void sendData(QString str);
    void saveClicked();
private slots:
    void on_pushButton_clicked();

private:
    Ui::Range* ui;
};

#endif // RANGE_H
