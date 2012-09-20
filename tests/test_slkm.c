/*------------------------------------------------------------------
 * test_slkm.c - kernel unit tests for slkm
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

#define DRV_NAME        "test-slkm"
#define DRV_VERSION     "0.0"
#define DRV_DESCRIPTION "Safe C Library Kernel Module Unit Tests"
#define DRV_COPYRIGHT   "Copyright(c) 2012 cisco Systems, Inc."

MODULE_DESCRIPTION(DRV_DESCRIPTION);
MODULE_AUTHOR(DRV_COPYRIGHT);
MODULE_LICENSE("MIT");
MODULE_VERSION(DRV_VERSION);
MODULE_ALIAS("test-slkm");

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

struct test_entry {
	int (*test_func) (void);
	char *name;
};

extern int test_memset_s(void);
extern int test_memcpy_s(void);
extern int test_memmove_s(void);
extern int test_memcmp_s(void);
extern int test_strcpy_s(void);
extern int test_strncpy_s(void);
extern int test_strcat_s(void);
extern int test_strncat_s(void);
extern int test_strcmp_s(void);
extern int test_strcasecmp_s(void);
extern int test_strstr_s(void);
extern int test_strnlen_s(void);
extern int test_strpbrk_s(void);
extern int test_strspn_s(void);
extern int test_strcspn_s(void);


/**
 * @brief Module initialization function.
 *
 * @return 0 on success; < 0 on failure
 */
static int __init testslkm_init(void)
{
	int rc = 0;
	int i  = 0;
	struct test_entry test_vec[] = {
		{ test_memset_s, "test_memset_s" },
		{ test_memcpy_s, "test_memcpy_s" },
		{ test_memmove_s, "test_memmove_s" },
		{ test_memcmp_s, "test_memcmp_s" },
		{ test_strcpy_s, "test_strcpy_s" },
		{ test_strncpy_s, "test_strncpy_s" },
		{ test_strcat_s, "test_strcat_s" },
		{ test_strncat_s, "test_strncat_s" },
		{ test_strcmp_s, "test_strcmp_s" },
		{ test_strcasecmp_s, "test_strcasecmp_s" },
		{ test_strstr_s, "test_strstr_s" },
		{ test_strnlen_s, "test_strnlen_s" },
		{ test_strpbrk_s, "test_strpbrk_s" },
		{ test_strspn_s, "test_strspn_s" },
		{ test_strcspn_s, "test_strcspn_s" }
	};

	printk(KERN_INFO "%s, v%s - %s\n", DRV_DESCRIPTION, DRV_VERSION,
	       DRV_COPYRIGHT);

	for (i = 0; i < ARRAY_SIZE(test_vec); i++) {
		rc = test_vec[i].test_func();
		if (rc)
			printk(KERN_INFO "%s tests failed\n", test_vec[i].name);
	}
	return rc;
}

/**
 * @brief Module exit function.
 */
static void __exit testslkm_exit(void)
{
	printk(KERN_INFO "%s called\n", __func__);
}

/* Module entry and exit points */
module_init(testslkm_init);
module_exit(testslkm_exit);
