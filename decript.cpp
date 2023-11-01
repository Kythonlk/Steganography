#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <iostream>
#include <string>

int main() {
    const char* imageFileName = "./output_image.jpg";  // Use the name of the modified image
    const int maxWordLength = 100;  // Maximum word length to extract

    // Read the image using stb_image
    int width, height, numChannels;
    unsigned char* image = stbi_load(imageFileName, &width, &height, &numChannels, 0);
    if (!image) {
        std::cerr << "Error: Unable to open the image file." << std::endl;
        return 1;
    }

    std::string hiddenWord;
    int textIndex = 0;
    char currentChar = 0;

    // Extract the word from the image (LSB steganography)
    for (int i = 0; i < width * height; ++i) {
        for (int channel = 0; channel < numChannels; ++channel) {
            if (textIndex < maxWordLength) {
                currentChar |= (image[i * numChannels + channel] & 1) << (7 - (textIndex % 8));
                if ((textIndex % 8) == 7) {
                    if (currentChar == '\0') {
                        // End of the word (null terminator)
                        break;
                    }
                    hiddenWord += currentChar;
                    currentChar = 0;
                }
                textIndex++;
            }
        }
    }

    // Display the extracted word
    if (!hiddenWord.empty()) {
        std::cout << "Extracted word: " << hiddenWord << std::endl;
    } else {
        std::cerr << "No word found in the image." << std::endl;
    }

    // Free memory
    stbi_image_free(image);

    return 0;
}
