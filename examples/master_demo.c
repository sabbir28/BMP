#include <BMP/BMP.h>
#include <BMP/Version.h>
#include <stdio.h>
#include <stdlib.h>

void print_banner() {
    printf("========================================\n");
    printf("   %s\n", BMP_PRODUCT_NAME);
    printf("   Version: %s\n", BMP_VERSION_STRING);
    printf("   Company: %s\n", BMP_COMPANY_NAME);
    printf("   %s\n", BMP_COPYRIGHT);
    printf("========================================\n\n");
}

void demo_24bit() {
    printf("[*] Stage 1: Creating 24-bit Color Image\n");
    BMPImage* img = BMP_Create(300, 300, 24);
    if (!img) return;

    // Create a color pattern
    for (int y = 0; y < 300; y++) {
        for (int x = 0; x < 300; x++) {
            BMPColor c = { (uint8_t)x, (uint8_t)y, (uint8_t)((x+y)/2), 0 };
            BMP_SetPixel(img, x, y, c);
        }
    }

    printf("[+] Printing Header Details:\n");
    BMP_PrintHeaderDetails(img);

    printf("\n[*] Transfoming to Grayscale...\n");
    BMP_ToGrayscale(img);

    printf("[*] Saving as 'master_grayscale.bmp'...\n");
    BMP_Write("master_grayscale.bmp", img);
    
    BMP_Destroy(img);
    printf("[+] 24-bit Demo Complete.\n\n");
}

void demo_8bit() {
    printf("[*] Stage 2: Creating 8-bit Indexed Image\n");
    BMPImage* img = BMP_Create(256, 256, 8);
    if (!img) return;

    // Create a simple grayscale palette
    for (int i = 0; i < 256; i++) {
        img->palette[i].red = img->palette[i].green = img->palette[i].blue = (uint8_t)i;
        img->palette[i].reserved = 0;
    }

    // Fill with a horizontal gradient
    for (int y = 0; y < 256; y++) {
        for (int x = 0; x < 256; x++) {
            BMPColor c = { (uint8_t)x, 0, 0, 0 }; // blue field used as index
            BMP_SetPixel(img, x, y, c);
        }
    }

    printf("[+] Printing Programmatic Header Data (Manual Access):\n");
    BMPFileHeader fh = BMP_GetFileHeader(img);
    BMPInfoHeader ih = BMP_GetInfoHeader(img);
    printf("    File Size: %u bytes\n", fh.bfSize);
    printf("    Image Dimensions: %dx%d\n", ih.biWidth, ih.biHeight);
    printf("    Bit Depth: %d bpp\n", ih.biBitCount);

    printf("[*] Saving as 'master_indexed.bmp'...\n");
    BMP_Write("master_indexed.bmp", img);

    BMP_Destroy(img);
    printf("[+] 8-bit Demo Complete.\n\n");
}

int main() {
    print_banner();
    demo_24bit();
    demo_8bit();
    printf("All demonstrations completed successfully.\n");
    return 0;
}
