package com.ingenieriedavinci.leonardusui;

import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothSocket;
import android.content.Intent;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.Toast;

import java.io.IOException;
import java.io.OutputStream;
import java.util.UUID;

public class Main extends AppCompatActivity
{
    //Statics
    enum TAG{DEBUG, AUTONOME, MANUEL} TAG tag;
    private static final String DEBUG = "Débuggage";
    private static final int REQUEST_ENABLE_BT = 1;//TODO

    //Variables
    private BluetoothAdapter adapteurBT = null;
    private BluetoothSocket connectionBT = null;
    private OutputStream outStream = null; //TODO

    private static final UUID MON_UUID = UUID.fromString("00001101-0000-1000-8000-00805F9B34FB");//Universel UUID
    public static String addresseMac = "00:00:00:00:00:00";//Normalement l'adresse est « private » mais nous allons précder à des manipulations non-catholiques. Ex: "20:16:09:12:32:93"

    Button debut;

    @Override
    protected void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.layout_main);

        //Attributions
        debut = (Button) findViewById(R.id.debut);
        adapteurBT = BluetoothAdapter.getDefaultAdapter();

        adapteurBT = BluetoothAdapter.getDefaultAdapter();
        checkBTState();

        //Comportements
        debut.setOnClickListener(new View.OnClickListener()
        {
            @Override
            public void onClick(View view)
            {
                envoieDonnees(TAG.DEBUG, "B");
                startActivity(new Intent(Main.this, Formes.class));//Ouvrir l'activité des formes
            }
        });
    }
    @Override
    public void onResume()
    {
        super.onResume();

        Log.d(DEBUG, "...Dans onResume - Attempting client connect...");

        // Set up a pointer to the remote node using it's address.
        BluetoothDevice device = adapteurBT.getRemoteDevice(addresseMac);

        // Two things are needed to make a connection:
        //   A MAC address, which we got above.
        //   A Service ID or UUID.  In this case we are using the
        //     UUID for SPP.
        try
        {
            connectionBT = device.createRfcommSocketToServiceRecord(MON_UUID);
        }catch(IOException e)
        {
            errorExit("ALERTE", "Dans onResume(), création de la connection échouée: " + e.getMessage() + ".");
        }

        // Discovery is resource intensive.  Make sure it isn't going on
        // when you attempt to connect and pass your message.
        adapteurBT.cancelDiscovery();

        // Establish the connection.  This will block until it connects.
        Log.d(DEBUG, "...Connecting to Remote...");
        try
        {
            connectionBT.connect();
            Log.d(DEBUG, "...Connection established and data link opened...");
        }catch(IOException e)
        {
            try
            {
                connectionBT.close();
            }catch(IOException e2)
            {
                errorExit("ALERTE", "Dans onResume(), fermeture de la connection échouée" + e2.getMessage() + ".");
            }
        }

        // Create a data stream so we can talk to server.
        Log.d(DEBUG, "...Creating Socket...");

        try
        {
            outStream = connectionBT.getOutputStream();
        }catch(IOException e)
        {
            errorExit("ALERTE", "Dans onResume(), création du OutputStream échouée:" + e.getMessage() + ".");
        }
    }

    @Override
    public void onPause()
    {
        super.onPause();

        Log.d(DEBUG, "...Dans onPause()...");

        if(outStream != null)
        {
            try
            {
                outStream.flush();
            }catch(IOException e)
            {
                errorExit("ALERTE", "Dans onPause(), flush du OutputStream échoué: " + e.getMessage() + ".");
            }
        }

        try
        {
            connectionBT.close();
        }catch(IOException e2)
        {
            errorExit("ALERTE", "Dans onPause(), fermeture de la connection échouée: " + e2.getMessage() + ".");
        }
    }

    //Méthodes
    private void envoieDonnees(TAG tag, String message)
    {
        switch(tag)
        {
            case MANUEL:
                addresseMac = "20:16:09:12:32:93";
                break;
            case AUTONOME:
                //addresseMac = "00:00:00:00:00:00";
                break;
            default://mode DEBUG
                addresseMac = "20:16:09:12:32:93";
                break;
        }

        byte[] msgBuffer = message.getBytes();
        Log.d(DEBUG, "...Données envoyées: " + message + "...");

        try
        {
            outStream.write(msgBuffer);
        }catch(IOException e)
        {
            String msg = "In onResume() and an exception occurred during write: " + e.getMessage();
            if(addresseMac.equals("00:00:00:00:00:00"))
                msg = msg + ".\n\nUpdate your server address from 00:00:00:00:00:00 to the correct address on line 37 in the java code";
            msg = msg +  ".\n\nCheck that the SPP UUID: " + MON_UUID.toString() + " exists on server.\n\n";

            errorExit("Fatal Error", msg);
        }
    }
    private void checkBTState()
    {
        // Check for Bluetooth support and then check to make sure it is turned on

        // Emulator doesn't support Bluetooth and will return null
        if(adapteurBT == null)
        {
            errorExit("ALERTE", "Bluetooth Non supporté. Abandon de la misson.");
        }
        else
        {
            if(adapteurBT.isEnabled())
            {
                Log.d(DEBUG, "...Bluetooth activé...");
            }
            else
            {
                //Prompt user to turn on Bluetooth
                Intent enableBtIntent = new Intent(adapteurBT.ACTION_REQUEST_ENABLE);
                startActivityForResult(enableBtIntent, REQUEST_ENABLE_BT);
            }
        }
    }

    private void errorExit(String title, String message)
    {
        Toast msg = Toast.makeText(getBaseContext(), title + " - " + message, Toast.LENGTH_SHORT);
        msg.show();
        finish();
    }
}