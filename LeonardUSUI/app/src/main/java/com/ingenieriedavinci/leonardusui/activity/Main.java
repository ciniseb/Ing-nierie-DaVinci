package com.ingenieriedavinci.leonardusui.activity;

import android.content.Intent;
import android.graphics.PixelFormat;
import android.media.MediaPlayer;
import android.net.Uri;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.VideoView;

import com.ingenieriedavinci.leonardusui.R;

public class Main extends AppCompatActivity
{
    //Statics
    public static final String DEBUG = "Débuggage";
    public static final String ERREUR = "Erreur";

    //Variables
    Button debut;
    VideoView couverture;

    @Override
    protected void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.layout_main);

        //Attributions
        debut = (Button) findViewById(R.id.debut);
        couverture = (VideoView) findViewById(R.id.couverture);

        getWindow().setFormat(PixelFormat.UNKNOWN);

        //Comportements
        debut.setOnClickListener(new View.OnClickListener()
        {
            @Override
            public void onClick(View view)
            {
                startActivity(new Intent(Main.this, Formes.class));//Ouvrir l'activité des formes
            }
        });

        String uriPath = "android.resource://" + getPackageName() + "/" + R.raw.geometry_vortex;
        Uri uri = Uri.parse(uriPath);
        couverture.setVideoURI(uri);
        couverture.requestFocus();
        couverture.start();

        couverture.setOnPreparedListener(new MediaPlayer.OnPreparedListener()
        {
            @Override
            public void onPrepared(MediaPlayer mediaPlayer)
            {
                mediaPlayer.setLooping(true);
                mediaPlayer.setVolume(0,0);

            }
        });
    }

    @Override
    protected void onResume()
    {
        super.onResume();
        couverture.start();
    }

}