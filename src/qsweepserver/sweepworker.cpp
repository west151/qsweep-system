#include "sweepworker.h"

#include "sweepparams.h"

#ifdef QT_DEBUG
#include <QtCore/qdebug.h>
#endif

#define _FILE_OFFSET_BITS 64

int num_ranges = 0;
uint16_t frequencies[MAX_SWEEP_RANGES*2];

FILE* fd = NULL;
volatile uint32_t byte_count = 0;

struct timeval time_start;
struct timeval t_start;

bool amp = false;
uint32_t amp_enable;

bool antenna = false;
uint32_t antenna_enable;

//bool binary_output = false;
bool one_shot = true;
volatile bool sweep_started = false;
volatile bool do_exit = false;

int fftSize = 20;
fftwf_complex *fftwIn = NULL;
fftwf_complex *fftwOut = NULL;
fftwf_plan fftwPlan = NULL;
float* pwr;
float* window;
time_t time_now;
struct tm *fft_time;
char time_str[50];

#if defined(__GNUC__)
#include <unistd.h>
#include <sys/time.h>
#endif

static float TimevalDiff(const struct timeval *a, const struct timeval *b) {
   return (a->tv_sec - b->tv_sec) + 1e-6f * (a->tv_usec - b->tv_usec);
}

float logPower(fftwf_complex in, float scale)
{
    float re = in[0] * scale;
    float im = in[1] * scale;
    float magsq = re * re + im * im;
    return log2f(magsq) * 10.0f / log2(10.0f);
}

int SweepWorker::rx_callback(hackrf_transfer *transfer)
{
    SweepWorker *obj = (SweepWorker *)transfer->rx_ctx;
    return obj->hackrf_rx_callback(transfer->buffer, transfer->valid_length);
}

int SweepWorker::hackrf_rx_callback(unsigned char *buffer, uint32_t length)
{
    int8_t* buf;
    uint8_t* ubuf;
    uint64_t frequency; /* in Hz */

    if(NULL == fd) {
        return -1;
    }

    byte_count += length;
    buf = (int8_t*) buffer;

    for(int j=0; j<BLOCKS_PER_TRANSFER; j++) {
        if(do_exit) {
            return 0;
        }
        ubuf = (uint8_t*) buf;
        if(ubuf[0] == 0x7F && ubuf[1] == 0x7F) {
            frequency = ((uint64_t)(ubuf[9]) << 56) | ((uint64_t)(ubuf[8]) << 48) | ((uint64_t)(ubuf[7]) << 40)
                    | ((uint64_t)(ubuf[6]) << 32) | ((uint64_t)(ubuf[5]) << 24) | ((uint64_t)(ubuf[4]) << 16)
                    | ((uint64_t)(ubuf[3]) << 8) | ubuf[2];
        } else {
            buf += SAMPLES_PER_BLOCK;
            continue;
        }
        if(!sweep_started) {
            if (frequency == (uint64_t)(FREQ_ONE_MHZ*frequencies[0])) {
                sweep_started = true;
            } else {
                buf += SAMPLES_PER_BLOCK;
                continue;
            }
        }
        if((FREQ_MAX_MHZ * FREQ_ONE_MHZ) < frequency) {
            buf += SAMPLES_PER_BLOCK;
            continue;
        }
        /* copy to fftwIn as floats */
        buf += SAMPLES_PER_BLOCK - (fftSize * 2);
        for(int i=0; i < fftSize; i++) {
            fftwIn[i][0] = buf[i*2] * window[i] * 1.0f / 128.0f;
            fftwIn[i][1] = buf[i*2+1] * window[i] * 1.0f / 128.0f;
        }
        buf += fftSize * 2;
        fftwf_execute(fftwPlan);
        for (int i=0; i < fftSize; i++) {
            pwr[i] = logPower(fftwOut[i], 1.0f / fftSize);
        }

        time_now = time(NULL);
        fft_time = localtime(&time_now);
        strftime(time_str, 50, "%Y-%m-%d, %H:%M:%S", fft_time);

        //qDebug() << "fft_bin_width:" << (float)fft_bin_width;

        printf("%s, %" PRIu64 ", %" PRIu64 ", %u",
               time_str,
               (uint64_t)(frequency),
               (uint64_t)(frequency+DEFAULT_SAMPLE_RATE_HZ/4),
               fftSize);

        for(int i=1+(fftSize*5)/8; (1+(fftSize*7)/8) > i; i++) {
            printf(", %.2f", pwr[i]);
        }
        printf(" ups1 (%u) \n", (uint32_t)((1+(fftSize*7)/8)-(1+(fftSize*5)/8)));

        printf("%s, %" PRIu64 ", %" PRIu64 ", %u",
               time_str,
               (uint64_t)(frequency+(DEFAULT_SAMPLE_RATE_HZ/2)),
               (uint64_t)(frequency+((DEFAULT_SAMPLE_RATE_HZ*3)/4)),
               fftSize);

        for(int i=1+fftSize/8; (1+(fftSize*3)/8) > i; i++) {
            printf(", %.2f", pwr[i]);
        }
        printf(" ups2 (%u) \n", (uint32_t)((1+(fftSize*3)/8) - (1+fftSize/8) ));

        //        }
        if(one_shot && ((uint64_t)(frequency+((DEFAULT_SAMPLE_RATE_HZ*3)/4))
                        >= (uint64_t)(FREQ_ONE_MHZ*frequencies[num_ranges*2-1]))) {
            do_exit = true;
        }

        //qDebug() << "BLOCKS_PER_TRANSFER";

    } // for(j=0; j<BLOCKS_PER_TRANSFER; j++)

    //printf("RETURN 0\n");

    return 0;
}

