#include <avr/io.h>         /* Include AVR std. library file */
#include <stdio.h>          /* Include std. library file */
#include <util/delay.h>     /* Include Delay header file */
// LCD Pin Definitions
#define LCD_RS PC1
#define LCD_EN PC3
#define LCD_RW PC2
#define LCD_D4 PC4
#define LCD_D5 PC5
#define LCD_D6 PC6
#define LCD_D7 PC7
#define LCD_DATA PORTC  // Data lines of LCD
#define LCD_CTRL PORTC // Control lines of LCD
#define LCD_DATA_DDR DDRC  // Data direction register for LCD
#define LCD_CTRL_DDR DDRC  // Control direction register for LCD
#define ADC_MINx 0  // Giá tr? OCR ?ng v?i -45
#define ADC_MAXx 1023   // Giá tr? OCR ?ng v?i 45
#define ADC_CENTERx 511.5  // Giá tr? OCR t??ng ?ng 0°
float ocr1a_angle;
float ocr1b_angle;
int adcx,adcy;
////////////////////////////////////////////////////////////////////////////


void long_to_string(long number, char* str) {
	sprintf(str, "%ld", number);  // Convert long to string
}

// LCD command function to send commands to the LCD
void lcd_command(unsigned char cmd) {
	LCD_DATA = (LCD_DATA & 0x0F) | (cmd & 0xF0);  // Send high 4 bits of command
	LCD_CTRL &= ~(1 << LCD_RS);  // Select command mode
	LCD_CTRL &= ~(1 << LCD_RW);  // Write mode
	LCD_CTRL |= (1 << LCD_EN);  // Enable LCD
	_delay_us(1);
	LCD_CTRL &= ~(1 << LCD_EN);  // Disable LCD
	_delay_us(20);

	LCD_DATA = (LCD_DATA & 0x0F) | (cmd << 4);  // Send low 4 bits of command
	LCD_CTRL |= (1 << LCD_EN);  // Enable LCD
	_delay_us(1);
	LCD_CTRL &= ~(1 << LCD_EN);  // Disable LCD
	_delay_us(2);
}

// LCD data function to send data (characters) to the LCD
void lcd_data(unsigned char data) {
	LCD_DATA = (LCD_DATA & 0x0F) | (data & 0xF0);  // Send high 4 bits of data
	LCD_CTRL |= (1 << LCD_RS);  // Select data mode
	LCD_CTRL &= ~(1 << LCD_RW);  // Write mode
	LCD_CTRL |= (1 << LCD_EN);  // Enable LCD
	_delay_us(1);
	LCD_CTRL &= ~(1 << LCD_EN);  // Disable LCD
	_delay_us(20);

	LCD_DATA = (LCD_DATA & 0x0F) | (data << 4);  // Send low 4 bits of data
	LCD_CTRL |= (1 << LCD_EN);  // Enable LCD
	_delay_us(1);
	LCD_CTRL &= ~(1 << LCD_EN);  // Disable LCD
	_delay_us(2);
}

// LCD initialization function
void lcd_init() {
	LCD_DATA_DDR |= 0xF0;  // Set upper 4 bits of PORTA as output for data
	LCD_CTRL_DDR |= (1 << LCD_RS) | (1 << LCD_RW) | (1 << LCD_EN);  // Set control pins as output

	_delay_ms(50);  // Wait for LCD to power up
	lcd_command(0x33);    // G?i l?nh 3 l?n ?? chuy?n sang ch? ?? 8-bit
	lcd_command(0x32);    // Chuy?n sang ch? ?? 4-bit
	lcd_command(0x28);    // Ch?n 2 dòng, font 5x8
	lcd_command(0x0C);    // B?t màn hình, t?t con tr?
	lcd_command(0x06);    // T?ng con tr?, không d?ch màn hình
	lcd_command(0x01);    // Xóa màn hình
	_delay_ms(2);
}

// LCD print function to display a string on the LCD
void lcd_print(const char *str) {
	while (*str) {
		lcd_data(*str++);  // Send each character to the LCD
	}
}

// LCD cursor positioning function
void lcd_set_cursor(unsigned char x, unsigned char y) {
	unsigned char pos[] = {0x80, 0xC0};  // Row 1 and Row 2 start positions
	lcd_command(pos[y] + x);  // Set cursor position based on x, y
}
float adc_to_angle_x(uint16_t adc_value) {
	return 0 - ((adc_value - 499.0) / (1023.0 - 499.0)) * 45.0;
}

