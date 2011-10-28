/*barber:
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
Debuggo has to be defined at 1 if you want to see debug.
Warning: there could be a lot of output, that can even slow your game
**/
#define debuggo 0

/*Declaring various type of structures needed*/

/*Pedina: struct containing information about every pawn*/
typedef struct
{
	int player;/*Player owner*/
	int moved;/*The pawn has been moved? (Needed for castling and starting move of the pawn*/
	int l;/*Coordinate n°1 of the position*/
	int n;/*Coordinate n°2 of the position*/
	char tipo;/*What kind of pawn?*/
} pedina;

/*Posizione: information about pawn' position*/
typedef struct
{
	int l;/*Coordinate n°1 of the position*/
	int n;/*Coordinate n°2 of the position*/
	pedina *loc;/*Puntator to the casella*/
} posizione;

/*Salvataggio: struct used to save data in a binary file*/
typedef struct
{
	int player;/*Current player*/
	int numero_celle;/*Moves number*/
	char name1[31];/*Player 1 name*/
	char name2[31];/*Player 2 name*/
	pedina pedine[33];/*Array containing pawn structures */
	int board[8][8];/*Array representing chessboard*/
} salvataggio;

/*Mossa: information about last move*/
struct mossa{
	int player;/*Player name that has done the last move*/
	char coordinate[15];/*Coordinate of beginning and arrive*/
    double durata_mossa;/*Move's duration in seconds*/
    struct mossa*next;/*Puntator to next structure*/
    struct mossa*prev;/*Puntator to previous structure*/};

/*Coda: structure where 10 fastest moves will be saved*/
struct Coda
{
	struct mossa *primo;/*head*/
	struct mossa *ultimo;/*tail*/
};


/*Declaring functions*/
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
#define MENU "Q: quit ## N: new game ## H: help ## M: Move\nS: Stall ## X: Save Game ## C: Load Game"
int getch(void);
#include "for_linux.c"
#else
#define MENU "Q: quit ## N: new game ## H: help ## M: Move\nS: Stall"
#endif


