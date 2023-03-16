#include <stdio.h>
#include <string.h>
#include "header.h"

// functie pentru citirea numelor de fisiere din master.txt
// si adaugarea site-urilor continute de acestea la baza de date
void citeste_pag(vector_site *siteuri, int *ct_pag, FILE *f)
{
    // initializam numarul de pagini cu 0
    *ct_pag = 0;

    // cat timp putem citi nume de fisiere din master.txt
    while( fgets(nume_pagini, nume_pagini_max, f) != NULL)
    {
        // functia fgets a adaugat la citire caracterul \n la finalul
        // vectorului "nume_pagini" in care retinem numele fisierului
        // HTML al site-ului curent, deci stergem acest caracter 
        nume_pagini[strlen(nume_pagini) - 1] = '\0';

        // adaugam site-ul curent la baza de date
        adauga_date(siteuri, nume_pagini, *ct_pag);

        // actualizam numarul de pagini
        (*ct_pag)++;
    }
}

// functie pentru adaugarea datelor unui site din fisierul sau HTML
// in baza de date
void adauga_date(vector_site *siteuri, char nume_pagini[], int ct_pag)
{
    char linie_cod[linie_max], aux; 
    char *p_inceputTitlu, *p_sfarsitTitlu;
    char *p_inceputParag, *p_sfarsitParag, *p_tagDescParag;
    char *p_style, *p_atribStyle, *p_separator;

    // deschidem fisierul HTML al site-ului
    FILE *g;
    g = fopen(nume_pagini, "rt");

    // daca se poate deschide fisierul
    if(g != NULL)
    {
        // actualizam numarul de elemente ale vectorului de site-uri
        siteuri->n = ct_pag;

        // verificam daca numarul curent de elemente depaseste 
        // capacitatea vectorului
        if( (siteuri->n) >= (siteuri->cap) )
        {
            // in caz afirmativ, realocam spatiu pentru inca 3 elemente
            siteuri->v = realloc(siteuri->v, (siteuri->cap + 3) * sizeof(site));

            // actualizam noua capacitate a vectorului
            siteuri->cap = ( (siteuri->cap) + 3 );
        }

        // citim metadatele site-ului (URL, lungimea in octeti a codului HTML,
        // numarul de accesari, checksum-ul) si le retinem in baza de date
        fscanf(g, "%s%d%d%d", (siteuri->v)[ct_pag].URL, 
                             &(siteuri->v)[ct_pag].lung, 
                             &(siteuri->v)[ct_pag].nrAcc, 
                             &(siteuri->v)[ct_pag].checksum);
        
        // alocam memorie vectorului in care retinem continutul (paragraful)
        // site-ului curent si il initializam cu \0
        init_continut( &siteuri->v[ct_pag].cont, 10);
        strcpy(siteuri->v[ct_pag].cont.c, "");

        // variabila "gasit_titlu" retine valoarea 0 daca nu am gasit inca
        // titlul, respectiv 1 in caz contrar

        // variabila "paragraf_deschis" retine valoarea 1 daca am gasit
        // tag-ul de inceput de paragraf, respectiv 0 in sens contrar

        // variabila "paragraf_incheiat" retine valoarea 1 daca am gasit
        // tag-ul de sfarsit de paragraf, respectiv 0 in sens contrar
        int gasit_titlu = 0, paragraf_deschis = 0, paragraf_incheiat = 0;

        // cat timp putem citi o linie de cod, o retinem in
        // vectorul linie_cod[]
        while( fgets(linie_cod, linie_max, g) != NULL)
        {
            // verificam daca am gasit titlul
            if(gasit_titlu == 0)
            {
                // in caz negativ, cautam tag-ul de inceput de titlu
                // in linia curenta
                p_inceputTitlu = strstr(linie_cod, "<title>");

                // verificam daca s-a gasit tag-ul
                if( p_inceputTitlu != NULL)
                {
                    // in caz afirmativ, determinam pozitia tag-ului
                    // de final de titlu
                    p_sfarsitTitlu = strstr(linie_cod, "</title>");

                    // adaugam titlul la baza de date, izoland textul ce se afla
                    // intre cele doua tag-uri de inceput si final
                    aux = *p_sfarsitTitlu;
                    *p_sfarsitTitlu = '\0';
                    strcpy((siteuri->v)[ct_pag].titlu, 
                            p_inceputTitlu + strlen("<title>"));
                    *p_sfarsitTitlu = aux; 

                    // actualizam variabila "gasit_titlu" la valoarea 1
                    gasit_titlu = 1;
                }
            }
            
            // verificam daca am gasit titlu, dar nu si un paragraf complet
            if( gasit_titlu == 1 && paragraf_incheiat == 0)
            {
                // in caz afirmativ, verificam daca nu s-a gasit inca tag de
                // deschidere de paragraf
                if(paragraf_deschis == 0)
                {
                    // in caz afirmativ, cautam tag-ul de deschidere
                    // in linia curenta
                    p_inceputParag = strstr(linie_cod, "<p");

                    // verificam daca s-a gasit tag-ul in linia curenta
                    if(p_inceputParag != NULL)
                    {
                        // in caz afirmativ, actualizam valoarea variabilei
                        // "paragraf_deschis" la 1 si cautam tag-ul de inchidere
                        // de paragraf 
                        paragraf_deschis = 1;
                        p_sfarsitParag = strstr(p_inceputParag + 2, "</p>");

                        // verficiam daca avem si tag de inchidere de paragraf
                        // in linia curenta
                        if( p_sfarsitParag != NULL)
                        {
                            // in caz afirmativ, izolam textul aflat intre
                            // inceputul tag-ului de deschidere de paragraf si 
                            // inceputul tag-ului de inchidere
                            *p_sfarsitParag = '\0';

                            // verificam daca exista suficient spatiu in
                            // vectorul de continut al site-ului curent 
                            // pentru a retine paragraful si 
                            // realocam spatiu la nevoie
                            check_realloc(&siteuri->v[ct_pag].cont,
                                          p_inceputParag);

                            // adaugam continutul la baza de date
                            strcat(siteuri->v[ct_pag].cont.c, p_inceputParag);

                            // actualizam numarul de elemente ale vectorului
                            // de continut
                            siteuri->v[ct_pag].cont.n = siteuri->v[ct_pag].cont.n + 
                                                        strlen(p_inceputParag);

                            // actualizam valoarea variabilei 
                            // "paragraf_incheiat" la 1
                            paragraf_incheiat = 1;
                        }
                        else
                        {
                            // in caz negativ, paragraful ramane deschis si
                            // adaugam, prin aceeasi procedura ca la cazul
                            // anterior, portiunea de continut gasita la 
                            // vectorul de continut
                            check_realloc(&siteuri->v[ct_pag].cont,
                                          p_inceputParag);
                            strcat(siteuri->v[ct_pag].cont.c, p_inceputParag);
                            siteuri->v[ct_pag].cont.n = siteuri->v[ct_pag].cont.n +
                                                        strlen(p_inceputParag);
                        }
                    }
                }
                else
                {
                    // in caz negativ, ne aflam in interiorul paragrafului, deci
                    // cautam in linia curenta tag-ul de inchidere de paragraf
                    p_sfarsitParag = strstr(linie_cod, "</p>");

                    // verificam daca am gasit tag de inchidere in linia curenta
                    if(p_sfarsitParag != NULL)
                    {
                        // in caz afirmativ, izolam textul aflat intre inceputul
                        // liniei si inceputul tag-ului de inchidere
                        *p_sfarsitParag = '\0';

                        // adaugam portiunea astfel determinata de continut la
                        // vectorul de continut, dupa cum am precizat anterior
                        check_realloc(&siteuri->v[ct_pag].cont, linie_cod);
                        strcat(siteuri->v[ct_pag].cont.c, linie_cod);
                        siteuri->v[ct_pag].cont.n = siteuri->v[ct_pag].cont.n +
                                                    strlen(linie_cod);

                        // actualizam valoarea variabilei "paragraf_incheiat"
                        // la 1
                        paragraf_incheiat = 1;
                    }
                    else
                    {
                        // in caz negativ, paragraful ramane deschis si ii atasam
                        // intreaga linie de cod
                        check_realloc(&siteuri->v[ct_pag].cont, linie_cod);
                        strcat(siteuri->v[ct_pag].cont.c, linie_cod);
                        siteuri->v[ct_pag].cont.n = siteuri->v[ct_pag].cont.n +
                                                    strlen(linie_cod);
                    }
                }
            } 
        }

        // vectorul in care am retinut paragraful site-ului curent
        // contine si tag-ul de deschidere al acestuia in intregime.
        // verificam daca acesta are specificate elemente de CSS

        // determinam pozitia unde se termina tag-ul de deschidere
        p_tagDescParag = strstr(siteuri->v[ct_pag].cont.c, ">");

        // verificam daca s-a gasit aceasta
        if(p_tagDescParag != NULL)
        {
            // in caz afirmativ, izolam spatiul din interiorul tag-ului
            *p_tagDescParag = '\0';

            // cautam cuvantul cheie "style" in spatiul mai sus specificat
            p_style = strstr(siteuri->v[ct_pag].cont.c, "style");

            // verificam daca s-a gasit cuvantul "style"
            if(p_style != NULL)
            {
                // in caz afirmativ, deducem ca sunt prezente elemente de CSS

                // izolam caracterele aflate intre ghilimele,
                // de dupa cuvantul "style"
                p_atribStyle = strtok(p_style, "\"");
                p_atribStyle = strtok(NULL, "\"");

                // verificam daca pentru paragraful curent sunt specificate
                // atat background-color, cat si color
                // deoarece secventa "color" este continuta in secventa
                // "background-color", verificam daca avem doua pozitii
                // diferite la care incepe secventa "color"
                if( strstr(p_atribStyle, "color:") != NULL && 
                    strstr(p_atribStyle, "background-color:") != NULL && 
                   (strstr(p_atribStyle, "color:") != 
                   (strstr(p_atribStyle, "background-color:") + 
                    strlen("background-") ) ) )
                {
                    // in caz afirmativ, izolam pe rand atribuirile de forma
                    // (background-)color:NUME_CULOARE si le trimitem pentru
                    // interpretare si adaugare in baza de date la functia
                    // check_CSS
                    p_separator = strtok(p_atribStyle, ";");
                    check_CSS(&siteuri->v[ct_pag], p_separator);
                    
                    p_separator = strtok(NULL, ";");
                    check_CSS(&siteuri->v[ct_pag], p_separator);
                }
                else
                {
                    // in caz negativ, avem fie specificat doar "color",
                    // fie doar "background-color". izolam atribuirea si
                    // o trimitem la functia check_CSS
                    p_separator = strtok(p_atribStyle, ";");
                    check_CSS(&siteuri->v[ct_pag], p_separator);
                }
            }
            else
            {
                // in caz negativ, deducem ca nu exista elemente de CSS si
                // adaugam in baza de date culorile implicite: negru pentru
                // text si alb pentru fundal 
                siteuri->v[ct_pag].color = black;
                siteuri->v[ct_pag].back_color = white;
            }
        }

        // deoarece am prelucrat continutul tag-ului de deschidere de paragraf
        // si am adaugat in baza de date eventualele elemente de CSS,
        // vom pastra in continuare in vectorul continutului site-ului curent
        // strict textul ce formeaza paragraful

        // alocam dinamic un vector auxiliar pentru suprascrierea continutului,
        // luand in calcul dimenisunea actuala a continutului. numim acest
        // vector "sir_tmp"
        char *sir_tmp = malloc(siteuri->v[ct_pag].cont.n * sizeof(char));

        // initializam vectorul "sir_tmp"
        strcpy(sir_tmp, "");

        // folosim pointerul p_tagDescParag, ce retinea sfarsitul tag-ului de
        // deschidere de paragraf pentru a copia textul paragrafului in "sir_tmp"
        strcpy(sir_tmp, p_tagDescParag + 1);

        // copiem "sir_tmp" in vectorul continutului site-ului curent
        strcpy(siteuri->v[ct_pag].cont.c, sir_tmp);

        // eliberam memoria alocata vectorului auxiliar
        free(sir_tmp);

        // inchidem fisierul HTML al site-ului curent
        fclose(g);
    }
}

