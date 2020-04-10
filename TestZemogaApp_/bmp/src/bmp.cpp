#include "bmp.h"

/* STATIC DEFINITIONS ********************************************************/

static const uint8_t WHITE_COLOR_THRES = 230;


/* METHODS DEFINITIONS ********************************************************/

BMP::BMP(const char *fname) {
	read(fname);
}

void BMP::read(const char *fname) {
	std::ifstream inp{ fname, std::ios_base::binary };
	if (inp) {
		inp.read((char*)&file_header, sizeof(file_header));
		if(file_header.file_type != 0x4D42) {
			throw std::runtime_error("Error! Unrecognized file format.");
		}
		inp.read((char*)&bmp_info_header, sizeof(bmp_info_header));

		// The BMPColorHeader is used only for transparent images
		if(bmp_info_header.bit_count == 32) {
			// Check if the file has bit mask color information
			if(bmp_info_header.size >= (sizeof(BMPInfoHeader) + sizeof(BMPColorHeader))) {
				inp.read((char*)&bmp_color_header, sizeof(bmp_color_header));
				// Check if the pixel data is stored as BGRA and if the color space type is sRGB
				check_color_header(bmp_color_header);
			} else {
				std::cerr << "Error! The file \"" << fname << "\" does not seem to contain bit mask information\n";
				throw std::runtime_error("Error! Unrecognized file format.");
			}
		}

		// Jump to the pixel data location
		inp.seekg(file_header.offset_data, inp.beg);

		// Adjust the header fields for output.
		// Some editors will put extra info in the image file, we only save the headers and the data.
		if(bmp_info_header.bit_count == 32) {
			bmp_info_header.size = sizeof(BMPInfoHeader) + sizeof(BMPColorHeader);
			file_header.offset_data = sizeof(BMPFileHeader) + sizeof(BMPInfoHeader) + sizeof(BMPColorHeader);
		} else {
			bmp_info_header.size = sizeof(BMPInfoHeader);
			file_header.offset_data = sizeof(BMPFileHeader) + sizeof(BMPInfoHeader);
		}
		file_header.file_size = file_header.offset_data;

		if (bmp_info_header.height < 0) {
			throw std::runtime_error("The program can treat only BMP images with the origin in the bottom left corner!");
		}

		data.resize(bmp_info_header.width * bmp_info_header.height * bmp_info_header.bit_count / 8);

		// Here we check if we need to take into account row padding
		if (bmp_info_header.width % 4 == 0) {
			inp.read((char*)data.data(), data.size());
			file_header.file_size += static_cast<uint32_t>(data.size());
		}
		else {
			row_stride = bmp_info_header.width * bmp_info_header.bit_count / 8;
			uint32_t new_stride = make_stride_aligned(4);
			std::vector<uint8_t> padding_row(new_stride - row_stride);

			for (int y = 0; y < bmp_info_header.height; ++y) {
				inp.read((char*)(data.data() + row_stride * y), row_stride);
				inp.read((char*)padding_row.data(), padding_row.size());
			}
			file_header.file_size += static_cast<uint32_t>(data.size()) + bmp_info_header.height * static_cast<uint32_t>(padding_row.size());
		}
        // Output file details on screen:
        std::cout << std::endl;
        std::cout << "File type: BMP" << std::endl;
        std::cout << "File size: " << file_header.file_size << " bytes" << std::endl;
        std::cout << "Image dimensions: " << "width = " << bmp_info_header.width << " pixels, height = "
                  << bmp_info_header.height << " pixels"<< std::endl;
        std::cout << "Image depth: " << bmp_info_header.bit_count / 8 << std::endl;
	}
	else {
		throw std::runtime_error("Unable to open the input image file.");
	}
}

void BMP::write(const char *fname) {
	std::ofstream of{ fname, std::ios_base::binary };
	if (of) {
		if (bmp_info_header.bit_count == 32) {
			write_headers_and_data(of);
		}
		else if (bmp_info_header.bit_count == 24) {
			if (bmp_info_header.width % 4 == 0) {
				write_headers_and_data(of);
			}
			else {
				uint32_t new_stride = make_stride_aligned(4);
				std::vector<uint8_t> padding_row(new_stride - row_stride);

				write_headers(of);

				for (int y = 0; y < bmp_info_header.height; ++y) {
					of.write((const char*)(data.data() + row_stride * y), row_stride);
					of.write((const char*)padding_row.data(), padding_row.size());
				}
			}
		}
		else {
			throw std::runtime_error("The program can treat only 24 or 32 bits per pixel BMP files");
		}
	}
	else {
		throw std::runtime_error("Unable to open the output image file.");
	}
}

