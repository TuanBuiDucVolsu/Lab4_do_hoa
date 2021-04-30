#define _CRT_SECURE_NO_WARNINGS
#include "Func.h"
#include "Struct.h"
#include <codecvt>
#include <Windows.h>
#include <fstream>
#include <glut.h>
int main() {

    std::wcout.imbue(std::locale(std::locale::empty(), new std::codecvt_utf8<wchar_t>));
    SetConsoleCP(65001);
    SetConsoleOutputCP(65001);
    
    int bRed = 10, bGreen = 10, bBlue = 10, bReserved = 10; 
	int Q1 = 10, Q2 = 20;
	double gamma = 10;
    int pNegative = 10, pBinarization = 10;

    int H_Red[256]{ 0 }, H_Green[256]{ 0 }, H_Blue[256]{ 0 }, H_Alpha[256]{ 0 };

    FILE* inFile = fopen("Butterfly.bmp", "rb");
    FILE* outFileBW = fopen("Butterfly_BW.bmp", "wb");
    FILE* outFileLightness = fopen("Butterfly_Lightness.bmp", "wb");
    FILE* outFileNegative = fopen("Butterfly_Negative.bmp", "wb");
    FILE* outFileBinarization = fopen("Butterfly_Binarization.bmp", "wb");
    FILE* outFileIntensityStretching = fopen("Butterfly_IntensityStrtching.bmp", "wb");
    FILE* outFileContrast = fopen("Butterfly_Contrast.bmp", "wb");
    FILE* outFileColorGamma = fopen("Butterfly_colorGamma.bmp", "wb");
    std::ofstream outFileH_Red("HistogrammaR.txt");
    std::ofstream outFileH_Green("HistogrammaG.txt");
    std::ofstream outFileH_Blue("HistogrammaB.txt");
    std::ofstream outFileH_Alpha("HistogrammaA.txt");

    BITMAPFILEHEADER_STRUCT header;

    header.bfType = read_u16(inFile);
    header.bfSize = read_u32(inFile);
    header.bfReserved1 = read_u16(inFile);
    header.bfReserved2 = read_u16(inFile);
    header.bfOffBits = read_u32(inFile);

    BITMAPINFOHEADER_STRUCT bmiHeader;

    bmiHeader.biSize = read_u32(inFile);
    bmiHeader.biWidth = read_s32(inFile);
    bmiHeader.biHeight = read_s32(inFile);
    bmiHeader.biPlanes = read_u16(inFile);
    bmiHeader.biBitCount = read_u16(inFile);
    bmiHeader.biCompression = read_u32(inFile);
    bmiHeader.biSizeImage = read_u32(inFile);
    bmiHeader.biXPelsPerMeter = read_s32(inFile);
    bmiHeader.biYPelsPerMeter = read_s32(inFile);
    bmiHeader.biClrUsed = read_u32(inFile);
    bmiHeader.biClrImportant = read_u32(inFile);

    std::wcout << L"Image density - " << bmiHeader.biBitCount << std::endl;  //Плотность изображения

    RGBQUAD_STRUCT* pixels = new RGBQUAD_STRUCT[bmiHeader.biHeight * bmiHeader.biWidth];

    // Читать пиксели
    for (int i = 0; i < bmiHeader.biHeight * bmiHeader.biWidth; i++) {
        pixels[i].rgbBlue = getc(inFile);
        H_Blue[pixels[i].rgbBlue]++;
       
        pixels[i].rgbGreen = getc(inFile);
        H_Green[pixels[i].rgbGreen]++;
        
        pixels[i].rgbRed = getc(inFile);
        H_Red[pixels[i].rgbRed]++;
        if (bmiHeader.biBitCount == 32) {

            pixels[i].rgbReserved = getc(inFile);
            H_Alpha[pixels[i].rgbReserved]++;
        }
    }
    fclose(inFile);

    outFileH_Red << "ÃÈÑÒÎÃÐÀÌÌÀ ÊÐÀÑÍÎÃÎ ÖÂÅÒÀ\n\n";
    outFileH_Green << "ÃÈÑÒÎÃÐÀÌÌÀ ÇÅËÅÍÎÃÎ ÖÂÅÒÀ\n\n";
    outFileH_Blue << "ÃÈÑÒÎÃÐÀÌÌÀ ÑÈÍÅÃÎ ÖÂÅÒÀ\n\n";
    outFileH_Alpha << "ÃÈÑÒÎÃÐÀÌÌÀ ÀËÜÔÀ ÊÀÍÀËÀ\n\n";

    for (int i = 0; i < 256; ++i) {

        outFileH_Red << i << " - " << H_Red[i] << std::endl;
        outFileH_Green << i << " - " << H_Green[i] << std::endl;
        outFileH_Blue << i << " - " << H_Blue[i] << std::endl;
        outFileH_Alpha << i << " - " << H_Alpha[i] << std::endl;
    }
    outFileH_Red.close();
    outFileH_Green.close();
    outFileH_Blue.close();
    outFileH_Alpha.close();

    std::wcout << L"Enter the Bone Coefficients (Integer - Bore Measurement): \n\n R - red channel (integer) > ";
    std::cin >> bRed; std::cin.get();
    std::wcout << L"\n G - light channel (whole number) > ";
    std::cin >> bGreen; std::cin.get();
    std::wcout << L"\n B - blue channel (integer) > ";
    std::cin >> bBlue; std::cin.get();
    std::wcout << L"\n A - alufa-kanal (whole number) > ";
    std::cin >> bReserved; std::cin.get();

    std::wcout << L"\nEnter the adjusting efficiency (Q2 > Q1): \n\n Q1 ( from 0 to 255) > ";
    std::cin >> Q1; std::cin.get();
    std::wcout << L"\n Q2 (from 0 to 255) > ";
    std::cin >> Q2; std::cin.get();
    std::wcout << L"\n gamma (general) > ";
    std::cin >> gamma; std::cin.get();

    std::wcout << L"\nEnter binarization efficiency (from 0 to 255): \n> ";
    std::cin >> pBinarization; std::cin.get();
    std::wcout << L"\nEnter the negative image efficiency (from 0 to 255):  \n> ";
    std::cin >> pNegative; std::cin.get();

    // BITMAPFILEHEADER
    {
    write_u16(header.bfType, outFileBW);
    write_u32(header.bfSize, outFileBW);
    write_u16(header.bfReserved1, outFileBW);
    write_u16(header.bfReserved2, outFileBW);
    write_u32(header.bfOffBits, outFileBW);

    write_u16(header.bfType, outFileLightness);
    write_u32(header.bfSize, outFileLightness);
    write_u16(header.bfReserved1, outFileLightness);
    write_u16(header.bfReserved2, outFileLightness);
    write_u32(header.bfOffBits, outFileLightness);

    write_u16(header.bfType, outFileNegative);
    write_u32(header.bfSize, outFileNegative);
    write_u16(header.bfReserved1, outFileNegative);
    write_u16(header.bfReserved2, outFileNegative);
    write_u32(header.bfOffBits, outFileNegative);

    write_u16(header.bfType, outFileBinarization);
    write_u32(header.bfSize, outFileBinarization);
    write_u16(header.bfReserved1, outFileBinarization);
    write_u16(header.bfReserved2, outFileBinarization);
    write_u32(header.bfOffBits, outFileBinarization);

    write_u16(header.bfType, outFileIntensityStretching);
    write_u32(header.bfSize, outFileIntensityStretching);
    write_u16(header.bfReserved1, outFileIntensityStretching);
    write_u16(header.bfReserved2, outFileIntensityStretching);
    write_u32(header.bfOffBits, outFileIntensityStretching);

    write_u16(header.bfType, outFileContrast);
    write_u32(header.bfSize, outFileContrast);
    write_u16(header.bfReserved1, outFileContrast);
    write_u16(header.bfReserved2, outFileContrast);
    write_u32(header.bfOffBits, outFileContrast);

    write_u16(header.bfType, outFileColorGamma);
    write_u32(header.bfSize, outFileColorGamma);
    write_u16(header.bfReserved1, outFileColorGamma);
    write_u16(header.bfReserved2, outFileColorGamma);
    write_u32(header.bfOffBits, outFileColorGamma);
    }
    // BITMAPINFOHEADER
    {
        write_u32(bmiHeader.biSize, outFileBW);
        write_s32(bmiHeader.biWidth, outFileBW);
        write_s32(bmiHeader.biHeight, outFileBW);
        write_u16(bmiHeader.biPlanes, outFileBW);
        write_u16(bmiHeader.biBitCount, outFileBW);
        write_u32(bmiHeader.biCompression, outFileBW);
        write_u32(bmiHeader.biSizeImage, outFileBW);
        write_s32(bmiHeader.biXPelsPerMeter, outFileBW);
        write_s32(bmiHeader.biYPelsPerMeter, outFileBW);
        write_u32(bmiHeader.biClrUsed, outFileBW);
        write_u32(bmiHeader.biClrImportant, outFileBW);

        write_u32(bmiHeader.biSize, outFileLightness);
        write_s32(bmiHeader.biWidth, outFileLightness);
        write_s32(bmiHeader.biHeight, outFileLightness);
        write_u16(bmiHeader.biPlanes, outFileLightness);
        write_u16(bmiHeader.biBitCount, outFileLightness);
        write_u32(bmiHeader.biCompression, outFileLightness);
        write_u32(bmiHeader.biSizeImage, outFileLightness);
        write_s32(bmiHeader.biXPelsPerMeter, outFileLightness);
        write_s32(bmiHeader.biYPelsPerMeter, outFileLightness);
        write_u32(bmiHeader.biClrUsed, outFileLightness);
        write_u32(bmiHeader.biClrImportant, outFileLightness);

        write_u32(bmiHeader.biSize, outFileNegative);
        write_s32(bmiHeader.biWidth, outFileNegative);
        write_s32(bmiHeader.biHeight, outFileNegative);
        write_u16(bmiHeader.biPlanes, outFileNegative);
        write_u16(bmiHeader.biBitCount, outFileNegative);
        write_u32(bmiHeader.biCompression, outFileNegative);
        write_u32(bmiHeader.biSizeImage, outFileNegative);
        write_s32(bmiHeader.biXPelsPerMeter, outFileNegative);
        write_s32(bmiHeader.biYPelsPerMeter, outFileNegative);
        write_u32(bmiHeader.biClrUsed, outFileNegative);
        write_u32(bmiHeader.biClrImportant, outFileNegative);

        write_u32(bmiHeader.biSize, outFileBinarization);
        write_s32(bmiHeader.biWidth, outFileBinarization);
        write_s32(bmiHeader.biHeight, outFileBinarization);
        write_u16(bmiHeader.biPlanes, outFileBinarization);
        write_u16(bmiHeader.biBitCount, outFileBinarization);
        write_u32(bmiHeader.biCompression, outFileBinarization);
        write_u32(bmiHeader.biSizeImage, outFileBinarization);
        write_s32(bmiHeader.biXPelsPerMeter, outFileBinarization);
        write_s32(bmiHeader.biYPelsPerMeter, outFileBinarization);
        write_u32(bmiHeader.biClrUsed, outFileBinarization);
        write_u32(bmiHeader.biClrImportant, outFileBinarization);

        write_u32(bmiHeader.biSize, outFileIntensityStretching);
        write_s32(bmiHeader.biWidth, outFileIntensityStretching);
        write_s32(bmiHeader.biHeight, outFileIntensityStretching);
        write_u16(bmiHeader.biPlanes, outFileIntensityStretching);
        write_u16(bmiHeader.biBitCount, outFileIntensityStretching);
        write_u32(bmiHeader.biCompression, outFileIntensityStretching);
        write_u32(bmiHeader.biSizeImage, outFileIntensityStretching);
        write_s32(bmiHeader.biXPelsPerMeter, outFileIntensityStretching);
        write_s32(bmiHeader.biYPelsPerMeter, outFileIntensityStretching);
        write_u32(bmiHeader.biClrUsed, outFileIntensityStretching);
        write_u32(bmiHeader.biClrImportant, outFileIntensityStretching);

        write_u32(bmiHeader.biSize, outFileContrast);
        write_s32(bmiHeader.biWidth, outFileContrast);
        write_s32(bmiHeader.biHeight, outFileContrast);
        write_u16(bmiHeader.biPlanes, outFileContrast);
        write_u16(bmiHeader.biBitCount, outFileContrast);
        write_u32(bmiHeader.biCompression, outFileContrast);
        write_u32(bmiHeader.biSizeImage, outFileContrast);
        write_s32(bmiHeader.biXPelsPerMeter, outFileContrast);
        write_s32(bmiHeader.biYPelsPerMeter, outFileContrast);
        write_u32(bmiHeader.biClrUsed, outFileContrast);
        write_u32(bmiHeader.biClrImportant, outFileContrast);

        write_u32(bmiHeader.biSize, outFileColorGamma);
        write_s32(bmiHeader.biWidth, outFileColorGamma);
        write_s32(bmiHeader.biHeight, outFileColorGamma);
        write_u16(bmiHeader.biPlanes, outFileColorGamma);
        write_u16(bmiHeader.biBitCount, outFileColorGamma);
        write_u32(bmiHeader.biCompression, outFileColorGamma);
        write_u32(bmiHeader.biSizeImage, outFileColorGamma);
        write_s32(bmiHeader.biXPelsPerMeter, outFileColorGamma);
        write_s32(bmiHeader.biYPelsPerMeter, outFileColorGamma);
        write_u32(bmiHeader.biClrUsed, outFileColorGamma);
        write_u32(bmiHeader.biClrImportant, outFileColorGamma);
    }
    
    // Changing the Speed
    for (int i = 0; i < bmiHeader.biHeight * bmiHeader.biWidth; i++) {

        if (pixels[i].rgbBlue + bBlue < 0) {
            putc(0, outFileLightness);
        }
        else if (pixels[i].rgbBlue > 255) {
            putc(255, outFileLightness);
        }
        else {
            putc(pixels[i].rgbBlue + bBlue, outFileLightness);
        }

        if (pixels[i].rgbGreen + bGreen < 0) {
            putc(0, outFileLightness);
        }
        else if (pixels[i].rgbGreen > 255) {
            putc(255, outFileLightness);
        }
        else {
            putc(pixels[i].rgbGreen + bGreen, outFileLightness);
        }

        if (pixels[i].rgbRed + bRed < 0) {
            putc(0, outFileLightness);
        }
        else if (pixels[i].rgbRed > 255) {
            putc(255, outFileLightness);
        }
        else {
            putc(pixels[i].rgbRed + bRed, outFileLightness);
        }
       
       if (bmiHeader.biBitCount == 32) {
        
           if (pixels[i].rgbReserved + bReserved < 0) {
               putc(0, outFileLightness);
           }
           else if (pixels[i].rgbReserved > 255) {
               putc(255, outFileLightness);
           }
           else {
               putc(pixels[i].rgbReserved + bReserved, outFileLightness);
           }  
       }
    }
    fclose(outFileLightness);

    // inactive imaging
    for (int i = 0; i < bmiHeader.biHeight * bmiHeader.biWidth; i++) {

        if (255 - pixels[i].rgbBlue >= pNegative) {
            putc(255 - pixels[i].rgbBlue, outFileNegative);
        }
        else {
            putc(pixels[i].rgbBlue, outFileNegative);
        }

        if (255 - pixels[i].rgbGreen >= pNegative) {
            putc(255 - pixels[i].rgbGreen, outFileNegative);
        }
        else {
            putc(pixels[i].rgbGreen, outFileNegative);
        }

        if (255 - pixels[i].rgbRed >= pNegative) {
            putc(255 - pixels[i].rgbRed, outFileNegative);
        }
        else {
            putc(pixels[i].rgbRed, outFileNegative);
        }

        if (bmiHeader.biBitCount == 32) {

            if (255 - pixels[i].rgbReserved >= pNegative) {
                putc(255 - pixels[i].rgbReserved, outFileNegative);
            }
            else {
                putc(pixels[i].rgbReserved, outFileNegative);
            }
        }
    }
    fclose(outFileNegative);

    // binarization
    for (int i = 0; i < bmiHeader.biHeight * bmiHeader.biWidth; i++) {

		if (0.3 * pixels[i].rgbRed + 0.59 * pixels[i].rgbGreen + 0.11 * pixels[i].rgbBlue < pBinarization) {
			putc(0, outFileBinarization);
			putc(0, outFileBinarization);
			putc(0, outFileBinarization);
		}
		else {
			putc(255, outFileBinarization);
			putc(255, outFileBinarization);
			putc(255, outFileBinarization);
		}

    }
    fclose(outFileBinarization);

    // Conversion into a half-tone image
    for (int i = 0; i < bmiHeader.biHeight * bmiHeader.biWidth; i++) {
        putc(0.3 * pixels[i].rgbRed + 0.59 * pixels[i].rgbGreen + 0.11 * pixels[i].rgbBlue, outFileBW);
        putc(0.3 * pixels[i].rgbRed + 0.59 * pixels[i].rgbGreen + 0.11 * pixels[i].rgbBlue, outFileBW);
        putc(0.3 * pixels[i].rgbRed + 0.59 * pixels[i].rgbGreen + 0.11 * pixels[i].rgbBlue, outFileBW);
    }
    fclose(outFileBW);

    // Linear scraping
    for (int i = 0; i < bmiHeader.biHeight * bmiHeader.biWidth; i++) {

		if (Q2 - Q1 == 0) {
			std::cout << "Q2 - Q1 == 0\n\n\n";
			break;
		}

        putc((pixels[i].rgbBlue - Q1) * 255 / (Q2 - Q1), outFileIntensityStretching);
        putc((pixels[i].rgbGreen - Q1) * 255 / (Q2 - Q1), outFileIntensityStretching);
        putc((pixels[i].rgbRed - Q1) * 255 / (Q2 - Q1), outFileIntensityStretching);
    }
    fclose(outFileIntensityStretching);

    // Changing the contrast
    for (int i = 0; i < bmiHeader.biHeight * bmiHeader.biWidth; i++) {

        putc(Q1 + pixels[i].rgbBlue * (Q2 - Q1) / 255, outFileContrast);
        putc(Q1 + pixels[i].rgbGreen * (Q2 - Q1) / 255, outFileContrast);
        putc(Q1 + pixels[i].rgbRed * (Q2 - Q1) / 255, outFileContrast);
    }
    fclose(outFileContrast);

    // color gamma
    for (int i = 0; i < bmiHeader.biHeight * bmiHeader.biWidth; i++) {
        putc(255 * pow(pixels[i].rgbBlue / 255.0f, gamma), outFileColorGamma);
        putc(255 * pow(pixels[i].rgbGreen / 255.0f, gamma), outFileColorGamma);
        putc(255 * pow(pixels[i].rgbRed / 255.0f, gamma), outFileColorGamma);
    }
    fclose(outFileColorGamma);

    return 0;
}