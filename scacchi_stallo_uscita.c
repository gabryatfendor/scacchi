/****************************************
uscita()
Schermata di conferma per l'uscita dal gioco

Parametri in ingresso: nessuno

Valore restituito: nessuno
*****************************************/
void uscita()
{
    char esci;
		        printf("\nSei sicuro di voler uscire? Premi y per uscire, n per continuare a giocare!\n");
			    esci=getch();
			    if(esci=='y')
			    {
			    printf("\nGrazie per aver giocato! Buona giornata.");
			    getch();
				exit(0);
                }				
				if(esci=='n')
				return;
				else
				{
                    printf("\nScelta non consentita, premere y o n\n");
                    uscita();
                }    
}				
/************************************
stallo()
Schermata di conferma per la dichiarazione di stallo.

Parametri in ingresso: nessuno

Valore restituito: nessuno
************************************/
void stallo()
{
                char pari;
                printf("\nDichiarando stallo la partita finira\' in parita\'. Premere y per confermare\nn per annullare.\n");
                pari=getch();
                if(pari=='y')
                { //stampa le mosse piu' veloci ed esci
                    exit(0);
                }
                if(pari=='n')
                return;
                else
                {
                    printf("\nScelta non consentita, premere y o n\n");
                    stallo();
                }
}    

