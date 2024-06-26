/*

    ESP32 Library for BQ25896 Power Management and Battery Charger IC from Texas Instrument

    MIT License

    Copyright (c) 2024 sqmsmu

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in all
    copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
    SOFTWARE.

*/

#ifndef PMIC_BQ25896_H
#define PMIC_BQ25896_H

#include "Arduino.h"
#include "Wire.h"

typedef enum {
    BQ25896_ADDR = 0x6B
} bq25896_addr_t;

typedef enum {
    ILIM        = 0x00,
    VINDPM_OS   = 0x01,
    ADC_CTRL    = 0x02,
    SYS_CTRL    = 0x03,
    ICHG        = 0x04,
    IPRE_ITERM  = 0x05,
    VREG        = 0x06,
    TIMER       = 0x07,
    BAT_COMP    = 0x08,
    CTRL1       = 0x09,
    BOOST_CTRL  = 0x0A,
    VBUS_STAT   = 0x0B,
    FAULT       = 0x0C,
    VINDPM      = 0x0D,
    BATV        = 0x0E,
    SYSV        = 0x0F,
    TSPCT       = 0x10,
    VBUSV       = 0x11,
    ICHGR       = 0x12,
    IDPM_LIM    = 0x13,
    CTRL2       = 0x14
} bq25896_reg_t;

typedef enum {
    SUCCESS = 0x00,
    INVALID_RANGE
} bq25896_error_t;

typedef struct {
    // Input Current Limit
    // Offset: 100mA 
    // Range: 100mA (000000) – 3.25A (111111) (LSB = 50mA)
    // Default:0001000 (500mA)
    // (Actual input current limit is the lower of I2C or ILIM pin, changes with input type detection)
    uint8_t iinlim:6;
    // Enable ILIM Pin
    // 0 – Disable
    // 1 – Enable (default: Enable ILIM pin (1))
    uint8_t en_ilim:1;
    // Enable HIZ Mode
    // 0 - Disable (default)
    // 1 - Enable
    uint8_t en_hiz:1;
} ilim_reg_t __attribute__(());

typedef struct {
    // Input Voltage Limit Offset
    // Default: 600mV (00110)
    // Range: 0mV (00000) – 3100mV (11111) (LSB = 100mV)
    // Minimum VINDPM threshold is clamped at 3.9V
    // Maximum VINDPM threshold is clamped at 15.3V
    uint8_t vindpm_os:5;
    // Boost Mode Cold Temperature Monitor Threshold
    // 0 – VBCOLD0 Threshold (Typ. 77%) (default)
    // 1 – VBCOLD1 Threshold (Typ. 80%)
    uint8_t bcold:1;
    // Boost Mode Hot Temperature Monitor Threshold
    // 00 – VBHOT1 Threshold (34.75%) (default)
    // 01 – VBHOT0 Threshold (Typ. 37.75%)
    // 10 – VBHOT2 Threshold (Typ. 31.25%)
    // 11 – Disable boost mode thermal protection
    uint8_t bhot:2;
} vindpm_os_reg_t __attribute__(());

typedef struct {
    // Automatic Input Detection Enable
    // 0 – Disable PSEL detection when VBUS is plugged-in
    // 1 – Enable PEL detection when VBUS is plugged-in (default)
    uint8_t auto_dpdm_en:1;
    // Force Input Detection
    // 0 – Not in PSEL detection (default)
    // 1 – Force PSEL detection
    uint8_t force_dpdm:1;
    // Reserved - both bits defualt to 0
    uint8_t reserved:2;
    // Input Current Optimizer (ICO) Enable
    // 0 – Disable ICO Algorithm
    // 1 – Enable ICO Algorithm (default)
    uint8_t ico_en:1;
    // Boost Mode Frequency Selection
    // 0 – 1.5MHz (default)
    // 1 – 500KHz
    // Note: Write to this bit is ignored when OTG_CONFIG is enabled.
    uint8_t boost_freq:1;
    // ADC Conversion Rate Selection
    // 0 – One shot ADC conversion (default)
    // 1 – Start 1s Continuous Conversion
    uint8_t conv_rate:1;
    // ADC Conversion Start Control
    // 0 – ADC conversion not active (default).
    // 1 – Start ADC Conversion
    // This bit is read-only when CONV_RATE = 1. The bit stays high during ADC conversion and during input source detection.
    uint8_t conv_start:1;
} adc_ctrl_reg_t __attribute__(());

