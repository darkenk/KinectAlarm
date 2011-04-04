#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_kinect(OpenNIObject::instance()),
    m_kinectWidget(new KinectImageWidget(m_kinect))

{
    ui->setupUi(this);
    ui->horizontalLayout->addWidget(m_kinectWidget);
    m_kinect->initialize();
    m_kinect->startGenerating();
}

MainWindow::~MainWindow()
{
    m_kinect->pauseGenerating();
    delete ui;
}
