#include "adc_driver.h"
#include "system_init.h"
#include <math.h>

//define ADC channels connected to pins used to read sensors data
#define SENSOR1		ADC_CHANNEL_14		//A0 L4 pin - PC5 STM32 pin
#define SENSOR2		ADC_CHANNEL_13		//A1 L4 pin - PC4 STM32 pin
#define SENSOR3		ADC_CHANNEL_4		//A2 L4 pin - PC3 STM32 pin
#define SENSOR4		ADC_CHANNEL_3		//A3 L4 pin - PC2 STM32 pin
#define SENSOR5		ADC_CHANNEL_2		//A4 L4 pin - PC1 STM32 pin
#define SENSOR6		ADC_CHANNEL_1		//A5 L4 pin - PC0 STM32 pin
#define SENSOR7		ADC_CHANNEL_9		//D7 L4 pin - PA4 STM32 pin
#define SENSOR8		ADC_CHANNEL_6		//D0 L4 pin - PA1 STM32 pin
#define SENSOR9		ADC_CHANNEL_5		//D1 L4 pin - PA0 STM32 pin

//#define DEBUG_MODE

//fine tune the range considered to same color
// color_sensitivity = 3 means +- 3 range
uint8_t color_sensitivity = 10;

//define local variables
int32_t sensor_average = 0;
int32_t background_color = 0;
int32_t background_sum = 0;
int8_t cntr = 0;
int8_t line_position;

//define sensor data structure
typedef struct {
	int16_t sensor1_data;
	int16_t sensor2_data;
	int16_t sensor3_data;
	int16_t sensor4_data;
	int16_t sensor5_data;
	int16_t sensor6_data;
	int16_t sensor7_data;
	int16_t sensor8_data;
	int16_t sensor9_data;
} sensor_data_t;

void select_adc_channel(uint32_t sensor_nr);
int16_t get_sensor1_value();
int16_t get_sensor2_value();
int16_t get_sensor3_value();
int16_t get_sensor4_value();
int16_t get_sensor5_value();
int16_t get_sensor6_value();
int16_t get_sensor7_value();
int16_t get_sensor8_value();
int16_t get_sensor9_value();
sensor_data_t get_line_sensor_data();
int16_t calculate_background_color(sensor_data_t detected_color_tmp);
int8_t calculate_line_position(sensor_data_t detected_color_tmp, int16_t background_color_tmp);
sensor_data_t process_sensor_data(sensor_data_t sensor_data_tmp);



int8_t handle_line_position()
{
	no_line_flag = 1;
	line_position = 100;

	sensor_data_t sensor_data = get_line_sensor_data();

	sensor_data_t detected_color = process_sensor_data(sensor_data);

	int16_t detected_background_color = calculate_background_color(detected_color);

#ifdef DEBUG_MODE
	printf("background_color: %d; ", detected_background_color);
#endif

	line_position = calculate_line_position(detected_color, detected_background_color);

#ifdef DEBUG_MODE
	printf("line position: %d; no line flag: %d", line_position, no_line_flag);
#endif

	return line_position;
}

//read data from line sensor in 2 separate groups
sensor_data_t get_line_sensor_data()
{
	sensor_data_t sensor_data_tmp;

	//turn on sensor group 1
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, GPIO_PIN_SET);
	HAL_Delay(1); //time for mosfet

	//get sensor group 1 data
	sensor_data_tmp.sensor1_data = get_sensor1_value();
	sensor_data_tmp.sensor3_data = get_sensor3_value();
	sensor_data_tmp.sensor5_data = get_sensor5_value();
	sensor_data_tmp.sensor7_data = get_sensor7_value();
	sensor_data_tmp.sensor9_data = get_sensor9_value();

	//turn on sensor group 2
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, GPIO_PIN_RESET);
	HAL_Delay(1); //time for mosfet

	//get sensor group 2 data
	sensor_data_tmp.sensor2_data = get_sensor2_value();
	sensor_data_tmp.sensor4_data = get_sensor4_value();
	sensor_data_tmp.sensor6_data = get_sensor6_value();
	sensor_data_tmp.sensor8_data = get_sensor8_value();

