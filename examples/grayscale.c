#include <BMP/BMP.h>
#include <stdio.h>

int main() {
    printf("Loading demo.bmp for grayscale conversion...\n");
    BMPImage* img = BMP_Read("demo.bmp");
    if (!img) {
        printf("Failed to read demo.bmp. Make sure to run the first example first!\n");
        return 1;
    }

    if (BMP_ToGrayscale(img)) {
        printf("Converted to grayscale successfully.\n");
        if (BMP_Write("demo_grayscale.bmp", img)) {
            printf("Saved grayscale image as demo_grayscale.bmp\n");
        } else {
            printf("Failed to save grayscale image.\n");
        }
    } else {
        printf("Grayscale conversion failed.\n");
    }

    BMP_Destroy(img);
    return 0;
}
