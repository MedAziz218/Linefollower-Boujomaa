#define ENABLE_GPIO_VIEWER 0
#ifdef ENABLE_GPIO_VIEWER
#include <gpio_viewer.h>
GPIOViewer gpio_viewer;
void setupGPIOViewer(void)
{
    gpio_viewer.connectToWifi("TOPNET_89B0", "dr7nmh836k");
    gpio_viewer.begin();
}
#endif
