#include <iostream>
#include <vector>
// Using lodepng as png image manipulator
#include "lodepng.h" // PNG loader 
// Including c++ Nvdia openCL 
#include <CL/cl.hpp>

// Constants
const char *FILE_1 = "Images/im0.png"; // First image file name
const char *FILE_2 = "Images/im1.png"; // Second image file name
const char *FILE_D1 = "Images/d_im0.png"; // Downsampled first image file name
const char *FILE_D2 = "Images/d_im1.png"; // Downsampled second image file name
const char *FILE_L2R = "Images/left_on_right.png";
const char *FILE_R2L = "Images/right_on_left.png";
const char *FILE_CROSS_CHECKED = "Images/cross_checked.png";
const char *FILE_OCCLUSION_FILLED = "Images/occlusion_filled.png";
const int M = 16; // Downsampling factor
const int WIN_SIZE = 9; // window size

#define MAX_SOURCE_SIZE (0x100000)

// Defining used namespaces !
using namespace std;
using namespace cl;

/*
	---------------------------------------- Function signature -------------------------------------------------
*/
void getAvailableDevicesInfo();
void loadImages(const char* filename, vector<unsigned char>& image, unsigned& width, unsigned& height);
void writeImage(vector<std::vector<unsigned char> >& image, const char* filename, bool color);



// Main function
int main(void) {
	cout << "Program starting \n" << endl;
	
	// Priting out all information of available devices
	cout << "Gettings devices information:\n" << endl;
	getAvailableDevicesInfo();



	system("pause");
	return 0;
}

// Functions implementation

// Devices informations function
void getAvailableDevicesInfo() 
{
	// Gettings all available platforms
	vector<cl::Platform> platforms;
	Platform::get(&platforms);

	_ASSERT(platforms.size() > 0);

	vector <Device> devices;

	for (auto platform : platforms)
	{
		// Get all platform devices 
		platform.getDevices(CL_DEVICE_TYPE_ALL, &devices);

		cout << "* Platform name: ";
		cout << platform.getInfo<CL_PLATFORM_NAME>() << endl;

		_ASSERT(devices.size() > 0);

		cout << "Platform devices:" << endl;
		for (auto device : devices)
		{
			cout << "Device name: " << device.getInfo<CL_DEVICE_NAME>() << endl;
			cout << "Device vendor: " << device.getInfo<CL_DEVICE_VENDOR>() << endl;
			cout << "Device local memory type: " << device.getInfo<CL_DEVICE_LOCAL_MEM_TYPE>() << endl;
			cout << "Device local memory size: " << device.getInfo<CL_DEVICE_LOCAL_MEM_SIZE>() << endl;
			cout << "Device max frequency: " << device.getInfo<CL_DEVICE_MAX_CLOCK_FREQUENCY>() << endl;
			cout << "Device max constant buffer size: " << device.getInfo<CL_DEVICE_MAX_CONSTANT_BUFFER_SIZE>() << endl;
			cout << "Device max work group size: " << device.getInfo<CL_DEVICE_MAX_WORK_GROUP_SIZE>() << endl;
			cout << "Device max work item size: " << device.getInfo<CL_DEVICE_MAX_WORK_ITEM_SIZES>()[0] << endl;
			cout << endl;
		}
		cout << endl;
	}

}

// Source: https://raw.githubusercontent.com/lvandeve/lodepng/master/examples/example_decode.cpp
//Load PNG file from disk to memory first, then decode to raw pixels in memory.
void loadImages(const char* filename, vector<unsigned char>& image, unsigned& width, unsigned& height) {
	std::vector<unsigned char> png; // Image will be represented in 8-bit unsinged representation

	//load and decode
	unsigned error = lodepng::load_file(png, filename);
	if (!error) error = lodepng::decode(image, width, height, png);

	//if there's an error, display it
	if (error) std::cout << "decoder error " << error << ": " << lodepng_error_text(error) << std::endl;
	else std::cout << "Image " << filename << " loaded successfully ! " << std::endl;

	//the pixels are now in the vector "image", 4 bytes per pixel, ordered RGBARGBA..., use it as texture, draw it, ...
	png.clear();
	png.shrink_to_fit();
	return;
}

//Encode from raw pixels to an in-memory PNG file first, then write it to disk
//The image argument has width * height RGBA pixels or width * height * 4 bytes
void writeImage(vector<std::vector<unsigned char> >& image, const char* filename, bool color)
{
	std::vector<unsigned char> png, tmp;
	int height = image.size();
	int width = image[0].size();

	// Linearization

	if (!color) // If the input image is a gray image
	{
		for (int j = 0; j < height; j++)
		{
			for (int i = 0; i < width; i++)
			{
				tmp.push_back(image[j][i]);			 // R
				tmp.push_back(image[j][i]);			 // G
				tmp.push_back(image[j][i]);			 // B
				tmp.push_back((unsigned char)255); // A
			}
		}
	}
	else // In case the input image is a colored image with RGBA
	{
		for (int j = 0; j < height; j++)
		{
			for (int i = 0; i < width; i++)
			{
				tmp.push_back(image[j][i]);
			}
		}
	}

	unsigned error = lodepng::encode(png, tmp, width, height);
	if (!error) lodepng::save_file(png, filename);

	//if there's an error, display it
	if (error) std::cout << "encoder error " << error << ": " << lodepng_error_text(error) << std::endl;
	else std::cout << "Image was successfully written to disk at: " << filename << std::endl;

	png.clear();
	tmp.clear();
	png.shrink_to_fit();
	tmp.shrink_to_fit();
	return;
}