#ifdef DEBUG_MODE
	printf("S1: %d; ", sensor_data_tmp.sensor1_data);
	printf("S2: %d; ", sensor_data_tmp.sensor2_data);
	printf("S3: %d; ", sensor_data_tmp.sensor3_data);
	printf("S4: %d; ", sensor_data_tmp.sensor4_data);
	printf("S5: %d; ", sensor_data_tmp.sensor5_data);
	printf("S6: %d; ", sensor_data_tmp.sensor6_data);
	printf("S7: %d; ", sensor_data_tmp.sensor7_data);
	printf("S8: %d; ", sensor_data_tmp.sensor8_data);
	printf("S9: %d; ", sensor_data_tmp.sensor9_data);
	printf("\n");
#endif

	return sensor_data_tmp;
}

//read adc data of optosensors
int16_t get_sensor1_value()
{
	select_adc_channel(SENSOR1);
	HAL_ADC_Start(&adc_handle);
	HAL_ADC_PollForConversion(&adc_handle, HAL_MAX_DELAY);
	return HAL_ADC_GetValue(&adc_handle);
}

int16_t get_sensor2_value()
{
	select_adc_channel(SENSOR2);
	HAL_ADC_Start(&adc_handle);
	HAL_ADC_PollForConversion(&adc_handle, HAL_MAX_DELAY);
	return HAL_ADC_GetValue(&adc_handle);
}

int16_t get_sensor3_value()
{
	select_adc_channel(SENSOR3);
	HAL_ADC_Start(&adc_handle);
	HAL_ADC_PollForConversion(&adc_handle, HAL_MAX_DELAY);
	return HAL_ADC_GetValue(&adc_handle);
}

int16_t get_sensor4_value()
{
	select_adc_channel(SENSOR4);
	HAL_ADC_Start(&adc_handle);
	HAL_ADC_PollForConversion(&adc_handle, HAL_MAX_DELAY);
	return HAL_ADC_GetValue(&adc_handle);
}

int16_t get_sensor5_value()
{
	select_adc_channel(SENSOR5);
	HAL_ADC_Start(&adc_handle);
	HAL_ADC_PollForConversion(&adc_handle, HAL_MAX_DELAY);
	return HAL_ADC_GetValue(&adc_handle);
}

int16_t get_sensor6_value()
{
	select_adc_channel(SENSOR6);
	HAL_ADC_Start(&adc_handle);
	HAL_ADC_PollForConversion(&adc_handle, HAL_MAX_DELAY);
	return HAL_ADC_GetValue(&adc_handle);
}

int16_t get_sensor7_value()
{
	select_adc_channel(SENSOR7);
	HAL_ADC_Start(&adc_handle);
	HAL_ADC_PollForConversion(&adc_handle, HAL_MAX_DELAY);
	return HAL_ADC_GetValue(&adc_handle);
}

int16_t get_sensor8_value()
{
	select_adc_channel(SENSOR8);
	HAL_ADC_Start(&adc_handle);
	HAL_ADC_PollForConversion(&adc_handle, HAL_MAX_DELAY);
	return HAL_ADC_GetValue(&adc_handle);
}

int16_t get_sensor9_value()
{
	select_adc_channel(SENSOR9);
	HAL_ADC_Start(&adc_handle);
	HAL_ADC_PollForConversion(&adc_handle, HAL_MAX_DELAY);
	return HAL_ADC_GetValue(&adc_handle);
}

//select opto sensor before reading adc data
void select_adc_channel(uint32_t sensor_nr)
{
	adc_ch_conf.Channel = sensor_nr;
	HAL_ADC_ConfigChannel(&adc_handle, &adc_ch_conf);
}

