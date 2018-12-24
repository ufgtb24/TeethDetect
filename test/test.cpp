// test.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"


int main()
{
	TeethDetect* td = new TeethDetect("E:/TensorFlowCplusplus/TeethDetect/x64/Release/output_graph.pb");
	int num,w,h;
	float** coord = new float*[14];
	for (int i = 0; i < 14; i++) {
		coord[i] = new float[4];
	}

	td->detect("E:/TensorFlowCplusplus/TeethDetect/x64/Release/up.png", num, coord, w, h);
	cout << "num = " << num << endl;
	for (int i = 0; i < num; ++i) {
	 	for (int j = 0; j < 4; ++j)
	 		// using (index_1, index_2, index_3) to access the element in a tensor
	 		cout << coord[i][j]<< "\t";
	 	std::cout << std::endl;
	}

	getchar();
		return 0;
}

