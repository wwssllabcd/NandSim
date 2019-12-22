#pragma once

#include "DefineFiles\DefineFile.h"
#include "Utility\Utility.h"

#define ENABLE_COMPRESS
#define TOTAL_COMPRESS_SECTOR_CNT       (0x20)
#define COMPRESS_DATA_LENGTH            (1024)
#define USER_DATA_LEN                   (16384)

using namespace EricCore;




class CompressData
{
public:
    CompressData(void);
	~CompressData(void);

    Utility m_u;
    
    bool CompressData::compress_data(eu8_p srcBuf, eu8_p descBuf);
    bool CompressData::is_repeat_data(eu8_p buffer, eu32 length);
    void CompressData::decompress_data(eu8_p srcBuf, eu8_p descBuf);
};

