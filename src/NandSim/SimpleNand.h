#pragma once

#include "DefineFiles/DefineFile.h"
#include "NandUtility.h"
#include "NandHwApapter.h"
#include "AlignModeApapter.h"

#define SN_FRAME_PER_PAGE          (16)
#define SN_PAGE_SIZE               (SN_FRAME_PER_PAGE*1024)
#define SIMPLE_NANDSIM_PAGELEN     (SN_PAGE_SIZE + 1024) // 16k+ecc

class SimpleNand
{
public:
	SimpleNand(eu32 ceNo, eu32 pageNo);
	~SimpleNand();

	void set_max_page_no(eu32 pageNo, eu32 blockGap);
	void erase_block_1plane(eu32 rowAddr);
	void writeData(eu32 rowAddr, eu32 planeNo, eu8* ctrlBuf, eu8* tagBuf, bool isSlc);
	void read_flash_data(eu32 rowAddr, eu32 planeNo, bool isSlc, eu32 frame, eu32* dataPtrList, eu32* tagPtrList, eu8 xferK);

	void showError(bool cond, const echar* str);

private:
	eu32 m_ceNo;
	eu32 m_pagePerBlock;
	eu32 m_block_gap;



	// nand buffer
	eu8 m_PlaneBuf_0[SIMPLE_NANDSIM_PAGELEN];
	eu8 m_PlaneBuf_1[SIMPLE_NANDSIM_PAGELEN];
	eu8 m_PlaneBuf_2[SIMPLE_NANDSIM_PAGELEN];
	eu8 m_PlaneBuf_3[SIMPLE_NANDSIM_PAGELEN];


	NandUtility m_nu;
    AlignModeApapter m_nha;


	eu8*  _get_nand_buffer(eu32 planeNo);
};