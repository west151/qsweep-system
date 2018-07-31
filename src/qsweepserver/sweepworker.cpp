#include "sweepworker.h"

#include <QThread>

#include "qsweepparams.h"
#include "qsweeprequest.h"
#include "qsweepanswer.h"
#include "qsweepmessagelog.h"

#ifdef QT_DEBUG
#include <QtCore/qdebug.h>
#endif

int num_ranges = 0;
uint16_t frequencies[MAX_SWEEP_RANGES*2];

FILE* fd = NULL;
volatile uint32_t byte_count = 0;

struct timeval time_start;
struct timeval t_start;

bool amp = false;
uint32_t amp_enable;

//bool antenna = false;
//uint32_t antenna_enable;

bool one_shot = true;
volatile bool sweep_started = false;
volatile bool do_exit = false;

int fftSize = 20;
fftwf_complex *fftwIn = nullptr;
fftwf_complex *fftwOut = nullptr;
fftwf_plan fftwPlan = nullptr;
float* pwr;
float* window;
time_t time_now;
struct tm *fft_time;
char time_str[50];

#if defined(__GNUC__)
#include <unistd.h>
#include <sys/time.h>
#endif

SweepWorker* SweepWorker::m_instance = 0;

SweepWorker::SweepWorker(QObject *parent) : QObject(parent)
{

}

SweepWorker *SweepWorker::getInstance()
{
    if (m_instance == 0)
        m_instance = new SweepWorker();
    return m_instance;
}

void SweepWorker::onTestDataCallbacks(const QByteArray &value)
{

    emit sendData(value);
}

void SweepWorker::onDataPowerSpectrCallbacks(const PowerSpectr &power, const bool &isSending)
{
    m_powerSpectrBuffer.append(power);

    if(isSending)
    {
        qDebug() << "-------------------------------";
        qDebug() << "Sending data" << m_powerSpectrBuffer.count();

        QSweepAnswer answer;
        answer.setTypeAnswer(TypeAnswer::SWEEP_POWER_SPECTR);
        QSweepSpectr spectr;
        spectr.setPowerSpectr(m_powerSpectrBuffer);
        answer.setDataAnswer(spectr.exportToJson());

        emit sendPowerSpectr(answer.exportToJson());

        m_powerSpectrBuffer.clear();
    }
}

int SweepWorker::rx_callback(hackrf_transfer *transfer)
{
    SweepWorker *obj = (SweepWorker *)transfer->rx_ctx;
    if( transfer->valid_length == 0) return(0);
    return obj->hackrf_rx_callback(transfer->buffer, transfer->valid_length);
}

