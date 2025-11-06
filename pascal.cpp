// g++ -o pascal pascal.cpp -lflint -L/usr/local/lib
#include <iostream>
#include <string>
#include <climits>
#include <flint/fmpz.h>

using namespace std;

// Programme : afficher la n-ième ligne du triangle de Pascal (coefficients binomiaux)
// Utilisation : ./pascal_fmpz <n>  ou  echo <n> | ./pascal_fmpz
// - n peut être un entier très grand (lu en base 10 dans un fmpz)
// - le programme n'affiche la ligne complète que si n tient dans un unsigned long
//   (il est impossible de boucler n fois si n dépasse la capacité d'indexation du programme).
// - tout est calculé en fmpz (entiers de FLINT) pour garder la précision.

int main(int argc, char** argv)
{
    string s;

    if (argc >= 2) {
        s = argv[1];
    } else {
        if (! (cin >> s)) {
            cerr << "Erreur : aucun entier fourni en argument ou stdin\n";
            return 1;
        }
    }

    fmpz_t n;
    fmpz_init(n);
    if (fmpz_set_str(n, s.c_str(), 10) != 0) {
        cerr << "Erreur : impossible de parser l'entier '" << s << "'\n";
        fmpz_clear(n);
        return 2;
    }

    // Cas n < 0
    if (fmpz_sgn(n) < 0) {
        cerr << "Erreur : n doit etre >= 0\n";
        fmpz_clear(n);
        return 3;
    }

    // On ne peut pas itérer jusqu'à n si n ne tient pas dans un unsigned long.
    // On vérifie donc si n tient dans un unsigned long (fmpz_fits_ulong_p).
    // Vérification que n tient dans un unsigned long : n <= ULONG_MAX
    if (fmpz_cmp_ui(n, ULONG_MAX) > 0) {
        cerr << "n est trop grand pour iterer (ne tient pas dans unsigned long).\n";
        cerr << "Impossible d'afficher la ligne complete du triangle de Pascal dans ce cas." << endl;
        fmpz_clear(n);
        return 4;
    }

    unsigned long n_ui = fmpz_get_ui(n);

    // On imprimera les coef de la ligne n : C(n,0) ... C(n,n)
    // On utilise une formule multiplicative en fmpz :
    //   C(n,k) = C(n,k-1) * (n - k + 1) / k
    // avec division exacte par k (k est un entier petit, on utilise fmpz_divexact_ui).

    fmpz_t c;
    fmpz_init(c);
    fmpz_set_ui(c, 1); // C(n,0) = 1

    // Impression de la première valeur
    char* buf = fmpz_get_str(NULL, 10, c);
    cout << buf;
    flint_free(buf);

    for (unsigned long k = 1; k <= n_ui; ++k) {
        unsigned long mult = n_ui - k + 1; // tient dans unsigned long parce que n_ui tient
        // c = c * mult
        fmpz_mul_ui(c, c, mult);
        // c = c / k  (division exacte)
        fmpz_divexact_ui(c, c, k);
        // afficher c
        buf = fmpz_get_str(NULL, 10, c);
        cout << ' ' << buf;
        flint_free(buf);
    }

    cout << endl;

    fmpz_clear(c);
    fmpz_clear(n);
    return 0;
}
