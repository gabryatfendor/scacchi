/*******************************
getch()
Riproduce l'effetto dell funzione getch()
su piattaforma linux

Parametri in ingresso: nessuno

Valore restituito: nessuno
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
Salva tutti i dati delle mosse eseguite, prendendole dalla lista,
e le salva in un file binario, in modo da renderne possibile il caricamento
in un momento successivo

Parametri in ingresso:
    pedine: Le strutture contenenti i dati di tutte le pedine
    board: array bidimensionale di puntatori a struttura, con tutte le informazioni sulla scacchiera
           e la posizione delle varie pedine
    name1: nome giocatore 1
    name 2: nome giocatore 2
    player: giocatore attuale
    numero_celle: numero di mosse eseguite sinora
    testa: puntatore alla testa della lista
    
Valore restituito: nessuno
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
	printf("~~~SALVA PARTITA~~~\n");
	printf("Inserire il nome da dare al file. Assicurati che non sia già"
			"presente o verrà sovrascritto!\n");
	scanf("%s%*c", fileName);
	//salvo player
	saveStruct.player = player;
	//salvo numero_celle
	saveStruct.numero_celle = numero_celle;
	//salvo i nomi dei giocatori
	for(i=0; i<31; i++) { saveStruct.name1[i] = name1[i]; }
	for(i=0; i<31; i++) { saveStruct.name2[i] = name2[i]; }
	//salvo board
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
	//raggiungo l'ultimo elemento della lista
	tmpUltimo = testa;
	while(tmpUltimo->prev!=NULL) { tmpUltimo = tmpUltimo->prev; }
	/*----Ora salvo i dati nel file----*/
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
Carica i dati di una partita precedente, prendendoli da
un file precedentemente salvato.

Parametri in ingresso:
    pedine: Le strutture contenenti i dati di tutte le pedine
    board: array bidimensionale di puntatori a struttura, con tutte le informazioni sulla scacchiera
           e la posizione delle varie pedine
    name1: nome giocatore 1
    name 2: nome giocatore 2
    player: giocatore attuale
    numero_celle: numero di mosse eseguite sinora
    
Valore restituito:
    testa: la testa della lista
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
	printf("~~~CARICA PARTITA~~~\n");
	do
	{
		printf("Indica il nome del file da caricare, deve essere nella cartella"
				"del gioco\n");
		scanf("%s%*c", fileName);
		saveFile = fopen(fileName, "r");
		if(!saveFile)
		{
			printf("C'è stato un errore, sicuro di aver indicato "
					"il file giusto?\n");
		}
	} while(!saveFile);
	fread(&saveStruct, sizeof(salvataggio), 1, saveFile);
	//carico il giocatore di turno
	*player = saveStruct.player;
	//carico numero_celle
	*numero_celle = saveStruct.numero_celle;
	//carico il nome dei 2 giocatori
	for(i=0; i<31; i++) { name1[i] = saveStruct.name1[i]; }
	for(i=0; i<31; i++) { name2[i] = saveStruct.name2[i]; }
	//collego board con le pedine
	for(n=0; n<8; n++)
	{
		for(l=0; l<8; l++)
		{
			p = saveStruct.board[n][l];
			board[n][l] = &pedine[p];
		}
	}
	//creo la lista dall'array
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
	printf("Caricamento eseguito, ora i giocatori sono:\n%s e %s\n",name1, name2);
	printf("È il turno di %s\n", name);
	printf("[premere un tasto per proseguire]");
	getch();
	return testa;
}
