/*barbiere:
d2 d3
d7 d5
e1 c3
g8 h6
c1 f4
h6 g8
c3 c7
**/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "scacchi_help.c"
#include "scacchi_stallo_uscita.c"
#include "scacchi_presentazione.c"
/**
Debuggo va definito a 1 se si vuole visualizzare il debug.
Attenione: l'output può essere molto e rallentare alcuni passaggi
**/
#define debuggo 0

/*Dichiaro i vari tipi di strutture necessari*/

/*Pedina: struttura contenente le informazioni della pedina*/
typedef struct
{
	int player;/*Giocatore di appartenenza*/
	int moved;/*La pedina è stata mossa o no? (Serve per l'arrocco e la mossa di 2 del pedone*/
	int l;/*Coordinata 1 della posizione*/
	int n;/*Coordinata 2 della posizione*/
	char tipo;/*Che pedina è?*/
} pedina;

/*Posizione: informazioni sulla posizione della pedina*/
typedef struct
{
	int l;/*Coordinata 1 della posizione*/
	int n;/*Coordinata 2 della posizione*/
	pedina *loc;/*Puntatore alla casella*/
} posizione;

/*Salvataggio: struttura usata per salvare tutti i dati in un file binario*/
typedef struct
{
	int player;//Giocatore di turno
	int numero_celle;//numero di mosse eseguite
	char name1[31];//Nome del giocatore 1
	char name2[31];//Nome del giocatore 2
	pedina pedine[33];//Array contenente strutture delle pedine
	int board[8][8];//Array rappresentante la scacchiera
} salvataggio;

/*Mossa: informazioni sulla mossa appena eseguita*/
struct mossa{
	int player;/*Nome del giocatore che ha eseguito la mossa*/
	char coordinate[15];/*Coordinate di partenza e arrivo*/
    double durata_mossa;/*Tempo di durata della mossa in secondi*/
    struct mossa*next;/*Puntatore a struttura sucessiva*/
    struct mossa*prev;/*Puntatore a struttura precedente*/};

/*Coda: struttura dove verranno salvate le 10 mosse più veloci*/
struct Coda
{
	struct mossa *primo;/*testa*/
	struct mossa *ultimo;/*coda*/
};


/*Dichiarazioni funzioni*/
int clear();
void presentazione();
void debug(char deb[51]);
void daCapo(pedina *pedine, pedina *board[8][8], char *name1, char *name2, char *name, int *numero_celle);
void inizio(pedina *pedine, pedina *board[8][8]);
char userInput();
void showBoard(pedina *board[8][8]);
void getNames(char *name1, char *name2);
int getCoordinate(pedina *board[8][8], posizione *pos1, posizione *pos2, char *name, int player);
int checkMove(pedina *pedine, pedina *board[8][8], posizione *pos1, posizione *pos2, int player, int checkScacco);
int checkPedone(int l1, int n1, int l2, int n2, posizione *pos1, posizione *pos2, pedina *board[8][8], int player, int checkScacco);
char promozione();
int checkTorre(int l1, int n1, int l2, int n2, pedina *board[8][8]);
int checkCavallo(int l1, int n1, int l2, int n2);
int checkAlfiere(int l1, int n1, int l2, int n2, pedina *board[8][8]);
int checkRe(int l1, int n1, int l2, int n2, pedina *board[8][8]);
int scacco(int l, int n, int giocatore, pedina *board[8][8], pedina *pedine, int player);
int checkArrocco(int l1, int n1, int l2, int n2, posizione *pos1, posizione *pos2, pedina *board[8][8], pedina *pedine, int player);
int roundRe(int l, int n, int giocatore, pedina *board[8][8], pedina *pedine, int player);
void boardCopy(pedina *board[8][8], pedina *testBoard[8][8]);
void pedCopy(pedina *pedine, pedina *testPedine);
int checkMatto(int lr, int nr, pedina *board[8][8], pedina *pedine, int player);
void move(int l1, int n1, int l2, int n2, pedina *board[8][8], pedina *pedine);
void nexTurn(int *player);
struct mossa* newtime(int player, struct mossa *testa, struct mossa *nuova, posizione *pos1, posizione *pos2,time_t prima_della_mossa, int numero_celle, char *name1, char *name2);
int dichiaroMatto(struct mossa *testa,struct mossa *puntatore_ad_array_struct,int numero_celle, char *name1, char *name2);
void coda(struct mossa *mosse_da_ordinare,int numero_celle, char *name1, char *name2);
void help();
void uscita();
void stallo();
char vittoria(int player,char *name1,char *name2,struct mossa *testa,struct mossa *puntatore_ad_array_struct,int numero_celle);
int conteggio_celle(int numero_celle);
#ifndef _WIN32
#include <termios.h>
#include <unistd.h>
#define MENU "Q: quit ## N: new game ## H: help ## M: muovi\nS: dichiara stallo ## X: salva partita ## C: carica partita"
int getch(void);
#include "for_linux.c"
#else
#define MENU "Q: quit ## N: new game ## H: help ## M: muovi\nS: dichiara stallo"
#endif


