# TestZemoga

## This repository includes the solution to the C++ test Binary Image Flipping

Along with this README file you can find:

<ol>
  <li>TestZemogaApp_: A repository of the C++ project with the implementation of the solution to the test.</li>
  <li>TestZemogaTests_: A repository including the unit tests for the functions implemented in TestZemogaApp_</li>
</ol>

## Tools required for cross-compiation of the aforementioned projects:

<ul>
  <li>CMake - Any version able to support modern C++ standards (11, 14, 17)</li>
  <li>Ninja - a small build system with a focus on speed</li>
  <li>GCC - GNU C/C++ compiler, any version able to support modern C++ standards (11, 14, 17)</li>
</ul>
  
 ## Steps for cross-compilation:
 
 <ol>
  <li>Clone this repository and save it in a folder of your preference</li>
  <li>Navigate to TestZemogaApp_ directory and add a new directory, call it "build"</li>
  <li>Enter build directory and execute the following CMake command to compile the project:
    <br>cmake.exe -DCMAKE_BUILD_TYPE=Debug -DCXX_STAND=14 -G Ninja .. </li>
  <li>Build the app solution using the following command:
    <br>cmake.exe --build . --target all -- -j 2 </li>
  <li>Navigate to TestZemogaTests_ directory and add a new directory, call it "build"</li>
  <li>Enter build directory and execute the following CMake command to compile the project:
    <br>cmake.exe -DCMAKE_BUILD_TYPE=Debug -G Ninja .. </li>
  <li>Build the test solution using the following command:
    <br>cmake.exe --build . --target all -- -j 2 </li>
 </ol>
 
 ## Running the app and test solutions:
 
 <ol>
  <li>Navigate to TestZemogaApp_/build/app directory and run TestZemogaConsoleApp executable file with the appropiate options:
    <br>--input (-i) path to input image file
    <br>--output (-o) path to output image file
    <br>--black (-b) RGB color code to replace black pixels
    <br>--white (-w) RGB color code to replace white pixels
    <br>--flip (-f) flipping direction (v , h, d) </li>
  <li>Navigate to TestZemogaTests_/build directory and run TestZemogaTests executable file (no options required)</li>
  
  ## Implementation details:
  
  <ul>
  <li>Initial assumptions:
  <ul>
  <li>As per the black and white options format (colors in 24 bits) and the image style itself, it is assumed this application is aimed at working with bmp image files with a depth of 3 (RGB without transparency) and black and white input images exclusively.</li>
  <li>According to the latter, the binary format of bmp files is assumed to include: BMPFileHeader + BMPInfoHeader + image data bitmap</li>
  <li>BMPFileHeader (size is 14 bytes) contains the following relevant image details:
    <br>Image type (BMP)
    <br>Image size (Headers + image data) </li>
  <li>BMPInfoHeader (size is 40 bytes) contains the following relevant image details:
    <br>Width of bitmap in pixels
    <br>Height of bitmap in pixels
    <br>Image depth, number of bits per pixel
    <br>Image size (Headers + image data) </li>
  <li>Image data bitmap (arbitrary size (width * height * depth)) is a stream of bits ordered using little endian: Blue, Green and Red. Every pixel color is described by 24 bits where the first octect corresponds to Blue color, the second one to Green color and the last one to Red color.</li>
  <li>Any operation or transformation is implemented and applied according to the graphical description in the requirement.</li>
</ul>
    </li>
  <li>Input file reading:
    <br>The image file is read as a binary input stream from the image absolute path
    <br>The BMPFileHeader is read and stored in a struct (contains members according to header fields), and the file type is checked.
    <br>The BMPInfoHeader is read and stored in a struct (contains members according to header fields), image depth is checked.
    <br>The image data bitmap is stored in a vector, the storage process takes on count image padding on input file.
    </li>
  <li>Output file writing:
    <br>A binary output binary stream is prepared to write the output image in the absolute output path
    <br>Structs containing input file headers are witten as they are because the output image must keep the original dimensions
    <br>The processed imaged data bitmap is appended to the headers in the output stream, , the writing process takes on count image padding when nedded.</li>
  <li>Vertical flipping operation:
    <br>As understood from the graphical description in the requirement, the vertical flipping issues a swapping operation between rows of pixels located at opposite positions in the image, then obtaining a reflection of the whole image around a horizontal axis in the middle of the image. For instance, pixels in the first row exchange their position with the pixels in the last row, then pixels in the second row exchange positions with pixels in the row before the last row, and so on until the image is completely flipped. To implement this operation the function std::swap_ranges was used to swap the contents of the rows in the image. Vector iterators where used to point the positions of the rows to be exchanged, the process is executed until the iterators point to the same row. </li>
  <li>Horizontal flipping operation:
    <br>As understood from the graphical description in the requirement, the horizontal flipping reflects the image around a vertical axis in the middle of the image. Then the solution is just to visit the image pixels row by row and reverse the order of the pixels in each row. For instance, in the first row the first pixel takes the place of the last pixel and viceversa, and so on for all rows of pixels in the image. Image padding does not affect this operation because the pading bits are just moved from one end in the image to the other one. To implement this operation the function std::reverse was used iteratively to visit all pixel rows in the image and then reverse the order of the pixels in the specific row.</li>
  <li>Diagonal flip operation:
    <br>As understood from the graphical description in the requirement, the diagonal flipping reflects the image around a diagonal axis in the image (either from left-top corner to right-bottom corner or from right-top corner to leftt-bottom corner). For this particular case, the assumption of input images in black and white colors exclusively is of extreme importance because the implementation is just to reverse the order of all the pixels in the bitmap then the first pixel becomes the last pixel and the last pixel becomes the first and so on. This operation would probably fail for colorful images because the endian-ness of the color octects is reversed as well (BGR -> RGB). To implement this operation the function std::reverse is applied to the vector that contains the image data bitmap.</li>
  <li>Color change operation:
    <br>As understood from the graphical description in the requirement, black and white colors in the pixels are changed for the colors described in the input options. The color change operation visits all 3-octect pixels in the image, checks whether the pixel is black or white according to a pre-fixed color threshold. Then the blue, green and red octects values are changed to the new values captured in the input options. In images without an abrupt contrast between black and white pixels, say grey-scale images, this color change operation tends to shrink white regions while stretching black regions.</li>
</ul>

## Links to information and code examples consulted:

https://solarianprogrammer.com/2018/11/19/cpp-reading-writing-bmp-images/
https://stackoverflow.com/questions/2654480/writing-bmp-image-in-pure-c-c-without-other-libraries
https://web.archive.org/web/20080912171714/http://www.fortunecity.com/skyscraper/windows/364/bmpffrmt.html
https://stackoverflow.com/questions/22112412/reading-bmp-file-c-trouble-with-reading-bmp-header?rq=1
https://en.wikipedia.org/wiki/BMP_file_format
http://www.cplusplus.com/reference/istream/istream/seekg/
http://www.cplusplus.com/reference/algorithm/swap_ranges/
http://www.cplusplus.com/reference/algorithm/reverse/
http://www.cplusplus.com/reference/cstdlib/strtoul/
http://www.cplusplus.com/articles/DEN36Up4/


  
  
  