typedef struct {
    // Minimum Battery Voltage (falling) to exit boost mode
    // 0 - 2.9V (default)
    // 1 - 2.5V
    uint8_t min_vbat_sel:1;
    // Minimum System Voltage Limit
    // Offset: 3.0V
    // Range 3.0V (000) - 3.7V (111) (LSB = 0.1V)
    // Default: 3.5V (101)
    uint8_t sys_min:3;
    // Charge Enable Configuration
    // 0 - Charge Disable
    // 1 - Charge Enable (default)
    uint8_t chg_config:1;
    // Boost (OTG) Mode Configuration
    // 0 – OTG Disable (default)
    // 1 – OTG Enable
    uint8_t otg_config:1;
    // I2C Watchdog Timer Reset
    // 0 – Normal (default)
    // 1 – Reset (Back to 0 after timer reset)
    uint8_t wd_rst:1;
    // Battery Load (IBATLOAD) Enable
    // 0 – Disabled (default)
    // 1 – Enabled
    uint8_t bat_loaden:1;
} sys_ctrl_reg_t __attribute__(());

typedef struct {
    // Fast Charge Current Limit
    // Offset: 0mA
    // Range: 0mA (0000000) – 3008mA (0101111) (LSB = 64mA)
    // Default: 2048mA (0100000)
    // Note: ICHG=000000 (0mA) disables charge
    // Note: ICHG > 0101111 (3008mA) is clamped to register value 0101111 (3008mA)
    uint8_t ichg:7;
    // Current pulse control Enable
    // 0 - Disable Current pulse control (default)
    // 1- Enable Current pulse control (PUMPX_UP and PUMPX_DN)
    uint8_t en_pumpx:1;
} ichg_reg_t __attribute__(());

typedef struct {
    // Termination Current Limit
    // Offset: 64mA
    // Range: 64mA (0000) – 1024mA (1111) (LSB = 64mA)
    // Default: 256mA (0011)
    uint8_t iterm:4;
    // Precharge Current Limit
    // Offset: 64mA
    // Range: 64mA (0000) – 1024mA (1111) (LSB = 64mA)
    // Default: 128mA (0001)
    uint8_t iprechg:4;
} ipre_iterm_reg_t __attribute__(());

typedef struct {
    // Battery Recharge Threshold Offset (below Charge Voltage Limit)
    // 0 – 100mV (VRECHG) below VREG (REG06[7:2]) (default)
    // 1 – 200mV (VRECHG) below VREG (REG06[7:2])
    uint8_t vrechg:1;
    // Battery Precharge to Fast Charge Threshold
    // 0 – 2.8V
    // 1 – 3.0V (default)
    uint8_t batlowv:1;
    // Charge Voltage Limit
    // Offset: 3.840V
    // Range: 3.840V (00000) – 4.608V (110000) (LSB = 16mV)
    // Default: 4.208V (010111)
    // Note: VREG > 110000 (4.608V) is clamped to register value 110000 (4.608V)
    uint8_t vreg:6;
} vreg_reg_t __attribute__(());

typedef struct {
    // JEITA Low Temperature Current Setting
    // 0 – 50% of ICHG (REG04[6:0])
    // 1 – 20% of ICHG (REG04[6:0]) (default)
    uint8_t jeita_iset:1;
    // Fast Charge Timer Setting
    // 00 – 5 hrs
    // 01 – 8 hrs
    // 10 – 12 hrs (default)
    // 11 – 20 hrs
    uint8_t chg_timer:2;
    // Charging Safety Timer Enable
    // 0 – Disable
    // 1 – Enable (default)
    uint8_t en_timer:1;
    // I2C Watchdog Timer Setting
    // 00 – Disable watchdog timer
    // 01 – 40s (default)
    // 10 – 80s
    // 11 – 160s
    uint8_t watchdog:2;
    // STAT Pin Disable
    // 0 – Enable STAT pin function (default)
    // 1 – Disable STAT pin function
    uint8_t stat_dis:1;
    // Charging Termination Enable
    // 0 – Disable
    // 1 – Enable (default)
    uint8_t en_term:1;
} timer_reg_t __attribute__(());

