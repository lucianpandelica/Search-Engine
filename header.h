#ifndef HEADER_H
#define HEADER_H
#include <stdio.h>
#include <stdlib.h>

// constante definite pentru dimensiunile sirurilor de caractere
#define nume_pagini_max 30
#define linie_max 100
#define titlu_URL_max 50

// enumerare pentru culorile de text si de fundal
typedef enum {white, black, red, green, blue, yellow} culori_p;

// structura pentru paragraful unui site
typedef struct
{
    char *c;
    int cap, n;
}continut;

// structura pentru reprezentarea unui site in baza de date
typedef struct
{
    char URL[titlu_URL_max], titlu[titlu_URL_max];
    continut cont;
    culori_p back_color, color;
    int lung, nrAcc, checksum;
}site;

// structura de vector de site-uri
typedef struct
{
    site *v;
    int cap, n; 
}vector_site;

// antetele functiilor definite in fisierul "header.c"
void init_vector(vector_site *v, int nr);

void init_continut(continut *cont, int nr);

void check_realloc(continut *cont, char *p);

void check_CSS(site *site, char *p);

int ind_culoare(char *p);

void free_vector(vector_site *siteuri, int ct_pag);

void citeste_pag(vector_site *siteuri, int *ct_pag, FILE *f);

void adauga_date(vector_site *siteuri, char nume_pagini[], int ct_pag);

void sortare(vector_site *siteuri, int ct_pag, int ok[], int (*compara)());

int compara_2(site site1, site site2);

int compara_3(site site1, site site2);

void verif_exist(vector_site siteuri, int ct_pag, char *p, int ok[]);

void verif_nu_exist(vector_site siteuri, int ct_pag, char *p, int ok[]);

// declararea vectorului de site-uri (baza de date)
vector_site siteuri;

// declararea variabilei ce retine numarul de site-uri
int ct_pag;

// declararea vectorului ce retine nume de fisiere HTML
char nume_pagini[nume_pagini_max];

#endif