/*MAIN*/
int main()
{
	struct mossa *testa; /*Puntator to list's head*/
	struct mossa *nuova; /*Puntator to list's tail*/
	struct mossa *nuova_da_ordinare; /*Puntator to new move BEFORE is sorted*/
	struct mossa *testa_da_ordinare; /*Puntator to tail's head to be sorted*/
	struct mossa *temp; /*Puntator to temporary structure*/
	struct mossa *puntatore_ad_array_struct; /*Puntator to structure's array*/
	testa=NULL;/*Initializing head to NULL*/
	testa_da_ordinare=NULL;/*Initializing head to be sorted to NULL*/
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
					debug("Initializing head's test");
					temp=testa;
					while(temp != NULL)
					{
						sprintf(deb, "player: %s\n", (temp->player==0)?name1:name2); debug(deb);
						sprintf(deb, "move: %s\n", temp->coordinate); debug(deb);
						sprintf(deb, "duration: %.2lf secs\n", temp->durata_mossa); debug(deb);
						temp=temp->next;
						contatore3++;
					}
					debug("End head's test");
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
			else { printf("\nWrong coordinates, re-enter\n"); }
		}
		toTest = (player==0)? pedine+27 : pedine+3;	//Re avversario
		if (scacco(toTest->l, toTest->n, player, board, pedine, player)) {sottoScacco=1; printf("\nScacco al RE\n"); }
		if (sottoScacco)
		{
			debug("UNDER CHECK");
			if(roundRe(toTest->l, toTest->n, player, board, pedine, player))
			{
				debug("maybe mate??\n");
				if(checkMatto(toTest->l, toTest->n, board, pedine, player))
				{
				    clear();
					printf("\nCheck mate!!!!!\n");
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
						printf("\nThank for playing! Have a nice day.");
						getch();
						exit(0);
					}
				}
				else {debug("not mate..");}
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
//DEFINING FUNCTIONS
//-----------------

/********************************
clear()
Recalling right function to clear console


Entering parameters: none

Returned value: none
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
Start the game, and it has to be called
if you want to start a new game.
It contains 2 functions: inizio() and getNames()

Entering parameters:
   pedine: The structs containing all pawn data
    board: bidimensional array of struct puntators, with all the information
           about the chessboard and the position of every pawn
    name1: player 1 name's
    name2: player 2 name's

Returned value: none
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
Creating structures for pawn (32) and the structure of empty square:
they're contained in the array pedine[32]. It also link every square with the structure (correlated) to it.
The 64 (caselle) are in the bidimensional array board[8][8].
In this way the arrays are linked to represent the starting situation of the game.
So, you can access to everything "board", with a simple coordinate call.

Entering parameters::
    pedine: The structs containing all pawn data
    board: bidimensional array of struct puntators, with all the information
           about the chessboard and the position of every pawn


********************************/
void inizio(pedina *pedine, pedina *board[8][8])
{
	int casellaUnivoca, n, l, z=0;
	/*Creating empty pawn*/
	pedine[32].player = 2; pedine[32].moved = 2; pedine[32].tipo='V';
/********************************
initPedina()
Function caontained in inizio that initialize every pawn.
Entering parameter:
    tipo_:kind of pawn to create
Returned value: none
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
			casellaUnivoca = 8*(n+1)-(8-(l+1)); /*number from 1 to 64 representing the square*/
			switch(casellaUnivoca)
			{
				case 1: case 8: case 57: case 64:/*rook*/
					initPedina('T');
					break;
				case 2: case 7: case 58: case 63:/*knight*/
					initPedina('C');
					break;
				case 3: case 6: case 59: case 62:/*bishop*/
					initPedina('A');
					break;
				case 4: case 60:/*queen*/
					initPedina('R');
					break;
				case 5: case 61:/*king*/
					initPedina('D');
					break;
				case 9 ... 16: case 49 ... 56:/*pawn*/
					initPedina('P');
					break;
				default:/*empty*/
					board[n][l]=&pedine[32];
					break;
			}
		}
	}
}

/*****************************************
getNames()
Input and save the players names

Entering parameters:
    name1: player 1 name's
    name2: player 2 name's

Returned value: none
*****************************************/
void getNames(char *name1, char *name2)
{
	printf("\nInsert Player1 name: ");
	scanf("%s%*c", name1);
	printf("\nInsert Player2 name: ");
	scanf("%s%*c", name2);
	clear();
}

/*****************************************
newtime()
Manage the saving of the time of every move.
It take the time, using the CPU clock, at the beginning of every move, 
it take it again at the end and make the difference between them.
Then this value is saved in a structure that is put in a double-linked list.
It also save the name of the player that moved and the starting coordinates
so that you can print them at the end of the game, the duration,
player name that moved and pawn that has been moved.

Entering parameters:
    name: puntator to current player name
    testa: puntator to list's head
    nuova: puntator to new "cell" of the list
    pos1: pawn's starting position
    pos2: pawn's arriving position
    prima_della_mossa: CPU time at the move's beginning

Returned value:
    testa: puntator to list's head
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
Picking the data from the function dichiaroMatto,
copy in a tail the ten fastest moves and it print them to screen.
If there were less than ten moves, it stop.

Entering parameters:
    mosse_da_ordinare: puntator to struct's array previously created, with 
                       sorted moves
    numero_celle: moves number

Returned value: none

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
	printf("\n10 fastest moves were:\n");
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
	while(contatore2<contatore) /*Print them until I get a NULL or I have printed 10 of them*/
	{
		printf("(%d)%s, %s. Move duration %.2lf seconds\n", (contatore2+1), (temp->player==0)?name1:name2, temp->coordinate, temp->durata_mossa);
		temp=temp->next;
		contatore2++;
	}
	return;
}

