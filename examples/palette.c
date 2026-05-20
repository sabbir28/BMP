#include <BMP/BMP.h>
#include <stdio.h>
#include <stdlib.h>

int main() {
    printf("Creating an 8-bit indexed BMP with custom palette...\n");
    int width = 256, height = 256;
    BMPImage* img = BMP_Create(width, height, 8);
    if (!img) {
        printf("Failed to create image!\n");
        return 1;
    }

    // Initialize palette with a blue-to-red gradient
    for (int i = 0; i < 256; i++) {
        img->palette[i].red = (uint8_t)i;
        img->palette[i].green = 0;
        img->palette[i].blue = (uint8_t)(255 - i);
        img->palette[i].reserved = 0;
    }

    // Fill data with vertical stripes using palette indices
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            BMPColor pixel;
            pixel.blue = (uint8_t)x; // In 8-bit, blue field is used as index in SetPixel helper
            BMP_SetPixel(img, x, y, pixel);
        }
    }

    if (BMP_Write("palette_demo.bmp", img)) {
        printf("Successfully wrote palette_demo.bmp\n");
    } else {
        printf("Failed to write palette_demo.bmp\n");
    }

    BMP_Destroy(img);
    return 0;
}
