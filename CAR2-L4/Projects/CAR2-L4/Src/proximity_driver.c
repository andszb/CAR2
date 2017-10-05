#include "proximity_driver.h"
#include "cmsis_os.h"
#include "motor_control.h"

//#define DEBUG_MODE

uint32_t proxim1_cntr = 0;
uint32_t proxim2_cntr = 0;
int8_t proxim_flag = 0;
uint32_t cm_cntr = 0;
//extern int16_t required_rpm;


void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin);


void proximity1_send_trigger();
void proximity2_send_trigger();


void proximity1_send_trigger()
{
	/* @todo: for test on car use D3 (PB0) */
	//send trigger pin to D3 (PB0)
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET);
	osDelay(1);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET);

	/* @todo: for test on test panel use D2 (PD14) */
	/*HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, GPIO_PIN_SET);
	osDelay(1);
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, GPIO_PIN_RESET);*/

#ifdef DEBUG_MODE
	printf("Proxim trigger sent.\n");
#endif
	osDelay(1);
}

void proximity2_send_trigger()
{
	//send trigger pin to D2 (PD14)
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, GPIO_PIN_SET);
	osDelay(1);
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, GPIO_PIN_RESET);
#ifdef DEBUG_MODE
	printf("Proxim trigger sent.\n");
#endif
	osDelay(1);
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	if (GPIO_Pin == GPIO_PIN_5) {
		if (proxim_flag == 0) {
			HAL_NVIC_DisableIRQ(TIM4_IRQn);
			proxim_flag = 1;
#ifdef DEBUG_MODE
				printf("up ");
#endif
		} else if (proxim_flag == 1) {
			HAL_NVIC_EnableIRQ(TIM4_IRQn);
			proxim_flag = 0;
#ifdef DEBUG_MODE
				printf("down\n");
#endif
		}
	}
}

uint32_t read_proximity_data()
{
	uint8_t measure_failed = 0;
	uint32_t sum = 0;
	uint8_t error_filter = 5;

	for (int i = 0; i < error_filter; i++) {
		cm_cntr = 0;
		proxim1_cntr = 0;
		proxim_flag = 1;
		proximity1_send_trigger();

		while (proxim_flag == 0){
#ifdef DEBUG_MODE
			printf("interrupt 1.\n");
#endif
			HAL_Delay(3);
		}
		proxim1_cntr = cm_cntr;
#ifdef DEBUG_MODE
		printf("proxim1_cntr: %lu", proxim1_cntr);
#endif


		cm_cntr = 0;
		proxim2_cntr = 0;
		proxim_flag = 1;
		proximity2_send_trigger();

		while (proxim_flag == 0){
#ifdef DEBUG_MODE
			printf("interrupt 2.\n");
#endif
			HAL_Delay(3);
		}
		proxim2_cntr = cm_cntr;
#ifdef DEBUG_MODE
		printf("proxim2_cntr: %lu - \n", proxim2_cntr);
#endif

		if ((proxim1_cntr > 600) || (proxim2_cntr > 600)) {
			//measure failure
			measure_failed++;

		} else if ((proxim1_cntr <= 600) && (proxim2_cntr <= 600)) {
			sum = sum + (proxim1_cntr + proxim2_cntr);
		}
	}

	distance = sum / (2 * (error_filter - measure_failed));

#ifdef DEBUG_MODE
	printf("distance: %lu, failure: %d\n\n", distance, measure_failed);
#endif

	return distance;
}

uint8_t process_proximity(uint32_t distance)
{
	object_flag = 0;
	if ((distance < 30) && (distance > 8)) {
//		stop();
		object_flag = 40;

#ifdef DEBUG_MODE
		printf("Disable signal sent.\n");
#endif

		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_RESET); 	//green led
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_SET);		//red led

	} else if ((distance < 50) && (distance >= 30)) {
//		stop();
		object_flag = 40;

#ifdef DEBUG_MODE
		printf("Stop signal sent.\n");
#endif

		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_RESET); 	//green led
		HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_6);					//red led

	} else if ((distance < 150)  && (distance >= 50)) {
//		decelerate();
		object_flag = 20;

#ifdef DEBUG_MODE
		printf("Decelerate signal sent.\n");
#endif

		HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_7); 	//green led
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_RESET);	//red led

	} else {
//		go();
		object_flag = 10;
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_SET);		//green led
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_RESET);	//red led
	}

	return 0;
}

int8_t proximity_control_thread()
{
	while (1){

		uint32_t measured_distance = read_proximity_data();
		process_proximity(measured_distance);

	}

	while (1)
		osThreadTerminate(NULL);

	return 0;
}
