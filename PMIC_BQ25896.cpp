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

#include "PMIC_BQ25896.h"

void PMIC_BQ25896::_read(bq25896_reg_t reg, uint8_t *val) {
    _i2c->beginTransmission(_i2c_addr);
    _i2c->write(reg);
    _i2c->endTransmission();

    _i2c->requestFrom(_i2c_addr, 1);

    if (_i2c->available())
    {
      *val = _i2c->read();
    }
}

void PMIC_BQ25896::_write(bq25896_reg_t reg, uint8_t *val) {
    _i2c->beginTransmission(_i2c_addr);
    _i2c->write(reg);
    _i2c->write(*val);
    _i2c->endTransmission();
}

void PMIC_BQ25896::begin(TwoWire *theWire){
    _i2c = theWire;
    _i2c->begin();
}

bool PMIC_BQ25896::isConnected(){
    _i2c->beginTransmission(_i2c_addr);
    uint8_t error = _i2c->endTransmission();
    if(error == 0) return true;
    else return false;
}

void PMIC_BQ25896::reset(){
    PMIC_BQ25896::setREG_RST(1);
}

// REG00
ilim_reg_t PMIC_BQ25896::getILIM_reg(){
    ilim_reg_t temp_reg;
    _read(ILIM, (uint8_t*)&temp_reg);
    return temp_reg;
}

void PMIC_BQ25896::setEN_HIZ(bool value){
    ilim_reg_t temp_reg;
    _read(ILIM, (uint8_t*)&temp_reg);
    temp_reg.en_hiz = value;
    _write(ILIM, (uint8_t*)&temp_reg);
}
void PMIC_BQ25896::setEN_ILIM(bool value){
    ilim_reg_t temp_reg;
    _read(ILIM, (uint8_t*)&temp_reg);
    temp_reg.en_ilim = value;
    _write(ILIM, (uint8_t*)&temp_reg);
}
bq25896_error_t PMIC_BQ25896::setIINLIM(int value){
    ilim_reg_t temp_reg;
    if(value < 100 && value > 3250){
        return BQ_RANGE_ERR;
    }
    uint16_t data = value - 100;
    data = (float)data/50.0;
    _read(ILIM, (uint8_t*)&temp_reg);
    temp_reg.iinlim = data;
    _write(ILIM, (uint8_t*)&temp_reg);
    return BQ_OK;    
}
uint16_t PMIC_BQ25896::getIINLIM(){
    ilim_reg_t temp_reg = PMIC_BQ25896::getILIM_reg();
    uint16_t data = 100 + (temp_reg.iinlim * 50);
    return data;
}

// REG01
vindpm_os_reg_t PMIC_BQ25896::getVINDPM_OS_reg(){
    vindpm_os_reg_t temp_reg;
    _read(VINDPM_OS, (uint8_t*)&temp_reg);
    return temp_reg;
}
bq25896_error_t PMIC_BQ25896::setBHOT(int value){
    vindpm_os_reg_t temp_reg;
    if(value < 0 && value > 3){
        return BQ_RANGE_ERR;
    }
    _read(VINDPM_OS, (uint8_t*)&temp_reg);
    temp_reg.bhot = value;
    _write(VINDPM_OS, (uint8_t*)&temp_reg);
    return BQ_OK; 
}
void PMIC_BQ25896::setBCOLD(bool value){
    vindpm_os_reg_t temp_reg;
    _read(VINDPM_OS, (uint8_t*)&temp_reg);
    temp_reg.bcold = value;
    _write(VINDPM_OS, (uint8_t*)&temp_reg);
}
bq25896_error_t PMIC_BQ25896::setVINDPM_OS(int value){
    vindpm_os_reg_t temp_reg;
    if(value < 0 && value > 3100){
        return BQ_RANGE_ERR;
    }
    uint16_t data = (float)value / 100.0;
    _read(VINDPM_OS, (uint8_t*)&temp_reg);
    temp_reg.vindpm_os = data;
    _write(VINDPM_OS, (uint8_t*)&temp_reg);
    return BQ_OK; 
}
uint16_t PMIC_BQ25896::getVINDPM_OS(){
    vindpm_os_reg_t temp_reg = PMIC_BQ25896::getVINDPM_OS_reg();
    uint16_t data = temp_reg.vindpm_os * 100;
    return data;
}

