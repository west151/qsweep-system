#ifndef RINGBUFFER_H
#define RINGBUFFER_H

#include <QObject>

class RingBuffer : public QObject
{
    Q_OBJECT
public:
    explicit RingBuffer(quint32 _size, QObject *parent = 0);
    ~RingBuffer();

    void clean();
    bool read(float *data, quint32 len);
    bool write(float *data, quint32 len);
    float getSample();
    void addSample(float sample);
    quint32 availableRead(); // available size read data
    quint32 availableWrite(); // available size write data

private:
    quint32 size;
    quint32 mask;
    quint32 idxRead;
    quint32 idxWrite;
    quint32 dataCount; // available read data size
    float *fbuf = NULL;
};

#endif // RINGBUFFER_H
