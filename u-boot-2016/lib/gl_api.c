#include <common.h>
#include <cli.h>
#include <gl_api.h>
#include <asm/gpio.h>
#include <fdtdec.h>

DECLARE_GLOBAL_DATA_PTR;

void led_toggle(const char *gpio_name)
{

	int node, value;
	unsigned int gpio;
	node = fdt_path_offset(gd->fdt_blob, gpio_name);
	if (node < 0) {
		printf("Could not find %s node in fdt\n", gpio_name);
		return;
	}
	gpio = fdtdec_get_uint(gd->fdt_blob, node, "gpio", 0);
	if (gpio < 0) {
		printf("Could not find %s node's gpio in fdt\n", gpio_name);
		return;
	}

	value = gpio_get_value(gpio);
	value = !value;
	gpio_set_value(gpio, value);
}

void led_on(const char *gpio_name)
{
	int node;
	unsigned int gpio;
	node = fdt_path_offset(gd->fdt_blob, gpio_name);
	if (node < 0) {
		printf("Could not find %s node in fdt\n", gpio_name);
		return;
	}
	gpio = fdtdec_get_uint(gd->fdt_blob, node, "gpio", 0);
	if (gpio < 0) {
		printf("Could not find %s node's gpio in fdt\n", gpio_name);
		return;
	}

	gpio_set_value(gpio, 1);
}

void led_off(const char *gpio_name)
{
	int node;
	unsigned int gpio;
	node = fdt_path_offset(gd->fdt_blob, gpio_name);
	if (node < 0) {
		printf("Could not find %s node in fdt\n", gpio_name);
		return;
	}
	gpio = fdtdec_get_uint(gd->fdt_blob, node, "gpio", 0);
	if (gpio < 0) {
		printf("Could not find %s node's gpio in fdt\n", gpio_name);
		return;
	}

	gpio_set_value(gpio, 0);
}

bool button_is_press(const char *gpio_name, int value)
{
	int node;
	unsigned int gpio;
	node = fdt_path_offset(gd->fdt_blob, gpio_name);
	if (node < 0) {
		printf("Could not find %s node in fdt\n", gpio_name);
		return false;
	}
	gpio = fdtdec_get_uint(gd->fdt_blob, node, "gpio", 0);
	if (gpio < 0) {
		printf("Could not find %s node's gpio in fdt\n", gpio_name);
		return false;
	}

	if(gpio_get_value(gpio) == value)
	{
		mdelay(10);
		if(gpio_get_value(gpio) == value)
			return true;
		else
			return false;
	}
	else
		return false;
}

void check_button_is_press(void)
{
	int counter = 0;
	char *button_name = NULL;

	// 检测哪个按键被按下
    if (button_is_press("reset_key", GL_RESET_BUTTON_IS_PRESS)) {
        button_name = "RESET";
    }
#ifdef HAS_WPS_KEY
	else if (button_is_press("wps_key", GL_WPS_BUTTON_IS_PRESS)) {
        button_name = "WPS";
    }
#endif

	// 如果任一按键被按下
	while (button_name != NULL) {
		// 重新检测按键状态
        int still_pressed = 0;

        if (strcmp(button_name, "RESET") == 0) {
            still_pressed = button_is_press("reset_key", GL_RESET_BUTTON_IS_PRESS);
        }
#ifdef HAS_WPS_KEY
		else if (strcmp(button_name, "WPS") == 0) {
            still_pressed = button_is_press("wps_key", GL_WPS_BUTTON_IS_PRESS);
        }
#endif

        if (!still_pressed) {
            break;  // 按键已释放
        }

		if (counter == 0)
			printf("%s button is pressed for: %2d ", button_name, counter);

		// LED 闪烁
		led_off("power_led");
		mdelay(350);
		led_on("power_led");
		mdelay(350);

		counter++;

		// how long the button is pressed?
		printf("\b\b\b%2d ", counter);

		if(counter >= 5){
			led_off("power_led");
			led_on("blink_led");
			printf("\n");
			run_command("httpd 192.168.1.1", 0);
			cli_loop();
			break;
		}
	}

	if (counter != 0)
		printf("\n");

	return;
}

