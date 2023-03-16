					Search Engine

#Task 1

Dupa ce am initializat baza de date, am citit din fisierul master.txt numele
fisierelor HTML ale site-urilor, iar pentru fiecare pagina am prelucrat
informatiile si datele din fisierul corespondent si le-am adaugat la elementul
corespunzator din vectorul de site-uri (baza de date).

Astfel, am retinut pentru inceput metadatele fisierului (URL, lungimea in octeti
a codului HTML, numarul de accesari, checksum-ul), iar apoi am prelucrat codul
HTML pentru a retine titlul, continutul (paragraful), impreuna cu elementele de
CSS ale paragrafului, daca acestea au fost mentionate (culoarea de text si cea
de fundal).

Am parcurs codul HTML linie cu linie, cautand in continutul fiecareia tag-ul de
deschidere de titlu. Deoarece s-a precizat ca titlul se afla pe o singura linie,
in cazul in care am gasit tag-ul de deschidere de titlu l-am cautat in aceeasi
linie si pe cel de inchidere, iar apoi am izolat spatiul dintre cele doua tag-uri
si am copiat sirul de caractere rezultat in vectorul ce retine titlul site-ului.

Pentru ca paragraful se afla mereu dupa titlu, dupa gasirea titlului am inceput
cautarea tag-ului de deschidere de paragraf in fiecare linie citita. Dupa ce
l-am gasit, am verificat incepand cu linia curenta daca gasesc si tag-ul de
inchidere de paragraf. Daca cele doua tag-uri s-au gasit pe aceeasi linie,
am izolat spatiul dintre ele si am obtinut paragraful. Daca cele doua tag-uri
s-au gasit pe linii diferite, am atasat la coninutul paragrafului fiecare
linie intreaga, fara tag de inchidere, impreuna cu portiunea corespunzatoare
din linia ce continea tag-ul de inchidere.

Pentru a interpreta elementele de CSS pentru paragraf, daca au fost mentionate,
am retinut paragraful impreuna cu tag-ul de deschidere. Am cautat cuvantul cheie
"style" in interiorul tag-ului, iar in caz ca l-am gasit, am izolat secventa/ 
secventele de atribuire de culori, iar apoi le-am interpretat si am retinut
in baza de date rezultatele. Am eliminat tag-ul de deschidere de paragraf
din vectorul de continut dupa aceasta prelucrare. 

Dupa construirea bazei de date, am afisat pentru fiecare site elementele
specificate in enunt (URL-ul site-ului, numarul vizualizarilor si titlul).

La final, am eliberat baza de date alocata dinamic.

#Task 2

Pentru inceput, am construit baza de date, dupa metoda explicata la task 1. Apoi
am citit cautarea utilizatorului de la tastatura si am extras, pe rand, fiecare
cuvant. 

La fiecare cuvant gasit, am verificat existenta sa in continutul 
site-urilor din baza de date, iar, in caz ca l-am gasit intr-o pagina, am
marcat aceasta pagina drept potrivita cautarii, folosind vectorul ok[] cu numar
de elemente egal cu numarul de pagini, ce poate lua valori de 0 si 1.

Am efectuat cautarile in vectorul de continut incercand mai multe situatii
posibile: asezarea caracterelor \n sau ' ' inainte, dupa, ori si inainte si
dupa cuvant, sau atasarea unor semne de punctuatie dupa cuvantul cautat. 

Folosind vectorul mai sus mentionat, am sortat paginile potrivite cautarii
alfabetic, iar pentru cele ce au continutul identic descrescator dupa numarul
de accesari. Am realizat acest lucru prin interschimbarea lor in baza de date.

Am afisat apoi rezultatele cautarii (URL-ul fiecarei pagini), sortate, 
folosind vectorul ok[].

La final, am eliberat baza de date alocata dinamic.

#Task 3

Analog task-ului 2, am inceput prin construirea bazei de date si citirea cautarii
utilizatorului de la tastatura. Am extras, pe rand, fiecare element separat de
spatii, iar apoi i-am interpretat forma, luand in calcul si elementele extrase
anterior.

Am folosit trei vectori pentru semnificarea potrivirii unei pagini la cautare.
Unul pentru potrivirea din punct de vedere al aparitiei unor cuvinte, altul
pentru potrivirea din punct de vedere al inexistentei cuvintelor eliminate
la cautare, impreuna cu un altul care sa decida daca un site se potriveste
sau nu tuturor criteriilor de cautare, folosind datele din ceilalti doi.

Astfel, puteam gasi elemente ce au primul caracter '-', insemnand ca avem de
verificat inexistenta cuvantului care urmeaza dupa acest carcater. Am procedat
analog metodei descrise la task 2, doar ca am inversat logica acesteia.

Un alt caz posibil era gasirea inceputului unei secvente, adica un element cu
primul caracter '"'. In acest caz, am retinut faptul ca ne aflam in interiorul
unei secvente si am adaugat cuvantul gasit la un vector ce retine secventa
curenta.

