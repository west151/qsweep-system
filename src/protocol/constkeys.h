#ifndef CONSTKEYS_H
#define CONSTKEYS_H

#include <QString>

static const QString DT_FORMAT = QStringLiteral("hh:mm dd.MM.yyyy");

static const QString DATA_KEY = QStringLiteral("data");
static const QString DT_KEY = QStringLiteral("dt");

static const QString INDEX_KEY = QStringLiteral("index");
static const QString SERIAL_NUMBERS_KEY = QStringLiteral("serial");
static const QString BOARD_ID_KEY = QStringLiteral("board_id");
static const QString FIRMWARE_VERSION_KEY = QStringLiteral("firmware_version");
static const QString PART_ID_NUMBER_KEY = QStringLiteral("part_id_number");
static const QString LIB_HACKRF_VERSION_KEY = QStringLiteral("lib_hackrf_version");

static const QString ID_KEY = QStringLiteral("id");
static const QString TYPE_MESSAGE_KEY = QStringLiteral("type_msg");
static const QString TYPE_ANSWER_KEY = QStringLiteral("TypeAnswer");
static const QString DT_ANSWER_KEY = QStringLiteral("DateTimeAnswer");
static const QString TEXT_MESSAGE_KEY = QStringLiteral("text_msg");

static const QString FREQUENCY_MIN_KEY = QStringLiteral("frequency_min");
static const QString FREQUENCY_MAX_KEY = QStringLiteral("frequency_max");
static const QString FFT_BIN_WIDTH_KEY = QStringLiteral("fft_bin_width");
static const QString LNA_GAIN_KEY = QStringLiteral("lna_gain");
static const QString VGA_GAIN_KEY = QStringLiteral("vga_gain");
static const QString ONE_SHOT_KEY = QStringLiteral("one_shot");
static const QString START_SPECTR_KEY = QStringLiteral("start_spectr");

static const QString TYPE_REQUEST_KEY = QStringLiteral("TypeRequest");
static const QString DT_REQUEST_KEY = QStringLiteral("DateTimeRequest");

static const QString POWERS_KEY = QStringLiteral("powers");
static const QString FFT_SIZE_KEY = QStringLiteral("fft_size");

static const QString HOST_NAME_KEY = QStringLiteral("hostname");
static const QString UPTIME_KEY = QStringLiteral("uptime");
static const QString CPU_ARCHITECTURE_KEY = QStringLiteral("cpu_arch");
static const QString TOTAL_MEMORY_KEY = QStringLiteral("totalmem");
static const QString FREE_MEMORY_KEY = QStringLiteral("freememory");
static const QString BUFFER_MEMORY_KEY = QStringLiteral("buffermem");

#endif // CONSTKEYS_H
