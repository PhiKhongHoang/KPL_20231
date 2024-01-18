/*
 * @copyright (c) 2008, Hedspi, Hanoi University of Technology
 * @author Huu-Duc Nguyen
 * @version 1.0
 */

#ifndef __READER_H__
#define __READER_H__

#define IO_ERROR 0
#define IO_SUCCESS 1

int readChar(void); // đọc 1 kí tự từ kênh vào
int openInputStream(char *fileName); // mở kênh vào
void closeInputStream(void); // đóng kênh vào

#endif
