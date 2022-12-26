#include <stdio.h>
#include <stdlib.h>

#define PRZEGRANA -1000
#define WYGRANA 1000
#define PUSTE 12
#define GLEBOKOSC 5
//Gracz    pokazanie funkcjonalnosci 6755 4644 5703 5574 3634
//przyjmuj¹c: 0-król, 1-hetman, 2-wie¿a, 3-goniec, 4-skoczek, gracza 5-pionek

//Komp
//6-król_k, 7-hetman_k, 8-wie¿a_k, 9-goniec_k, 10-skoczek_k, 11-pionek_k, 12-pole puste

// mozna podawac kolejne ruchy po spacji
/*
//***************************************
                DODATKOWE
1. blokada ruszania figurami przeciwnika
2. zablokowane stawianie swoich figur na swoje
3. testowanie "ruszając" polem pustym

//***************************************
*/
int p[8][8];
int MAX_KIER[]={8,8,4,4,8,3,8,8,4,4,8,3,0}; //
int MAX_ODL[]={2,8,8,8,2,2,2,8,8,8,2,2,0};
//wektory przemieszczenia
int WX[12][8]={{0,1,1,1,0,-1,-1,-1},{0,1,1,1,0,-1,-1,-1},{0,1,0,-1},{1,1,-1,-1},{1,2,2,1,-1,-2,-2,-1},{-1,0,1},{0,1,1,1,0,-1,-1,-1},{0,1,1,1,0,-1,-1,-1},{0,1,0,-1},{1,1,-1,-1},{1,2,2,1,-1,-2,-2,-1},{-1,0,1} };
int WY[12][8]={{-1,-1,0,1,1,1,0,-1},{-1,-1,0,1,1,1,0,-1},{-1,0,1,0},{-1,1,1,-1},{-2,-1,1,2,2,1,-1,-2},{-1,-1,-1},{-1,-1,0,1,1,1,0,-1},{-1,-1,0,1,1,1,0,-1},{-1,0,1,0},{-1,1,1,-1},{-2,-1,1,2,2,1,-1,-2},{1,1,1} };

int x,y,k,o; // x,y -> kordy figury ktora bedziemy ruszac, k-kierunek ruchu, o odleglosc,
int w;

void setup(){
    for(int i=0; i<8;i++){
        p[i][1]=11;
        p[i][2]=12;
        p[i][3]=12;
        p[i][4]=12;
        p[i][5]=12;
        p[i][6]=5;
    }
    p[0][0]=8;
    p[7][0]=8;
    p[1][0]=10;
    p[6][0]=10;
    p[2][0]=9;
    p[5][0]=9;
    p[3][0]=7;
    p[4][0]=6;

    p[0][7]=2;
    p[7][7]=2;
    p[1][7]=4;
    p[6][7]=4;
    p[2][7]=3;
    p[5][7]=3;
    p[4][7]=0;
    p[3][7]=1;
}

int ocena(){
/*
    int i, j, w=0;
    for (i=0; i<8; i++)
    for (j=0; j<8; j++){
        if(p[i][j]==11) w++;
        if(p[i][j]==10) w+=3;
        if(p[i][j]==9) w+=4;
        if(p[i][j]==8) w+=5;
        if(p[i][j]==7) w+=9;
        if(p[i][j]==6) w+=WYGRANA;
        if(p[i][j]==5) w--;
        if(p[i][j]==4) w-=3;
        if(p[i][j]==3) w-=4;
        if(p[i][j]==2) w-=5;
        if(p[i][j]==1) w-=9;
        if(p[i][j]==0) w-=WYGRANA;
    }
return w;
*/
    int i, j, w=0;
	int ocen[]={-1000, -9, -5, -4, -3, -1, 1000, 9, 5, 4, 3, 1, 0};
	for(int i=0;i<8;i++){
		for(int j=0;j<8;j++){
            //printf("pole to %d\n", ocen[p[i][j]]);
			w+=ocen[p[i][j]];
			//printf("w: %d\n", w);
		}
		//printf("w z oceny: %d\n", w);
    }
    //printf("222  w z oceny: %d\n", w);
	return w;


}

void wyswietl(){
     /*  // zwykle wyswietlanie
    for(int i=0; i<8; i++){
        for(int j=0; j<8; j++){
            printf(" %d ", p[i][j]);
        }
    printf("\n");
    }

*/
    char figury[]="khwgspKHWGSP ";
	printf("\nKomputer                       Gracz\n");
	printf("| 0    1    2    3    4    5    6    7 | j/i\n");
	for(int i=7;i>=0;i--){
		for(int j=0;j<8;j++)
			printf("| %c |", figury[p[j][i]]);
		printf(" || %d \n========================================\n", i);
	}


    return 0;
}

