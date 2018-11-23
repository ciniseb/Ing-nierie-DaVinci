package com.ingenieriedavinci.leonardusui;

import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothSocket;
import android.content.Context;
import android.content.Intent;
import android.support.v4.content.ContextCompat;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.Gravity;
import android.view.View;
import android.widget.ImageButton;
import android.widget.TextView;
import android.widget.Toast;

import java.io.IOException;
import java.io.OutputStream;
import java.util.UUID;

public class Telecommande extends AppCompatActivity implements Manette.JoystickListener
{
    //Statics
    private static final int REQUEST_ENABLE_BT = 1;//TODO
    private static final String DEBUG = "Débuggage";
    private static final String ERREUR = "Erreur";
    enum ROBOT{DEBUG, AUTONOME, MANUEL} ROBOT robot;

    //Variables
    ChargementBT chargementBT = null;
    private BluetoothAdapter adapteurBT = null;
    private BluetoothSocket priseBT = null;
    private OutputStream outStream = null; //TODO

    private static final UUID MON_UUID = UUID.fromString("00001101-0000-1000-8000-00805F9B34FB");//Universel UUID
    public static String addresseMac = "00:00:00:00:00:00";//Normalement l'adresse est « private » mais nous allons procéder à des manipulations non-catholiques. Ex: "20:16:09:12:32:93"

    private static float ancienGYPercent = 0;
    private static float ancienDYPercent = 0;

    Manette manetteGauche, manetteDroite;
    ImageViewCarre imageForme;
    ImageButton toggle;

    boolean etatToggle = false;


    @Override
    protected void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.layout_telecommande);

        new ChargementBT(this, getFragmentManager()).execute();

        //Attributions
        //adapteurBT = BluetoothAdapter.getDefaultAdapter();

        manetteGauche = (Manette) findViewById(R.id.manetteGauche);
        manetteGauche.setZOrderOnTop(true);
        manetteGauche.setColor(0,0, 0);
        manetteDroite = (Manette) findViewById(R.id.manetteDroite);
        manetteDroite.setZOrderOnTop(true);
        manetteDroite.setColor(0,0, 0);

        toggle = (ImageButton) findViewById(R.id.boutonToggle);

        imageForme = (ImageViewCarre) findViewById(R.id.imageForme);
        String nomForme = getIntent().getExtras().getString("nomForme");
        imageForme.setImageResource(getResources().getIdentifier(nomForme, "drawable", getPackageName()));
        if(!nomForme.equals("forme_autre_informatique") && !nomForme.equals("forme_autre_electrique"))
            imageForme.setColorFilter(ContextCompat.getColor(getApplicationContext(), R.color.bleu));

        //Connexion BLUETOOTH
        /*addresseMac = "20:16:09:26:40:23";//ROBOT AUTONOME
        connexionBT();
        verifEtatBT();

        String messageBT = getIntent().getExtras().getString("messageBT");
        Log.d(DEBUG, messageBT);
        envoieDonnees(messageBT);

        addresseMac = "20:16:09:12:32:93";//ROBOT MANUEL
        connexionBT();
        verifEtatBT();*/

        //Comportements
        toggle.setOnClickListener(new View.OnClickListener()
        {
            @Override
            public void onClick(View view)
            {
                etatToggle = !etatToggle;
                toggle.setSelected(etatToggle);

                if(etatToggle)
                {
                    Log.d(DEBUG, "#B");
                    //envoieDonnees("#B");//TODO BLUETOOTH
                }
                else
                {
                    Log.d(DEBUG, "#H");
                    //envoieDonnees("#H");//TODO BLUETOOTH
                }
            }
        });
    }
    @Override
    public void onJoystickMoved(float xPercent, float yPercent, int source) throws IOException
    {
        String messageBT;
        switch(source)
        {
            case R.id.manetteGauche:
                if(ancienGYPercent != yPercent)
                {
                    if(-yPercent > 0)
                        messageBT = "#G+" + (-yPercent);
                    else
                        messageBT = "#G" + (-yPercent);
                    Log.d(DEBUG, messageBT);
                    //envoieDonnees(messageBT);//TODO BLUETOOTH
                    ancienGYPercent = yPercent;
                }
                break;
            case R.id.manetteDroite:
                if(ancienDYPercent != yPercent)
                {
                    if(-yPercent > 0)
                        messageBT = "#D+" + (-yPercent);
                    else
                        messageBT = "#D" + (-yPercent);
                    Log.d(DEBUG, messageBT);
                    //envoieDonnees(messageBT);//TODO BLUETOOTH
                    ancienDYPercent = yPercent;
                }
                break;
        }
    }
    @Override
    public void onResume()
    {
        super.onResume();
        //connexionBT();
    }
    @Override
    public void onPause()
    {
        super.onPause();

        /*if(outStream != null)
        {
            try
            {
                outStream.flush();
            }catch(IOException e)
            {
                Log.e(ERREUR,"Dans onPause(), flush du OutputStream échoué: " + e.getMessage() + ".");
            }
        }

        try
        {
            priseBT.close();
        }catch(IOException e2)
        {
            Log.e(ERREUR,"Dans onPause(), fermeture de la connection échouée: " + e2.getMessage() + ".");
        }*/
    }

    //Méthodes
    private void verifEtatBT()
    {
        if(adapteurBT == null)
        {
            Log.e(ERREUR,"Bluetooth Non supporté. Abandon de la misson.");
        }
        else
        {
            if(adapteurBT.isEnabled())
            {
                Toast message = Toast.makeText(getApplicationContext(),"...Bluetooth activé...", Toast.LENGTH_LONG);
                message.setGravity(Gravity.CENTER, 0, 0);
                message.show();
            }
            else
            {
                Intent enableBtIntent = new Intent(adapteurBT.ACTION_REQUEST_ENABLE);
                startActivityForResult(enableBtIntent, REQUEST_ENABLE_BT);
            }
        }
    }
    private void connexionBT()
    {
        BluetoothDevice device = adapteurBT.getRemoteDevice(addresseMac);

        try
        {
            priseBT = device.createRfcommSocketToServiceRecord(MON_UUID);
        }catch(IOException e)
        {
            Log.e(ERREUR,"Création de la connection échouée: " + e.getMessage() + ".");
        }

        adapteurBT.cancelDiscovery();

        Log.d(DEBUG, "...Connection à l'appareil...");
        try
        {
            priseBT.connect();
            //Log.d(DEBUG, "...Connection établie et lien de données ouvert...");
        }catch(IOException e)
        {
            try
            {
                priseBT.close();
            }catch(IOException e2)
            {
                Log.e(ERREUR,"Fermeture de la connection échouée" + e2.getMessage() + ".");
            }
        }

        try
        {
            outStream = priseBT.getOutputStream();
        }catch(IOException e)
        {
            Log.e(ERREUR,"Création du OutputStream échouée:" + e.getMessage() + ".");
        }
    }
    private void envoieDonnees(String message)
    {
        byte[] msgBuffer = message.getBytes();
        //Log.d(DEBUG, "...Données envoyées: " + message + "...");

        try
        {
            outStream.write(msgBuffer);
        }catch(IOException e)
        {
            Log.e(ERREUR,"Erreur pendant l'écriture: " + e.getMessage());
        }
    }
    public void startActivity(String nomForme, String messageBT, Context context)
    {
        Intent intent = new Intent(context, Telecommande.class);
        intent.putExtra("nomForme", nomForme);
        intent.putExtra("messageBT", messageBT);
        context.startActivity(intent);
    }
}