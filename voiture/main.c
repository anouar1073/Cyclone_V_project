#include"system.h"
#include <stdio.h>
#include "sys/alt_irq.h"
#include "altera_avalon_pio_regs.h"
#include "sys/alt_alarm.h"
#define ON_1 0x1
#define OFF_1 0xFFFFFFFE
#define ON_8 0x000000FF
#define OFF_8 0xFFFFFF00
#define BIT1 0x1
#define BIT2 0x2
#define BIT3 0x4
#define BIT4 0x8
#define ONE 0xFFFFFFF9
#define TWO 0xFFFFFFA4
#define THREE 0xFFFFFFB0

enum etat{initialisation,fonctionnement,urgence};
int * edge_capture_ptr;
int * edge_capture_ptr2;
// variables globales
volatile int urgence_var;
volatile int problem;
int alternance_1=1;
int alternance_2=1;
int alternance_hex_1=0;
int alternance_hex_2=0;
int alternance_hex_3=0;
int alternance_hex_4=0;
int alternance_sw_3=0;
int alternance_sw_4=0;
int KEY_value;
	int byte_value_1;
	int byte_value_2;
	int byte_value_3;
	int byte_value_4;
	int byte_value_4_1;
	int hex_value;
unsigned int tps;

alt_u32 alarm_1_callback (void* context)
{
int switch1,switch2;
int led_val;
int value_bit_1;
int value_bit_2;
	
	switch1 = IORD_ALTERA_AVALON_PIO_DATA(SWITCHES_BASE)& BIT1;
	switch2 = IORD_ALTERA_AVALON_PIO_DATA(SWITCHES_BASE)& BIT2;
	led_val = IORD_ALTERA_AVALON_PIO_DATA(LEDS_BASE);
	if(switch1 ==0x1){
		if(alternance_1==0)	{
			value_bit_1 = led_val | ON_1;
			IOWR_ALTERA_AVALON_PIO_DATA(LEDS_BASE ,value_bit_1);
			alternance_1=1;
		} else {
			value_bit_1= led_val & OFF_1;
			IOWR_ALTERA_AVALON_PIO_DATA(LEDS_BASE ,value_bit_1);
			alternance_1=0;
		}
	}
	if(switch2==0x2){
		if(alternance_2==0){
			value_bit_2 = led_val | (ON_1 <<1);
			IOWR_ALTERA_AVALON_PIO_DATA(LEDS_BASE ,value_bit_2);
			alternance_2 =1;
		} else {
			value_bit_2 = led_val & (OFF_1<<1);
			IOWR_ALTERA_AVALON_PIO_DATA(LEDS_BASE ,value_bit_2);
			alternance_2=0;
		}
	}
	
	return tps/3;
}

void position(void) {

	int SW_value= IORD_ALTERA_AVALON_PIO_DATA(SWITCHES_BASE);
  	int SW2= ((SW_value)&0x4)!=0;
 	int SW3= ((SW_value)&0x8)!=0;

  	int led_value = IORD_ALTERA_AVALON_PIO_DATA(LEDS_BASE);

	if(SW2){
    	led_value=led_value|0xC; //Allumer LED2 et LED3
    	IOWR_ALTERA_AVALON_PIO_DATA(LEDS_BASE,led_value);
  	}
  	else {
    	led_value=led_value&0x7F3; //Eteindre LED2 et LED3
    	IOWR_ALTERA_AVALON_PIO_DATA(  LEDS_BASE,led_value);
  	}

  	if(SW3){
    	led_value=led_value|0x30; //Allumer LED4 et LED5
    	IOWR_ALTERA_AVALON_PIO_DATA(LEDS_BASE,led_value);
  	}
  	else{
    	led_value=led_value&0x7CF; //Eteindre LED4 et LED5
    	IOWR_ALTERA_AVALON_PIO_DATA(LEDS_BASE,led_value);
  	}

  	return;
}

