#pragma once

struct BITMAPFILEHEADER_STRUCT
{
    unsigned int    bfType;      // тип файла : 0x4D42
    unsigned long   bfSize;      // размер файла в байтах
    unsigned int    bfReserved1; // зарезервирован, содержит нуль
    unsigned int    bfReserved2; // зарезервирован, содержит нуль
    unsigned long   bfOffBits;   // содержит смещение в байтах от начала структуры
                                 // BITMAPFILEHEADER до непосредственно битов изображения.
};

struct BITMAPINFOHEADER_STRUCT
{
    unsigned int    biSize;          // размер структуры в байтах.
    int             biWidth;         // ширина изображения в пикселях
    int             biHeight;        // высота изображения в пикселях
    unsigned short  biPlanes;        // количество плоскостей в битовом изображении
    unsigned short  biBitCount;      // указывает количество бит на пиксель
    unsigned int    biCompression;   // указывает тип сжатия для сжатых изображени
    unsigned int    biSizeImage;     // указывает размер изображения в байтах
    int             biXPelsPerMeter; // указывает горизонтальное разрешение в пикселях на метр для целевого устройства.
    int             biYPelsPerMeter; // указывает вертикальное разрешение в пикселях на метр для целевого устройства.
    unsigned int    biClrUsed;       // указывает количество используемых цветовых индексов в палитре
    unsigned int    biClrImportant;  // указывает количество индексов, необходимых для отображения изображения
};

struct RGBQUAD_STRUCT
{
    int   rgbBlue;
    int   rgbGreen;
    int   rgbRed;
    int   rgbReserved;
};