// REG02
adc_ctrl_reg_t PMIC_BQ25896::getADC_CTRL_reg(){
    adc_ctrl_reg_t temp_reg;
    _read(ADC_CTRL, (uint8_t*)&temp_reg);
    return temp_reg;
}
void PMIC_BQ25896::setCONV_START(bool value){
    adc_ctrl_reg_t temp_reg;
    _read(ADC_CTRL, (uint8_t*)&temp_reg);
    temp_reg.conv_start = value;
    _write(ADC_CTRL, (uint8_t*)&temp_reg);
}
void PMIC_BQ25896::setCONV_RATE(bool value){
    adc_ctrl_reg_t temp_reg;
    _read(ADC_CTRL, (uint8_t*)&temp_reg);
    temp_reg.conv_rate = value;
    _write(ADC_CTRL, (uint8_t*)&temp_reg);
}
void PMIC_BQ25896::setBOOST_FREQ(bool value){
    adc_ctrl_reg_t temp_reg;
    _read(ADC_CTRL, (uint8_t*)&temp_reg);
    temp_reg.boost_freq = value;
    _write(ADC_CTRL, (uint8_t*)&temp_reg);
}
void PMIC_BQ25896::setICO_EN(bool value){
    adc_ctrl_reg_t temp_reg;
    _read(ADC_CTRL, (uint8_t*)&temp_reg);
    temp_reg.ico_en = value;
    _write(ADC_CTRL, (uint8_t*)&temp_reg);
}
void PMIC_BQ25896::setFORCE_DPDM(bool value){
    adc_ctrl_reg_t temp_reg;
    _read(ADC_CTRL, (uint8_t*)&temp_reg);
    temp_reg.force_dpdm = value;
    _write(ADC_CTRL, (uint8_t*)&temp_reg);
}
void PMIC_BQ25896::setAUTO_DPDM_EN(bool value){
    adc_ctrl_reg_t temp_reg;
    _read(ADC_CTRL, (uint8_t*)&temp_reg);
    temp_reg.auto_dpdm_en = value;
    _write(ADC_CTRL, (uint8_t*)&temp_reg);
}

// REG03
sys_ctrl_reg_t PMIC_BQ25896::getSYS_CTRL_reg(){
    sys_ctrl_reg_t temp_reg;
    _read(SYS_CTRL, (uint8_t*)&temp_reg);
    return temp_reg;
}
void PMIC_BQ25896::setBAT_LOADEN(bool value){
    sys_ctrl_reg_t temp_reg;
    _read(SYS_CTRL, (uint8_t*)&temp_reg);
    temp_reg.bat_loaden = value;
    _write(SYS_CTRL, (uint8_t*)&temp_reg);
}
void PMIC_BQ25896::setWD_RST(bool value){
    sys_ctrl_reg_t temp_reg;
    _read(SYS_CTRL, (uint8_t*)&temp_reg);
    temp_reg.wd_rst = value;
    _write(SYS_CTRL, (uint8_t*)&temp_reg);
}
void PMIC_BQ25896::setOTG_CONFIG(bool value){
    sys_ctrl_reg_t temp_reg;
    _read(SYS_CTRL, (uint8_t*)&temp_reg);
    temp_reg.otg_config = value;
    _write(SYS_CTRL, (uint8_t*)&temp_reg);
}
void PMIC_BQ25896::setCHG_CONFIG(bool value){
    sys_ctrl_reg_t temp_reg;
    _read(SYS_CTRL, (uint8_t*)&temp_reg);
    temp_reg.chg_config = value;
    _write(SYS_CTRL, (uint8_t*)&temp_reg);
}
bq25896_error_t PMIC_BQ25896::setSYS_MIN(int value){
    sys_ctrl_reg_t temp_reg;
    if(value < 3000 && value > 3700){
        return BQ_RANGE_ERR;
    }
    uint16_t data = value - 3000;
    data = (float)data/100.0;
    _read(SYS_CTRL, (uint8_t*)&temp_reg);
    temp_reg.sys_min = data;
    _write(SYS_CTRL, (uint8_t*)&temp_reg);
    return BQ_OK;
}
uint16_t PMIC_BQ25896::getSYS_MIN(){
    sys_ctrl_reg_t temp_reg = PMIC_BQ25896::getSYS_CTRL_reg();
    uint16_t data = 3000 + (temp_reg.sys_min * 100);
    return data;
}
void PMIC_BQ25896::setMIN_VBAT_SEL(bool value){
    sys_ctrl_reg_t temp_reg;
    _read(SYS_CTRL, (uint8_t*)&temp_reg);
    temp_reg.min_vbat_sel = value;
    _write(SYS_CTRL, (uint8_t*)&temp_reg);
}

