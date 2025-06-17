#include <REG51F380.H>
#include <string.h>
#include "lcd.h"

// LCD adresses
#define LCD_ADDR_W 0x4E
#define LCD_ADDR_R 0x4F

// 0xFFFF - Frequency
#define RELOAD 34286     

char horas[6] = "12:00";
static volatile int segundos = 0;

sbit pb1 = P0^6;
sbit pb2 = P0^7;
sbit buzz = P1^2;


int index = 0;

void updateHora(void);
void relogio(void);
void init_device(void);
void timer_init(void);
void lcd_refresh(void);
void config_hora(void);
void delay(void);
char* config_alarm(void);
void alarm(void);

void relogio(void) {
		
	updateHora();
	
}	

void alarm (void) {

	while(1) {	
		
		buzz = 1;
	
		if(pb1 == 0) {
		
			buzz = 0;
			break;
		}
	}
}
	
char* config_alarm(void) {

	char alarm_hour[6];
	TR0 = 0;  // stop Timer 0
	
	strcpy(alarm_hour, horas);
	
	lcd1602Clear(LCD_ADDR_W);
	
	lcd1602SetCursor(LCD_ADDR_W, 5, 0);
	lcd1602WriteString(LCD_ADDR_W, alarm_hour);
	
	lcd1602SetCursor(LCD_ADDR_W, 3, 1);
	lcd1602WriteString(LCD_ADDR_W, "ALARM CONFIG");
	
	
	while(1) {
	
		if(pb1 == 0) {
			
				alarm_hour[1] += 1;
				
				if (alarm_hour[0] > '1' && alarm_hour[1] > '3') {
            alarm_hour[0] = alarm_hour[1] = alarm_hour[3] = alarm_hour[4] = '0';

				}

        if (alarm_hour[1] > '9' && alarm_hour[0] < '2' && alarm_hour[0] >= '0') {
            alarm_hour[1] = '0';
            alarm_hour[0]++;

        }
				
				lcd1602SetCursor(LCD_ADDR_W, 5, 0);
				lcd1602WriteString(LCD_ADDR_W, alarm_hour);
		}
			
		if(pb2 == 0) {
		
			delay();	
			
			lcd1602SetCursor(LCD_ADDR_W, 3, 1);
			lcd1602WriteString(LCD_ADDR_W, "MIN CONFIG ");
			
			break;
		}
	
	}
	
	while(1) {
		
		if(pb1 == 0) {
			
			alarm_hour[4] += 1;
			
			if (alarm_hour[4] > '9') {
            alarm_hour[4] = '0';
            alarm_hour[3]++;

				}

        if (alarm_hour[3] > '5') {
            alarm_hour[3] = '0';
            alarm_hour[1]++;
				}	
			
				lcd1602SetCursor(LCD_ADDR_W, 5, 0);
				lcd1602WriteString(LCD_ADDR_W, alarm_hour);
		
		}
	
		if(pb2 == 0) {
		
			delay();
			
			lcd1602SetCursor(LCD_ADDR_W, 3, 1);
			lcd1602WriteString(LCD_ADDR_W, "CONFIG OVER");
			
			break;
		}
	
		
	}
	
	segundos = 0;
	
	lcd1602Clear(LCD_ADDR_W);
	lcd_refresh();
	lcd1602SetCursor(LCD_ADDR_W, 1, 1);
	lcd1602WriteString(LCD_ADDR_W, "DIG CLOCK 8051");
	
	
	TR0 = 1;  // restart Timer 0
	
	return alarm_hour;
	
}

void delay(void) {

	// dummy delay
	
	int i, j;
	for (i = 0; i<150; i++) {
		for(j = 0; j<150; j++) {
		
		}
	}

}

