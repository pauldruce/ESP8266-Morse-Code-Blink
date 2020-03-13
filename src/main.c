#include "esp_common.h"
#include "gpio.h"


/* 
    I'm not precisely sure what this code is needed for, it seems to fetch the flash-size (whatever that is) and store 
    it as a variable that the ESP8266 RTOS SDK system uses, why it isn't already within the system is a mystery.
*/
uint32 user_rf_cal_sector_set(void)
{
    flash_size_map size_map = system_get_flash_size_map();
    uint32 rf_cal_sec = 0;
    switch (size_map) {
        case FLASH_SIZE_4M_MAP_256_256:
            rf_cal_sec = 128 - 5;
            break;

        case FLASH_SIZE_8M_MAP_512_512:
            rf_cal_sec = 256 - 5;
            break;

        case FLASH_SIZE_16M_MAP_512_512:
        case FLASH_SIZE_16M_MAP_1024_1024:
            rf_cal_sec = 512 - 5;
            break;

        case FLASH_SIZE_32M_MAP_512_512:
        case FLASH_SIZE_32M_MAP_1024_1024:
            rf_cal_sec = 1024 - 5;
            break;

        default:
            rf_cal_sec = 0;
            break;
    }

    return rf_cal_sec;
}

/*
    This function returns a pointer to a string of dots and dashes for each letter of the alphabet, a,b,c,d, etc. This was the only way
    I coud get this damn function work in C. I'm sure in Python this would be very easy by using a json dictionary or similar. 
    I wanted to keep it easily editable and readable. For this reason I didn't use arrays of numerical values etc. 
*/
char* morseEncode(char x)
{
    char *temp; // This is going to be the string that m points to in the main fucntion
    switch (x)
    {
    case 'a':;
        temp =".-";
        return temp;
    case 'b':
        temp= "-...";
        return temp;
    case 'c':;
        temp = "-.-.";
        return temp;
    case 'd':;
        temp = "-..";
        return temp;
    case 'e':;
        temp = ".";
        return temp;
    case 'f':;
        temp = "..-.";
        return temp;
    case 'g':;
        temp= "--.";
        return temp;
    case 'h':;
        temp = "....";
        return temp;
    case 'i':;
        temp= "..";
        return temp;
    case 'j':;
        temp = ".---";
        return temp;
    case 'k':;
        temp = "-.-";
        return temp;
    case 'l':;
        temp= ".-..";
        return temp;
    case 'm':;
        temp = "--";
        return temp;
    case 'n':;
        temp= "-.";
        return temp;
    case 'o':;
        temp = "---";
        return temp;
    case 'p':;
        temp = ".--.";
        return temp;
    case 'q':;
        temp= "--.-";
        return temp;
    case 'r':;
        temp = ".-.";
        return temp;
    case 's':;
        temp = "...";
        return temp;
    case 't':;
        temp= "-";
        return temp;
    case 'u':;
        temp = "..-";
        return temp;
    case 'v':;
        temp = "...-";
        return temp;
    case 'w':;
        temp = ".--";
        return temp;
    case 'x':;
        temp = "-..-";
        return temp;
    case 'y':;
        temp = "-.--";
        return temp;
    // // for space
    case 'z':;
        temp = "--..";
        return temp;
    default:
        printf("Error #1\n");
        break;
    }
}

void task_blink(void* ignore)
{
    int LED_Pin = 2; // The Pin for the built in LED for Lulin NodeMCU v3, it is pin 2.

    /************       This is a program to turn the built in LED of the ESP8266 on and off        ********/
    // int numOfSecs = 2; // The time between the LED turning on and off in seconds.
    // while (true)
    // {
    //     // GPIO_OUTPUT_SET(LED_Pin, 0);
    //     // vTaskDelay(numOfSecs * 1000 / portTICK_RATE_MS);
    //     // GPIO_OUTPUT_SET(LED_Pin, 1);
    //     // vTaskDelay(numOfSecs * 1000 / portTICK_RATE_MS);
    // }
    /************      End of programm 1     ***************************************************************/

    /**************     This is a program to make the LED produce the Morse code for SOS.                 */
    /**************   This can be modified to give the morse code for any one word                        */
    
    float pulseLength = 0.2; // The time between the LED turning on and off in seconds.
    float breakLength = 2;
    
    while (true)
    {
        
        char s[] = "sos"; // Change this to what ever single english word you want.
        /*  
            If you want to do sentences, you need to define the morse code for spaces, full stops, commas etc. 
            But the code should deal with it. Add the new definition to the switch cases in the morseEncode function.
        */

        char* m;  // Define a pointer of characters to hold the morse code, one pointer should work for all the characters of the string s
        for (int i = 0; s[i]; i++) // loop over the three characters of sos
        {
            printf("%c\n", s[i]);  // print the first letter of the string s
            m = morseEncode(s[i]); // try to point m to the morse code

            /*
                This for loop below could easily be made into a function that is called. 
            */
            for (int j=0; m[j]; j++) // loop over the 4 digits of morse code
            {
                if (m[j]=='.'){
                    printf("%c\n", m[j]);                                   // Print a dot - this tests that the conditional statement works
                    GPIO_OUTPUT_SET(LED_Pin, 0);                            // Turn on the LED
                    vTaskDelay(pulseLength * 1000 / portTICK_RATE_MS);      // Wait for some time
                    // GPIO_OUTPUT_SET(LED_Pin, 1);                            // Turn off LED
                    // vTaskDelay(pulseLength * 1000 / portTICK_RATE_MS);
                }
                else if (m[j]=='-'){
                    printf("%c\n", m[j]);                                   // Print a dash - this tests that the conditional statement works
                    GPIO_OUTPUT_SET(LED_Pin, 0);                            // Turn on the LED
                    vTaskDelay(3*pulseLength * 1000 / portTICK_RATE_MS);    //Wait for some time 
                    // GPIO_OUTPUT_SET(LED_Pin, 1);                            //Turn off the LED
                    // vTaskDelay(pulseLength*1000 / portTICK_RATE_MS);
                }
                else{
                    printf("Error #2\n");                                   // Something has gone wrong, m is neither a dash or a dot.
                    vTaskDelay(5*breakLength *1000/ portTICK_RATE_MS);      // Pause the program for a few seconds to give chance to cancel before it proceeds.
                }
                GPIO_OUTPUT_SET(LED_Pin, 1);                                //Turn off the LED
                vTaskDelay(pulseLength * 1000 / portTICK_RATE_MS);          // Pause between lights on. This should be the same for dots and dashes.
            }
            vTaskDelay(pulseLength*2*1000/ portTICK_RATE_MS);               // Pause between letters, this should be double that of gap between light pulses.
        }
        vTaskDelay( breakLength * 1000 / portTICK_RATE_MS);                 // Pause between words, I'm not sure what the protocol is in general, so mine it just a longer pause.

    }

    /************      End of programm  2    ***************************************************************/

    vTaskDelete(NULL); // Not sure what this is yet, probably a way to safely exit the programm.

}

/*
        This seems to be the equivalent of main() in other programs. I'm not sure yet how this works. The documentation for this
        board is fairly hard to read. 
*/
void user_init(void)
{
    xTaskCreate(&task_blink, "startup", 2048, NULL, 1, NULL);
}

