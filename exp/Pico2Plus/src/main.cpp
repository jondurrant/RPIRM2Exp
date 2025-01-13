/***
 * main.cpp -
 * Test with RM2 Module and PIMoroni Pico 2 Plus
 * Jon Durrant
 * Jan-2025
 *
 *
 */

#include "pico/stdlib.h"
#include <stdio.h>

#include "FreeRTOS.h"
#include "task.h"
#include <stdio.h>

#include "BlinkAgent.h"
#include "WifiHelper.h"
#include "Request.h"




#define TASK_PRIORITY     ( tskIDLE_PRIORITY + 1UL )

#define BUF_LEN 1024

void main_task(void* params){

	printf("Main task started\n");

	BlinkAgent blink(PICO_DEFAULT_LED_PIN);

	blink.start("Blink", TASK_PRIORITY);

	if (!WifiHelper::init()){
		printf("CYW43 failed\n");
	} else {
		if (!WifiHelper::join(
				WIFI_SSID,
				WIFI_PASSWORD
				)){
			printf("Wifi Join failed\n");
		} else {
			char ip[20];
			WifiHelper::getIPAddressStr(ip);
			printf("IP address:  %s\n", ip);

			char url[] =  "http://vmu22a.local.jondurrant.com:5000/time";
			char userBuf[BUF_LEN];
			Request req((char *)userBuf, BUF_LEN);
			bool res;
			res = req.get(url);
			if ( res ){
				res = (req.getStatusCode() == 200);
			}
			if (res){
				printf("Time: %.*s\n", req.getPayloadLen(), req.getPayload());
			} else {
				printf("Status %d\n", req.getStatusCode());
			}

		}
	}

	for(;;){
		vTaskDelay(3000);

	}

}


void vLaunch(void) {
  TaskHandle_t task;

  xTaskCreate(main_task, "MainThread", 10000, NULL, TASK_PRIORITY, &task);

  /* Start the tasks and timer running. */
  vTaskStartScheduler();
}


int main(void) {
	bool ok= false;

    stdio_init_all();
    sleep_ms(2000);

  printf("GO\n");

  /* Configure the hardware ready to run the demo. */
  const char* rtos_name;
  rtos_name = "FreeRTOS";
  printf("Starting %s on core 0:\n", rtos_name);
  vLaunch();

  return 0;
}
