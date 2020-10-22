package com.example.reg

import android.content.Context
import android.graphics.*
import android.util.AttributeSet
import android.view.MotionEvent
import android.view.View

class DoodleVIew : View {
    private var bitmap: Bitmap? = null
    private val pathList = arrayListOf<Path>()
    private lateinit var currentPath: Path
    private var xLast = 0.0F
    private var yLast = 0.0F
    private val mPaint = Paint()

    constructor(context: Context) : super(context) {
    }

    constructor(context: Context, attributeSet: AttributeSet) : super(context, attributeSet) {

    }

    constructor(context: Context, attributeSet: AttributeSet, defStyleAttr: Int) : super(
        context,
        attributeSet,
        defStyleAttr
    ) {
    }

    init {
        mPaint.color = Color.WHITE
        mPaint.strokeWidth = 60F
        mPaint.style = Paint.Style.STROKE
        mPaint.isAntiAlias = true
        mPaint.strokeCap = Paint.Cap.ROUND
    }

    override fun onLayout(changed: Boolean, left: Int, top: Int, right: Int, bottom: Int) {
        super.onLayout(changed, left, top, right, bottom)
        val width = right - left
        val height = bottom - top
        if ((width > 0 && height > 0) and (bitmap == null || bitmap!!.width != width
                    || bitmap!!.height != height)
        ) {
            bitmap = Bitmap.createBitmap(width, height, Bitmap.Config.ARGB_8888)
        }
    }

    override fun onTouchEvent(event: MotionEvent?): Boolean {
        val x = event!!.x
        val y = event.y
        when (event.action) {
            MotionEvent.ACTION_DOWN -> {
                currentPath = Path()
                pathList.add(currentPath)
                currentPath.moveTo(x, y)
            }
            MotionEvent.ACTION_MOVE -> {
                currentPath.quadTo(xLast, yLast, (x + xLast) / 2, (y + yLast) / 2)
            }
            MotionEvent.ACTION_CANCEL, MotionEvent.ACTION_UP -> {
            }
        }
        invalidate()
        xLast = x
        yLast = y
        return true
    }

    override fun onDraw(canvas: Canvas?) {
        super.onDraw(canvas)
        pathList.forEach {
            canvas?.drawPath(it, mPaint)
        }
    }

    fun getBitmap(): Bitmap {
        val canvas = Canvas(bitmap!!)
        val p = Path()
        bitmap!!.eraseColor(Color.BLACK)
        pathList.forEach {
            p.addPath(it)
        }
        val rectF = RectF()
        p.computeBounds(rectF, false)
        canvas.save()
        canvas.translate(
            (bitmap!!.width - rectF.right - rectF.left) / 2,
            (bitmap!!.height - rectF.bottom - rectF.top) / 2
        )
        canvas.drawPath(p, mPaint)
        canvas.restore()
        canvas.setBitmap(null)
        return bitmap!!
    }

    fun clear() {
        pathList.clear()
        postInvalidate()
    }
}