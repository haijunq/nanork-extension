/******************************************************************************
 *  Nano-RK, a real-time operating system for sensor networks.
 *  Copyright (C) 2007, Real-Time and Multimedia Lab, Carnegie Mellon University
 *  All rights reserved.
 *
 *  This is the Open Source Version of Nano-RK included as part of a Dual
 *  Licensing Model. If you are unsure which license to use please refer to:
 *  http://www.nanork.org/nano-RK/wiki/Licensing
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, version 2.0 of the License.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 *******************************************************************************/

#include <nrk.h>
#include <include.h>
#include <ulib.h>
#include <stdio.h>
#include <hal.h>
#include <nrk_error.h>
#include <nrk_timer.h>
#include <nrk_stack_check.h>
#include <nrk_stats.h>
#include <nrk_defs.h>

NRK_STK Stack1[NRK_APP_STACKSIZE];
nrk_task_type TaskOne;
void Task1(void);

NRK_STK Stack2[NRK_APP_STACKSIZE];
nrk_task_type TaskTwo;
void Task2(void);

NRK_STK Stack3[NRK_APP_STACKSIZE];
nrk_task_type TaskThree;
void Task3(void);

//NRK_STK Stack4[NRK_APP_STACKSIZE];
//nrk_task_type TaskFour;
//void Task4 (void);

void nrk_create_taskset();

int main() {
	nrk_setup_ports();
	nrk_setup_uart(UART_BAUDRATE_115K2);

	nrk_init();

	nrk_led_clr(ORANGE_LED);
	nrk_led_clr(BLUE_LED);
	nrk_led_clr(GREEN_LED);
	nrk_led_clr(RED_LED);

	nrk_time_set(0, 0);
	nrk_create_taskset();
	nrk_stats_reset();
	nrk_start();

	return 0;
}

void Task1() {
	uint16_t cnt;
	cnt = 0;
	while (1) {
		nrk_led_toggle(ORANGE_LED);
//		printf("T1_next_period\t[%u]\n", nrk_cur_task_TCB->next_period);
//		printf("T1_cpu_remain\t[%u]\n", nrk_cur_task_TCB->cpu_remaining);
		if (cnt == 5) {
			break;
		}
		printf("In T1 --- ");
		nrk_print_readyQ();
		printf("\n");
		nrk_wait_until_next_period();
		cnt++;
	}
	nrk_stats_display_all();
}

void Task2() {
	int16_t cnt;
	cnt = 0;
	while (1) {
		nrk_led_toggle(BLUE_LED);
//		printf("T2_next_period\t[%u]\n", nrk_cur_task_TCB->next_period);
//		printf("T2_cpu_remain\t[%u]\n", nrk_cur_task_TCB->cpu_remaining);
		printf("In T2 --- ");
		nrk_print_readyQ();
		printf("\n");
		nrk_wait_until_next_period();
		cnt++;
	}
}

void Task3() {
	uint16_t cnt;
	cnt = 0;
	while (1) {
		nrk_led_toggle(GREEN_LED);
//		printf("T3_next_period\t[%u]\n", nrk_cur_task_TCB->next_period);
		printf("In T3 --- ");
		nrk_print_readyQ();
		printf("\n");
		nrk_wait_until_next_period();
		cnt++;
	}
}

void Task4() {
	uint16_t cnt;
	cnt = 0;
	while (1) {
		nrk_led_toggle(RED_LED);
		printf("T4_next_period\t[%u]\n", nrk_cur_task_TCB->next_period);
		cnt++;
	}
}

