#include <stdio.h>
#include <string.h>
#include "header.h"

int main()
{
    // construim baza de date
    FILE *f;
    f = fopen("master.txt", "rt");
    
    init_vector(&siteuri, 3);
    citeste_pag(&siteuri, &ct_pag, f);
    fclose(f);

    // vectorul ok[] retine pentru fiecare site valoarea 1 daca acesta se
    // potriveste cautarii, sau 0 in caz contrar
    int i, ok[ct_pag];

    // presupunem ca niciun site nu se potriveste cautarii
    for(i = 0; i < ct_pag; i++)
        ok[i] = 0;
    
    // vectorul cuv[] retine cuvintele introduse de la tastatura
    char cuv[100], *p;

    fgets(cuv, 100, stdin);
    
    // adaugam caracterul \n la finalul paragrafului fiecarui site pentru 
    // o cautare precisa a secventelor aflate la final de paragraf
    for(i = 0; i < ct_pag; i++)
        strcat(siteuri.v[i].cont.c, "\n");
    
    // identificam cuvintele introduse la tastatura, pe rand
    
    p = strtok( cuv, " \n");
    // cat timp am gasit un cuvant
    while( p != NULL )
    {
        // verificam existenta cuvantului curent in continutul site-urilor
        verif_exist(siteuri, ct_pag, p, ok);

        // identificam urmatorul cuvant
        p = strtok( NULL, " \n"); 
    }

    // sortam rezultatele cautarii
    sortare(&siteuri, ct_pag, ok, compara_2);

    // afisam rezultatele cautarii
    for(i = 0; i < ct_pag; i++)
        if(ok[i] == 1)
            printf("%s\n", siteuri.v[i].URL);
    
    // eliberam baza de date alocata dinamic
    free_vector(&siteuri, ct_pag);
}