//process line sensor data, get line position, or no line signal
sensor_data_t process_sensor_data(sensor_data_t sensor_data_tmp)
{
	sensor_data_t detected_color_tmp;

	//turn ADC data into -35 -- +35 range detected color data
	//negative data means dark colors, positive data means light colors
	detected_color_tmp.sensor1_data = (sensor_data_tmp.sensor1_data) / 100;
	detected_color_tmp.sensor2_data = (sensor_data_tmp.sensor2_data) / 100;
	detected_color_tmp.sensor3_data = (sensor_data_tmp.sensor3_data) / 100;
	detected_color_tmp.sensor4_data = (sensor_data_tmp.sensor4_data) / 100;
	detected_color_tmp.sensor5_data = (sensor_data_tmp.sensor5_data) / 100;
	detected_color_tmp.sensor6_data = (sensor_data_tmp.sensor6_data) / 100;
	detected_color_tmp.sensor7_data = (sensor_data_tmp.sensor7_data) / 100;
	detected_color_tmp.sensor8_data = (sensor_data_tmp.sensor8_data) / 100;
	detected_color_tmp.sensor9_data = (sensor_data_tmp.sensor9_data) / 100;
#ifdef DEBUG_MODE
	printf("C1: %d; ", detected_color_tmp.sensor1_data);
	printf("C2: %d; ", detected_color_tmp.sensor2_data);
	printf("C3: %d; ", detected_color_tmp.sensor3_data);
	printf("C4: %d; ", detected_color_tmp.sensor4_data);
	printf("C5: %d; ", detected_color_tmp.sensor5_data);
	printf("C6: %d; ", detected_color_tmp.sensor6_data);
	printf("C7: %d; ", detected_color_tmp.sensor7_data);
	printf("C8: %d; ", detected_color_tmp.sensor8_data);
	printf("C9: %d; ", detected_color_tmp.sensor9_data);
	printf("\n");
#endif

	return detected_color_tmp;
}

//calculate background color based on measured color data
int16_t calculate_background_color(sensor_data_t detected_color_tmp)
{
	sensor_average = 0;
	cntr = 0;
	background_sum = 0;
	int16_t background_color_tmp = 0;

	//calculate all sensor data average
	sensor_average =   (detected_color_tmp.sensor1_data +
						detected_color_tmp.sensor2_data +
						detected_color_tmp.sensor3_data +
						detected_color_tmp.sensor4_data +
						detected_color_tmp.sensor5_data +
						detected_color_tmp.sensor6_data +
						detected_color_tmp.sensor7_data +
						detected_color_tmp.sensor8_data +
						detected_color_tmp.sensor9_data) / 9;

	//select background data (sensors measuring data around average), excluding line(s)
	if ((detected_color_tmp.sensor1_data < sensor_average + color_sensitivity) &&
		(detected_color_tmp.sensor1_data > sensor_average - color_sensitivity)) {
		background_sum = background_sum + detected_color_tmp.sensor1_data;
		cntr++;
	}
	if ((detected_color_tmp.sensor2_data < sensor_average + color_sensitivity) &&
		(detected_color_tmp.sensor2_data > sensor_average - color_sensitivity)) {
		background_sum = background_sum + detected_color_tmp.sensor2_data;
		cntr++;
	}
	if ((detected_color_tmp.sensor3_data < sensor_average + color_sensitivity) &&
		(detected_color_tmp.sensor3_data > sensor_average - color_sensitivity)) {
		background_sum = background_sum + detected_color_tmp.sensor3_data;
		cntr++;
	}
	if ((detected_color_tmp.sensor4_data < sensor_average + color_sensitivity) &&
		(detected_color_tmp.sensor4_data > sensor_average - color_sensitivity)) {
		background_sum = background_sum + detected_color_tmp.sensor4_data;
		cntr++;
	}
	if ((detected_color_tmp.sensor5_data < sensor_average + color_sensitivity) &&
		(detected_color_tmp.sensor5_data > sensor_average - color_sensitivity)) {
		background_sum = background_sum + detected_color_tmp.sensor5_data;
		cntr++;
	}
	if ((detected_color_tmp.sensor6_data < sensor_average + color_sensitivity) &&
		(detected_color_tmp.sensor6_data > sensor_average - color_sensitivity)) {
		background_sum = background_sum + detected_color_tmp.sensor6_data;
		cntr++;
	}
	if ((detected_color_tmp.sensor7_data < sensor_average + color_sensitivity) &&
		(detected_color_tmp.sensor7_data > sensor_average - color_sensitivity)) {
		background_sum = background_sum + detected_color_tmp.sensor7_data;
		cntr++;
	}
	if ((detected_color_tmp.sensor8_data < sensor_average + color_sensitivity) &&
		(detected_color_tmp.sensor8_data > sensor_average - color_sensitivity)) {
		background_sum = background_sum + detected_color_tmp.sensor8_data;
		cntr++;
	}
	if ((detected_color_tmp.sensor9_data < sensor_average + color_sensitivity) &&
		(detected_color_tmp.sensor9_data > sensor_average - color_sensitivity)) {
		background_sum = background_sum + detected_color_tmp.sensor9_data;
		cntr++;
	}
	background_color_tmp = background_sum / cntr;

#ifdef DEBUG_MODE
	printf("background_sum: %lu, cntr: %d; ", background_sum, cntr);
#endif

	return background_color_tmp;
}