Mai departe, pentru gasirea unui cuvant de sine statator, am verificat daca ne
aflam la momentul curent in interiorul unei secvente. In caz afirmativ, am
atasat cuvantul la secventa curenta, iar in caz contrar am verificat existenta
cuvantului gasit in baza de date.

Ultimul caz posibil era gasirea unui cuvant la final de secventa, adica un
element cu ultimul caracter '"', pe care l-am atasat secventei curente si
am verificat existenta acesteia in baza de date, analog metodei descrise la task 2.

Folosind vectorii mentionati la inceputul explicatiei, am sortat rezultatele
cautarii descrescator dupa numarul de accesari, iar apoi am afisat URL-ul
acestor site-uri ordonate.

La final, am eliberat baza de date alocata dinamic.

#Task 4

Pentru fiecare URL citit de la tastatura, am parcurs fisierele HTML ale site-urilor
din master.txt, iar in caz ca am gasit acelasi URL in metadatele unui fisier ca si
cel cautat, am parcurs codul HTML al acestuia caracter cu caracter si am calculat
checksum-ul paginii, dupa metoda descrisa in enuntul problemei.

Am folosit functia "rotr", ce primeste ca argumente un caracter x si un intreg k si
roteste la dreapta bitii lui x cu k pozitii, impreuna cu functia "rotl", ce primeste
aceleasi argumente ca si functia anterioara si roteste la stanga.

Deoarece printr-o rotire a lui x cu multiplu de 8 biti, obtinem tot x, am simulat
rotirea la dreapta aplicand operatia OR intre rezultatul deplasarii lui x 
la stanga cu diferenta dintre 8 si restul impartirii lui k la 8
si rezultatul deplasarii lui x la dreapta cu restul impartirii lui k la 8. La final,
am aplicat operatia AND intre rezultatul obtinut mai sus si valoarea 255 pentru a 
pastra doar ultimii 8 biti din numarul obtinut. Analog am procedat si pentru
rotirea la stanga.

Pentru fiecare caracter din codul HTML, am verificat paritatea indicelui sau si
am rotit la stanga/ dreapta caracterul cu un numar de pozitii egal cu valoarea
indicelui, in cazul in care acesta este par, respectiv impar. Am aplicat apoi 
operatorul XOR intre valoarea curenta a checksum-ului si rezultatul rotirii,
obtinand noua valoare curenta a checksum-ului.

La final, am comparat pentru fiecare site valoarea checksum-ului specificat in
metadatele fisierului sau si cea obtinuta prin calcul, urmand sa afisez
mesajul corespunzator, conform enuntului.

#Task 5

Se construieste fereastra principala a browser-ului la lansarea programului,
continand titlul acestuia si o legenda de comenzi: 'C' pentru a efectua o 
cautare si 'Q' pentru parasirea browser-ului. La apasarea tastei 'C', apare
pe ecran o caseta de cautare, impreuna cu mesajul "Introduceti maxim 100 de
caractere". Cursorul devine activ, iar utilizatorul poate introduce cautarea.
Legenda comenzilor se modifica: ENTER pentru blocarea casetei.

Dupa blocarea casetei, sunt disponibile tastele 'S' pentru cautare simpla,
'A' pentru cautare avansata, respectiv 'Q' pentru parasirea browser-ului. 

La alegerea cautarii simple, se afiseaza pagina de rezultate corespunzatoare,
dupa rularea algoritmului de la task 2, cu o bara de cautare ce contine textul
cautat in partea de sus si o lista cu titlurile si URL-urile site-urilor ce
corespund cautarii. Navigarea prin aceasta lista se efectueaza folosind tastele
KEY_UP si KEY_DOWN, tasta ENTER fiind rezervata selectiei unui anumit site.
Trecerea selectiei curente de la un site la altul este marcata prin inversarea
culorilor de fundal si de text pentru titlul si URL-ul site-ului respectiv.

La selectarea unui site, se afiseaza in coltul sus-stanga al ecranului titlul
site-ului cu text bold, iar imediat sub paragraful, avand culorile de text si
de fundal pe cele specificate in fisierul HTML, sau negru, respectiv alb in
cazul in care nu au fost specificate.

Cautarea avansata se comporta asemenea celei simple, dar aplica algoritmul de 
la task 3. 

In fiecare dintre paginile de rezultate si de afisare a unui site, apasarea 
tastei 'B' ne duce inapoi la pagina de cautare, respectiv la pagina de rezultate
aferenta, iar tasta 'Q' inchide browser-ul.

#HEADER.C

Fisier ce contine functii comune task-urilor, impreuna cu explicatiile
aferente functionalitatii acestora.

#HEADER.H

Fisier ce contine declararile functiilor din fisierul "header.c", declararile
structurilor folosite pe parcursul temei, dar si definirea unor constante comune
tuturor task-urilor.
