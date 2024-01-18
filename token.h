/*
 * @copyright (c) 2008, Hedspi, Hanoi University of Technology
 * @author Huu-Duc Nguyen
 * @version 1.0
 */

#ifndef __TOKEN_H__
#define __TOKEN_H__

#define MAX_IDENT_LEN 15
#define KEYWORDS_COUNT 20

typedef enum
{
  TK_NONE,   // đại diện cho 1 lỗi
  TK_IDENT,  // định danh
  TK_NUMBER, // số
  TK_CHAR,   // hằng kí tự
  TK_EOF,    // kết thúc chương trình

  // các từ khóa
  KW_PROGRAM,
  KW_CONST,
  KW_TYPE,
  KW_VAR,
  KW_INTEGER,
  KW_CHAR,
  KW_ARRAY,
  KW_OF,
  KW_FUNCTION,
  KW_PROCEDURE,
  KW_BEGIN,
  KW_END,
  KW_CALL,
  KW_IF,
  KW_THEN,
  KW_ELSE,
  KW_WHILE,
  KW_DO,
  KW_FOR,
  KW_TO,

  // các kí hiệu đặc biệt
  SB_SEMICOLON,
  SB_COLON,
  SB_PERIOD,
  SB_COMMA,
  SB_ASSIGN,
  SB_EQ,
  SB_NEQ,
  SB_LT,
  SB_LE,
  SB_GT, // >
  SB_GE, // >=
  SB_PLUS,
  SB_MINUS,
  SB_TIMES,
  SB_SLASH,
  SB_LPAR, // (
  SB_RPAR,
  SB_LSEL, // [ mở giới hạn chỉ số
  SB_RSEL // ] đóng giới hạn chỉ số
} TokenType;

// cấu trúc lưu trữ 1 token
typedef struct
{
  char string[MAX_IDENT_LEN + 1]; // lưu chuỗi kí tự của token
  int lineNo, colNo;
  TokenType tokenType;
  int value; // lưu trữ giá trị của token khi token đó là một số (TK_NUMBER) hoặc hằng kí tự (TK_CHAR).
} Token;

TokenType checkKeyword(char *string);                         // kiểm tra 1 xâu có là từ khóa không
Token *makeToken(TokenType tokenType, int lineNo, int colNo); // tạo 1 token mới với kiểu và vị trí

#endif
