#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/prctl.h>
#include <pthread.h>
#include <rttshell.h>

#include "gpio.h"

/*
 function: gpio trig led blink
 gpio_num: which gpio to light
*/
static int gpio_blink(unsigned int gpio_num)
{
    int status;
    int toggle = 0;

    prctl(PR_SET_NAME, "gpio_blink");
    printf("[gpio_demo] Testing gpio %d for %s\n", gpio_num, "output");
    status = gpio_request(gpio_num);  /* / tab key for 8 char */
    if (status < 0)
    {
        printf("[gpio_demo] ERROR can not open GPIO %d\n", gpio_num);
        return status;
    }

    gpio_direction_output(gpio_num, 0);

    toggle = gpio_get_value(gpio_num);

    while (1)
    {
        toggle = !(toggle);

        gpio_set_value(gpio_num, toggle);

        usleep(100);

        if (gpio_get_value(gpio_num) != toggle)
        {
            return -1;
        }
    }
    return 0;
}

static void *gpio_blink_main(void *parameter) { gpio_blink(7); return NULL; }
/*
 function: gpio trig led blink
 gpio_num: input gpio
 gpio_num_out: output gpio to light led
 */
static int gpio_light(int gpio_num, int gpio_num_out)
{
    int ret          = 0;
    int PreValue     = 0;
    int CurrentValue = 0;
    int status;

    prctl(PR_SET_NAME, "gpio_light");
    printf("[gpio_demo] Testing gpio %d for %s\n", gpio_num, "output");

    status = gpio_request(gpio_num);
    if (status < 0)
    {
        printf("[gpio_demo] ERROR can not open GPIO %d\n", gpio_num);
        return status;
    }

    status = gpio_request(gpio_num_out);
    if (status < 0)
    {
        printf("[gpio_demo] ERROR can not open GPIO %d\n", gpio_num_out);
        return status;
    }

    gpio_direction_input(gpio_num);

    gpio_direction_output(gpio_num_out, 0);

    while (1)
    {
        CurrentValue = gpio_get_value(gpio_num);

        usleep(50);

        if (CurrentValue == PreValue)
            continue;

        gpio_set_value(gpio_num_out, CurrentValue);

        PreValue = CurrentValue;
    }
    return ret;
}

static void *gpio_light_main(void *parameter) { gpio_light(6, 5); return NULL; }
int gpio_demo_init(void)
{
    int ret;
    pthread_t threadBlink;
    pthread_t threadLight;
    pthread_attr_t Battr;
    pthread_attr_t Lattr;

    pthread_attr_init(&Battr);
    pthread_attr_setdetachstate(&Battr, PTHREAD_CREATE_DETACHED);
    pthread_attr_setstacksize(&Battr, 10 * 1024);

    ret = pthread_create(&threadBlink, &Battr, gpio_blink_main, NULL);
    if(ret)
    {
        printf("[gpio_demo] Error: Create gpio_blink_main thread failed!\n");
        return -1;
    }

    pthread_attr_init(&Lattr);
    pthread_attr_setdetachstate(&Lattr, PTHREAD_CREATE_DETACHED);
    pthread_attr_setstacksize(&Lattr, 10 * 1024);

    ret = pthread_create(&threadLight, &Lattr, gpio_light_main, NULL);
    if(ret)
    {
        printf("[gpio_demo] Error: Create gpio_light_main thread failed!\n");
        return -1;
    }

    return 0;
}
