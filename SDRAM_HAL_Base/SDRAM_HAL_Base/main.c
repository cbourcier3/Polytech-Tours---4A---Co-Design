
#include <stdio.h>
#include "system.h"
#include "altera_avalon_pio_regs.h"
#include <io.h>
//les readkeyX correspondent à la lecture des états des différents boutons poussoirs
#define readkey0 (volatile unsigned short)(IORD_ALTERA_AVALON_PIO_DATA(BUTTON_BASE)&0b0001)
#define readkey1 (volatile unsigned short)(IORD_ALTERA_AVALON_PIO_DATA(BUTTON_BASE)&0b0010)
#define readkey2 (volatile unsigned short)(IORD_ALTERA_AVALON_PIO_DATA(BUTTON_BASE)&0b0100)
#define readkey3 (volatile unsigned short)(IORD_ALTERA_AVALON_PIO_DATA(BUTTON_BASE)&0b1000)

int oe = 0;
//Active la sortie voie avec le signal pwm en sortie
void activerSortie(unsigned int voie, unsigned int pwm)
{
	unsigned long nVoie = 0;
	nVoie = 0b1 << voie;
	oe = oe|nVoie;
	printf("from function : %d / %d = %d // oe = %d \n", nVoie, voie , pwm, oe);
	IOWR_ALTERA_AVALON_PIO_DATA(NLATCH_PWM_BASE,nVoie);
	IOWR_ALTERA_AVALON_PIO_DATA(OE_PWM_BASE,oe);
	IOWR_ALTERA_AVALON_PIO_DATA(TON_PWM_BASE, pwm);
	printf("TON : %d OE : %d NLATCH : %d\n",	IORD_ALTERA_AVALON_PIO_DATA(TON_PWM_BASE),IORD_ALTERA_AVALON_PIO_DATA(OE_PWM_BASE),IORD_ALTERA_AVALON_PIO_DATA(NLATCH_PWM_BASE));
	IOWR_ALTERA_AVALON_PIO_DATA(NLATCH_PWM_BASE,0);
}
//Desactive la sortie sélectionnée
void desactiverSortie(unsigned int voie)
{
	unsigned long nVoie = 0;
	nVoie = 0b1 << voie;
	oe = oe - nVoie;
	IOWR_ALTERA_AVALON_PIO_DATA(NLATCH_PWM_BASE,nVoie);
	IOWR_ALTERA_AVALON_PIO_DATA(OE_PWM_BASE,oe);
	IOWR_ALTERA_AVALON_PIO_DATA(TON_PWM_BASE, 0);
	IOWR_ALTERA_AVALON_PIO_DATA(NLATCH_PWM_BASE,0);
}

