package com.ingenieriedavinci.leonardusui;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;

import java.io.IOException;

public class Telecommande extends AppCompatActivity implements Manette.JoystickListener
{
    //Variables
    Manette manetteGauche, manetteDroite;

    @Override
    protected void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.layout_telecommande);

        //Attributions
        manetteGauche = (Manette) findViewById(R.id.manetteGauche);
        manetteGauche.setZOrderOnTop(true);
        manetteGauche.setColor(105,105, 105);
        manetteDroite = (Manette) findViewById(R.id.manetteDroite);
        manetteDroite.setZOrderOnTop(true);
        manetteDroite.setColor(105,105, 105);
    }

    @Override
    public void onJoystickMoved(float xPercent, float yPercent, int source) throws IOException
    {
        switch(source)
        {
            case R.id.manetteGauche:
                //TODO
                break;
            case R.id.manetteDroite:
                //TODO
                break;
        }
    }
}