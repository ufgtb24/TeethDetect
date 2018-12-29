// test.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"


Teeth_Detector* get_TD_Obj(string graph_path,LPCSTR dllpath
) {

	typedef Teeth_Detector*(*loadFun)(string graph_path); //宏定义函数指针类型
	HINSTANCE hDll; //DLL句柄 
	cout <<"inside  :"<< dllpath<<endl;
	hDll = LoadLibrary(dllpath);

	if (hDll == NULL) {
		int dwError = GetLastError();
		cout << "load failed \n" << dwError;
	}
	loadFun getFDObj = (loadFun)GetProcAddress(hDll, "getFDObj");
	return getFDObj(graph_path);
}


int main()
{
	char* dllpath = "TeethDetect_CPU.dll";
	cout << dllpath;
	Teeth_Detector* td = get_TD_Obj(
		"E:/TensorFlowCplusplus/TeethDetect/x64/Release/output_graph.pb", dllpath);

// 	TeethDetect_GPU* td = new TeethDetect_GPU("E:/TensorFlowCplusplus/TeethDetect/x64/Release/output_graph.pb");
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

