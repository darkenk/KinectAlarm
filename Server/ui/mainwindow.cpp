#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QDebug"

MainWindow::MainWindow(IKinect* _kinect, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_kinect(_kinect),
    m_kinectWidget(new KinectImageWidget(m_kinect))

{
    ui->setupUi(this);
    ui->horizontalLayout->addWidget(m_kinectWidget);
    this->setAttribute(Qt::WA_DeleteOnClose);
}

MainWindow::~MainWindow()
{
    delete ui;
}
