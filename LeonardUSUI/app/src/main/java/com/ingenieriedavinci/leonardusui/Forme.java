package com.ingenieriedavinci.leonardusui;

public class Forme
{
    //Variables
    String nom;
    String imageDrawableString;
    enum Difficulte{Facile, Normale, Difficile, Impossible} Difficulte difficulte;

    //Constructeurs
    Forme(String nom, String imageDrawableString, Difficulte difficulte)
    {
        this.nom = nom;
        this.imageDrawableString = imageDrawableString;
        this.difficulte = difficulte;
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
    public Difficulte getDifficulte()
    {
        return difficulte;
    }

    public void setNom(String nom)
    {
        this.nom = nom;
    }
    public void setImageDrawableString(String imageDrawableString)
    {
        this.imageDrawableString = imageDrawableString;
    }
    public void setDifficulte(Difficulte difficulte)
    {
        this.difficulte = difficulte;
    }

    //Méthodes

}
