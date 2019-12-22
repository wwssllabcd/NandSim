#pragma once


#include "DefineFiles/EricType.h"
#include "DefineFiles\EricType.h"

#define SINGLE_PAGE_SIZE_K       (16)
typedef struct {
    eu32 dataPtrList[SINGLE_PAGE_SIZE_K];
    eu32 tagPtrList[SINGLE_PAGE_SIZE_K];
    eu8 xferK;
}DataXfer;

void* get_nandsim_instance(int ceNo);
void erase_block(void* ptr, eu32 rowAddr);
void write_one_page(void* ptr, eu32 rowAddr, eu32 planeNo, eu8* ctrlRam, eu8* tagBuf, bool isSlc);
void set_max_page_no(void* ptr, eu32 maxPageNo, eu32 blockGap);

//void read_flash_1p(void* ptr, eu32 rowAddr, eu32 planeNo, bool isSlc, eu32 frameColOffset, eu32* dataPtrList, eu32* tagPtrList, Frame xferK);