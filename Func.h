#pragma once
#include <iostream>

unsigned short read_u16(FILE* fp);
unsigned int read_u32(FILE* fp);
int read_s32(FILE* fp);

void write_u16(unsigned short input, FILE* fp);
void write_u32(unsigned int input, FILE* fp);
void write_s32(int input, FILE* fp);