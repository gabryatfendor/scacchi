/*******************************
getch()
Reproduce effect of getch() function
on Linux.

Entering Parameters: none

Returned value: none
*******************************/
int getch(void)
{
	struct termios oldt, newt;
	int ch;
	tcgetattr(STDIN_FILENO, &oldt);
	newt = oldt;
	newt.c_lflag &= ~( ICANON | ECHO);
	tcsetattr( STDIN_FILENO, TCSANOW, &newt );
	ch = getchar();
	tcsetattr( STDIN_FILENO, TCSANOW, &oldt );
return ch;
}

/**************************************
save()
Save the data of every moves, picking them from the list,
and save them in a binary file, so that you can load them later

Entering Parameters:
    pedine: The structs containing all chess data
    board: bidimensional array of struct puntators, with all the information
           about the chessboard and the position of every chess
    name1: player 1 name's
    name 2: player 2 name's
    player: current player
    numero_celle: numebr of moves done up to this point
    testa: puntator to the head of the list
    
Returned value: none
*****************************************/

void save(pedina *pedine, pedina *board[8][8], char *name1, char *name2, int player, int numero_celle, struct mossa *testa)
{
	struct mossa *tmp, *tmpUltimo, *mosseArray;
	salvataggio saveStruct;
	FILE *saveFile;
	char fileName[101], deb[51];
	int n, l, p, i;
	mosseArray=(struct mossa*)malloc(sizeof(struct mossa)*numero_celle);
	clear();
	printf("~~~SAVE GAME~~~\n");
	printf("Insert file name. Be sure it doesn't already"
			"exist, or it will be overwrited!\n");
	scanf("%s%*c", fileName);
	/*saving player*/
	saveStruct.player = player;
	/*saving numero_celle*/
	saveStruct.numero_celle = numero_celle;
	/*saving players names*/
	for(i=0; i<31; i++) { saveStruct.name1[i] = name1[i]; }
	for(i=0; i<31; i++) { saveStruct.name2[i] = name2[i]; }
	/*saving board*/
	for(n=0; n<8; n++)
	{
		for(l=0; l<8; l++)
		{
			for(p=0; p<33; p++)
			{
				if(board[n][l] == pedine+p)
				{
					sprintf(deb, "(%d-%d)%c --> pedine[%d]", l, n, board[n][l]->tipo, p); debug(deb);
					saveStruct.board[n][l] = p;
					p=33;
				}
			}
		}
	}
	/*reaching the last element of the list*/
	tmpUltimo = testa;
	while(tmpUltimo->prev!=NULL) { tmpUltimo = tmpUltimo->prev; }
	/*----Now I save data in the file----*/
	saveFile = fopen(fileName, "w");
	fwrite(&saveStruct, sizeof(salvataggio), 1, saveFile);
	tmp = tmpUltimo;
	for(i=0; i<numero_celle; i++)
	{
		sprintf(deb, "giocatore: %s\n", (tmp->player==0)?name1:name2); debug(deb);
		sprintf(deb, "mossa: %s\n", tmp->coordinate); debug(deb);
		sprintf(deb, "durata: %.2lf secs\n", tmp->durata_mossa); debug(deb);
		fwrite(tmp, sizeof(struct mossa), 1, saveFile);
		tmp = tmp->next;
	}
	fclose(saveFile);
}
/************************************
load()
Load the data of a previous game, taking them from
a file previously saved.

Entering parameters:
    pedine: The structs containing all chess data
    board: bidimensional array of struct puntators, with all the information
           about the chessboard and the position of every chess
    name1: player 1 name's
    name 2: player 2 name's
    player: current player
    numero_celle: numebr of moves done up to this point
    
Returned value:
    testa: the head of the list
*************************************/
struct mossa *load(pedina *pedine, pedina *board[8][8], char *name1, char *name2, char *name, int *player, int *numero_celle)
{
	FILE *saveFile;
	char fileName[51], deb[51];
	salvataggio saveStruct;
	struct mossa *mossaTmp, *tmp1, *tmp2, *testa;
	testa = NULL;
	int n, l, p, i;
	clear();
	printf("~~~LOAD GAME~~~\n");
	do
	{
		printf("Write the file-name to load, it must be in the"
				"game folder\n");
		scanf("%s%*c", fileName);
		saveFile = fopen(fileName, "r");
		if(!saveFile)
		{
			printf("Error, are you sure that the file "
					"exist?\n");
		}
	} while(!saveFile);
	fread(&saveStruct, sizeof(salvataggio), 1, saveFile);
	/*loading current player*/
	*player = saveStruct.player;
	/*loading numero_celle*/
	*numero_celle = saveStruct.numero_celle;
	/*loading player names*/
	for(i=0; i<31; i++) { name1[i] = saveStruct.name1[i]; }
	for(i=0; i<31; i++) { name2[i] = saveStruct.name2[i]; }
	/*linking board to pedine*/
	for(n=0; n<8; n++)
	{
		for(l=0; l<8; l++)
		{
			p = saveStruct.board[n][l];
			board[n][l] = &pedine[p];
		}
	}
	/*making list from array*/
	debug("Inizio a prendere la lista");
	for(i=0; i<*numero_celle; i++)
	{
		tmp1=(struct mossa*)malloc(sizeof(struct mossa));
		fread(tmp1, sizeof(struct mossa), 1, saveFile);
		sprintf(deb, "giocatore: %s\n", (tmp1->player==0)?name1:name2); debug(deb);
		sprintf(deb, "mossa: %s\n", tmp1->coordinate); debug(deb);
		sprintf(deb, "durata: %.2lf secs\n", tmp1->durata_mossa); debug(deb);
		tmp1->next=testa;
		tmp1->prev=NULL;
		if(testa!=NULL) { testa->prev=tmp1; }
		testa=tmp1;
	}
	debug("Lista caricata");
	name = (*player==0)?name1:name2;
	printf("Loading done, now players are:\n%s and %s\n",name1, name2);
	printf("It's %s turn!\n", name);
	printf("[Press a key to continue]");
	getch();
	return testa;
}