int main()
{
	// initialisation des variables
	unsigned int sortie = 0; 	//correspond aux LED affichées
	unsigned int curseur= 1; 	//correspond à la position de la LED de sélection, au "curseur"
	char conf = 0;			 	//Mode de configuration : 	0 correspond au menu de sélection et d'activation des sorties
								//							1 correspond au menu de changement de PWM
	unsigned int pwm[24] = {0};	//Niveaux de PWM
	unsigned char selPwm = 1;	//PWM en cours de configuration
	FILE* LCD;					//Ecran LCD
	LCD = fopen("/dev/lcd_16207_0", "w");
	fprintf(LCD, "\x1B[2J");	//Remise à zéro LCD
	fprintf(LCD, "Bienvenue sur\nPWMGenerator2400\n");
	while(1)
	{
	  if(readkey0==0)// on vérifie l'état des boutons : 0 signifie qu'il sont enclenchés, 1 qu'ils sont relachés
		  {
		  if (conf == 0)// on agit selon le mode de configuration dans lequel nous sommes
		  {
			  if ((sortie&curseur)!=0)
			  {
				  sortie = sortie - curseur;
				  fprintf(LCD, "\x1B[2J");
				  fprintf(LCD, "Voie %d OFF\n",selPwm);
				  desactiverSortie(selPwm-1);
				  printf("Voie %d désactivée\n",selPwm);
			  }
			  else {
				  sortie = sortie + curseur;
				  fprintf(LCD, "\x1B[2J");
				  fprintf(LCD, "Voie %d ON\nDuty Cycle %d\n",selPwm,pwm[selPwm-1] );
				  activerSortie(selPwm-1,pwm[selPwm-1]);
				  printf("Voie %d activée à %d/1024\n",selPwm,pwm[selPwm-1]);
			  }
		  }else
			  {
			  if (pwm[selPwm-1] < 1024)
				  {
				  if(pwm[selPwm-1]%10!=9)
					  pwm[selPwm-1] += 1;
				  else pwm[selPwm-1] = pwm[selPwm-1] - 9;
			  }else pwm[selPwm-1] = 0;
			  printf("%d = %d\n",selPwm, pwm[selPwm-1]);
			  fprintf(LCD, "\x1B[2J");
			  fprintf(LCD, "Voie %d \nDuty Cycle %d\n",selPwm,pwm[selPwm-1] );
			  }
		  while (readkey0 == 0);
		  }
	  if(readkey1==0)
		  {
		  if(conf == 0)
		  {
			  if (curseur == 0x000001)
				  curseur = 0x800000;
			  else curseur = curseur /2;
			  if (selPwm == 1)
				  selPwm = 24;
			  else selPwm+= -1;
		  }else if (pwm[selPwm-1] < 1015)
		  {
			  if((pwm[selPwm-1]/10)%10!=9)
				  pwm[selPwm-1] += 10;
			  else pwm[selPwm-1] = pwm[selPwm-1] - 90;
		  }else if (pwm[selPwm-1]== 1024)
			  pwm[selPwm-1] = 0;
		  else pwm[selPwm-1] = 1024;
		  printf("%d = %d\n",selPwm, pwm[selPwm-1]);
		  fprintf(LCD, "\x1B[2J");
		  fprintf(LCD, "Voie %d \nDuty Cycle %d\n",selPwm,pwm[selPwm-1] );
		  while (readkey1 == 0);
		  }
	  if(readkey2==0)
		  {
		  if (conf == 0)
		  {
			  if (curseur == 0x800000)
				  curseur = 0x000001;
			  else curseur = curseur * 2;
			  if (selPwm == 24)
				  selPwm = 1;
			  else selPwm+= 1;
		  }else if (pwm[selPwm-1] < 925)
		  {
			  if((pwm[selPwm-1]/100)%11!=10)
				  pwm[selPwm-1] += 100;
			  else pwm[selPwm-1] = pwm[selPwm-1] - 1000;
		  }else if (pwm[selPwm-1]== 1024)
			  pwm[selPwm-1] = 0;
		  else pwm[selPwm-1] = 1024;
		  printf("%d = %d\n",selPwm, pwm[selPwm-1]);
		  fprintf(LCD, "\x1B[2J");
		  fprintf(LCD, "Voie %d \nDuty Cycle %d\n",selPwm,pwm[selPwm-1] );
		  while (readkey2 == 0);
		  }
	  if(readkey3==0)
		  {
		  if (conf == 0)
		  {
			  conf =1;
			  fprintf(LCD, "\x1B[2J");
			  fprintf(LCD, "In conf voie %d\n",selPwm);
			  printf("Entrée dans la configuration du port n° %d\n", selPwm);
		  }
		  else {
			  conf = 0;
			  fprintf(LCD, "\x1B[2J");
			  fprintf(LCD, "Out conf voie %d\n",selPwm);
			  printf("Sortie de la configuration du port n° %d\n", selPwm);
		  }
		  while (readkey3 == 0);
		  }
	  IOWR_ALTERA_AVALON_PIO_DATA(LED_BASE,sortie|curseur);//appliquer les modifications au niveau des LEDs
	}
}
