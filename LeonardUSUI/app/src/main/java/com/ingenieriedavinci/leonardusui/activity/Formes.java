package com.ingenieriedavinci.leonardusui.activity;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.Gravity;
import android.view.View;
import android.widget.Button;
import android.widget.Toast;

import com.ingenieriedavinci.leonardusui.R;

import java.util.concurrent.TimeUnit;

import static com.ingenieriedavinci.leonardusui.activity.Main.player;

public class Formes extends AppCompatActivity
{
    //Variables
    Button boutonsFormes[];
    int typeFormes;
    ListeFormes listeFormes;

    @Override
    protected void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.layout_formes);

        Toast message = Toast.makeText(getApplicationContext(),"Choisis le dessin que tu veux faire!", Toast.LENGTH_LONG);
        message.setGravity(Gravity.CENTER, 0, 0);
        message.show();

        //Attributions
        listeFormes = new ListeFormes();
        boutonsFormes = new Button[9];
        for(typeFormes = 0 ; typeFormes < 9 ; typeFormes++)
        {
            final String boutonID = "bouton" + typeFormes;//String de la référence du bouton xml
            int resID = getResources().getIdentifier(boutonID, "id", getPackageName());//Créer l'ID
            boutonsFormes[typeFormes] = ((Button) findViewById(resID));//Passage de l'ID
        }

        //Comportements
        boutonsFormes[0].setOnClickListener(new View.OnClickListener()
        {
            @Override
            public void onClick(View view)
            {
                listeFormes.startActivity(0, getApplicationContext());
            }
        });
        boutonsFormes[1].setOnClickListener(new View.OnClickListener()
        {
            @Override
            public void onClick(View view)
            {
                Toast message = Toast.makeText(getApplicationContext(),"Cette option n'est pas encore disponible...", Toast.LENGTH_LONG);
                message.setGravity(Gravity.CENTER, 0, 0);
                message.show();
            }
        });
        boutonsFormes[2].setOnClickListener(new View.OnClickListener()
        {
            @Override
            public void onClick(View view)
            {
                Toast message = Toast.makeText(getApplicationContext(),"Cette option n'est pas encore disponible...", Toast.LENGTH_LONG);
                message.setGravity(Gravity.CENTER, 0, 0);
                message.show();
            }
        });
        boutonsFormes[3].setOnClickListener(new View.OnClickListener()
        {
            @Override
            public void onClick(View view)
            {
                listeFormes.startActivity(3, getApplicationContext());
            }
        });
        boutonsFormes[4].setOnClickListener(new View.OnClickListener()
        {
            @Override
            public void onClick(View view)
            {
                Toast message = Toast.makeText(getApplicationContext(),"Cette option n'est pas encore disponible...", Toast.LENGTH_LONG);
                message.setGravity(Gravity.CENTER, 0, 0);
                message.show();
            }
        });
        boutonsFormes[5].setOnClickListener(new View.OnClickListener()
        {
            @Override
            public void onClick(View view)
            {
                listeFormes.startActivity(5, getApplicationContext());
            }
        });
        boutonsFormes[6].setOnClickListener(new View.OnClickListener()
        {
            @Override
            public void onClick(View view)
            {
                listeFormes.startActivity(6, getApplicationContext());
            }
        });
        boutonsFormes[7].setOnClickListener(new View.OnClickListener()
        {
            @Override
            public void onClick(View view)
            {
                Toast message = Toast.makeText(getApplicationContext(),"Cette option n'est pas encore disponible...", Toast.LENGTH_LONG);
                message.setGravity(Gravity.CENTER, 0, 0);
                message.show();
            }
        });
        boutonsFormes[8].setOnClickListener(new View.OnClickListener()
        {
            @Override
            public void onClick(View view)
            {
                Toast message = Toast.makeText(getApplicationContext(),"Cette option n'est pas encore disponible...", Toast.LENGTH_LONG);
                message.setGravity(Gravity.CENTER, 0, 0);
                message.show();
            }
        });

        //Comportements
        /*debut.setOnClickListener(new View.OnClickListener()
        {
            @Override
            public void onClick(View view)
            {
                startActivity(new Intent(Main.this, Formes.class));//Ouvrir l'activité des formes
            }
        });*/
    }
}
