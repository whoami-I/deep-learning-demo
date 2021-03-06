# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.4.1)

# Creates and names a library, sets it as either STATIC
# or SHARED, and provides the relative paths to its source code.
# You can define multiple libraries, and CMake builds them for you.
# Gradle automatically packages shared libraries with your APK.
#设置参数
set(LIBS_DIR ${CMAKE_SOURCE_DIR}/libs)
set(SRC_DIR src/main/cpp)
include_directories(src/main/cpp)
include_directories(src/main/cpp/include)
add_library( # Sets the name of the library.
        filters

        # Sets the library as a shared library.
        SHARED

        # Provides a relative path to your source file(s).
        src/main/cpp/detect.cpp
        ${SRC_DIR}/native_alg.cpp
        ${SRC_DIR}/inference.cpp
        ${SRC_DIR}/tflite.cpp
        )

# 添加 opencv 的动态库
add_library(libopencv STATIC IMPORTED)
set_target_properties(libopencv PROPERTIES IMPORTED_LOCATION ${LIBS_DIR}/${ANDROID_ABI}/libopencv_java4.so)

add_library(libtfliteplant SHARED IMPORTED)
set_target_properties(libtfliteplant PROPERTIES IMPORTED_LOCATION ${LIBS_DIR}/${ANDROID_ABI}/libtensorflowlite_plant.so)
set_target_properties(libtfliteplant PROPERTIES IMPORTED_NO_SONAME TRUE)

# Searches for a specified prebuilt library and stores the path as a
# variable. Because CMake includes system libraries in the search path by
# default, you only need to specify the name of the public NDK library
# you want to add. CMake verifies that the library exists before
# completing its build.

find_library( # Sets the name of the path variable.
        log-lib

        # Specifies the name of the NDK library that
        # you want CMake to locate.
        log)

find_library(jnigraphics-lib jnigraphics)
find_library(android-lib android)


# Specifies libraries CMake should link to your target library. You
# can link multiple libraries, such as libraries you define in this
# build script, prebuilt third-party libraries, or system libraries.
#link_libraries(${LIBS_DIR}/${ANDROID_ABI}/libopencv_java4.so)
#link_libraries(${LIBS_DIR}/${ANDROID_ABI}/libtensorflowlite_plant.so)
target_link_libraries( # Specifies the target library.
        filters

        # Links the target library to the log library
        # included in the NDK.
        ${log-lib}
        libopencv
        libtfliteplant
        ${jnigraphics-lib}
        ${android-lib})