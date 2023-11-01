#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include "stb_image.h"
#include "stb_image_write.h"
#include <iostream>
#include <vector>

int main() {
    const char* wordToHide = "Hello";
    const char* imageFileName = "./test.jpg";
    const char* outputFileName = "./output_image.jpg";

    // Read the image using stb_image
    int width, height, numChannels;
    unsigned char* image = stbi_load(imageFileName, &width, &height, &numChannels, 0);
    if (!image) {
        std::cerr << "Error: Unable to open the image file." << std::endl;
        return 1;
    }

    // Embed the word into the image (LSB steganography)
    int textIndex = 0;
    for (int i = 0; i < width * height; ++i) {
        for (int channel = 0; channel < numChannels; ++channel) {
            if (textIndex < strlen(wordToHide)) {
                image[i * numChannels + channel] = (image[i * numChannels + channel] & 0xFE) | ((wordToHide[textIndex] >> 7) & 1);
                textIndex++;
            }
        }
    }

    // Save the modified image
    stbi_write_jpg(outputFileName, width, height, numChannels, image, 100);

    std::cout << "Word hidden in the image and saved as " << outputFileName << std::endl;

    // Free memory
    stbi_image_free(image);

    return 0;
}
