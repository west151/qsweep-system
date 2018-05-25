#include "src/coresweep.h"

int main(int argc, char *argv[])
{
    CoreSweep core;
    int result = core.runCoreSweep(argc, argv);
    return result;
}