// REG04
ichg_reg_t PMIC_BQ25896::getICHG_reg(){
    ichg_reg_t temp_reg;
    _read(ICHG, (uint8_t*)&temp_reg);
    return temp_reg;
}
void PMIC_BQ25896::setEN_PUMPX(bool value){
    ichg_reg_t temp_reg;
    _read(ICHG, (uint8_t*)&temp_reg);
    temp_reg.en_pumpx = value;
    _write(ICHG, (uint8_t*)&temp_reg);
}
bq25896_error_t PMIC_BQ25896::setICHG(int value){
    ichg_reg_t temp_reg;
    if(value < 0 && value > 3008){
        return BQ_RANGE_ERR;
    }
    uint16_t data = (float)value / 64.0;
    _read(ICHG, (uint8_t*)&temp_reg);
    temp_reg.ichg = data;
    _write(ICHG, (uint8_t*)&temp_reg);
    return BQ_OK;
}
uint16_t PMIC_BQ25896::getICHG(){
    ichg_reg_t temp_reg = PMIC_BQ25896::getICHG_reg();
    uint16_t data = temp_reg.ichg * 64;
    return data;
}

// REG05
ipre_iterm_reg_t PMIC_BQ25896::getIPRE_ITERM_reg(){
    ipre_iterm_reg_t temp_reg;
    _read(IPRE_ITERM, (uint8_t*)&temp_reg);
    return temp_reg;
}
bq25896_error_t PMIC_BQ25896::setIPRECHG(int value){
    ipre_iterm_reg_t temp_reg;
    if(value < 64 && value > 1024){
        return BQ_RANGE_ERR;
    }
    uint16_t data = value - 64.0;
    data = (float)data / 64.0;
    _read(IPRE_ITERM, (uint8_t*)&temp_reg);
    temp_reg.iprechg = data;
    _write(IPRE_ITERM, (uint8_t*)&temp_reg);
    return BQ_OK;
}
uint16_t PMIC_BQ25896::getIPRECHG(){
    ipre_iterm_reg_t temp_reg = PMIC_BQ25896::getIPRE_ITERM_reg();
    uint16_t data = 64 + (temp_reg.iprechg * 64);
    return data;
}
bq25896_error_t PMIC_BQ25896::setITERM(int value){
    ipre_iterm_reg_t temp_reg;
    if(value < 64 && value > 1024){
        return BQ_RANGE_ERR;
    }
    uint16_t data = value - 64.0;
    data = (float)data / 64.0;
    _read(IPRE_ITERM, (uint8_t*)&temp_reg);
    temp_reg.iterm = data;
    _write(IPRE_ITERM, (uint8_t*)&temp_reg);
    return BQ_OK;
}
uint16_t PMIC_BQ25896::getITERM(){
    ipre_iterm_reg_t temp_reg = PMIC_BQ25896::getIPRE_ITERM_reg();
    uint16_t data = 64 + (temp_reg.iterm * 64);
    return data;
}