void pushbutton_ISR(unsigned int id)
{
	
	//read the pushbutton interrupt register 
	KEY_value= IORD_ALTERA_AVALON_PIO_EDGE_CAP(BOUTONS_BASE);
	IOWR_ALTERA_AVALON_PIO_EDGE_CAP(BOUTONS_BASE, KEY_value&0xF); // clear the interrupt

	int KEY1 = KEY_value&BIT1;
	int KEY2 = (KEY_value&BIT2)>>1;
	int KEY3 = (KEY_value&BIT3)>>2;
	int KEY4 = (KEY_value&BIT4)>>3;

	hex_value = IORD_ALTERA_AVALON_PIO_DATA(HEX_0_BASE);
	if(KEY1==0x1){
		// write 1 in hex0 for fault 1
		if (alternance_hex_1==0) {
				byte_value_1= hex_value | ON_8;
				IOWR_ALTERA_AVALON_PIO_DATA(HEX_0_BASE ,byte_value_1);
				byte_value_1 = hex_value & ONE;
			IOWR_ALTERA_AVALON_PIO_DATA(HEX_0_BASE ,byte_value_1);
			alternance_hex_1=1;
			}
		else {
			byte_value_1= hex_value | ON_8;
			//byte_value_1= hex_value & OFF_8;
			IOWR_ALTERA_AVALON_PIO_DATA(HEX_0_BASE ,byte_value_1);
			alternance_hex_1=0;
		}
	}
	if(KEY2==0x1){
		// write 1 in hex0 for fault 1
		if (alternance_hex_2==0) {
			byte_value_2 = hex_value | ON_8;
			IOWR_ALTERA_AVALON_PIO_DATA(HEX_0_BASE ,byte_value_2);
			byte_value_2 = hex_value & TWO;
			IOWR_ALTERA_AVALON_PIO_DATA(HEX_0_BASE ,byte_value_2);
			alternance_hex_2=1;
			}
		else {
			byte_value_2 = hex_value | ON_8;
			IOWR_ALTERA_AVALON_PIO_DATA(HEX_0_BASE ,byte_value_2);
			alternance_hex_2=0;
		}
	}
	if(KEY3==0x1){
		// write 1 in hex0 for fault 1
		if (alternance_hex_3==0) {
				byte_value_3 = hex_value | ON_8;
			IOWR_ALTERA_AVALON_PIO_DATA(HEX_0_BASE ,byte_value_3);
				byte_value_3 = hex_value & THREE;
			IOWR_ALTERA_AVALON_PIO_DATA(HEX_0_BASE ,byte_value_3);
			alternance_hex_3=1;
			}
		else {
			byte_value_3 = hex_value | ON_8;
			IOWR_ALTERA_AVALON_PIO_DATA(HEX_0_BASE ,byte_value_3);
			alternance_hex_3=0;
		}
	}
	if(KEY4==0x1){
		// write 1 in hex0 for fault 1
		if (urgence_var==0) {
				urgence_var =1;
				printf("%d\n",urgence_var);
		}
		else {
				urgence_var=0;
				printf("%d\n",urgence_var);
	}
	}
		
	return;
	}
	
//void switch_ISR (unsigned int id);
int main() 
{
	// alarm setup
		alt_alarm alarm_1;
		tps = alt_ticks_per_second();
		alt_alarm_start ( &alarm_1,tps, alarm_1_callback, NULL );
	
	/* enable interrupts for all pushbuttons */
		IOWR_ALTERA_AVALON_PIO_IRQ_MASK(BOUTONS_BASE, 0xF);
    
		/* Reset the edge capture register. */
		IOWR_ALTERA_AVALON_PIO_EDGE_CAP(BOUTONS_BASE, 0xF);  
		alt_irq_register(BOUTONS_IRQ, edge_capture_ptr, (void *) pushbutton_ISR);
		//alt_irq_enable(BOUTONS_IRQ);


  	
	/* Reset the edge capture register. */
	

	//alt_irq_register (1, NULL, (void *) pushbutton_ISR_2);
	// initialisation du 7 segment
	
	int etat =initialisation;
	while(1) {
	switch(etat) {
	case initialisation:
			etat=fonctionnement;
	break;
	case fonctionnement:
		position();
		IOWR_ALTERA_AVALON_PIO_IRQ_MASK(BOUTONS_BASE, 0xF);
		if( urgence_var ) {
				/* disable interrupts for all pushbuttons exept emergency button*/
				//alt_irq_disable(BOUTONS_IRQ);
				IOWR_ALTERA_AVALON_PIO_IRQ_MASK(BOUTONS_BASE, 0x8);
				etat=urgence;
				alt_alarm_stop(&alarm_1);	
			}	
	break;
	case urgence:
			
			//byte_value_4 = hex_value | 0x20;
			IOWR_ALTERA_AVALON_PIO_DATA(HEX_0_BASE ,0x3F);
			if(urgence_var==0){
					alt_alarm_start ( &alarm_1,tps, alarm_1_callback, NULL );
				etat=initialisation;
			}
		// do emergency procedure
	break;
	}
	}
 return 0;
}
