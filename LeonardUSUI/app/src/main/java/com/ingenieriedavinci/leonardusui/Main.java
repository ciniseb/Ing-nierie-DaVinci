package com.ingenieriedavinci.leonardusui;

import android.content.Intent;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;

public class Main extends AppCompatActivity
{
    //Variables
    Button debut;

    @Override
    protected void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.layout_main);

        //Attributions
        debut = (Button) findViewById(R.id.debut);

        //Comportements
        debut.setOnClickListener(new View.OnClickListener()
        {
            @Override
            public void onClick(View view)
            {
                startActivity(new Intent(Main.this, Formes.class));//Ouvrir l'activité des formes
            }
        });
    }
}