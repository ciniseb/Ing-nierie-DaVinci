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

import static java.lang.Math.abs;

//Inspiré par le tutoriel suivant
//http://www.instructables.com/id/A-Simple-Android-UI-Joystick/

public class Manette extends SurfaceView implements Callback, View.OnTouchListener
{
    private float centreX;
    private float centreY;
    private float rayonBase;
    private float rayonCentre;
    private JoystickListener joystickCallback;
    private Paint couleur;


    //Constructeurs
    public Manette(Context context)
    {
        super(context);
        getHolder().addCallback(this);
        getHolder().setFormat(PixelFormat.TRANSPARENT);
        setOnTouchListener(this);
        if(context instanceof JoystickListener)
        {
            joystickCallback = (JoystickListener) context;
        }
        couleur = new Paint();
        couleur.setARGB(100, 255,0,0);
    }

    public Manette(Context context, AttributeSet attrs)
    {
        super(context, attrs);
        getHolder().addCallback(this);
        getHolder().setFormat(PixelFormat.TRANSPARENT);
        setOnTouchListener(this);
        if(context instanceof JoystickListener)
        {
            joystickCallback = (JoystickListener) context;
        }
        couleur = new Paint();
        couleur.setARGB(100, 255,0,0);

    }

    public Manette(Context context, AttributeSet attrs, int defStyleAttr)
    {
        super(context, attrs, defStyleAttr);
        getHolder().addCallback(this);
        getHolder().setFormat(PixelFormat.TRANSPARENT);
        setOnTouchListener(this);
        if(context instanceof JoystickListener){
            joystickCallback = (JoystickListener) context;
        }
        couleur = new Paint();
        couleur.setARGB(100, 255,0,0);

    }


    @Override
    public void surfaceCreated(SurfaceHolder surfaceHolder)
    {
        setDimension();
        draw(centreX, centreY);

    }

    @Override
    public void surfaceChanged(SurfaceHolder surfaceHolder, int i, int i1, int i2)
    {

    }

    @Override
    public void surfaceDestroyed(SurfaceHolder surfaceHolder)
    {

    }

    @Override
    public boolean onTouch(View view, MotionEvent motionEvent)
    {
        try
        {
            if(view.equals(this))
            {
                if(motionEvent.getAction() != MotionEvent.ACTION_UP)
                {
                    float deplacement = abs(motionEvent.getY() - centreY);

                    if(deplacement < centreY)
                    {
                        draw(centreX, motionEvent.getY());
                        joystickCallback.onJoystickMoved(0, ((motionEvent.getY() - centreY) / centreY), getId());
                    }
                }
                else
                {
                    draw(centreX, centreY);
                    joystickCallback.onJoystickMoved(0, 0, getId());
                }
            }
        }catch(IOException e)
        {
            e.printStackTrace();
        }
        return true;
    }

    public void setDimension()
    {
        //Utilise les dimensions du SurfaceView pour placer le centre
        centreX = getWidth()/2;
        centreY = getHeight()/2;
        //Utilise les dimensions du SurfaceView pour calculer la grosseur du joystick
        rayonBase = Math.min(getWidth(), getHeight()) / 2.25f;
        rayonCentre = Math.min(getWidth(), getHeight()) / 2.75f;
    }

    private void draw(float x,float y)
    {
        //Dessine le joystick dans le SurfaceView
        if(getHolder().getSurface().isValid())
        {
            Canvas canvas = this.getHolder().lockCanvas();
            canvas.drawColor(Color.GREEN, PorterDuff.Mode.OVERLAY);
            Paint colorBottom = new Paint();

            colorBottom.setARGB(100, 50,50,50);
            canvas.drawCircle(centreX, centreY, rayonBase, colorBottom);
            canvas.drawCircle(x, y, rayonCentre, couleur);
            //canvas.drawRect(x-100,y-50, x+100, y+50, colorBottom);
            //canvas.drawRect(x-50,y-25, x+50, y+25, couleur);
            getHolder().unlockCanvasAndPost(canvas);
        }
    }

    //Permet de changer la couleur du baton du Joystick
    public void setColor(int r, int g, int b)
    {
        this.couleur.setARGB(100,r,g,b);
    }

    public interface JoystickListener
    {
        void onJoystickMoved(float xPercent, float yPercent, int source) throws IOException;
    }

}
