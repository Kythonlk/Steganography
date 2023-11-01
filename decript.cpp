#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <iostream>
#include <string>
#include <vector>

int main() {
    const char* inputFileName = "./output_image2.jpg";  // Modified image with hidden text

    // Read the image using stb_image
    int width, height, numChannels;
    unsigned char* image = stbi_load(inputFileName, &width, &height, &numChannels, 0);
    if (!image) {
        std::cerr << "Error: Unable to open the image file." << std::endl;
        return 1;
    }

    int textIndex = 0;
    char currentChar = 0;
    std::string extractedText;

    // Extract the hidden text from the image (LSB steganography)
    for (int i = 0; i < width * height; ++i) {
        for (int channel = 0; channel < numChannels; ++channel) {
            if (textIndex % 8 == 0) {
                if (currentChar == '\0') {
                    // End of the text (null terminator)
                    break;
                }
                extractedText += currentChar;
                currentChar = 0;
            }
            currentChar |= (image[i * numChannels + channel] & 1) << (textIndex % 8);
            textIndex++;
        }
    }

    // Display the extracted text
    std::cout << "Extracted text: " << extractedText << std::endl;

    // Free memory
    stbi_image_free(image);

    return 0;
}
