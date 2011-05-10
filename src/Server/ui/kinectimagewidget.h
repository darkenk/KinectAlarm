#ifndef KINECTIMAGEWIDGET_H
#define KINECTIMAGEWIDGET_H

#include <QGLWidget>
#include <ikinect.h>

class KinectImageWidget : public QGLWidget
{
    Q_OBJECT
public:
    KinectImageWidget(IKinect* _kinect, QWidget *_parent = 0);
    virtual ~KinectImageWidget();

    void setKinect(IKinect* _kinect) {
	m_kinect = _kinect;
    }

signals:

protected:
    void paintGL();
    void initializeGL();
    void resizeGL(int w, int h);

protected slots:
    void animate();

private:
    IKinect* m_kinect;
    GLuint m_rgbTexture;
    GLuint m_depthTexture;
    QTimer* m_animationTimer;

    quint32 m_rgbWidth;
    quint32 m_rgbHeight;
    quint32 m_depthWidth;
    quint32 m_depthHeight;
};

#endif // KINECTIMAGEWIDGET_H
