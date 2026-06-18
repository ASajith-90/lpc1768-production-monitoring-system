#include "header143.h" 
extern volatile uint32_t product_count; 
extern volatile uint32_t defect_count; 
extern volatile uint32_t rate; 
extern volatile uint32_t efficiency; 
int main(void) 
{
 lcd_init(); 
 led_init(); 
 buzzer_init(); 
 uart0_init(); 
 delay(500); 
 eint0_init(); 
 eint1_init(); 
 timer0_init(); 
 lcd_cmd(0x01); 
 while(1) 
 { 
 maintenance_alert(); 
 lcd_cmd(0x80); 
 lcd_string("CNT:"); 
 lcd_num(product_count); 
 lcd_string("DFT_CNT:"); 
 lcd_num(defect_count); 
 lcd_cmd(0xC0); 
 lcd_string("Rate:"); 
 lcd_num(rate); 
 lcd_string(" EFF:"); 
 lcd_num(efficiency); 
 uart0_send_data(); 
 delay(100); 
 } 
}