void config_hora(void) {
	
	TR0 = 0;  // stop Timer 0
	
	lcd1602Clear(LCD_ADDR_W);
	
	lcd1602SetCursor(LCD_ADDR_W, 5, 0);
	lcd1602WriteString(LCD_ADDR_W, horas);
	
	lcd1602SetCursor(LCD_ADDR_W, 3, 1);
	lcd1602WriteString(LCD_ADDR_W, "HOUR CONFIG");
	
	
	
	while(1) {
	
		if(pb1 == 0) {
			
				horas[1] += 1;
				updateHora();
				lcd_refresh();	
		}
			
		if(pb2 == 0) {
		
			delay();	
			
			lcd1602SetCursor(LCD_ADDR_W, 3, 1);
			lcd1602WriteString(LCD_ADDR_W, "MIN CONFIG ");
			
			break;
		}
	
	}
	
	while(1) {
		
		if(pb1 == 0) {
			
			horas[4] += 1;
			updateHora();
			lcd_refresh();
		
		}
	
		if(pb2 == 0) {
		
			delay();
			
			lcd1602SetCursor(LCD_ADDR_W, 3, 1);
			lcd1602WriteString(LCD_ADDR_W, "CONFIG OVER");
			
			break;
		}
	
		
	}
	
	segundos = 0;
	
	lcd_refresh();
	lcd1602SetCursor(LCD_ADDR_W, 1, 1);
	lcd1602WriteString(LCD_ADDR_W, "DIG CLOCK 8051");
	
	
	TR0 = 1;  // restart Timer 0
	
}


void lcd_refresh(void) {

	lcd1602SetCursor(LCD_ADDR_W, 5, 0);
	lcd1602WriteString(LCD_ADDR_W, horas);

}

void updateHora(void) {
	
				if (segundos > 59) {
						segundos = 0;
            horas[4]++;

        }

        if (horas[4] > '9') {
            horas[4] = '0';
            horas[3]++;

				}

        if (horas[3] > '5') {
            horas[3] = '0';
            horas[1]++;

        }

        // Como o rel gio   24h, checar se as dezenas da hora s o 20 (nesse caso so podemos ter at  23:59:59 -> 00:00:00)
        if (horas[0] > '1' && horas[1] > '3') {
            horas[0] = horas[1] = horas[3] = horas[4] = horas[6] = horas[7] = '0';

				}

        if (horas[1] > '9' && horas[0] < '2' && horas[0] >= '0') {
            horas[1] = '0';
            horas[0]++;

        }
				
				lcd_refresh();
				
}

void timer_init(void) {
		
	CKCON |= 0x02;  // SYSCLK / 48 F = 31.250 KHz 
	TMOD |= 0x01;   // 16 bit mode
	
	ET0 = 1;
	EA = 1;
	
	TF0 = 0;
	
	TH0 = (RELOAD) >> 8;
	TL0 = (RELOAD);  
	
	TR0 = 1;
}

void timer0_isr() interrupt 1 {

	TH0 = (RELOAD) >> 8;
	TL0 = (RELOAD); 
	
	segundos++;
	TF0 = 0;
	
}

void init_device(void) {

	PCA0MD = 0x00;
	P0MDOUT = 0x04;
	P1MDOUT |= 0x04;
	FLSCL     = 0x90;
	XBR1      = 0x40;
	
	timer_init();
}

void main() {
	
	char* temp;		
	int rc;
	init_device();
	
	buzz = 0;
	rc = lcd1602Init(LCD_ADDR_W);
	lcd1602Clear(LCD_ADDR_W);
	lcd1602Control(LCD_ADDR_W, 1,0,0);
	
	if(rc)
		while(1);
	
		lcd1602SetCursor(LCD_ADDR_W, 5, 0);
		lcd1602WriteString(LCD_ADDR_W, horas);
		
		lcd1602SetCursor(LCD_ADDR_W, 1, 1);
		lcd1602WriteString(LCD_ADDR_W, "DIG CLOCK 8051");
		
		while(1) {
			relogio();
			
			if(pb1 == 0) {
			
				delay();
				config_hora();
			}
			
			
			if(pb2 == 0) {
			
				delay();
				temp = config_alarm();
				
			}
			
			if(strcmp(temp, horas) == 0) {
			
				alarm();
				strcpy(temp, "00:00");
			}
			
		}
	
}