typedef struct {
    // Thermal Regulation Threshold
    // 00 – 60°C
    // 01 – 80°C
    // 10 – 100°C
    // 11 – 120°C (default)
    uint8_t treg:2;
    // IR Compensation Voltage Clamp
    // above VREG (REG06[7:2])
    // Offset: 0mV
    // Range: 0mV (000) - 224mV (111) (LSB = 32mV)
    // Default: 0mV (000)
    uint8_t vclamp:3;
    // IR Compensation Resistor Setting
    // Range: 0mΩ (000) – 140mΩ (111) (LSB = 20mΩ)
    // Default: 0Ω (000) (i.e. Disable IRComp)
    uint8_t bat_comp:3;
} bat_comp_reg_t __attribute__(());

typedef struct {
    // Current pulse control voltage down enable
    // 0 – Disable (default)
    // 1 – Enable
    // Note: This bit is can only be set when EN_PUMPX bit is set and returns to 0 after current pulse control sequence is completed
    uint8_t pumpx_dn:1;
    // Current pulse control voltage up enable
    // 0 – Disable (default)
    // 1 – Enable
    // Note: This bit is can only be set when EN_PUMPX bit is set and returns to 0 after current pulse control sequence is completed
    uint8_t pumpx_up:1;
    // BATFET full system reset enable
    // 0 – Disable BATFET full system reset
    // 1 – Enable BATFET full system reset (default)
    uint8_t batfet_rst_en:1;
    // BATFET turn off delay control
    // 0 – BATFET turn off immediately when BATFET_DIS bit is set (default)
    // 1 – BATFET turn off delay by tSM_DLY when BATFET_DIS bit is set
    uint8_t batfet_dly:1;
    // JEITA High Temperature Voltage Setting
    // 0 – Set Charge Voltage to VREG-200mV during JEITA hig temperature (default)
    // 1 – Set Charge Voltage to VREG during JEITA high temperature
    uint8_t jeita_vset:1;
    // Force BATFET off to enable ship mode
    // 0 – Allow BATFET turn on (default)
    // 1 – Force BATFET off
    uint8_t batfet_dis:1;
    // Safety Timer Setting during DPM or Thermal Regulation
    // 0 – Safety timer not slowed by 2X during input DPM or thermal regulation
    // 1 – Safety timer slowed by 2X during input DPM or thermal regulation (default)
    uint8_t tmr2x_en:1;
    // Force Start Input Current Optimizer (ICO)
    // 0 – Do not force ICO (default)
    // 1 – Force ICO
    // Note: This bit is can only be set only and always returns to 0 after ICO starts
    uint8_t force_ico:1;
} ctrl1_reg_t __attribute__(());

typedef struct {
    // Boost Mode Current Limit
    // 000: 0.5A
    // 001: 0.75A
    // 010: 1.2A
    // 011: 1.4A (default)
    // 100: 1.65A
    // 101: 1.875A
    // 110: 2.15A
    // 111: Reserved
    uint8_t boost_lim:3;
    // PFM mode allowed in boost mode
    // 0 – Allow PFM in boost mode (default)
    // 1 – Disable PFM in boost mode
    uint8_t pfm_otg_dis:1;
    // Boost Mode Voltage Regulation
    // Offset: 4.55V
    // Range: 4.55V (0000) – 5.51V (1111) (LSB = 64mV)
    // Default: 4.998V (0111)
    uint8_t boostv:4;
} boost_ctrl_reg_t __attribute__(());

typedef struct {
    // VSYS Regulation Status
    // 0 – Not in VSYSMIN regulation (BAT > VSYSMIN)
    // 1 – In VSYSMIN regulation (BAT < VSYSMIN)
    uint8_t vsys_stat:1;
    // Reserved - always reads 1
    uint8_t reserved:1;
    // Power Good Status
    // 0 – Not Power Good
    // 1 – Power Good
    uint8_t pg_stat:1;
    // Charging Status
    // 00 – Not Charging
    // 01 – Pre-charge ( < VBATLOWV)
    // 10 – Fast Charging
    // 11 – Charge Termination Done
    uint8_t chrg_stat:2;
    // VBUS Status register
    // 000: No Input
    // 001: USB Host SDP
    // 010: Adapter (3.25A)
    // 111: OTG
    // Note: Software current limit is reported in IINLIM register
    uint8_t vbus_stat:3;
} vbus_stat_reg_t __attribute__(());

