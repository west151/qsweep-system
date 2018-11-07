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

#include "constant.h"
#include "data_spectr.h"

class SweepWorker : public QObject
{
    Q_OBJECT
public:
    explicit SweepWorker(QObject *parent = nullptr);
    static SweepWorker* getInstance();

    void onDataPowerSpectrCallbacks(const power_spectr &, const bool &isSending = false);

public slots:
    void onRunSweepWorker(const QByteArray &value);
    void onStopSweepWorker();

signals:
    //void sendPowerSpectr(const QByteArray &value);

    //void signal_data_log(const QByteArray &);

    void signal_sweep_message(const QByteArray &);

private:
    static SweepWorker* m_instance;

    int opt, i, result = 0;
    const char* path = "/dev/null";
    const char* serial_number = nullptr;
    int exit_code = EXIT_SUCCESS;
    struct timeval t_end;
    float time_diff;
    unsigned int lna_gain = 16;   // RX LNA (IF) gain, 0-40dB, 8dB steps
    unsigned int vga_gain = 20;   // RX VGA (baseband) gain, 0-62dB, 2dB steps
    uint32_t num_samples = DEFAULT_SAMPLE_COUNT;    // Number of samples per frequency, 16384-4294967296
    uint32_t fft_bin_width = 500000;    // FFT bin width (frequency resolution) in Hz\n")
    int step_count;

    hackrf_device* device = nullptr;
    QVector<power_spectr> m_powerSpectrBuffer;

    static int rx_callback(hackrf_transfer *transfer);
    int hackrf_rx_callback(unsigned char *buffer, uint32_t length);
    float logPower(fftwf_complex in, float scale);
    float TimevalDiff(const struct timeval *a, const struct timeval *b);

    void errorHackrf(const QString &, int result);
    void sweepWorkerMessagelog(const QString &);
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
