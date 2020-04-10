#include <algorithm>
#include <iostream>
#include "bmp.h"

/* STATIC DEFINITIONS ********************************************************/

static const int REQUIRED_INPUT_ARGS = 11;
static const int NUM_COLOR_CHARS     = 6;

/* CLASS IMPLEMENTATION ******************************************************/

int main(int const argc, char* argv[])
{
  
  if (argc < REQUIRED_INPUT_ARGS)
  {
    std::cout << "Usage: --input (-i) PATH_TO_INPUT_FILE --output (-o) PATH_TO_OUTPUT_FILE "
                 "--black (-b) BLACK_REPLACEMENT --white (-w) WHITE_REPLACEMENT "
                 "--flip (-f) FLIP_DIRECTION" << std::endl;
  }
  else
  {
    std::string inputPath;
	std::string outputPath;
	std::string blackReplace;
	std::string whiteReplace;
	std::string flipSetting;

    for (int i = 1; i < argc; ++i)
    {
      std::string const arg = argv[i];

      if (!(arg.compare("-i")) || !(arg.compare("--input")))
      {
        if (i + 1 < argc)
        {
          inputPath = argv[i + 1];
          std::cout << "Input image path: " << inputPath << std::endl;
          ++i;
        }
        else
        {
          std::cout << "An input file is required!" << std::endl;
        }
      }
	  else if (!(arg.compare("-o")) || !(arg.compare("--output")))
      {
        if (i + 1 < argc)
        {
          outputPath = argv[i + 1];
          std::cout << "Output image path: " << outputPath << std::endl;
          ++i;
        }
        else
        {
          std::cout << "A path for output file is required!" << std::endl;
        }
      }
	  else if (!(arg.compare("-b")) || !(arg.compare("--black")))
      {
        if (i + 1 < argc)
        {
          blackReplace = argv[i + 1];
		  std::transform(blackReplace.begin(), blackReplace.end(), blackReplace.begin(), [](unsigned char c){ return std::tolower(c); });
          ++i;
		  
		  if(blackReplace.size() != NUM_COLOR_CHARS)
		  {
			  std::cout << "Bad color setting for replacement of black! " << std::endl;
			  return -1;
		  }
		  else
			  std::cout << "Color to replace black color: " << blackReplace << std::endl;
			  
        }
        else
        {
          std::cout << "Color to replace black is required!" << std::endl;
        }
      }
	  else if (!(arg.compare("-w")) || !(arg.compare("--white")))
      {
        if (i + 1 < argc)
        {
          whiteReplace = argv[i + 1];
		  std::transform(whiteReplace.begin(), whiteReplace.end(), whiteReplace.begin(), [](unsigned char c){ return std::tolower(c); });
          ++i;
		  
		  if(whiteReplace.size() != NUM_COLOR_CHARS)
		  {
			  std::cout << "Bad color setting for replacement of white! " << std::endl;
			  return -1;
		  }
		  else
			  std::cout << "Color to replace white color: " << whiteReplace << std::endl;
        }
        else
        {
          std::cout << "Color to replace white is required!" << std::endl;
        }
      }
	  else if (!(arg.compare("-f")) || !(arg.compare("--flip")))
      {
        if (i + 1 < argc)
        {
          flipSetting = argv[i + 1];
		  std::transform(flipSetting.begin(), flipSetting.end(), flipSetting.begin(), [](unsigned char c){ return std::tolower(c); });
          ++i;
		  
		  if(!(flipSetting.compare("v")) || !(flipSetting.compare("h")) || !(flipSetting.compare("d")))
			  std::cout << "Flip setting: " << flipSetting << std::endl;
		  else
		  {
			  std::cout << "Bad option, flip setting not supported! " << std::endl;
			  return -1;
		  }
			  
        }
        else
        {
          std::cout << "Flip setting is required!" << std::endl;
        }
      }
      else
      {
        std::cout << "Bad option not supported! " << std::endl;
		return -1;
      }
    }
	
	BMP bmp(inputPath.data());
	
	if(flipSetting[0] == 'v')
		bmp.verticalFlip();
	else if (flipSetting[0] == 'h')
		bmp.horizontalFlip();
	else if (flipSetting[0] == 'd')
		bmp.diagonalFlip();

	uint8_t Bb = (std::stoul(blackReplace, nullptr, 16)) >> 16;
	uint8_t Gb = ((std::stoul(blackReplace, nullptr, 16)) >> 8) & 255;
	uint8_t Rb = (std::stoul(blackReplace, nullptr, 16)) & 255;
	
	uint8_t Bw = (std::stoul(whiteReplace, nullptr, 16)) >> 16;
	uint8_t Gw = ((std::stoul(whiteReplace, nullptr, 16)) >> 8) & 255;
	uint8_t Rw = (std::stoul(whiteReplace, nullptr, 16)) & 255;
	
	bmp.changeColors(Bb, Gb, Rb, Bw, Gw, Rw);
	
	bmp.write(outputPath.data());
	
  }

  return 0;
}