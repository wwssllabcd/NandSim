#include "stdafx.h"
#include "SimpleNand.h"
#include "NandSimGlobalDefine.h"

//Copyright © 2006- EricWang(wwssllabcd@gmail.com). All rights reserved
#define _SIMPLE_NAND_
#ifdef _SIMPLE_NAND_

//#include "global/FlashParam.h" // for Ftl

#define WP_PAGE_PER_BLOCK (0xFFFF)        // for erase_block
SimpleNand m_nand_0(0, WP_PAGE_PER_BLOCK);
SimpleNand m_nand_1(1, WP_PAGE_PER_BLOCK);
SimpleNand m_nand_2(2, WP_PAGE_PER_BLOCK);
SimpleNand m_nand_3(3, WP_PAGE_PER_BLOCK);



extern "C" {

	void* get_nandsim_instance(int ceNo) {
#ifdef _SIMPLE_NAND_
		if (ceNo == 0) {
			return &m_nand_0;
		}
		else if (ceNo == 1) {
			return &m_nand_1;
		}
		else if (ceNo == 2) {
			return &m_nand_2;
		}
		else if (ceNo == 3) {
			return &m_nand_3;
		}
		else {
			m_nand_0.showError(0, "ceNo OFB");
		}
#endif
		return 0;
	}

	void set_max_page_no(void* ptr, eu32 maxPageNo, eu32 blockGap) {
		SimpleNand* nand = (SimpleNand*)ptr;
		nand->set_max_page_no(maxPageNo, blockGap);
	}

	void erase_block(void* ptr, eu32 rowAddr) {
		SimpleNand* nand = (SimpleNand*)ptr;
		nand->erase_block_1plane(rowAddr);
	}

	void write_one_page(void* ptr, eu32 rowAddr, eu32 planeNo, eu8* ctrlRam, eu8* tagBuf, bool isSlc) {
		SimpleNand* nand = (SimpleNand*)ptr;
		nand->writeData(rowAddr, planeNo, ctrlRam, tagBuf, isSlc);
	}

    //frameColOffset(offsetK) = 1, 2, 3, 4, ...
	void read_flash_1p(void* ptr, eu32 rowAddr, eu32 planeNo, bool isSlc, eu32 frameColOffset, eu32* dataPtrList, eu32* tagPtrList, eu8 xferK) {
		SimpleNand* nand = (SimpleNand*)ptr;
		nand->read_flash_data(rowAddr, planeNo, isSlc, frameColOffset, dataPtrList, tagPtrList, xferK);
	}
}
#endif