/* Scanner
 * @copyright (c) 2008, Hedspi, Hanoi University of Technology
 */

#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <ctype.h>
#include <string.h>
#include <limits.h>
#include "reader.h"
#include "charcode.h"
#include "token.h"
#include "error.h"

extern int lineNo;      // chỉ số dòng hiện tại
extern int colNo;       // chỉ số cột hiện tại
extern int currentChar; // kí tự hiện tại

extern CharCode charCodes[];
int state;
int ln, cn;
char str[MAX_IDENT_LEN];
char c;
int idx = 0;
/***************************************************************/

// đọc 1 token tính từ vị trí hiện tại
Token *getToken(void)
{
  Token *token;
  // start switch (state)
  switch (state)
  {
  // case 0: từ tố chỉ gồm 1 kí tự
  case 0:
    if (currentChar == EOF)
      state = 1;
    else
      switch (charCodes[currentChar])
      {
      case CHAR_SPACE:
        state = 2; // dấu cách
        break;
      case CHAR_LETTER: // chữ cái
        ln = lineNo;
        cn = colNo;
        state = 3;
        break;
      case CHAR_DIGIT: // chữ số
        state = 7;
        break;
      case CHAR_PLUS: // dấu +
        state = 9;
        break;
      case CHAR_MINUS: // dấu -
        state = 10;
        break;
      case CHAR_TIMES: // dấu *
        state = 11;
        break;
      case CHAR_SLASH: // dấu /
        state = 12;
        break;
      case CHAR_LT: // <
        state = 13;
        break;
      case CHAR_GT: // >
        state = 16;
        break;
      case CHAR_EQ: // =
        state = 19;
        break;
      case CHAR_COMMA: // ,
        state = 23;
        break;
      case CHAR_PERIOD: // .
        state = 26;
        break;
      case CHAR_SEMICOLON: // ;
        state = 27;
        break;
      case CHAR_COLON: // :
        state = 28;
        break;
      case CHAR_SINGLEQUOTE: // dấu gạch chéo '\'
        state = 31;
        break;
      case CHAR_LPAR: // (
        state = 35;
        break;
      case CHAR_RPAR: // )
        state = 42;
        break;
      case CHAR_MONGOACVUONG: // [
        state = 44;
        break;
      case CHAR_DONGNGOACVUONG: // ]
        state = 45;
        break;
      default:
        state = 43; // đại diện cho 1 lỗi
      }
    return getToken(); // gọi đệ quy thực hiện lại với các token khác

  case 1: // gặp EOF
    return makeToken(TK_EOF, lineNo, colNo);

  case 2: // dấu cách
    // TODO (Skip blanks)
    while (currentChar != EOF && charCodes[currentChar] == CHAR_SPACE)
      readChar();
    state = 0; // Chuyển về trạng thái 0 sau khi xử lý dấu cách
    return getToken();

  case 3: // chữ cái
    // TODO Recognize Identifiers and keywords
    ln = lineNo;
    cn = colNo;
    int count = 1;
    str[0] = (char)currentChar;
    readChar();
    while ((currentChar != EOF) &&
           ((charCodes[currentChar] == CHAR_LETTER) || (charCodes[currentChar] == CHAR_DIGIT)))
    {
      if (count <= MAX_IDENT_LEN)
      {
        str[count++] = (char)currentChar;
      }
      readChar(); // kí tự sau là chữ hoặc số thì tiếp tục đọc
    }
    if (count > MAX_IDENT_LEN)
    {
      error(ERR_IDENTTOOLONG, ln, cn);
      return makeToken(TK_NONE, ln, cn);
    }
    str[count] = '\0';
    state = 4;
    return getToken();

  case 4:
    token->tokenType = checkKeyword(str); // checkKeyword: khi hàm này return TK_NONE nghĩa là xâu cần kiểm tra là định danh
    if (token->tokenType == TK_NONE)
      state = 5; // nếu là định danh
    else
      state = 6; // nếu là từ khóa
    return getToken();

  case 5:
    // TODO Identifiers
    token = makeToken(TK_IDENT, ln, cn);
    strcpy(token->string, str);
    return token;

  case 6:
    // TODO Keywords
    token = makeToken(checkKeyword(str), ln, cn);
    return token;

  case 7: // chữ số
    // TODO  Numbers
    token = makeToken(TK_NUMBER, lineNo, colNo);
    int index1 = 0;
    while (charCodes[currentChar] == TK_NUMBER)
    {
      // 10 digits maximum
      if (index1 > 9)
      {
        error(ERR_NUMBERTOOLONG, lineNo, colNo);
        // break;
      }
      token->string[index1] = currentChar;
      readChar();
      index1++;
    }
    token->string[index1] = '\0';

    token->value = atoi(token->string);
    return token;

  case 9: // dấu +
    readChar();
    return makeToken(SB_PLUS, lineNo, colNo - 1);

  case 10: // dấu -
    // TODO
    readChar();
    return makeToken(SB_MINUS, lineNo, colNo - 1);

  case 11: // dấu *
    // TODO
    readChar();
    return makeToken(SB_TIMES, lineNo, colNo - 1);

  case 12: // dấu /
           // TODO
    readChar();
    return makeToken(SB_SLASH, lineNo, colNo - 1);

  // case 13 -> 15: từ tố gồm 2 kí tự
  case 13: // <
    readChar();
    if (charCodes[currentChar] == CHAR_EQ) // nếu kí tự là dấu = thì có nghĩa nó là <=
      state = 14;
    else if (charCodes[currentChar] == CHAR_GT) // nếu kí tự là dấu > thì có nghĩa nó là <>
      state = 21;
    else // còn kp dấu = thì nó chỉ là <
      state = 15;
    return getToken();

  case 14:
    readChar();
    return makeToken(SB_LE, lineNo, colNo - 1);

  case 15:
    return makeToken(SB_LT, lineNo, colNo - 1);

  case 21:
    readChar();
    return makeToken(SB_NEQ, lineNo, colNo - 1);

  case 16: // >
    // TODO
    readChar();
    if (charCodes[currentChar] == CHAR_EQ)
      state = 17;
    else
      state = 18;
    return getToken();

  case 17:
    // TODO
    readChar();
    return makeToken(SB_GE, lineNo, colNo - 1);

  case 18:
    // TODO
    return makeToken(SB_GT, lineNo, colNo - 1);

  case 19: // =
    // TODO
    readChar();
    return makeToken(SB_EQ, lineNo, colNo - 1);

  case 23:
    // TODO
    readChar();
    return makeToken(SB_COMMA, lineNo, colNo - 1);

  case 26:
    readChar();
    return makeToken(SB_PERIOD, lineNo, colNo - 1);

  case 27:
    // TODO
    readChar();
    return makeToken(SB_SEMICOLON, lineNo, colNo - 1);

  case 28:
    // TODO
    readChar();
    if (charCodes[currentChar] == CHAR_EQ)
      state = 29;
    else
      state = 30;
    return getToken();

  case 29:
    // TODO
    readChar();
    return makeToken(SB_ASSIGN, lineNo, colNo - 2);

  case 30:
    // TODO
    return makeToken(SB_COLON, lineNo, colNo - 1);

    // case 31 -> 34: nhận biết hằng kí tự
  case 31:
    readChar();
    if (currentChar == EOF)
      state = 34;
    else if (isprint(currentChar)) // kiểm tra xem kí tự hiện tại (currentChar) có phải là một kí tự có thể in ra màn hình hay không.
      state = 32;
    else
      state = 34;
    return getToken();

  case 32:
    c = currentChar;
    readChar();
    if (charCodes[currentChar] == CHAR_SINGLEQUOTE)
      state = 33;
    else
      state = 34;
    return getToken();

  case 33:
    token = makeToken(TK_CHAR, lineNo, colNo - 1);
    token->string[0] = c;
    token->string[1] = '\0';
    readChar();
    return token;

  case 34:
    error(ERR_INVALIDCHARCONSTANT, lineNo, colNo - 2);

    // case 35: bỏ qua chú thích
  case 35: // đọc dấu (, nhận ra SB_LSEL, SB_LPAR hoặc bỏ qua chú thích
    ln = lineNo;
    cn = colNo;
    readChar();
    if (currentChar == EOF)
      state = 41;
    else
      switch (charCodes[currentChar])
      {
      case CHAR_TIMES:
        state = 38; // (* *) bắt đầu và kết thúc cmt
        break;
      default:
        state = 41; // SB_LPAR
      }
    return getToken();

  case 37:
    // TODO
    readChar();
    if (currentChar == EOF)
      state = 40;
    else if (charCodes[currentChar] == CHAR_TIMES)
      state = 38;
    else
      state = 37;
    return getToken();

  case 38:
    // TODO
    readChar();
    if (currentChar == EOF)
      state = 40;
    else if (charCodes[currentChar] == CHAR_RPAR)
      state = 39;
    else if (charCodes[currentChar] == CHAR_TIMES)
      state = 38;
    else
      state = 37;
    return getToken();

  case 39:
    // TODO
    readChar();
    state = 0;
    return getToken();

  case 40:
    error(ERR_ENDOFCOMMENT, lineNo, colNo);

  case 41:
    return makeToken(SB_LPAR, ln, cn);

  case 42:
    readChar();
    return makeToken(SB_RPAR, lineNo, colNo);

  case 43:
    token = makeToken(TK_NONE, lineNo, colNo);
    error(ERR_INVALIDSYMBOL, lineNo, colNo);
    readChar();
    return token;

  // Trạng thái mới để nhận diện chỉ số trong cặp dấu ngoặc vuông
  case 44:
    readChar();
    return makeToken(SB_LSEL, lineNo, colNo - 1);

  case 45:
    readChar();
    return makeToken(SB_RSEL, lineNo, colNo - 1);

    } // end switch (state)
}

