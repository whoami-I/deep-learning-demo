#ifndef TFLITE_BACKEND_H
#define TFLITE_BACKEND_H

#include <memory>
#include <tensorflow/lite/model.h>
#include <tensorflow/lite/interpreter.h>
#include <tensorflow/lite/kernels/register.h>

class TfBackend {
// /nas/projects/on-device-ai/benchmarks/models/THEBEST99k_MACE/tflite/models/THEBEST99k_MACE-float32.lite


    std::string imageNode = "ImageTensor";
    std::string outputNode = "SemanticPredictions";

    size_t num_threads = 2;
    size_t in_height;
    size_t in_width;
    size_t in_depth = 3;
    size_t out_height;
    size_t out_width;
    size_t out_depth = 2;

    void VerifyShapes();

public:
    std::unique_ptr<tflite::FlatBufferModel> model_;
    std::unique_ptr<tflite::Interpreter> interpreter_;
    TfBackend(std::string modelPath);

    float* getInputPtr();
    float * getOutputPtr();
    void invoke();

    TfBackend(const char *file_buffer, size_t size);
};

#endif
