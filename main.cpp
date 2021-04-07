#include <ios>
#include <stdio.h>
#include <string>
#include <string.h>
#include <limits>
#include <iostream>
#include <memory>

#include "pico/bootrom.h"
#include "pico/stdlib.h"
#include "pico/multicore.h"
#include "pico/time.h"
#include "hardware/adc.h"
#include "hardware/gpio.h"
#include "hardware/irq.h"
#include "hardware/watchdog.h"

#include <type_expr.hpp>
#include "jsch/jsch.hpp"


constexpr uint LED_PIN = PICO_DEFAULT_LED_PIN;



bool alarm_blink(struct repeating_timer * t)
{
	static bool blinking = false;
	gpio_put(LED_PIN, blinking);
	blinking = !blinking;
	return true;
}

bool GPIO26_blink(struct repeating_timer * t)
{
	static bool blinking = false;
	gpio_put(22, blinking);
	blinking = !blinking;
	return true;
}


int blocking_cmd_parse();

constexpr auto timer_deleter = [](repeating_timer* t){cancel_repeating_timer(t);};

std::unique_ptr<repeating_timer,decltype(timer_deleter)> timer_core0 (new repeating_timer(),timer_deleter);
std::unique_ptr<repeating_timer,decltype(timer_deleter)> timer_core1 (new repeating_timer(),timer_deleter);



int main() {
	stdio_init_all();

	gpio_init(LED_PIN);
	gpio_set_dir(LED_PIN, GPIO_OUT);
	gpio_init(22);
	gpio_set_dir(22,GPIO_OUT);

	multicore_launch_core1([](){
		add_repeating_timer_ms(500,alarm_blink,NULL,timer_core1.get());
		while(true)
			tight_loop_contents();
	});


	adc_init();
	adc_gpio_init(26);
	adc_set_temp_sensor_enabled(true);
	add_repeating_timer_ms(
		1000,
		[]( repeating_timer * t)
		{
			constexpr float cf=3.3f/(1<<12);
			adc_select_input(4);
			uint16_t voltage =adc_read();
			float temp_c = 27.0f -(voltage * cf - 0.706)/0.001721;
			printf("Voltage[%f] Temp[%f oC]\n", voltage * cf, temp_c);
			return true;
		},NULL,timer_core0.get());

	blocking_cmd_parse();
}

int blocking_cmd_parse()
{
	std::string cmd ;
	while (1)
	{
		auto c = getchar();
		switch(c)
		{
			case '\n':
			case '\r':
			{
		
				if(0 == strcmp(cmd.c_str(), "reboot"))
				{
					std::cout << "Rebooting" << std::endl;
					watchdog_enable(100, 1);
					watchdog_update();
				}
				else if(0 == cmd.compare("exit"))
				{
					std::cout << "Quitting" << std::endl;
					return 0;
				}
				else if(cmd.compare(0,4,"echo") == 0 )
				{
					std::cout << cmd << std::endl;
				}
				cmd.clear();
				break;
			}
			case 0x08: // Backspace
				cmd.pop_back();
				break;
			default:
				cmd += c;
		}
	}
}
