#pragma once

struct BITMAPFILEHEADER_STRUCT
{
    unsigned int    bfType;      // ��� ����� : 0x4D42
    unsigned long   bfSize;      // ������ ����� � ������
    unsigned int    bfReserved1; // ��������������, �������� ����
    unsigned int    bfReserved2; // ��������������, �������� ����
    unsigned long   bfOffBits;   // �������� �������� � ������ �� ������ ���������
                                 // BITMAPFILEHEADER �� ��������������� ����� �����������.
};

struct BITMAPINFOHEADER_STRUCT
{
    unsigned int    biSize;          // ������ ��������� � ������.
    int             biWidth;         // ������ ����������� � ��������
    int             biHeight;        // ������ ����������� � ��������
    unsigned short  biPlanes;        // ���������� ���������� � ������� �����������
    unsigned short  biBitCount;      // ��������� ���������� ��� �� �������
    unsigned int    biCompression;   // ��������� ��� ������ ��� ������ ����������
    unsigned int    biSizeImage;     // ��������� ������ ����������� � ������
    int             biXPelsPerMeter; // ��������� �������������� ���������� � �������� �� ���� ��� �������� ����������.
    int             biYPelsPerMeter; // ��������� ������������ ���������� � �������� �� ���� ��� �������� ����������.
    unsigned int    biClrUsed;       // ��������� ���������� ������������ �������� �������� � �������
    unsigned int    biClrImportant;  // ��������� ���������� ��������, ����������� ��� ����������� �����������
};

struct RGBQUAD_STRUCT
{
    int   rgbBlue;
    int   rgbGreen;
    int   rgbRed;
    int   rgbReserved;
};