// functie pentru sortarea rezultatelor cautarii
void sortare(vector_site *siteuri, int ct_pag, int ok[], int (*compara)())
{
    // variabila "max_size" retine dimensiunea maxima a sirurilor de caractere
    // folosite in baza de date
    int i, j, aux, max_size = 0;
    
    // parcurgem vectorii ce retin paragrafele site-urilor din baza de date
    // si determinam care este dimensiunea maxima a acestora
    for(i = 0; i < ct_pag; i++)
        if(siteuri->v[i].cont.cap > max_size)
            max_size = siteuri->v[i].cont.cap;

    // daca dimensiunea maxima a titlului si a URL-ului este mai mare decat
    // cea determinata mai sus in randul paragrafelor, actualizam 
    // valoarea lui "max_size"
    if(titlu_URL_max > max_size)
        max_size = titlu_URL_max;
    
    // vectorul sir_aux[] va fi folosit ca sir auxiliar in interschimbarea
    // datelor site-urilor la sortare
    char sir_aux[max_size];

    // parcurgem fiecare pereche de site-uri
    for(i = 0; i < ct_pag-1; i++)
        for(j = i + 1; j < ct_pag; j++)
            // verificam daca ambele site-uri se potrivesc cautarii
            if(ok[i] == 1 && ok[j] == 1)
                // in caz afirmativ, verificam daca ordinea site-urilor
                // trebuie inversata
                if( compara(siteuri->v[i], siteuri->v[j]) == 1 )
                {
                    // in caz afirmativ, interschimbam datele celor doua site-uri

                    // interschimbam numarul de accesari
                    aux = siteuri->v[i].nrAcc;
                    siteuri->v[i].nrAcc = siteuri->v[j].nrAcc;
                    siteuri->v[j].nrAcc = aux;
                    
                    // interschimbam continutul
                    strcpy(sir_aux, siteuri->v[i].cont.c);
                    strcpy(siteuri->v[i].cont.c, siteuri->v[j].cont.c);
                    strcpy(siteuri->v[j].cont.c, sir_aux);

                    // interschimbam URL
                    strcpy(sir_aux, siteuri->v[i].URL);
                    strcpy(siteuri->v[i].URL, siteuri->v[j].URL);
                    strcpy(siteuri->v[j].URL, sir_aux);

                    // interschimbam titlu
                    strcpy(sir_aux, siteuri->v[i].titlu);
                    strcpy(siteuri->v[i].titlu, siteuri->v[j].titlu);
                    strcpy(siteuri->v[j].titlu, sir_aux);

                    // interschimbam culori
                    aux = siteuri->v[i].color;
                    siteuri->v[i].color = siteuri->v[j].color;
                    siteuri->v[j].color = aux;

                    aux = siteuri->v[i].back_color;
                    siteuri->v[i].back_color = siteuri->v[j].back_color;
                    siteuri->v[j].back_color = aux;

                    // interschimbam lungime in octeti
                    aux = siteuri->v[i].lung;
                    siteuri->v[i].lung = siteuri->v[j].lung;
                    siteuri->v[j].lung = aux;

                    // interschimbam checksum
                    aux = siteuri->v[i].checksum;
                    siteuri->v[i].checksum = siteuri->v[j].checksum;
                    siteuri->v[j].checksum = aux;
                }
}