typedef struct {
    // NTC Fault Status
    // Buck Mode:
    // 000 – Normal
    // 010 – TS Warm
    // 011 – TS Cool
    // 101 – TS Cold
    // 110 – TS Hot
    // Boost Mode:
    // 000 – Normal
    // 101 – TS Cold
    // 110 – TS Hot
    uint8_t ntc_fault:3;
    // Battery Fault Status
    // 0 – Normal
    // 1 – BATOVP (VBAT > VBATOVP)
    uint8_t bat_fault:1;
    // Charge Fault Status
    // 00 – Normal
    // 01 – Input fault (VBUS > VACOV or VBAT < VBUS < VVBUSMIN(typical 3.8V))
    // 10 - Thermal shutdown
    // 11 – Charge Safety Timer Expiration
    uint8_t chrg_fault:2;
    // Boost Mode Fault Status
    // 0 – Normal
    // 1 – VBUS overloaded in OTG, or VBUS OVP, or battery is too low in boost mode
    uint8_t boost_fault:1;
    // Watchdog Fault Status
    // 0 – Normal
    // 1 - Watchdog timer expiration
    uint8_t watchdog_fault:1;
} fault_reg_t __attribute__(());

typedef struct {
    // Absolute VINDPM Threshold
    // Offset: 2.6V
    // Range: 3.9V (0001101) – 15.3V (1111111) (LSB = 100mV)
    // Default: 4.4V (0010010)
    // Note: Value < 0001101 is clamped to 3.9V (0001101)
    // Register is read only when FORCE_VINDPM=0 and can be written by internal control based on relative VINDPM threshold setting
    // Register can be read/write when FORCE_VINDPM = 1
    // Note: Register is reset to default value when input source is plugged-in
    uint8_t vindpm:7;
    // VINDPM Threshold Setting Method
    // 0 – Run Relative VINDPM Threshold (default)
    // 1 – Run Absolute VINDPM Threshold
    // Note: Register is reset to default value when input source is plugged-in
    uint8_t force_vindpm:1;
} vindpm_reg_t __attribute__(());

typedef struct {
    // ADC conversion of Battery Voltage (VBAT)
    // Offset: 2304mV
    // Range: 2304mV (0000000) – 4848mV (1111111) (LSB = 20mV)
    // Default: 2304mV (0000000)
    uint8_t batv:7;
    // Thermal Regulation Status
    // 0 – Normal
    // 1 – In Thermal Regulation
    uint8_t therm_stat:1;
} batv_reg_t __attribute__(());

typedef struct {
    // ADC conversion of System Voltage (VSYS)
    // Offset: 2304mV
    // Range: 2304mV (0000000) – 4848mV (1111111) (LSB = 20mV)
    // Default: 2304mV (0000000)
    uint8_t sysv:7;
    // Reserved - always reads 0
    uint8_t reserved:1;
} sysv_reg_t __attribute__(());

typedef struct {
    // ADC conversion of TS Voltage (TS) as percentage of REGN
    // Offset: 21%
    // Range 21% (0000000) – 80% (1111111) (LSB = 0.465%)
    // Default: 21% (0000000)
    uint8_t tspct:7;
    // Reserved - always reads 0
    uint8_t reserved:1;
} tspct_reg_t __attribute__(());

typedef struct {
    // ADC conversion of VBUS voltage (VBUS)
    // Offset: 2600mV
    // Range 2600mV (0000000) – 15300mV (1111111) (LSB = 100mV)
    // Default: 2600mV (0000000)
    uint8_t vbusv:7;
    // VBUS Good Status
    // 0 – Not VBUS attached
    // 1 – VBUS Attached
    uint8_t vbus_gd:1;
} vbusv_reg_t __attribute__(());

typedef struct {
    // ADC conversion of Charge Current (IBAT) when VBAT > VBATSHORT
    // Offset: 0mA
    // Range 0mA (0000000) – 6350mA (1111111) (LSB = 50mA)
    // Default: 0mA (0000000)
    // Note: This register returns 0000000 for VBAT < VBATSHORT
    uint8_t ichgr:7;
    // Unused - always reads 0
    uint8_t unused:1;
} ichgr_reg_t __attribute__(());

