#include "adc_driver.h"
#include "system_init.h"

#define SENSOR1_CH	ADC_CHANNEL_14
#define SENSOR2_CH	ADC_CHANNEL_13
#define SENSOR3_CH	ADC_CHANNEL_4
#define SENSOR4_CH	ADC_CHANNEL_3
#define SENSOR5_CH	ADC_CHANNEL_2
#define SENSOR6_CH	ADC_CHANNEL_1
#define SENSOR7_CH	ADC_CHANNEL_9
#define SENSOR8_CH	ADC_CHANNEL_6
#define SENSOR9_CH	ADC_CHANNEL_5


#define DEBUG_MODE

/*extern ADC_HandleTypeDef sensor1_handle;
extern ADC_HandleTypeDef sensor2_handle;
extern ADC_HandleTypeDef sensor3_handle;
extern ADC_HandleTypeDef sensor4_handle;
extern ADC_HandleTypeDef sensor5_handle;
extern ADC_HandleTypeDef sensor6_handle;
extern ADC_HandleTypeDef sensor7_handle;
extern ADC_HandleTypeDef sensor8_handle;
extern ADC_HandleTypeDef sensor9_handle;*/
typedef struct
{
	uint16_t sensor1_data;
	uint16_t sensor2_data;
	uint16_t sensor3_data;
	uint16_t sensor4_data;
	uint16_t sensor5_data;
	uint16_t sensor6_data;
	uint16_t sensor7_data;
	uint16_t sensor8_data;
	uint16_t sensor9_data;
} sensor_data_t;

sensor_data_t sensor_data;

void select_adc_channel(uint32_t sensor_nr);
uint16_t get_sensor1_value();
uint16_t get_sensor2_value();
uint16_t get_sensor3_value();
uint16_t get_sensor4_value();
uint16_t get_sensor5_value();
uint16_t get_sensor6_value();
uint16_t get_sensor7_value();
uint16_t get_sensor8_value();
uint16_t get_sensor9_value();



sensor_data_t get_sensor_data()
{
	sensor_data_t get_sensor_data();
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
	return sensor_data;
}

uint16_t get_sensor1_value()
{
	select_adc_channel(SENSOR1_CH);
	HAL_ADC_Start(&adc_handle);
	HAL_ADC_PollForConversion(&adc_handle, HAL_MAX_DELAY);
	return HAL_ADC_GetValue(&adc_handle);
}

uint16_t get_sensor2_value()
{
	select_adc_channel(SENSOR2_CH);
	HAL_ADC_Start(&adc_handle);
	HAL_ADC_PollForConversion(&adc_handle, HAL_MAX_DELAY);
	return HAL_ADC_GetValue(&adc_handle);
}

uint16_t get_sensor3_value()
{
	select_adc_channel(SENSOR3_CH);
	HAL_ADC_Start(&adc_handle);
	HAL_ADC_PollForConversion(&adc_handle, HAL_MAX_DELAY);
	return HAL_ADC_GetValue(&adc_handle);
}

uint16_t get_sensor4_value()
{
	select_adc_channel(SENSOR4_CH);
	HAL_ADC_Start(&adc_handle);
	HAL_ADC_PollForConversion(&adc_handle, HAL_MAX_DELAY);
	return HAL_ADC_GetValue(&adc_handle);
}

uint16_t get_sensor5_value()
{
	select_adc_channel(SENSOR5_CH);
	HAL_ADC_Start(&adc_handle);
	HAL_ADC_PollForConversion(&adc_handle, HAL_MAX_DELAY);
	return HAL_ADC_GetValue(&adc_handle);
}

uint16_t get_sensor6_value()
{
	select_adc_channel(SENSOR6_CH);
	HAL_ADC_Start(&adc_handle);
	HAL_ADC_PollForConversion(&adc_handle, HAL_MAX_DELAY);
	return HAL_ADC_GetValue(&adc_handle);
}

uint16_t get_sensor7_value()
{
	select_adc_channel(SENSOR7_CH);
	HAL_ADC_Start(&adc_handle);
	HAL_ADC_PollForConversion(&adc_handle, HAL_MAX_DELAY);
	return HAL_ADC_GetValue(&adc_handle);
}

uint16_t get_sensor8_value()
{
	select_adc_channel(SENSOR8_CH);
	HAL_ADC_Start(&adc_handle);
	HAL_ADC_PollForConversion(&adc_handle, HAL_MAX_DELAY);
	return HAL_ADC_GetValue(&adc_handle);
}

uint16_t get_sensor9_value()
{
	select_adc_channel(SENSOR9_CH);
	HAL_ADC_Start(&adc_handle);
	HAL_ADC_PollForConversion(&adc_handle, HAL_MAX_DELAY);
	return HAL_ADC_GetValue(&adc_handle);
}

void select_adc_channel(uint32_t sensor_nr)
{
	adc_ch_conf.Channel = sensor_nr;
	HAL_ADC_ConfigChannel(&adc_handle, &adc_ch_conf);
}
