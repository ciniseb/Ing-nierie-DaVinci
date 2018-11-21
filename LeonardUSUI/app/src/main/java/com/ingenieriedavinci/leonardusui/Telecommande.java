package com.ingenieriedavinci.leonardusui;

import android.content.Context;
import android.content.Intent;
import android.support.v4.content.ContextCompat;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.Gravity;
import android.widget.Toast;

import java.io.IOException;

public class Telecommande extends AppCompatActivity implements Manette.JoystickListener
{
    //Variables
    Manette manetteGauche, manetteDroite;
    ImageViewCarre imageForme;


    @Override
    protected void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.layout_telecommande);

        Toast message = Toast.makeText(getApplicationContext(),"À toi de jouer!", Toast.LENGTH_LONG);
        message.setGravity(Gravity.CENTER, 0, 0);
        message.show();

        //Attributions
        manetteGauche = (Manette) findViewById(R.id.manetteGauche);
        manetteGauche.setZOrderOnTop(true);
        manetteGauche.setColor(105,105, 105);
        manetteDroite = (Manette) findViewById(R.id.manetteDroite);
        manetteDroite.setZOrderOnTop(true);
        manetteDroite.setColor(105,105, 105);

        imageForme = findViewById(R.id.imageForme);
        String nomForme = getIntent().getExtras().getString("nomForme");
        imageForme.setImageResource(getResources().getIdentifier(nomForme, "drawable", getPackageName()));
        if(!nomForme.equals("forme_autre_informatique") && !nomForme.equals("forme_autre_electrique"))
            imageForme.setColorFilter(ContextCompat.getColor(getApplicationContext(), R.color.bleu));
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

    //Méthodes
    public void startActivity(String nomForme, Context context)
    {
        Intent intent = new Intent(context, Telecommande.class);
        intent.putExtra("nomForme", nomForme);
        context.startActivity(intent);
    }
}