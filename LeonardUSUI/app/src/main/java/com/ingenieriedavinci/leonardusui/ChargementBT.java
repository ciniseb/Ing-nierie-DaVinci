package com.ingenieriedavinci.leonardusui;

import android.app.Dialog;
import android.app.DialogFragment;
import android.app.FragmentManager;
import android.content.Context;
import android.os.AsyncTask;
import android.view.Gravity;
import android.widget.TextView;
import android.widget.Toast;

public class ChargementBT extends AsyncTask<Void, Void, Void>
{
    //Variables
    Context context;
    FragmentManager fragmentManager;
    Transition transition;

    //Constructeurs
    public ChargementBT(Context context, FragmentManager fragmentManager)
    {
        this.context = context;
        this.fragmentManager = fragmentManager;
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
        try
        {
            Thread.sleep(10000);
        }catch(InterruptedException e)
        {
            e.printStackTrace();
        }
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

}