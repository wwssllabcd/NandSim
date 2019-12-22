//Copyright © EricWang(wwssllabcd@gmail.com). All rights reserved
#include "stdafx.h"
#include "CompressData.h"
#include "Utility/EricException.h"



using namespace EricCore;


CompressData::CompressData(void) {
}

CompressData::~CompressData(void) {
}

bool CompressData::is_repeat_data(eu8_p u8Buffer, eu32 byteLen) {
    eu32_p buf = (eu32_p)u8Buffer;
    eu32 length = byteLen / 4;
    eu32 pattern = buf[0];
    for (eu32 i = 1; i < length; i++) {
        if (pattern != buf[i]) {
            return false;
        }
    }
    return true;
}

bool CompressData::compress_data(eu8_p srcBuf, eu8_p descBuf) {
    eu32 pattern[TOTAL_COMPRESS_SECTOR_CNT];
    for (int secNo = 0; secNo < TOTAL_COMPRESS_SECTOR_CNT; secNo++) {
        eu32 offset = secNo * 512;
        bool res = is_repeat_data(srcBuf + offset, 512);
        if (res == false) {
            return false;
        }
        pattern[secNo] = PTR_TO_EU32(srcBuf + offset);
    }

    memcpy(descBuf, pattern, sizeof(pattern));
    return true;
}

void CompressData::decompress_data(eu8_p srcBuf, eu8_p descBuf) {
    eu32_p eu32Ptr = (eu32_p)srcBuf;
    eu32_usp eu32dscPtr = (eu32_p)descBuf;

    for (int secNo = 0; secNo < TOTAL_COMPRESS_SECTOR_CNT; secNo++) {
        eu32 pattern = eu32Ptr[secNo];
        for (int i = 0; i < 128; i++) {
            eu32dscPtr[i] = pattern;
        }
        eu32dscPtr += 128;
    }
    memcpy(descBuf + USER_DATA_LEN, srcBuf + 512, 512);
}

