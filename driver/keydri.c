#include <linux/init.h>
#include<linux/module.h>
#include<linux/kernel.h>
#include <linux/platform_device.h>
#include <linux/mutex.h>
#include <linux/errno.h>
#include <linux/slab.h>
#include <linux/fs.h>
//#include <linux/device.h>
#include <linux/cdev.h>
#include <asm/uaccess.h>

#define DRIVER_NAME "aatgc"
static dev_t dev;
struct class *class;
struct cdev cdev;
char * driver_name = DRIVER_NAME;

struct file_operations fops = {
};

static int __init key_driver_init(void)
{
    int major_number = alloc_chrdev_region(&dev, 0, 1, driver_name); /* Proc entry */
    if(major_number < 0)
    {
        pr_alert("major number create failed \n");
	return -1;
    }
    pr_alert("major number obtained is %d \n",major_number);

    class = class_create(THIS_MODULE, driver_name); /* Sys entry */
    if(NULL == class)
    {
        pr_alert("Class create failed \n");
	unregister_chrdev_region(dev, 1);
	return -1;
    }

    if(device_create(class, NULL, dev, NULL, driver_name) == NULL) /* udev device creation */
    {
        pr_alert("device create failed \n");
	class_destroy(class);
	unregister_chrdev_region(dev, 1);
	return -1;
    }
    cdev_init(&cdev, &fops);
    if(cdev_add(&cdev, dev, 1) == -1)
    {
        device_destroy(class, dev);
	class_destroy(class);
	unregister_chrdev_region(dev, 1);
	return -1;
    }

    printk("Key driver init done\n");
    return 0;
}

static void __exit key_driver_exit(void)
{
    cdev_del(&cdev);
    device_destroy(class, dev);
    class_destroy(class);
    unregister_chrdev_region(dev, 1);
    printk("Key driver Exited\n");
}

module_param(driver_name,charp,0);

#if 0
static int omap_kp_probe(struct platform_device *pdev)
{
	struct omap_kp *omap_kp;
	struct input_dev *input_dev;
	struct omap_kp_platform_data *pdata = dev_get_platdata(&pdev->dev);
	int i, col_idx, row_idx, ret;
	unsigned int row_shift, keycodemax;

	if (!pdata->rows || !pdata->cols || !pdata->keymap_data) {
		printk(KERN_ERR "No rows, cols or keymap_data from pdata\n");
		return -EINVAL;
	}

//	row_shift = get_count_order(pdata->cols);
//	keycodemax = pdata->rows << row_shift;

	omap_kp = kzalloc(sizeof(struct omap_kp) +
			keycodemax * sizeof(unsigned short), GFP_KERNEL);
	input_dev = input_allocate_device();
	if (!omap_kp || !input_dev) {
		kfree(omap_kp);
		input_free_device(input_dev);
		return -ENOMEM;
	}

//	platform_set_drvdata(pdev, omap_kp);

	omap_kp->input = input_dev;
#endif
#if 0
	/* Disable the interrupt for the MPUIO keyboard */
	omap_writew(1, OMAP1_MPUIO_BASE + OMAP_MPUIO_KBD_MASKIT);

	if (pdata->delay)
		omap_kp->delay = pdata->delay;

	if (pdata->row_gpios && pdata->col_gpios) {
		row_gpios = pdata->row_gpios;
		col_gpios = pdata->col_gpios;
	}

	omap_kp->rows = pdata->rows;
	omap_kp->cols = pdata->cols;

	col_idx = 0;
	row_idx = 0;

	setup_timer(&omap_kp->timer, omap_kp_timer, (unsigned long)omap_kp);

	/* get the irq and init timer*/
	kp_tasklet.data = (unsigned long) omap_kp;
	tasklet_enable(&kp_tasklet);

	ret = device_create_file(&pdev->dev, &dev_attr_enable);
	if (ret < 0)
		goto err2;

	/* setup input device */
	input_dev->name = "omap-keypad";
	input_dev->phys = "omap-keypad/input0";
	input_dev->dev.parent = &pdev->dev;

	input_dev->id.bustype = BUS_HOST;
	input_dev->id.vendor = 0x0001;
	input_dev->id.product = 0x0001;
	input_dev->id.version = 0x0100;

	if (pdata->rep)
		__set_bit(EV_REP, input_dev->evbit);

	ret = matrix_keypad_build_keymap(pdata->keymap_data, NULL,
					 pdata->rows, pdata->cols,
					 omap_kp->keymap, input_dev);
	if (ret < 0)
		goto err3;

	ret = input_register_device(omap_kp->input);
	if (ret < 0) {
		printk(KERN_ERR "Unable to register omap-keypad input device\n");
		goto err3;
	}

	if (pdata->dbounce)
		omap_writew(0xff, OMAP1_MPUIO_BASE + OMAP_MPUIO_GPIO_DEBOUNCING);

	/* scan current status and enable interrupt */
	omap_kp_scan_keypad(omap_kp, keypad_state);
	omap_kp->irq = platform_get_irq(pdev, 0);
	if (omap_kp->irq >= 0) {
		if (request_irq(omap_kp->irq, omap_kp_interrupt, 0,
				"omap-keypad", omap_kp) < 0)
			goto err4;
	}
	omap_writew(0, OMAP1_MPUIO_BASE + OMAP_MPUIO_KBD_MASKIT);

	return 0;

err4:
	input_unregister_device(omap_kp->input);
	input_dev = NULL;
err3:
	device_remove_file(&pdev->dev, &dev_attr_enable);
err2:
	for (i = row_idx - 1; i >= 0; i--)
		gpio_free(row_gpios[i]);
	for (i = col_idx - 1; i >= 0; i--)
		gpio_free(col_gpios[i]);

	kfree(omap_kp);
	input_free_device(input_dev);

	return -EINVAL;
    return 0;
}
#endif


module_init(key_driver_init);
module_exit(key_driver_exit);

MODULE_VERSION("0.1");
MODULE_DESCRIPTION("Key driver");
MODULE_AUTHOR("AATGC");
MODULE_LICENSE("GPL");

