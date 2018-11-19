package com.ingenieriedavinci.leonardusui;

import android.content.Context;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.graphics.PixelFormat;
import android.graphics.PorterDuff;
import android.util.AttributeSet;
import android.view.MotionEvent;
import android.view.SurfaceHolder;
import android.view.SurfaceHolder.Callback;
import android.view.SurfaceView;
import android.view.View;

import java.io.IOException;

/**
 * Created by Antoine Mascolo on 2018-04-04.
 */
//Inspiré par le tutoriel suivant
//http://www.instructables.com/id/A-Simple-Android-UI-Joystick/

public class Manette extends SurfaceView implements Callback, View.OnTouchListener {

    private float centerX;
    private float centerY;
    private float jsBottom;
    private float jsTop;
    private JoystickListener joystickCallback;
    private Paint color;


    //Constructeurs
    public Manette(Context context) {
        super(context);
        getHolder().addCallback(this);
        getHolder().setFormat(PixelFormat.TRANSPARENT);
        setOnTouchListener(this);
        if(context instanceof JoystickListener){
            joystickCallback = (JoystickListener) context;
        }
        color = new Paint();
        color.setARGB(100, 255,0,0);
    }

    public Manette(Context context, AttributeSet attrs) {
        super(context, attrs);
        getHolder().addCallback(this);
        getHolder().setFormat(PixelFormat.TRANSPARENT);
        setOnTouchListener(this);
        if(context instanceof JoystickListener){
            joystickCallback = (JoystickListener) context;
        }
        color = new Paint();
        color.setARGB(100, 255,0,0);

    }

    public Manette(Context context, AttributeSet attrs, int defStyleAttr) {
        super(context, attrs, defStyleAttr);
        getHolder().addCallback(this);
        getHolder().setFormat(PixelFormat.TRANSPARENT);
        setOnTouchListener(this);
        if(context instanceof JoystickListener){
            joystickCallback = (JoystickListener) context;
        }
        color = new Paint();
        color.setARGB(100, 255,0,0);

    }


    @Override
    public void surfaceCreated(SurfaceHolder surfaceHolder) {
        setDimension();
        draw(centerX,centerY);

    }

    @Override
    public void surfaceChanged(SurfaceHolder surfaceHolder, int i, int i1, int i2) {

    }

    @Override
    public void surfaceDestroyed(SurfaceHolder surfaceHolder) {

    }

    @Override
    public boolean onTouch(View view, MotionEvent motionEvent) {

        if(view.equals(this)){
            float displacement = (float) Math.sqrt(Math.pow(motionEvent.getX() - centerX, 2) + Math.pow(motionEvent.getY() - centerY, 2));
            try {
                //Vérifie quel est le type de l'action
                if(motionEvent.getAction() != MotionEvent.ACTION_UP){
                    //Si est autre chose que de relacher l'écran
                    if (displacement < jsBottom){
                        //si le contact à lieu à l'interieur de la base du joystick
                        //Affecte les valeurs de direction au joystick
                        draw(motionEvent.getX(), motionEvent.getY());
                        joystickCallback.onJoystickMoved((motionEvent.getX() - centerX) / jsBottom, ((motionEvent.getY() - centerY) / jsBottom)*-1, getId());

                    }
                    else{
                        //si le contact à ieu à l'exterieur de la base
                        //Limite l'affichage du joystick à la limite de la base
                        float ratio = jsBottom / displacement;
                        float constrainedX = centerX + (motionEvent.getX() - centerX) * ratio;
                        float constrainedY = centerY + (motionEvent.getY() - centerY) * ratio;
                        //Affecte les valeurs de direction au joystick
                        draw(constrainedX, constrainedY);
                        joystickCallback.onJoystickMoved((constrainedX - centerX) / jsBottom, ((constrainedY - centerY) / jsBottom) *-1, getId());
                    }
                }else{
                    //Si on relache l'écran
                    //recentre le joystick
                    draw(centerX,centerY);
                    joystickCallback.onJoystickMoved(0, 0, getId());
                }
            } catch (IOException e) {
                e.printStackTrace();
            }
        }
        return true;
    }

    public void setDimension(){
        //Utilise les dimensions du SurfaceView pour placer le centre
        centerX = getWidth()/2;
        centerY = getHeight()/2;
        //Utilise les dimensions du SurfaceView pour calculer la grosseur du joystick
        jsBottom = Math.min(getWidth(), getHeight()) / 3;
        jsTop = Math.min(getWidth(), getHeight()) / 7;

    }

    private void draw(float x,float y){

        //Dessine le joystick dans le SurfaceView
        if(getHolder().getSurface().isValid()){
            Canvas canvas = this.getHolder().lockCanvas();
            canvas.drawColor(Color.TRANSPARENT, PorterDuff.Mode.CLEAR);
            Paint colorBottom = new Paint();

            colorBottom.setARGB(100, 50,50,50);
            canvas.drawCircle(centerX, centerY, jsBottom, colorBottom);
            canvas.drawCircle(x, y, jsTop, color);
            getHolder().unlockCanvasAndPost(canvas);

        }

    }

    //Permet de changer la couleur du baton du Joystick
    public void setColor(int r, int g, int b) {
        this.color.setARGB(100,r,g,b);
    }

    public interface JoystickListener
    {
        void onJoystickMoved(float xPercent, float yPercent, int source) throws IOException;
    }

}