/*MAIN*/
int main()
{
	struct mossa *testa; /*Puntatore alla testa della lista*/
	struct mossa *nuova; /*Puntatore alla nuova cella della lista*/
	struct mossa *nuova_da_ordinare; /*Puntatore alla nuova mossa prima che venga ordinata */
	struct mossa *testa_da_ordinare; /*Puntatore alla testa della coda da ordinata*/
	struct mossa *temp; /*Puntatore a struttura temporanea*/
	struct mossa *puntatore_ad_array_struct; /*Puntatore ad array di strutture*/
	testa=NULL;/*Inizializzo la testa a NULL*/
	testa_da_ordinare=NULL;/*Inizializzo la testa da ordinare a NULL*/
	char name1[31], name2[31], *name, rigioca, deb[51];
	char r4[201]=MENU;
	int i, controlli, sottoScacco=0, partita=1, player=0, numero_celle, contatore3=0;
	time_t prima_della_mossa=0;
	pedina pedine[33];
	pedina *board[8][8], *testBoard[8][8], *toTest;
	posizione pos1, pos2;
	presentazione();
	name=name1;
	daCapo(pedine, board, name1, name2, name, &numero_celle);
	/*----------*/
	do {
		i=0, controlli=0;
		while (controlli==0)
		{
			printf("\n%s\n", r4);
			switch (userInput())
{
				case 'n':
					clear();
					daCapo(pedine, board, name1, name2, name, &numero_celle);
					showBoard(board);
					player=0;
					continue;
				case 'q':
					uscita();
					showBoard(board);
					continue;
				case 'h':
					help();
					showBoard(board);
					continue;
                #ifndef _WIN32
				case 'x':
					save(pedine, board, name1, name2, player, numero_celle, testa);
					continue;
				case 'c':
					testa = load(pedine, board, name1, name2, name, &player, &numero_celle);
					debug("inizio test testa");
					temp=testa;
					while(temp != NULL)
					{
						sprintf(deb, "giocatore: %s\n", (temp->player==0)?name1:name2); debug(deb);
						sprintf(deb, "mossa: %s\n", temp->coordinate); debug(deb);
						sprintf(deb, "durata: %.2lf secs\n", temp->durata_mossa); debug(deb);
						temp=temp->next;
						contatore3++;
					}
					debug("fine test testa");
					continue;
                #endif
				case 's':
					stallo();
					showBoard(board);
					continue;
			}
			showBoard(board);
			if(prima_della_mossa==0)
			{
				time(&prima_della_mossa);
			}
			if (getCoordinate(board, &pos1, &pos2, name, player) &&
				pos1.loc->player==player &&
				checkMove(pedine, board, &pos1, &pos2, player, 0))
			{
				numero_celle=conteggio_celle(numero_celle);
				testa=newtime(player, testa, nuova, &pos1, &pos2, prima_della_mossa, numero_celle, name1, name2);
				prima_della_mossa=0;
				controlli = 1;
			}
			else { printf("\nCoordinate errate, reinseriscile\n"); }
		}
		toTest = (player==0)? pedine+27 : pedine+3;	//Re avversario
		if (scacco(toTest->l, toTest->n, player, board, pedine, player)) {sottoScacco=1; printf("\nScacco al RE\n"); }
		if (sottoScacco)
		{
			debug("SOTTO SCACCO");
			if(roundRe(toTest->l, toTest->n, player, board, pedine, player))
			{
				debug("forse matto??\n");
				if(checkMatto(toTest->l, toTest->n, board, pedine, player))
				{
				    clear();
					printf("\nScacco matto!!!!!\n");
					rigioca=vittoria(player,name1,name2,testa,puntatore_ad_array_struct,numero_celle);
					if(rigioca=='y')
					{
						clear();
						daCapo(pedine, board, name1, name2, name, &numero_celle);
						showBoard(board);
						player=0;
						continue;
					}
					else
					{
						printf("\nGrazie per aver giocato! Buona giornata.");
						getch();
						exit(0);
					}
				}
				else {debug("non matto..");}
			}
		}
		sprintf(deb, "%c --> moved=%d", pos1.loc->tipo, pos1.loc->moved); debug(deb);
		sprintf(deb, "%c (%d,%d) in %c (%d,%d)", pos1.loc->tipo, pos1.l+1, pos1.n+1, (*board[pos2.n][pos2.l]).tipo, pos2.l+1, pos2.n+1); debug(deb);
		sottoScacco=0;
		clear();
		nexTurn(&player);
		name=(player==0)?name1:name2;
	} while(partita != 0);
	return 0;
}

//-----------------
//DEFINISCO LE FUNZIONI
//-----------------

/********************************
clear()
Richiama la funzione adatta per pulire la console


Parametri in ingresso: nessuno

Valore restituito: nessuno
********************************/
int clear()
{
	if(!debuggo)
	{
		#ifdef _WIN32
		system("CLS");
		#else
		system("clear");
		#endif
	}
}


/******************************
daCapo()
Avvia la partita, e viene richiamata
nel caso si voglia iniziare una nuova partita.
Contiene a sua volta 2 funzioni: inizio() e getNames()

Parametri in ingresso:
    pedine: Le strutture contenenti i dati di tutte le pedine
    board: La scacchiera con la posizione di tutte le pedine
    name1: Nome del primo giocatore
    name2: Nome del secondo giocatore

Valore restituito: nessuno
********************************/
void daCapo(pedina *pedine, pedina *board[8][8], char *name1, char *name2, char *name, int *numero_celle)
{
	*numero_celle=0;
	inizio(pedine, board);
	getNames(name1, name2);
	name=name1;
	return;
}

/*******************************
inizio()
Crea le strutture delle pedine (32) più la struttura della casella vuota:
contenute nell'array pedine[32]. Si occupa anche di collegare ogni casella con la struttura alla quale è correlata.
Le 64 caselle risiedono nell'array bidimensionale board[8][8].
In questa maniera i 2 array sono collegati in modo da rappresentare la situazione iniziale di gioco.
Si accede a tutto, quindi, tramite board, con una semplice chiamata in coordinate.

Parametri in ingresso:
    pedine: Le strutture contenenti i dati di tutte le pedine
    board: La scacchiera con la posizione di tutte le pedine

Valore restituito: nessuno

********************************/
void inizio(pedina *pedine, pedina *board[8][8])
{
	int casellaUnivoca, n, l, z=0;
	//Creo la pedina vuota
	pedine[32].player = 2; pedine[32].moved = 2; pedine[32].tipo='V';
/********************************
initPedina()
Funzione interna a inizio che si occupa di inizializzare
ogni pedina.

Parametri in ingresso:
    tipo_:indica il tipo di pedina da creare
Valore restituito:
    nessuno
********************************/
	void initPedina(char tipo_)
	{
		pedine[z].player = (n<5)? 0 : 1;
		pedine[z].moved = 0;
		pedine[z].n = n; pedine[z].l = l;
		pedine[z].tipo = tipo_;
		board[n][l] = &pedine[z];
		z++;
		return;
	}
	for(n=0; n<8; n++)
	{
		for(l=0; l<8; l++)
		{
			casellaUnivoca = 8*(n+1)-(8-(l+1)); //numero da 1 a 64 rappresentante casella
			switch(casellaUnivoca)
			{
				case 1: case 8: case 57: case 64://torre
					initPedina('T');
					break;
				case 2: case 7: case 58: case 63://cavallo
					initPedina('C');
					break;
				case 3: case 6: case 59: case 62://alfiere
					initPedina('A');
					break;
				case 4: case 60://regina
					initPedina('R');
					break;
				case 5: case 61://re
					initPedina('D');
					break;
				case 9 ... 16: case 49 ... 56://pedone
					initPedina('P');
					break;
				default://vuota
					board[n][l]=&pedine[32];
					break;
			}
		}
	}
}

