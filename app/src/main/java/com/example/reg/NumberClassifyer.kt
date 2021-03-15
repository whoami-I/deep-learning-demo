package com.example.reg

import android.content.Context
import android.graphics.Bitmap
import android.util.Log
import org.tensorflow.lite.Interpreter
import org.tensorflow.lite.gpu.GpuDelegate
import java.io.FileInputStream
import java.io.IOException
import java.nio.ByteBuffer
import java.nio.ByteOrder
import java.nio.MappedByteBuffer
import java.nio.channels.FileChannel

class NumberClassifyer {

    companion object {
        @Volatile
        private var instance: NumberClassifyer? = null
        private lateinit var context: Context
        val NUMBER_CLASSIFY_FILENAME: String = "model_cnn.tflite"
        val IMAGE_WIDTH = 28
        val IMAGE_HEIGHT = 28
        fun getInstance(): NumberClassifyer {
            return instance ?: synchronized(this) {
                instance ?: NumberClassifyer().also { instance = it }
            }
        }

        fun init(context: Context) {
            this.context = context
        }
    }


    private var resultArray = Array(1) { FloatArray(10) }

    /** Options for configuring the Interpreter.  */
    private val tfliteOptions =
        Interpreter.Options()

    /** The loaded TensorFlow Lite model.  */
    private var tfliteModel: MappedByteBuffer

    /** Optional GPU delegate for accleration.  */
    private var gpuDelegate: GpuDelegate

    /** An instance of the driver class to run model inference with Tensorflow Lite.  */
    private var tflite: Interpreter

    /** A ByteBuffer to hold image data, to be feed into Tensorflow Lite as inputs.  */
    private var imgData: ByteBuffer

    constructor() {
        tfliteModel = loadModelFile(context)
        gpuDelegate = GpuDelegate()
        tfliteOptions.addDelegate(gpuDelegate)
        tfliteOptions.setNumThreads(2)
        tflite = Interpreter(tfliteModel, tfliteOptions)
        imgData = ByteBuffer.allocate(IMAGE_HEIGHT * IMAGE_WIDTH * 4)
            .order(ByteOrder.nativeOrder())
    }

    init {
        System.loadLibrary("filters")
    }

    private val intValues = IntArray(IMAGE_HEIGHT * IMAGE_HEIGHT)
    /**
     * 传进来必须是一张灰度图
     */
    fun getNumber(bitmap: Bitmap): Int {
        if (bitmap.width != IMAGE_WIDTH || bitmap.height != IMAGE_HEIGHT) return -1
        bitmap.getPixels(intValues, 0, bitmap.width, 0, 0, bitmap.width, bitmap.height)
        Log.d("TAG", "------------array size:${resultArray.size}")
        var pixel = 0
        imgData.rewind()
        for (i in 0 until bitmap.height) {
            for (j in 0 until bitmap.width) {
                val color = intValues[pixel++]
                val rColor = (color and 0xFF)
                imgData.putFloat(rColor.toFloat())
            }
        }
//        imgData.compact()
        detect(imgData,resultArray)
        resultArray.forEach {
            it.forEach { Log.d("TAGTAG", " the result is ${it}") }
        }
        var largest = Float.MIN_VALUE
        var index = -1
        for (i in 0 until resultArray[0].size) {
            if (resultArray[0][i] > largest) {
                largest = resultArray[0][i]
                index = i
            }
        }
        return index
    }

    fun getNum(bitmap: Bitmap) {
        Log.d("TAGTAG","byte order->${imgData.order()}");
        bitmap.getPixels(intValues, 0, bitmap.width, 0, 0, bitmap.width, bitmap.height)
        getNum(bitmap, bitmap.width, bitmap.height)
    }

    external fun getNum(bitmap: Bitmap, width: Int, height: Int);

    fun detect(inData: ByteBuffer, result: Any) {
        Log.d("TAGTAG"," is direct ${inData.isDirect}");

        var pixel = 0
//        inData.order(ByteOrder.nativeOrder())
//        inData.rewind()
//        for (i in 0 until 28) {
//            for (j in 0 until 28) {
//                val color = intValues[pixel++]
//                val rColor = (color and 0xFF)
//                inData.putFloat(rColor.toFloat())
//            }
//        }

//        Thread.sleep(1000)

        tflite.run(inData, result)


        var largest = Float.MIN_VALUE
        var index = -1
        for (i in resultArray[0].indices) {
            if (resultArray[0][i] > largest) {
                largest = resultArray[0][i]
                index = i
            }
        }
        Log.d("TAGTAG","index is $index");
    }

//    fun detect(inData: ByteBuffer) {
//        tflite.run(imgData, result)
//    }

    /** Memory-map the model file in Assets.  */
    @Throws(IOException::class)
    private fun loadModelFile(context: Context): MappedByteBuffer {
        val fileDescriptor = context.assets.openFd(NUMBER_CLASSIFY_FILENAME)
        val inputStream =
            FileInputStream(fileDescriptor.fileDescriptor)
        val fileChannel = inputStream.channel
        val startOffset = fileDescriptor.startOffset
        val declaredLength = fileDescriptor.declaredLength
        return fileChannel.map(
            FileChannel.MapMode.READ_ONLY,
            startOffset,
            declaredLength
        )
    }
}