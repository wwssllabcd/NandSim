//Copyright © EricWang(wwssllabcd@gmail.com). All rights reserved
#include "stdafx.h"
#include "NandHwApapter.h"

#include "Utility/EricException.h"



#define TAG_LEN                             (0x4)
#define DATA_STATUS_OFFSET                  (TAG_LEN) 
#define TOTAL_PARITY_1k                     (0x10) //align 0x10

using namespace EricCore;

NandHwApapter::NandHwApapter(void)
{
}

NandHwApapter::~NandHwApapter(void) {}

void NandHwApapter::_show_error(bool cond, const echar* str) {
	if (cond == false) {
		THROW_MYEXCEPTION(0, str);
	}
}

void NandHwApapter::setEccParity(eu8* nandBuf, eu8* tagBuf) {
	// init ecc parity
	for (eu8 offset = 0; offset < TOTAL_PARITY_1k; offset++) {
		nandBuf[offset] = offset;
	}

	memcpy(nandBuf, tagBuf, TAG_LEN);

	//set unc info ( no any ecc occur after program)
	//nandBuf[DATA_STATUS_OFFSET] = 0x78;
}



eu32 NandHwApapter::ecc_parity_len_per_1k(void) {
	return TOTAL_PARITY_1k;
}

void NandHwApapter::ctrller_to_nand_align_mode(eu8_p nandBuf, eu8_p ctrlBuf, eu8_p tagBuf, int dataLen) {
    memcpy(nandBuf, ctrlBuf, dataLen);
    eu32 tagCnt = dataLen / _1K;
    for (eu32 i = 0; i < tagCnt; i++) {
        setEccParity(nandBuf + dataLen + i * 0x10, tagBuf + i * 4);
    }
}

void NandHwApapter::ctrller_to_nand_true_mode(eu8_p nandBuf, eu8_p dataBuf, eu8_p tagBuf, int length) {
    if (length == 0) {
        return;
    }
    if (length < 0) {
        _show_error(0, _ET("NS: length = 0"));
        return;
    }
    if (length < _1K) {
        _show_error(0, _ET("NS: not support write less 1024"));
    }

    eu32 opLen = _1K;

    // copy user data 1k
    memcpy(nandBuf, dataBuf, opLen);

    //set ECC parity
    setEccParity(nandBuf + opLen, tagBuf);

    ctrller_to_nand(
        nandBuf + _1K + ecc_parity_len_per_1k()
        , dataBuf + _1K
        , tagBuf + TAG_LEN
        , length - opLen
    );
}

void NandHwApapter::nand_to_ctrller_true_mode(eu8_p nandBuf, eu8_p ctrllerBuf, eu8_p tagBuf, int length) {
    if (length == 0) {
        return;
    }
    if (length < 0) {
        _show_error(0, _ET("NS: length = 0"));
        return;
    }

    if (length < _1K) {
        _show_error(0, _ET("NS: not support write less 1024"));
    }

    eu32 opLen = _1K;

    // copy user data 1k
    memcpy(ctrllerBuf, nandBuf, opLen);

    // clear
    eu8 tmp[TAG_LEN] = { 0x78, 0x78, 0x78, 0x78 };
    memcpy(tagBuf, tmp, TAG_LEN);

    // copy ecc data
    memcpy(tagBuf, nandBuf + opLen, TAG_LEN);

    //tail recursive
    nand_to_ctrller_true_mode(
        nandBuf + _1K + ecc_parity_len_per_1k()
        , ctrllerBuf + _1K
        , tagBuf + TAG_LEN
        , length - opLen
    );
}

void NandHwApapter::nand_to_ctrller_align_mode(eu8_p nandBuf, eu8_p ctrllerBuf, eu8_p tagBuf, int dataLen) {
    memcpy(ctrllerBuf, nandBuf,  dataLen);
    eu32 tagCnt = dataLen / _1K;
    for (eu32 i = 0; i < tagCnt; i++) {
        eu8 tmp[4];
        memcpy(tmp, nandBuf + dataLen + (0x10 * i), TAG_LEN);
        memcpy(tagBuf + i*TAG_LEN, nandBuf + dataLen + (0x10 * i), TAG_LEN);
    }
}

void NandHwApapter::ctrller_to_nand(eu8_p nandBuf, eu8_p dataBuf, eu8_p tagBuf, int length) {
    ctrller_to_nand_true_mode(nandBuf, dataBuf, tagBuf, length);
}

//void NandHwApapter::hw_nand_to_ctrller(eu8_p nandBuf, eu8_p ctrllerBuf, eu8_p tagBuf, int length, eu32 frameNo) {
//    eu32 readOffset = frameNo*(_1K + ecc_parity_len_per_1k());
//    nand_to_ctrller_true_mode(nandBuf + readOffset, ctrllerBuf, tagBuf, length);
//}