void BMP::diagonalFlip()
{
	std::reverse(data.begin(), data.end());
}

void BMP::horizontalFlip()
{
	uint32_t numRows = (bmp_info_header.size_image / bmp_info_header.width / (bmp_info_header.bit_count / 8)) - 1;
	uint32_t rowSize = bmp_info_header.width * bmp_info_header.bit_count / 8;

	for(uint32_t i=0; i<numRows; ++i)
	{
		auto fromiter = data.begin()+(i*rowSize);
		auto lastiter = fromiter+rowSize;

		std::reverse(fromiter, lastiter);
	}
}

void BMP::verticalFlip()
{
	uint32_t numRows = (bmp_info_header.size_image / bmp_info_header.width / (bmp_info_header.bit_count / 8)) - 2;
	uint32_t rowSize = bmp_info_header.width * bmp_info_header.bit_count / 8;

	auto rowIterA = data.begin();
	auto rowIterB = data.begin()+(numRows*rowSize);
	uint16_t i = 0;

	while(rowIterA != rowIterB)
	{
		std::swap_ranges(rowIterA, rowIterA+rowSize, rowIterB);
		++i;
		rowIterA = data.begin()+(rowSize*i);
		rowIterB = (data.begin()+(numRows*rowSize)) - (rowSize*i);
	}
}

void BMP::changeColors(uint8_t Bb, uint8_t Gb, uint8_t Rb, uint8_t Bw, uint8_t Gw, uint8_t Rw)
{
	for (int32_t y = 0; y < bmp_info_header.height ; ++y) {
		for (int32_t x = 0; x < bmp_info_header.width; ++x) {

			uint32_t channels = bmp_info_header.bit_count / 8;

			if(data[channels * (y * bmp_info_header.width + x) + 0] < WHITE_COLOR_THRES)
			{
				data[channels * (y * bmp_info_header.width + x) + 0] = Bb;
				data[channels * (y * bmp_info_header.width + x) + 1] = Gb;
				data[channels * (y * bmp_info_header.width + x) + 2] = Rb;

			} else {
				data[channels * (y * bmp_info_header.width + x) + 0] = Bw;
				data[channels * (y * bmp_info_header.width + x) + 1] = Gw;
				data[channels * (y * bmp_info_header.width + x) + 2] = Rw;
			}
		}
	}
}

void BMP::write_headers(std::ofstream &of) {
	of.write((const char*)&file_header, sizeof(file_header));
	of.write((const char*)&bmp_info_header, sizeof(bmp_info_header));
	if(bmp_info_header.bit_count == 32) {
		of.write((const char*)&bmp_color_header, sizeof(bmp_color_header));
	}
}

void BMP::write_headers_and_data(std::ofstream &of) {
	write_headers(of);
	of.write((const char*)data.data(), data.size());
}

// Add 1 to the row_stride until it is divisible with align_stride
uint32_t BMP::make_stride_aligned(uint32_t align_stride) {
	uint32_t new_stride = row_stride;
	while (new_stride % align_stride != 0) {
		new_stride++;
	}
	return new_stride;
}

// Check if the pixel data is stored as BGRA and if the color space type is sRGB
void BMP::check_color_header(BMPColorHeader &bmp_color_header) {
	BMPColorHeader expected_color_header;
	if(expected_color_header.red_mask != bmp_color_header.red_mask ||
		expected_color_header.blue_mask != bmp_color_header.blue_mask ||
		expected_color_header.green_mask != bmp_color_header.green_mask ||
		expected_color_header.alpha_mask != bmp_color_header.alpha_mask) {
		throw std::runtime_error("Unexpected color mask format! The program expects the pixel data to be in the BGRA format");
	}
	if(expected_color_header.color_space_type != bmp_color_header.color_space_type) {
		throw std::runtime_error("Unexpected color space type! The program expects sRGB values");
	}
}

