#ifndef SPECTRUM_NATIVE_WORKER_H
#define SPECTRUM_NATIVE_WORKER_H

#include <QObject>

#include "hackrf.h"
#include <fftw3.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <math.h>
#include <inttypes.h>

#include "../constant.h"
#include "data_spectr.h"

class spectrum_native_worker : public QObject
{
    Q_OBJECT
public:
    explicit spectrum_native_worker(QObject *parent = nullptr);
    static spectrum_native_worker* getInstance();

    void onDataPowerSpectrCallbacks(const power_spectr &, const bool &isSending = false);

public slots:
    void slot_run_sweep_worker(const QByteArray &value);
    void slot_stop_sweep_worker();

signals:
    void signal_sweep_message(const QByteArray &);
    void signal_sweep_worker(const bool &);

private:
    static spectrum_native_worker* m_instance;

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

#endif // SPECTRUM_NATIVE_WORKER_H
