/*******************************
help()
Sezione help attivabile durante la partita in qualsiasi momento.
Consta smeplicemente di una serie di printf, che danno le informazioni 
base sul programma ai giocatori.

Parametri in ingresso: nessuno

Valore restituito: nessuno
*******************************/
void help()
{
    printf("Welcome to the help section of Chess. Speriamo di dissipare \n");
    printf("ogni tuo dubbio sull'utilizzo di questo programma!\n\n\n");
    printf("Partiamo innanzitutto con l'utilizzo tecnico del programma, che potrebbe non \n");
    printf("risultare subito chiarissimo.\n");
    printf("All'inizio di ogni turno ti viene mostrato in alto\n");
    printf("il menu: premendo una delle lettere mostrate si \"attiva\" la relativa opzione:\n\n");
    printf("Q = esci. Verra\' chiesto al giocatore se intende uscire dal gioco e, in caso di\n");
    printf("conferma, il programma verra\' chiuso.\n\n");
    printf("N = nuovo gioco. Verrà iniziata una nuova partita.\n\n");
    printf("M = muovi. Verra\' chiesto al giocatore l'inserimento delle coordinate, \n");
    printf("che devono essere ASSOLUTAMENTE inserite nel seguente formato: a2 a4.\n");
    printf("La prima coppia di caratteri identifica la casella di partenza, la seconda \n");
    printf("quella di arrivo.\n");
    printf("Nota che la scacchiera verra\' visualizzata solo dopo aver premuto M\n");
    printf("in modo che la tua mossa venga cronometrata correttamente\n\n");
    printf("S = dichiara stallo. La partita finisce in parita\'.\n\n\n");
    printf("                                                                 1 di 6");
    
        getch();
        clear();
    
    printf("Ora passiamo alle regole del gioco. Le spiegheremo qui in modo chiaro e\n");
    printf("conciso, per ogni approfondimento rimandiamo a:\n\n");
    printf("http://it.wikipedia.org/wiki/Scacchi#Descrizione_e_regolamento\n\n");
    printf("Il gioco si svolge su una scacchiera di 8 per 8 caselle, sulla quale\n");
    printf("si muovono i vari pezzi. Ognuno di questi, contraddistinto nel programma\n");
    printf("da una lettera diversa, si puo\' muovere in modi differenti. Se la mossa di uno\n");
    printf("dei tuoi pezzi termina sulla casella occupata da una pedina avversaria, questa\n");
    printf("si dice \"mangiata\" ed e\' considerata eliminata dal gioco.\n");
    printf("Lo scopo del gioco e\' quello di mangiare il re avversario.\n\n\n");
    printf("Descriviamo ora nel dettaglio i movimenti di ogni pedina.\n\n\n\n\n\n\n\n\n\n\n");
    printf("                                                                 2 di 6");
    
        getch();
        clear();
        
    printf("\nPEDONE: contraddistinto dalla lettera P, e\' il pezzo piu\' comune.\n");
    printf("E\' anche la pedina con piu\' regole, dato che si contraddistingue di 3 movimenti:\n");
    printf("- Ogni turno si puo\' muovere in avanti di una casella\n");
    printf("- Ogni pedone, al suo primo movimento, puo\' essere mosso di 2 caselle in avanti\n");
    printf("- Puo\' mangiare SOLO in diagonale-avanti, sempre muovendosi di una casella\n\n");
    printf("Il pedone possiede anche un'\"abilita\'\" particolare, la promozione.\n");
    printf("Infatti se un pedone raggiunge la parte opposta della scacchiera, il giocatore\n");
    printf("puo\' sostituirlo con un pezzo a sua scelta.\n\n");
    printf("TORRE: contraddistinta dalla lettera T, si muove di quante caselle vuole\n");
    printf("seguendo gli assi orizzontali e verticali della scacchiera. Mangia nello stesso modo.\n\n");
    printf("ALFIERE: contraddistinto dalla lettera A, si muove di quante caselle vuole\n");
    printf("seguendo gli assi diagonali della scacchiera. Mangia nello stesso modo.\n\n");
    printf("REGINA: contraddistinta dalla lettera D, e\' il pezzo piu\' potente.\n");
    printf("Si muove di quante caselle vuole seguendo sia gli assi orizzontali che\n");
    printf("quelli diagonali della scacchiera. Mangia allo stesso modo.\n\n\n");
    printf("                                                                 3 di 6");
    
    
        getch();
        clear();
    
    printf("CAVALLO: contraddistinto dalla lettera C, si muove seguendo un\n");
    printf("percorso ad elle, come mostrato in figura.\n");
    printf("La C rappresenta la posizione di partenza del cavallo, le x sono i\n");
    printf("possibili punti di arrivo.\n");
    printf("______________________\n");
    printf("|__|__|__|__|__|__|__|\n");
    printf("|__|__|_x|__|_x|__|__|\n");
    printf("|__|_x|__|__|__|_x|__|\n");
    printf("|__|__|__|C_|__|__|__|\n");
    printf("|__|_x|__|__|__|_x|__|\n");
    printf("|__|__|_x|__|_x|__|__|\n");
    printf("|__|__|__|__|__|__|__|\n\n");
    printf("La mangiata avviene nello stesso modo SOLO SE la pedina avversaria si trova\n");
    printf("sulla casella di arrivo del percorso (la x, per intenderci). Inoltre\n");
    printf("il cavallo e\' l'unica pedina che puo\' scavalcare le altre, amiche\n");
    printf("e nemiche.\n\n");
    printf("RE: contraddistinto dalla lettera R, si muove di una sola casella alla\n");
    printf("volta in una direzione scelta. Possiede anche una mossa particolare,\n");
    printf("l'arrocco: se dall'inizio della partita il re e una delle due torri\n");
    printf("non sono state mosse, il re si puo\' spostare di 2 caselle verso la torre\n");
    printf("(l'asse tra i due deve quindi essere libero) e la torre puo\' scavalcarlo,in\n");
    printf("modo da proteggerlo.\n");
    printf("                                                                 4 di 6");
    
        getch();
        clear();
        
    printf("Esempio:\n\n");
    printf("Prima dell'arrocco:\n");
    printf("______________________\n");
    printf("|__|__|__|__|__|__|__|\n");
    printf("|_T|__|__|_R|__|__|__|\n\n");
    printf("Dopo l'arrocco:\n");
    printf("______________________\n");
    printf("|__|__|__|__|__|__|__|\n");
    printf("|__|_R|_T|__|__|__|__|\n\n\n");
    printf("Ultima regola e\' quella dello SCACCO. Ogni volta\n");
    printf("che una delle tue pedine si trovera\' in posizione tale\n");
    printf("da poter mangiare il re avversario, dovrai dichiarare SCACCO,\n");
    printf("in modo da permettere all'avversario di correre ai ripari. Quando\n");
    printf("invece riesci a bloccare l'avversario in modo tale che non possa\n");
    printf("eseguire nessuna mossa senza che il suo re, al turno sucessivo,\n");
    printf("venga mangiato, puoi dichiarare SCACCO MATTO e porre fine alla partita.\n");
    printf("(non ti preoccupare, il gioco riconosce automaticamente la situazione\n");
    printf("di scacco matto, ponendo fine alla partita, e anche quella di scacco,\n");
    printf("obbligandoti a togliere il re da questa situazione.\n\n");
    printf("                                                                 5 di 6");
    
        getch();
        clear();
        
    printf("Ultime note di carattere tecnico: a fine partita ti verranno mostrate\n");
    printf("le dieci mosse piu\' veloci della partita. Inoltre la lista di tutte le\n");
    printf("mosse con relativa durata verra\' salvata a fine partita in un file\n");
    printf("chiamato \"mosse.txt\", nella stessa directory del gioco. Se inizierai una\n");
    printf("nuova partita, questo file verra\' irrimediabilmente sovrascritto.\n\n");
    printf("Su piattafroma Linux il gioco ha implementato anche una funzione di\n");
    printf("Salvataggio e caricamento. Dando un nome al file di salvataggio su di\n");
    printf("esso verranno salvate tutte le informazioni relative alla partita\n");
    printf("in corso, che potrà quindi essere ripresa in seguito.\n\n");
    printf("Speriamo che questa sezione help ti sia stata utile e ti auguriamo\n");
    printf("buona partita. Premendo un tasto qualsiasi tornerai alla schermata di gioco\n");
    printf("esattamente dove l'avevi lasciata...\n\n\n\n\n\n\n\n\n\n\n");
    printf("                                                                 6 di 6");
    
        getch();
        clear();
    
    
    
    
    
        return;
}    

