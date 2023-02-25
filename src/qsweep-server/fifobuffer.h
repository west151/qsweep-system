#ifndef FIFOBUFFER_H
#define FIFOBUFFER_H

#include <QSemaphore>

struct _chunk {
    int data_len ;
    int pri ;

    void *id ;
    void* samples;

    struct _chunk *next;
    struct _chunk *prev;
};

typedef struct _chunk Chunk;

struct _fifo {
    int size;
    int seq ;
    Chunk *head, *queue;
};

typedef struct _fifo  Fifo;

class FifoBuffer
{
public:
    FifoBuffer();
    ~FifoBuffer();

    int getSize();
    int isEmpty();
    void printq();

    int EnqueueData( void* data, int data_size, int data_prio, void* data_id ) ;
    void *DequeueData( int* data_size, int data_prio, void** data_id );

    long getElementCount() {
        return( byte_size );
    }

private:
    QSemaphore *sem ;
    Fifo *fifo ;
    long byte_size ;

    int enqueue( Chunk *c ) ;
    Chunk* dequeue();
    Chunk* dequeuePrio( int prio );
};

#endif // FIFOBUFFER_H
