#ifndef IKINECTSTORAGE_H
#define IKINECTSTORAGE_H

#include <QImage>

class IKinectStorage {
public:
    virtual void saveImage(const QImage& _image) = 0;
    virtual QString name() = 0;
};

#endif // IKINECTSTORAGE_H
