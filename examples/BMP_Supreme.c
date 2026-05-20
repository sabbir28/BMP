#include <BMP/BMP.h>
#include <BMP/Version.h>
#include <stdio.h>
#include <stdlib.h>

void print_header(const char* title) {
    printf("\n>>> %s <<<\n", title);
}

int main() {
    printf("==================================================\n");
    printf("   %s - SUPREME DEMO\n", BMP_PRODUCT_NAME);
    printf("   Company: %s | Version: %s\n", BMP_COMPANY_NAME, BMP_VERSION_STRING);
    printf("==================================================\n\n");

    // 1. Create a 24-bit original image
    print_header("Step 1: Creating 24-bit Original");
    BMPImage* img24 = BMP_Create(500, 500, 24);
    if (img24) {
        for (int y = 0; y < 500; y++) {
            for (int x = 0; x < 500; x++) {
                BMPColor c = { (uint8_t)(x % 256), (uint8_t)(y % 256), (uint8_t)((x+y) % 256), 0 };
                BMP_SetPixel(img24, x, y, c);
            }
        }
        BMP_Write("1_original_24.bmp", img24);
        BMP_SaveHeaderDetails(img24, "1_original_24_details.txt");
        printf("[+] Saved 1_original_24.bmp and its details.\n");
    }

    // 2. Grayscale conversion
    print_header("Step 2: Grayscale Conversion");
    if (img24) {
        BMP_ToGrayscale(img24);
        BMP_Write("2_grayscale_24.bmp", img24);
        printf("[+] Saved 2_grayscale_24.bmp.\n");
        BMP_Destroy(img24);
    }

    // 3. Create an 8-bit indexed image
    print_header("Step 3: Creating 8-bit Indexed Image");
    BMPImage* img8 = BMP_Create(256, 256, 8);
    if (img8) {
        char* colors[] = {"Blue", "Green", "Red", "Yellow", "Cyan", "Magenta", "White", "Black"};
        for (int i = 0; i < 256; i++) {
            img8->palette[i].red = (uint8_t)i;
            img8->palette[i].green = (uint8_t)(255 - i);
            img8->palette[i].blue = (uint8_t)((i * 2) % 256);
            img8->palette[i].reserved = 0;
        }
        for (int y = 0; y < 256; y++) {
            for (int x = 0; x < 256; x++) {
                BMPColor c = { (uint8_t)x, 0, 0, 0 };
                BMP_SetPixel(img8, x, y, c);
            }
        }
        BMP_Write("3_indexed_8.bmp", img8);
        BMP_SaveHeaderDetails(img8, "3_indexed_8_details.txt");
        printf("[+] Saved 3_indexed_8.bmp and its details.\n");
        BMP_Destroy(img8);
    }

    printf("\n[SUCCESS] All operations completed. Check the current directory for output files.\n");
    return 0;
}
