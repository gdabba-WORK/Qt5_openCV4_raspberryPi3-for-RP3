#include "mainwindow.hpp"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::MainWindow), color(new Color), storage(new Storage), wiringPi(new WiringPi),
      prev(0)
{
    cout << "Mainwindow constructor start!" << endl;
    ui->setupUi(this);

    ui->graphicsView->setScene(new QGraphicsScene(this));
    ui->graphicsView->scene()->addItem(&pixmap);

    connect(color, &Color::pixmapReady, this, &MainWindow::handlePixmap);
    connect(color, &Color::frameReady, storage, &Storage::handleFrame);
    connect(wiringPi, &WiringPi::cdsReady, color, &Color::handleCds);
}

MainWindow::~MainWindow()
{
    cout << "Mainwindow destructor start!" << endl;
    delete ui;
    while ( color->isRunning() ) {
        std::cout << "Thread color is running" << std::endl;
        color->cameraOff();
    }
    delete color;
}

Color* MainWindow::getColor()
{
    return color;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if(color->getCapture().isOpened())
    {
        QMessageBox::warning(this,
                             "Warning",
                             "Stop the video before closing the application!");
        event->ignore();
    }
    else
    {
        event->accept();
    }
}

void MainWindow::handlePixmap(const QPixmap& result)
{
    pixmap.setPixmap(result);
    ui->graphicsView->fitInView(&pixmap, Qt::KeepAspectRatio);
}

void MainWindow::on_btn_default_clicked()
{
    color->setFlag(DEFAULT);
}

void MainWindow::on_btn_red_clicked()
{
    color->setFlag(RED);
}

void MainWindow::on_btn_green_clicked()
{
    color->setFlag(GREEN);
}

void MainWindow::on_btn_blue_clicked()
{
    color->setFlag(BLUE);
}

void MainWindow::on_btn_redgreen_clicked()
{
    color->setFlag(RED_GREEN);
}

void MainWindow::on_btn_redblue_clicked()
{
    color->setFlag(RED_BLUE);
}

void MainWindow::on_btn_greenblue_clicked()
{
    color->setFlag(GREEN_BLUE);
}

void MainWindow::on_btn_cam_clicked()
{
    if (ui->btn_cam->text().toStdString() == "Cam-On")
    {
        color->cameraOn();
        color->start();
        ui->btn_cam->setText("Cam-Off");
    }
    else if(ui->btn_record->text().toStdString() == "Record-On")
    {
        color->cameraOff();
        ui->btn_cam->setText("Cam-On");
    }
}

void MainWindow::on_btn_record_clicked()
{
    if (ui->btn_cam->text().toStdString() == "Cam-On")
        return;

    if (ui->btn_record->text().toStdString() == "Record-On")
    {
        storage->recorderOn();
        storage->start();
        color->setIsRecord(true);
        wiringPi->setIsLED(true);
        if(!wiringPi->getIsRun())
            wiringPi->start();
        ui->btn_record->setText("Record-Off");
    }
    else
    {
        color->setIsRecord(false);
        storage->recorderOff();
        wiringPi->setIsLED(false);
        ui->btn_record->setText("Record-On");
    }
}

void MainWindow::on_btn_cds_clicked()
{
    if (ui->btn_cds->text().toStdString() == "CDS-On")
    {
        wiringPi->setIsCDS(true);
        if(!wiringPi->getIsRun())
            wiringPi->start();
        color->setIsCds(true);
        ui->btn_cds->setText("CDS-Off");
    }
    else
    {
        color->setIsCds(false);
        wiringPi->setIsCDS(false);
        ui->btn_cds->setText("CDS-On");
    }
}

void MainWindow::on_horizontalSlider_valueChanged(int value)
{
    ui->graphicsView->rotate(value-prev);
    prev = value;
}

void MainWindow::on_horizontalScrollBar_valueChanged(int value)
{
    ui->graphicsView->move(value, 10);
}




