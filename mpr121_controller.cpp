#pragma once
#include "mpr121_controller.h"

MPR121_Controller::MPR121_Controller() : ft_handle(nullptr), ft_status(FT_OK), mpr_address(0x59) {}

MPR121_Controller::~MPR121_Controller() {
    if (ft_handle) {
        FT_Close(ft_handle);
    }
}

FT_STATUS MPR121_Controller::configure(const std::string& serial_number) {
    ft_status = FT_OpenEx((PVOID)serial_number.c_str(), FT_OPEN_BY_SERIAL_NUMBER, &ft_handle);
    emit logMessage(QString("FT_OpenEx status: %1\n").arg(ft_status));
    if (ft_status != FT_OK) {
        emit logMessage(QString("Error opening FTDI device: %1\n").arg(ft_status));
        return ft_status;
    }
    ChannelConfig config;
    config.ClockRate = I2C_CLOCK_STANDARD_MODE; // 100kHz
    config.LatencyTimer = 16; // 16ms
    ft_status = I2C_InitChannel(ft_handle, &config);
    emit logMessage(QString("I2C_InitChannel status: %1\n").arg(ft_status));

    DWORD reset_written;
    ft_status = FT_DEVICE_NOT_FOUND;
    while (ft_status == FT_DEVICE_NOT_FOUND)
    {
        mpr_address++;
        ft_status = I2C_DeviceWrite(ft_handle, mpr_address, 2, (UCHAR *)MPR121_SOFT_RESET_CMD, &reset_written, I2C_TRANSFER_OPTIONS_START_BIT | I2C_TRANSFER_OPTIONS_STOP_BIT);
        if (mpr_address > 0x5D) {
            emit logMessage("No MPR121 found\n");
            return FT_DEVICE_NOT_FOUND;
        }
    }
    emit logMessage(QString("Found MPR121 at address: 0x%1").arg(mpr_address, 2, 16, QChar('0')));
    return FT_OK;
}

FT_STATUS MPR121_Controller::sensor_read(UCHAR *rx_data)
{
    // Write the address for the data with the lowest level of filtering
    // We have to write the address so that the MPR121 knows where to start reading from.
    UCHAR data[1] = {MPR121_FILTDATA_0L};
    DWORD written = 0;
    ft_status = I2C_DeviceWrite(ft_handle, mpr_address, sizeof(data), data, &written, I2C_TRANSFER_OPTIONS_START_BIT | I2C_TRANSFER_OPTIONS_STOP_BIT);
    if (ft_status != FT_OK) {
        emit logMessage(QString("Error writing to MPR121: %1").arg(ft_status));
        return ft_status;
    }
    // Read back 24 bytes of data from the MPR121 (2 bytes for each of the 12 channels)
    DWORD read = 0;
    ft_status = I2C_DeviceRead(ft_handle, mpr_address, sizeof(rx_data), rx_data, &read, I2C_TRANSFER_OPTIONS_START_BIT | I2C_TRANSFER_OPTIONS_STOP_BIT);
    if (ft_status != FT_OK) {
        emit logMessage(QString("Error reading from MPR121: %1").arg(ft_status));
        return ft_status;
    }
    
    return ft_status;
}

FT_STATUS MPR121_Controller::stop_recording()
{
    DWORD stop_written = 0;
    ft_status = I2C_DeviceWrite(ft_handle, mpr_address, 2, (UCHAR *)MPR121_STOP_CMD, &stop_written, I2C_TRANSFER_OPTIONS_START_BIT | I2C_TRANSFER_OPTIONS_STOP_BIT);
    if ( ft_status != FT_OK ) {
        emit logMessage(QString("Error stopping recording: %1").arg(ft_status));
        return ft_status;
    }
    emit logMessage(QString("Stopped recording: %1").arg(ft_status));
    return ft_status;
}

FT_STATUS MPR121_Controller::start_recording()
{
    DWORD start_written = 0;
    ft_status = I2C_DeviceWrite(ft_handle, mpr_address, 2, (UCHAR *)MPR121_START_CMD, &start_written, I2C_TRANSFER_OPTIONS_START_BIT | I2C_TRANSFER_OPTIONS_STOP_BIT);
    if ( ft_status != FT_OK ) {
        emit logMessage(QString("Error starting recording: %1").arg(ft_status));
        return ft_status;
    }
    emit logMessage(QString("Started recording: %1").arg(ft_status));
    return ft_status;
}