/*************************************
boardCopy()
Copy the chessboard and pawns position

Entering parameters:
    board: chessboard with position of every pawn
    testBoard: temporary chessboard

Returned value: none
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
Copy values in "pedine" into "testPedine"

Entering parameters:
    pedine: puntator to pawn position
    testPedine: temporary puntatorto pawn position

Returned value: none
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
If the king can't free himself from check, this function check
if this is possible moving another pawn.

Entering parameters:
    lr: coordinate 1 of the king
    nr: coordinate 2 of the king
    board: chessboard with every pawn position
    pedine: puntator to pawn
    player: referral to current player

Returned value
    1: end of controls
**********************************************/
int checkMatto(int lr, int nr, pedina *board[8][8], pedina *pedine, int player)
{
	debug("starting checkMatto");
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
						/*Testing knight's steps*/
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
						debug("checkMatto failed");
						return 0;
					}
					pedCopy(tmpPedine, pedine);
					boardCopy(board, testBoard);
				}
			}
		}
	}
	debug("checkMatto ok");
	sprintf(deb, "considered pawns: %d", deb1); debug(deb);
	return 1;
}


/*************************************
roundRe()
check if every square around the king is under check

Entering parameters:
    l: coordinate 1 of the king
    n: coordinate 2 of the king
    giocatore: referral to the player that made check
    board: chessboard with every pawn position
    pedine: puntator to pawn
    player: referral to current player
Returned value
    1: end of controls
**************************************/
int roundRe(int l, int n, int giocatore, pedina *board[8][8], pedina *pedine, int player)
{
	debug("starting roundRe");
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
				debug("Not mate");
				debug("ending roundRe");
				pedCopy(tmpPedine, pedine);
				return 0;
			}
		}
		pedCopy(tmpPedine, pedine);
	}
	debug("Ending roundRe");
	return 1;
}

/**************************************
debug()
Manage debug (verbose mode).

Entering parameters:
    deb[51]: charachters rray with debug information

Returned value: none
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
Save the choosen option from the player at the beginning

Entering parameters: none

Returned value:
    ch: choosen option

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
After checking for check mate, create a structure's array, 
where are saved moves, then it order for duration.
Then it call coda function.

Entering parameters:
    testa: puntator to list's head
    puntatore_ad_array_struct: puntator to array of structures
    numero_celle: moves number

Returned value:
        0
******************************************/
int dichiaroMatto(struct mossa *testa,struct mossa *puntatore_ad_array_struct,int numero_celle, char *name1, char *name2)
{
	debug("starting dichiaroMatto");
	char deb[51];
	int contatore3=0;
	struct mossa *temp, *tmp, tmp1;
	struct mossa mosse_da_ordinare[numero_celle];
	puntatore_ad_array_struct=mosse_da_ordinare;
	debug("starting first debug dichiaroMatto");
	temp=testa;
	while(temp != NULL)
	{
		sprintf(deb, "player: %s\n", (temp->player==0)?name1:name2); debug(deb);
		sprintf(deb, "move: %s\n", temp->coordinate); debug(deb);
		sprintf(deb, "duration: %.2lf secs\n", temp->durata_mossa); debug(deb);
		temp=temp->next;
		contatore3++;
	}
	debug("end first debug dichiaroMatto");
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
	debug("end dichiaroMatto");
	coda(mosse_da_ordinare, numero_celle, name1, name2);
	return 0;
}


