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

#include <vector>
using namespace std;
namespace tensorflow {
	class Tensor;
	class Session;
	class Status;
	class Env;
}
// class Tensor;
// class Session;
// class Status;

// This class is exported from the TeethDetect.dll
class TEETHDETECT_API CTeethDetect {
public:
	CTeethDetect(void);
	// TODO: add your methods here.

	tensorflow::Status ReadTensorFromImageFile(const string& file_name, const int input_height,
		const int input_width, const float input_mean,
		const float input_std,
		vector<tensorflow::Tensor>* out_tensors);

	tensorflow::Status ReadEntireFile(tensorflow::Env* env, const string& filename, tensorflow::Tensor* output);

};

extern TEETHDETECT_API int nTeethDetect;

TEETHDETECT_API int fnTeethDetect(void);
