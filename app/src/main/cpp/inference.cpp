#include "inference.h"
#include "logutils.h"

#include <opencv2/imgproc.hpp>
#include <unistd.h>
#include <include/detect.h>

using namespace cv;

InferenceContext::InferenceContext(const char *file_buffer, size_t size) : backend(file_buffer,
                                                                                   size) {}

int InferenceContext::get_num(JNIEnv *env, jobject bitmap, int width, int height) {
    void *destination = 0;
    int ret = 0;
    if ((ret = AndroidBitmap_lockPixels(env, bitmap, &destination)) < 0) {
        LOG("First Bitmap LockPixels Failed return=%d!", ret);
        return -1;
    }

    Mat src(height, width, CV_8UC4, destination);
    Mat out(height, width, CV_8UC1);
    float *inputArray = backend.getInputPtr();
    Mat input(height, width, CV_32FC1, inputArray);
    int fromTo[] = {0, 0};
    mixChannels(&src, 1, &out, 1, fromTo, 1);
    out.convertTo(input, CV_32FC1);
    backend.invoke();
    float *result = backend.getOutputPtr();

    float max = -100000;
    int index = -1;
    for (int i = 0; i < 10; ++i) {
        if (max < result[i]) {
            max = result[i];
            index = i;
        }
//        LOG("the result is %f\n", result[i]);
    }

    AndroidBitmap_unlockPixels(env, bitmap);
    return index;
}