/*****************************************
getNames()
Prende in ingresso e salva i nomi dei 2 giocatori

Parametri in ingresso:
    name 1: puntatore alla struttura nella quale verrà salvato il nome del primo giocatore
    name 2: puntatore alla struttura nella quale verrà salvato il nome del secondo giocatore

Valore restituito: nessuno
*****************************************/
void getNames(char *name1, char *name2)
{
	printf("\nInserire nome Giocatore1: ");
	scanf("%s%*c", name1);
	printf("\nInserire nome Giocatore2: ");
	scanf("%s%*c", name2);
	clear();
}

/*****************************************
newtime()
Si occupa di gestire il salvataggio della durata di ogni mossa.
Prende il tempo, tramite l'orologio interno del pc, all'inizio
della mossa, lo prende di nuovo alla fine e calcola la differenza.
Dopodichè questo valore viene salvato in una struttura che viene
messa in una lista doppiamente linkata.
Viene inoltre salvato il nome del giocatore che ha eseguito la
mossa e le coordinate di partenza e arrivo della mossa ,
in modo da poter stampare a fine partita, associati alla durata,
il nome del giocatore che ha fatto la mossa e che pedina è stata
spostata.

Parametri in ingresso:
    name: puntatore al nome del giocatore attuale
    testa: puntatore alla testa della lista
    nuova: puntatore alla nuova "cella" della lista
    pos1: posizione di partenza della pedina
    pos2: posizione di arrivo della pedina
    prima_della_mossa: orario all'inizio della mossa

Valori restituiti:
    testa: puntatore alla testa della lista
******************************************/
struct mossa* newtime(int player, struct mossa *testa, struct mossa *nuova, posizione *pos1, posizione *pos2,time_t prima_della_mossa, int numero_celle, char *name1, char *name2)
{
	char L[8] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h'};
	char flag;
	FILE *mosse;
	time_t dopo_la_mossa;
	time(&dopo_la_mossa);
	double differenza;
	differenza=difftime(dopo_la_mossa, prima_della_mossa);
	nuova=(struct mossa*)malloc(sizeof(struct mossa));
	nuova->player = player;
	sprintf(nuova->coordinate, "%c(%c-%d) in (%c-%d)", pos1->loc->tipo, L[pos1->l], pos1->n+1, L[pos2->l], pos2->n+1);
	nuova->durata_mossa=differenza;
	nuova->prev=NULL;
	nuova->next=testa;
	if(testa!=NULL)
	{
		testa->prev=nuova;
	}
	testa=nuova;
	mosse = (numero_celle==1)? fopen("mosse.txt", "w") : fopen("mosse.txt", "a");
	fprintf(mosse, "%s: %s ~ %.2lf secs\n", (player==0)?name1:name2, testa->coordinate, testa->durata_mossa);
	fclose(mosse);
	return testa;
}

/*********************************************
coda()
Prendendo i dati dal risultato della funzione dichiaroMatto,
copia in una coda le dieci mosse più veloci e le stampa a video.
Se sono state fatte meno di 10 mosse si ferma prima.

Parametri in ingresso:
    mosse_da_ordinare: puntatore all'array di struct precedentemente creato, con le mosse
                       ordinate
    numero_celle: numero delle mosse eseguite

Valore restituito: nessuno

*********************************************/
void coda(struct mossa *mosse_da_ordinare,int numero_celle, char *name1, char *name2)
{
	char deb[51];
	int contatore,contatore2=0, i;
	struct mossa *nuovo_elemento_coda;
	struct mossa *temp;
	struct Coda coda;
	coda.primo = NULL;
	coda.ultimo = NULL;
	printf("\nLe 10 mosse piÃ¹ veloci sono state:\n");
	for(contatore=0; contatore < 10 && contatore < numero_celle; contatore++)
	{
		nuovo_elemento_coda=(struct mossa*)malloc(sizeof(struct mossa));
		nuovo_elemento_coda->durata_mossa=mosse_da_ordinare[contatore].durata_mossa;
		nuovo_elemento_coda->player = mosse_da_ordinare[contatore].player;
		for(i=0; i<15; i++)
		{
			nuovo_elemento_coda->coordinate[i] = mosse_da_ordinare[contatore].coordinate[i];
		}
		if (coda.primo==NULL)
		{
			coda.primo=nuovo_elemento_coda;
		}
		else
		{
			coda.ultimo->next = nuovo_elemento_coda;
		}
		coda.ultimo = nuovo_elemento_coda;
	}
	temp = coda.primo;
	while(contatore2<contatore) //le stampo finchÃ¨ non ne trovo una null o finchÃ¨ non arrivo a 10
	{
		printf("(%d)%s, %s. Durata mossa %.2lf secondi\n", (contatore2+1), (temp->player==0)?name1:name2, temp->coordinate, temp->durata_mossa);
		temp=temp->next;
		contatore2++;
	}
	return;
}