int SweepWorker::hackrf_rx_callback(unsigned char *buffer, uint32_t length)
{
    int8_t* buf;
    uint8_t* ubuf;
    uint64_t frequency; /* in Hz */

    if(nullptr == fd) {
        return -1;
    }

    byte_count += length;
    buf = (int8_t*) buffer;

    for(int j=0; j<BLOCKS_PER_TRANSFER; j++)
    {
        if(do_exit) {
            return 0;
        }

        ubuf = (uint8_t*) buf;
        if(ubuf[0] == 0x7F && ubuf[1] == 0x7F) {
            frequency = ((uint64_t)(ubuf[9]) << 56) | ((uint64_t)(ubuf[8]) << 48) | ((uint64_t)(ubuf[7]) << 40)
                    | ((uint64_t)(ubuf[6]) << 32) | ((uint64_t)(ubuf[5]) << 24) | ((uint64_t)(ubuf[4]) << 16)
                    | ((uint64_t)(ubuf[3]) << 8) | ubuf[2];
        } else {
            buf += BYTES_PER_BLOCK;
            continue;
        }

        if(!sweep_started) {
            if (frequency == static_cast<uint64_t>(FREQ_ONE_MHZ*frequencies[0])) {
                sweep_started = true;
            } else {
                buf += BYTES_PER_BLOCK;
                continue;
            }
        }

        if((FREQ_MAX_MHZ * FREQ_ONE_MHZ) < frequency) {
            buf += BYTES_PER_BLOCK;
            continue;
        }

        /* copy to fftwIn as floats */
        buf += BYTES_PER_BLOCK - (fftSize * 2);
        for(int i=0; i < fftSize; i++) {
            fftwIn[i][0] = buf[i*2] * window[i] * 1.0f / 128.0f;
            fftwIn[i][1] = buf[i*2+1] * window[i] * 1.0f / 128.0f;
        }

        buf += fftSize * 2;
        fftwf_execute(fftwPlan);

        for(int i=0; i < fftSize; i++)
            pwr[i] = logPower(fftwOut[i], 1.0f / fftSize);


        //************************************************************************************
        // PowerSpectr for sending
        PowerSpectr dataPowerSpectr;
        bool isSending = false;
        dataPowerSpectr.dateTime = QDateTime::currentDateTimeUtc();

        time_now = time(nullptr);
        fft_time = localtime(&time_now);
        strftime(time_str, 50, "%Y-%m-%d, %H:%M:%S", fft_time);

        dataPowerSpectr.m_frequency_min = static_cast<uint64_t>(frequency);
        dataPowerSpectr.m_frequency_max = static_cast<uint64_t>(frequency + DEFAULT_SAMPLE_RATE_HZ/4);
        dataPowerSpectr.m_fft_bin_width = DEFAULT_SAMPLE_RATE_HZ / fftSize;
        dataPowerSpectr.m_fft_size = static_cast<quint32>(fftSize) ;

        for(int i = 0; (fftSize / 4) > i; i++)
            dataPowerSpectr.m_power.append(static_cast<qreal>(pwr[i + 1 + (fftSize*5)/8]));

        // segment 1
        getInstance()->onDataPowerSpectrCallbacks(dataPowerSpectr);


#ifdef QT_DEBUG
        printf("%s, %" PRIu64 ", %" PRIu64 ", %u",
               time_str,
               static_cast<uint64_t>(frequency),
               static_cast<uint64_t>(frequency+DEFAULT_SAMPLE_RATE_HZ/4),
               fftSize);
        for(int i = 0; (fftSize / 4) > i; i++)
            printf(", %.2f", static_cast<qreal>(pwr[i + 1 + (fftSize*5)/8]));
        printf("\n");
#endif

        //---------------------------------------------------------------------
        dataPowerSpectr.m_power.clear();
        dataPowerSpectr.m_frequency_min = static_cast<uint64_t>(frequency+(DEFAULT_SAMPLE_RATE_HZ/2));
        dataPowerSpectr.m_frequency_max = static_cast<uint64_t>(frequency+((DEFAULT_SAMPLE_RATE_HZ*3)/4));
        dataPowerSpectr.m_fft_bin_width = DEFAULT_SAMPLE_RATE_HZ / fftSize;
        dataPowerSpectr.m_fft_size = static_cast<quint32>(fftSize) ;

        for(int i = 0; (fftSize / 4) > i; i++)
            dataPowerSpectr.m_power.append(static_cast<qreal>(pwr[i + 1 + (fftSize/8)]));

        if(one_shot && (static_cast<uint64_t>(frequency+((DEFAULT_SAMPLE_RATE_HZ*3)/4))
                        >= static_cast<uint64_t>(FREQ_ONE_MHZ*frequencies[num_ranges*2-1]))){
            isSending = true;
            do_exit = true;
#ifdef QT_DEBUG
            qDebug() << "======================================";
            qDebug() << static_cast<uint64_t>(frequency+((DEFAULT_SAMPLE_RATE_HZ*3)/4))
                     << " >= "
                     << static_cast<uint64_t>(FREQ_ONE_MHZ*frequencies[num_ranges*2-1]);
            qDebug() << "======================================";
#endif
        }

        // segment 2
        getInstance()->onDataPowerSpectrCallbacks(dataPowerSpectr, isSending);

#ifdef QT_DEBUG
        printf("%s, %" PRIu64 ", %" PRIu64 ", %u",
               time_str,
               static_cast<uint64_t>(frequency+(DEFAULT_SAMPLE_RATE_HZ/2)),
               static_cast<uint64_t>(frequency+((DEFAULT_SAMPLE_RATE_HZ*3)/4)),
               fftSize);
        for(int i = 0; (fftSize / 4) > i; i++)
            printf(", %.2f", static_cast<qreal>(pwr[i + 1 + (fftSize/8)]));
        printf("\n");
#endif

    } // for(j=0; j<BLOCKS_PER_TRANSFER; j++)

    return 0;
}

