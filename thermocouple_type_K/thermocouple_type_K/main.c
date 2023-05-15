#include <atmel_start.h>
#include <math.h>
#include <atomic.h>

#define CLK_PER 20000000ul
#define TIMEBASE_VALUE ((uint8_t) ceil(CLK_PER*0.000001))


volatile int32_t adc_res;
volatile uint8_t reading_in_course, data_process_in_course;
volatile uint8_t mV_integer_part, mV_frac_integer, dezena, unidade, pcdv, scdv;
volatile float temp_calc, temp_frac;
volatile int16_t u_voltage;
volatile uint16_t temperature;

volatile int16_t uv[] = {-778, -739, -701, -663, -624, -586, -547, -508, -470, -431, -392, -353, -314, -275, -236, -197, -157, -118, -79, -39, 0, 39, 79, 119, 158, 198, 238, 277, 317, 357, 397, 437, 477, 517, 557, 597, 637, 677, 718, 758, 798, 838, 879, 919, 960, 1000, 1041, 1081, 1122, 1163, 1203, 1244, 1285, 1326, 1366, 1407, 1448, 1489, 1530, 1571, 1612, 1653, 1694, 1735, 1776, 1817, 1858, 1899, 1941, 1982, 2023, 2064, 2106, 2147, 2188, 2230, 2271, 2312, 2354, 2395, 2436, 2478, 2519, 2561, 2602, 2644, 2685, 2727, 2768, 2810, 2851, 2893, 2934, 2976, 3017, 3059, 3100, 3142, 3184, 3225, 3267, 3308, 3350, 3391, 3433, 3474, 3516, 3557, 3599, 3640, 3682, 3723, 3765, 3806, 3848, 3889, 3931, 3972, 4013, 4055, 4096, 4138, 4179, 4220, 4262, 4303, 4344, 4385, 4427, 4468, 4509, 4550, 4591, 4633, 4674, 4715, 4756, 4797, 4838, 4879, 4920, 4961, 5002, 5043, 5084, 5124, 5165, 5206, 5247, 5288, 5328, 5369, 5410, 5450, 5491, 5532, 5572, 5613, 5653, 5694, 5735, 5775, 5815, 5856, 5896, 5937, 5977, 6017, 6058, 6098, 6138, 6179, 6219, 6259, 6299, 6339, 6380, 6420, 6460, 6500, 6540, 6580, 6620, 6660, 6701, 6741, 6781, 6821, 6861, 6901, 6941, 6981, 7021, 7060, 7100, 7140, 7180, 7220, 7260, 7300, 7340, 7380, 7420, 7460, 7500, 7540, 7579, 7619, 7659, 7699, 7739, 7779, 7819, 7859, 7899, 7939, 7979, 8019, 8059, 8099, 8138, 8178, 8218, 8258, 8298, 8338, 8378, 8418, 8458, 8499, 8539, 8579, 8619, 8659, 8699, 8739, 8779, 8819, 8860, 8900, 8940, 8980, 9020, 9061, 9101, 9141, 9181, 9222, 9262, 9302, 9343, 9383, 9423, 9464, 9504, 9545, 9585, 9626, 9666, 9707, 9747, 9788, 9828, 9869, 9909, 9950, 9991, 10031, 10072, 10113, 10153, 10194, 10235, 10276, 10316, 10357, 10398, 10439, 10480, 10520};

volatile int16_t temp_var[] = {-10, -9, -8, -7, -6, -5, -4, -3, -2, -1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122, 123, 124, 125, 126, 127, 128, 129, 130, 131, 132, 133, 134, 135, 136, 137, 138, 139, 140, 141, 142, 143, 144, 145, 146, 147, 148, 149, 150, 151, 152, 153, 154, 155, 156, 157, 158, 159, 160, 161, 162, 163, 164, 165, 166, 167, 168, 169, 170, 171, 172, 173, 174, 175, 176, 177, 178, 179, 180, 181, 182, 183, 184, 185, 186, 187, 188, 189, 190, 191, 192, 193, 194, 195, 196, 197, 198, 199, 200, 201, 202, 203, 204, 205, 206, 207, 208, 209, 210, 211, 212, 213, 214, 215, 216, 217, 218, 219, 220, 221, 222, 223, 224, 225, 226, 227, 228, 229, 230, 231, 232, 233, 234, 235, 236, 237, 238, 239, 240, 241, 242, 243, 244, 245, 246, 247, 248, 249, 250};



void ADC_init(void);
void ADC_pins_init(void);
void start_ADC_reading(void);
void process_data(void);
uint16_t find_index_temp(void);


