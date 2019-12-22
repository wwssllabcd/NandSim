#pragma once

#include "DefineFiles\DefineFile.h"


class NandHwApapter
{
public:
	NandHwApapter(void);
	~NandHwApapter(void);
	
    //void hw_nand_to_ctrller(eu8_p nandBuf, eu8_p ctrllerBuf, eu8_p tagBuf, int length, eu32 frameNo);
	void ctrller_to_nand(eu8_p nandBuf, eu8_p dataBuf, eu8_p tagBuf, int length);
	eu32 ecc_parity_len_per_1k(void);
    

private:
    void setEccParity(eu8* nandBuf, eu8* tagBuf);
	void _show_error(bool cond, const echar* str);

    void ctrller_to_nand_true_mode(eu8_p nandBuf, eu8_p dataBuf, eu8_p tagBuf, int length);
    void ctrller_to_nand_align_mode(eu8_p nandBuf, eu8_p ctrlBuf, eu8_p tagBuf, int dataLen);

    void nand_to_ctrller_true_mode(eu8_p nandBuf, eu8_p ctrllerBuf, eu8_p tagBuf, int length);
    void nand_to_ctrller_align_mode(eu8_p nandBuf, eu8_p ctrllerBuf, eu8_p tagBuf, int length);
};