/******************************************************************/

void printToken(Token *token)
{

  printf("%d-%d:", token->lineNo, token->colNo);

  switch (token->tokenType)
  {
  case TK_NONE:
    printf("TK_NONE\n");
    break;
  case TK_IDENT:
    printf("TK_IDENT(%s)\n", token->string);
    break;
  case TK_NUMBER:
    printf("TK_NUMBER(%s)\n", token->string);
    break;
  case TK_CHAR:
    printf("TK_CHAR(\'%s\')\n", token->string);
    break;
  case TK_EOF:
    printf("TK_EOF\n");
    break;

  case KW_PROGRAM:
    printf("KW_PROGRAM\n");
    break;
  case KW_CONST:
    printf("KW_CONST\n");
    break;
  case KW_TYPE:
    printf("KW_TYPE\n");
    break;
  case KW_VAR:
    printf("KW_VAR\n");
    break;
  case KW_INTEGER:
    printf("KW_INTEGER\n");
    break;
  case KW_CHAR:
    printf("KW_CHAR\n");
    break;
  case KW_ARRAY:
    printf("KW_ARRAY\n");
    break;
  case KW_OF:
    printf("KW_OF\n");
    break;
  case KW_FUNCTION:
    printf("KW_FUNCTION\n");
    break;
  case KW_PROCEDURE:
    printf("KW_PROCEDURE\n");
    break;
  case KW_BEGIN:
    printf("KW_BEGIN\n");
    break;
  case KW_END:
    printf("KW_END\n");
    break;
  case KW_CALL:
    printf("KW_CALL\n");
    break;
  case KW_IF:
    printf("KW_IF\n");
    break;
  case KW_THEN:
    printf("KW_THEN\n");
    break;
  case KW_ELSE:
    printf("KW_ELSE\n");
    break;
  case KW_WHILE:
    printf("KW_WHILE\n");
    break;
  case KW_DO:
    printf("KW_DO\n");
    break;
  case KW_FOR:
    printf("KW_FOR\n");
    break;
  case KW_TO:
    printf("KW_TO\n");
    break;
  case SB_SEMICOLON:
    printf("SB_SEMICOLON\n");
    break;
  case SB_COLON:
    printf("SB_COLON\n");
    break;
  case SB_PERIOD:
    printf("SB_PERIOD\n");
    break;
  case SB_COMMA:
    printf("SB_COMMA\n");
    break;
  case SB_ASSIGN:
    printf("SB_ASSIGN\n");
    break;
  case SB_EQ:
    printf("SB_EQ\n");
    break;
  case SB_NEQ:
    printf("SB_NEQ\n");
    break;
  case SB_LT:
    printf("SB_LT\n");
    break;
  case SB_LE:
    printf("SB_LE\n");
    break;
  case SB_GT:
    printf("SB_GT\n");
    break;
  case SB_GE:
    printf("SB_GE\n");
    break;
  case SB_PLUS:
    printf("SB_PLUS\n");
    break;
  case SB_MINUS:
    printf("SB_MINUS\n");
    break;
  case SB_TIMES:
    printf("SB_TIMES\n");
    break;
  case SB_SLASH:
    printf("SB_SLASH\n");
    break;
  case SB_LPAR:
    printf("SB_LPAR\n");
    break;
  case SB_RPAR:
    printf("SB_RPAR\n");
    break;
  case SB_LSEL:
    printf("SB_LSEL\n");
    break;
  case SB_RSEL:
    printf("SB_RSEL\n");
    break;
  }
}

// phát hiện từng token trong chương trình nguồn, sau khi trả về 1 token, quay lại trạng thái 0
int scan(char *fileName)
{
  Token *token;
  if (openInputStream(fileName) == IO_ERROR)
    return IO_ERROR;

  token = getToken();
  while (token->tokenType != TK_EOF)
  {
    printToken(token);
    free(token);
    state = 0; // mỗi khi hoàn tất nhận dạng 1 token automat chuyển lại về trạng thái 0
    token = getToken();
  }

  free(token);
  closeInputStream();
  return IO_SUCCESS;
}

int main()
{
  if (scan("test\\testprj.kpl") == IO_ERROR)
  {
    printf("Can\'t read input file!\n");
  }
  return 0;
}
