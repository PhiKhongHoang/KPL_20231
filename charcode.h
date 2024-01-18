/*
 * @copyright (c) 2008, Hedspi, Hanoi University of Technology
 * @author Huu-Duc Nguyen
 * @version 1.0
 */

#ifndef __CHARCODE_H__
#define __CHARCODE_H__

typedef enum
{
  CHAR_SPACE,         // Khoảng trắng
  CHAR_LETTER,        // chữ cái
  CHAR_DIGIT,         // chữ số
  CHAR_PLUS,          // dấu +
  CHAR_MINUS,         // dấu -
  CHAR_TIMES,         // dấu *
  CHAR_SLASH,         // dấu /
  CHAR_LT,            // dấu <
  CHAR_GT,            // dấu >
  CHAR_EQ,            // dấu =
  CHAR_COMMA,         // dấu ,
  CHAR_PERIOD,        // dấu .
  CHAR_COLON,         // dấu :
  CHAR_SEMICOLON,     // dấu ;
  CHAR_SINGLEQUOTE,   // dấu nháy đơn '
  CHAR_LPAR,          // dấu (
  CHAR_RPAR,          // dấu )
  CHAR_UNKNOWN,       // kí tự ngoài bảng chữ cái
  CHAR_MONGOACVUONG,  // dấu [
  CHAR_DONGNGOACVUONG // dấu ]
} CharCode;

#endif
