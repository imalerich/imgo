#include <iostream>
#include <cstdlib>
#include <string>
#include <cinttypes>
#include <memory>
#include <algorithm>
#include <iosfwd>
#include <utility>
#include <vector>

#include <gtp/gtp.hpp>
#include <caffe/caffe.hpp>

using namespace caffe;

const static std::string MODEL = "../value/imgo_value_net_deploy.prototxt";
const static std::string TRAINED = "../net/imgo_value_iter_10000.caffemodel";

int main(int argc, char ** argv) {
	// Perform classifications on the GPU.
	Caffe::set_mode(Caffe::CPU);

	// Test a simple classification via Caffe.
	std::shared_ptr<Net<float>> net;
	net.reset(new Net<float>(MODEL, TEST));
	net->CopyTrainedLayersFrom(TRAINED);

	// Get some information about the dimmensions of our network.
	Blob<float> * input_layer = net->input_blobs()[0];
	int num_channels = input_layer->channels();
	int layer_width = input_layer->width();
	int layer_height = input_layer->height();

	std::cout << "Channels: " << num_channels << "\t";
	std::cout << "Layer Width: " << layer_width << "\t";
	std::cout << "Layer Height: " << layer_height << "\n";

	return 0;
}
