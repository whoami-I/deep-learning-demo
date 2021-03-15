#include "opencv2/opencv.hpp"
#include "detect.h"

using namespace cv;
int min_face_size = 20;

extern "C"
void JNIFUNCF(NumberClassifyer, getNum, jobject bitmap,
              jint width, jint height) {
    void *destination = 0;
    int ret = 0;
    if ((ret = AndroidBitmap_lockPixels(env, bitmap, &destination)) < 0) {
        LOG("First Bitmap LockPixels Failed return=%d!", ret);
        return;
    }
    cv::Mat img(height, width, CV_8UC4, destination);
    cv::Mat im;
//    img.convertTo(im, CV_32FC4);

    jfloat *byte_buffer = static_cast<float *>(malloc(
            static_cast<size_t>(img.rows * img.cols * 4)));

    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            float color = (float) (img.at<int>(i, j)&0xFF);
            byte_buffer[i * height + j] = color;
//            img.at<uchar>(i, j) = 255;
        }
        LOG("%d %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f ",
            i,byte_buffer[i * height], byte_buffer[i * height + 1], byte_buffer[i * height + 2],
            byte_buffer[i * height + 3], byte_buffer[i * height + 4], byte_buffer[i * height + 5],
            byte_buffer[i * height + 6], byte_buffer[i * height + 7], byte_buffer[i * height + 8],
            byte_buffer[i * height + 9], byte_buffer[i * height + 10], byte_buffer[i * height + 11],
            byte_buffer[i * height + 12], byte_buffer[i * height + 13],
            byte_buffer[i * height + 14],
            byte_buffer[i * height + 15], byte_buffer[i * height + 16],
            byte_buffer[i * height + 17],
            byte_buffer[i * height + 18], byte_buffer[i * height + 19],
            byte_buffer[i * height + 20],
            byte_buffer[i * height + 21], byte_buffer[i * height + 22],
            byte_buffer[i * height + 23],
            byte_buffer[i * height + 24], byte_buffer[i * height + 25],
            byte_buffer[i * height + 26],
            byte_buffer[i * height + 27]);
    }

    jobject jbuffer = env->NewDirectByteBuffer(byte_buffer,
                                               img.rows * img.cols * 4);

    jclass jniHandle = env->GetObjectClass(thiz);
//    jclass jniHandle = env->FindClass("com/example/reg/NumberClassifyer");
    jmethodID jmethodId = env->GetMethodID(jniHandle, "detect",
                                           "(Ljava/nio/ByteBuffer;Ljava/lang/Object;)V");
//    jmethodID jmethodId = env->GetMethodID(jniHandle, "detect",
//                                           "(Ljava/nio/ByteBuffer;)V");
//    float *result = static_cast<float *>(malloc(10 * sizeof(float)));
    jfloatArray jresult = env->NewFloatArray(10);

    jclass floatClass = env->FindClass("[F");
    jobjectArray jo = env->NewObjectArray(1, floatClass, jresult);

    env->CallVoidMethod(thiz, jmethodId, jbuffer, jo);
//    env->CallVoidMethod(thiz, jmethodId, jbuffer);
    jfloat *result = env->GetFloatArrayElements(jresult, 0);
    jfloatArray ja = static_cast<jfloatArray>(env->GetObjectArrayElement(jo, 0));
    result = env->GetFloatArrayElements(ja, 0);
    float max = -100000;
    int index = -1;
    for (int i = 0; i < 10; ++i) {
        if (max < result[i]) {
            max = result[i];
            index = i;
        }
//        LOG("the result is %f\n", result[i]);
    }
    LOG("the number is %d\n", index);

    free(byte_buffer);
    free(result);
    AndroidBitmap_unlockPixels(env, bitmap);
}

cv::Mat normalizeMat(cv::Mat in, float scale) {
    float h = in.rows * scale;
    float w = in.cols * scale;
    cv::Mat resizedImg;
    cv::resize(in, resizedImg, cv::Size(h, w), 0, 0, cv::InterpolationFlags::INTER_LINEAR);
    Mat norm_img;
    resizedImg.convertTo(norm_img, CV_32FC3);
    return norm_img;
}