int8_t calculate_line_position(sensor_data_t detected_color_tmp, int16_t background_color_tmp)
{
	//select sensors measuring different color compared to background
	//determine line position based on sensor number
	//start with middle sensor #5, move outward by pairs: line position range is -4 to +4, 0--middle position
	//also set no_line_flag: 0--line is detected, 1--no line detected

	if ((detected_color_tmp.sensor5_data > background_color_tmp + color_sensitivity) ||
		(detected_color_tmp.sensor5_data < background_color_tmp - color_sensitivity)) {
		line_position = 0;
		no_line_flag = 0;
	}
	if ((detected_color_tmp.sensor4_data > background_color_tmp + color_sensitivity) ||
		(detected_color_tmp.sensor4_data < background_color_tmp - color_sensitivity)) {
		line_position = 1;
		no_line_flag = 0;
	}
	if ((detected_color_tmp.sensor6_data > background_color_tmp + color_sensitivity) ||
		(detected_color_tmp.sensor6_data < background_color_tmp - color_sensitivity)) {
		line_position = -1;
		no_line_flag = 0;
	}
	if ((detected_color_tmp.sensor3_data > background_color_tmp + color_sensitivity) ||
		(detected_color_tmp.sensor3_data < background_color_tmp - color_sensitivity)) {
		line_position = 2;
		no_line_flag = 0;
	}
	if ((detected_color_tmp.sensor7_data > background_color_tmp + color_sensitivity) ||
		(detected_color_tmp.sensor7_data < background_color_tmp - color_sensitivity)) {
		line_position = -2;
		no_line_flag = 0;
	}
	if ((detected_color_tmp.sensor2_data > background_color_tmp + color_sensitivity) ||
		(detected_color_tmp.sensor2_data < background_color_tmp - color_sensitivity)) {
		line_position = 3;
		no_line_flag = 0;
	}
	if ((detected_color_tmp.sensor8_data > background_color_tmp + color_sensitivity) ||
		(detected_color_tmp.sensor8_data < background_color_tmp - color_sensitivity)) {
		line_position = -3;
		no_line_flag = 0;
	}
	if ((detected_color_tmp.sensor1_data > background_color_tmp + color_sensitivity) ||
		(detected_color_tmp.sensor1_data < background_color_tmp - color_sensitivity)) {
		line_position = 4;
		no_line_flag = 0;
	}
	if ((detected_color_tmp.sensor9_data > background_color_tmp + color_sensitivity) ||
		(detected_color_tmp.sensor9_data < background_color_tmp - color_sensitivity)) {
		line_position = -4;
		no_line_flag = 0;
	}

	return line_position;
}
