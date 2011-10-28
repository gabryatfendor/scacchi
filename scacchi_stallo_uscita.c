/****************************************
uscita()
Confermation screen to quit from the game

Entering parameters: none

Returned value: none
*****************************************/
void uscita()
{
    char esci;
		        printf("\nAre you quitting? Are you sure? Press y to quit, n to go back to the game!\n");
			    esci=getch();
			    if(esci=='y')
			    {
			    printf("\nThanks for playing! Have a nice day.");
			    getch();
				exit(0);
                }				
				if(esci=='n')
				return;
				else
				{
                    printf("\nNo, you have to press y o n\n");
                    uscita();
                }    
}				
/************************************
stallo()
Confermation screen for stall.

Entering parameters: none

Returned value: none
************************************/
void stallo()
{
                char pari;
                printf("\nDeclaring stall the game will finish null. Press y to confirm\nn to return to game.\n");
                pari=getch();
                if(pari=='y')
                { 
                    //dichiaroMatto(testa,puntatore_ad_array_struct,numero_celle, name1, name2);
                    exit(0);
                }
                if(pari=='n')
                return;
                else
                {
                    printf("\nNo, you have to press y o n\n");
                    stallo();
                }
}    

