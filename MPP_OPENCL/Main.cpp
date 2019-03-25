#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>

#ifdef __APPLE__
#include <OpenCL/opencl.h>
#else
#include <CL/cl.hpp>
#endif

using namespace std;
using namespace cl;
#define MAX_SOURCE_SIZE (0x100000)

/* Funtions declarations */
void getAvailableDevicesInfo();

int main(void) {
	cout << "Program starting \n" << endl;
	
	// Priting out all information of available devices
	cout << "Gettings devices information:\n" << endl;
	getAvailableDevicesInfo();



	system("pause");
	return 0;
}

// Devices informations
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