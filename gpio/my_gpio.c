#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/platform_device.h> /* For platform devices */
#include <linux/gpio/consumer.h> /* For GPIO Descriptor */
#include <linux/interrupt.h> /* For IRQ */
#include <linux/of.h> /* For DT*/

static struct gpio_desc *led, *btn;

static const struct of_device_id gpiod_dt_ids[] = {
{ 
    .compatible = "my,gpio-dev", },
    { /* sentinel */ }
};

static int my_pdrv_probe (struct platform_device *pdev)
{
    struct device *dev = &pdev->dev;
    /*
    * We use gpiod_get/gpiod_get_index() along with the flags
    * in order to configure the GPIO direction and an initial
    * value in a single function call.
    *
    * One could have used:
    * red = gpiod_get_index(dev, "led", 0);
    * gpiod_direction_output(red, 0);
    */
    led = gpiod_get_index(dev, "led", 0, GPIOD_OUT_HIGH);
    /*
    * Configure GPIO Buttons as input
    *
    * After this, one can call gpiod_set_debounce()
    * only if the controller has the feature
    * For example, to debounce a button with a delay of 200ms
    * gpiod_set_debounce(btn1, 200);
    */
    btn = gpiod_get_index(dev, "btn", 0, GPIOD_IN);

    pr_info("Hello! device probed!\n");
    return 0;
}

static int my_pdrv_remove(struct platform_device *pdev)
{
    gpiod_put(led);
    gpiod_put(btn);
    pr_info("good bye!\n");

    return 0;
}
static struct platform_driver mypdrv = {
    .probe = my_pdrv_probe,
    .remove = my_pdrv_remove,
    .driver = {
        .name = "my_gpio",
        .of_match_table = of_match_ptr(gpiod_dt_ids),
        .owner = THIS_MODULE,
    },
};

module_platform_driver(mypdrv);
MODULE_AUTHOR("John Madieu <john.madieu@gmail.com>");
MODULE_LICENSE("GPL");