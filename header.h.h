#include <LPC17xx.h> 
#include <stdint.h>
 #define p0 LPC_GPIO0 
 #define p1 LPC_GPIO1 
 #define p2 LPC_GPIO2
  /* LCD */ 
  #define RS (1<<10) 
  #define EN (1<<11) 
  #define DATA (0xFF<<15) 
  /* 8 LEDs */ 
  #define LED1 (1<<19) 
  #define LED2 (1<<20) 
  #define LED3 (1<<21) 
  #define LED4 (1<<22) 
  #define LED5 (1<<23) 
  #define LED6 (1<<24) 
  #define LED7 (1<<25) 
  #define LED8 (1<<26) 
  #define ALL_LEDS (LED1|LED2|LED3|LED4|LED5|LED6|LED7|LED8) 
  /* Buzzer */ 
  #define BUZZER (1<<27) 
  /* Functions */ 
  void delay(uint16_t); 
  void lcd_init(void); 
  void lcd_cmd(uint8_t); 
  void lcd_data(uint8_t); 
  void lcd_string(char *); 
  void lcd_num(uint32_t); 
  void led_init(void); 
  void buzzer_init(void); 
  void eint0_init(void); 
  void eint1_init(void); 
  void timer0_init(void); 
  void uart0_init(void); 
  void uart0_send_data(void); 
  void maintenance_alert(void);