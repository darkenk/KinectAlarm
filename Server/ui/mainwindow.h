#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#include <ikinect.h>
#include <kinectimagewidget.h>
#include <QMainWindow>

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(IKinect* _kinect, QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    IKinect* m_kinect;
    KinectImageWidget* m_kinectWidget;
};


#endif // MAINWINDOW_H
