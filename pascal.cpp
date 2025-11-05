// g++ -o pascal pascal.cpp -lflint -L/usr/local/lib
#include <iostream>
#include <flint/fmpz.h>
#include <vector>
#include <string>

using namespace std;

int main(int argc, char* argv[])
{
    if(argc < 2) {
        cerr << "Usage: " << argv[0] << " n" << endl;
        return 1;
    }

    // Lire n depuis la chaîne
    string s = argv[1];
    fmpz_t n;
    fmpz_init(n);
    fmpz_set_str(n, s.c_str(), 10); // base 10

    // Conversion en unsigned long si possible pour l'itération
    // Pour n trop grand (> 2^64), il faudra un algorithme spécial
    unsigned long n_ul = fmpz_get_ui(n);

    // Créer la ligne
    vector<fmpz_t> line(n_ul + 1);
    for(unsigned long k=0; k <= n_ul; k++)
        fmpz_init(line[k]);

    // C(n,0) = 1
    fmpz_set_ui(line[0], 1);

    // Calcul itératif de C(n,k+1) = C(n,k)*(n-k)/(k+1) // + efficace que fmpz_bin_uiui
    for(unsigned long k=0; k < n_ul; k++)
    {
        fmpz_mul_ui(line[k+1], line[k], n_ul - k);
        fmpz_divexact_ui(line[k+1], line[k+1], k+1); // division exacte
    }

    for(unsigned long k=0; k <= n_ul; k++)
    {
        fmpz_print(line[k]);
        cout << " ";
        fmpz_clear(line[k]);
    }
    cout << endl;

    fmpz_clear(n);
    return 0;
}
