#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include "stb_image.h"
#include "stb_image_write.h"
#include <iostream>
#include <fstream>
#include <vector>

int main() {
    const char* textFileName = "./pass.txt";      // Text file to hide
    const char* imageFileName = "./test.jpg";    // Image in which to hide text
    const char* outputFileName = "output_image2.jpg";  // Output image with hidden text

    // Read the text from text.txt
    std::ifstream textFile(textFileName);
    if (!textFile.is_open()) {
        std::cerr << "Error: Unable to open the text file." << std::endl;
        return 1;
    }

    std::string text((std::istreambuf_iterator<char>(textFile)), (std::istreambuf_iterator<char>()));
    textFile.close();

    // Read the image using stb_image
    int width, height, numChannels;
    unsigned char* image = stbi_load(imageFileName, &width, &height, &numChannels, 0);
    if (!image) {
        std::cerr << "Error: Unable to open the image file." << std::endl;
        return 1;
    }

    int textIndex = 0;
    char currentChar = 0;

    // Embed the text into the image (LSB steganography)
    for (int i = 0; i < width * height; ++i) {
        for (int channel = 0; channel < numChannels; ++channel) {
            if (textIndex < text.length()) {
                currentChar = text[textIndex];
                image[i * numChannels + channel] = (image[i * numChannels + channel] & 0xFE) | ((currentChar >> 7) & 1);
                textIndex++;
            }
        }
    }

    // Save the modified image
    stbi_write_jpg(outputFileName, width, height, numChannels, image, 100);

    std::cout << "Text hidden in the image and saved as " << outputFileName << std::endl;

    // Free memory
    stbi_image_free(image);

    return 0;
}
