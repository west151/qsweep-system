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

class QSweepParams;

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

public slots:
    void onRunSweepWorker(const QSweepParams &value);
    void onStopSweepWorker();

signals:
    void sendSignal();

private:
    int opt, i, result = 0;
    const char* path = "/dev/null";
    const char* serial_number = NULL;
    int exit_code = EXIT_SUCCESS;
    struct timeval t_end;
    float time_diff;
    unsigned int lna_gain = 16;   // RX LNA (IF) gain, 0-40dB, 8dB steps
    unsigned int vga_gain = 20;   // RX VGA (baseband) gain, 0-62dB, 2dB steps
    uint32_t num_samples = DEFAULT_SAMPLE_COUNT;    // Number of samples per frequency, 16384-4294967296
    uint32_t fft_bin_width = 500000;    // FFT bin width (frequency resolution) in Hz\n")
    int step_count;
    uint32_t freq_min = 30;      // freq_min = 0;
    uint32_t freq_max = 6000;   // freq_max = 6000;

    hackrf_device* device = NULL;

    static int rx_callback(hackrf_transfer *transfer);
    int hackrf_rx_callback(unsigned char *buffer, uint32_t length);
};

//fprintf(stderr, "Usage:\n");
//fprintf(stderr, "\t[-h] # this help\n");
//fprintf(stderr, "\t[-d serial_number] # Serial number of desired HackRF\n");
//fprintf(stderr, "\t[-a amp_enable] # RX RF amplifier 1=Enable, 0=Disable\n");
//fprintf(stderr, "\t[-f freq_min:freq_max] # minimum and maximum frequencies in MHz\n");
//fprintf(stderr, "\t[-p antenna_enable] # Antenna port power, 1=Enable, 0=Disable\n");
//fprintf(stderr, "\t[-l gain_db] # RX LNA (IF) gain, 0-40dB, 8dB steps\n");
//fprintf(stderr, "\t[-g gain_db] # RX VGA (baseband) gain, 0-62dB, 2dB steps\n");
//fprintf(stderr, "\t[-n num_samples] # Number of samples per frequency, 16384-4294967296\n");
//fprintf(stderr, "\t[-w bin_width] # FFT bin width (frequency resolution) in Hz\n");
//fprintf(stderr, "\t[-1] # one shot mode\n");
//fprintf(stderr, "\t[-B] # binary output\n");
//fprintf(stderr, "\n");
//fprintf(stderr, "Output fields:\n");
//fprintf(stderr, "\tdate, time, hz_low, hz_high, hz_bin_width, num_samples, dB, dB, . . .\n");


#endif // SWEEPWORKER_H
