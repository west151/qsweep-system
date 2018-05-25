#ifndef CORESWEEP_H
#define CORESWEEP_H

#include <QObject>

class HackrfInfo;

class CoreSweep : public QObject
{
    Q_OBJECT
public:
    explicit CoreSweep(QObject *parent = nullptr);
    int runCoreSweep(int argc, char *argv[]);

private:
    HackrfInfo* ptrHackrfInfo;
};

#endif // CORESWEEP_H
