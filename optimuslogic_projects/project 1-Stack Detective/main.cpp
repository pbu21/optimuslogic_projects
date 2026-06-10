void setup()
{
    Serial.begin(115200);

    Serial.println("Iteration,FreeHeap");

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

        Serial.printf("%d,%u\n", i, ESP.getFreeHeap());

        if(i % 2 == 0)
        {
            free(ptr);
        }

        delay(10);
    }
}

void loop()
{
}