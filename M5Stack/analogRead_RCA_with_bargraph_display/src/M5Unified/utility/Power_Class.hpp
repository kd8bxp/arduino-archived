// Copyright (c) M5Stack. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#ifndef __M5_Power_Class_H__
#define __M5_Power_Class_H__

#include "I2C_Class.hpp"
#include "AXP192_Class.hpp"
#include "AXP2101_Class.hpp"
#include "IP5306_Class.hpp"
#include "RTC8563_Class.hpp"

#include <driver/adc.h>

namespace m5
{
  class M5Unified;

  enum ext_port_mask_t
  { ext_none = 0
  , ext_PA   = 0b00000001
  , ext_PB1  = 0b00000010
  , ext_PB2  = 0b00000100
  , ext_PC1  = 0b00001000
  , ext_PC2  = 0b00010000
  , ext_USB  = 0b00100000
  , ext_MAIN = 0b10000000
  };

  class Power_Class
  {
  friend M5Unified;
  public:

    enum pmic_t
    { pmic_unknown
    , pmic_adc
    , pmic_axp192
    , pmic_ip5306
    , pmic_axp2101
    };

    enum is_charging_t
    { is_discharging = 0
    , is_charging
    , charge_unknown
    };

    bool begin(void);

    /// Set power supply to external ports.
    /// @param enable true=output / false=input
    /// @param port_mask for M5Station. ext_port (bitmask).
    void setExtPower(bool enable, ext_port_mask_t port_mask = (ext_port_mask_t)0xFF);

    /// Turn on/off the power LED.
    /// @param brightness 0=OFF: 1~255=ON (Set brightness if possible.)
    void setLed(uint8_t brightness = 255);

    /// all power off.
    void powerOff(void);

    /// sleep and timer boot. The boot condition can be specified by the argument.
    /// @param seconds Number of seconds to boot.
    void timerSleep(int seconds);

    /// sleep and timer boot. The boot condition can be specified by the argument.
    /// @param time Time to boot. (only minutes and hours can be specified. Ignore seconds)
    /// @attention CoreInk and M5Paper can't alarm boot because it can't be turned off while connected to USB.
    /// @attention CoreInk と M5Paper は USB接続中はRTCタイマー起動が出来ない。;
    void timerSleep(const rtc_time_t& time);

    /// sleep and timer boot. The boot condition can be specified by the argument.
    /// @param date Date to boot. (only date and weekDay can be specified. Ignore year and month)
    /// @param time Time to boot. (only minutes and hours can be specified. Ignore seconds)
    /// @attention CoreInk and M5Paper can't alarm boot because it can't be turned off while connected to USB.
    /// @attention CoreInk と M5Paper は USB接続中はRTCタイマー起動が出来ない。;
    void timerSleep(const rtc_date_t& date, const rtc_time_t& time);

    /// ESP32 deepsleep
    /// @param seconds Number of micro seconds to wakeup.
    void deepSleep(std::uint64_t micro_seconds = 0, bool touch_wakeup = true);

    /// ESP32 lightsleep
    /// @param seconds Number of micro seconds to wakeup.
    void lightSleep(std::uint64_t micro_seconds = 0, bool touch_wakeup = true);

    /// Get the remaining battery power.
    /// @return 0-100 level
    std::int32_t getBatteryLevel(void);

    /// set battery charge enable.
    /// @param enable true=enable / false=disable
    void setBatteryCharge(bool enable);

    /// set battery charge current
    /// @param max_mA milli ampere.
    /// @attention Non-functioning models : CoreInk , M5Paper , M5Stack(with non I2C IP5306)
    void setChargeCurrent(std::uint16_t max_mA);

    /// set battery charge voltage
    /// @param max_mV milli volt.
    /// @attention Non-functioning models : CoreInk , M5Paper , M5Stack(with non I2C IP5306)
    void setChargeVoltage(std::uint16_t max_mV);

    /// Get whether the battery is currently charging or not.
    /// @attention Non-functioning models : CoreInk , M5Paper , M5Stack(with non I2C IP5306)
    is_charging_t isCharging(void);

    pmic_t getType(void) const { return _pmic; }

#if defined (CONFIG_IDF_TARGET_ESP32S3)

    AXP2101_Class Axp2101;

#elif defined (CONFIG_IDF_TARGET_ESP32C3)

#else

    AXP192_Class Axp192;
    IP5306_Class Ip5306;

#endif

  private:
    void _powerOff(bool withTimer);
    void _timerSleep(void);

    std::int8_t _wakeupPin = -1;
    std::int8_t _pwrHoldPin = -1;
    pmic_t _pmic = pmic_t::pmic_unknown;
    adc1_channel_t _batAdc;
    float _adc_ratio = 0;
  };
}

#endif