typedef struct {
    // Input Current Limit in effect while Input Current Optimizer (ICO) is enabled
    // Offset: 100mA
    // Range 100mA (0000000) – 3.25mA (1111111) (LSB = 50mA)
    // Default: 100mA (0000000)
    uint8_t idpm_lim:6;
    // IINDPM Status
    // 0 – Not in IINDPM
    // 1 – IINDPM
    uint8_t idpm_stat:1;
    // VINDPM Status
    // 0 – Not in VINDPM
    // 1 – VINDPM
    uint8_t vdpm_stat:1;
} idpm_lim_reg_t __attribute__(());

typedef struct {
    // Device Revision: 10
    uint8_t dev_rev:2;
    // Temperature Profile
    // 1- JEITA (default)
    uint8_t ts_profile:1;
    // Device Configuration
    // 000: bq25896 
    uint8_t pn:3;
    // Input Current Optimizer (ICO) Status
    // 0 – Optimization is in progress
    // 1 – Maximum Input Current Detected
    uint8_t ico_optimized:1;
    // Register Reset
    // 0 – Keep current register setting (default)
    // 1 – Reset to default register value and reset safety timer
    // Note: Reset to 0 after register reset is completed
    uint8_t reg_rst:1;
} ctrl2_reg_t __attribute__(());

class PMIC_BQ25896 {

    // Arduino's I2C library
    TwoWire *_i2c;

    // I2C address
    bq25896_addr_t _i2c_addr;

    // Reads 16 bytes from a register.
    void _read(bq25896_reg_t reg, uint8_t *val);

    // Writes 16 bytes to a register.
    void _write(bq25896_reg_t reg, uint8_t *val);

public:

    PMIC_BQ25896(bq25896_addr_t addr = BQ25896_ADDR) : _i2c_addr(addr) {};
    // Initializes BQ25896
    void begin(TwoWire *theWire = &Wire);

    // Check if IC is communicating
    bool isConnected();

    // Resets BQ25896
    void reset();

    // REG00
    // Read and return stored values in this register
    ilim_reg_t getILIM_reg();
    // Enable HIZ Mode
    // 0 - Disable (default)
    // 1 - Enable
    void setEN_HIZ(bool value);
    // Enable ILIM Pin
    // 0 – Disable
    // 1 – Enable (default: Enable ILIM pin (1))
    void setEN_ILIM(bool value);
    // Input Current Limit
    // Offset: 100mA 
    // Range: 100mA (000000) – 3250mA (111111) (LSB = 50mA)
    // Default:0001000 (500mA)
    // (Actual input current limit is the lower of I2C or ILIM pin, changes with input type detection)
    bq25896_error_t setIINLIM(int value);
    // Returns Input Current Limit in mA
    uint16_t getIINLIM();

    // REG01
    // Read and return stored values in this register
    vindpm_os_reg_t getVINDPM_OS_reg();
    // Boost Mode Hot Temperature Monitor Threshold
    // 00 – VBHOT1 Threshold (34.75%) (default)
    // 01 – VBHOT0 Threshold (Typ. 37.75%)
    // 10 – VBHOT2 Threshold (Typ. 31.25%)
    // 11 – Disable boost mode thermal protection
    bq25896_error_t setBHOT(int value);
    // Boost Mode Cold Temperature Monitor Threshold
    // 0 – VBCOLD0 Threshold (Typ. 77%) (default)
    // 1 – VBCOLD1 Threshold (Typ. 80%)
    void setBCOLD(bool value);
    // Input Voltage Limit Offset
    // Default: 600mV (00110)
    // Range: 0mV (00000) – 3100mV (11111) (LSB = 100mV)
    // Minimum VINDPM threshold is clamped at 3.9V
    // Maximum VINDPM threshold is clamped at 15.3V
    bq25896_error_t setVINDPM_OS(int value);
    // Returns Input Voltage Limit Offset in mV
    uint16_t getVINDPM_OS();

