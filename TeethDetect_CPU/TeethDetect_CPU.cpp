// TeethDetect.cpp : Defines the exported functions for the DLL application.
//


#include "stdafx.h"
// This is an example of an exported variable

// This is the constructor of a class that has been exported.
// see TeethDetect.h for the class definition
TeethDetect_CPU::TeethDetect_CPU(string graph_path)
{
	Status load_graph_status = LoadGraph(graph_path, &session);
	if (!load_graph_status.ok()) {
		LOG(ERROR) << "!!!!!!!!!!!!!!!\n";
		LOG(ERROR) << load_graph_status;
		LOG(ERROR) << "!!!!!!!!!!!!!!!\n";
	}

    return;
}

int TeethDetect_CPU::detect(string image_path, int& num_box, float** coord, int& width, int& height)
{
	string input_layer = "input";
	string output_layer = "output_node";
	std::vector<Tensor> resized_tensors;
	Status read_tensor_status =
		ReadTensorFromImageFile(image_path, 416, 416,
			255, &resized_tensors);
	if (!read_tensor_status.ok()) {
		LOG(ERROR) << "!!!!!!!!!!!!!!!\n";
		LOG(ERROR) << read_tensor_status;
		LOG(ERROR) << "!!!!!!!!!!!!!!!\n";
		return -1;
	}

	// define feed value for resized image 
	const Tensor& resized_tensor = resized_tensors[0];
	const Tensor& ori_size = resized_tensors[1];
	auto ori_size_value = ori_size.tensor<int, 1>();
	cout <<"ori_size: "<< ori_size_value(0) << "   " << ori_size_value(1) << endl;

	// define feed value for phase
	Tensor is_training(DT_BOOL, TensorShape());
	is_training.scalar<bool>()() = false;

	// define feed value for original image size
	Tensor image_shape(DT_FLOAT, TensorShape({2}));
	auto t_vec = image_shape.vec<float>();
	t_vec(0) = ori_size_value(0);
	t_vec(1) = ori_size_value(1);
	width = ori_size_value(0);
	height= ori_size_value(1);
	// Actually run the image through the model.
	std::vector<Tensor> outputs;
	Status run_status = session->Run({ 
		{ input_layer, resized_tensor },{ "phase", is_training },{ "image_shape",image_shape } },
	{ output_layer }, {}, &outputs);

	if (!run_status.ok()) {
		LOG(ERROR) << "!!!!!!!!!!!!!!!\n";
		LOG(ERROR) << "Running model failed: " << run_status;
		LOG(ERROR) << "!!!!!!!!!!!!!!!\n";
		return -1;
	}
	else {
		const TensorShape& output_shape = outputs[0].shape();
		num_box = output_shape.dim_size(0);
		cout <<"num_box = "<< num_box <<endl;
		auto output_detection_boxes = outputs[0].tensor<float, 2>();

		for (int i = 0; i < num_box; i++) {
			for (int j = 0; j < 4; j++) {
				coord[i][j] = output_detection_boxes(i,j);
// 				cout << coord[i][j] << "\t";
			}
// 			cout << endl;
		}

	}
}

