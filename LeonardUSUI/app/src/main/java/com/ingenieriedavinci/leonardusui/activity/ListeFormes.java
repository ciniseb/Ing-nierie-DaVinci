package com.ingenieriedavinci.leonardusui.activity;

import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.graphics.Bitmap;
import android.support.v4.content.ContextCompat;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.ListView;
import android.widget.TextView;

import com.ingenieriedavinci.leonardusui.model.Forme;
import com.ingenieriedavinci.leonardusui.ImageViewCarre;

import com.ingenieriedavinci.leonardusui.R;

import static com.ingenieriedavinci.leonardusui.model.Forme.Difficulte.Difficile;
import static com.ingenieriedavinci.leonardusui.model.Forme.Difficulte.Facile;
import static com.ingenieriedavinci.leonardusui.model.Forme.Difficulte.Impossible;
import static com.ingenieriedavinci.leonardusui.model.Forme.Difficulte.Normale;

public class ListeFormes extends AppCompatActivity
{
    //Statics - Types de formes
    private static final int POLYGONES = 0;
    private static final int ELLIPSES = 1;
    private static final int SPIRALES = 2;
    private static final int PARALLELOGRAMMES = 3;
    private static final int EMOJI = 4;
    private static final int ETOILES = 5;
    private static final int AUTRES = 6;

    //Variables
    int typeFormes;

    Forme[] formes;
    ArrayAdapter<Forme> adapteurFormes;
    Telecommande telecommande;

    //Constructeurs
    public ListeFormes() {}

    @Override
    protected void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_liste_formes);

        //Attributions
        typeFormes = getIntent().getExtras().getInt("typeFormes");
        telecommande = new Telecommande();

        switch(typeFormes)
        {
            case POLYGONES:
                formes = new Forme[11];//De 2 à 12 inclu
                formes[0] = new Forme("Digone","forme_polygone_digone", Facile);
                formes[1] = new Forme("Triangle","forme_polygone_triangle", Normale);
                formes[2] = new Forme("Carré","forme_polygone_carre", Facile);
                formes[3] = new Forme("Pentagone","forme_polygone_pentagone", Normale);
                formes[4] = new Forme("Hexagone","forme_polygone_hexagone", Normale);
                formes[5] = new Forme("Heptagone","forme_polygone_heptagone", Difficile);
                formes[6] = new Forme("Octogone","forme_polygone_octogone", Difficile);
                formes[7] = new Forme("Ennéagone","forme_polygone_enneagone", Difficile);
                formes[8] = new Forme("Décagone","forme_polygone_decagone", Impossible);
                formes[9] = new Forme("Hendécagone","forme_polygone_hendecagone", Impossible);
                formes[10] = new Forme("Dodécagone","forme_polygone_dodecagone", Impossible);

                break;
            case ELLIPSES:
                formes = new Forme[1];
                formes[0] = new Forme("Cercle", "forme_ellipse_cercle", Facile);

                break;
            case SPIRALES:
                formes = new Forme[1];
                formes[0] = new Forme("Spirale", "forme_spirale_spirale0", Normale);

                break;
            case PARALLELOGRAMMES:
                formes = new Forme[7];
                formes[0] = new Forme("Carré", "forme_polygone_carre", Facile);
                formes[1] = new Forme("Rectangle 80%", "forme_parallelogramme_rectangle_810", Facile);
                formes[2] = new Forme("Rectangle 60%", "forme_parallelogramme_rectangle_610", Facile);
                formes[3] = new Forme("Rectangle 40%", "forme_parallelogramme_rectangle_410", Facile);
                formes[4] = new Forme("Rectangle 20%", "forme_parallelogramme_rectangle_210", Facile);
                formes[5] = new Forme("Parallélogramme 60%\n120 degrés", "forme_parallelogramme_120_610", Normale);
                formes[6] = new Forme("Parallélogramme 100%\n80 degrés", "forme_parallelogramme_80_1010", Normale);


                break;
            case EMOJI:
                formes = new Forme[2];
                formes[0] = new Forme("Content", "forme_emotion_content", Difficile);
                formes[1] = new Forme("Triste", "forme_emotion_triste", Difficile);
                //formes[2] = new Forme("Blazé", "", Normale);

                break;
            case ETOILES:
                formes = new Forme[4];
                formes[0] = new Forme("Étoile à 5 cotés {5/2}", "forme_polygone_etoile_5_2", Difficile);
                formes[1] = new Forme("Étoile à 7 cotés {7/2}", "forme_polygone_etoile_7_2", Impossible);
                formes[2] = new Forme("Étoile à 7 cotés {7/3}", "forme_polygone_etoile_7_3", Impossible);
                formes[3] = new Forme("Étoile à 8 cotés {8/3}", "forme_polygone_etoile_8_3", Impossible);

                break;
            case AUTRES:
                formes = new Forme[3];
                formes[0] = new Forme("Informatique", "forme_autre_informatique", Impossible);
                formes[1] = new Forme("Électrique", "forme_autre_electrique", Difficile);
                formes[2] = new Forme("Croix", "forme_autre_croix", Normale);

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
                String messageBT = "#" + typeFormes + " " + position;
                telecommande.startActivity(formes[position].getImageDrawableString(), messageBT, getApplicationContext());
            }
        });

    }

    //Méthodes
    public void startActivity(int typeFormes, Context context)
    {
        Intent intent = new Intent(context, ListeFormes.class);
        intent.putExtra("typeFormes", typeFormes);
        intent.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
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
            //view.setPaddingRelative(20,20,20,20);

            ImageViewCarre imageForme = view.findViewById(R.id.imageForme);
            TextView nom = (TextView) view.findViewById(R.id.typeForme);
            TextView difficulte = (TextView) view.findViewById(R.id.difficulteForme);

            imageForme.setImageResource(getResources().getIdentifier(nomForme, "drawable", getContext().getPackageName()));
            if(!nomForme.equals("forme_autre_informatique") && !nomForme.equals("forme_autre_electrique"))
                imageForme.setColorFilter(ContextCompat.getColor(context, R.color.bleu));
            nom.setText(formes[position].getNom());
            difficulte.setText(formes[position].getDifficulte().name());
            switch(formes[position].getDifficulte())
            {
                case Facile:
                    difficulte.setTextColor(ContextCompat.getColor(context, R.color.vertUDeS));
                    break;
                case Normale:
                    difficulte.setTextColor(ContextCompat.getColor(context, R.color.jauneUDeS));
                    break;
                case Difficile:
                    difficulte.setTextColor(ContextCompat.getColor(context, R.color.rouge));
                    break;
                case Impossible:
                    difficulte.setTextColor(ContextCompat.getColor(context, R.color.mauve));
                    break;
                default:
                    break;
            }
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