void nrk_create_taskset() {
	nrk_task_set_entry_function(&TaskOne, Task1);
	nrk_task_set_stk(&TaskOne, Stack1, NRK_APP_STACKSIZE);
	TaskOne.prio = 63;
	TaskOne.FirstActivation = TRUE;
	TaskOne.Type = BASIC_TASK;
	TaskOne.SchType = PREEMPTIVE;
	TaskOne.period.secs = 2;
	TaskOne.period.nano_secs = 0;
	TaskOne.cpu_reserve.secs = 1;
	TaskOne.cpu_reserve.nano_secs = 0;
	TaskOne.offset.secs = 1;
	TaskOne.offset.nano_secs = 1 * NANOS_PER_MS;
	nrk_activate_task(&TaskOne);

	nrk_task_set_entry_function(&TaskTwo, Task2);
	nrk_task_set_stk(&TaskTwo, Stack2, NRK_APP_STACKSIZE);
	TaskTwo.prio = 63;
	TaskTwo.FirstActivation = TRUE;
	TaskTwo.Type = BASIC_TASK;
	TaskTwo.SchType = PREEMPTIVE;
	TaskTwo.period.secs = 2;
	TaskTwo.period.nano_secs = 2 * NANOS_PER_MS;
	TaskTwo.cpu_reserve.secs = 1;
	TaskTwo.cpu_reserve.nano_secs = 0;
	TaskTwo.offset.secs = 1;
	TaskTwo.offset.nano_secs = 0 * NANOS_PER_MS;
	nrk_activate_task(&TaskTwo);

	nrk_task_set_entry_function(&TaskThree, Task3);
	nrk_task_set_stk(&TaskThree, Stack3, NRK_APP_STACKSIZE);
	TaskThree.prio = 63;
	TaskThree.FirstActivation = TRUE;
	TaskThree.Type = BASIC_TASK;
	TaskThree.SchType = PREEMPTIVE;
	TaskThree.period.secs = 2;
	TaskThree.period.nano_secs = 4 * NANOS_PER_MS;
	TaskThree.cpu_reserve.secs = 1;
	TaskThree.cpu_reserve.nano_secs = 0 * NANOS_PER_MS;
	TaskThree.offset.secs = 0;
	TaskThree.offset.nano_secs = 999 * NANOS_PER_MS;
	nrk_activate_task(&TaskThree);

	//===============================================

//	nrk_task_set_entry_function( &TaskOne, Task1);
//	nrk_task_set_stk( &TaskOne, Stack1, NRK_APP_STACKSIZE);
//	TaskOne.prio = 63;
//	TaskOne.FirstActivation = TRUE;
//	TaskOne.Type = BASIC_TASK;
//	TaskOne.SchType = PREEMPTIVE;
//	TaskOne.period.secs = 2;
//	TaskOne.period.nano_secs = 0;
//	TaskOne.cpu_reserve.secs = 1;
//	TaskOne.cpu_reserve.nano_secs = 0;
//	TaskOne.offset.secs = 1;
//	TaskOne.offset.nano_secs= 0;
//	nrk_activate_task (&TaskOne);
//
//	nrk_task_set_entry_function( &TaskTwo, Task2);
//	nrk_task_set_stk( &TaskTwo, Stack2, NRK_APP_STACKSIZE);
//	TaskTwo.prio = 63;
//	TaskTwo.FirstActivation = TRUE;
//	TaskTwo.Type = BASIC_TASK;
//	TaskTwo.SchType = PREEMPTIVE;
//	TaskTwo.period.secs = 4;
//	TaskTwo.period.nano_secs = 0;
//	TaskTwo.cpu_reserve.secs = 2;
//	TaskTwo.cpu_reserve.nano_secs = 0;
//	TaskTwo.offset.secs = 1;
//	TaskTwo.offset.nano_secs= 0;
//	nrk_activate_task (&TaskTwo);

//	nrk_task_set_entry_function( &TaskThree, Task3);
//	nrk_task_set_stk( &TaskThree, Stack3, NRK_APP_STACKSIZE);
//	TaskThree.prio = 63;
//	TaskThree.FirstActivation = TRUE;
//	TaskThree.Type = BASIC_TASK;
//	TaskThree.SchType = PREEMPTIVE;
//	TaskThree.period.secs = 8;
//	TaskThree.period.nano_secs = 0*NANOS_PER_MS;;
//	TaskThree.cpu_reserve.secs = 1;
//	TaskThree.cpu_reserve.nano_secs = 0*NANOS_PER_MS;
//	TaskThree.offset.secs = 1;
//	TaskThree.offset.nano_secs= 0;
//	nrk_activate_task (&TaskThree);
//
//
//	nrk_task_set_entry_function( &TaskFour, Task4);
//	nrk_task_set_stk( &TaskFour, Stack4, NRK_APP_STACKSIZE);
//	TaskFour.prio = 63;
//	TaskFour.FirstActivation = TRUE;
//	TaskFour.Type = BASIC_TASK;
//	TaskFour.SchType = PREEMPTIVE;
//	TaskFour.period.secs = 10;
//	TaskFour.period.nano_secs = 0;
//	TaskFour.cpu_reserve.secs = 2;
//	TaskFour.cpu_reserve.nano_secs = 0*NANOS_PER_MS;
//	TaskFour.offset.secs = 1;
//	TaskFour.offset.nano_secs= 0;
//	nrk_activate_task (&TaskFour);

}

uint8_t kill_stack(uint8_t val) {
	char bad_memory[10];
	uint8_t i;
	for (i = 0; i < 10; i++)
		bad_memory[i] = i;
	for (i = 0; i < 10; i++)
		printf("%d ", bad_memory[i]);
	printf("Die Stack %d\r\n", val);
	if (val > 1)
		kill_stack(val - 1);
	return 0;
}

