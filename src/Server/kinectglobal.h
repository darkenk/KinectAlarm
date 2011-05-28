#ifndef KINECTGLOBAL_H
#define KINECTGLOBAL_H

#include <QDebug>

#define INFO_LINE __FILE__ << "::" << __LINE__

//#define BEGIN qDebug() << "+++++" << INFO_LINE << Q_FUNC_INFO << " begin";
//#define INFO(x) qDebug() << INFO_LINE << Q_FUNC_INFO << x;
//#define END qDebug() <<   "=====" << INFO_LINE << Q_FUNC_INFO << " end";

#define BEGIN
#define INFO(x)
#define END

#endif // KINECTGLOBAL_H
