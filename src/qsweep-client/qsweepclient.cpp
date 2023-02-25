#include "coresweepclient.h"

int main(int argc, char *argv[])
{
    qputenv("QT_IM_MODULE", QByteArray("qtvirtualkeyboard"));

    CoreSweepClient m_core;
    int result = m_core.runCoreSweepClient(argc, argv);
    return result;
}
