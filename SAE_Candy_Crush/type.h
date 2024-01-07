#ifndef TYPE_H
#define TYPE_H
#include <sys/types.h>
#include <vector>

typedef short contenueDUneCase;
typedef std::vector <contenueDUneCase> CVLigne; // un type représentant une ligne de la grille
typedef std::vector <CVLigne> CMatrice; // un type représentant la grille
using CInt = std::pair<size_t, size_t>;



#endif // TYPE_H