// functie comparator pentru task 2
int compara_2(site site1, site site2)
{
    // verificam daca este necesara schimbarea ordinii site-urilor comparate,
    // adica daca acestea fie nu sunt in ordine alfabetica, fie au continut
    // identic iar numarul de accesari ale primului site este mai mic decat
    // ale celui de-al doilea
    if( ( strcmp(site1.cont.c, site2.cont.c) > 0 ) || 
        ( strcmp(site1.cont.c, site2.cont.c) == 0 && 
        ( site1.nrAcc < site2.nrAcc) ) ) 
        // in caz afirmativ, returnam valoarea 1, conventie pentru necesitatea
        // interschimbarii
        return 1;
    else
        // altfel, returnam 0
        return 0;
}

// functie comparator pentru task 3
int compara_3(site site1, site site2)
{
    // verificam daca este necesara schimbarea ordinii site-urilor comparate,
    // adica daca numarul de accesari ale primului site este mai mic decat
    // ale celui de-al doilea
    if( site1.nrAcc < site2.nrAcc )
        // in caz afirmativ, returnam valoarea 1, conventie pentru necesitatea
        // interschimbarii
        return 1;
    else
        // altfel, returnam 0
        return 0;
}

// functie pentru verificarea existentei unei secvente de text
// in continutul (paragrafele) site-urilor din baza de date,
// impreuna cu marcarea site-urilor ce se potrivesc cautarii 
void verif_exist(vector_site siteuri, int ct_pag, char *p, int ok[])
{
    int i, j;

    // vectorul caut[] retine sirul de caractere pe care il cautam
    // in continutul site-urilor la un moment dat

    // matricea caract[][] retine caractere ce pot fi alaturate
    // secventelor pe care le cautam, la finalul acestora, precum
    // spatiu, semne de punctuatie, \n
    char caut[100], caract[7][2];

    // initializam vectorul caut[]
    strcpy(caut, "");

    // memoram in matricea caract[][] caracterele ce se pot afla
    // in mod uzual dupa cuvinte, si anume \n . , : ! ? ' '
    strcpy(caract[0], "\n");
    strcpy(caract[1], ".");
    strcpy(caract[2], ",");
    strcpy(caract[3], ":");
    strcpy(caract[4], "!");
    strcpy(caract[5], "?");
    strcpy(caract[6], " ");
    
    // cautam toate sirurile de forma | secventa*caract*|, unde
    // *caract* reprezinta orice element din matricea caract[][]
    for(j = 0; j < 7; j++)
    {
        // compunem secventa curenta in vectorul caut[]
        strcat(caut, " ");
        strcat(caut, p);
        strcat(caut, caract[j]);

        // cautam secventa in continutul fiecarui site
        // din baza de date
        for(i = 0; i < ct_pag; i++)
            // verificam daca s-a gasit secventa la site-ul curent
            if( strstr(siteuri.v[i].cont.c, caut) != NULL)
                // in caz afirmativ, marcam site-ul drept potrivit
                ok[i] = 1;
        
        // stergem continutul vectorului caut[]
        strcpy(caut, "");
    }
    
    // cautam toate sirurile de forma |secventa*caract*| aflate la
    // inceput de paragraf
    for(j = 0; j < 7; j++)
    {
        // compunem secventa curenta 
        strcat(caut, p);
        strcat(caut, caract[j]);

        for(i = 0; i < ct_pag; i++)
            // verificam daca s-a gasit secventa la inceputul
            // paragrafului site-ului curent
            if( strstr(siteuri.v[i].cont.c, caut) != NULL && 
                strstr(siteuri.v[i].cont.c, caut) == siteuri.v[i].cont.c)
                // in caz afirmativ, marcam site-ul drept potrivit
                ok[i] = 1;

        // stergem continutul vectorului caut[]
        strcpy(caut, "");
    }
    
    // cautam toate sirurile de forma |\nsecventa*caract*|
    for(j = 0; j < 7; j++)
    {
        // compunem secventa curenta
        strcat(caut, "\n");
        strcat(caut, p);
        strcat(caut, caract[j]);

        for(i = 0; i < ct_pag; i++)
            // verificam existenta pentru site-ul curent
            if( strstr(siteuri.v[i].cont.c, caut) != NULL)
                // in caz afirmativ, marcam site-ul drept potrivit
                ok[i] = 1;
        
        // stergem continutul vectorului caut[]
        strcpy(caut, "");
    }
    
    // cautam toate sirurile de forma |\nsecventa\n|
    strcat(caut, "\n");
    strcat(caut, p);
    strcat(caut, "\n");

    for(i = 0; i < ct_pag; i++)
        if( strstr(siteuri.v[i].cont.c, caut) != NULL)
            ok[i] = 1;
    strcpy(caut, "");

    // cautam toate sirurile de forma | secventa |
    strcat(caut, " ");
    strcat(caut, p);
    strcat(caut, " ");

    for(i = 0; i < ct_pag; i++)
        if( strstr(siteuri.v[i].cont.c, caut) != NULL)
            ok[i] = 1;
            
    strcpy(caut, "");
}