int main(void)
{
	/* Initializes MCU, drivers and middleware */
	atmel_start_init();
	ADC_init();
	ENABLE_INTERRUPTS(); //habilita interrupcao
	reading_in_course = 0;
	data_process_in_course = 0;
	
	/* Replace with your application code */
	while (1) {
		if((!reading_in_course)&&(!data_process_in_course)){
			start_ADC_reading();
		}
		if((!reading_in_course)&&(data_process_in_course)){
			process_data();
			mV_integer_part = floor(temp_calc);
			temp_frac = temp_calc - (float)mV_integer_part;
			temp_frac = temp_frac*100;
			mV_frac_integer = floor(temp_frac);
			
			dezena = mV_integer_part/10;
			unidade = mV_integer_part%10;
			pcdv = mV_frac_integer/10;
			scdv = mV_frac_integer%10;
			
			u_voltage = floor(1000*temp_calc);
			temperature = temp_var[find_index_temp()-9];
			
			USART_0_write('\r');
			USART_0_write(dezena+48);
			USART_0_write(unidade+48);
			USART_0_write('.');
			USART_0_write(pcdv+48);
			USART_0_write(scdv+48);
			USART_0_write('m');
			USART_0_write('V');
		}
	}
}


void ADC_pins_init(void){
	PORTA.DIRCLR = 0b00000110; //PA1 e PA2 sao inputs
	PORTA.PIN1CTRL = PORT_ISC_INPUT_DISABLE_gc; //buffer digital desligado no PA1
	PORTA.PIN2CTRL = PORT_ISC_INPUT_DISABLE_gc; //buffer digital desligado no PA2
}




void ADC_init(void){
	ADC_pins_init();
	ADC0.CTRLA = ADC0.CTRLA|ADC_ENABLE_bm|ADC_LOWLAT_bm;	//enable ADC and low latency
	ADC0.CTRLB = ADC_PRESC_DIV48_gc;			//ADC_CLOCK = 5MHz (20/4)
	ADC0.CTRLC = ADC_REFSEL_1024MV_gc | (TIMEBASE_VALUE << ADC_TIMEBASE_gp); //referencia 1.024V e configura timebase. VRef eh o PA5
	ADC0.DBGCTRL = ADC_DBGRUN_bm;
	ADC0.CTRLE = 250; //sampdur eh 150
	ADC0.PGACTRL = ADC_GAIN_16X_gc|ADC_ADCPGASAMPDUR_20CLK_gc|ADC_PGAEN_bm; //ganho 16x, sampdur do pga 20 clock, enable PGA
	ADC0.CTRLF = ADC_SAMPNUM_ACC1024_gc; //acumula 1024 samples
	ADC0.MUXPOS = ADC_MUXPOS_AIN1_gc; //PA1 positivo
	ADC0.MUXNEG = ADC_MUXNEG_AIN2_gc; //PA2 negativo
	ADC0.MUXPOS = ADC0.MUXPOS|ADC_VIA_PGA_gc; //habilita via para o PGA
	ADC0.COMMAND = ADC_DIFF_bm|ADC_MODE_BURST_gc|ADC_START_MUXPOS_WRITE_gc; //conversao diferencial, modo burst com scalling (right shift 4), inicia conversao ao escrever no MUXPOS
	ADC0.INTCTRL = ADC_RESRDY_bm; //habilita interrupção de resultado
	ADC0.INTFLAGS = ADC_RESRDY_bm; //limpa a flag de resultado
}

void start_ADC_reading(void){
	ADC0.MUXPOS = ADC0.MUXPOS|ADC_VIA_PGA_gc; //escrever no MUXPOS para iniciar a leitura
	reading_in_course = 1;
	data_process_in_course = 1;
}

void process_data(void){
	//add data processing here
	data_process_in_course = 0;
	/*
	temp_calc = ((float)adc_res)-9.5259;
	temp_calc =temp_calc/31.283;*/
	temp_calc = (((float)adc_res)*64)/((float)2047);
	if(temp_calc>7){
		temp_calc = temp_calc +0.20;
	}else{
		temp_calc = temp_calc +0.10;
	}	
}

uint16_t find_index_temp(void){
	uint16_t nnn;
	for(nnn=0;nnn<260;nnn++){
		if((uv[nnn]<=u_voltage)&&(uv[nnn+1]>u_voltage)){
			break;
		}
	}
	
	return nnn;
}



ISR(ADC0_RESRDY_vect)
{
	ADC0.INTFLAGS = ADC_RESRDY_bm;
	reading_in_course = 0;
	adc_res = ADC0.RESULT; //ler o resultado (que ja está com shift 4)
	adc_res = adc_res/1024; //fazer mais um shift. A teoria do oversampling diz: acumule 4^n amostras e divida por 2^n. No nosso caso n eh 5
}