/*************************************
boardCopy()
Esegue una copia della scacchiera e della posizione
delle pedine

Parametri in ingresso:
    board: scacchiera con la posizione di tutte le pedine
    testBoard: scacchiera temporanea?

Valore restituito: nessuno
**************************************/
void boardCopy(pedina *board[8][8], pedina *testBoard[8][8])
{
	int n, l;
	for(n=0; n<8; n++)
	{
		for(l=0; l<8; l++)
		{
			testBoard[n][l] = board[n][l];
		}
	}
}
/*********************************************
pedCopy()
Copia i valori dell'array "pedine" nell'array "testPedine"

Parametri in ingresso:
    pedine: puntatore alla posizione della pedina
    testPedine: puntatore temporaneo alla posizione delle pedine?

Valore restituito: nessuno
*********************************************/
void pedCopy(pedina *pedine, pedina *testPedine)
{
	int i;
	for(i=0; i<33; i++)
	{
		testPedine[i] = pedine[i];
	}
}
/**********************************************
checkMatto()
Se il re non riesce da solo a liberarsi dallo scacco, questa funzione
controlla se ciò è possibile muovendo un'altra pedina.

Parametri in ingresso:
    lr: coordinata 1 della posizione del re
    nr: coordinata 2 della posizione del re
    board: scacchiera con la posizione di tutte le pedine
    pedine: puntatore alle pedine
    player: riferimento al giocatore corrente

Valore restituito
    1: pone fine ai controlli
**********************************************/
int checkMatto(int lr, int nr, pedina *board[8][8], pedina *pedine, int player)
{
	debug("inizio checkMatto");
	char deb[51];
	int o_player, i, j, z, lt, nt, n, l, passaggi, step, flag, deb1=0, deb2=0, deb3=0;
	int L[8] = {0, 1, 1, 1, 0, -1, -1, -1};
	int N[8] = {1, 1, 0, -1, -1, -1, 0, 1};
	pedina tmpPedine[33], *testBoard[8][8];
	posizione pos1, pos2;
	o_player=(player==0)?1:0;
	for(n=0; n<8; n++)
	{
		for(l=0; l<8; l++)
		{
			debug("checkMatto newCasella");
			boardCopy(board, testBoard);
			if((*testBoard[n][l]).player != o_player) {continue;}
			if((*testBoard[n][l]).tipo == 'R') {continue;}
			debug("checkMatto newPedina");
			deb1++;
			pedCopy(pedine, tmpPedine);
			pos1.n=n; pos1.l=l; pos1.loc=testBoard[n][l];
			for(passaggi=0; passaggi<16; passaggi++)
			{
				for(step=0, flag=0; flag==0; step++)
				{
					debug("checkMatto step");
					sprintf(deb, "step==%d", step); debug(deb);
					if(passaggi<8) {lt=lr+(L[passaggi]*step); nt=nr+(N[passaggi]*step);}
					else
					{
						//Testo i passi del cavallo
						switch(passaggi)
						{
							case 0: lt=lr+2; nt=nr+1; break;
							case 1: lt=lr+2; nt=nr-1; break;
							case 2: lt=lr-2; nt=nr+1; break;
							case 3: lt=lr-2; nt=nr-1; break;
							case 4: lt=lr+1; nt=nr+2; break;
							case 5: lt=lr-1; nt=nr+2; break;
							case 6: lt=lr+1; nt=nr-2; break;
							case 7: lt=lr-1; nt=nr-2; break;
						}
						flag=1;
					}
					if(lt>7 || lt<0 || nt>7 || nt<0) {debug("checkMatto over++"); flag=1; continue;}
					pos2.l=lt; pos2.n=nt; pos2.loc=testBoard[nt][lt];
					if(checkMove(pedine, testBoard, &pos1, &pos2, o_player, 0))
					{
						pedCopy(tmpPedine, pedine);
						debug("checkMatto fallito");
						return 0;
					}
					pedCopy(tmpPedine, pedine);
					boardCopy(board, testBoard);
				}
			}
		}
	}
	debug("checkMatto ok");
	sprintf(deb, "pedine considerate: %d", deb1); debug(deb);
	return 1;
}


/*************************************
roundRe()
Controlla se tutte le caselle attorno al Re sono sotto scacco

Parametri in ingresso:
    l: Coordinata 1 del re sotto controllo
    n: Coordinata 2 del re sotto controllo
    giocatore: riferimento al giocatore che mette sotto scacco
    board: scacchiera con la posizione di tutte le pedine
    pedine: puntatore alla pedina
    player: riferimento al giocatore attualmente attivo

Valore restituito:
    1: Pone fine ai controlli
**************************************/
int roundRe(int l, int n, int giocatore, pedina *board[8][8], pedina *pedine, int player)
{
	debug("inizio roundRe");
	pedina *testBoard[8][8], tmpPedine[33];
	char deb[51];
	int i, lt, nt, o_player;
	int L[8] = {0, 1, 1, 1, 0, -1, -1, -1};
	int N[8] = {1, 1, 0, -1, -1, -1, 0, 1};
	o_player=(player==0)?1:0;
	for(i=0; i<8; i++)
	{
		pedCopy(pedine, tmpPedine);
		sprintf(deb, "i=%d: ", i); debug(deb);
		sprintf(deb, "%d-%d", L[i], N[i]); debug(deb);
		lt = l+L[i];
		nt = n+N[i];
		sprintf(deb, "roundRe: %d-%d", lt, nt); debug(deb);
		if(lt>7 || nt>7 || lt<0 || nt<0) {continue;}
		sprintf(deb, ".player=%d; o_player=%d", (*board[nt][lt]).player, o_player); debug(deb);
		if ((*board[nt][lt]).player != o_player)
		{
			boardCopy(board, testBoard);
			move(l, n, lt, nt, testBoard, pedine);
			if(!scacco(lt, nt, giocatore, testBoard, pedine, player))
			{
				debug("Non matto");
				debug("fine roundRe");
				pedCopy(tmpPedine, pedine);
				return 0;
			}
		}
		pedCopy(tmpPedine, pedine);
	}
	debug("Fine roundRe");
	return 1;
}

/**************************************
debug()
codice utilizzato per gestire il debug.

parametri in ingresso:
    deb[51]: array di caratteri con le info di debug

valore restituito: nessuno
**************************************/
void debug(char deb[51])
{
	FILE *dFile;
	if(debuggo>0) { printf("\nDebug: \"%s\"\n", deb); }
	/**************************
	if(debuggo==2)
	{
		dFile = fopen("/root/programming/C/scacchiDebug.txt", "a");
		fprintf(dFile, "%s\n", deb);
		fclose(dFile);
	}
	**************************/
}

/*********************************
userInput()
Si occupa di salvare l'opzione scelta dal giocatore a inizio turno

Parametri in ingresso: nessuno

Valore restituito:
    ch: variabile dove è salvata l'opzione scelta dal giocatore

*********************************/

char userInput()
{
	char ch;
	int flag=1;
	while (flag==1)
	{
		ch = getch();

		switch(ch)
		{
			case 'q': case 'n': case 'h':
			case 'm': case 's': case 'c':
			case 'x':
				return ch;
				break;
		}
	}
}