// Chuy?n ??i giá tr? ADC c?a tr?c Y sang góc
float adc_to_angle_y(uint16_t adc_value) {
	return 0-((adc_value - 513.0) / (1023.0 - 513.0)) * 45.0;
}
// Function to display speed and percentage on the LCD
void lcd_display_info() {
	lcd_set_cursor(0, 0);  // Set cursor to first line, 9th column
	lcd_print("Speed");
	lcd_set_cursor(6, 0);  // Set cursor to first line, 6th column
	char buffer[10];
	int phantram=(OCR2*100)/255;
	long_to_string(phantram, buffer);  // Convert percentage to string
	lcd_print(buffer);
	if (phantram<100){
		lcd_set_cursor(8, 0);  // Set cursor to first line, 9th column
		lcd_print(" ");
	}
	if (phantram<10){
			lcd_set_cursor(7, 0);  // Set cursor to first line, 9th column
			lcd_print(" ");
	}
		lcd_set_cursor(9, 0);
		lcd_print("%");
	

	float angle1 = adc_to_angle_x(adcx);  // Tính góc t? ADC kênh 0
	float angle2 = adc_to_angle_y(adcy);  // Tính góc t? ADC kênh 1
	lcd_set_cursor(0, 1);
	lcd_print("X: ");
	char angleA[10];
	long_to_string(angle1, angleA);
	lcd_print(angleA);
	if (angle1<10 && angle1>-1){
		lcd_set_cursor(4, 1);  // Set cursor to first line, 9th column
		lcd_print(" ");
	}
	if (angle1>-10){
		lcd_set_cursor(5, 1);  // Set cursor to first line, 9th column
		lcd_print(" ");
	}
	
	lcd_set_cursor(8, 1);
	lcd_print("Y: ");
	char angleB[10];
	long_to_string(angle2, angleB);
	lcd_print(angleB);
	if (angle2<10 && angle2>-1){
		lcd_set_cursor(12, 1);  // Set cursor to first line, 9th column
		lcd_print(" ");
	}
	if (angle2>-10){
		lcd_set_cursor(13, 1);  // Set cursor to first line, 9th column
		lcd_print(" ");
	}
}


////////////////////////////////////////////////////////////////////////////////////////
void ADC_Init()             /* ADC Initialization function */
{
	DDRA = 0x00;            /* Make ADC port as input */
	ADCSRA = 0x87;          /* Enable ADC, with freq/128  */
	ADMUX = 0x40;           /* Vref: Avcc, ADC channel: 0 */
}

int ADC_Read(char channel)  /* ADC Read function */
{
	ADMUX = 0x40 | (channel & 0x07);    /* set input channel to read */
	ADCSRA |= (1<<ADSC);                /* Start ADC conversion */

	/* Wait until end of conversion by polling ADC interrupt flag */
	while (!(ADCSRA & (1<<ADIF)));
	ADCSRA |= (1<<ADIF);                /* Clear interrupt flag */
	return ADCW;                        /* Return ADC word */
}


int main(void)
{
	ADC_Init();             /* Initialize ADC */
	DDRD |= (1 << PD7);     /* Set PD7 (OC2) as output */
	DDRD |= (1 << PD4);       /* Set OC1B (PD4) as output */
	DDRD |= (1 << PD5);       /* Set OC1A (PD5) as output */

	TCNT1 = 0;              /* Set timer1 count to zero */
	ICR1 = 312;            /* Set TOP count for Timer1 in ICR1 register */

	/* Set Fast PWM, TOP in ICR1, Clear OC1A and OC1B on compare match, clk/64 */
	TCCR1A = (1<<WGM11) | (1<<COM1A1) | (1<<COM1B1);
	TCCR1B = (1<<WGM12) | (1<<WGM13) | (1<<CS10) | (1<<CS11);
	TCCR2 = (1 << WGM00) | (1 << WGM01) | (1 << COM21) | (1 << CS21);
	lcd_init();
	while(1)
	{	lcd_display_info();
		adcx=ADC_Read(0);
		adcy=ADC_Read(1);
		OCR1A = 15.625 + ((1023 - ADC_Read(0)) / 65.3);  /* ??o chi?u ADC cho OC1A */
		OCR1B = 15.625 + (ADC_Read(1) / 65.3);  /* PWM on OC1B (PD4) */
		OCR2 = (1023 - ADC_Read(2)) / 4; 
	}
}
