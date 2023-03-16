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

    // vectorul ok[] retine pentru fiecare site din baza de date
    // valoarea 1 daca acesta se potriveste cautarii, sau 0 in caz contrar

    // vectorul ok_1[] retine pentru fiecare site valoarea 1 daca acesta
    // contine elementele specificate in cautare, sau 0 in sens contrar

    // vectorul ok_2[] retine pentru fiecare site valoarea 1 daca acesta
    // nu contine niciunul dintre cuvintele ce trebuie excluse la cautare,
    // sau 0 in sens contrar
    int i, ok[ct_pag], ok_1[ct_pag], ok_2[ct_pag];

    // presupunem ca niciun site nu se potriveste cautarii,
    // niciunul nu contine elementele specificate in cautare, iar
    // site-urile nu contin niciunul dintre cuvintele ce trebuie excluse
    for(i = 0; i < ct_pag; i++)
    {
        ok[i] = 0;
        ok_1[i] = 0;
        ok_2[i] = 1;
    }
    
    // vectorul cuv[] retine cautarea introdusa la tastatura
    char cuv[100], *p;

    fgets(cuv, 100, stdin);

    // adaugam caracterul \n la finalul paragrafului fiecarui site pentru 
    // o cautare precisa a secventelor aflate la final de paragraf
    for(i = 0; i < ct_pag; i++)
        strcat(siteuri.v[i].cont.c, "\n");

    // vectorul caut[] retine cuvantul/ secventa careia trebuie sa ii verificam
    // existenta sau inexistenta in continutul site-urilor
    char caut[100] = "";

    // variabila "gasit_secventa" retine valoarea 0 daca nu ne aflam la
    // momentul curent in interiorul unei secvente, sau 0 in sens contrar
    int gasit_secventa = 0;

    // identificam elementele separate de spatii introduse in cautare, pe rand

    p = strtok( cuv, " \n");
    // cat timp am gasit un astfel de element
    while(p != NULL)
    {
        // am gasit un cuvant ce trebuie exclus din cautare
        if(*p == '-')
        {
            // copiem cuvantul in vectorul caut[]
            strcpy(caut, p + 1);

            // verificam inexistenta cuvantului
            verif_nu_exist(siteuri, ct_pag, caut, ok_2);

            // stergem continutul vectorului caut[]
            strcpy(caut, "");
        }
        // am gasit un cuvant ce nu trebuie exclus si nu se afla
        // la inceput/ sfarsit de secventa
        else if (*p != '"' && strcmp((p + strlen(p) - 1), "\"") != 0)
        {
            // verificam daca ne aflam in interiorul unei secvente
            if(gasit_secventa == 1)
            {
                // in caz afirmativ, adaugam cuvantul la secventa curenta
                strcat(caut, " ");
                strcat(caut, p);
            }   
            else
            {
                // in caz negativ, verificam existenta cuvantului
                strcpy(caut, p);
                verif_exist(siteuri, ct_pag, caut, ok_1);

                strcpy(caut, "");
            }         
        }
        // am gasit un cuvant la inceput de secventa
        else if (*p == '"')
        {
            // adaugam cuvantul la secventa curenta
            strcat(caut, p + 1);

            // marcam inceputul secventei
            gasit_secventa = 1;
        }
        // am gasit un cuvant la sfarsit de secventa
        else if (strcmp((p + strlen(p) - 1), "\"") == 0)
        {
            // adaugam cuvantul la secventa curenta
            strcat(caut, " ");
            strcat(caut, p);

            // stergem ghilimelele de final de secventa
            caut[strlen(caut) - 1] = '\0';

            // verificam existenta secventei formate
            verif_exist(siteuri, ct_pag, caut, ok_1);

            strcpy(caut, "");

            // am incheiat secventa
            gasit_secventa = 0;
        }
        // identificam urmatorul element despartit de spatii din cautare
        p = strtok(NULL, " \n");
    }

    // parcurgem site-urile din baza de date
    for(i = 0; i < ct_pag; i++)
        // verificam daca site-ul se potriveste cautarii
        if(ok_1[i] == 1 && ok_2[i] == 1)
            ok[i] = 1;
    
    // sortam rezultatele cautarii
    sortare(&siteuri, ct_pag, ok, compara_3);

    // afisam rezultatele cautarii
    for(i = 0; i < ct_pag; i++)
        if(ok[i] == 1)
            printf("%s\n", siteuri.v[i].URL);

    // eliberam baza de date alocata dinamic
    free_vector(&siteuri, ct_pag);
}