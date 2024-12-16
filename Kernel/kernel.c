/*
Explication de la mémoire vidéo
index linéaire = y * 80 + x
ou y est la ligne (0 à 24)
ou x est la colonne (0 à 79)

dimension de l'écran en x86 : 80x25

adresse mémoire vidéo = 0xB8000 + (index linéaire * 2)

*/

//on call les fonctions pour les tests

extern void fprint(const char *str, const int y);

void _start(){
    fprint("Welcome to Oscour my OS",0);
    fprint("Made by Matheo   0.4", 1);
}


