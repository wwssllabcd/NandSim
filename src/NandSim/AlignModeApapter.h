#pragma once

#include "DefineFiles\DefineFile.h"


class AlignModeApapter
{
public:
    AlignModeApapter(void);
	~AlignModeApapter(void);
    void AlignModeApapter::_show_error(bool cond, const echar* str);
    void AlignModeApapter::setEccParity(eu8* nandBuf, eu8* tagBuf);
    eu32 AlignModeApapter::ecc_parity_len_per_1k(void);
    void AlignModeApapter::ctrller_to_nand_align_mode(eu8_p nandBuf, eu8_p ctrlBuf, eu8_p tagBuf, int dataLen);


    void AlignModeApapter::ctrller_to_nand(eu8_p nandBuf, eu8_p dataBuf, eu8_p tagBuf, int length);
    void AlignModeApapter::nand_to_ctrller(eu8_p nandBuf, eu8_p ctrllerBuf, eu8_p tagBuf, eu8 xferK, eu32 frameOffset);

    void AlignModeApapter::nand_to_ctrller_align_mode(eu8_p nandBuf, eu8_p ctrllerBuf, eu8_p tagBuf, eu8 xferK, eu32 frameOffset);

};

