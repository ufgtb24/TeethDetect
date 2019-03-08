// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the TEETHDETECT_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// TEETHDETECT_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef TEETHDETECT_EXPORTS
#define TEETHDETECT_API __declspec(dllexport)
#else
#define TEETHDETECT_API __declspec(dllimport)
#endif

#include "Teeth_Detector.h"
#include <vector>
#include <memory>
namespace tensorflow {
	class Tensor;
	class Session;
	class Status;
	class Env;
}
using namespace std;
using namespace tensorflow;
// class Tensor;
// class Session;
// class Status;

// This class is exported from the TeethDetect.dll
class TEETHDETECT_API TeethDetect_CPU :public Teeth_Detector {
public:
	TeethDetect_CPU(string graph_path);
	// TODO: add your methods here.
	int detect(const char* image_path, int& num_box, float** coord,int& width, int& height);
private:
	unique_ptr<Session> session_detect;

	Status ReadTensorFromImageFile(const string& file_name, const int input_height,
		const int input_width, const float input_std,
		vector<Tensor>* out_tensors);

	Status ReadEntireFile(Env* env, const string& filename, Tensor* output);
	Status TeethDetect_CPU::LoadGraph(const string& graph_file_name,
		unique_ptr<tensorflow::Session>* session);

};

