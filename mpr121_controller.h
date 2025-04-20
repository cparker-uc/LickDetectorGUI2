#ifndef MPR121_CONTROLLER_H
#define MPR121_CONTROLLER_H

#include <iostream>
#include <vector>
#include <string>
#include "ftd2xx.h"
#include "third_party/libmpsse/include/libmpsse_i2c.h"
#include "third_party/libmpsse/libftd2xx/ftd2xx.h"
#include <windows.h>
#include <QObject>

/*!
 *  MPR121 Device register map
 */
enum {
    MPR121_TOUCHSTATUS_L = 0x00,
    MPR121_TOUCHSTATUS_H = 0x01,
    MPR121_FILTDATA_0L = 0x04,
    MPR121_FILTDATA_0H = 0x05,
    MPR121_BASELINE_0 = 0x1E,
    MPR121_MHDR = 0x2B,
    MPR121_NHDR = 0x2C,
    MPR121_NCLR = 0x2D,
    MPR121_FDLR = 0x2E,
    MPR121_MHDF = 0x2F,
    MPR121_NHDF = 0x30,
    MPR121_NCLF = 0x31,
    MPR121_FDLF = 0x32,
    MPR121_NHDT = 0x33,
    MPR121_NCLT = 0x34,
    MPR121_FDLT = 0x35,
  
    MPR121_TOUCHTH_0 = 0x41,
    MPR121_RELEASETH_0 = 0x42,
    MPR121_DEBOUNCE = 0x5B,
    MPR121_CONFIG1 = 0x5C,
    MPR121_CONFIG2 = 0x5D,
    MPR121_CHARGECURR_0 = 0x5F,
    MPR121_CHARGETIME_1 = 0x6C,
    MPR121_ECR = 0x5E,
    MPR121_AUTOCONFIG0 = 0x7B,
    MPR121_AUTOCONFIG1 = 0x7C,
    MPR121_UPLIMIT = 0x7D,
    MPR121_LOWLIMIT = 0x7E,
    MPR121_TARGETLIMIT = 0x7F,
  
    MPR121_GPIODIR = 0x76,
    MPR121_GPIOEN = 0x77,
    MPR121_GPIOSET = 0x78,
    MPR121_GPIOCLR = 0x79,
    MPR121_GPIOTOGGLE = 0x7A,
  
    MPR121_SOFTRESET = 0x80,
  };

/*
 Some constant commands to send via I2C to the MPR121 device.
*/
constexpr UCHAR MPR121_STOP_CMD[2] = {MPR121_ECR, 0x00};
constexpr UCHAR MPR121_START_CMD[2] = {MPR121_ECR, 0x0F};
constexpr UCHAR MPR121_SOFT_RESET_CMD[2] = {MPR121_SOFTRESET, 0x63};

class MPR121_Controller : public QObject
{
  Q_OBJECT
public:
    MPR121_Controller();
    ~MPR121_Controller();

    FT_STATUS configure(const std::string& device_name);
    FT_STATUS sensor_read(UCHAR *rx_data);
    FT_STATUS stop_recording();
    FT_STATUS start_recording();

    FT_HANDLE ft_handle;
    FT_STATUS ft_status;
    DWORD mpr_address;
signals:
    void logMessage(const QString& message);
};



#endif // MPR121_CONTROLLER_H