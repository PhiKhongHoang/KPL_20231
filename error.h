#ifndef __ERROR_H__
#define __ERROR_H__

// danh sách các lỗi trong quá trình phân tích từ vựng
typedef enum
{
  ERR_ENDOFCOMMENT,
  ERR_IDENTTOOLONG,
  ERR_INVALIDCHARCONSTANT,
  ERR_INVALIDSYMBOL,
  ERR_NUMBERTOOLONG
} ErrorCode;

// các hàm thông báo lỗi
#define ERM_ENDOFCOMMENT "End of comment expected!"
#define ERM_IDENTTOOLONG "Identification too long!"
#define ERM_INVALIDCHARCONSTANT "Invalid const char!"
#define ERM_INVALIDSYMBOL "Invalid symbol!"
#define ERM_NUMBERTOOLONG "Value of integer number exceeds the range!"

// hàm thông báo lỗi
void error(ErrorCode err, int lineNo, int colNo);

#endif
