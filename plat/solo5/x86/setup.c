/* SPDX-License-Identifier: ISC */
/*
 * Authors: Dan Williams
 *          Martin Lucina
 *          Ricardo Koller
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

#include <string.h>
#include <solo5.h>
#include <setup.h>
#include <uk/arch/limits.h>
#include <uk/plat/console.h>
#include <uk/assert.h>
#include <uk/essentials.h>

#define MAX_CMDLINE_SIZE 8192

struct libsolo5plat_opts _libsolo5plat_opts = { 0 };

int solo5_app_main(const struct solo5_start_info *si) {
	uk_printd(DLVL_INFO, "Entering from SOLO5 (x86)...\n");

	_libsolo5plat_opts.heap.len  = si->heap_size;
	_libsolo5plat_opts.heap.base = (void*)(si->heap_start);

	ukplat_entry_argp(NULL, si->cmdline, MAX_CMDLINE_SIZE);

	return SOLO5_EXIT_SUCCESS;
}
