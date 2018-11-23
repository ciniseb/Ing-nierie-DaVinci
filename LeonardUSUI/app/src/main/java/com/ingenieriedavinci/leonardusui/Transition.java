package com.ingenieriedavinci.leonardusui;

import android.app.Activity;
import android.app.Dialog;
import android.app.DialogFragment;
import android.content.DialogInterface;
import android.support.annotation.Nullable;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.Gravity;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ProgressBar;
import android.widget.Toast;

public class Transition extends DialogFragment
{
    //Variables
    private int essaies = 0;
    ProgressBar chargement;

    @Override
    public void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);
        setStyle(STYLE_NO_FRAME, android.R.style.Theme_Holo_Light);
    }
    @Nullable
    @Override
    public View onCreateView(LayoutInflater inflater, @Nullable ViewGroup container, Bundle savedInstanceState)
    {
        View view = inflater.inflate(R.layout.layout_transition, container, false);

        //Attributions
        chargement = view.findViewById(R.id.chargement);

        chargement.setIndeterminate(true);
        chargement.setIndeterminateDrawable(getResources().getDrawable(R.drawable.style_chargement, null));
        chargement.setSecondaryProgress(50);

        return view;
    }
    @Override
    public Dialog onCreateDialog(Bundle savedInstanceState)
    {
        return new Dialog(getActivity(), getTheme())
        {
            @Override
            public void onBackPressed()
            {
                Toast message;
                essaies++;
                if(essaies <= 1)
                {
                    message = Toast.makeText(getContext(),"Bien éssayé!", Toast.LENGTH_SHORT);
                }
                else if(essaies == 2)
                {
                    message = Toast.makeText(getContext(),"NOPE! Vive les plans de tests.", Toast.LENGTH_SHORT);
                }
                else
                {
                    message = Toast.makeText(getContext(),"Cet Easter Egg prouve à quel point mon équipe mon fait confiance sur Android.. XD", Toast.LENGTH_LONG);
                }
                message.setGravity(Gravity.CENTER, 0, 0);
                message.show();
            }
        };
    }
}