//该函数未调用
int check_test()
{
	int ret = 0;
	volatile unsigned char *f1f = NULL;
	volatile unsigned char *f2i = NULL;
	volatile unsigned char *f3r = NULL;
	volatile unsigned char *f4s = NULL;
	volatile unsigned char *f5t = NULL;
	volatile unsigned char *f6t = NULL;
	volatile unsigned char *f7e = NULL;
	volatile unsigned char *f8s = NULL;
	volatile unsigned char *f9t = NULL;

	volatile unsigned char *s0s = NULL;
	volatile unsigned char *s1e = NULL;
	volatile unsigned char *s2c = NULL;
	volatile unsigned char *s3o = NULL;
	volatile unsigned char *s4n = NULL;
	volatile unsigned char *s5d = NULL;
	volatile unsigned char *s6t = NULL;
	volatile unsigned char *s7e = NULL;
	volatile unsigned char *s8s = NULL;
	volatile unsigned char *s9t = NULL;

	char cmd[128] = {0};
	sprintf(cmd, "sf probe && sf read 0x44000000 0x%x 32",
		(CONFIG_ART_START + 0x50));
	run_command(cmd, 0);

	f1f = (volatile unsigned char *)0x44000000;
	f2i = (volatile unsigned char *)0x44000001;
	f3r = (volatile unsigned char *)0x44000002;
	f4s = (volatile unsigned char *)0x44000003;
	f5t = (volatile unsigned char *)0x44000004;
	f6t = (volatile unsigned char *)0x44000005;
	f7e = (volatile unsigned char *)0x44000006;
	f8s = (volatile unsigned char *)0x44000007;
	f9t = (volatile unsigned char *)0x44000008;

	s0s = (volatile unsigned char *)0x44000010;
	s1e = (volatile unsigned char *)0x44000011;
	s2c = (volatile unsigned char *)0x44000012;
	s3o = (volatile unsigned char *)0x44000013;
	s4n = (volatile unsigned char *)0x44000014;
	s5d = (volatile unsigned char *)0x44000015;
	s6t = (volatile unsigned char *)0x44000016;
	s7e = (volatile unsigned char *)0x44000017;
	s8s = (volatile unsigned char *)0x44000018;
	s9t = (volatile unsigned char *)0x44000019;


	if (*f1f==0x66 && *f2i==0x69 && *f3r==0x72 && *f4s==0x73 && \
			*f5t==0x74 && *f6t==0x74 && *f7e==0x65 && *f8s==0x73 && \
			*f9t==0x74 && \
			*s0s==0x73 && *s1e==0x65 && *s2c==0x63 && *s3o==0x6f && \
			*s4n==0x6e && *s5d==0x64 && *s6t==0x74 && *s7e==0x65 && \
			*s8s==0x73 && *s9t==0x74) {
		printf("Device haven tested, checking MAC info...\n");
		ret = 0;
	} else {
		printf("Device haven't tested, please test device in calibration firmware...\n");
		ret = -1;
	}

	return ret;

}

int check_config()
{
	int i = 0;
	u8 addr[6];
	u8 addr_tmp[6] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff};
	volatile unsigned char *tmp = NULL;

	char cmd[128] = {0};
	sprintf(cmd, "sf read 0x44000000 0x%x 16",
		CONFIG_ART_START);
	run_command(cmd, 0);

	/*check WAN mac*/
	for (i=0; i<6; i++) {
		tmp = (volatile unsigned char *)0x44000000 + i;
		addr[i] = *tmp;
	}
	//printf("WAN: %x:%x:%x:%x:%x:%x\n", addr[0], addr[1], addr[2], addr[3], addr[4], addr[5]);
	if (!memcmp(addr, addr_tmp, 6)) {
		printf("Device don't have WAN MAC info, please write MAC in calibration firmware...\n");
		return -1;
	}

	/*check LAN mac*/
	for (i=0; i<6; i++) {
		tmp = (volatile unsigned char *)0x44000006 + i;
		addr[i] = *tmp;
	}
	//printf("LAN: %x:%x:%x:%x:%x:%x\n", addr[0], addr[1], addr[2], addr[3], addr[4], addr[5]);
	if (!memcmp(addr, addr_tmp, 6)) {
		printf("Device don't have LAN MAC info, please write MAC in calibration firmware...\n");
		return -1;
	}
	printf("Device have MAC info, starting firmware...\n\n");
	return 0;
}

// 只检查文件的开头几个特殊 Magic Num
int check_fw_type(void *address) {
	u32 *header_magic1 = (u32 *)(address);
	u32 *header_magic2 = (u32 *)(address + 0x4);

	switch (*header_magic1) {
		case HEADER_MAGIC_CDT:
			return FW_TYPE_CDT;
		case HEADER_MAGIC_ELF:
			return FW_TYPE_ELF;
		case HEADER_MAGIC_FIT:
			return FW_TYPE_FIT;
		case HEADER_MAGIC_MBN1:
			if (*header_magic2 == HEADER_MAGIC_MBN2)
				return FW_TYPE_MIBIB;
		case HEADER_MAGIC_SBL_NAND1:
			if (*header_magic2 == HEADER_MAGIC_SBL_NAND2)
				return FW_TYPE_NAND;
		case HEADER_MAGIC_UBI:
			return FW_TYPE_UBI;
		default:
			return FW_TYPE_UNKNOWN;
	}
}

void print_fw_type(int fw_type) {
	printf("* The upload file type: ");
	switch (fw_type) {
		case FW_TYPE_CDT:
			printf("CDT *");
			break;
		case FW_TYPE_ELF:
			printf("ELF *");
			break;
		case FW_TYPE_FIT:
			printf("FIT IMAGE *");
			break;
		case FW_TYPE_MIBIB:
			printf("MIBIB *");
			break;
		case FW_TYPE_NAND:
			printf("NAND IMAGE *");
			break;
		case FW_TYPE_UBI:
			printf("UBI FIRMWARE *");
			break;
		case FW_TYPE_UNKNOWN:
		default:
			printf("UNKNOWN *");
	}
	return;
}
