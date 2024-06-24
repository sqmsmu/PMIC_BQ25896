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
    FAULT_      = 0x0C,
    VINDPM      = 0x0D,
    BATV        = 0x0E,
    SYSV        = 0x0F,
    TSPCT       = 0x10,
    VBUSV       = 0x11,
    ICHGR       = 0x12,
    IDPM_LIM    = 0x13,
    CTRL2       = 0x14
} bq25896_reg_t;

class PMIC_BQ25896 {

    typedef struct {
        uint16_t en_hiz:1;
        uint16_t en_ilim:1;
        uint16_t iinlim:6;
    } ilim_reg_t __attribute__(());//

    typedef struct {
        uint16_t bhot:2;
        uint16_t bcold:1;
        uint16_t vindpm_os:5;
    } vindpm_os_reg_t __attribute__(());//

    typedef struct {
        uint16_t conv_start:1;
        uint16_t conv_rate:1;
        uint16_t boost_freq:1;
        uint16_t ico_en:1;
        uint16_t force_dpdm:1;
        uint16_t auto_dpdm_en:1;
    } adc_ctrl_reg_t __attribute__(());

    typedef struct {
        uint16_t bat_loaden:1;
        uint16_t wd_rst:1;
        uint16_t otg_config:1;
        uint16_t chg_config:1;
        uint16_t sys_min:3;
        uint16_t min_vbat_sel:1;
    } sys_ctrl_reg_t __attribute__(());

    typedef struct {
        uint16_t en_pumpx:1;
        uint16_t ichg:7;
    } ichg_reg_t __attribute__(());

    typedef struct {
        uint16_t iprechg:4;
        uint16_t iterm:4;
    } ipre_iterm_reg_t __attribute__(());

    typedef struct {
        uint16_t vreg:6;
        uint16_t batlowv:1;
        uint16_t vrechg:1;
    } vreg_reg_t __attribute__(());

    typedef struct {
        uint16_t en_term:1;
        uint16_t stat_dis:1;
        uint16_t watchdog:2;
        uint16_t en_timer:1;
        uint16_t chg_timer:2;
        uint16_t jeita_iset:1;
    } timer_reg_t __attribute__(());

    typedef struct {
        uint16_t bat_comp:3;
        uint16_t vclamp:3;
        uint16_t treg:2;
    } bat_comp_reg_t __attribute__(());

    typedef struct {
        uint16_t force_ico:1;
        uint16_t tmr2x_en:1;
        uint16_t batfet_dis:1;
        uint16_t jeita_vset:1;
        uint16_t batfet_dly:1;
        uint16_t batfet_rst_en:1;
        uint16_t pumpx_up:1;
        uint16_t pumpx_dn:1;
    } ctrl1_reg_t __attribute__(());

    typedef struct {
        uint16_t boostv:4;
        uint16_t pfm_otg_dis:1;
        uint16_t boost_lim:3;
    } boost_ctrl_reg_t __attribute__(());

    typedef struct {
        uint16_t vbus_stat:3;
        uint16_t chrg_stat:2;
        uint16_t pg_stat:1;
        uint16_t vsys_stat:1;
    } vbus_stat_reg_t __attribute__(());

    typedef struct {
        uint16_t watchdog_fault:1;
        uint16_t boost_fault:1;
        uint16_t chrg_fault:2;
        uint16_t bat_fault:1;
        uint16_t ntc_fault:3;
    } fault_reg_t __attribute__(());

    typedef struct {
        uint16_t force_vindpm:1;
        uint16_t vindpm:7;
    } vindpm_reg_t __attribute__(());

    typedef struct {
        uint16_t therm_stat:1;
        uint16_t batv:7;
    } batv_reg_t __attribute__(());

    typedef struct {
        uint16_t sysv:7;
    } sysv_reg_t __attribute__(());

    typedef struct {
        uint16_t tspct:7;
    } tspct_reg_t __attribute__(());

