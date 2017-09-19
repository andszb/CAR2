#include "adc_driver.h"
#include "system_init.h"

extern ADC_HandleTypeDef sensor1_handle;
extern ADC_HandleTypeDef sensor2_handle;
extern ADC_HandleTypeDef sensor3_handle;
extern ADC_HandleTypeDef sensor4_handle;
extern ADC_HandleTypeDef sensor5_handle;
extern ADC_HandleTypeDef sensor6_handle;
extern ADC_HandleTypeDef sensor7_handle;
extern ADC_HandleTypeDef sensor8_handle;
extern ADC_HandleTypeDef sensor9_handle;
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



uint16_t get_sensor1_value();
uint16_t get_sensor2_value();
uint16_t get_sensor3_value();
uint16_t get_sensor4_value();
uint16_t get_sensor5_value();
uint16_t get_sensor6_value();
uint16_t get_sensor7_value();
uint16_t get_sensor8_value();
uint16_t get_sensor9_value();
sensor_data_t get_sensor_data();




sensor_data_t get_sensor_data()
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

	return sensor_data;
}

uint16_t get_sensor1_value()
{
	HAL_ADC_Start(&sensor1_handle);
	HAL_ADC_PollForConversion(&sensor1_handle, HAL_MAX_DELAY);
	return HAL_ADC_GetValue(&sensor1_handle);
}

uint16_t get_sensor2_value()
{
	HAL_ADC_Start(&sensor2_handle);
	HAL_ADC_PollForConversion(&sensor2_handle, HAL_MAX_DELAY);
	return HAL_ADC_GetValue(&sensor2_handle);
}

uint16_t get_sensor3_value()
{
	HAL_ADC_Start(&sensor3_handle);
	HAL_ADC_PollForConversion(&sensor3_handle, HAL_MAX_DELAY);
	return HAL_ADC_GetValue(&sensor3_handle);
}

uint16_t get_sensor4_value()
{
	HAL_ADC_Start(&sensor4_handle);
	HAL_ADC_PollForConversion(&sensor4_handle, HAL_MAX_DELAY);
	return HAL_ADC_GetValue(&sensor4_handle);
}

uint16_t get_sensor5_value()
{
	HAL_ADC_Start(&sensor5_handle);
	HAL_ADC_PollForConversion(&sensor5_handle, HAL_MAX_DELAY);
	return HAL_ADC_GetValue(&sensor5_handle);
}

uint16_t get_sensor6_value()
{
	HAL_ADC_Start(&sensor6_handle);
	HAL_ADC_PollForConversion(&sensor6_handle, HAL_MAX_DELAY);
	return HAL_ADC_GetValue(&sensor6_handle);
}

uint16_t get_sensor7_value()
{
	HAL_ADC_Start(&sensor7_handle);
	HAL_ADC_PollForConversion(&sensor7_handle, HAL_MAX_DELAY);
	return HAL_ADC_GetValue(&sensor7_handle);
}

uint16_t get_sensor8_value()
{
	HAL_ADC_Start(&sensor8_handle);
	HAL_ADC_PollForConversion(&sensor8_handle, HAL_MAX_DELAY);
	return HAL_ADC_GetValue(&sensor8_handle);
}

uint16_t get_sensor9_value()
{
	HAL_ADC_Start(&sensor9_handle);
	HAL_ADC_PollForConversion(&sensor9_handle, HAL_MAX_DELAY);
	return HAL_ADC_GetValue(&sensor9_handle);
}
