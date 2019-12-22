#pragma once

#include "DefineFiles/OtherDefine.h"
#include "DefineFiles/DefineFile.h"
#include "Utility/Utility.h"

using namespace EricCore;

class NandUtility
{
public:
	NandUtility(void);
	~NandUtility(void);

	
	void read_page_1p(eu32 ce, eu32 rowAddr, bool isSlc, eu8_p buffer, eu32 xferLen);
	void write_page_1p(eu32 ce, eu32 rowAddr, bool isSlc, eu8_p buffer, eu32 xferLen);
	void erase_block_1p(eu32 ce, eu32 rowAddr, eu32 endPage, bool isSlc);

private:
	estring m_dataFolder;
	Utility m_u;

	estring _to_file_path(eu32 ce, eu32 rowAddr, bool isSlc);
	void _show_error(bool cond, const echar* str);
    void _write_to_file(estring filePath, eu8_p buffer, eu32 xferLen);
    void _read_from_file(estring filePath, eu8_p buffer, eu32 xferLen);
};

