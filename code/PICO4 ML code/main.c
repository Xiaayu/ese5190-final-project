#include <stdio.h>
#include "pico/stdlib.h"
#include <tusb.h>
#include <math.h>
#include "pico/multicore.h"
#include "arducam/arducam.h"
#include "lib/st7735.h"
#include "lib/fonts.h"
uint8_t image_buf[324*324];
uint8_t displayBuf[80*160*2];
uint8_t header[2] = {0x55,0xAA};
int counter = 0;
int areaCounter[3][3];

#define FLAG_VALUE 123

int * getMax(int ar[3][3]){
	static int tmp[2];
	// value
	tmp[0] = 0;
	for (int a = 0; a < 3; a++){
		for (int b = 0; b < 3; b++){
			if (ar[a][b] > tmp[0]){
				tmp[0] = ar[a][b];
				tmp[1] = a;
				tmp[2] = b;
			}
		}
	}
	return tmp;
}

void core1_entry() {
        multicore_fifo_push_blocking(FLAG_VALUE);

        uint32_t g = multicore_fifo_pop_blocking();

        if (g != FLAG_VALUE)
          printf("Hmm, that's not right on core 1!\n");
        else
          printf("It's all gone well on core 1!\n");

	gpio_init(PIN_LED);
	gpio_set_dir(PIN_LED, GPIO_OUT);

	ST7735_Init();
	ST7735_DrawImage(0, 0, 80, 160, arducam_logo);
	//ST7735_InvertColors(true);

	struct arducam_config config;
	config.sccb = i2c0;
	config.sccb_mode = I2C_MODE_16_8;
	config.sensor_address = 0x24;
	config.pin_sioc = PIN_CAM_SIOC;
	config.pin_siod = PIN_CAM_SIOD;
	config.pin_resetb = PIN_CAM_RESETB;
	config.pin_xclk = PIN_CAM_XCLK;
	config.pin_vsync = PIN_CAM_VSYNC;
	config.pin_y2_pio_base = PIN_CAM_Y2_PIO_BASE;

	config.pio = pio0;
	config.pio_sm = 0;

	config.dma_channel = 0;
	config.image_buf = image_buf;
	config.image_buf_size = sizeof(image_buf);

	arducam_init(&config);



	while (true) {
	  	gpio_put(PIN_LED, !gpio_get(PIN_LED));
	  	arducam_capture_frame(&config);

		//initialize the counter
		counter = 0;

		//initialize the variables
		int max = 0;
		int dir[2] = {-1, -1};

		//initialize the area counter
		for (int i = 0; i < 3; i ++){
			for  (int j = 0; j < 3; j ++){
				areaCounter[i][j] = 0;
			}
		}

		

		// Method3 average frame
		int flag = 0;
		int xWeight[3];
		int yWeight[3];

		while (flag < 1){	
			// Method2 variables
			float x_weight = 0;
			float y_weight = 0;	

			uint16_t index = 0;
			for (int y = 0; y < 160; y++) {
				for (int x = 0; x < 80; x++) {
					uint8_t c = image_buf[(2+320-2*y)*324+(2+40+2*x)];
					uint16_t imageRGB   = ST7735_COLOR565(c, c, c);
					//if (x == 1 && y == 1){printf("%d\n",c);}
					//ST7735_DrawPixel(x,y,imageRGB);

					// Method 1 : Counter
					/*
					if (imageRGB == 65535){
						displayBuf[index++] = (uint8_t)(0xF800 >> 8) & 0xFF;
						displayBuf[index++] = (uint8_t)(0xF800)&0xFF;
						counter ++;
						if (x < 25){
							if (y < 50){ areaCounter[0][0]++;}
							else if (y >= 50 && y < 110){ areaCounter[1][0]++;}
							else if (y >= 110 && y < 160){ areaCounter[2][0]++;}
						} else if (x >= 25 && x < 55){
							if (y < 50){ areaCounter[0][1]++;}
							else if (y >= 50 && y < 110){ areaCounter[1][1]++;}
							else if (y >= 110 && y < 160){ areaCounter[2][1]++;}
						} else if (x >= 55 && x < 80){
							if (y < 50){ areaCounter[0][2]++;}
							else if (y >= 50 && y < 110){ areaCounter[1][2]++;}
							else if (y >= 110 && y < 160){ areaCounter[2][2]++;}
						} 
					} else{
						displayBuf[index++] = (uint8_t)(imageRGB >> 8) & 0xFF;
						displayBuf[index++] = (uint8_t)(imageRGB)&0xFF;}
					*/


					// Method 2: Average weighted
					if (imageRGB == 65535){
						displayBuf[index++] = (uint8_t)(0x001F >> 8) & 0xFF;
						displayBuf[index++] = (uint8_t)(0x001F) & 0xFF;

						// add up weighted x and y
						x_weight += x;
						y_weight += y;
						counter++;
					}else{
						displayBuf[index++] = (uint8_t)(imageRGB >> 8) & 0xFF;
						displayBuf[index++] = (uint8_t)(imageRGB)&0xFF;}
				}
			}

			x_weight /= counter;
			y_weight /= counter;

			xWeight[flag] = x_weight;
			yWeight[flag] = y_weight;

			flag++;
		}

		// newweight variables
		float x_weight = 0;
		float y_weight = 0;

		for (int i = 0; i < 1; i++){
			x_weight += xWeight[i];
			y_weight += yWeight[i];
		}

		// check the location of x and y weight
		if (x_weight < 25){
			if (y_weight < 50){ areaCounter[0][0]++;}
			else if (y_weight >= 50 && y_weight < 110){ areaCounter[1][0]++;}
			else if (y_weight >= 110 && y_weight < 160){ areaCounter[2][0]++;}
		} else if (x_weight >= 25 && x_weight < 55){
			if (y_weight < 50){ areaCounter[0][1]++;}
			else if (y_weight >= 50 && y_weight < 110){ areaCounter[1][1]++;}
			else if (y_weight >= 110 && y_weight < 160){ areaCounter[2][1]++;}
		} else if (x_weight >= 55 && x_weight < 80){
			if (y_weight < 50){ areaCounter[0][2]++;}
			else if (y_weight >= 50 && y_weight < 110){ areaCounter[1][2]++;}
			else if (y_weight >= 110 && y_weight < 160){ areaCounter[2][2]++;}
		} 


		// obtain the max white point area
		max = getMax(areaCounter)[0];
		if (max != 0){
			dir[0]= getMax(areaCounter)[1];
			dir[1] = getMax(areaCounter)[2];
		}
		

		/*
		printf("%d  %d  %d\n", areaCounter[0][0],areaCounter[0][1],areaCounter[0][2]);
		printf("%d  %d  %d\n", areaCounter[1][0],areaCounter[1][1],areaCounter[1][2]);
		printf("%d  %d  %d\n", areaCounter[2][0],areaCounter[2][1],areaCounter[2][2]);
		printf("\n");
		printf("%d  (%d, %d)\n", max, dir[0], dir[1]);
		printf("\n");
		*/
		
		// instruction set 
		// 0000: nothing()
		// 0001: left-up
		// 0010: up
		// 0011: right-up
		// 0100: left
		// 0101: middle
		// 0110: right
		// 0111: left-down
		// 1000: down
		// 1001: right-down


		// state 
		if (dir[0] == -1 && dir[1] == -1) {printf("nothing\n"); 
		//ST7735_DrawImage(0, 130, 80, 160, ST7735_COLOR565(0xaa,0xaa ,0xaa));
		ST7735_WriteString(10,130,"NOTH",Font_16x26, 0, ST7735_COLOR565(0xaa,0xaa ,0xaa));
		gpio_put(0, 0);
		gpio_put(1, 0);
		gpio_put(2, 0);
		gpio_put(3, 0);
		}

		else if (dir[0] == 0 && dir[1] == 0) {printf("left-up\n");
		ST7735_WriteString(10,130,"L-UP",Font_16x26, 0, ST7735_COLOR565(0xaa,0xaa ,0xaa));
		gpio_put(0, 0);
		gpio_put(1, 0);
		gpio_put(2, 0);
		gpio_put(3, 1);}

		else if (dir[0] == 0 && dir[1] == 1) {printf("up\n");
		ST7735_WriteString(30,130,"UP",Font_16x26, 0, ST7735_COLOR565(0xaa,0xaa ,0xaa));
		gpio_put(0, 0);
		gpio_put(1, 0);
		gpio_put(2, 1);
		gpio_put(3, 0);}

		else if (dir[0] == 0 && dir[1] == 2) {printf("right-up\n");
		ST7735_WriteString(10,130,"R-UP",Font_16x26, 0, ST7735_COLOR565(0xaa,0xaa ,0xaa));
	  gpio_put(0, 0);
		gpio_put(1, 0);
		gpio_put(2, 1);
		gpio_put(3, 1);}

		else if (dir[0] == 1 && dir[1] == 0) {printf("left\n");
		ST7735_WriteString(10,130,"LEFT",Font_16x26, 0, ST7735_COLOR565(0xaa,0xaa ,0xaa));
		gpio_put(0, 0);
		gpio_put(1, 1);
		gpio_put(2, 0);
		gpio_put(3, 0);}

		else if (dir[0] == 1 && dir[1] == 1) {printf("middle\n");
		ST7735_WriteString(10,130,"MIDD",Font_16x26, 0, ST7735_COLOR565(0xaa,0xaa ,0xaa));
		gpio_put(0, 0);
		gpio_put(1, 1);
		gpio_put(2, 0);
		gpio_put(3, 1);}

		else if (dir[0] == 1 && dir[1] == 2) {printf("right\n");
		ST7735_WriteString(0,130,"RIGHT",Font_16x26, 0, ST7735_COLOR565(0xaa,0xaa ,0xaa));
		gpio_put(0, 0);
		gpio_put(1, 1);
		gpio_put(2, 1);
		gpio_put(3, 0);}

		else if (dir[0] == 2 && dir[1] == 0) {printf("left-down\n");
		ST7735_WriteString(0,130,"L-DOW",Font_16x26, 0, ST7735_COLOR565(0xaa,0xaa ,0xaa));
		gpio_put(0, 0);
		gpio_put(1, 1);
		gpio_put(2, 1);
		gpio_put(3, 1);}

		else if (dir[0] == 2 && dir[1] == 1) {printf("down\n");
		ST7735_WriteString(10,130,"DOWN",Font_16x26, 0, ST7735_COLOR565(0xaa,0xaa ,0xaa));
	  gpio_put(0, 1);
		gpio_put(1, 0);
		gpio_put(2, 0);
		gpio_put(3, 0);}

		else if (dir[0] == 2 && dir[1] == 2) {printf("right-down\n");
		ST7735_WriteString(0,130,"R-DOW",Font_16x26, 0, ST7735_COLOR565(0xaa,0xaa ,0xaa));
		gpio_put(0, 1);
		gpio_put(1, 0);
		gpio_put(2, 0);
		gpio_put(3, 1);}

		// draw the image
	  ST7735_DrawImage(0, 0, 80, 160, displayBuf);
		//ST7735_FillScreen(ST7735_COLOR565(0xaa,0xaa ,0xaa));
		//ST7735_WriteString(0,120,"nothin",Font_16x26, 0, ST7735_BLACK);
		//ST7735_FillScreen(ST7735_COLOR565(0xaa,0xaa ,0xaa));
		
	}
}

#include "hardware/vreg.h"

int main() {
  int loops=20;
  stdio_init_all();
  gpio_init(0);
  gpio_init(1);
  gpio_init(2);
  gpio_init(3);
  gpio_set_dir(0, GPIO_OUT);
  gpio_set_dir(1, GPIO_OUT);
  gpio_set_dir(2, GPIO_OUT);
  gpio_set_dir(3, GPIO_OUT);

  while (!tud_cdc_connected()) { sleep_ms(100); if (--loops==0) break;  }

  printf("tud_cdc_connected(%d)\n", tud_cdc_connected()?1:0);

  vreg_set_voltage(VREG_VOLTAGE_1_30);
  sleep_ms(1000);
  set_sys_clock_khz(250000, true);

  multicore_launch_core1(core1_entry);

  uint32_t g = multicore_fifo_pop_blocking();

  if (g != FLAG_VALUE)
    printf("Hmm, that's not right on core 0!\n");
  else {
    multicore_fifo_push_blocking(FLAG_VALUE);
    printf("It's all gone well on core 0!\n");
  }

  while (1)
    tight_loop_contents();
}