// REG06
vreg_reg_t PMIC_BQ25896::getVREG_reg(){
    vreg_reg_t temp_reg;
    _read(VREG, (uint8_t*)&temp_reg);
    return temp_reg;
}
bq25896_error_t PMIC_BQ25896::setVREG(int value){
    vreg_reg_t temp_reg;
    if(value < 3840 && value > 4608){
        return BQ_RANGE_ERR;
    }
    uint16_t data = value - 3840.0;
    data = (float)data / 16.0;
    _read(VREG, (uint8_t*)&temp_reg);
    temp_reg.vreg = data;
    _write(VREG, (uint8_t*)&temp_reg);
    return BQ_OK;
}
uint16_t PMIC_BQ25896::getVREG(){
    vreg_reg_t temp_reg = PMIC_BQ25896::getVREG_reg();
    uint16_t data = 3840 + (temp_reg.vreg * 16);
    return data;
}
void PMIC_BQ25896::setBATLOWV(bool value){
    vreg_reg_t temp_reg;
    _read(VREG, (uint8_t*)&temp_reg);
    temp_reg.batlowv = value;
    _write(VREG, (uint8_t*)&temp_reg);
}
void PMIC_BQ25896::setVRECHG(bool value){
    vreg_reg_t temp_reg;
    _read(VREG, (uint8_t*)&temp_reg);
    temp_reg.vrechg = value;
    _write(VREG, (uint8_t*)&temp_reg);
}

// REG07
timer_reg_t PMIC_BQ25896::getTIMER_reg(){
    timer_reg_t temp_reg;
    _read(TIMER, (uint8_t*)&temp_reg);
    return temp_reg;
}
void PMIC_BQ25896::setEN_TERM(bool value){
    timer_reg_t temp_reg;
    _read(TIMER, (uint8_t*)&temp_reg);
    temp_reg.en_term = value;
    _write(TIMER, (uint8_t*)&temp_reg);
}
void PMIC_BQ25896::setSTAT_DIS(bool value){
    timer_reg_t temp_reg;
    _read(TIMER, (uint8_t*)&temp_reg);
    temp_reg.stat_dis = value;
    _write(TIMER, (uint8_t*)&temp_reg);
}
bq25896_error_t PMIC_BQ25896::setWATCHDOG(int value){
    timer_reg_t temp_reg;
    if(value < 0 && value > 3){
        return BQ_RANGE_ERR;
    }
    _read(TIMER, (uint8_t*)&temp_reg);
    temp_reg.watchdog = value;
    _write(TIMER, (uint8_t*)&temp_reg);
    return BQ_OK;
}
void PMIC_BQ25896::setEN_TIMER(bool value){
    timer_reg_t temp_reg;
    _read(TIMER, (uint8_t*)&temp_reg);
    temp_reg.en_timer = value;
    _write(TIMER, (uint8_t*)&temp_reg);
}
bq25896_error_t PMIC_BQ25896::setCHG_TIMER(int value){
    timer_reg_t temp_reg;
    if(value < 0 && value > 3){
        return BQ_RANGE_ERR;
    }
    _read(TIMER, (uint8_t*)&temp_reg);
    temp_reg.chg_timer = value;
    _write(TIMER, (uint8_t*)&temp_reg);
    return BQ_OK;
}
void PMIC_BQ25896::setJEITA_ISET(bool value){
    timer_reg_t temp_reg;
    _read(TIMER, (uint8_t*)&temp_reg);
    temp_reg.jeita_iset = value;
    _write(TIMER, (uint8_t*)&temp_reg);
}

