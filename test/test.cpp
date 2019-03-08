// test.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"


Teeth_Detector* get_TD_Obj(char* graph_path,LPCSTR dllpath
) {

	typedef Teeth_Detector*(*loadFun)(char* graph_path); //宏定义函数指针类型
	HINSTANCE hDll; //DLL句柄 
	cout <<"\n inside  :  "<< dllpath<<endl;
	hDll = LoadLibrary(dllpath);

	if (hDll == NULL) {
		int dwError = GetLastError();
		cout << "load failed \n" << dwError;
	}
	loadFun getObj = (loadFun)GetProcAddress(hDll, "getObj");
	return getObj(graph_path);
}


int main()
{
	char* dllpath = "TeethDetect_GPU.dll";
	cout << dllpath;
	Teeth_Detector* td = get_TD_Obj(
		"E:/TensorFlowCplusplus/TeethDetect/x64/Release/output_graph.pb", dllpath);

	int num,w,h;
	float** coord = new float*[16];
	for (int i = 0; i < 16; i++) {
		coord[i] = new float[7];
	}
	for (int i = 0; i < 2; i++)
	{
		td->detect("E:/TensorFlowCplusplus/TeethDetect/x64/Release/up.png", num, coord, w, h);
		cout << "num = " << num << endl;
		for (int i = 0; i < num; ++i) {
			for (int j = 0; j < 7; ++j)
				cout << coord[i][j] << "\t";
			std::cout << std::endl;
		}

// 		getchar();
	}
	getchar();

		return 0;
}

