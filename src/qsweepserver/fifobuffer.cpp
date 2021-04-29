#include <stdio.h>
#include <stdlib.h>
#include <QDebug>
#include "fifobuffer.h"
#define DEBUG (0)

Chunk* newchunk( int pri, void* id ) {
    Chunk *result ;

    result = (Chunk*)malloc( sizeof( Chunk ));
    if( result == NULL ) {
        return( NULL );
    }

    result->pri = pri ;
    result->id = id ;
    result->next = NULL ;
    result->prev = NULL ;

    return( result );
}

FifoBuffer::FifoBuffer()
{

    fifo = (Fifo*)malloc( sizeof(Fifo ));
    fifo->size = 0 ;
    fifo->seq = 0 ;
    fifo->head = NULL ;
    fifo->queue = NULL ;

    byte_size = 0 ;
    sem = new QSemaphore(1);
}

FifoBuffer::~FifoBuffer() {
    free( fifo );
    delete sem ;
}


int FifoBuffer::getSize() {
    int size = 0 ;
    if( fifo != NULL ) {
        //sem_wait(&fifo->mutex );
        size = fifo->size ;
        //sem_post(&fifo->mutex);
    }
    if( DEBUG ) printf("Queue size is %d\n", size );
    return( size );
}

int FifoBuffer::isEmpty() {
    int size = 0 ;
    if( fifo != NULL ) {
        sem->acquire();
        size = fifo->size ;
        sem->release();
    }

    return( size == 0 );
}

Chunk* FifoBuffer::dequeue() {
    Chunk *result ;

    //printf("Taking head from fifo\n");

    if( fifo == NULL ) return( NULL );
    sem->acquire();
    if( fifo->size == 0 ) {
        sem->release();
        byte_size = 0 ;
        return( NULL );
    }
    result = fifo->head ;

    if( fifo->size == 1 ) {
        fifo->head = NULL ;
        fifo->queue = NULL ;
    } else if( fifo->size > 0 ) {
        fifo->head = fifo->head->next ;
        fifo->head->prev = NULL ; // first elt
    }
    fifo->size = fifo->size - 1 ;
    byte_size -= result->data_len ;
    sem->release();
    return( result );
}

Chunk* FifoBuffer::dequeuePrio( int prio ) {
    Chunk *result ;
    //printf("Taking head from fifo\n");

    if( fifo == NULL ) return( NULL );
    sem->acquire();
    if( fifo->size == 0 ) {
        sem->release();
        return( NULL );
    }

    result = fifo->head ;
    if( fifo->size == 1 ) {
        fifo->head = NULL ;
        fifo->queue = NULL ;
        fifo->size = 0 ;
    } else {
        fifo->size = fifo->size - 1 ;
        while( result != NULL ) {
            if( result->pri == prio ) {
                if( result->prev != NULL )  {
                    result->prev->next = result->next ;
                } else {
                    fifo->head = result->next ;
                }
                if( result->next != NULL ) {
                    result->next->prev = result->prev ;
                } else {
                    fifo->queue = result->prev ;
                }
                sem->release();
                byte_size -= result->data_len ;
                return( result );
            }
            result = result->next ;
        }
        result = NULL ;
    }

    byte_size -= result->data_len ;
    sem->release();

    return( result );
}

#define FIFO_MAX_LENGH (200)
int  FifoBuffer::enqueue( Chunk *c ) {
    Chunk *tmp ;
    int lsize ;
    if( DEBUG ) printf("+adding %d\n", c->pri );
    if( fifo == NULL ) return(0) ;

    sem->acquire();
    if( fifo->size > FIFO_MAX_LENGH ) {
        //printf("Warning... element not pushed ,queue too long (%d elements)\n" , fifo->size );
        qDebug() << "Warning... element not pushed ,queue too long" << fifo->size ;
        sem->release();
        return(-1) ;
    }

    byte_size += c->data_len ;

    fifo->seq++ ;
    if( fifo->size == 0 ) {
        if( DEBUG ) printf("+empty\n");
        fifo->head = c ;
        fifo->queue = c ;
        fifo->size = 1 ;

        c->next = NULL ;
        c->prev = NULL ;

    } else  {
        tmp = fifo->queue ;
        while( tmp->pri < c->pri ) {
            if( DEBUG ) printf("+while:tmp=%d\n", tmp->pri );
            if( tmp->prev == NULL ) break ;
            tmp = tmp->prev ;
        }

        if( DEBUG ) printf("+tmp=%d\n", tmp->pri );
        if( tmp->pri >= c->pri ) {
            c->next = tmp->next ;
            tmp->next = c ;
            c->prev = tmp ;
            if( c->next != NULL ) c->next->prev = c ;
        } else {
            c->next = tmp ;
            c->prev = tmp->prev ;
            tmp->prev = c ;
            if( c->prev != NULL ) c->prev->next = c ;
        }


        if( c->prev == NULL ) {
            fifo->head = c ;
        }
        if( c->next == NULL ) {
            fifo->queue = c ;
        }
        ++(fifo->size) ;
    }
    lsize = fifo->size ;
    sem->release();
    return( lsize );
}

void FifoBuffer::printq() {
    Chunk *tmp ;
    int k ;
    printf("-------------\n");
    k = 0 ;

    sem->acquire();
    tmp = fifo->head ;
    while( tmp != NULL ) {
        printf("%d pri=%d \n", k++, tmp->pri  );
        tmp = tmp->next ;
    }

    sem->release();
    if( DEBUG ) printf("-------------\n");
}

int FifoBuffer::EnqueueData( void* data, int data_size, int data_prio, void* data_id ) {
    Chunk *tmp ;
    int rc ;

    if( fifo == NULL ) return(0) ;

    tmp = newchunk( data_prio, data_id );
    if( tmp == NULL ) {
        return(0) ;
    }
    tmp->data_len = data_size ;
    tmp->samples = data ;
    tmp->pri = data_prio ;
    tmp->id = data_id ;

    //qDebug() << "SampleFifo::EnqueueData" << data_size ;
    rc = enqueue(tmp);
    //qDebug() << "done" ;
    return( rc );
}

void *FifoBuffer::DequeueData( int* data_size, int data_prio, void** data_id ) {
    Chunk *tmp ;
    void* res ;

    if( fifo == NULL ) return(NULL) ;
    if( data_size != NULL ) *data_size = 0 ;

    if( data_prio == 0 ) {
        tmp = dequeue() ;
    } else {
        tmp = dequeuePrio(data_prio) ;
    }
    if( tmp == NULL ) return( NULL );
    if( data_size != NULL ) *data_size = tmp->data_len ;
    if( data_id != NULL ) *data_id = tmp->id ;

    res = tmp->samples ;
    free( tmp );
    return( res );
}