// REG08
bat_comp_reg_t PMIC_BQ25896::getBAT_COMP_reg(){
    bat_comp_reg_t temp_reg;
    _read(BAT_COMP, (uint8_t*)&temp_reg);
    return temp_reg;
}
bq25896_error_t PMIC_BQ25896::setBAT_COMP(int value){
    bat_comp_reg_t temp_reg;
    if(value < 0 && value > 140){
        return BQ_RANGE_ERR;
    }
    uint16_t data = (float)value / 20.0;
    _read(BAT_COMP, (uint8_t*)&temp_reg);
    temp_reg.bat_comp = data;
    _write(BAT_COMP, (uint8_t*)&temp_reg);
    return BQ_OK;
}
uint16_t PMIC_BQ25896::getBAT_COMP(){
    bat_comp_reg_t temp_reg = PMIC_BQ25896::getBAT_COMP_reg();
    uint16_t data = temp_reg.bat_comp * 20;
    return data;
}
bq25896_error_t PMIC_BQ25896::setVCLAMP(int value){
    bat_comp_reg_t temp_reg;
    if(value < 0 && value > 224){
        return BQ_RANGE_ERR;
    }
    uint16_t data = (float)value / 32.0;
    _read(BAT_COMP, (uint8_t*)&temp_reg);
    temp_reg.vclamp = data;
    _write(BAT_COMP, (uint8_t*)&temp_reg);
    return BQ_OK;
}
uint16_t PMIC_BQ25896::getVCLAMP(){
    bat_comp_reg_t temp_reg = PMIC_BQ25896::getBAT_COMP_reg();
    uint16_t data = temp_reg.vclamp * 32;
    return data;
}
bq25896_error_t PMIC_BQ25896::setTREG(int value){
    bat_comp_reg_t temp_reg;
    if(value < 0 && value > 3){
        return BQ_RANGE_ERR;
    }
    _read(BAT_COMP, (uint8_t*)&temp_reg);
    temp_reg.treg = value;
    _write(BAT_COMP, (uint8_t*)&temp_reg);
    return BQ_OK;
}

// REG09
ctrl1_reg_t PMIC_BQ25896::getCTRL1_reg(){
    ctrl1_reg_t temp_reg;
    _read(CTRL1, (uint8_t*)&temp_reg);
    return temp_reg;
}
void PMIC_BQ25896::setFORCE_ICO(bool value){
    ctrl1_reg_t temp_reg;
    _read(CTRL1, (uint8_t*)&temp_reg);
    temp_reg.force_ico = value;
    _write(CTRL1, (uint8_t*)&temp_reg);
}
void PMIC_BQ25896::setTMR2X_EN(bool value){
    ctrl1_reg_t temp_reg;
    _read(CTRL1, (uint8_t*)&temp_reg);
    temp_reg.tmr2x_en = value;
    _write(CTRL1, (uint8_t*)&temp_reg);
}
void PMIC_BQ25896::setBATFET_DIS(bool value){
    ctrl1_reg_t temp_reg;
    _read(CTRL1, (uint8_t*)&temp_reg);
    temp_reg.batfet_dis = value;
    _write(CTRL1, (uint8_t*)&temp_reg);
}
void PMIC_BQ25896::setJEITA_VSET(bool value){
    ctrl1_reg_t temp_reg;
    _read(CTRL1, (uint8_t*)&temp_reg);
    temp_reg.jeita_vset = value;
    _write(CTRL1, (uint8_t*)&temp_reg);
}
void PMIC_BQ25896::setBATFET_DLY(bool value){
    ctrl1_reg_t temp_reg;
    _read(CTRL1, (uint8_t*)&temp_reg);
    temp_reg.batfet_dly = value;
    _write(CTRL1, (uint8_t*)&temp_reg);
}
void PMIC_BQ25896::setBATFET_RST_EN(bool value){
    ctrl1_reg_t temp_reg;
    _read(CTRL1, (uint8_t*)&temp_reg);
    temp_reg.batfet_rst_en = value;
    _write(CTRL1, (uint8_t*)&temp_reg);
}
void PMIC_BQ25896::setPUMPX_UP(bool value){
    ctrl1_reg_t temp_reg;
    _read(CTRL1, (uint8_t*)&temp_reg);
    temp_reg.pumpx_up = value;
    _write(CTRL1, (uint8_t*)&temp_reg);
}
void PMIC_BQ25896::setPUMPX_DN(bool value){
    ctrl1_reg_t temp_reg;
    _read(CTRL1, (uint8_t*)&temp_reg);
    temp_reg.pumpx_dn = value;
    _write(CTRL1, (uint8_t*)&temp_reg);
}

