#include "tflite.h"
#include "logutils.h"

TfBackend::TfBackend(std::string modelPath):
    in_width(0),
    in_height(0),
    out_width(0),
    out_height(0)
{
    LOGD("TFLite - Opening model: %s", modelPath.c_str());
    model_ = tflite::FlatBufferModel::BuildFromFile(modelPath.c_str());
    if (!model_) {
        throw std::runtime_error("Failed to mmap model");
    }

    tflite::ops::builtin::BuiltinOpResolver resolver;
    tflite::InterpreterBuilder{*model_, resolver}(&interpreter_);
    if (!interpreter_) {
        throw std::runtime_error("Failed to construct TFLite interpreter");
    }

    interpreter_->SetNumThreads(num_threads);
    //VerifyShapes();
    if (interpreter_->AllocateTensors() != kTfLiteOk) {
        throw std::runtime_error("Could not allocate tensors");
    }
}

float* TfBackend::getInputPtr() {
    return interpreter_->typed_input_tensor<float>(0);
}

float * TfBackend::getOutputPtr() {
    return interpreter_->typed_output_tensor<float >(0);
}

void TfBackend::invoke() {
    interpreter_->Invoke();
}

TfBackend::TfBackend(const char *file_buffer, size_t size) {
    model_ = tflite::FlatBufferModel::VerifyAndBuildFromBuffer(file_buffer,size);
    if (!model_) {
        throw std::runtime_error("Failed to mmap model");
    }

    tflite::ops::builtin::BuiltinOpResolver resolver;
    tflite::InterpreterBuilder{*model_, resolver}(&interpreter_);
    if (!interpreter_) {
        throw std::runtime_error("Failed to construct TFLite interpreter");
    }

    interpreter_->SetNumThreads(num_threads);
    //VerifyShapes();
    if (interpreter_->AllocateTensors() != kTfLiteOk) {
        throw std::runtime_error("Could not allocate tensors");
    }
}

