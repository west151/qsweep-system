#include "spectrum_native_worker.h"

#include <QThread>

#include "sweep_message.h"
#include "data_log.h"
#include "params_spectr.h"
//#include "cstring"

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

volatile bool one_shot = true;
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
char params_id_str[8];

#if defined(__GNUC__)
#include <unistd.h>
#include <sys/time.h>
#endif

spectrum_native_worker* spectrum_native_worker::m_instance = nullptr;

spectrum_native_worker::spectrum_native_worker(QObject *parent) : QObject(parent)
{

}

spectrum_native_worker *spectrum_native_worker::getInstance()
{
    if (m_instance == nullptr)
        m_instance = new spectrum_native_worker();
    return m_instance;
}

void spectrum_native_worker::onDataPowerSpectrCallbacks(const power_spectr &power, const bool &isSending)
{
    m_powerSpectrBuffer.append(power);

    if(isSending)
    {
//        // sort
//        std::sort(m_powerSpectrBuffer.begin(), m_powerSpectrBuffer.end(), [](const PowerSpectr& a, const PowerSpectr& b) {
//            return a.m_frequency_min < b.m_frequency_min;
//        });

        sweep_message send_data;
        send_data.set_type(type_message::data_spectr);

        data_spectr spectr;
        spectr.set_id_params(params_id_str);
        spectr.set_spectr(m_powerSpectrBuffer);

        send_data.set_data_message(spectr.to_json());

        emit signal_sweep_message(send_data.to_json());

        m_powerSpectrBuffer.clear();
    }
}

int spectrum_native_worker::rx_callback(hackrf_transfer *transfer)
{
    spectrum_native_worker *obj = (spectrum_native_worker *)transfer->rx_ctx;
    if( transfer->valid_length == 0) return(0);
    return obj->hackrf_rx_callback(transfer->buffer, transfer->valid_length);
}

int spectrum_native_worker::hackrf_rx_callback(unsigned char *buffer, uint32_t length)
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
        power_spectr dataPowerSpectr;
        bool isSending = false;
        dataPowerSpectr.m_date_time = QDateTime::currentDateTimeUtc();

        time_now = time(nullptr);
        fft_time = localtime(&time_now);
        strftime(time_str, 50, "%Y-%m-%d, %H:%M:%S", fft_time);

        dataPowerSpectr.hz_low = static_cast<uint64_t>(frequency);
        dataPowerSpectr.hz_high = static_cast<uint64_t>(frequency + DEFAULT_SAMPLE_RATE_HZ/4);
        dataPowerSpectr.m_fft_bin_width = DEFAULT_SAMPLE_RATE_HZ / fftSize;
        //dataPowerSpectr.m_fft_size = static_cast<quint32>(fftSize) ;

        for(int i = 0; (fftSize / 4) > i; i++)
            dataPowerSpectr.m_power.append(static_cast<qreal>(pwr[i + 1 + (fftSize*5)/8]));

        // segment 1
        getInstance()->onDataPowerSpectrCallbacks(dataPowerSpectr);

        //---------------------------------------------------------------------
        dataPowerSpectr.m_power.clear();
        dataPowerSpectr.hz_low = static_cast<uint64_t>(frequency+(DEFAULT_SAMPLE_RATE_HZ/2));
        dataPowerSpectr.hz_high = static_cast<uint64_t>(frequency+((DEFAULT_SAMPLE_RATE_HZ*3)/4));
        dataPowerSpectr.m_fft_bin_width = DEFAULT_SAMPLE_RATE_HZ / fftSize;
        //dataPowerSpectr.m_fft_size = static_cast<quint32>(fftSize) ;

        for(int i = 0; (fftSize / 4) > i; i++)
            dataPowerSpectr.m_power.append(static_cast<qreal>(pwr[i + 1 + (fftSize/8)]));

        if(static_cast<uint64_t>(frequency+((DEFAULT_SAMPLE_RATE_HZ*3)/4))
                >= static_cast<uint64_t>(FREQ_ONE_MHZ*frequencies[num_ranges*2-1]))
        {
            isSending = true;

            if(one_shot){
                do_exit = true;
            }
        }

        // segment 2
        getInstance()->onDataPowerSpectrCallbacks(dataPowerSpectr, isSending);

    } // for(j=0; j<BLOCKS_PER_TRANSFER; j++)

    return 0;
}

float spectrum_native_worker::logPower(fftwf_complex in, float scale)
{
    float re = in[0] * scale;
    float im = in[1] * scale;
    float magsq = re * re + im * im;
    return log2f(magsq) * 10.0f / log2(10.0f);
}

float spectrum_native_worker::TimevalDiff(const timeval *a, const timeval *b)
{
    return (a->tv_sec - b->tv_sec) + 1e-6f * (a->tv_usec - b->tv_usec);
}

void spectrum_native_worker::errorHackrf(const QString &text, int result)
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

void spectrum_native_worker::sweepWorkerMessagelog(const QString &value)
{
    data_log message;
    message.set_text_message(value);

    sweep_message send_data;
    send_data.set_type(type_message::data_message_log);
    send_data.set_data_message(message.to_json());

    emit signal_sweep_message(send_data.to_json());
}

