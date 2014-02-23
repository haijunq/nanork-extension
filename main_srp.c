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
 *  Contributing Authors (specific to this file):
 *  Zane Starr
 *******************************************************************************/

#include <nrk.h>
#include <include.h>
#include <ulib.h>
#include <stdio.h>
#include <avr/sleep.h>
#include <hal.h>
#include <nrk_defs.h>
#include <nrk_task.h>
#include <nrk_error.h>
#include <nrk_events.h>
#include <nrk_timer.h>
#include <nrk_stats.h>
#include <nrk_stack_check.h>

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
//void Task4(void);

void nrk_create_taskset();

nrk_sem_t *sem1;
//nrk_sem_t *sem2;

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

//	sem1 = nrk_sem_create(1, 10);
	nrk_sem_list[0].value = 1;
	nrk_sem_list[0].count = 1;
	nrk_sem_list[0].resource_ceiling = 5;
	sem1 = nrk_sem_list;

	if (sem1 == NULL)
		printf("Creating sem error\n");
	nrk_start();

	return 0;
}

void Task1() {
	uint16_t cnt;
	int8_t v;

	cnt = 0;
	while (1) {
		nrk_led_toggle(ORANGE_LED);
//		printf("Task1 cnt=%d\r\n", cnt);
//		nrk_kprintf(PSTR("Task1 accessing semaphore\r\n"));
//		printf("T1_next_period = %u \n", nrk_cur_task_TCB->next_period);
		v = nrk_sem_pend(sem1);
		if (v == NRK_ERROR)
			nrk_kprintf(PSTR("T1 error pend\r\n"));
//		nrk_kprintf(PSTR("Task1 holding semaphore\r\n"));
//		printf("In T1 sys_ceiling = %u \n", system_ceiling);
		printf("In T1 Q --- ");
		nrk_print_readyQ();
		printf("\n");
		// to something within the critical section
		nrk_wait_until_next_period();
		v = nrk_sem_post(sem1);
		if (v == NRK_ERROR)
			nrk_kprintf(PSTR("T1 error post\r\n"));
//		nrk_kprintf(PSTR("Task1 released semaphore\r\n"));
		nrk_wait_until_next_period();

		if (cnt == 3)
			break;
		cnt++;
	}
	nrk_stats_display_all();
}

void Task2() {
	uint8_t cnt;
	int8_t v;

	cnt = 0;
	while (1) {
		nrk_led_toggle(BLUE_LED);
//		printf("Task2 cnt=%d\r\n", cnt);
//		nrk_kprintf(PSTR("Task2 accessing semaphore\r\n"));
//		printf("T2_next_period = %u \n", nrk_cur_task_TCB->next_period);

		v = nrk_sem_pend(sem1);

		if (v == NRK_ERROR)
			nrk_kprintf(PSTR("T2 error pend\r\n"));
//		nrk_kprintf(PSTR("Task2 holding semaphore\r\n"));
//		printf("In T2 sys_ceiling = %u \n", system_ceiling);
		printf("In T2 Q --- ");
		nrk_print_readyQ();
		printf("\n");
		// to something within the critical section
		nrk_wait_until_next_period();
		v = nrk_sem_post(sem1);
		if (v == NRK_ERROR)
			nrk_kprintf(PSTR("T2 error post\r\n"));
//		nrk_kprintf(PSTR("Task2 released semaphore\r\n"));
		nrk_wait_until_next_period();

		cnt++;
	}

}

void Task3() {
	uint8_t cnt;
	cnt = 0;
	while (1) {
		nrk_led_toggle(GREEN_LED);
//		nrk_sem_pend(sem1);
//		printf("In T3 sys_ceiling = %u \n", system_ceiling);
		printf("In T3 Q --- ");
		nrk_print_readyQ();
		printf("\n");
		nrk_wait_until_next_period();
//		nrk_sem_post(sem1);
		nrk_wait_until_next_period();
		cnt++;
	}

}

void Task4() {
	uint8_t cnt;
	cnt = 0;
	while (1) {
		nrk_led_toggle(GREEN_LED);
		printf("In T4 sys_ceiling = %u \n", system_ceiling);
		printf("In T4 Q --- ");
		nrk_print_readyQ();
		printf("\n");
		nrk_wait_until_next_period();
		cnt++;
	}
}

void nrk_create_taskset() {
	TaskOne.task = Task1;
	TaskOne.Ptos = (void *) &Stack1[NRK_APP_STACKSIZE];
	TaskOne.Pbos = (void *) &Stack1[0];
	TaskOne.prio = 10; // preemption level, inversely proportional to the period
	TaskOne.FirstActivation = TRUE;
	TaskOne.Type = SRP_TASK;
	TaskOne.SchType = PREEMPTIVE;
	TaskOne.period.secs = 2;
	TaskOne.period.nano_secs = 1 * NANOS_PER_MS;
	TaskOne.cpu_reserve.secs = 1;
	TaskOne.cpu_reserve.nano_secs = 0;
	TaskOne.offset.secs = 0;
	TaskOne.offset.nano_secs = 999 * NANOS_PER_MS;
	nrk_activate_task(&TaskOne);

	TaskTwo.task = Task2;
	TaskTwo.Ptos = (void *) &Stack2[NRK_APP_STACKSIZE];
	TaskTwo.Pbos = (void *) &Stack2[0];
	TaskTwo.prio = 15;
	TaskTwo.FirstActivation = TRUE;
	TaskTwo.Type = SRP_TASK;
	TaskTwo.SchType = PREEMPTIVE;
	TaskTwo.period.secs = 2;
	TaskTwo.period.nano_secs = 0;
	TaskTwo.cpu_reserve.secs = 1;
	TaskTwo.cpu_reserve.nano_secs = 0;
	TaskTwo.offset.secs = 1;
	TaskTwo.offset.nano_secs = 0;
	nrk_activate_task(&TaskTwo);

	TaskThree.task = Task3;
	TaskThree.Ptos = (void *) &Stack3[NRK_APP_STACKSIZE];
	TaskThree.Pbos = (void *) &Stack3[0];
	TaskThree.prio = 5;
	TaskThree.FirstActivation = TRUE;
	TaskThree.Type = SRP_TASK;
	TaskThree.SchType = PREEMPTIVE;
	TaskThree.period.secs = 1;
	TaskThree.period.nano_secs = 999 * NANOS_PER_MS;
	TaskThree.cpu_reserve.secs = 1;
	TaskThree.cpu_reserve.nano_secs = 0;
	TaskThree.offset.secs = 1;
	TaskThree.offset.nano_secs = 1 * NANOS_PER_MS;
	nrk_activate_task(&TaskThree);

//	TaskFour.task = Task4;
//	TaskFour.Ptos = (void *) &Stack4[NRK_APP_STACKSIZE];
//	TaskFour.Pbos = (void *) &Stack4[0];
//	TaskFour.prio = 1;
//	TaskFour.FirstActivation = TRUE;
//	TaskFour.Type = SRP_TASK;
//	TaskFour.SchType = PREEMPTIVE;
//	TaskFour.period.secs = 2;
//	TaskFour.period.nano_secs = 0;
//	TaskFour.cpu_reserve.secs = 0;
//	TaskFour.cpu_reserve.nano_secs = 50 * NANOS_PER_MS;
//	TaskFour.offset.secs = 1;
//	TaskFour.offset.nano_secs = 0;
//	nrk_activate_task(&TaskFour);
}

