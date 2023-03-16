#include <stdio.h>
#include <string.h>
#include <ncurses.h>
#include "header.h"

// functie pentru afisarea in pagina de rezultate a casetei cu textul cautat
void print_search_bar(char cuvinte[])
{
    // stergem ecranul
    clear();
    refresh();

    // afisam bara de cautare
    attron(COLOR_PAIR(1));
    mvprintw(1, 1, "Search for: ");
    refresh();
    attroff(COLOR_PAIR(1));
    refresh();
    WINDOW *search_bar = newwin(4, 52, 2, 1);
    box(search_bar, 0, 0);
    wrefresh(search_bar);

    mvwprintw(search_bar, 1, 1, cuvinte);
    wrefresh(search_bar);
}

// functie pentru afisarea rezultatelor cautarii
void print_results(char URL_site[][titlu_URL_max], 
                   char titlu_site[][titlu_URL_max], 
                   int ok[], 
                   int *lin_crt, 
                   int *go_back_c)
{
    // initiem fereastra de rezultate
    WINDOW *rez_search = newwin(40, 80, 8, 1);

    attron(COLOR_PAIR(1));
    mvprintw(6, 1, "Results: ");
    refresh();
    attroff(COLOR_PAIR(1));
    refresh();

    box(rez_search, 0, 0);
    wrefresh(rez_search);
    keypad(rez_search, TRUE);

    // afisam legenda comenzilor
    wattron(rez_search, COLOR_PAIR(1));
    mvwprintw(rez_search, 37, 1, "Comenzi:");
    wrefresh(rez_search);
    wattroff(rez_search, COLOR_PAIR(1));
    wrefresh(rez_search);

    wmove(rez_search, 38, 1);
    wprintw(rez_search, 
    " KEY_UP - sus, KEY_DOWN - jos, Enter - selectare site, B - back, Q - iesire");
    wrefresh(rez_search);
    wmove(rez_search, 1, 1);
    wrefresh(rez_search);

    int i, ct_rez = 0;
    int ch, ch_neutru = '.';

    // afisam rezultatele
    for(i = 0; i < ct_pag; i++)
        if(ok[i] == 1)
        {
            strcpy(URL_site[ct_rez], siteuri.v[i].URL);
            strcpy(titlu_site[ct_rez], siteuri.v[i].titlu);

            ct_rez++;
        }
    
    // lin_crt retine indicele liniei la care incepe afisarea site-ului
    // selectat la momentul curent, din fereastra de rezultate
    (*lin_crt) = 0;

    noecho();
    
    ch = ch_neutru;

    // simulam deplasarea prin rezultatele cautarii folosind
    // tastele KEY_UP si KEY_DOWN, pana la selectarea unui
    // site (ENTER), revenirea la pagina de cautare (B), sau
    // parasirea browser-ului (Q)
    while( ch != '\n' && ch != 'b' && ch != 'B' && ch != 'q' && ch != 'Q' )
    {
        // afisam rezultatele si evidentiem site-ul
        // selectat curent, folosind valoarea variabilei lin_crt
        wmove(rez_search, 0, 0);
        for(i = 0; i < ct_rez; i++)
        {
            // pentru fiecare site, afisam o linie cu titlul sau,
            // una cu URL-ul si o linie de delimitare

            // evidentiem folosind atributul A_REVERSE
            if((*lin_crt) / 3 == i)
                wattron(rez_search, A_REVERSE);

            wmove(rez_search, i * 3, 2);
            wprintw(rez_search, "%s", titlu_site[i]);
            wrefresh(rez_search);

            wmove(rez_search, i * 3 + 1, 2);
            wprintw(rez_search, "%s", URL_site[i]);
            wrefresh(rez_search);

            wattroff(rez_search, A_REVERSE);
            wrefresh(rez_search);
        }

        ch = wgetch(rez_search);

        // simulam deplasarea in lista, in limitele numarului de elemente
        if(ch == KEY_UP && (*lin_crt) > 0)
            (*lin_crt) = (*lin_crt) - 3; 

        if(ch == KEY_DOWN && (*lin_crt) < ((ct_rez * 3) - 3))
            (*lin_crt) = (*lin_crt) + 3;   

        // daca se doreste revenirea la cautare
        if(ch == 'b' || ch == 'B')
        {
            (*go_back_c) = 1;
            break;
        }
        // daca se doreste parasirea browser-ului
        if(ch == 'q' || ch == 'Q')
        {
            endwin();
            exit(0);
        }
    }
}