Status TeethDetect_CPU::ReadTensorFromImageFile(const string& file_name, const int input_height,
	const int input_width, const float input_std,std::vector<Tensor>* out_tensors) {
	auto root = tensorflow::Scope::NewRootScope();
	using namespace ::tensorflow::ops;  // NOLINT(build/namespaces)

	string input_name = "file_reader";
	string output_name = "normalized";

	// read file_name into a tensor named input
	Tensor input(tensorflow::DT_STRING, tensorflow::TensorShape());
	TF_RETURN_IF_ERROR(
		ReadEntireFile(tensorflow::Env::Default(), file_name, &input));

	// use a placeholder to read input data
	auto file_reader =
		Placeholder(root.WithOpName("input"), tensorflow::DataType::DT_STRING);

	std::vector<std::pair<string, tensorflow::Tensor>> inputs = {
		{ "input", input },
	};
	// Now try to figure out what kind of file it is and decode it.
	const int wanted_channels = 3;
	tensorflow::Output image_reader;
	if (tensorflow::StringPiece(file_name).ends_with(".png")) {
		image_reader = DecodePng(root.WithOpName("png_reader"), file_reader,
			DecodePng::Channels(wanted_channels));
	}
	else if (tensorflow::StringPiece(file_name).ends_with(".gif")) {
		// gif decoder returns 4-D tensor, remove the first dim
		image_reader =
			Squeeze(root.WithOpName("squeeze_first_dim"),
				DecodeGif(root.WithOpName("gif_reader"), file_reader));
	}
	else if (tensorflow::StringPiece(file_name).ends_with(".bmp")) {
		image_reader = DecodeBmp(root.WithOpName("bmp_reader"), file_reader);
	}
	else {
		// Assume if it's neither a PNG nor a GIF then it must be a JPEG.
		image_reader = DecodeJpeg(root.WithOpName("jpeg_reader"), file_reader,
			DecodeJpeg::Channels(wanted_channels));
	}




	auto get_shape = Shape(root.WithOpName("get_shape"), image_reader);

	// Now cast the image data to float so we can do normal math on it.
	auto float_caster =
		Cast(root.WithOpName("float_caster"), image_reader, tensorflow::DT_FLOAT);
	// The convention for image ops in TensorFlow is that all images are expected
	// to be in batches, so that they're four-dimensional arrays with indices of
	// [batch, height, width, channel]. Because we only have a single image, we
	// have to add a batch dimension of 1 to the start with ExpandDims().
	auto dims_expander = ExpandDims(root, float_caster, 0);

	// Bilinearly resize the image to fit the required dimensions.
	auto resized = ResizeBilinear(
		root, dims_expander,
		Const(root.WithOpName("size"), { input_height, input_width }));
	// Subtract the mean and divide by the scale.
	Div(root.WithOpName(output_name), resized,{ input_std });

	// This runs the GraphDef network definition that we've just constructed, and
	// returns the results in the output tensor.
	tensorflow::GraphDef graph;

	TF_RETURN_IF_ERROR(root.ToGraphDef(&graph));


	std::unique_ptr<tensorflow::Session> session(
		tensorflow::NewSession(tensorflow::SessionOptions()));
	TF_RETURN_IF_ERROR(session->Create(graph));
	TF_RETURN_IF_ERROR(session->Run({ inputs }, { output_name,"get_shape" }, {}, out_tensors));
	return Status::OK();
}

Status TeethDetect_CPU::ReadEntireFile(tensorflow::Env* env, const string& filename,
	Tensor* output) {
	tensorflow::uint64 file_size = 0;
	TF_RETURN_IF_ERROR(env->GetFileSize(filename, &file_size));

	string contents;
	contents.resize(file_size);

	std::unique_ptr<tensorflow::RandomAccessFile> file;
	TF_RETURN_IF_ERROR(env->NewRandomAccessFile(filename, &file));

	tensorflow::StringPiece data;
	TF_RETURN_IF_ERROR(file->Read(0, file_size, &data, &(contents)[0]));
	if (data.size() != file_size) {
		return tensorflow::errors::DataLoss("Truncated read of '", filename,
			"' expected ", file_size, " got ",
			data.size());
	}
	output->scalar<string>()() = data.ToString();
	return Status::OK();
}

Status TeethDetect_CPU::LoadGraph(const string& graph_file_name,
	unique_ptr<tensorflow::Session>* session) {
	tensorflow::GraphDef graph_def;
	Status load_graph_status =
		ReadBinaryProto(tensorflow::Env::Default(), graph_file_name, &graph_def);
	if (!load_graph_status.ok()) {
		return tensorflow::errors::NotFound("Failed to load compute graph at '",
			graph_file_name, "'");
	}
	session->reset(tensorflow::NewSession(tensorflow::SessionOptions()));
	Status session_create_status = (*session)->Create(graph_def);
	if (!session_create_status.ok()) {
		return session_create_status;
	}
	return Status::OK();
}

extern "C" __declspec(dllexport) Teeth_Detector* getFDObj(string graph_path)
{
	return new TeethDetect_CPU(graph_path);
}


