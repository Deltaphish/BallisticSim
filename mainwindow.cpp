#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <cmath>
#include <qstring.h>
#include <vector>

#define PI 3.14159265
#define g -9.82

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_Run_button_clicked(){
    // Gather input from user
    double alpha = ui->A_input->text().toDouble();
    double V0 = ui->Vo_input->text().toDouble();
    double y_position = ui->Ho_input->text().toDouble();
    double x_position;

    // If no input, halt

    if(alpha == 0.0||V0 == 0.0){
        return;
    }

    double resolution;

    if(V0 < 1){
        resolution = 10000.0;
    }
    else if(V0 < 10){
        resolution = 1000.0;
    }
    else{
        resolution = 100.0;
    }

    // Canculate vertical and horisontal velocity

    double Vy = sin(alpha*(2*PI)/360)*V0;
    double Vx = cos(alpha*(2*PI)/360)*V0/resolution;


    QVector<double> x_position_v;
    QVector<double> y_position_v;

    //Starting Simulation
    int steps = 0;

    double max_y = 0;

    do{
        Vy += g/resolution;

        y_position += Vy/resolution;
        x_position += Vx;

        y_position_v.push_back(y_position);
        x_position_v.push_back(x_position);

        if(y_position < 0){
            break;
        }
        else if(y_position > max_y) max_y = y_position;
        steps++;
    }
    while(true);

    ui->BallisticPlot->addGraph();
    ui->BallisticPlot->graph(0)->setData(x_position_v,y_position_v);
    ui->BallisticPlot->xAxis->setRange(0,x_position);
    ui->BallisticPlot->xAxis->setLabel("Distance (m)");

    if(x_position < max_y || x_position - max_y > max_y*10){
       ui->BallisticPlot->yAxis->setRange(0,max_y);
    }else{
        ui->BallisticPlot->yAxis->setRange(0,x_position);
    }

    ui->BallisticPlot->yAxis->setLabel("Height (m)");

    ui->BallisticPlot->replot();

    ui->Max_height_text->setText("Max Height: "+QString::number(max_y,'f',3)+" (m)");
    ui->Max_distance_text->setText("Max Distance: "+QString::number(x_position,'f',3)+" (m)");
    ui->Time_in_air_text->setText("Time in air: "+QString::number(steps/resolution,'f',3)+" (s)");
    return;
}
