// test.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <time.h>
#include "TeethDetect.h"



int main()
{

	int num,w,h;
	float** coord = new float*[16];
	for (int i = 0; i < 16; i++) {
		coord[i] = new float[7];
	}
	clock_t  start, stop;
	start = clock();

	TeethDetector* td = new TeethDetector(
		"E:/TensorFlowCplusplus/TeethDetect/x64/Release/output_graph.pb");
	stop = clock();
	cout <<"init time: "<< stop - start << endl;

	for (int i = 0; i < 5; i++)
	{
		clock_t  start, stop;
		start = clock();
		cout << i << endl;
		td->detect("E:/TensorFlowCplusplus/TeethDetect/x64/Release/low.png", num, coord, w, h);
		stop = clock();
		cout <<"once time: "<<stop - start <<"\n\n";

		//cout << "num = " << num << endl;
		//for (int i = 0; i < num; ++i) {
		//	for (int j = 0; j < 7; ++j)
		//		cout << coord[i][j] << "\t";
		//	std::cout << std::endl;
		//}

	}


		return 0;
}

