/*------------------------------------------------------------------
 * slk_init.c - example kernel module skeleton
 *
 * 2012, Jonathan Toppins <jtoppins@users.sourceforge.net>
 *
 * Copyright (c) 2012 Cisco Systems, Inc.
 * All rights reserved.
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following
 * conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT.  IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *------------------------------------------------------------------
 */

#include <linux/module.h>
#include <linux/kernel.h>
#include "../safeclib/safeclib_private.h"

#define DRV_NAME        "slk"
#define DRV_VERSION     "0.0"
#define DRV_DESCRIPTION "Safe C Library Kernel Module"
#define DRV_COPYRIGHT   "Copyright(c) 2012 cisco Systems, Inc."

MODULE_DESCRIPTION(DRV_DESCRIPTION);
MODULE_AUTHOR(DRV_COPYRIGHT);
MODULE_LICENSE("MIT");
MODULE_VERSION(DRV_VERSION);
MODULE_ALIAS("slk");

/*
 * // example match table... show how to properally mark the table
 *static const struct of_device_id _match[] __devinitconst = {
 *        {
 *                .compatible = "blah",
 *                .data       = (void *) (&_ops),
 *        },
 *        {},
 *};
 *MODULE_DEVICE_TABLE(of, _match);
 */


/**
 * @brief Module initialization function.
 *
 * @return 0 on success; < 0 on failure
 */
static int __init slk_init(void)
{
	printk(KERN_INFO "%s, v%s - %s\n", DRV_DESCRIPTION, DRV_VERSION,
	       DRV_COPYRIGHT);
	(void) set_mem_constraint_handler_s(abort_handler_s);
	(void) set_str_constraint_handler_s(abort_handler_s);
	return 0;
}

/**
 * @brief Module exit function.
 */
static void __exit slk_exit(void)
{
	printk(KERN_INFO "%s called\n", __func__);
}

/* Module entry and exit points */
module_init(slk_init);
module_exit(slk_exit);