    typedef struct {
        uint16_t vbus_gd:1;
        uint16_t vbusv:7;
    } vbusv_reg_t __attribute__(());

    typedef struct {
        uint16_t ichgr:7;
    } ichgr_reg_t __attribute__(());

    typedef struct {
        uint16_t vdpm_stat:1;
        uint16_t idpm_stat:1;
        uint16_t idpm_lim:6;
    } idpm_lim_reg_t __attribute__(());

    typedef struct {
        uint16_t reg_rst:1;
        uint16_t ico_optimized:1;
        uint16_t pn:3;
        uint16_t ts_profile:1;
        uint16_t dev_rev:2;
    } ctrl2_reg_t __attribute__(());

    // Arduino's I2C library
    TwoWire *_i2c;

    // I2C address
    bq25896_addr_t _i2c_addr;

    // Value of all registers.
    ilim_reg_t _ilim_reg;
    vindpm_os_reg_t _vindpm_os_reg;
    adc_ctrl_reg_t _adc_ctrl_reg;
    sys_ctrl_reg_t _sys_ctrl_reg;
    ichg_reg_t _ichg_reg;
    ipre_iterm_reg_t _ipre_iterm_reg;
    vreg_reg_t _vreg_reg;
    timer_reg_t _timer_reg;
    bat_comp_reg_t _bat_comp_reg;
    ctrl1_reg_t _ctrl1_reg;
    boost_ctrl_reg_t _boost_ctrl_reg;
    vbus_stat_reg_t _vbus_stat_reg;
    fault_reg_t _fault_reg;
    vindpm_reg_t _vindpm_reg;
    batv_reg_t _batv_reg;
    sysv_reg_t _sysv_reg;
    tspct_reg_t _tspct_reg;
    vbusv_reg_t _vbusv_reg;
    ichgr_reg_t _ichgr_reg;
    idpm_lim_reg_t _idpm_lim_reg;
    ctrl2_reg_t _ctrl2_reg;

    // Reads 16 bytes from a register.
    void _read(bq25896_reg_t reg, uint16_t *val);

    // Writes 16 bytes to a register.
    void _write(bq25896_reg_t reg, uint16_t *val);

public:

    PMIC_BQ25896(bq25896_addr_t addr) : _i2c_addr(addr) {};
    // Initializes BQ25896
    void begin(TwoWire *theWire = &Wire);

    // Resets BQ25896
    void reset();

    // Enable HIZ Mode
    // 0 - Disable (default)
    // 1 - Enable
    void setEN_HIZ(bool value);

    // Enable ILIM Pin
    // 0 - Disable 
    // 1 - Enable (default)
    void setEN_ILIM(bool value);

    // Input Current Limit
    // Offset: 100mA 
    // Range: 100mA (000000) – 3.25A (111111) 
    // Default:0001000 (500mA)
    void setIINLIM(uint16_t value);

    ilim_reg_t getILIM_reg();
    vindpm_os_reg_t getVINDPM_OS_reg();
    adc_ctrl_reg_t getADC_CTRL_reg();
    sys_ctrl_reg_t getSYS_CTRL_reg();
    ichg_reg_t getICHG_reg();
    ipre_iterm_reg_t getIPRE_ITERM_reg();
    vreg_reg_t getVREG_reg();
    timer_reg_t getTIMER_reg();
    bat_comp_reg_t getBAT_COMP_reg();
    ctrl1_reg_t getCTRL1_reg();
    boost_ctrl_reg_t getBOOST_CTRL_reg();
    vbus_stat_reg_t get_VBUS_STAT_reg();
    fault_reg_t getFAULT_reg();
    vindpm_reg_t getVINDPM_reg();
    batv_reg_t getBATV_reg();
    sysv_reg_t getSYSV_reg();
    tspct_reg_t getTSPCT_reg();
    vbusv_reg_t getVBUSV_reg();
    ichgr_reg_t getICHGR_reg();
    idpm_lim_reg_t getIDPM_LIM_reg();
    ctrl2_reg_t getCTRL2_reg();

};

#endif