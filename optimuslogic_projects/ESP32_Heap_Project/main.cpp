#include "esp_heap_caps.h"

void setup()
{
    Serial.begin(115200);

    for(int i = 0; i < 1000; i++)
    {
        int size = random(50,500);

        char *ptr = (char*)malloc(size);

        if(ptr == NULL)
        {
            Serial.println("Allocation Failed");
            break;
        }

        memset(ptr,0,size);

        Serial.printf("%d,%u\n",
                      i,
                      heap_caps_get_free_size(MALLOC_CAP_8BIT));

        if(i % 2 == 0)
        {
            free(ptr);
        }

        delay(20);
    }
}

void loop()
{
}