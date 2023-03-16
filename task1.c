#include <stdio.h>
#include <string.h>
#include "header.h"

int main()
{
    // deschidem fisierul master.txt
    FILE *f;
    f = fopen("master.txt", "rt");
    
    // initializam vectorul de site-uri
    init_vector(&siteuri, 3);

    // formam baza de date a site-urilor
    citeste_pag(&siteuri, &ct_pag, f);

    // inchidem fisierul master.txt
    fclose(f);
    
    int i;

    // afisam baza de date, conform cerintei
    for(i = 0; i < ct_pag; i++)
        printf("%s %d %s\n", siteuri.v[i].URL, siteuri.v[i].nrAcc, siteuri.v[i].titlu);

    // eliberam baza de date alocata dinamic
    free_vector(&siteuri, ct_pag);
}
