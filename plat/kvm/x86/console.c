/* SPDX-License-Identifier: ISC */
/*
 * Authors: Dan Williams
 *          Martin Lucina
 *          Felipe Huici <felipe.huici@neclab.eu>
 *          Florian Schmidt <florian.schmidt@neclab.eu>
 *          Simon Kuenzer <simon.kuenzer@neclab.eu>
 *
 * Copyright (c) 2015-2017 IBM
 * Copyright (c) 2016-2017 Docker, Inc.
 * Copyright (c) 2017 NEC Europe Ltd., NEC Corporation
 *
 * Permission to use, copy, modify, and/or distribute this software
 * for any purpose with or without fee is hereby granted, provided
 * that the above copyright notice and this permission notice appear
 * in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL
 * WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE
 * AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR
 * CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS
 * OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT,
 * NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN
 * CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include <kvm/console.h>
#include <kvm-x86/cpu_x86_64.h>
#include <uk/plat/console.h>
#include <uk/essentials.h>
#include <uk/print.h>

#define COM1 0x3f8

#define COM1_DATA (COM1 + 0)
#define COM1_INTR (COM1 + 1)
#define COM1_CTRL (COM1 + 3)
#define COM1_STATUS (COM1 + 5)

/* only when DLAB is set */
#define COM1_DIV_LO (COM1 + 0)
#define COM1_DIV_HI (COM1 + 1)

#define DLAB 0x80
#define PROT 0x03 /* 8N1 (8 bits, no parity, one stop bit) */

void _libkvmplat_init_console(void)
{
	uk_printd(DLVL_INFO, "initializing serial\n");
	outb(COM1_INTR, 0x00);  /* Disable all interrupts */
	outb(COM1_CTRL, DLAB);  /* Enable DLAB (set baudrate divisor) */
	outb(COM1_DIV_LO, 0x01);/* Set div to 1 (lo byte) 115200 baud */
	outb(COM1_DIV_HI, 0x00);/*              (hi byte) */
	outb(COM1_CTRL, PROT);  /* Set 8N1, clear DLAB */
}

int ukplat_coutd(const char *str, unsigned int len)
{
	return ukplat_coutk(str, len);
}

static int serial_tx_empty(void)
{
	return inb(COM1_STATUS) & 0x20;
}

static void serial_write(char a)
{
	while (!serial_tx_empty())
		;

	outb(COM1_DATA, a);
}

static void serial_putc(char a)
{
	if (a == '\n')
		serial_write('\r');
	serial_write(a);
}

static int serial_rx_ready(void)
{
	return inb(COM1_STATUS) & 0x01;
}

static int serial_getc(void)
{
	if (!serial_rx_ready())
		return -1;
	return (int) inb(COM1_DATA);
}

int ukplat_coutk(const char *buf, unsigned int len)
{
	for (unsigned int i = 0; i < len; i++)
		serial_putc(buf[i]);
	return len;
}

int ukplat_cink(char *buf, unsigned int maxlen)
{
	int ret;
	unsigned int num = 0;

	while (num < maxlen
	       && (ret = serial_getc()) >= 0) {
		*(buf++) = (char) ret;
		num++;
	}

	return (int) num;
}
