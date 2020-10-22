package com.example.reg

import android.graphics.*
import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.util.Log
import android.widget.Toast
import kotlinx.android.synthetic.main.activity_main.*

class MainActivity : AppCompatActivity() {
    private val paint = Paint()
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)
        NumberClassifyer.init(applicationContext)
        btn_clear.setOnClickListener {
            doodleView.clear()
        }
        paint.isAntiAlias = true
        val bitmapB = Bitmap.createBitmap(
            NumberClassifyer.IMAGE_WIDTH
            , NumberClassifyer.IMAGE_HEIGHT, Bitmap.Config.ARGB_8888
        )
        btn_detect.setOnClickListener {
            val bitmap = doodleView.getBitmap()
            val canvas = Canvas(bitmapB)
            bitmapB.eraseColor(Color.BLACK)
            canvas.drawBitmap(
                bitmap, Rect(0, 0, bitmap.width, bitmap.height),
                Rect(0, 0, bitmapB.width, bitmapB.height), paint
            )
            canvas.setBitmap(null)
            val number = NumberClassifyer.getInstance().getNumber(bitmapB)

//            val decodeResource = BitmapFactory.decodeResource(resources, R.drawable.b4)
//            val number = NumberClassifyer.getInstance().getNumber(decodeResource)
            Log.d("TAG", "bumber is $number")
            Toast.makeText(MainActivity@this,"bumber is $number",0).show()
        }
    }
}
