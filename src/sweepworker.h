#ifndef SWEEPWORKER_H
#define SWEEPWORKER_H

#include <QObject>

#include <hackrf.h>
#include <fftw3.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <math.h>
#include <inttypes.h>

#define FD_BUFFER_SIZE (8*1024)

#define FREQ_ONE_MHZ (1000000ull)

#define FREQ_MIN_MHZ (0)    /*    0 MHz */
#define FREQ_MAX_MHZ (7250) /* 7250 MHz */

#define DEFAULT_SAMPLE_RATE_HZ (20000000) /* 20MHz default sample rate */
#define DEFAULT_BASEBAND_FILTER_BANDWIDTH (15000000) /* 5MHz default */

#define TUNE_STEP (DEFAULT_SAMPLE_RATE_HZ / FREQ_ONE_MHZ)
#define OFFSET 7500000

#define DEFAULT_SAMPLE_COUNT 0x4000
#define BLOCKS_PER_TRANSFER 16

class SweepWorker : public QObject
{
    Q_OBJECT
public:
    explicit SweepWorker(QObject *parent = nullptr);

    int runSweepWorker();

private:
    int opt, i, result = 0;
    const char* path = "/dev/null";
    const char* serial_number = NULL;
    int exit_code = EXIT_SUCCESS;
    struct timeval t_end;
    float time_diff;
    unsigned int lna_gain=16, vga_gain=20;
    uint32_t num_samples = DEFAULT_SAMPLE_COUNT;
    int step_count;
    uint32_t freq_min = 2400;      // uint32_t freq_min = 0;
    uint32_t freq_max = 2700;   // uint32_t freq_max = 6000;

    hackrf_device* device = NULL;
};

#endif // SWEEPWORKER_H