int najlepszy(int tryb, int *x, int *y, int *k, int *o){ // najlepszy ruch. Plansza, g³êbokoœæ, ktory pion, kierunek, odleglosc
    int px_pom, py_pom, k_pom, o_pom, px, py, dx, dy, kierunek, odleglosc; //robimy zmienne pomocnicze dx, dy o ile sie zmienia xy w ruchu
    int wynik, wmax=2*PRZEGRANA, wmin=2*WYGRANA, ruch_figury, bita_figura;

    wynik=ocena(); // ocena aktualnej sytuacji
    //printf("wynik jest rowny: %d\n", wynik);
    if(tryb==0 || 2*wynik>WYGRANA || 2*wynik<PRZEGRANA) return wynik; // jesli skonczylismy tryb, albo juz wygralismy, albo juz przegralismy
    if(tryb%2==1){ // parzysty ruch, ruch komputera
        for(px=0; px<8; px++){  // px ruch po x
            for(py=0; py<8; py++){ // analizujemy cala plasze
                if(p[px][py]>=6 && p[px][py]<12){ // na polu jest figura komputera -> komputer sie rusza
                    for(kierunek=0; kierunek<MAX_KIER[p[px][py]]; kierunek++){ // sprawdzamy, rozpatrujemy tyle kierunków,
                                            // w ilu dana figura moze sie przemieszczac. Krol w kazda, wieza w 4
                        for(odleglosc=1; odleglosc<MAX_ODL[p[px][py]]; odleglosc++){// o jaka ilosc wektorow mozemy sie
                            //przemiescic w jednym ruchu( krol 1, wieza max 8, bo jeden wektor to o jedno pole)
                            dx=(odleglosc-1)*WX[p[px][py]][kierunek];
                            dy=(odleglosc-1)*WY[p[px][py]][kierunek];
                            if(odleglosc>=2 && p[px+dx][py+dy]!= PUSTE){ // jesli po drodze nie jest pusto (cos stoi)
                                break; // sprawdzamy tylko do momentu, w ktorym nie natrafimy na figure
                            }
                            dx=odleglosc*WX[p[px][py]][kierunek]; // okreslenie koncowej pozycji
                            dy=odleglosc*WY[p[px][py]][kierunek];

                            if(px+dx>=0 && px+dx<8 && py+dy>=0 && py+dy<8){ // ruch jest w obrebie planszy
                                if(p[px+dx][py+dy]==PUSTE || p[px+dx][py+dy]<=5){ // jesli docelowe pole jest puste / stoi figura przeciwnika (gracza)
                                    if(p[px][py]!=11 || (p[px+dx][py+dy]==PUSTE && dx==0) || (p[px+dx][py+dy]!=PUSTE && dx!=0)){ // jeœli ruszamy sie pionkiem (ponek_k to 11) docelowe jest 0, ale przesuniecie=0
                                        ruch_figury=p[px][py]; // tam gdzie stalismy
                                        bita_figura=p[px+dx][py+dy]; // tam gdzie sie ruszylismy
                                        p[px+dx][py+dy]=p[px][py];   // dokonujemy ruchu figura z [px][py] na [px+dx][py+dy]
                                        p[px][py]=PUSTE; // ruszylismy sie, wiec tam gdzie stalismy jest juz puste

                                        if(p[px+dx][py+dy]==11 && py+dy==7){ // jesli pionek doszedl do konca mozliwa jest zamiana na hetmana
                                            p[px+dx][py+dy]=7;    // wymieniamy pionka na hetmana
                                        }
                                        //printf("wynik to %d", wynik);
                                        wynik=najlepszy(tryb-1, &px_pom, &py_pom, &k_pom, &o_pom); // tu mamy ta rekurencje
                                        p[px][py]=ruch_figury;       // cofamy nasza figure do stanu przed analiza
                                        p[px+dx][py+dy]=bita_figura; // wskrzeszamy ewentualnie zbita figure podczas ruchu
                                        if(wynik>=wmax){    // jesli aktualnie analizowany jest lepszy niz poprzedni, to wstawia aktualny jako najlepszy
                                            wmax=wynik;
                                            *x=px;          // zapisujemy kordy figury o najlepszym ruchu
                                            *y=py;          // tak aby wiedziec ktorym chcielismy sie ruszych
                                            *k=kierunek;    // w ktora storne
                                            *o=odleglosc;   // i o ile
                                        }
                                        //printf("wynik ruchu %d\n", wmax);
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        return wmax;
    }
    else{ // ruch gracza
         for(px=0; px<8; px++){  // px ruch po x
                // dajemy na start najlepszy ruch, bo wg nas gracz wybierze dla nas najgorszy ruch
            for(py=0; py<8; py++){ // analizujemy cala plasze
                if(p[px][py]<=5){ // na polu jest figura gracza
                    for(kierunek=0; kierunek<MAX_KIER[p[px][py]]; kierunek++){
                        for(odleglosc=1; odleglosc<MAX_ODL[p[px][py]]; odleglosc++){
                            dx=(odleglosc-1)*WX[p[px][py]][kierunek];
                            dy=(odleglosc-1)*WY[p[px][py]][kierunek];
                            if(odleglosc>=2 && p[dx+px][dy+py]!= PUSTE){
                                break;
                            }
                            dx=odleglosc*WX[p[px][py]][kierunek];
                            dy=odleglosc*WY[p[px][py]][kierunek];

                            if(px+dx>=0 && px+dx<8 && py+dy>=0 && py+dy<8){
                                if(p[px+dx][py+dy]>6){ // jesli docelowe pole jest puste / stoi figura komputera
                                    if(p[px][py]!=5 || (p[px+dx][py+dy]==PUSTE && dx==0) || (p[px+dx][py+dy]!=PUSTE && dx!=0)){ //(ponek gracza to 5)
                                        ruch_figury=p[px][py];
                                        bita_figura=p[px+dx][py+dy];
                                        p[px+dx][py+dy]=p[px][py];   // dokonujemy ruchu figura z [px][py] na [px+dx][py+dy]
                                        p[px][py]=PUSTE; // ruszylismy sie, wiec tam gdzie stalismy jest juz puste

                                        if(p[px+dx][py+dy]==5 && py+dy==0){ // jesli pionek doszedl do konca mozliwa jest zamiana na hetmana
                                            // pionek to 5, a "koniec to 0"
                                            p[px+dx][py+dy]=1;    // wymieniamy pionka na hetmana
                                        }
                                        wynik=najlepszy(tryb-1, &px_pom, &py_pom, &k_pom, &o_pom); // tu mamy ta rekurencje
                                        p[px][py]=ruch_figury;       // cofamy nasza figure do stanu przed analiza
                                        p[px+dx][py+dy]=bita_figura; // wskrzeszamy ewentualnie zbita figure podczas ruchu
                                        if(wynik<=wmin){    // jesli aktualnie analizowany jest lepszy niz poprzedni, to wstawia aktualny jako najlepszy
                                            wmin=wynik;
                                            *x=px;          // zapisujemy kordy figury o najlepszym ruchu
                                            *y=py;          // tak aby wiedziec ktorym chcielismy sie ruszych
                                            *k=kierunek;    // w ktora storne
                                            *o=odleglosc;   // i o ile
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        return wmin;
    }
}

int main()
{
    int x, y, wyn, k, o, dx, dy, x2, y2; // juz faktyczne ruchy kompa oraz kordy ruchu gracza
    int ruch=0; // zmienna czy ruszyl swoja figura
    setup();
    while(1){

        najlepszy(GLEBOKOSC,&x,&y,&k,&o); // pole, ile ruchow, komorki kordow figury ktorą ruszamy, kierunku, odleglosci
        dx=o*WX[p[x][y]][k]; // obliczamy o ile zmieni sie x z aktualnego do docelowego
        dy=o*WY[p[x][y]][k]; // obliczamy ... y
        p[x+dx][y+dy]=p[x][y]; // dokonujemy ruchu. przemieszczamy sie o zapamietane wartosci z "najlepszego" ruchu
        p[x][y] = PUSTE; // w pole gdzie stala figura wstawiamy puste pole

        if(p[x+dx][y+dy]==11 && y+dy==7){ // jesli ruszylismy sie pionkiem z xy na x+dx, y+dy, i jest to ostatnie "pole" (do konca)
            p[x+dx][y+dy]=7; // to zamieniamy piona na hetmana
        }
        wyswietl(p); // wyswietlamy plansze po ruchu komputera

        //printf("%d", ocena());
        if(ocena()>=WYGRANA) { // sprawdza czy komp wygral
            printf("Komputer wygral!");
            return 0;
        }
		else if(ocena()<=PRZEGRANA){ // sprawdza czy komp przegral
		    printf("Wygrales!");
            return 0;
        }

        while (!ruch){
            printf("podaj ruch: i/j\n");
            // dlatego rozbite, zeby odrazu pokazywalo czy rusza swoja figura
            scanf("%d%d%d%d", &x,&y,&x2,&y2);
            if(p[x][y]==PUSTE) break; // wychdzi
            if(p[x][y]>5){ // czy figura ktora chce sie ruszyc jest jego // to puste dla testow, abym mogl testowac pustym polem
                // ta kolejnosc, bo jesli p[x][y] ==12, to nie sprawdza kolejnego
                printf("rusz sie swoja figura gosciu\n");
                continue;
            }
            if (p[x2][y2]<=5){ // chce wejsc swoja figura na swoja
                printf("nie trolluj\n");
                continue;
            }
            ruch=1;
        }
        ruch=0;
        if(p[x][y]==5 && y2==0) p[x2][y2]=1; // promocja do hetmana pionka gracza
        else p[x2][y2]=p[x][y];
        p[x][y] = PUSTE;

        printf("%d,%d,%d,%d; \n", x,y,x2,y2);
        if(ocena()>=WYGRANA/2) { // sprawdza czy komp wygral
            printf("Komputer wygral!");
            return 0;
        }
		else if(ocena()<=PRZEGRANA/2){ // sprawdza czy komp przegral
		    printf("Wygrales!");
            return 0;
        }
    }
    return 0;
}
