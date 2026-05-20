#include <BMP/BMP.h>
#include <stdio.h>
#include <stdlib.h>

void create_demo_image() {
    printf("Creating a 400x400 24-bit demo BMP...\n");
    BMPImage* img = BMP_Create(400, 400, 24);
    if (!img) {
        printf("Failed to create image!\n");
        return;
    }

    for (int y = 0; y < 400; y++) {
        for (int x = 0; x < 400; x++) {
            BMPColor color;
            color.red = (uint8_t)(x % 256);
            color.green = (uint8_t)(y % 256);
            color.blue = (uint8_t)((x + y) % 256);
            color.reserved = 0;
            BMP_SetPixel(img, x, y, color);
        }
    }

    if (BMP_Write("demo.bmp", img)) {
        printf("Successfully wrote demo.bmp\n");
    } else {
        printf("Failed to write demo.bmp\n");
    }

    BMP_Destroy(img);
}

void read_and_verify() {
    printf("Reading back demo.bmp to verify...\n");
    BMPImage* img = BMP_Read("demo.bmp");
    if (!img) {
        printf("Failed to read image!\n");
        return;
    }

    printf("Image dimensions: %dx%d, bit count: %d\n", 
           BMP_GetWidth(img), BMP_GetHeight(img), BMP_GetBitCount(img));

    BMP_PrintHeaderDetails(img);

    BMP_Destroy(img);
}

int main() {
    create_demo_image();
    read_and_verify();
    return 0;
}
