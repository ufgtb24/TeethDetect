#pragma once
#include <string>
using namespace std;

class  Teeth_Detector {
public:
	virtual int detect(const char* image_path, 
		int& num_box, float** coord, 
		int& width, int& height) = 0;

};
