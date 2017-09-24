#include "adc_driver.h"
#include "system_init.h"
#include <math.h>

//define ADC channels connected to pins used to read sensors data
#define SENSOR1_CH	ADC_CHANNEL_14		//A0 L4 pin - PC5 STM32 pin
#define SENSOR2_CH	ADC_CHANNEL_13		//A1 L4 pin - PC4 STM32 pin
#define SENSOR3_CH	ADC_CHANNEL_4		//A2 L4 pin - PC3 STM32 pin
#define SENSOR4_CH	ADC_CHANNEL_3		//A3 L4 pin - PC2 STM32 pin
#define SENSOR5_CH	ADC_CHANNEL_2		//A4 L4 pin - PC1 STM32 pin
#define SENSOR6_CH	ADC_CHANNEL_1		//A5 L4 pin - PC0 STM32 pin
#define SENSOR7_CH	ADC_CHANNEL_9		//D7 L4 pin - PA4 STM32 pin
#define SENSOR8_CH	ADC_CHANNEL_6		//D0 L4 pin - PA1 STM32 pin
#define SENSOR9_CH	ADC_CHANNEL_5		//D1 L4 pin - PA0 STM32 pin
#define DEBUG_MODE

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
void process_sensor_data(sensor_data_t sensor_data);

//read data from line sensor in 2 separate groups
sensor_data_t get_line_sensor_data()
{
	sensor_data_t sensor_data;

	//turn on sensor group 1
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, GPIO_PIN_SET);
	//get sensor group 1 data
	sensor_data.sensor1_data = get_sensor1_value();
	sensor_data.sensor3_data = get_sensor3_value();
	sensor_data.sensor5_data = get_sensor5_value();
	sensor_data.sensor7_data = get_sensor7_value();
	sensor_data.sensor9_data = get_sensor9_value();
	//turn on sensor group 2
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, GPIO_PIN_RESET);
	//get sensor group 2 data
	sensor_data.sensor2_data = get_sensor2_value();
	sensor_data.sensor4_data = get_sensor4_value();
	sensor_data.sensor6_data = get_sensor6_value();
	sensor_data.sensor8_data = get_sensor8_value();
#ifdef DEBUG_MODE
	printf("S1: %d; ", sensor_data.sensor1_data);
	printf("S2: %d; ", sensor_data.sensor2_data);
	printf("S3: %d; ", sensor_data.sensor3_data);
	printf("S4: %d; ", sensor_data.sensor4_data);
	printf("S5: %d; ", sensor_data.sensor5_data);
	printf("S6: %d; ", sensor_data.sensor6_data);
	printf("S7: %d; ", sensor_data.sensor7_data);
	printf("S8: %d; ", sensor_data.sensor8_data);
	printf("S9: %d; ", sensor_data.sensor9_data);
	printf("\n");
#endif
	process_sensor_data(sensor_data);
	return sensor_data;
}

//read adc data of optosensors
int16_t get_sensor1_value()
{
	select_adc_channel(SENSOR1_CH);
	HAL_ADC_Start(&adc_handle);
	HAL_ADC_PollForConversion(&adc_handle, HAL_MAX_DELAY);
	return HAL_ADC_GetValue(&adc_handle);
}

int16_t get_sensor2_value()
{
	select_adc_channel(SENSOR2_CH);
	HAL_ADC_Start(&adc_handle);
	HAL_ADC_PollForConversion(&adc_handle, HAL_MAX_DELAY);
	return HAL_ADC_GetValue(&adc_handle);
}

int16_t get_sensor3_value()
{
	select_adc_channel(SENSOR3_CH);
	HAL_ADC_Start(&adc_handle);
	HAL_ADC_PollForConversion(&adc_handle, HAL_MAX_DELAY);
	return HAL_ADC_GetValue(&adc_handle);
}

int16_t get_sensor4_value()
{
	select_adc_channel(SENSOR4_CH);
	HAL_ADC_Start(&adc_handle);
	HAL_ADC_PollForConversion(&adc_handle, HAL_MAX_DELAY);
	return HAL_ADC_GetValue(&adc_handle);
}

int16_t get_sensor5_value()
{
	select_adc_channel(SENSOR5_CH);
	HAL_ADC_Start(&adc_handle);
	HAL_ADC_PollForConversion(&adc_handle, HAL_MAX_DELAY);
	return HAL_ADC_GetValue(&adc_handle);
}

int16_t get_sensor6_value()
{
	select_adc_channel(SENSOR6_CH);
	HAL_ADC_Start(&adc_handle);
	HAL_ADC_PollForConversion(&adc_handle, HAL_MAX_DELAY);
	return HAL_ADC_GetValue(&adc_handle);
}

int16_t get_sensor7_value()
{
	select_adc_channel(SENSOR7_CH);
	HAL_ADC_Start(&adc_handle);
	HAL_ADC_PollForConversion(&adc_handle, HAL_MAX_DELAY);
	return HAL_ADC_GetValue(&adc_handle);
}

int16_t get_sensor8_value()
{
	select_adc_channel(SENSOR8_CH);
	HAL_ADC_Start(&adc_handle);
	HAL_ADC_PollForConversion(&adc_handle, HAL_MAX_DELAY);
	return HAL_ADC_GetValue(&adc_handle);
}

int16_t get_sensor9_value()
{
	select_adc_channel(SENSOR9_CH);
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

void process_sensor_data(sensor_data_t sensor_data)
{
	sensor_data_t detected_color;
	detected_color.sensor1_data = (sensor1_config.sensor_mid_value - sensor_data.sensor1_data)/10;
	detected_color.sensor2_data = (sensor2_config.sensor_mid_value - sensor_data.sensor2_data)/10;
	detected_color.sensor3_data = (sensor3_config.sensor_mid_value - sensor_data.sensor3_data)/10;
	detected_color.sensor4_data = (sensor4_config.sensor_mid_value - sensor_data.sensor4_data)/10;
	detected_color.sensor5_data = (sensor5_config.sensor_mid_value - sensor_data.sensor5_data)/10;
	detected_color.sensor6_data = (sensor6_config.sensor_mid_value - sensor_data.sensor6_data)/10;
	detected_color.sensor7_data = (sensor7_config.sensor_mid_value - sensor_data.sensor7_data)/10;
	detected_color.sensor8_data = (sensor8_config.sensor_mid_value - sensor_data.sensor8_data)/10;
	detected_color.sensor9_data = (sensor9_config.sensor_mid_value - sensor_data.sensor9_data)/10;
#ifdef DEBUG_MODE
	printf("C1: %d; ", detected_color.sensor1_data);
	printf("C2: %d; ", detected_color.sensor2_data);
	printf("C3: %d; ", detected_color.sensor3_data);
	printf("C4: %d; ", detected_color.sensor4_data);
	printf("C5: %d; ", detected_color.sensor5_data);
	printf("C6: %d; ", detected_color.sensor6_data);
	printf("C7: %d; ", detected_color.sensor7_data);
	printf("C8: %d; ", detected_color.sensor8_data);
	printf("C9: %d; ", detected_color.sensor9_data);
	printf("\n");
#endif
}