float SweepWorker::logPower(fftwf_complex in, float scale)
{
    float re = in[0] * scale;
    float im = in[1] * scale;
    float magsq = re * re + im * im;
    return log2f(magsq) * 10.0f / log2(10.0f);
}

float SweepWorker::TimevalDiff(const timeval *a, const timeval *b)
{
    return (a->tv_sec - b->tv_sec) + 1e-6f * (a->tv_usec - b->tv_usec);
}

void SweepWorker::errorHackrf(const QString &text, int result)
{
#ifdef QT_DEBUG
    qDebug() << text
             << hackrf_error_name(static_cast<hackrf_error>(result))
             << tr("(%1)").arg(result);
#endif

    QString msg(text);
    msg.append("\n");
    msg.append(hackrf_error_name(static_cast<hackrf_error>(result)));
    msg.append("\n");
    msg.append(tr("code (%1)").arg(result));

    sweepWorkerMessagelog(msg);
}

void SweepWorker::sweepWorkerMessagelog(const QString &value)
{
    QSweepAnswer answer;
    answer.setTypeAnswer(TypeAnswer::SWEEP_MESSAGE_LOG);
    QSweepMessageLog log;
    log.setTextMessage(value);
    answer.setDataAnswer(log.exportToJson());

    emit sendSweepWorkerMessagelog(answer.exportToJson());
}

