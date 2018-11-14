package com.ingenieriedavinci.leonardusui;

import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Color;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.Gravity;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.ListView;
import android.widget.TextView;
import android.widget.Toast;


//import static daynight.daynnight.MainActivity.SurChangementActivity;
//import static daynight.daynnight.MainActivity.joueur;

public class ListeFormes extends AppCompatActivity
{
    //Statics - Types de formes
    private static final int POLYGONES = 0;
    private static final int ELLIPSES = 1;
    private static final int SPIRALES = 2;
    private static final int PARALLELOGRAMMES = 3;
    private static final int EMOJI = 4;
    private static final int AUTRES = 5;

    //Variables
    int typeFormes;

    Forme[] formes;
    ArrayAdapter<Forme> adapteurFormes;

    //Constructeurs
    public ListeFormes() {}

    @Override
    protected void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_liste_formes);

        //Attributions
        typeFormes = getIntent().getExtras().getInt("typeFormes");

        switch(typeFormes)
        {
            case POLYGONES:
                formes = new Forme[11];//De 2 à 12 inclu
                formes[0] = new Forme("Ligne","");
                formes[1] = new Forme("Triangle","");
                formes[2] = new Forme("Carré","");
                formes[3] = new Forme("Pentagone","");
                formes[4] = new Forme("Hexagone","");
                formes[5] = new Forme("Heptagone","");
                formes[6] = new Forme("Octogone","");
                formes[7] = new Forme("Ennéagone","");
                formes[8] = new Forme("Décagone","");
                formes[9] = new Forme("Hendécagone","");
                formes[10] = new Forme("Dodécagone","");

                break;
            case ELLIPSES:

                break;
            case SPIRALES:

                break;
            case PARALLELOGRAMMES:

                break;
            case EMOJI:

                break;
            case AUTRES:

                break;
            default:
                break;
        }

        adapteurFormes = new AdapteurArrayListeFormes(this, 0, formes);
        final ListView listeFormes = (ListView) findViewById(R.id.listeFormes);
        listeFormes.setAdapter(adapteurFormes);

        listeFormes.setOnItemClickListener(new AdapterView.OnItemClickListener()
        {
            public void onItemClick(AdapterView<?> parent, View view, int position, long id)
            {
                //TODO
                /*partieActive.getJoueurs().get(position).setCompteBancaire(partieActive.getJoueurs().get(position));
                adapteur.notifyDataSetChanged();
                BanqueZirconienne.actualisationComptesBancaires();
                finish();*/
            }
        });


    }

    //Méthodes
    public void startActivity(int typeFormes, Context context)
    {
        Intent intent = new Intent(context, ListeFormes.class);
        intent.putExtra("typeFormes", typeFormes);
        context.startActivity(intent);
    }

    //custom ArrayAdapters
    class AdapteurArrayListeFormes extends ArrayAdapter<Forme>
    {

        private Context context;
        private Forme[] formes;

        public AdapteurArrayListeFormes(Context context, int resource, Forme[] objects)
        {
            super(context, resource, objects);

            this.context = context;
            this.formes = objects;
        }

        public View getView(int position, View convertView, ViewGroup parent)
        {
            String nomForme = formes[position].getImageDrawableString();

            //View d'un placement
            LayoutInflater inflater = (LayoutInflater) context.getSystemService(Activity.LAYOUT_INFLATER_SERVICE);
            View view = inflater.inflate(R.layout.layout_bouton_forme, null);
            view.setPaddingRelative(20,20,20,20);

            TextView nom = (TextView) view.findViewById(R.id.typeForme);
            ImageViewCarre imageForme = view.findViewById(R.id.imageForme);

            Bitmap bitmap = BitmapFactory.decodeResource(context.getResources(), getResources().getIdentifier(nomForme, "drawable", getContext().getPackageName()));
            bitmap = getBitmapGrandeur(bitmap, 70);
            imageForme.setImageBitmap(bitmap);

            //view.setBackgroundColor(Color.WHITE);
            //view.setClickable(false);

            return view;
        }

        //Méthodes
        public Bitmap getBitmapGrandeur(Bitmap image, int maxSize)
        {
            int width, height;
            if(image != null)
            {
                width = image.getWidth();
                height = image.getHeight();

                float bitmapRatio = (float) width / (float) height;
                if (bitmapRatio > 1)
                {
                    width = maxSize;
                    height = (int) (width / bitmapRatio);
                } else
                {
                    height = maxSize;
                    width = (int) (height * bitmapRatio);
                }
                return Bitmap.createScaledBitmap(image, width, height, true);
            }
            else
            {
                return image;
            }
        }
    }
}


/*

*/