// functie pentru gasirea site-urilor care nu contin un anumit cuvant si
// marcarea acestora
void verif_nu_exist(vector_site siteuri, int ct_pag, char *p, int ok[])
{
    int i;
    
    // inversam logica functiei verif_exist
    for(i = 0; i < ct_pag; i++)
        ok[i] = 0;
    
    verif_exist(siteuri, ct_pag, p, ok);

    for(i = 0; i < ct_pag; i++)
        if(ok[i] == 1)
            ok[i] = 0;
        else
            ok[i] = 1;
}

// functie pentru crearea vectorului de site-uri
void init_vector(vector_site *siteuri, int nr)
{
    siteuri->v = malloc(nr * sizeof(site));
    siteuri->n = 0;
    siteuri->cap = nr;
}

// functie pentru crearea vectorului de continut
// al unui site
void init_continut(continut *cont, int nr)
{
    cont->c = malloc(nr * sizeof(char));
    cont->n = 0;
    cont->cap = nr;
}

// functie pentru verificarea necesitatii realocarii
// de memorie pentru vectorul de continut al unui site,
// la concatenarea acestuia cu un sir p
void check_realloc(continut *cont, char *p)
{
    if( cont->n + strlen(p) >= cont->cap )
    {
        cont->c = realloc(cont->c, (cont->cap+100) * sizeof(char) );
        cont->cap = cont->cap + 100;
    }
}