void SweepWorker::onRunSweepWorker(const QByteArray &value)
{
    const QSweepRequest request(value, false);
    const QSweepParams params(request.dataRequest());

    fft_bin_width = params.fftBinWidth();   // FFT bin width (frequency resolution) in Hz
    lna_gain = params.lnaGain();            // RX LNA (IF) gain, 0-40dB, 8dB steps
    vga_gain = params.vgaGain();            // RX VGA (baseband) gain, 0-62dB, 2dB steps

    if (0 == num_ranges) {
        frequencies[0] = static_cast<uint16_t>(params.frequencyMin());
        frequencies[1] = static_cast<uint16_t>(params.frequencyMax());
        num_ranges++;
    }

    fftSize = DEFAULT_SAMPLE_RATE_HZ / fft_bin_width;
    one_shot = params.oneShot();

    amp = true;

    QString msgTask;
    msgTask.append(QString::number(fft_bin_width));
    msgTask.append(tr(" "));
    msgTask.append(QString::number(lna_gain));
    msgTask.append(tr(" "));
    msgTask.append(QString::number(vga_gain));
    msgTask.append(tr(" "));
    msgTask.append(QString::number(frequencies[0]));
    msgTask.append(tr(" "));
    msgTask.append(QString::number(frequencies[1]));

    sweepWorkerMessagelog(msgTask);

    if(4 > fftSize) {
        fprintf(stderr,"argument error: FFT bin width must be no more than one quarter the sample rate\n");
        sweepWorkerMessagelog(tr("argument error: FFT bin width must be no more than one quarter the sample rate"));
        exit(0);
    }

    if(8184 < fftSize) {
        fprintf(stderr,	"argument error: FFT bin width too small, resulted in more than 8184 FFT bins\n");
        sweepWorkerMessagelog(tr("argument error: FFT bin width too small, resulted in more than 16368 FFT bins"));
        exit(0);
    }

    /* In interleaved mode, the FFT bin selection works best if the total
     * number of FFT bins is equal to an odd multiple of four.
     * (e.g. 4, 12, 20, 28, 36, . . .)
     */
    while((fftSize + 4) % 8) {
        fftSize++;
    }

    fft_bin_width = DEFAULT_SAMPLE_RATE_HZ / fftSize;
    fftwIn = (fftwf_complex*)fftwf_malloc(sizeof(fftwf_complex) * fftSize);
    fftwOut = (fftwf_complex*)fftwf_malloc(sizeof(fftwf_complex) * fftSize);
    fftwPlan = fftwf_plan_dft_1d(fftSize, fftwIn, fftwOut, FFTW_FORWARD, FFTW_MEASURE);
    pwr = (float*)fftwf_malloc(sizeof(float) * fftSize);
    window = (float*)fftwf_malloc(sizeof(float) * fftSize);

    for (i = 0; i < fftSize; i++) {
        window[i] = (float) (0.5f * (1.0f - cos(2 * M_PI * i / (fftSize - 1))));
        //window[i] = 0.5f * (1.0f - cos(2 * M_PI * i / (fftSize - 1)));
    }

    result = hackrf_init();
    if( result != HACKRF_SUCCESS ) {
        errorHackrf("hackrf_init() failed:", result);
        exit(0);
    }

    result = hackrf_open_by_serial(serial_number, &device);
    if( result != HACKRF_SUCCESS ) {
        errorHackrf("hackrf_open() failed:", result);
        exit(0);
    }

    fd = fopen(path, "wb");
    if( fd == nullptr ) {
        fprintf(stderr, "Failed to open file: %s\n", path);
        exit(0);
    }
    /* Change fd buffer to have bigger one to store or read data on/to HDD */
    result = setvbuf(fd , nullptr , _IOFBF , FD_BUFFER_SIZE);
    if( result != 0 ) {
        fprintf(stderr, "setvbuf() failed: %d\n", result);
        exit(0);
    }

    fprintf(stderr, "call hackrf_sample_rate_set(%.03f MHz)\n",
            ((float)DEFAULT_SAMPLE_RATE_HZ/(float)FREQ_ONE_MHZ));

    sweepWorkerMessagelog(tr("call hackrf_sample_rate_set(%1 MHz)").arg((float)DEFAULT_SAMPLE_RATE_HZ/(float)FREQ_ONE_MHZ));

    result = hackrf_set_sample_rate_manual(device, DEFAULT_SAMPLE_RATE_HZ, 1);

    if( result != HACKRF_SUCCESS ) {
        errorHackrf("hackrf_sample_rate_set() failed:", result);
        exit(0);
    }

    fprintf(stderr, "call hackrf_baseband_filter_bandwidth_set(%.03f MHz)\n",
            ((float)DEFAULT_BASEBAND_FILTER_BANDWIDTH/(float)FREQ_ONE_MHZ));

    sweepWorkerMessagelog(tr("call hackrf_baseband_filter_bandwidth_set(%1 MHz)").arg((float)DEFAULT_BASEBAND_FILTER_BANDWIDTH/(float)FREQ_ONE_MHZ));

    result = hackrf_set_baseband_filter_bandwidth(device, DEFAULT_BASEBAND_FILTER_BANDWIDTH);
    if( result != HACKRF_SUCCESS ) {
        errorHackrf("hackrf_baseband_filter_bandwidth_set() failed:", result);
        exit(0);
    }

    result = hackrf_set_vga_gain(device, vga_gain);
    result |= hackrf_set_lna_gain(device, lna_gain);
    result |= hackrf_start_rx(device, rx_callback, nullptr);
    if (result != HACKRF_SUCCESS) {
        errorHackrf("hackrf_start_rx() failed:", result);
        exit(0);
    }

    /*
     * For each range, plan a whole number of tuning steps of a certain
     * bandwidth. Increase high end of range if necessary to accommodate a
     * whole number of steps, minimum 1.
     */
    for(i = 0; i < num_ranges; i++) {
        step_count = 1 + (frequencies[2*i+1] - frequencies[2*i] - 1)
                / TUNE_STEP;
        frequencies[2*i+1] = (uint16_t) (frequencies[2*i] + step_count * TUNE_STEP);
        fprintf(stderr, "Sweeping from %u MHz to %u MHz\n", frequencies[2*i], frequencies[2*i+1]);
        sweepWorkerMessagelog(tr("Sweeping from %1 MHz to %2 MHz").arg(frequencies[2*i]).arg(frequencies[2*i+1]));
    }

    result = hackrf_init_sweep(device, frequencies, num_ranges, num_samples,
            TUNE_STEP * FREQ_ONE_MHZ, OFFSET, INTERLEAVED);

    if( result != HACKRF_SUCCESS ) {
        errorHackrf("hackrf_init_sweep() failed:", result);
        exit(0);
    }

    if (amp) {
        fprintf(stderr, "call hackrf_set_amp_enable(%u)\n", amp_enable);
        sweepWorkerMessagelog(tr("call hackrf_set_amp_enable(%1)").arg(amp_enable) );
        result = hackrf_set_amp_enable(device, (uint8_t)amp_enable);
        if (result != HACKRF_SUCCESS) {
            errorHackrf("hackrf_set_amp_enable() failed:", result);
            exit(0);
        }
    }

    gettimeofday(&t_start, nullptr);
    gettimeofday(&time_start, nullptr);

    while((hackrf_is_streaming(device) == HACKRF_TRUE) && (do_exit == false)) {
        uint32_t byte_count_now;
        struct timeval time_now;
        float time_difference, rate;
        sleep(1);

        gettimeofday(&time_now, nullptr);

        byte_count_now = byte_count;
        byte_count = 0;

        time_difference = TimevalDiff(&time_now, &time_start);
        rate = (float)byte_count_now / time_difference;

        fprintf(stderr, "%4.1f MiB / %5.3f sec = %4.1f MiB/second\n",
                (byte_count_now / 1e6f), time_difference, (rate / 1e6f) );

        sweepWorkerMessagelog(tr("%1 MiB / %2 sec = %3 MiB/second").arg((byte_count_now/1e6f)).arg(time_difference).arg((rate/1e6f)) );

        time_start = time_now;

        if (byte_count_now == 0) {
            exit_code = EXIT_FAILURE;
            fprintf(stderr, "\nCouldn't transfer any bytes for one second.\n");
            break;
        }
    }

    result = hackrf_is_streaming(device);

    if (do_exit) {
        fprintf(stderr, "\nExiting...\n");
        sweepWorkerMessagelog(tr("Exiting..."));
    } else {
        fprintf(stderr, "\nExiting... hackrf_is_streaming() result: %s (%d)\n",
               hackrf_error_name(static_cast<hackrf_error>(result)), result);
    }

    gettimeofday(&t_end, nullptr);
    time_diff = TimevalDiff(&t_end, &t_start);
    fprintf(stderr, "Total time: %5.5f s\n", time_diff);

    if(device != nullptr) {
        result = hackrf_stop_rx(device);
        if(result != HACKRF_SUCCESS) {
            errorHackrf("hackrf_stop_rx() failed:", result);
        } else {
            fprintf(stderr, "hackrf_stop_rx() done\n");
            sweepWorkerMessagelog(tr("hackrf_stop_rx() done"));
        }

        result = hackrf_close(device);

        if(result != HACKRF_SUCCESS) {
            errorHackrf("hackrf_close() failed:", result);
        } else {
            fprintf(stderr, "hackrf_close() done\n");
            sweepWorkerMessagelog(tr("hackrf_close() done"));
        }

        hackrf_exit();
        fprintf(stderr, "hackrf_exit() done\n");
        sweepWorkerMessagelog(tr("hackrf_exit() done"));
    }

    if(fd != nullptr) {
        fclose(fd);
        fd = nullptr;
        fprintf(stderr, "fclose(fd) done\n");
        sweepWorkerMessagelog(tr("fclose(fd) done"));
    }

    fftwf_free(fftwIn);
    fftwf_free(fftwOut);
    fftwf_free(pwr);
    fftwf_free(window);

//#ifdef QT_DEBUG
//    qDebug() << "--------------------------";
//    qDebug() << "FFT Size:" << fftSize;
//    qDebug() << "RING_BUFFER Available Read:" << m_ringBuffer->availableRead();
//    qDebug() << "--------------------------";
//#endif

    do_exit = false;
    num_ranges = 0;
}

void SweepWorker::onStopSweepWorker()
{
    do_exit = true;
}
