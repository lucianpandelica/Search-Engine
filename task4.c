#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "header.h"

// roteste la dreapta bitii lui x cu k pozitii
int rotr(char x, int k)
{
    // printr-o rotire a lui x cu multiplu de 8 biti, obtinem tot x

    // efectuam rotirea aplicand operatia OR intre rezultatul deplasarii lui x 
    // la stanga cu diferenta dintre 8 si restul impartirii lui k la 8
    // si rezultatul deplasarii lui x la dreapta cu restul impartirii lui k la 8

    // aplicam apoi operatia AND intre rezultatul obtinut mai sus si 
    // valoarea 255 pentru a pastra doar ultimii 8 biti din numarul obtinut
    return ( x << (8 - (k % 8)) | x >> (k % 8) ) & 255;
}

// roteste la stanga bitii lui x cu k pozitii
int rotl(char x, int k)
{
    // printr-o rotire a lui x cu multiplu de 8 biti, obtinem tot x
    
    // efectuam rotirea aplicand operatia OR intre rezultatul deplasarii lui x 
    // la stanga cu restul impartirii lui k la 8 si rezultatul deplasarii lui x 
    // la dreapta diferenta dintre 8 si restul impartirii lui k la 8

    // aplicam apoi operatia AND intre rezultatul obtinut mai sus si 
    // valoarea 255 pentru a pastra doar ultimii 8 biti din numarul obtinut
    return ( x << (k % 8) | x >> (8 - (k % 8)) ) & 255;
}

int main()
{
    FILE *f;
    
    // vectorul URL_crt[] retine URL-ul site-ului pe care il verificam
    // la momentul curent

    // vectorul URL_cautat[] retine, pe rand, fiecare dintre URL-urile
    // introduse la input, pe care le avem de cautat

    char URL_crt[titlu_URL_max], URL_cautat[titlu_URL_max], x;

    // variabila "lung_crt" retine lungimea in octeti a codului HTML
    // pentru site-ul pe care il verificam la momentul curent

    // variabila "nrAcc_crt" retine numarul de accesari
    // pentru site-ul pe care il verificam la momentul curent

    // variabila "checksum_crt" retine checksum-ul specificat in fisierul
    // site-ului pe care il verificam la momentul curent

    // variabila "lung" retine lungimea in octeti a partii de cod HTML pe care
    // am parcurs-o pana la momentul curent, pentru site-ul pe care il verificam

    // variabila "gasit" retine valoarea 1 daca am gasit URL-ul cautat la
    // momentul curent, respectiv 0 in caz contrar 

    int lung_crt, nrAcc_crt, checksum_crt, lung = 0, gasit = 0;

    // variabila "checksum" retine checksum-ul site-ului pe care il verificam
    // la un moment dat, calculat conform precizarilor din cerinta
    unsigned int checksum = 0;

    // cat timp putem citi URL-uri introduse la input
    while( fgets(URL_cautat, titlu_URL_max, stdin) != NULL)
    {
        // deschidem fisierul master.txt
        f = fopen("master.txt", "rt");

        // initializam variabila "gasit" cu 0
        gasit = 0;

        // citim numele fisierelor HTML din master.txt
        while( fgets(nume_pagini, nume_pagini_max, f) != NULL)
        {
            // stergem caracterul \n adaugat de functia fgets la finalul
            // numelui fisierului
            nume_pagini[strlen(nume_pagini) - 1] = '\0';
            
            // deschidem fisierul site-ului verificat curent
            FILE *g;
            g = fopen(nume_pagini, "rt");

            // daca se poate deschide fisierul
            if(g != NULL)
            {
                // citim metadatele site-ului verificat curent, inclusiv
                // caracterul \n de la finalul liniei
                fscanf(g, "%s%d%d%d\n", 
                            URL_crt,
                            &lung_crt,
                            &nrAcc_crt, 
                            &checksum_crt);

                // initializam variabilele "checksum" si "lung" cu 0
                checksum = 0;
                lung = 0;

                // deoarece functia fgets folosita mai sus pentru a citi vectorul
                // URL_cautat[] a adaugat la finalul acestuia caracterul \n,
                // adaugam acelasi caracter si la finalul vectorului URL_crt[]
                // pentru a le putea compara continutul in mod corect
                strcat(URL_crt, "\n");

                // verificam daca site-ul din fisierul curent corespunde
                // celui cautat
                if(strcmp(URL_crt, URL_cautat) == 0)
                {
                    // in caz afirmativ, actualizam variabila "gasit"
                    gasit = 1;

                    // citim codul HTML al site-ului caracter cu caracter si 
                    // retinem caracterul curent in variabila x
                    while((x = fgetc(g)) != EOF)
                    {
                        // verificam daca am citit atatea caractere cate
                        // s-a specificat ca are codul HTML in metadatele paginii
                        if(lung == lung_crt)
                            // in caz afirmativ, incheiem parcurgerea
                            break;
                        
                        // verificam paritatea indicelui caracterului curent
                        // (reprezentat de valoarea variabilei "lung") si
                        // rotim la stanga/ dreapta caracterul cu un numar de 
                        // pozitii egal cu valoarea indicelui, in cazul in care
                        // acesta este par, respectiv impar

                        // aplicam apoi operatorul XOR intre valoarea curenta a
                        // checksum-ului si rezultatul rotirii, obtinand
                        // noua valoare curenta a checksum-ului
                        if(lung % 2 == 0)
                            checksum = checksum ^ rotl(x, lung);
                        else
                            checksum = checksum ^ rotr(x, lung);
                        
                        // actualizam lungimea in octeti a partii de cod parcurs
                        lung++;
                    }

                    // verificam daca checksum-ul calculat corespunde celui 
                    // specificat in fisierul site-ului verificat curent si 
                    // afisam dupa caz mesajele corespunzatoare
                    if(checksum == checksum_crt)
                        printf("Website safe!\n");
                    else
                        printf("Malicious website! Official key: %d. Found key: %d\n", 
                                checksum_crt, 
                                checksum);
                }

                // inchidem fisierul site-ului pe care l-am verificat
                fclose(g);
            }
        }

        // daca nu am gasit URL-ul site-ului specificat in input, afisam
        // mesajul corespunzator
        if(gasit == 0)
            printf("Website not found!\n");
        
        // inchidem fisierul master.txt
        fclose(f);
    }
}