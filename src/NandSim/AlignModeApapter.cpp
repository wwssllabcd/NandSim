//Copyright © EricWang(wwssllabcd@gmail.com). All rights reserved
#include "stdafx.h"
#include "AlignModeApapter.h"
#include "Utility/EricException.h"

#define PARITY_START_OFFSET                (_16K)
#define TAG_LEN                             (0x4)
#define DATA_STATUS_OFFSET                  (TAG_LEN) 
#define TOTAL_PARITY_1k                     (0x10) //align 0x10




AlignModeApapter::AlignModeApapter(void) {
}

AlignModeApapter::~AlignModeApapter(void) {
}

void AlignModeApapter::_show_error(bool cond, const echar* str) {
    if (cond == false) {
        THROW_MYEXCEPTION(0, str);
    }
}

void AlignModeApapter::setEccParity(eu8* nandBuf, eu8* tagBuf) {
    // init ecc parity
    for (eu8 offset = 0; offset < TOTAL_PARITY_1k; offset++) {
        nandBuf[offset] = offset;
    }

    memcpy(nandBuf, tagBuf, TAG_LEN);
}

eu32 AlignModeApapter::ecc_parity_len_per_1k(void) {
    return TOTAL_PARITY_1k;
}

void AlignModeApapter::ctrller_to_nand_align_mode(eu8_p nandBuf, eu8_p ctrlBuf, eu8_p tagBuf, int dataLen) {
    memcpy(nandBuf, ctrlBuf, dataLen);
    eu32 tagCnt = dataLen / _1K;
    for (eu32 i = 0; i < tagCnt; i++) {
        setEccParity(nandBuf + dataLen + i * 0x10, tagBuf + i * 4);
    }
}

void AlignModeApapter::nand_to_ctrller_align_mode(eu8_p nandBuf, eu8_p ctrllerBuf, eu8_p tagBuf, eu8 xferK, eu32 frameOffset) {
    memcpy(ctrllerBuf, nandBuf+ frameOffset*_1K, xferK*_1K);

    for (eu32 i = 0; i < xferK; i++) {
        memcpy(tagBuf + (i + frameOffset)*TAG_LEN, nandBuf + PARITY_START_OFFSET + (0x10 * i), TAG_LEN);
    }
}

void AlignModeApapter::ctrller_to_nand(eu8_p nandBuf, eu8_p dataBuf, eu8_p tagBuf, int length) {
    ctrller_to_nand_align_mode(nandBuf, dataBuf, tagBuf, length);
}

void AlignModeApapter::nand_to_ctrller(eu8_p nandBuf, eu8_p ctrllerBuf, eu8_p tagBuf, eu8 xferK, eu32 frameOffset) {
    nand_to_ctrller_align_mode(nandBuf, ctrllerBuf, tagBuf, xferK, frameOffset);
}