void SweepWorker::testSender()
{

}

SweepWorker::SweepWorker(QObject *parent) : QObject(parent)
{

}

void SweepWorker::onRunSweepWorker()
{
//    freq_min = freqMin;
//    freq_max = freqMax;
//    fft_bin_width = fftBinWidth;    // FFT bin width (frequency resolution) in Hz
//    lna_gain = lnaGain;     // RX LNA (IF) gain, 0-40dB, 8dB steps
//    vga_gain = vgaGain;     // RX VGA (baseband) gain, 0-62dB, 2dB steps

    if (0 == num_ranges) {
        frequencies[0] = (uint16_t)freq_min;
        frequencies[1] = (uint16_t)freq_max;
        num_ranges++;
    }

    fftSize = DEFAULT_SAMPLE_RATE_HZ / fft_bin_width;

    one_shot = true;

    amp = true;

    if(4 > fftSize) {
        fprintf(stderr,"argument error: FFT bin width (-w) must be no more than one quarter the sample rate\n");
        exit(0);
        //return EXIT_FAILURE;
    }

    if(16368 < fftSize) {
        fprintf(stderr,	"argument error: FFT bin width (-w) too small, resulted in more than 16368 FFT bins\n");
        exit(0);
        //return EXIT_FAILURE;
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
        window[i] = 0.5f * (1.0f - cos(2 * M_PI * i / (fftSize - 1)));
    }

    result = hackrf_init();
    if( result != HACKRF_SUCCESS ) {
        fprintf(stderr, "hackrf_init() failed: %s (%d)\n", hackrf_error_name(static_cast<hackrf_error>(result)), result);
        exit(0);
        //return EXIT_FAILURE;
    }

    result = hackrf_open_by_serial(serial_number, &device);
    if( result != HACKRF_SUCCESS ) {
        fprintf(stderr, "hackrf_open() failed: %s (%d)\n", hackrf_error_name(static_cast<hackrf_error>(result)), result);
        exit(0);
        //return EXIT_FAILURE;
    }

    fd = fopen(path, "wb");
    if( fd == NULL ) {
        fprintf(stderr, "Failed to open file: %s\n", path);
        exit(0);
        //return EXIT_FAILURE;
    }
    /* Change fd buffer to have bigger one to store or read data on/to HDD */
    result = setvbuf(fd , NULL , _IOFBF , FD_BUFFER_SIZE);
    if( result != 0 ) {
        fprintf(stderr, "setvbuf() failed: %d\n", result);
        exit(0);
        //return EXIT_FAILURE;
    }

    fprintf(stderr, "call hackrf_sample_rate_set(%.03f MHz)\n",
            ((float)DEFAULT_SAMPLE_RATE_HZ/(float)FREQ_ONE_MHZ));

    result = hackrf_set_sample_rate_manual(device, DEFAULT_SAMPLE_RATE_HZ, 1);

    if( result != HACKRF_SUCCESS ) {
        fprintf(stderr, "hackrf_sample_rate_set() failed: %s (%d)\n",
                hackrf_error_name(static_cast<hackrf_error>(result)), result);
        exit(0);
        //return EXIT_FAILURE;
    }

    fprintf(stderr, "call hackrf_baseband_filter_bandwidth_set(%.03f MHz)\n",
            ((float)DEFAULT_BASEBAND_FILTER_BANDWIDTH/(float)FREQ_ONE_MHZ));

    result = hackrf_set_baseband_filter_bandwidth(device, DEFAULT_BASEBAND_FILTER_BANDWIDTH);
    if( result != HACKRF_SUCCESS ) {
        fprintf(stderr, "hackrf_baseband_filter_bandwidth_set() failed: %s (%d)\n",
                hackrf_error_name(static_cast<hackrf_error>(result)), result);
        exit(0);
        //return EXIT_FAILURE;
    }

    result = hackrf_set_vga_gain(device, vga_gain);
    result |= hackrf_set_lna_gain(device, lna_gain);
    result |= hackrf_start_rx(device, rx_callback, NULL);
    if (result != HACKRF_SUCCESS) {
        fprintf(stderr, "hackrf_start_rx() failed: %s (%d)\n", hackrf_error_name(static_cast<hackrf_error>(result)), result);
        exit(0);
        //return EXIT_FAILURE;
    }

    /*
     * For each range, plan a whole number of tuning steps of a certain
     * bandwidth. Increase high end of range if necessary to accommodate a
     * whole number of steps, minimum 1.
     */
    for(i = 0; i < num_ranges; i++) {
        step_count = 1 + (frequencies[2*i+1] - frequencies[2*i] - 1)
                / TUNE_STEP;
        frequencies[2*i+1] = frequencies[2*i] + step_count * TUNE_STEP;
        fprintf(stderr, "Sweeping from %u MHz to %u MHz\n",
                frequencies[2*i], frequencies[2*i+1]);
    }


    result = hackrf_init_sweep(device, frequencies, num_ranges, num_samples,
            TUNE_STEP * FREQ_ONE_MHZ, OFFSET, INTERLEAVED);

    if( result != HACKRF_SUCCESS ) {
        fprintf(stderr, "hackrf_init_sweep() failed: %s (%d)\n",
               hackrf_error_name(static_cast<hackrf_error>(result)), result);
        exit(0);
        //return EXIT_FAILURE;
    }

    if (amp) {
        fprintf(stderr, "call hackrf_set_amp_enable(%u)\n", amp_enable);
        result = hackrf_set_amp_enable(device, (uint8_t)amp_enable);
        if (result != HACKRF_SUCCESS) {
            fprintf(stderr, "hackrf_set_amp_enable() failed: %s (%d)\n",
                    hackrf_error_name(static_cast<hackrf_error>(result)), result);
            exit(0);
            //return EXIT_FAILURE;
        }
    }

    gettimeofday(&t_start, NULL);
    gettimeofday(&time_start, NULL);

    fprintf(stderr, "Stop with Ctrl-C\n");

    while((hackrf_is_streaming(device) == HACKRF_TRUE) && (do_exit == false)) {
        uint32_t byte_count_now;
        struct timeval time_now;
        float time_difference, rate;
        sleep(1);

        gettimeofday(&time_now, NULL);

        byte_count_now = byte_count;
        byte_count = 0;

        time_difference = TimevalDiff(&time_now, &time_start);
        rate = (float)byte_count_now / time_difference;
        fprintf(stderr, "%4.1f MiB / %5.3f sec = %4.1f MiB/second\n",
                (byte_count_now / 1e6f), time_difference, (rate / 1e6f) );

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
    } else {
        fprintf(stderr, "\nExiting... hackrf_is_streaming() result: %s (%d)\n",
               hackrf_error_name(static_cast<hackrf_error>(result)), result);
    }

    gettimeofday(&t_end, NULL);
    time_diff = TimevalDiff(&t_end, &t_start);
    fprintf(stderr, "Total time: %5.5f s\n", time_diff);

    if(device != NULL) {
        result = hackrf_stop_rx(device);
        if(result != HACKRF_SUCCESS) {
            fprintf(stderr, "hackrf_stop_rx() failed: %s (%d)\n", hackrf_error_name(static_cast<hackrf_error>(result)), result);
        } else {
            fprintf(stderr, "hackrf_stop_rx() done\n");
        }

        result = hackrf_close(device);

        if(result != HACKRF_SUCCESS) {
            fprintf(stderr, "hackrf_close() failed: %s (%d)\n", hackrf_error_name(static_cast<hackrf_error>(result)), result);
        } else {
            fprintf(stderr, "hackrf_close() done\n");
        }

        hackrf_exit();
        fprintf(stderr, "hackrf_exit() done\n");
    }

    if(fd != NULL) {
        fclose(fd);
        fd = NULL;
        fprintf(stderr, "fclose(fd) done\n");
    }

    fftwf_free(fftwIn);
    fftwf_free(fftwOut);
    fftwf_free(pwr);
    fftwf_free(window);

    //return EXIT_SUCCESS;
}

void SweepWorker::onStopSweepWorker()
{

}

void SweepWorker::onParamsSweepWorker(const SweepParams &value)
{
//    freq_min = freqMin;
//    freq_max = freqMax;
//    fft_bin_width = fftBinWidth;    // FFT bin width (frequency resolution) in Hz
//    lna_gain = lnaGain;     // RX LNA (IF) gain, 0-40dB, 8dB steps
//    vga_gain = vgaGain;     // RX VGA (baseband) gain, 0-62dB, 2dB steps

//    const uint32_t &freqMin = 30 /* MHz */,
//            const uint32_t &freqMax = 6000 /* MHz */,
//            const uint32_t &fftBinWidth = 500000 /* Hz */,
//            const unsigned int &lnaGain = 0,
//            const unsigned int &vgaGain = 0
}
