
#define VIDEO_MEMORY  0xB8000

#define MAX_SIZE 100  // Taille maximale du tableau simulé
#define GROWTH_FACTOR 2  // Facteur de croissance pour la réallocation

int array[MAX_SIZE];  // Tableau statique simulant un tableau dynamique
int size = 0;         // Taille actuelle du tableau

unsigned char inb(unsigned short port);
int is_key_ready();
void fprint(const char *str, const int y);
char *readKey();

void add_value(int value) {
    if (size >= MAX_SIZE) {
        // Si le tableau est plein, on double la capacité (simulé)
        // Il est possible d'ajuster la logique de réallocation ici selon les besoins.
        // Ici, pour cet exemple, il est limité à MAX_SIZE.
        return;  // Le tableau ne peut pas croître plus que MAX_SIZE
    }
    array[size++] = value;  // Ajouter la valeur et augmenter la taille
}


void _start() {
    while (1) {
        char *key = readKey();
        fprint(key,0);
    }
}