// functie pentru interpretarea atribuirii unei culori pentru text sau fundal
// si adaugarea ei in baza de date
void check_CSS(site *site, char *p)
{
    char *pt;

    // daca avem o atribuire de culoare de text
    if(strstr(p, "background-color:") == NULL)
    {
        pt = strstr(p, "color:");
        if(pt != NULL)
        {
            // identificam tipul de culoare si il retinem
            switch( ind_culoare(pt + strlen("color:") ) )
            {
                case 0:
                    site->color = white;
                    break;
                
                case 1:
                    site->color = black;
                    break;
                
                case 2:
                    site->color = red;
                    break;
                
                case 3:
                    site->color = green;
                    break;
                
                case 4:
                    site->color = blue;
                    break;
                
                case 5:
                    site->color = yellow;
                    break;
            }
        }  
    }
    // daca avem o atribuire de culoare de fundal
    else
    {
        pt = strstr(p, "background-color:");
        if(pt != NULL)
        {
            // identificam tipul de culoare si il retinem
            switch( ind_culoare(pt + strlen("background-color:") ) )
            {
                case 0:
                    site->back_color = white;
                    break;
                
                case 1:
                    site->back_color = black;
                    break;
                
                case 2:
                    site->back_color = red;
                    break;
                
                case 3:
                    site->back_color = green;
                    break;
                
                case 4:
                    site->back_color = blue;
                    break;
                
                case 5:
                    site->back_color = yellow;
                    break;
            }
        }  
    }
}

// functie pentru determinarea indicelui culorii date ca argument
int ind_culoare(char *p)
{
    int i;

    // m_culori retine numele culorilor cunoscute
    char m_culori[6][10] = {"white", "black", "red", "green", "blue", "yellow"};

    // comparam argumentul functiei cu elementele din baza de date de culori
    for(i = 0; i < 6; i++)
        if( strcmp(p, m_culori[i]) == 0 )
            // returnam indicele culorii
            return i;
    
    return 0;
}

// functie pentru eliberarea memoriei alocate vectorului de site-uri
void free_vector(vector_site *siteuri, int ct_pag)
{
    int i;

    // eliberam memoria alocata continutului fiecarui site (paragraf)
    for(i = 0; i < ct_pag; i++)
        free(siteuri->v[i].cont.c);
    
    // eliberam vectorul de siteuri
    free(siteuri->v);
}