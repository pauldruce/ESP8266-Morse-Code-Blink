# ESP8266 Morse Code Blink
 Making the ESP8266 translate a message into morse code and blink the build in LED.

To run this code, I used the [PlatformIO](https://platformio.org/platformio-ide) for VSCode. 

The ESP8266 I am using is the Lolin NodeMCU v3 board. The key aspect for this project is to know which GPIO pin the internal LED is connected to. For this board it is the GPIO pin 2. For other boards you will have to find which pin this is. 

I am **not** using the Arduino framework. I **am** using the [RTOS](https://docs.espressif.com/projects/esp8266-rtos-sdk/en/latest/get-started/index.html) framework. The reason for this is I want to get familiar with this implementation, so I can learn how to use both cores on my ESP-32. This project is my first steps at using C to control a microcontroller. 

I have other projects where I use micropython (see my other GitHub repos). However, to get the most out of these small devices, using the C language is required. 

For this project you will need to download and install the Espresiff Toolchain and ESP8266_RTOS_SDK. This can be done by following the steps in [https://docs.espressif.com/projects/esp8266-rtos-sdk/en/latest/get-started/index.html](https://docs.espressif.com/projects/esp8266-rtos-sdk/en/latest/get-started/index.html). 

It might be worth installing the PatformIO for VSCode first, and trying to install the ESP8266 board there. It might skip some steps above. 

Once you are able to flash (upload in PlatformIO) and monitor the board. You can test out this project. 

There are some aspects which I do not fully understand, such as the first function in the main C file. However, removing them renders the code useless. 

The folders “include”, “lib” and “test” are included purely because they will likely be there when you set up your project in PlatformIO. 

The only files that you need to consider are the platformio.ini file and the main.c the folder “src".



### The platformio.ini file  

This is how you tell platformio what board you are dealing with, as well as setting some parameters.

For all the different options that you can use in this file see [https://docs.platformio.org/en/latest/projectconf/section_env.html](https://docs.platformio.org/en/latest/projectconf/section_env.html) 

I’ll go through mine line by line here:

“[env:nodemcuv2] “   - This just tells the software that I’m going to set some variables below. and I’ve name this setup nodemcu2, this feature seems to be useful when considering different boards for the same code.

“monitor_speed = 74880”		- 		This variable was a pain to track down, despite the back of my board saying the baud rate is 9200, garbage/gibberish was produced when using monitor at that speed. After some fidling with this variable I found the correct speed for my board. 

platform = espressif8266		- 		This is important, it tells the software of platformio what libraries are available etc. 

framework = esp8266-rtos-sdk		-		Again, important as it tells the software we are using the RTOS framework and to include those commands.

board = nodemcuv2		-		This i’m not sure of, as it’s not technically the board I have. 

That’s all that is need for this ini file.



### The main code. 

##### The includes

The esp_common.h provides us with tools to control the board. Specifically it lets us tell the chip which programs to run. 

The gpio.h header allows us to control the different GPIO pins of the board.



##### The functon "user_rf_cal_sector_set”

This function is a bit of a mystery, I’m not sure why I need it and I can’t remember where I found it. As far as I can tell, the ESP8266 RTOS framework code, the non-user accessible stuff, requires that rc_cal_sec be correctly assigned, otherwise it fails. Please let me know if you have a deeper knowledge of this. 

##### The function "morseEncode”

This function takes in a specific character and returns a pointer to a character array of dots (.) and dashes (-), which represent the Morse code. 

##### The function "task_blink”

This is the main function of the code. There are two independent codes within this function, one to just make the led blink on and off repeatedly. Then there is the morse Code blinker. 

The basic blink code is commented out.



The key variable to verify for your board is the following.

```c
int LED_Pin = 2; // The Pin for the built in LED for Lulin NodeMCU v3, it is pin 2.
```

Change this to which ever pin you have an LED hooked up to, or whichever Pin controls the on-board LED.

The following two definitions control aspects of the Morse Code functionality: 

```c
float pulseLength = 0.2; // The time between the LED turning on and off in seconds.
float breakLength = 2;
```

The “pulseLength" controls how long the LED is illuminated for a dot (.). The dashes are coded to last three times longer than a dot. Between each letter there is 2 pulseLengths.

The “breakLength” variables controls how long between distinct words. So you can tell when you are done with one word and need to start the next. 

If you change the following line

```c
char s[] = "sos";
```

so that “sos” is replace with another **single, english** world, without any spaces or punctuation, then the code should still work. 

##### The function “user_init”

This piece of code seems to be how you tell the ESP8266 to run certain functions and therefore code. The command xTaskCreate, tells the board which function to run, in our case this is task_blink. 