// REG0A
boost_ctrl_reg_t PMIC_BQ25896::getBOOST_CTRL_reg(){
    boost_ctrl_reg_t temp_reg;
    _read(BOOST_CTRL, (uint8_t*)&temp_reg);
    return temp_reg;
}
bq25896_error_t PMIC_BQ25896::setBOOSTV(int value){
    boost_ctrl_reg_t temp_reg;
    if(value < 4550 && value > 5510){
        return BQ_RANGE_ERR;
    }
    uint16_t data = value - 4550.0;
    data = (float)data / 64.0;
    _read(BOOST_CTRL, (uint8_t*)&temp_reg);
    temp_reg.boostv = data;
    _write(BOOST_CTRL, (uint8_t*)&temp_reg);
    return BQ_OK;
}
uint16_t PMIC_BQ25896::getBOOSTV(){
    boost_ctrl_reg_t temp_reg = PMIC_BQ25896::getBOOST_CTRL_reg();
    uint16_t data = 4550 + (temp_reg.boost_lim * 64);
    return data;
}
void PMIC_BQ25896::setPFM_OTG_DIS(bool value){
    boost_ctrl_reg_t temp_reg;
    _read(BOOST_CTRL, (uint8_t*)&temp_reg);
    temp_reg.pfm_otg_dis = value;
    _write(BOOST_CTRL, (uint8_t*)&temp_reg);
}
bq25896_error_t PMIC_BQ25896::setBOOST_LIM(int value){
    boost_ctrl_reg_t temp_reg;
    if(value < 0 && value > 6){
        return BQ_RANGE_ERR;
    }
    _read(BOOST_CTRL, (uint8_t*)&temp_reg);
    temp_reg.boost_lim = value;
    _write(BOOST_CTRL, (uint8_t*)&temp_reg);
    return BQ_OK;
}
uint16_t PMIC_BQ25896::getBOOST_LIM(){
    boost_ctrl_reg_t temp_reg = PMIC_BQ25896::getBOOST_CTRL_reg();
    uint16_t data = temp_reg.boost_lim;
    switch(data){
        case 0:
        return 500;
        break;
        case 1:
        return 750;
        break;
        case 2:
        return 1200;
        break;
        case 3:
        return 1400;
        break;
        case 4:
        return 1650;
        break;
        case 5:
        return 1875;
        break;
        case 6:
        return 2150;
        break;
        default:
        return 0;
    }
}

// REG0B
vbus_stat_reg_t PMIC_BQ25896::get_VBUS_STAT_reg(){
    vbus_stat_reg_t temp_reg;
    _read(VBUS_STAT, (uint8_t*)&temp_reg);
    return temp_reg;
}

// REG0C
fault_reg_t PMIC_BQ25896::getFAULT_reg(){
    fault_reg_t temp_reg;
    _read(FAULT, (uint8_t*)&temp_reg);
    return temp_reg;
}