/******************************************
dichiaroMatto()
Dopo aver accertato lo scacco matto, crea un array di strutture, dove
vengono salvate tutte le mosse eseguite, poi le ordina in base alla loro durata.
Infine chiama la funzione coda.

Parametri in ingresso:
    testa: puntatore alla testa della lista
    puntatore_ad_array_struct: puntatore all'array di strutture ^^
    numero_celle: numero delle mosse eseguite

Valore restituito:
    0
******************************************/
int dichiaroMatto(struct mossa *testa,struct mossa *puntatore_ad_array_struct,int numero_celle, char *name1, char *name2)
{
	debug("inizio dichiaroMatto");
	char deb[51];
	int contatore3=0;
	struct mossa *temp, *tmp, tmp1;
	struct mossa mosse_da_ordinare[numero_celle];
	puntatore_ad_array_struct=mosse_da_ordinare;
	debug("inizio primo debug dichiaroMatto");
	temp=testa;
	while(temp != NULL)
	{
		sprintf(deb, "giocatore: %s\n", (temp->player==0)?name1:name2); debug(deb);
		sprintf(deb, "mossa: %s\n", temp->coordinate); debug(deb);
		sprintf(deb, "durata: %.2lf secs\n", temp->durata_mossa); debug(deb);
		temp=temp->next;
		contatore3++;
	}
	debug("fine primo debug dichiaroMatto");
	contatore3=0;
	temp=testa;
	while(temp != NULL)
	{
		mosse_da_ordinare[contatore3]=*temp;
		temp=temp->next;
		contatore3++;
	}
	int i,j;
	for (i = (numero_celle - 1); i > 0; i--)
	{
		for (j = 1; j <= i; j++)
		{
			sprintf(deb, "%s", mosse_da_ordinare[j-1].coordinate); debug(deb);
			if (mosse_da_ordinare[j-1].durata_mossa > mosse_da_ordinare[j].durata_mossa)
			{
				tmp1 = mosse_da_ordinare[j-1];
				mosse_da_ordinare[j-1] = mosse_da_ordinare[j];
				mosse_da_ordinare[j] = tmp1;
			}
		}
	}
	debug("fine dichiaroMatto");
	coda(mosse_da_ordinare, numero_celle, name1, name2);
	return 0;
}


/*******************************
showBoard
Visualizza una scacchiare in caratteri, posizionando le pedine e le caselle vuote.

Parametri in ingresso:
    board: array bidimensionale di puntatori a struttura, con tutte le informazioni sulla scacchiera
           e la posizione delle varie pedine

Valore restituito: nessuno
*******************************/
void showBoard(pedina *board[8][8])
{
	char r1[60]="===|===|===|===|===|===|===|===|===|===";
	char r2[60]="---|---|---|---|---|---|---|---|---|---";
	char r3[60]="   | A | B | C | D | E | F | G | H |";
	char i, b;
	int l, n;
	
	char bold()
	{
	    if(board[n][l]->player==1)
	    {
            printf("\033[1m");
            //return '-';
	    }
	    return ' ';
	}
	char norm()
	{
		printf("\033[0m");
		if(board[n][l]->player==1)
		{
			//return '-';
		}
		return ' ';
	}
	printf("%s\n%s", r3, r1);
	for (n=0; n<=7; n++)
	{
		printf("\n%d  |", n+1);
		for (l=0; l<=7; l++)
		{
			i = board[n][l]->tipo;
			i = (i=='V')?' ':i;
			#ifndef _WIN32
			void bold() { printf("\033[1m"); }
			void norm() { printf("\033[0m"); }
			if(board[n][l]->player==1) { bold(); }
			printf(" %c ", i);
			norm();
			#else
			b = (board[n][l]->player==1)?'-':' ';
			printf("%c%c%c", b,i,b);
			#endif
			printf("|");
		}
		printf("  %d", n+1);
		if (n<7) { printf("\n%s", r2); }
	}
	printf("\n%s\n%s\n\n", r1, r3);
}


/**************************************
getCoordinate()
Si occupa di chiedere al giocatore di turno le coordinate per una mossa. Assieme alla funzione letToNum()
definisce le 4 variabili globali, contenenti le 4 coordinate in versione numerica, da 0 a 7.
Per la lettera ed il numero di partenza e di arrivo. Inoltre dichiara i due puntatori relativi
alla struttura “pedina” rappresentata dalle due posizioni.
Riporta errore se letToNum() ritorna un errore.

Parametri in ingresso:
    board:array bidimensionale di puntatori a struttura, con tutte le informazioni sulla scacchiera
           e la posizione delle varie pedine
    pos1: puntatore alla posizione di partenza della pedina
    pos2: puntatore alla posizione di arrivo della pedina
    name1: puntatore al nome del giocatore 1
    name2: puntatore al nome del giocatore 2
    player: giocatore corrente

Valore restituito:
    0 o 1: a seconda che le coordinate siano corrette o no
**************************************/
int getCoordinate(pedina *board[8][8], posizione *pos1, posizione *pos2, char *name, int player)
{
	char L[2], *giocatore, deb[51];
	int l1, l2, n1, n2;
/**************************************
letToNum()
Trasforma le due coordinate “lettera” nel corrispondente numero (da 0 a 7), completando il lavoro di getCoordinate().
Riporta errore se una delle 4 coordinate inserite dall'utente non è in {1,2,3,4,5,6,7,8} o in {a,b,c,d,e,f,g,h}

Parametri in ingresso: nessuno

Valore restituito:
    0 o 1: a seconda che le coordinate siano corrette o no
**************************************/
	int letToNum()
	{
		sprintf(deb, "l1 = |%d|(%c); l2 = |%d|(%c)", L[0], L[0], L[1], L[1]); debug(deb);
		int *ll[2], i;
		char deb[51];
		ll[0]=&l1; ll[1]=&l2;
		if(n1<1 || n1>8 || n2<1 || n2>8) { printf("Errore: coordinate n fuori range", n1, n2); return 0; }
		n1--;
		n2--;
		for(i=0; i<2; i++)
		{
			switch(L[i])
			{
				case 'a': *ll[i]=0; break;
				case 'b': *ll[i]=1; break;
				case 'c': *ll[i]=2; break;
				case 'd': *ll[i]=3; break;
				case 'e': *ll[i]=4; break;
				case 'f': *ll[i]=5; break;
				case 'g': *ll[i]=6; break;
				case 'h': *ll[i]=7; break;
				default: printf("Errore: coordinate l fuori range --> |%c|", L[i]);
				return 0;
			}
			sprintf(deb, "-[%d]-", *ll[i]); debug(deb);
		}
		debug("letToNum ok"); return 1;
	}
	printf("\n%s: Inserisci coordinate (LN LN):", name);
	scanf("%c%d %c%d%*c", &L[0], &n1, &L[1], &n2);
	sprintf(deb, "-%c-%d-%c-%d-", L[0], n1, L[1], n2); debug(deb);
	if(letToNum())
	{
		pos1->l=l1; pos1->n=n1; pos1->loc=board[n1][l1];
		pos2->l=l2; pos2->n=n2; pos2->loc=board[n2][l2];
		return 1;
	}
	else { return 0; }
}
/*************************************
checkMove()
Controlla che la mossa sia regolare.
Inizialmente verificato l'unico caso in cui il player di arrivo può essere lo stesso della partenza: l'arrocco.
Altrimenti si effettua un controllo specifico a seconda della tipologia di pedina della coordinata iniziale.
Questa e le funzioni da questa usate, cambiano comportamento a seconda se si sta eseguendo o meno il controllo di scacco
su una casella, identificato da checkScacco.
Da notare che la mossa della regina è convalidata con le regole di torre o di alfiere.
Riporta errore se i controlli utilizzati ritornano un errore.

Parametri in ingresso:
    pedine: puntatore alla pedina
    board: array bidimensionale di puntatori a struttura, con tutte le informazioni sulla scacchiera
           e la posizione delle varie pedine
    pos1: puntatore alla posizione di partenza della pedina
    pos2: puntatore alla posizione di arrivo della pedina
    player: giocatore corrente

Valore restituito:
    1: Ferma tutti i controlli
*************************************/
int checkMove(pedina *pedine, pedina *board[8][8], posizione *pos1, posizione *pos2, int player, int checkScacco)
{
	pedina *posRe, *testBoard[8][8], tmpPedine[33];
	int o_player, n1=pos1->n, l1=pos1->l, n2=pos2->n,l2=pos2->l;
	char deb[51];
	o_player=(player==0)?1:0;
	debug("inizio checkMove");
	posRe = (player==0)? pedine+3 : pedine+27;
	sprintf(deb, "posRe: %d - %d", posRe->l, posRe->n); debug(deb);
	sprintf(deb, "-%c-", (*pos1).loc->tipo); debug(deb);
	sprintf(deb, "in %d-%d", l2, n2); debug(deb);
	if((*pos2).loc->player == player && checkScacco==0)	//Valida SOLO per arrocco
	{
		if(checkArrocco(l1, n1, l2, n2, pos1, pos2, board, pedine, player))
		{
			return 1;
		}
		else {return 0;}
	}
	switch((*pos1).loc->tipo)
	{
		case 'P':
			if(!checkPedone(l1, n1, l2, n2, pos1, pos2, board, player, checkScacco)) {return 0;}
			break;
		case 'T':
			if(!checkTorre(l1, n1, l2, n2, board)) {return 0;}
			break;
		case 'C':
			if(!checkCavallo(l1, n1, l2, n2)) {return 0;}
			break;
		case 'A':
			if(!checkAlfiere(l1, n1, l2, n2, board)) {return 0;}
			break;
		case 'D':
			if(!checkTorre(l1, n1, l2, n2, board) && !checkAlfiere(l1, n1, l2, n2, board)) {return 0;}
			break;
		case 'R':
			if(checkRe(l1, n1, l2, n2, board) && !scacco(l2, n2, o_player, board, pedine, player))
			{
				if(!checkScacco) {move(l1, n1, l2, n2, board, pedine);}
				return 1;
			}
			else {return 0;}
			break;
	}
	if(!checkScacco)
	{
		boardCopy(board, testBoard);
		pedCopy(pedine, tmpPedine);
		move(l1, n1, l2, n2, testBoard, pedine);
		if (scacco(posRe->l, posRe->n, o_player, testBoard, pedine, player))
		{
			printf("\nRE ancora sotto scacco\n");
			pedCopy(tmpPedine, pedine);
			return 0;
		}
		pedCopy(tmpPedine, pedine);
		move(l1, n1, l2, n2, board, pedine);
	}
	return 1;
}