    // REG02
    // Read and return stored values in this register
    adc_ctrl_reg_t getADC_CTRL_reg();
    // ADC Conversion Start Control
    // 0 – ADC conversion not active (default).
    // 1 – Start ADC Conversion
    // This bit is read-only when CONV_RATE = 1. The bit stays high during ADC conversion and during input source detection.
    void setCONV_START(bool value);
    // ADC Conversion Rate Selection
    // 0 – One shot ADC conversion (default)
    // 1 – Start 1s Continuous Conversion
    void setCONV_RATE(bool value);
    // Boost Mode Frequency Selection
    // 0 – 1.5MHz (default)
    // 1 – 500KHz
    // Note: Write to this bit is ignored when OTG_CONFIG is enabled.
    void setBOOST_FREQ(bool value);
    // Input Current Optimizer (ICO) Enable
    // 0 – Disable ICO Algorithm
    // 1 – Enable ICO Algorithm (default)
    void setICO_EN(bool value);
    // Force Input Detection
    // 0 – Not in PSEL detection (default)
    // 1 – Force PSEL detection
    void setFORCE_DPDM(bool value);
    // Automatic Input Detection Enable
    // 0 – Disable PSEL detection when VBUS is plugged-in
    // 1 – Enable PEL detection when VBUS is plugged-in (default)
    void setAUTO_DPDM_EN(bool value);

    // REG03
    // Read and return stored values in this register
    sys_ctrl_reg_t getSYS_CTRL_reg();
    // Battery Load (IBATLOAD) Enable
    // 0 – Disabled (default)
    // 1 – Enabled
    void setBAT_LOADEN(bool value);
    // I2C Watchdog Timer Reset
    // 0 – Normal (default)
    // 1 – Reset (Back to 0 after timer reset)
    void setWD_RST(bool value);
    // Boost (OTG) Mode Configuration
    // 0 – OTG Disable (default)
    // 1 – OTG Enable
    void setOTG_CONFIG(bool value);
    // Charge Enable Configuration
    // 0 - Charge Disable
    // 1 - Charge Enable (default)
    void setCHG_CONFIG(bool value);
    // Minimum System Voltage Limit
    // Offset: 3000mV
    // Range 3000mV (000) - 3700mV (111) (LSB = 100mV)
    // Default: 3500mV (101)
    bq25896_error_t setSYS_MIN(int value);
    // Returns Minimum System Voltage Limit in mV
    uint16_t getSYS_MIN();
    // Minimum Battery Voltage (falling) to exit boost mode
    // 0 - 2.9V (default)
    // 1 - 2.5V
    void setMIN_VBAT_SEL(bool value);

    // REG04
    // Read and return stored values in this register
    ichg_reg_t getICHG_reg();
    // Current pulse control Enable
    // 0 - Disable Current pulse control (default)
    // 1- Enable Current pulse control (PUMPX_UP and PUMPX_DN)
    void setEN_PUMPX(bool value);
    // Fast Charge Current Limit
    // Offset: 0mA
    // Range: 0mA (0000000) – 3008mA (0101111) (LSB = 64mA)
    // Default: 2048mA (0100000)
    // Note: ICHG=000000 (0mA) disables charge
    // Note: ICHG > 0101111 (3008mA) is clamped to register value 0101111 (3008mA)
    bq25896_error_t setICHG(int value);
    // Returns Fast Charge Current Limit in mA
    uint16_t getICHG();

    // REG05
    // Read and return stored values in this register
    ipre_iterm_reg_t getIPRE_ITERM_reg();
    // Precharge Current Limit
    // Offset: 64mA
    // Range: 64mA (0000) – 1024mA (1111) (LSB = 64mA)
    // Default: 128mA (0001)
    bq25896_error_t setIPRECHG(int value);
    // Returns Precharge Current Limit in mA
    uint16_t getIPRECHG();
    // Termination Current Limit
    // Offset: 64mA
    // Range: 64mA (0000) – 1024mA (1111) (LSB = 64mA)
    // Default: 256mA (0011)
    bq25896_error_t setITERM(int value);
    // Returns Termination Current Limit in mA
    uint16_t getITERM();

