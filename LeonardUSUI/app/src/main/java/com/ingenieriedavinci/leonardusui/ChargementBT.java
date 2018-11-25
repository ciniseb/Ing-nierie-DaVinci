package com.ingenieriedavinci.leonardusui;

import android.app.Dialog;
import android.app.DialogFragment;
import android.app.FragmentManager;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothSocket;
import android.content.Context;
import android.content.Intent;
import android.os.AsyncTask;
import android.util.Log;
import android.view.Gravity;
import android.widget.TextView;
import android.widget.Toast;

import java.io.IOException;
import java.io.OutputStream;

import static com.ingenieriedavinci.leonardusui.Main.DEBUG;
import static com.ingenieriedavinci.leonardusui.Main.ERREUR;
import static com.ingenieriedavinci.leonardusui.Telecommande.MON_UUID;
import static com.ingenieriedavinci.leonardusui.Telecommande.REQUEST_ENABLE_BT;
import static com.ingenieriedavinci.leonardusui.Telecommande.adapteurBT;
import static com.ingenieriedavinci.leonardusui.Telecommande.outStream;
import static com.ingenieriedavinci.leonardusui.Telecommande.priseBT;
import static com.ingenieriedavinci.leonardusui.Telecommande.addresseMac;

public class ChargementBT extends AsyncTask<Void, Void, Void>
{
    //Variables
    private Context context;
    private FragmentManager fragmentManager;
    private Transition transition;

    private String messageBT;

    //Constructeurs
    public ChargementBT(Context context, FragmentManager fragmentManager, String messageBT)
    {
        this.context = context;
        this.fragmentManager = fragmentManager;

        this.messageBT = messageBT;
    }

    @Override
    protected void onPreExecute()
    {
        super.onPreExecute();

        transition = new Transition();
        transition.setCancelable(false);
        transition.show(fragmentManager, "Transition");
    }
    @Override
    protected Void doInBackground(Void... ignore)
    {

        //Connexion BLUETOOTH
        addresseMac = "20:16:09:26:40:23";//ROBOT AUTONOME
        connexionBT();

        envoieDonnees(messageBT);

        addresseMac = "20:16:09:12:32:93";//ROBOT MANUEL
        connexionBT();

        return null;
    }
    @Override
    protected void onPostExecute(Void ignore)
    {
        transition.dismiss();

        Toast message = Toast.makeText(context,"À toi de jouer!", Toast.LENGTH_LONG);
        message.setGravity(Gravity.CENTER, 0, 0);
        message.show();
    }

    //Méthodes
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
            Log.d(DEBUG, "...Connection établie et lien de données ouvert...");
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
        Log.d(DEBUG, "...Données envoyées: " + message + "...");

        try
        {
            outStream.write(msgBuffer);
        }catch(IOException e)
        {
            Log.e(ERREUR,"Erreur pendant l'écriture: " + e.getMessage());
        }
    }
}