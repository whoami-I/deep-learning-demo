#ifndef INFERENCE_H
#define INFERENCE_H
#include "utils.h"
#include "tflite.h"
#include <jni.h>
class InferenceContext {
private:
    Dims inputDims{513, 513, 3}; //512 256 + 1 TODO(Paweł Subko) hardcoded model property -> move to cfg
    Dims outputDims{129, 129, 2}; // 128 64 + 1 TODO(Paweł Subko) hardcoded model property -> move to cfg

    // Computed masks
    cv::Mat nativeSizeMask;
    cv::Mat currentMask;

    // Tmp mats
    cv::Mat intermediateSizeImgRGB;

    TfBackend backend;

public:
    InferenceContext(const char *file_buffer,size_t size);
    ~InferenceContext() {};
//    void computeSegmentationMask(cv::Mat &inputMat, bool CRF);
//    void applyEffect(cv::Mat &dstMat, cv::Mat &effectMat, bool transferColor=true);
    int get_num(JNIEnv *env, jobject bitmap, int width, int height);
};


#endif