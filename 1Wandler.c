/*
 * _1Wandler.c
 *
 * Created: 16.01.2019 16:35:02
 *  Author: Arno
 */ 

#define F_CPU 16000000

#include <avr/io.h>
#include <stdio.h>
#include <stdlib.h>
#include "lcd-routines.h"
#include <util/delay.h>
#include <string.h>

uint8_t i;						//Laufvariable Mittelwertbildung
double werte;					//ADC Ergebnis
double lin;					   //Begradigung der Hallsensorkennlienie
uint8_t endwert_mittelung = 10;//Anzahl Mittelwertbildung				
char buffer[20];				//Variable typ char für die Umwandlung

int main(void)
{
	 //ADC setup
	 PORTA = 0x00;		//Port A als input
   
	 ADMUX  = (1<<MUX0) | (1<<REFS0);							// Adresse from ADC1,
																// external reference voltage					     
	 ADCSRA = (1<<ADEN)  | (1<<ADPS2) | (1<<ADPS1) | (1<<ADPS0);// ADC enable, start conversion,
																// ADC prescaler 0-2 (factor=128)	
	 ADCSRA |= (1<<ADSC);									    //ADC Start 1 conversion
	 while ( ADCSRA & (1<<ADSC))
		{
		}								//wait for conversion
  
	  while (1)
	{
			for(i=0;i<endwert_mittelung;i++)// mehrfach wandeln
			{				
			ADCSRA |= (1<<ADSC);			// ADC start conversion
			while ( ADCSRA & (1<<ADSC))
				{
				}							// wait for conversion
						
			werte += ADC;					// ergebnisse aufaddieren
			}									    									    
		werte /= endwert_mittelung;//Mittelwert bilden
		werte *= 5.0;				//Anpassung an Uref = 5V					  
        werte /= 1024.0;			//Anzeige pro bit, hier 10 bit = 1024. 
									//wert entspricht U am Eingang ADC
		werte -= 2.5;				//Anpassung an Stromwandler ( 2,5V bei 0 A		
		
		//if (werte<2.0) { lin = 0.039;
		
		//}else if 
			//((werte<5.0) & (werte>2.0)){
				// lin = 0.040;
		//}else if 
				//(werte<10.0){
				 //lin = 0.047;
		//}else if
			  //(werte<20.0){
				  //lin = 0.046;
		//}else
		//{
		lin = 0.039;				// 40mV/A 	
		//}   
	   werte /= lin;
	   lcd_init();
	   lcd_setcursor(6,1);
	   lcd_string("A");
	   lcd_setcursor(1,1);
	   
	   //itoa(werte,buffer,10);		//(variable, Puffer in dem die
									// umgewandelte Variable steht,10 = dezimal)
	   
	   dtostrf(werte,5,2,buffer);//(variable, Gesamtstellenzahl incl. Punkt/Komma, 
								//Nachkommastellen, Puffer für umgewandelte Variable )
	   buffer[2]=',';			//[Stelle an der der Punkt gegen das Komma ausgetauscht wird]
	   lcd_string(buffer);
	   _delay_ms(200);
	   
	   
    }	 		       
}
