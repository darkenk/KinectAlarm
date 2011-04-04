#include "kinectimagewidget.h"

#include <QDebug>
#include <QTimer>

KinectImageWidget::KinectImageWidget(IKinect* _kinect, QWidget *_parent) :
    QGLWidget(_parent),
    m_kinect(_kinect),
    m_animationTimer(new QTimer),
    m_rgbWidth(320),
    m_rgbHeight(240),
    m_depthWidth(320),
    m_depthHeight(240)
{
    m_animationTimer->setSingleShot(false);
    connect(m_animationTimer, SIGNAL(timeout()), this, SLOT(animate()));
    m_animationTimer->start(25);
//    setFixedSize(320, 240);
}

KinectImageWidget::~KinectImageWidget()
{

}


void KinectImageWidget::paintGL()
{
    //qDebug() << Q_FUNC_INFO;
    m_rgbWidth = m_kinect->rgbImageWidth();
    m_rgbHeight = m_kinect->rgbImageHeight();
    m_depthWidth = m_kinect->depthImageWidth();
    m_depthHeight = m_kinect->depthImageHeight();
    quint8* rgbImage = m_kinect->rgbImage();
    quint8* depthImage = m_kinect->depthImage();
    if (rgbImage == NULL) {
	qWarning() << Q_FUNC_INFO << " rgbImage is null!";
    }
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, m_rgbTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, m_rgbWidth, m_rgbHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, rgbImage);

    glBegin(GL_TRIANGLE_FAN);
    glColor4f(255.0f, 255.0f, 255.0f, 255.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(0,0,0);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(m_rgbWidth,0,0);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(m_rgbWidth,m_rgbHeight,0);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(0,m_rgbHeight,0);
    glEnd();

    glBindTexture(GL_TEXTURE_2D, m_depthTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, m_depthWidth, m_depthHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, depthImage);

    glBegin(GL_TRIANGLE_FAN);
    glColor4f(255.0f, 255.0f, 255.0f, 255.0f);
    glTexCoord2f(0, 0); glVertex3f(m_rgbWidth,0,0);
    glTexCoord2f(1, 0); glVertex3f(m_rgbWidth+m_depthWidth,0,0);
    glTexCoord2f(1, 1); glVertex3f(m_rgbWidth+m_depthWidth,m_depthHeight,0);
    glTexCoord2f(0, 1); glVertex3f(m_rgbWidth,m_depthHeight,0);
    glEnd();
}

void KinectImageWidget::initializeGL()
{
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClearDepth(1.0);
    glDepthFunc(GL_LESS);
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glShadeModel(GL_SMOOTH);
    glGenTextures(1, &m_depthTexture);
    glBindTexture(GL_TEXTURE_2D, m_depthTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glGenTextures(1, &m_rgbTexture);
    glBindTexture(GL_TEXTURE_2D, m_rgbTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void KinectImageWidget::resizeGL( int w, int h )
{
    glViewport( 0, 0, (GLint)w, (GLint)h );
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    glOrtho (0, m_rgbWidth+m_depthWidth, (m_rgbHeight > m_depthHeight ? m_rgbHeight : m_depthHeight), 0, -1.0f, 1.0f);
    glMatrixMode( GL_MODELVIEW );
}

void KinectImageWidget::animate()
{
    updateGL();
}