// functie pentru afisarea unui site pe ecran
void print_site(char URL_site[][titlu_URL_max], 
                char titlu_site[][titlu_URL_max], 
                int lin_crt)
{
    clear();
    noecho();
    
    int i;

    // parcurgem baza de date si cautam site-ul curent selectat,
    // stiind URL-ul acestuia
    for(i = 0; i < ct_pag; i++)
        if(strcmp(siteuri.v[i].URL, URL_site[lin_crt / 3]) == 0)
        {
            // afisam titlul cu bold
            attron(A_BOLD);
            refresh();
            printw("%s\n", siteuri.v[i].titlu);
            refresh();
            attroff(A_BOLD);
            refresh();
            
            // variabila "culoareB" retine indicele culorii de background a
            // paragrafului, iar variabila "culoareF" pe cel al culorii
            // de foreground
            int culoareB, culoareF;
            
            // determinam "culoareB" si "culoareF" folisind baza de date
            switch(siteuri.v[i].back_color)
            {
                case 0:
                    culoareB = COLOR_WHITE;
                    break;
                
                case 1:
                    culoareB = COLOR_BLACK;
                    break;
                
                case 2:
                    culoareB = COLOR_RED;
                    break;
                
                case 3:
                    culoareB = COLOR_GREEN;
                    break;
                
                case 4:
                    culoareB = COLOR_BLUE;
                    break;
                
                case 5:
                    culoareB = COLOR_YELLOW;
                    break;
            }

            switch(siteuri.v[i].color)
            {
                case 0:
                    culoareF = COLOR_WHITE;
                    break;
                
                case 1:
                    culoareF = COLOR_BLACK;
                    break;
                
                case 2:
                    culoareF = COLOR_RED;
                    break;
                
                case 3:
                    culoareF = COLOR_GREEN;
                    break;
                
                case 4:
                    culoareF = COLOR_BLUE;
                    break;
                
                case 5:
                    culoareF = COLOR_YELLOW;
                    break;
            }
            // formam o pereche din cele doua culori
            init_pair(2, culoareF, culoareB);

            // afisam paragraful
            attron(COLOR_PAIR(2));
            printw("%s", siteuri.v[i].cont.c);
            attroff(COLOR_PAIR(2));

            refresh();

            // legenda vizualizare pagina
            move(38, 1);
            attron(COLOR_PAIR(1));
            printw("Comenzi: ");
            attroff(COLOR_PAIR(1));
            printw(" B - back, Q - iesire");
            refresh();
        }
}

// functie pentru cautarea simpla, dupa cum este detaliata la task2
void cautare_simpla (int ok[], char cuvinte[])
{
    char *p;
    int i;

    for(i = 0; i < ct_pag; i++)
        ok[i] = 0;
    
    p = strtok(cuvinte, " \n");
    while( p != NULL)
    {
        verif_exist(siteuri, ct_pag, p, ok);
        p = strtok( NULL, " \n"); 
    }

    sortare(&siteuri, ct_pag, ok, compara_2);
}

// functie pentru cautarea avansata, dupa cum este detaliata la task3
void cautare_avansata (int ok[], char cuvinte[])
{
    int i, ok_1[ct_pag], ok_2[ct_pag], gasit_secventa = 0;
    char caut[100], *p;

    for(i = 0; i < ct_pag; i++)
    {
        ok[i] = 0;
        ok_1[i] = 0;
        ok_2[i] = 1;
    }

    p = strtok( cuvinte, " \n");

    while(p != NULL)
    {
        if(*p == '-')
        {
            strcpy(caut, p + 1);
            verif_nu_exist(siteuri, ct_pag, caut, ok_2);
            strcpy(caut, "");
        }
        else if (*p != '"' && strcmp((p + strlen(p) - 1), "\"") != 0)
        {
            if(gasit_secventa == 1)
            {
                strcat(caut, " ");
                strcat(caut, p);
            }   
            else
            {
                verif_exist(siteuri, ct_pag, p, ok_1);
            }    
        }
        else if (*p == '"')
        {
            strcat(caut, p + 1);
            gasit_secventa = 1;
        }
        else if (strcmp((p + strlen(p) - 1), "\"") == 0)
        {
            strcat(caut, " ");
            strcat(caut, p);
            caut[strlen(caut) - 1] = '\0';

            verif_exist(siteuri, ct_pag, caut, ok_1);
            strcpy(caut, "");
            gasit_secventa = 0;
        }

        p = strtok(NULL, " \n");
    }

    for(i = 0; i < ct_pag; i++)
        if(ok_1[i] == 1 && ok_2[i] == 1)
            ok[i] = 1;
    
    sortare(&siteuri, ct_pag, ok, compara_3);
}

