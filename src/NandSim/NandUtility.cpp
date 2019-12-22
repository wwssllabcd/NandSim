//Copyright © EricWang(wwssllabcd@gmail.com). All rights reserved
#include "stdafx.h"
#include "NandUtility.h"
#include "Utility/EricException.h"
#include "CompressData.h"

using namespace EricCore;

NandUtility::NandUtility(void)
{
	m_dataFolder = "R:\\";
}

NandUtility::~NandUtility(void) {}

void NandUtility::_show_error(bool cond, const echar* str) {
	if(cond == false) {
		THROW_MYEXCEPTION(0, str);
	}
}

estring NandUtility::_to_file_path(eu32 ce, eu32 rowAddr, bool isSlc) {
	estring filePath = m_u.toString((eu32)rowAddr, _ET("%06X"));
	estring fileNameDec = m_u.toString((eu32)rowAddr, _ET("%08d"));
	estring strCeNo = m_u.toString((eu32)ce, _ET("%02X"));

	estring type;
	if(isSlc) {
		type = "_slc";
	}
	return m_dataFolder + fileNameDec + _ET("_") + strCeNo + _ET("_") + filePath + type + _ET(".bin");
}

void NandUtility::read_page_1p(eu32 ce, eu32 rowAddr, bool isSlc, eu8_p buffer, eu32 xferLen) {
	estring filePath = _to_file_path(ce, rowAddr, isSlc);
	if(m_u.isFileExist(filePath)) {
        _read_from_file(filePath, buffer, xferLen);
	} else {
		memset(buffer, 0xFF, xferLen);
	}
}

void NandUtility::write_page_1p(eu32 ce, eu32 rowAddr, bool isSlc, eu8_p buffer, eu32 xferLen) {
	estring filePath = _to_file_path(ce, rowAddr, isSlc);
	// check over write
	if(m_u.isFileExist(filePath)) {
		estring msg = _ET("NandCore: OverWritePage occur") + filePath;
		_show_error(0, msg.c_str());
	}
    _write_to_file(filePath, buffer, xferLen);
}

void NandUtility::erase_block_1p(eu32 ce, eu32 rowAddr, eu32 endPage, bool isSlc) {
	estring filePath;
	if(endPage > 0x100000) {
		THROW_MYEXCEPTION(0, "endPage too big, plz check");
	}

	for(eu32 i = 0; i < endPage; i++) {
		filePath = _to_file_path(ce, rowAddr + i, isSlc);
		if(m_u.isFileExist(filePath) == false) {
			continue;
		}
		m_u.deleteFile(filePath.c_str());
	}
}

void NandUtility::_write_to_file(estring filePath, eu8_usp buffer, eu32 xferLen) {
#ifdef ENABLE_COMPRESS
    CompressData cd;
    eu8 resBuf[COMPRESS_DATA_LENGTH];
    memset(resBuf, 0xFF, sizeof(resBuf));

    bool isCompress = cd.compress_data(buffer, resBuf);
    if (isCompress) {
        xferLen = COMPRESS_DATA_LENGTH;
        memcpy(resBuf + 512, buffer + USER_DATA_LEN, 512);
        buffer = resBuf;
    }
#endif
    m_u.toFile(filePath, buffer, xferLen);
}

void NandUtility::_read_from_file(estring filePath, eu8_p buffer, eu32 xferLen) {
    long fileSize = m_u.getFileSize(filePath);
    
    if (fileSize == (_17K)) {
        m_u.getFileData(filePath, fileSize, buffer);
    } else {
        CompressData cd;
        eu8 compBuf[COMPRESS_DATA_LENGTH];
        m_u.getFileData(filePath, fileSize, compBuf);
        cd.decompress_data(compBuf, buffer);
    }
}