    // REG06
    // Read and return stored values in this register
    vreg_reg_t getVREG_reg();
    // Charge Voltage Limit
    // Offset: 3840mV
    // Range: 3840mV (00000) – 4608mV (110000) (LSB = 16mV)
    // Default: 4208mV (010111)
    // Note: VREG > 110000 (4.608V) is clamped to register value 110000 (4.608V)
    bq25896_error_t setVREG(int value);
    // Returns Charge Voltage Limit in mV
    uint16_t getVREG();
    // Battery Precharge to Fast Charge Threshold
    // 0 – 2.8V
    // 1 – 3.0V (default)
    void setBATLOWV(bool value);
    // Battery Recharge Threshold Offset (below Charge Voltage Limit)
    // 0 – 100mV (VRECHG) below VREG (REG06[7:2]) (default)
    // 1 – 200mV (VRECHG) below VREG (REG06[7:2])
    void setVRECHG(bool value);

    // REG07
    // Read and return stored values in this register
    timer_reg_t getTIMER_reg();
    // Charging Termination Enable
    // 0 – Disable
    // 1 – Enable (default)
    void setEN_TERM(bool value);
    // STAT Pin Disable
    // 0 – Enable STAT pin function (default)
    // 1 – Disable STAT pin function
    void setSTAT_DIS(bool value);
    // I2C Watchdog Timer Setting
    // 00 – Disable watchdog timer
    // 01 – 40s (default)
    // 10 – 80s
    // 11 – 160s
    bq25896_error_t setWATCHDOG(int value);
    // Charging Safety Timer Enable
    // 0 – Disable
    // 1 – Enable (default)
    void setEN_TIMER(bool value);
    // Fast Charge Timer Setting
    // 00 – 5 hrs
    // 01 – 8 hrs
    // 10 – 12 hrs (default)
    // 11 – 20 hrs
    bq25896_error_t setCHG_TIMER(int value);
    // JEITA Low Temperature Current Setting
    // 0 – 50% of ICHG (REG04[6:0])
    // 1 – 20% of ICHG (REG04[6:0]) (default)
    void setJEITA_ISET(bool value);

    // REG08
    // Read and return stored values in this register
    bat_comp_reg_t getBAT_COMP_reg();
    // IR Compensation Resistor Setting
    // Range: 0mΩ (000) – 140mΩ (111) (LSB = 20mΩ)
    // Default: 0Ω (000) (i.e. Disable IRComp)
    bq25896_error_t setBAT_COMP(int value);
    // Returns IR Compensation Resistor Setting in mΩ
    uint16_t getBAT_COMP();
    // IR Compensation Voltage Clamp
    // above VREG (REG06[7:2])
    // Offset: 0mV
    // Range: 0mV (000) - 224mV (111) (LSB = 32mV)
    // Default: 0mV (000)
    bq25896_error_t setVCLAMP(int value);
    // Returns IR Compensation Voltage Clamp in mV
    uint16_t getVCLAMP();
    // Thermal Regulation Threshold
    // 00 – 60°C
    // 01 – 80°C
    // 10 – 100°C
    // 11 – 120°C (default)
    bq25896_error_t setTREG(int value);

    // REG09
    // Read and return stored values in this register
    ctrl1_reg_t getCTRL1_reg();
    // Force Start Input Current Optimizer (ICO)
    // 0 – Do not force ICO (default)
    // 1 – Force ICO
    // Note: This bit is can only be set only and always returns to 0 after ICO starts
    void setFORCE_ICO(bool value);
    // Safety Timer Setting during DPM or Thermal Regulation
    // 0 – Safety timer not slowed by 2X during input DPM or thermal regulation
    // 1 – Safety timer slowed by 2X during input DPM or thermal regulation (default)
    void setTMR2X_EN(bool value);
    // Force BATFET off to enable ship mode
    // 0 – Allow BATFET turn on (default)
    // 1 – Force BATFET off
    void setBATFET_DIS(bool value);
    // JEITA High Temperature Voltage Setting
    // 0 – Set Charge Voltage to VREG-200mV during JEITA hig temperature (default)
    // 1 – Set Charge Voltage to VREG during JEITA high temperature
    void setJEITA_VSET(bool value);
    // BATFET turn off delay control
    // 0 – BATFET turn off immediately when BATFET_DIS bit is set (default)
    // 1 – BATFET turn off delay by tSM_DLY when BATFET_DIS bit is set
    void setBATFET_DLY(bool value);
    // BATFET full system reset enable
    // 0 – Disable BATFET full system reset
    // 1 – Enable BATFET full system reset (default)
    void setBATFET_RST_EN(bool value);
    // Current pulse control voltage up enable
    // 0 – Disable (default)
    // 1 – Enable
    // Note: This bit is can only be set when EN_PUMPX bit is set and returns to 0 after current pulse control sequence is completed
    void setPUMPX_UP(bool value);
    // Current pulse control voltage down enable
    // 0 – Disable (default)
    // 1 – Enable
    // Note: This bit is can only be set when EN_PUMPX bit is set and returns to 0 after current pulse control sequence is completed
    void setPUMPX_DN(bool value);

