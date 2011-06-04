#ifndef KINECTGLOBAL_H
#define KINECTGLOBAL_H

#include <QDebug>

//#ifndef QT_NO_DEBUG

#define INFO_LINE __FILE__ << "::" << __LINE__

#define BEGIN qDebug() << "+++++" << INFO_LINE << Q_FUNC_INFO << " begin";
#define INFO(x) qDebug() << INFO_LINE << Q_FUNC_INFO << x;
#define END qDebug() <<   "=====" << INFO_LINE << Q_FUNC_INFO << " end";

//#elsif

//#define BEGIN
//#define INFO(x)
//#define END

//#end

#endif // KINECTGLOBAL_H
