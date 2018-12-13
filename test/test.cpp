// test.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"


int main()
{
	TeethDetect* td = new TeethDetect("E:/TensorFlowCplusplus/TeethDetect/x64/Release/output_graph.pb");
	td->detect("E:/TensorFlowCplusplus/TeethDetect/x64/Release/up.png");
	getchar();
		return 0;
}