/*************************************
checkPedone()
Verifica lo spostamento del pedone. Può avere 3 casi differenti.
La mangiata: se il pedone viene mosso di un passo in diagonale, in direzione consona all suo .player.
La casella di arrivo deve essere una pedina nemica, a parte in caso di controllo di scacco.
Spostamento di 1: si sposta di un passo in avanti. La casella di arrivo deve essere vuota. Non valido per il controllo di scacco.
Spostamento di 2: si sposta di 2 passi in avanti. Possibile solo al primo passo, la casella di arrivo deve essere vuota.
Non valido per il controllo di scacco.

Parametri in ingresso:
    l1: coordnata 1 posizione di partenza
    n1: coordinata 2 della posizione di partenza
    l2: coordinata 1 della posizione di arrivo
    n2: coordinata 2 della posizione di arrivo
    pos1: puntatore alla posizione di partenza della pedina
    pos2: puntatore alla posizione di arrivo della pedina
    board: array bidimensionale di puntatori a struttura, con tutte le informazioni sulla scacchiera
           e la posizione delle varie pedine
    player: giocatore corrente

Valore restituito:
    0
**************************************/
int checkPedone(int l1, int n1, int l2, int n2, posizione *pos1, posizione *pos2, pedina *board[8][8], int player, int checkScacco)
{
	int step, o_player;
	o_player=(player==0)?1:0;
	//pedone che mangia
	if((abs(l2-l1)==1 && n2==n1+1 && (*pos1).loc->player==0) ||
		(abs(l2-l1)==1 && n2==n1-1 && (*pos1).loc->player==1))//controllo posizione finale
	{
		if (((*pos2).loc->player == o_player && checkScacco==0) || checkScacco==1)
		{debug("controllo pedone mangia"); return 1;}
	}
	//pedone si muove di 1
	if ((l2==l1 && n2==n1+1 && (*pos1).loc->player==0) ||
		(l2==l1 && n2==n1-1 && (*pos1).loc->player==1))//controllo posizione finale
	{
		if((*pos2).loc->tipo == 'V' && checkScacco==0)
		{
			if((*pos2).n == 0 || (*pos2).n == 7) {(*pos1).loc->tipo = promozione();}
			debug("controllo pedone muove 1");
			return 1;
		}
	}
	//pedone si muove di 2
	if (checkScacco==0 && ((l2==l1 && n2==n1+2 && (*pos1).loc->moved==0 && (*pos1).loc->player==0) ||
		(l2==l1 && n2==n1-2 && (*pos1).loc->moved==0 && (*pos1).loc->player==1)))//controllo posizione finale
	{
		step=(player==0)? n1+1 : n1-1;
		if((*board[step][l2]).tipo == 'V' && (*pos2).loc->tipo == 'V') {debug("controllo pedone muove 2"); return 1;}
	}
	debug("Controllo pedone fallito");
	return 0;
}
/**************************************
promozione()
Promuove il pedone nel caso giunga in fondo alla scacchiera.

Parametro in ingresso: nessuno

Valore restituito:
    ch: lettera che identifica la pedina scelta
***************************************/
char promozione()
{
	char ch;
	printf("\nIn cosa vuoi promuovere il pedone?\n");
	while(1)
	{
		printf("\nP - C - A - D\n");
		ch = getch()-32;
		switch(ch)
		{
			case 'P': case 'C': case 'A': case 'D':
				return ch;
			default:
				debug("\nCarattere errato: reinserire\n");
		}
	}
}
/***********************************
checkTorre()
Verifica lo spostamento della torre. Al variare di uno degli assi, l'altro deve restare invariato.

Parametri in ingresso:
    l1: coordnata 1 posizione di partenza
    n1: coordinata 2 della posizione di partenza
    l2: coordinata 1 della posizione di arrivo
    n2: coordinata 2 della posizione di arrivo
    board: array bidimensionale di puntatori a struttura, con tutte le informazioni sulla scacchiera
           e la posizione delle varie pedine

Valore restituito:
    1 o 0: mossa non corretta o corretta
***********************************/
int checkTorre(int l1, int n1, int l2, int n2, pedina *board[8][8])
{
	int step, c2, *cl, *cn;
	if((l2==l1 && n1!=n2) || (l2!=l1 && n2==n1))	//controllo posizione finale
	{
		if(l1==l2) {cl=&l1, cn=&step, step=n1, c2=n2;}	//caso in cui si muove lungo un l
		else {cl=&step, cn=&n1, step=l1, c2=l2;} 		//caso in cui si muove lungo un n
		while (abs(step-c2) != 1) //finchÃ¨ non si affianca alla cella di arrivo
		{
			step=((step-c2) < 0)? step+1 : step-1;
			if((*board[*cn][*cl]).tipo != 'V')
			{
				debug("Controllo torre fallito");
				return 0;
			}
		}
		debug("controllo torre ok");
		return 1;
	}
	else
	{
		debug("Controllo torre fallito");
		return 0;
	}
	return 1;
}
/*******************************************
checkCavallo()
Verifica lo spostamento del cavallo. Uno degli assi deve variare di 2, mentre l'altro di 1.

Parametri in ingresso:
    l1: coordnata 1 posizione di partenza
    n1: coordinata 2 della posizione di partenza
    l2: coordinata 1 della posizione di arrivo
    n2: coordinata 2 della posizione di arrivo

Valore restituito:
    1 o 0: mossa non corretta o corretta
********************************************/
int checkCavallo(int l1, int n1, int l2, int n2)
{
	if(abs(l1-l2)==2 && abs(n1-n2)==1 || abs(l1-l2)==1 && abs(n1-n2)==2)//controllo posizione finale
	{
		debug("controllo cavallo ok");
		return 1;
	}
	else
	{
		debug("Controllo cavallo fallito");
		return 0;
	}
}
/*********************************************
checkAlfiere()
Verifica lo spostamento dell'alfiere. Entrambi gli assi devono variare della stessa quantità.

Parametri in ingresso:
    l1: coordnata 1 posizione di partenza
    n1: coordinata 2 della posizione di partenza
    l2: coordinata 1 della posizione di arrivo
    n2: coordinata 2 della posizione di arrivo
    board: array bidimensionale di puntatori a struttura, con tutte le informazioni sulla scacchiera
           e la posizione delle varie pedine

Valore restituito:
    1 o 0: mossa non corretta o corretta
**********************************************/
int checkAlfiere(int l1, int n1, int l2, int n2, pedina *board[8][8])
{
	char deb[51];
	int cl=l1, cn=n1;
	if(abs(l1-l2)==abs(n1-n2))//controllo posizione finale
	{
		while(abs(cl-l2) != 1) //finchÃ¨ non si affianca alla cella di arrivo
		{
			cl=((l1-l2) < 0)? cl+1 : cl-1;
			cn=((n1-n2) < 0)? cn+1 : cn-1;
			sprintf(deb, " %d-%d ", cl, cn); debug(deb);
			if((*board[cn][cl]).tipo != 'V')
			{
				debug("Controllo alfiere fallito");
				return 0;
			}
		}
	}
	else
	{
		debug("Controllo alfiere fallito");
		return 0;
	}
	debug("controllo alfiere ok");
	return 1;
}
/***********************************************
checkRe()
Verifica lo spostamento del re. Si muove come la regina, ma solo di 1 passo.
Da errore se non si sta eseguendo il controllo di scacco e contemporaneamente se la casella in cui lo si vuole muovere è sotto scacco.

Parametri in ingresso:
    l1: coordnata 1 posizione di partenza
    n1: coordinata 2 della posizione di partenza
    l2: coordinata 1 della posizione di arrivo
    n2: coordinata 2 della posizione di arrivo
    board: array bidimensionale di puntatori a struttura, con tutte le informazioni sulla scacchiera
           e la posizione delle varie pedine

Valore restituito:
    1 o 0: mossa non corretta o corretta

************************************************/
int checkRe(int l1, int n1, int l2, int n2, pedina *board[8][8])
{
	if (abs(l1-l2)==abs(n1-n2) && abs(l1-l2)==1 || l1==l2 && abs(n1-n2)==1 || abs(l1-l2)==1 && n1==n2)//controllo posizione finale
	{
		debug("controllo re ok");
		return 1;
	}
	else
	{
		debug("Controllo re fallito");
		return 0;
	}
}
/************************************************
scacco()
Verifica se la casella alle coordinate date è sotto scacco. Utilizza per lo scopo la funzione checkMove().
Per questo motivo, salva momentaneamente tutte le variabili globali utilizzate per il controllo degli spostamenti,
sostituendole con altre momentanee. Solo alla fine si occuperà di reimpostare le variabili globali ai loro valori originali.
Imposta checkScacco a 1, solo alla fina lo riporta a 0.
Se la casella da controllare contiene il re avversario, allora verranno prese in considerazione le pedine del giocatore di turno:
sarebbe la verifica dello scacco al re, dopo ogni mossa. Altrimenti (casella vuota) saranno le pedine avversarie a verificare
i percorsi. Le momentanee l2, n2, pos2, sono impostate secondo le 2 coordinate date in argomento.
Vengono poi sottoposte al vaglio tutte le caselle della scacchiera, tramite board, se si capita in una pedina
appartenente al giocatore richiesto, vengono cambiate l1, n1, pos1 relativamente alle coordinate della pedina attualmente sotto controllo. Quindi si esegue checkMove(), con le nuove variabili impostate, così da sapere se la pedina può arrivare alla casella sotto controllo, qunidi avendola sotto scacco.
Da errore se la casella non è sotto scacco.

Parametri in ingresso:
    l: coordinata 1 della posizione del re
    n: coordinata 2 della posizione del re
    giocatore: giocatore da utilizzare per testare lo scacco
    board:array bidimensionale di puntatori a struttura, con tutte le informazioni sulla scacchiera
           e la posizione delle varie pedine
    pedine: puntatore alla pedina
    player: giocatore attuale

Valore restituito:
    1 o 0: non sotto scacco o sotto scacco
************************************************/
int scacco(int l, int n, int giocatore, pedina *board[8][8], pedina *pedine, int player)
{
	char deb[51];
	debug("inizio checkScacco");
	sprintf(deb, "su: %d - %d", l, n); debug(deb);
	posizione pos1, pos2;
	pos2.l=l; pos2.n=n; pos2.loc=board[n][l];
	for(n=0; n<8; n++)
	{
		for(l=0; l<8; l++)
		{
			pos1.l=l; pos1.n=n; pos1.loc=board[n][l];
			if((pos1.loc->player == giocatore) && checkMove(pedine, board, &pos1, &pos2, player, 1))
			{
				debug("sotto scacco");
				sprintf(deb, "Sotto scacco da:%c %d-%d", pos1.loc->tipo, l, n);
				return 1;
			}
		}
	}
	debug("fine checkScacco");
	return 0;
}
/***********************************************
checkArrocco()
Controlla se è possibile eseguire l'arrocco

Parametri in ingresso:
    l1: coordnata 1 posizione di partenza
    n1: coordinata 2 della posizione di partenza
    l2: coordinata 1 della posizione di arrivo
    n2: coordinata 2 della posizione di arrivo
    pos1: puntatore alla posizione di partenza della pedina
    pos2: puntatore alla posizione di arrivo della pedina
    board: array bidimensionale di puntatori a struttura, con tutte le informazioni sulla scacchiera
           e la posizione delle varie pedine
    pedine: puntatore alle pedine
    player: giocatore corrente

Valore restituito:
    0 o 1: mossa corretta o non corretta
************************************************/
int checkArrocco(int l1, int n1, int l2, int n2, posizione *pos1, posizione *pos2, pedina *board[8][8], pedina *pedine, int player)
{
	int o_player, lr, lt, step=l1;
	o_player=(player==0)?1:0;
	debug("inizio checkArrocco");
	if ((*pos1).loc->tipo == 'R' && (*pos1).loc->moved == 0 && (*pos2).loc->tipo == 'T' && (*pos2).loc->moved == 0)
	{
		lr = (l2==0)? l1-2 : l1+2;
		lt = (l2==0)? l1-1 : l1+1;
		if (scacco(l1, n1, o_player, board, pedine, player))
		{
			debug("Conrollo arrocco fallito");
			debug("\nRE sotto scacco");
			return 0;
		}
		while (abs(step-l2) != 1)
		{
			step=((step-l2) < 0)? step+1 : step-1;
			if(board[n1][step]->tipo != 'V')
			{
				debug("Controllo arrocco fallito");
				debug("\nOstacolo tra RE e torre");
				return 0;
			}
		}
		if (scacco(lr, n1, o_player, board, pedine, player) || scacco(lt, n1, o_player, board, pedine, player))
			{
				debug("Controllo arrocco fallito");
				debug("\nPassaggio di RE sotto scacco");
				return 0;
			}
	}
	else
	{
		debug("Controllo arrocco fallito");
		return 0;
	}
	move(l2, n2, lt, n2, board, pedine);
	move(l1, n1, lr, n2, board, pedine);
	debug("controllo arrocco ok");
	return 1;
}
/***************************************
move()
Esegue lo spostamento della pedina.
Lo spostameto avviene tramite un reindirizzamento dei puntatori in board, le coordinate di arrivo verranno puntate
alla pedina presente nella casella di partenza; quelle di partenza alla struttura di casella vuota.

Parametri in ingresso:
    l1: coordnata 1 posizione di partenza
    n1: coordinata 2 della posizione di partenza
    l2: coordinata 1 della posizione di arrivo
    n2: coordinata 2 della posizione di arrivo
    board: array bidimensionale di puntatori a struttura, con tutte le informazioni sulla scacchiera
           e la posizione delle varie pedine
    pedine: puntatore alle pedine

Valore restituio: nessuno
***************************************/
void move(int l1, int n1, int l2, int n2, pedina *board[8][8], pedina *pedine)
{
	debug("muovo");
	printf("\nmossa: %d%d in %d%d\n", l1, n1, l2, n2);
	board[n1][l1]->moved++;
	board[n2][l2] = board[n1][l1];
	board[n2][l2]->n = n2;
	board[n2][l2]->l = l2;
	board[n1][l1] = pedine+32;
}
/****************************************
nexTurn()
Esegue il cambio di turno, cambiando valore del player di turno.

Parametri in ingresso:
    player: giocatore attuale

Valore restituito: nessuno
*****************************************/
void nexTurn(int *player)
{
	*player = (*player==0)? 1 : 0; //cambio giocatore di turno
}