int main()
{
    // creare baza de date
    FILE *f;
    f = fopen("master.txt", "rt");

    init_vector(&siteuri, 3);
    citeste_pag(&siteuri, &ct_pag, f);

    fclose(f);

    int i;

    for(i = 0; i < ct_pag; i++)
        strcat(siteuri.v[i].cont.c, "\n");

    // initializare interfata grafica
    initscr();
    cbreak();
    start_color();

    // creare fereastra de pornire
    WINDOW *main_win = newwin(40, 80, 1, 1);
    refresh();
    box(main_win, 0, 0);
    wrefresh(main_win);

    // afisare nume browser cu bold
    wattron(main_win, A_BOLD);
    mvwprintw(main_win, 15, 30, "Browser-ul lui Biju");
    wattroff(main_win, A_BOLD);
    wrefresh(main_win);

    // afisare legenda comenzi
    init_pair(1, COLOR_WHITE, COLOR_GREEN);
    attron(COLOR_PAIR(1));
    mvprintw(39, 2, "Comenzi:");
    attroff(COLOR_PAIR(1));
    printw(" C - deschidere caseta de cautare, Q - iesire");
    refresh();
    
    // tastele apasate de catre utilizator nu vor aparea pe ecran, iar
    // cursorul va fi invizibil in continuare
    noecho();
    curs_set(0);

    // variabila "go_back_c" ia valoarea 1 daca utilizatorul doreste
    // sa se intoarca la cautare, sau 0 in caz contrar

    // variabila "go_back_s" ia valoarea 1 daca utilizatorul doreste
    // sa se intoarca la pagina de rezultate a unei cautari simple,
    // sau 0 in caz contrar

    // variabila "go_back_a" are scop analog cu "go_back_s", dar pentru
    // cautarea avansata 
    int go_back_c = 0, go_back_s = 0, go_back_a = 0;

    int ch, ch_neutru = '.';
    int ok[ct_pag];
    int ct_rez = 0;
    int lin_crt = 0;

    char URL_site[ct_pag][titlu_URL_max];
    char titlu_site[ct_pag][titlu_URL_max];
    
    // initializam variabila "ch" cu un caracter ce nu reprezinta
    // o comanda in browser-ul nostru
    ch = ch_neutru;

    // asteptam ca utilizatorul sa introduca tasta 'c' pentru cautare
    // sau tasta 'q' pentru parasirea browser-ului
    while( ch != 'c' && ch != 'C' && ch != 'q' && ch != 'Q' )
    {
        // verificam daca accesam pentru prima data pagina de cautare
        if(go_back_c == 0)
            // in caz afirmativ, asteptam o comanda
            ch = wgetch(main_win);
        
        // utilizatorul a decis sa paraseasca browser-ul
        if(ch == 'q' || ch == 'Q')
        {
            endwin();
            return 0;
        }

        // utilizatorul vrea sa efectueze prima cautare sau revine
        // pentru o alta
        if(ch == 'c' || ch == 'C' || go_back_c == 1)
        {
            // daca revenim dintr-o alta cautare, refacem ecranul principal
            if(go_back_c == 1)
            {
                clear();
                refresh();

                WINDOW *main_win = newwin(40, 80, 1, 1);
                refresh();
                box(main_win, 0, 0);
                wrefresh(main_win);

                wattron(main_win, A_BOLD);
                mvwprintw(main_win, 15, 30, "Browser-ul lui Biju");
                wattroff(main_win, A_BOLD);
                wrefresh(main_win);
                
                // actualizam variabila "go_back_c" la valoarea 0
                go_back_c = 0;
            }

            // modificam legenda comenzilor
            wmove(main_win, 38, 1);
            wclrtoeol(main_win);
            wmove(main_win, 38, 1);
            wrefresh(main_win);

            box(main_win, 0, 0);
            wrefresh(main_win);

            wattron(main_win, COLOR_PAIR(1));
            mvwprintw(main_win, 38, 1, "Comenzi:");
            wattroff(main_win, COLOR_PAIR(1));
            wrefresh(main_win);
            wprintw(main_win, " ENTER - blocheaza caseta de cautare");
            wrefresh(main_win);

            // initializam fereastra de cautare
            WINDOW *search = newwin(5, 52, 20, 15);
            box(search, 0, 0);
            wrefresh(search);
            mvprintw(27, 24, "Introduceti maxim 100 de caractere.");
            refresh();

            // afisam caracterele introduse la tastatura si cursorul
            echo();
            curs_set(1);
            
            // mutam cursorul la inceputul casetei de cautare
            wmove(search, 1, 1);
            wrefresh(search);

            char cuvinte[300], cuvinte_aux[300];
            strcpy(cuvinte, "");
            strcpy(cuvinte_aux, "");

            // retinem cautarea utilizatorului la blocarea ferestrei
            wgetstr(search, cuvinte);

            // facem o copie a sirului cautat, pentru a-l afisa ulterior
            // in forma sa neprelucrata
            strcpy(cuvinte_aux, cuvinte);

            noecho();
            curs_set(0);

            // modificam legenda
            wattron(main_win, COLOR_PAIR(1));
            mvwprintw(main_win, 38, 1, "Comenzi:");
            wattroff(main_win, COLOR_PAIR(1));
            wrefresh(main_win);
            wprintw(main_win, 
                    " S - cautare normala, A - cautare avansata, Q - iesire");
            wrefresh(main_win);

            ch = ch_neutru;

            // asteptam specificarea tipului de cautare sau parasirea browser-ului
            while(ch != 's' && ch != 'S' && 
                  ch != 'a' && ch != 'A' && 
                  ch != 'q' && ch != 'Q')
            {
                // verificam daca accesam pentru prima data pagina de rezultate
                if(go_back_s == 0 && go_back_a == 0)
                    // in caz afirmativ, asteptam o comanda
                    ch = getch();
                
                // utilizatorul a decis sa paraseasca browser-ul
                if(ch == 'q' || ch == 'Q')
                {
                    endwin();
                    return 0;
                }

                // utilizatorul a ales cautarea simpla sau revine la pagina 
                // de rezultate a unei cautari simple anterioare
                if(ch == 's' || ch == 'S' || go_back_s == 1) 
                {
                    go_back_s = 0;

                    // afisam pagina de rezultate
                    print_search_bar(cuvinte_aux);
                    strcpy(cuvinte, cuvinte_aux);
                    cautare_simpla(ok, cuvinte);
                    print_results(URL_site, 
                                  titlu_site, 
                                  ok, 
                                  &lin_crt,
                                  &go_back_c);

                    // verificam daca s-a apasat tasta B (back)
                    if(go_back_c == 1)
                        // in caz afirmativ, revenim la cautare
                        break;
                    
                    // afisam site-ul selectat
                    print_site(URL_site, titlu_site, lin_crt);

                    // ne intoarcem la pagina de rezultate sau parasim browser-ul
                    ch = ch_neutru;
                    while(ch != 'b' && ch != 'B' && ch != 'q' && ch != 'Q')
                    {
                        ch = getch();
                        if(ch == 'q' || ch == 'Q')
                        {
                            endwin();
                            return 0;
                        }
                        if(ch == 'b' || ch == 'B')
                        {
                            go_back_s = 1;
                            break;
                        }
                    }  
                }

                // utilizatorul a ales cautarea avansata sau revine la pagina
                // de rezultate a unei cautari simple anterioare
                if(ch == 'a' || ch == 'A' || go_back_a == 1)
                {
                    go_back_a = 0;

                    // afisam pagina de rezultate
                    print_search_bar(cuvinte_aux);
                    strcpy(cuvinte, cuvinte_aux);
                    cautare_avansata(ok, cuvinte);
                    print_results(URL_site,
                                  titlu_site,
                                  ok,
                                  &lin_crt,
                                  &go_back_c);

                    // verificam daca s-a apasat tasta B (back)
                    if(go_back_c == 1)
                        break;

                    // afisam site-ul selectat
                    print_site(URL_site, titlu_site, lin_crt);

                    // ne intoarcem la pagina de rezultate sau parasim browser-ul
                    ch = ch_neutru;
                    while(ch != 'b' && ch != 'B' && ch != 'q' && ch != 'Q')
                    {
                        ch = getch();
                        if(ch == 'q' || ch == 'Q')
                        {
                            endwin();
                            return 0;
                        }
                        if(ch == 'b' || ch == 'B')
                        {
                            go_back_a = 1;
                            break;
                        }
                    }
                }     
            }
        }
    }
    getch();
    endwin();
    curs_set(1);

    // eliberam baza de date alocata dinamic
    free_vector(&siteuri, ct_pag);

    return 0;
}