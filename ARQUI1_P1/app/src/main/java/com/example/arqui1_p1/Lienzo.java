package com.example.arqui1_p1;

import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.Color;
import android.graphics.Paint;
import android.graphics.PorterDuff;
import android.util.AttributeSet;
import android.view.MenuItem;
import android.view.MotionEvent;
import android.view.View;
import android.graphics.Canvas;
import android.graphics.Path;

import androidx.annotation.NonNull;

import com.google.android.material.navigation.NavigationView;


public class Lienzo extends View{
    //Almacena los movimientos para poder pintar las lineas
    private Path drawPath;
    //Paint que contiene la pluma y el Paint de canvas
    private Paint drawPaint, canvasPaint;
    //Color inicial
    private int color = Color.BLACK;
    //Canvas
    private Canvas drawcanvas;
    //Canvas para guardar
    private Bitmap canvasBitmap;
    //Borrado
    private boolean borrado = false;

    public Lienzo(Context context, AttributeSet attrs) {
        super(context, attrs);
        setupDrawing();
    }
    void setupDrawing(){
        drawPath = new Path();
        drawPaint = new Paint();
        drawPaint.setColor(color);
        drawPaint.setAntiAlias(true);
        drawPaint.setStrokeWidth(10);
        drawPaint.setStyle(Paint.Style.STROKE);
        drawPaint.setStrokeJoin(Paint.Join.ROUND);
        drawPaint.setStrokeCap(Paint.Cap.ROUND);
        canvasPaint = new Paint(Paint.DITHER_FLAG);

    }
    @Override
    protected  void onDraw(Canvas canvas){
        canvas.drawBitmap(canvasBitmap,0,0,canvasPaint);
        canvas.drawPath(drawPath,drawPaint);

    }
    @Override
    public boolean onTouchEvent(MotionEvent event){
        float touchX = event.getX();
        float touchY = event.getY();

        switch (event.getAction()){
            case MotionEvent.ACTION_DOWN:
                drawPath.moveTo(touchX,touchY);
                break;
            case MotionEvent.ACTION_MOVE:
                drawPath.lineTo(touchX,touchY);
                break;
            case MotionEvent.ACTION_UP:
                drawPath.lineTo(touchX,touchY);
                drawcanvas.drawPath(drawPath,drawPaint);
                drawPath.reset();
                break;
            default:
                return false;
        }
        invalidate();
        return  true;
    }

    @Override
    protected void onSizeChanged(int w, int h, int oldw, int oldh) {
        super.onSizeChanged(w, h, oldw, oldh);
        canvasBitmap = Bitmap.createBitmap(w,h, Bitmap.Config.ARGB_8888);
        drawcanvas= new Canvas(canvasBitmap);
    }
    public void setBorrado(){

    }
    public void setColor(int color){
        drawPaint.setColor(color);
        invalidate();
    }
    public void nuevoLienzo(){
        drawcanvas.drawColor(0, PorterDuff.Mode.CLEAR);
        invalidate();
    }
}
