#pragma once
#include <string>
using namespace std;

class  Teeth_Detector {
public:
	virtual int detect(string image_path, 
		int& num_box, float** coord, 
		int& width, int& height) = 0;

};
