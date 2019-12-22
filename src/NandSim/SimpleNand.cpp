//Copyright © EricWang(wwssllabcd@gmail.com). All rights reserved

#include "stdafx.h"
#include "SimpleNand.h"
#include "NandSimGlobalDefine.h"

#include "Utility/Utility.h"
#include "Utility/EricException.h"
#include "UtilityDialog/DialogUtility.h"

using namespace EricCore;

SimpleNand::SimpleNand(eu32 ceNo, eu32 pageNo) {
    m_ceNo = ceNo;
    m_pagePerBlock = NULL_32;
}

void SimpleNand::set_max_page_no(eu32 pageNo, eu32 blockGap) {

    m_pagePerBlock = pageNo;
    m_block_gap = blockGap;
}

SimpleNand::~SimpleNand() {
}

void SimpleNand::showError(bool cond, const echar* str) {
    if (cond == false) {
        THROW_MYEXCEPTION(0, str);
    }
}

void SimpleNand::erase_block_1plane(eu32 rowAddr) {
    for (int i = 0; i < 2; i++) {
        bool isSlc = false;
        if (i == 1) {
            isSlc = true;
        }

        eu32 res = rowAddr % m_block_gap;
        showError((res == 0), _ET("NS-erase: pageSize Should be zero when erase"));
        showError(m_pagePerBlock != NULL_32, _ET("NS-erase: Wrong m_pagePerBlock"));
        m_nu.erase_block_1p(m_ceNo, rowAddr, m_pagePerBlock, isSlc);
    }
}

eu8*  SimpleNand::_get_nand_buffer(eu32 planeNo) {

    EricCore::DialogUtility du;
    du.updateOS();

    if (planeNo == 0) {
        return m_PlaneBuf_0;
    } else if (planeNo == 1) {
        return m_PlaneBuf_1;
    } else if (planeNo == 2) {
        return m_PlaneBuf_2;
    } else if (planeNo == 3) {
        return m_PlaneBuf_3;
    }
    showError(0, "plane over 4");
    return 0;
}

void SimpleNand::writeData(eu32 rowAddr, eu32 planeNo, eu8* ctrlBuf, eu8* tagBuf, bool isSlc) {
    eu8* nandBuf = _get_nand_buffer(planeNo);
    m_nha.ctrller_to_nand(nandBuf, ctrlBuf, tagBuf, SN_PAGE_SIZE);
    m_nu.write_page_1p(m_ceNo, rowAddr, isSlc, nandBuf, SIMPLE_NANDSIM_PAGELEN);
}

void SimpleNand::read_flash_data(eu32 rowAddr, eu32 planeNo, bool isSlc, eu32 startFrameNo, eu32* dataPtrList, eu32* tagPtrList, eu8 xferK) {

    if (xferK == 0) {
        showError(0, "xfer len = 0");
    }

    if (startFrameNo >= SN_FRAME_PER_PAGE) {
        showError(0, "Error col Offset");
    }

    eu8* nandBuf = _get_nand_buffer(planeNo);
    m_nu.read_page_1p(m_ceNo, rowAddr, isSlc, nandBuf, SIMPLE_NANDSIM_PAGELEN);


    //m_nha.nand_to_ctrller(nandBuf, ctrllerBuf, tagBuf, xferK, startFrameNo);

    for (eu32 i = 0; i < xferK; i++) {
        eu8* dataPtr = (eu8*)dataPtrList[i];
        eu8* tagPtr = (eu8*)tagPtrList[i];

        if ((eu32)dataPtr == NULL_32) {
            showError(0, "wrong ptr");
        }

        memcpy(dataPtr, nandBuf + (startFrameNo + i)*_1K, _1K);
        memcpy(tagPtr, nandBuf + PARITY_START_OFFSET + (PARITY_LEN_PER_FRAME * i), HW_TAG_LEN_PER_FRAME);
    }
}