void spectrum_native_worker::slot_run_sweep_worker(const QByteArray &value)
{
    const sweep_message ctrl_info(value);
    const params_spectr params_spectr_data(ctrl_info.data_message());

    fft_bin_width = params_spectr_data.fft_bin_width(); // FFT bin width (frequency resolution) in Hz
    lna_gain = params_spectr_data.lna_gain();           // RX LNA (IF) gain, 0-40dB, 8dB steps
    vga_gain = params_spectr_data.vga_gain();           // RX VGA (baseband) gain, 0-62dB, 2dB steps
    strncpy(params_id_str, qPrintable(params_spectr_data.id_params()), 7);

    //qstrncpy(params_id_str, params_spectr_data.id_params(), 7);

//    char c[100];
//    QStrint s;
//    strncpy(c, qPrintable(s), 99);

    //char * qstrncpy ( char * dst, const char * src, uint len )?

    if (0 == num_ranges) {
        frequencies[0] = static_cast<uint16_t>(params_spectr_data.frequency_min());
        frequencies[1] = static_cast<uint16_t>(params_spectr_data.frequency_max());
        num_ranges++;
    }

    fftSize = DEFAULT_SAMPLE_RATE_HZ / fft_bin_width;
    one_shot = params_spectr_data.one_shot();

    amp = true;

    QString msgTask;
    msgTask.append(tr("fft bin:"));
    msgTask.append(QString::number(fft_bin_width));
    msgTask.append(tr(" lna:"));
    msgTask.append(QString::number(lna_gain));
    msgTask.append(tr(" vga:"));
    msgTask.append(QString::number(vga_gain));
    msgTask.append(tr(" freq min:"));
    msgTask.append(QString::number(frequencies[0]));
    msgTask.append(tr(" freq max:"));
    msgTask.append(QString::number(frequencies[1]));
    msgTask.append(tr(" one shot:"));
    if(one_shot)
        msgTask.append("true");
    else
        msgTask.append("false");

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

    fft_bin_width = static_cast<uint32_t>(DEFAULT_SAMPLE_RATE_HZ / fftSize);
    fftwIn = (fftwf_complex*)fftwf_malloc(sizeof(fftwf_complex) * fftSize);
    fftwOut = (fftwf_complex*)fftwf_malloc(sizeof(fftwf_complex) * fftSize);
    fftwPlan = fftwf_plan_dft_1d(fftSize, fftwIn, fftwOut, FFTW_FORWARD, FFTW_MEASURE);
    pwr = (float*)fftwf_malloc(sizeof(float) * fftSize);
    window = (float*)fftwf_malloc(sizeof(float) * fftSize);

    for (i = 0; i < fftSize; i++) {
        window[i] = (float) (0.5f * (1.0f - cos(2 * M_PI * i / (fftSize - 1))));
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
        step_count = 1 + (frequencies[2*i+1] - frequencies[2*i] - 1) / TUNE_STEP;
        // frequencies[2*i+1] = (uint16_t) (frequencies[2*i] + step_count * TUNE_STEP);
        frequencies[2*i+1] = static_cast<uint16_t> (frequencies[2*i] + static_cast<uint16_t>(step_count) * TUNE_STEP);

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
        result = hackrf_set_amp_enable(device, static_cast<uint8_t>(amp_enable));
        if (result != HACKRF_SUCCESS) {
            errorHackrf("hackrf_set_amp_enable() failed:", result);
            exit(0);
        }
    }

    gettimeofday(&t_start, nullptr);
    gettimeofday(&time_start, nullptr);

    if((hackrf_is_streaming(device) == HACKRF_TRUE) && (do_exit == false))
        emit signal_sweep_worker(true);

    while((hackrf_is_streaming(device) == HACKRF_TRUE) && (do_exit == false))
    {
        uint32_t byte_count_now;
        struct timeval time_now;
        float time_difference, rate;
        sleep(1);

        gettimeofday(&time_now, nullptr);

        byte_count_now = byte_count;
        byte_count = 0;

        time_difference = TimevalDiff(&time_now, &time_start);
        rate = static_cast<float>(byte_count_now / time_difference);

        fprintf(stderr, "%4.1f MiB / %5.3f sec = %4.1f MiB/second\n",
                (byte_count_now / 1e6f), time_difference, (rate / 1e6f) );

        sweepWorkerMessagelog(tr("%1 MiB / %2 sec = %3 MiB/second")
                              .arg(static_cast<qreal>(byte_count_now/1e6f))
                              .arg(static_cast<qreal>(time_difference))
                              .arg(static_cast<qreal>(rate/1e6f)) );

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
    fprintf(stderr, "Total time: %5.5f s\n", static_cast<qreal>(time_diff));

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

    do_exit = false;
    num_ranges = 0;

    emit signal_sweep_worker(false);
}

void spectrum_native_worker::slot_stop_sweep_worker()
{
    do_exit = true;
}
