package com.ingenieriedavinci.leonardusui;

public class Forme
{
    //Variables
    String nom;
    String imageDrawableString;

    //Constructeurs
    Forme(String nom, String imageDrawableString)
    {
        this.nom = nom;
        this.imageDrawableString = imageDrawableString;
    }

    //Getteurs & setteurs
    public String getNom()
    {
        return nom;
    }
    public String getImageDrawableString()
    {
        return imageDrawableString;
    }

    public void setNom(String nom)
    {
        this.nom = nom;
    }
    public void setImageDrawableString(String imageDrawableString)
    {
        this.imageDrawableString = imageDrawableString;
    }

    //Méthodes

}
