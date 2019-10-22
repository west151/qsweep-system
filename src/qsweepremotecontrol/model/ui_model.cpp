#include "ui_model.h"

ui_model::ui_model(QObject *parent) : QObject(parent)
{

}

void ui_model::set_frequency_min(const quint32 &value)
{
    m_freq_min = value;

    emit signal_frequency_min_changed();
}

quint32 ui_model::frequency_min() const
{
    return m_freq_min;
}

void ui_model::set_frequency_max(const quint32 &value)
{
    m_freq_max = value;

    emit signal_frequency_max_changed();
}

quint32 ui_model::frequency_max() const
{
    return m_freq_max;
}

void ui_model::set_lna_gain(const quint32 &value)
{
    m_lna_gain = value;

    emit signal_lna_gain_changed();
}

quint32 ui_model::lna_gain() const
{
    return m_lna_gain;
}

void ui_model::set_vga_gain(const quint32 &value)
{
    m_vga_gain = value;

    emit signal_vga_gain_changed();
}

quint32 ui_model::vga_gain() const
{
    return m_vga_gain;
}

void ui_model::set_fft_bin_width(const quint32 &value)
{
    m_fft_bin_width = value;

    emit signal_fft_bin_width_changed();
}

quint32 ui_model::fft_bin_width() const
{
    return m_fft_bin_width;
}

QString ui_model::last_error_string() const
{
    return m_last_error_string;
}

void ui_model::set_last_error_string(const QString &value)
{
    m_last_error_string = value;

    emit signal_last_error_string_changed();
}


//in_freq_min{
//    //validator : RegExpValidator { regExp : /[0-9]+\.[0-9]+/ }
//    validator : IntValidator{bottom: 30; top: 6000;}
//    inputMethodHints: Qt.ImhDigitsOnly
//}

//in_freq_max{
//    validator : IntValidator{bottom: 30; top: 6000;}
//    inputMethodHints: Qt.ImhDigitsOnly
//}

//SurfaceSpectr {
//    id: idSpectrItem
//    objectName: "objSurfaceSpectr"
//    anchors.fill: item_spectr_surface
//}

//cbx_fft_size {
//    currentIndex: 2
//    textRole: "key"
//    model: ListModel {
//        id: fftSizeModel
//        ListElement { key: "500000 Hz"; value: 500000 }
//        ListElement { key: "250000 Hz"; value: 250000 }
//        ListElement { key: "125000 Hz"; value: 125000 }
//        ListElement { key: "100000 Hz"; value: 100000 }
//        ListElement { key: "50000 Hz"; value: 50000 }
//        ListElement { key: "25000 Hz"; value: 25000 }
//    }

//    onActivated: {
//        console.log("cbx_fft_size index:", cbx_fft_size.currentText, fftSizeModel.get(cbx_fft_size.currentIndex).value)
//        userInterface.fftBinWidth = fftSizeModel.get(cbx_fft_size.currentIndex).value
//    }
//}

//// RX VGA (baseband) gain, 0-62dB, 2dB steps
//cbx_vga_gain {
//    currentIndex: 10
//    textRole: "key"
//    inputMethodHints: Qt.ImhDigitsOnly
//    model: ListModel {
//        id: vgaGainModel
//        Component.onCompleted: {
//            for (var i = 0; i <= 62; i=i+2) {
//                vgaGainModel.append({"key":i+" dB", "value":i})
//            }
//        }
//    }

//    onActivated: {
//        console.log("cbx_vga_gain index:", cbx_vga_gain.currentText, vgaGainModel.get(cbx_vga_gain.currentIndex).value)
//        userInterface.vgaGain = vgaGainModel.get(cbx_vga_gain.currentIndex).value
//    }
//}

//// RX LNA (IF) gain, 0-40dB, 8dB steps
//cbx_lna_gain{
//    currentIndex: 4
//    textRole: "key"
//    model: ListModel {
//        id: lnaGainModel
//        ListElement { key: "0 dB"; value: 0 }
//        ListElement { key: "8 dB"; value: 8 }
//        ListElement { key: "16 dB"; value: 16 }
//        ListElement { key: "24 dB"; value: 24 }
//        ListElement { key: "32 dB"; value: 32 }
//        ListElement { key: "40 dB"; value: 40 }
//    }
//    onActivated: {
//        console.log("cbx_lna_gain index:", cbx_lna_gain.currentText, lnaGainModel.get(cbx_lna_gain.currentIndex).value)
//        userInterface.lnaGain = lnaGainModel.get(cbx_lna_gain.currentIndex).value // cbx_lna_gain.currentText
//    }
//}