    // REG0A
    // Read and return stored values in this register
    boost_ctrl_reg_t getBOOST_CTRL_reg();
    // Boost Mode Voltage Regulation
    // Offset: 4550mV
    // Range: 4550mV (0000) – 5510mV (1111) (LSB = 64mV)
    // Default: 4998mV (0111)
    bq25896_error_t setBOOSTV(int value);
    // Returns Boost Mode Voltage Regulation in mV
    uint16_t getBOOSTV();
    // PFM mode allowed in boost mode
    // 0 – Allow PFM in boost mode (default)
    // 1 – Disable PFM in boost mode
    void setPFM_OTG_DIS(bool value);
    // Boost Mode Current Limit
    // 000: 500mA
    // 001: 750mA
    // 010: 1200mA
    // 011: 1400mA (default)
    // 100: 1650mA
    // 101: 1875mA
    // 110: 2150mA
    // 111: Reserved
    bq25896_error_t setBOOST_LIM(int value);
    // Returns Boost Mode Current Limit in mA
    uint16_t getBOOST_LIM();

    // REG0B
    // Read and return stored values in this register
    vbus_stat_reg_t get_VBUS_STAT_reg();

    // REG0C
    // Read and return stored values in this register
    fault_reg_t getFAULT_reg();

    // REG0D
    // Read and return stored values in this register
    vindpm_reg_t getVINDPM_reg();
    // VINDPM Threshold Setting Method
    // 0 – Run Relative VINDPM Threshold (default)
    // 1 – Run Absolute VINDPM Threshold
    // Note: Register is reset to default value when input source is plugged-in
    void setFORCE_VINDPM(bool value);
    // Absolute VINDPM Threshold
    // Offset: 2600mV
    // Range: 3900mV (0001101) – 15300mV (1111111) (LSB = 100mV)
    // Default: 4400mV (0010010)
    // Note: Value < 0001101 is clamped to 3900mV (0001101)
    // Register is read only when FORCE_VINDPM=0 and can be written by internal control based on relative VINDPM threshold setting
    // Register can be read/write when FORCE_VINDPM = 1
    // Note: Register is reset to default value when input source is plugged-in
    bq25896_error_t setVINDPM(int value);
    // Returns Absolute VINDPM Threshold in mV
    uint16_t getVINDPM();

    // REG0E
    // Read and return stored values in this register
    batv_reg_t getBATV_reg();
    // Returns Battery Voltage (VBAT) in mV
    uint16_t getBATV();

    // REG0F
    // Read and return stored values in this register
    sysv_reg_t getSYSV_reg();
    // Returns System Voltage (VSYS) in mV
    uint16_t getSYSV();

    // REG10
    // Read and return stored values in this register
    tspct_reg_t getTSPCT_reg();
    // Returns TS Voltage (TS) as percentage of REGN in %
    uint16_t getTSPCT();

    // REG11
    // Read and return stored values in this register
    vbusv_reg_t getVBUSV_reg();
    // Returns VBUS voltage (VBUS) in mV
    uint16_t getVBUSV();

    // REG12
    // Read and return stored values in this register
    ichgr_reg_t getICHGR_reg();
    // Returns Charge Current (IBAT) in mA
    uint16_t getICHGR();

    // REG13
    // Read and return stored values in this register
    idpm_lim_reg_t getIDPM_LIM_reg();

    // REG14
    // Read and return stored values in this register
    ctrl2_reg_t getCTRL2_reg();
    // Register Reset
    // 0 – Keep current register setting (default)
    // 1 – Reset to default register value and reset safety timer
    // Note: Reset to 0 after register reset is completed
    void setREG_RST(bool value);


};

#endif