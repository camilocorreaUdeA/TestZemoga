#include <iostream>
#include "BMP.h"

static const char* testImage = "../img/img_for_tests.bmp";

void testBmpImageIsFlippedVertically()
{
    BMP image(testImage);
    std::vector<uint8_t> expectedValuesAfterFlip(9,255);
    std::vector<uint8_t> currentValuesBeforeFlip(9, 0);
    std::vector<uint8_t> currentImageData;

    std::cout << "Running test 1: Vertical flipping operation..." << std::endl;

    std::copy(image.data.begin(), image.data.begin()+9, back_inserter(currentImageData));

    if(currentImageData == currentValuesBeforeFlip)
    {
        currentImageData.clear();
        image.verticalFlip();
        std::copy(image.data.begin()+3, image.data.begin()+12, back_inserter(currentImageData));
        if(currentImageData == expectedValuesAfterFlip)
            std::cout << "Vertical flipping operation = Passed" << std::endl;
        else
            std::cout << "Vertical flipping operation = Failed" << std::endl;
    } else
        std::cout << "Check input file before attempting this test" << std::endl;
}

void testBmpImageIsFlippedHorizontally()
{
    BMP image(testImage);
    std::vector<uint8_t> expectedValuesAfterFlip(9,255);
    std::vector<uint8_t> currentValuesBeforeFlip(9, 0);
    std::vector<uint8_t> currentImageData;

    std::cout << "Running test 2: Horizontal flipping operation..." << std::endl;

    std::copy(image.data.begin(), image.data.begin()+9, back_inserter(currentImageData));

    if(currentImageData == currentValuesBeforeFlip)
    {
        currentImageData.clear();
        image.horizontalFlip();
        std::copy(image.data.begin()+3, image.data.begin()+12, back_inserter(currentImageData));
        if(currentImageData == expectedValuesAfterFlip)
            std::cout << "Horizontal flipping operation = Passed" << std::endl;
        else
            std::cout << "Horizontal flipping operation = Failed" << std::endl;
    } else
        std::cout << "Check input file before attempting this test" << std::endl;
}

void testBmpImageIsFlippedDiagonally()
{
    BMP image(testImage);
    std::vector<uint8_t> expectedValuesAfterFlip(9,255);
    std::vector<uint8_t> currentValuesBeforeFlip(9, 0);
    std::vector<uint8_t> currentImageData;

    std::cout << "Running test 3: Diagonal flipping operation..." << std::endl;

    std::copy(image.data.begin(), image.data.begin()+9, back_inserter(currentImageData));

    if(currentImageData == currentValuesBeforeFlip)
    {
        currentImageData.clear();
        image.diagonalFlip();
        std::copy(image.data.begin()+3, image.data.begin()+12, back_inserter(currentImageData));
        if(currentImageData == expectedValuesAfterFlip)
            std::cout << "Diagonal flipping operation = Passed" << std::endl;
        else
            std::cout << "Diagonal flipping operation = Failed" << std::endl;
    } else
        std::cout << "Check input file before attempting this test" << std::endl;
}

void testBmpImageColorsReplaced()
{
    BMP image(testImage);
    std::vector<uint8_t> expectedValuesAfterColorsB{0,255,204,0,255,204,0,255,204};
    std::vector<uint8_t> expectedValuesAfterColorsW{85,85,85,85,85,85,85,85,85};
    std::vector<uint8_t> currentValuesBeforeColorsB(9, 0);
    std::vector<uint8_t> currentValuesBeforeColorsW(9, 255);
    std::vector<uint8_t> currentImageData;

    std::cout << "Running test 4: Color replacement operation..." << std::endl;

    std::copy(image.data.begin(), image.data.begin()+9, back_inserter(currentImageData));

    if(currentImageData == currentValuesBeforeColorsB)
    {
        currentImageData.clear();
        std::copy(image.data.begin()+250, image.data.begin()+259, back_inserter(currentImageData));
        if(currentImageData == currentValuesBeforeColorsW) {

            currentImageData.clear();
            image.changeColors(0, 255, 204, 85, 85, 85);
            std::copy(image.data.begin()+3, image.data.begin() + 12, back_inserter(currentImageData));

            if (currentImageData == expectedValuesAfterColorsB)
            {
                std::cout << "Black color replacement operation = Passed" << std::endl;
                currentImageData.clear();
                std::copy(image.data.begin()+250, image.data.begin()+259, back_inserter(currentImageData));

                if (currentImageData == expectedValuesAfterColorsW)
                    std::cout << "White color replacement operation = Passed" << std::endl;
                else
                    std::cout << "White color replacement operation = Failed" << std::endl;
            }
            else
                std::cout << "Black color replacement operation = Failed" << std::endl;
        } else
            std::cout << "Check input file before attempting this test" << std::endl;
    } else
        std::cout << "Check input file before attempting this test" << std::endl;
}

int main() {

    testBmpImageIsFlippedVertically();
    testBmpImageIsFlippedHorizontally();
    testBmpImageIsFlippedDiagonally();
    testBmpImageColorsReplaced();
}