// REG0D
vindpm_reg_t PMIC_BQ25896::getVINDPM_reg(){
    vindpm_reg_t temp_reg;
    _read(VINDPM, (uint8_t*)&temp_reg);
    return temp_reg;
}
void PMIC_BQ25896::setFORCE_VINDPM(bool value){
    vindpm_reg_t temp_reg;
    _read(VINDPM, (uint8_t*)&temp_reg);
    temp_reg.force_vindpm = value;
    _write(VINDPM, (uint8_t*)&temp_reg);
}
bq25896_error_t PMIC_BQ25896::setVINDPM(int value){
    vindpm_reg_t temp_reg;
    if(value < 3900 && value > 15300){
        return BQ_RANGE_ERR;
    }
    uint16_t data = value - 2600.0;
    data = (float)data / 100.0;
    _read(VINDPM, (uint8_t*)&temp_reg);
    temp_reg.vindpm = value;
    _write(VINDPM, (uint8_t*)&temp_reg);
    return BQ_OK;
}
uint16_t PMIC_BQ25896::getVINDPM(){
    vindpm_reg_t temp_reg = PMIC_BQ25896::getVINDPM_reg();
    uint16_t data = 2600 + (temp_reg.vindpm * 100);
    return data;
}

// REG0E
batv_reg_t PMIC_BQ25896::getBATV_reg(){
    batv_reg_t temp_reg;
    _read(BATV, (uint8_t*)&temp_reg);
    return temp_reg;
}
uint16_t PMIC_BQ25896::getBATV(){
    batv_reg_t temp_reg = PMIC_BQ25896::getBATV_reg();
    uint16_t data = 2304 + (temp_reg.batv * 20);
    return data;
}


// REG0F
sysv_reg_t PMIC_BQ25896::getSYSV_reg(){
    sysv_reg_t temp_reg;
    _read(SYSV, (uint8_t*)&temp_reg);
    return temp_reg;
}
uint16_t PMIC_BQ25896::getSYSV(){
    sysv_reg_t temp_reg = PMIC_BQ25896::getSYSV_reg();
    uint16_t data = 2304 + (temp_reg.sysv * 20);
    return data;
}

// REG10
tspct_reg_t PMIC_BQ25896::getTSPCT_reg(){
    tspct_reg_t temp_reg;
    _read(TSPCT, (uint8_t*)&temp_reg);
    return temp_reg;
}
uint16_t PMIC_BQ25896::getTSPCT(){
    tspct_reg_t temp_reg = PMIC_BQ25896::getTSPCT_reg();
    uint16_t data = 21 + ((float)temp_reg.tspct * 0.465);
    return data;
}

// REG11
vbusv_reg_t PMIC_BQ25896::getVBUSV_reg(){
    vbusv_reg_t temp_reg;
    _read(VBUSV, (uint8_t*)&temp_reg);
    return temp_reg;
}
uint16_t PMIC_BQ25896::getVBUSV(){
    vbusv_reg_t temp_reg = PMIC_BQ25896::getVBUSV_reg();
    uint16_t data = 2600 + (temp_reg.vbusv * 100);
    return data;
}

// REG12
ichgr_reg_t PMIC_BQ25896::getICHGR_reg(){
    ichgr_reg_t temp_reg;
    _read(ICHGR, (uint8_t*)&temp_reg);
    return temp_reg;
}
uint16_t PMIC_BQ25896::getICHGR(){
    ichgr_reg_t temp_reg = PMIC_BQ25896::getICHGR_reg();
    uint16_t data = temp_reg.ichgr * 50;
    return data;
}

// REG13
idpm_lim_reg_t PMIC_BQ25896::getIDPM_LIM_reg(){
    idpm_lim_reg_t temp_reg;
    _read(IDPM_LIM, (uint8_t*)&temp_reg);
    return temp_reg;
}

// REG14
ctrl2_reg_t PMIC_BQ25896::getCTRL2_reg(){
    ctrl2_reg_t temp_reg;
    _read(CTRL2, (uint8_t*)&temp_reg);
    return temp_reg;
}

void PMIC_BQ25896::setREG_RST(bool value){
    ctrl2_reg_t temp_reg;

    _read(CTRL2, (uint8_t*)&temp_reg);
    temp_reg.reg_rst = value;
    _write(CTRL2, (uint8_t*)&temp_reg);
}

