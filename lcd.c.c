#include "header143.h" 
/* Global Variables */ 
volatile uint32_t product_count = 0; 
volatile uint32_t defect_count = 0; 
volatile uint32_t prev_count = 0; 
volatile uint32_t rate = 0; 
volatile uint32_t efficiency = 0; 
/* Delay */ 
void delay(uint16_t k) 
{
 uint32_t i,j; 
 for(i=0;i<k;i++) 
 {
  for(j=0;j<3000;j++); 
  } 
 } 
/* LCD */ 
void enable(void) 
{
 p0->FIOSET = EN; 
 delay(2); 
 p0->FIOCLR = EN; 
} 
void lcd_cmd(uint8_t cmd) 
{
 p0->FIOCLR = RS; 
 p0->FIOCLR = DATA; 
 p0->FIOSET = ((uint32_t)cmd << 15); 
 enable(); 
} 
void lcd_data(uint8_t d) 
{
 p0->FIOSET = RS; 
 p0->FIOCLR = DATA; 
 p0->FIOSET = ((uint32_t)d << 15); 
 enable(); 
}
void lcd_init(void) 
{
 p0->FIODIR |= RS | EN | DATA; 
 lcd_cmd(0x38); 
 lcd_cmd(0x0E); 
 lcd_cmd(0x01); 
} 
void lcd_string(char *s) 
{
 while(*s) 
 {
  lcd_data(*s++); 
 } 
} 
void lcd_num(uint32_t n) 
{
 char arr[10]; 
 int i = 0; 
 if(n == 0) 
 {
  lcd_data('0'); 
  return; 
 } 
 while(n) 
 {
  arr[i++] = (n%10)+'0'; n/=10; 
 } 
 while(i) 
 { 
 lcd_data(arr[--i]); 
 }
 } 
 /* LED */ 
void led_init(void) 
{
 p1->FIODIR |= ALL_LEDS; 
 p1->FIOCLR = ALL_LEDS; 
}
 /* Buzzer */ 
 void buzzer_init(void) 
 {
  p1->FIODIR |= BUZZER;
   p1->FIOCLR = BUZZER; 
 } 
/* Product Counter Interrupt */ 
void eint0_init(void) 
{
 LPC_PINCON->PINSEL4 &= ~(3<<20); 
 LPC_PINCON->PINSEL4 |= (1<<20); 
 LPC_SC->EXTMODE |= (1<<0); 
 LPC_SC->EXTPOLAR |= (1<<0); 
 LPC_SC->EXTINT = (1<<0);
 product_count = 0; 
 NVIC_EnableIRQ(EINT0_IRQn); 
}
 void EINT0_IRQHandler(void) 
{ 
 product_count++;
 LPC_SC->EXTINT = (1<<0); 
}
 /* Defect Counter Interrupt */ 
void eint1_init(void) 
{
 LPC_PINCON->PINSEL4 &= ~(3<<22); 
 LPC_PINCON->PINSEL4 |= (1<<22);
 LPC_SC->EXTMODE |= (1<<1);
 LPC_SC->EXTPOLAR &= ~(1<<1); 
 LPC_SC->EXTINT = (1<<1); 
 NVIC_EnableIRQ(EINT1_IRQn); 
} 
void EINT1_IRQHandler(void) 
{
 defect_count++; 
 LPC_SC->EXTINT = (1<<1);
} 
/* Timer */ 
void timer0_init(void) 
{
 LPC_SC->PCONP |= (1<<1); 
 LPC_SC->PCLKSEL0 |= (1<<2); 
 LPC_TIM0->PR = 1000-1; 
 LPC_TIM0->MR0 = 1; 
 LPC_TIM0->MCR = 3; 
 NVIC_EnableIRQ(TIMER0_IRQn); 
 LPC_TIM0->TCR = 1; 
} 
void TIMER0_IRQHandler(void) 
{
 rate = product_count - prev_count; 
 prev_count = product_count; 
 if(product_count != 0) 
 {
  efficiency = ((product_count - defect_count) * 100) / product_count; 
 } 
 LPC_TIM0->IR = 1; 
}
 /* ALERT */ 
 void maintenance_alert(void) 
 {
  static uint8_t blink = 0; 
  if(defect_count >= 5) 
  {
   p1->FIOSET = BUZZER; 
   if(blink) 
   {
    p1->FIOSET = ALL_LEDS; 
   }
   else 
   {
    p1->FIOCLR = ALL_LEDS;
   } 
   blink ^= 1;
  } 
  else 
  {
   p1->FIOCLR = BUZZER; 
   p1->FIOCLR = ALL_LEDS; 
   blink = 0; 
  } 
 } 
/* UART0 */ 
void uart0_init(void) 
{
 LPC_SC->PCONP |= (1<<3); 
 LPC_PINCON->PINSEL0 |= (1<<4)|(1<<6); 
 LPC_UART0->LCR = 0x83; 
 LPC_UART0->DLL = 97; 
 LPC_UART0->DLM = 0; 
 LPC_UART0->LCR = 0x03; 
} 
void uart0_tx(char c) 
{ 
while(!(LPC_UART0->LSR & (1<<5))); 
LPC_UART0->THR = c; } 
void uart0_string(char *s) 
{ 
while(*s) 
{ 
uart0_tx(*s++); 
} 
} 
void uart0_num(uint32_t n) 
{ 
  char arr[10]; 
  int i=0; 
  if(n==0) 
  { 
  uart0_tx('0'); 
  return; 
  } 
  while(n) 
  { 
  arr[i++] = (n%10)+'0'; n/=10; 
  } 
  while(i) 
  { 
  uart0_tx(arr[--i]); 
  } 
} 
void uart0_send_data(void) 
{ 
   uart0_string("CNT="); 
   uart0_num(product_count); 
   uart0_string(",BAD="); 
   uart0_num(defect_count); 
   uart0_string(",RATE="); 
   uart0_num(rate); 
   uart0_string(",EFF="); 
   uart0_num(efficiency); 
   uart0_string("\r\n"); 
  }