/*******************************
showBoard
Show a charachter chessboard, putting pawns and empty (caselle).

Entering parameters:
    board: bidimensional array of puntator to struct, with all information about chessboard
           and position of all pawns

Returned value: none
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
            /*return '-';*/
	    }
	    return ' ';
	}
	char norm()
	{
		printf("\033[0m");
		if(board[n][l]->player==1)
		{
			/*return '-';*/
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
Ask to the current player the coordinates for current move. With the function letToNum()
define 4 global variables, containin 4 numeric coordinates, from 0 to 7 for letter 
and number of start and finish. Also declare 2 puntators to pedina structure
represented by 2 positions.
Return error if letToNum() return an error.

Entering parameters:
    board: bidimensional array of puntator to struct, with all information about chessboard
           and position of all pawns
    pos1: puntator to starting position of pawn
    pos2: puntator to finishing position of pawn
    name1: player 1 name
    name2: player 2 name
    player: current player

Returned value:
    0 or 1: if coordinates are right or wrong
**************************************/
int getCoordinate(pedina *board[8][8], posizione *pos1, posizione *pos2, char *name, int player)
{
	char L[2], *giocatore, deb[51];
	int l1, l2, n1, n2;
/**************************************
letToNum()
Convert the charachter of the coordinate in a number (from 0 to 7), completing the job of getCoordinate().
Return error if any of the 4 coordinates isn't {1,2,3,4,5,6,7,8} or {a,b,c,d,e,f,g,h}

Entering parameters: none

Returned value:
    0 o 1: if coordinates are right or not
**************************************/
	int letToNum()
	{
		sprintf(deb, "l1 = |%d|(%c); l2 = |%d|(%c)", L[0], L[0], L[1], L[1]); debug(deb);
		int *ll[2], i;
		char deb[51];
		ll[0]=&l1; ll[1]=&l2;
		if(n1<1 || n1>8 || n2<1 || n2>8) { printf("Error: n coordinate out of range", n1, n2); return 0; }
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
				default: printf("Error: l coordinate out of range --> |%c|", L[i]);
				return 0;
			}
			sprintf(deb, "-[%d]-", *ll[i]); debug(deb);
		}
		debug("letToNum ok"); return 1;
	}
	printf("\n%s: Insert coordinate (LN LN):", name);
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
Control if move is regular.
At the beginning verified the only case in wich starting player and arriving player is the same: the castling.
Otherwise make a specific control depending from the starting pawn.
This and its sub-functions, change if they are doing the check control on a square, identified by checkScacco.
Note that the queen move is validated by rook AND bishop.
Return error if they control return error.

Entering parameters:
    pedine: puntator to pawn
    board: bidimensional array of puntator to struct, with all information about chessboard
           and position of all pawns
    pos1: puntator to starting position of pawn
    pos2: puntator to finishing position of pawn
    player: current player

Returned value:
    1: Break every control
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
	if((*pos2).loc->player == player && checkScacco==0)	/*Correct ONLY for castling*/
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
			printf("\nKing still under check\n");
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
Check pawn's move. It can have 3 different case.
Eating: if the pawn is moved diagonally.
Arriving square has to be an enemy pawn, except for a check control.
Move of 1 square: move up by 1 square. Arriving square has to be empty. Not valid for check control.
Move of 2 square: mvoe up by 2 square. Possible only at first step, arriving square has to be empty.
Not valid for check control.

Entering parameters:
    l1: starting coordinate 1
    n1: starting coordinate 2
    l2: arriving coordinate 1
    n2: arriving coordinate 2
    pos1: puntator to pawn's starting position
    pos2: puntator to pawn's arriving position
    board: bidimensional array of puntator to struct, with all information about chessboard
           and position of all pawns
    player: current player

Returned value:
    0
**************************************/
int checkPedone(int l1, int n1, int l2, int n2, posizione *pos1, posizione *pos2, pedina *board[8][8], int player, int checkScacco)
{
	int step, o_player;
	o_player=(player==0)?1:0;
	/*eating pawn*/
	if((abs(l2-l1)==1 && n2==n1+1 && (*pos1).loc->player==0) ||
		(abs(l2-l1)==1 && n2==n1-1 && (*pos1).loc->player==1))/*controlling final position*/
	{
		if (((*pos2).loc->player == o_player && checkScacco==0) || checkScacco==1)
		{debug("controllo pawn mangia"); return 1;}
	}
	/*pawn moving by 1*/
	if ((l2==l1 && n2==n1+1 && (*pos1).loc->player==0) ||
		(l2==l1 && n2==n1-1 && (*pos1).loc->player==1))/*controlling final position*/
	{
		if((*pos2).loc->tipo == 'V' && checkScacco==0)
		{
			if((*pos2).n == 0 || (*pos2).n == 7) {(*pos1).loc->tipo = promozione();}
			debug("controllo pawn muove 1");
			return 1;
		}
	}
	/*pawn moving by 2*/
	if (checkScacco==0 && ((l2==l1 && n2==n1+2 && (*pos1).loc->moved==0 && (*pos1).loc->player==0) ||
		(l2==l1 && n2==n1-2 && (*pos1).loc->moved==0 && (*pos1).loc->player==1)))/*controlling final position*/
	{
		step=(player==0)? n1+1 : n1-1;
		if((*board[step][l2]).tipo == 'V' && (*pos2).loc->tipo == 'V') {debug("controllo pawn muove 2"); return 1;}
	}
	debug("Controllo pawn fallito");
	return 0;
}
/**************************************
promozione()
Promote pawn if it arrive to the end of the checkboard.

Entering parameters: none

Returned value:
    ch: charachter indicating moving pawn
***************************************/
char promozione()
{
	char ch;
	printf("\nIn what do you want to promote your pawn?\n");
	while(1)
	{
		printf("\nP - C - A - D\n");
		ch = getch()-32;
		switch(ch)
		{
			case 'P': case 'C': case 'A': case 'D':
				return ch;
			default:
				debug("\nWrong charachter: re-enter\n");
		}
	}
}
/***********************************
checkTorre()
Verify rook move. Al variare di uno degli assi, l'altro deve restare invariato.

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
				debug("Controllo rook fallito");
				return 0;
			}
		}
		debug("controllo rook ok");
		return 1;
	}
	else
	{
		debug("Controllo rook fallito");
		return 0;
	}
	return 1;
}
/*******************************************
checkCavallo()
Verifica lo spostamento del knight. Uno degli assi deve variare di 2, mentre l'altro di 1.

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
		debug("controllo knight ok");
		return 1;
	}
	else
	{
		debug("Controllo knight fallito");
		return 0;
	}
}
/*********************************************
checkAlfiere()
Verifica lo spostamento dell'bishop. Entrambi gli assi devono variare della stessa quantità.

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
				debug("Controllo bishop fallito");
				return 0;
			}
		}
	}
	else
	{
		debug("Controllo bishop fallito");
		return 0;
	}
	debug("controllo bishop ok");
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
Controlla se è possibile eseguire l'castling

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
			debug("Conrollo castling fallito");
			debug("\nRE sotto scacco");
			return 0;
		}
		while (abs(step-l2) != 1)
		{
			step=((step-l2) < 0)? step+1 : step-1;
			if(board[n1][step]->tipo != 'V')
			{
				debug("Controllo castling fallito");
				debug("\nOstacolo tra RE e rook");
				return 0;
			}
		}
		if (scacco(lr, n1, o_player, board, pedine, player) || scacco(lt, n1, o_player, board, pedine, player))
			{
				debug("Controllo castling fallito");
				debug("\nPassaggio di RE sotto scacco");
				return 0;
			}
	}
	else
	{
		debug("Controllo castling fallito");
		return 0;
	}
	move(l2, n2, lt, n2, board, pedine);
	move(l1, n1, lr, n2, board, pedine);
	debug("controllo castling ok");
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
Count moves' number

Entering parameters:
    numero_celle: moves' number

Returned value:
    numero_celle: moves' number
****************************************/
int conteggio_celle(int numero_celle)
{
	numero_celle++;
	return numero_celle;
}
