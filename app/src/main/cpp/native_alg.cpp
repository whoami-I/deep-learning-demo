#include "opencv2/opencv.hpp"
#include "detect.h"
#include "inference.h"
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>

std::unique_ptr<InferenceContext> ctx;
extern "C"
jint JNIFUNCF(NumberClassifyer, getNumberByNativeAlg, jobject bitmap,
              jint width, jint height, jobject assetManager) {
    if (ctx == NULL) {
        AAssetManager *mgr = AAssetManager_fromJava(env, assetManager);
        AAsset *model_cnn = AAssetManager_open(mgr, "model_cnn.tflite",
                                               AASSET_MODE_STREAMING);
        int length = AAsset_getLength(model_cnn);
        unsigned char *file_buffer = new unsigned char[length];
        AAsset_read(model_cnn, file_buffer, length);
        AAsset_close(model_cnn);
        ctx = std::unique_ptr<InferenceContext>(new InferenceContext((char *) file_buffer, length));
    }

    return ctx->get_num(env, bitmap, width, height);
}