/************************************
vittoria()
Avviata alla convalida dello scacco matto, avvia la funzione dichiaroMatto

Parametri in ingresso:
    player: giocatore attuale
    name1: nome giocatore 1
    name2: nome giocatore 2
    testa: testa della lista
    puntatore_ad_array_struct: puntatore ad array di strutture
    numero_celle: numero delle mosse eseguite
************************************/
char vittoria(int player,char *name1,char *name2,struct mossa *testa,struct mossa *puntatore_ad_array_struct,int numero_celle)
{
	char rigioca;
	if(player==0)
	{
		printf("Complimenti %s, hai vinto!\n",name1);
	}
	else
	{
		printf("Complimenti %s, hai vinto!\n",name2);
	}
	printf("Premere un tasto qualsiasi per visualizzare le mosse più veloci");
	getch();
	dichiaroMatto(testa,puntatore_ad_array_struct,numero_celle, name1, name2);
	printf("\n\nVuoi giocare ancora? Premi y per riniziare, un altro tasto per uscire.");
	rigioca=getch();
	return rigioca;
}
/***************************************
conteggio_celle()
Tiene il conto delle mosse eseguite

Parametri in ingresso:
    numero_celle: numero delle mosse eseguite

Valore restituito:
    numero_celle: numero delle mosse eseguite
****************************************/
int conteggio_celle(int numero_celle)
{
	numero_celle++;
	return numero_celle;
}
