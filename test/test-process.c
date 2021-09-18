// SPDX-License-Identifier: GPL-2.0-or-later
/*
 * Test process for `cpu-usage` script.
 *
 * (c) 2021 Christophe BLAESS <christophe.blaess@logilin.fr>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms and conditions of the GNU General Public License,
 * version 2, as published by the Free Software Foundation.
 *
 * This program is distributed in the hope it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin St - Fifth Floor, Boston, MA 02110-1301 USA.
 *
 */

#include <fcntl.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void *user_cpuhog(void *unused)
{
	while(1)
		;
}


void *semi_user_cpuhog(void *unused)
{
	while (1) {
		for (int i = 0; i < 10000; i ++)
			;
		usleep(50);
	}
}


void *kernel_cpuhog(void *unused)
{
	int fd = open("/dev/null", O_WRONLY);
	char buffer[4096];
	while(1)
		write(fd, buffer, 4096);
}


void *sleeping_thread(void *unused)
{
	while (1)
		sleep(1);
}


int main(void)
{
	pthread_t thr;

	fprintf(stderr, "PID = %d\n", getpid());

	pthread_create(&thr, NULL, user_cpuhog, NULL);
	pthread_create(&thr, NULL, semi_user_cpuhog, NULL);
	pthread_create(&thr, NULL, kernel_cpuhog, NULL);
	pthread_create(&thr, NULL, sleeping_thread, NULL);

	pthread_join(thr, NULL);
	return 0;
}


