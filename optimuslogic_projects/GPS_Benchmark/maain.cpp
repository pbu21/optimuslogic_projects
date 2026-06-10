#include <WiFi.h>
#include <PubSubClient.h>

const char* ssid = "Shan";
const char* password = "1111111111";

const char* mqtt_server = "127.0.0.1";

WiFiClient espClient;
PubSubClient client(espClient);

struct GPSData
{
    float lat;
    float lon;
    float speed;
};

void connectWiFi()
{
    WiFi.begin(ssid, password);

    Serial.print("Connecting WiFi");

    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }

    Serial.println();
    Serial.println("WiFi Connected");
    Serial.print("IP: ");
    Serial.println(WiFi.localIP());
}

void reconnectMQTT()
{
    while (!client.connected())
    {
        Serial.print("Connecting MQTT...");

        if (client.connect("ESP32_GPS_BENCH"))
        {
            Serial.println("Connected");
        }
        else
        {
            Serial.print("Failed, rc=");
            Serial.println(client.state());
            delay(2000);
        }
    }
}

void publishStack()
{
    GPSData gps;

    gps.lat =
        12.9000 +
        random(-1000, 1000) / 100000.0;

    gps.lon =
        77.6000 +
        random(-1000, 1000) / 100000.0;

    gps.speed =
        random(0, 120);

    char msg[100];

    sprintf(msg,
            "{\"lat\":%.5f,\"lon\":%.5f,\"spd\":%.1f}",
            gps.lat,
            gps.lon,
            gps.speed);

    client.publish("gps/stack", msg);
}

void publishHeap()
{
    GPSData* gps =
        (GPSData*)malloc(sizeof(GPSData));

    if (gps == NULL)
        return;

    gps->lat =
        12.9000 +
        random(-1000, 1000) / 100000.0;

    gps->lon =
        77.6000 +
        random(-1000, 1000) / 100000.0;

    gps->speed =
        random(0, 120);

    char msg[100];

    sprintf(msg,
            "{\"lat\":%.5f,\"lon\":%.5f,\"spd\":%.1f}",
            gps->lat,
            gps->lon,
            gps->speed);

    client.publish("gps/heap", msg);

    free(gps);
}

void publishMetrics(uint32_t stackTime,
                    uint32_t heapTime,
                    uint32_t freeHeap)
{
    char buffer[30];

    sprintf(buffer, "%u", stackTime);
    client.publish("benchmark/stack_time", buffer);

    sprintf(buffer, "%u", heapTime);
    client.publish("benchmark/heap_time", buffer);

    sprintf(buffer, "%u", freeHeap);
    client.publish("benchmark/freeheap", buffer);
}

void setup()
{
    Serial.begin(115200);

    randomSeed(micros());

    connectWiFi();

    client.setServer(mqtt_server, 1883);
}

void loop()
{
    if (!client.connected())
    {
        reconnectMQTT();
    }

    client.loop();

    uint32_t start;

    start = micros();
    publishStack();
    uint32_t stackTime = micros() - start;

    start = micros();
    publishHeap();
    uint32_t heapTime = micros() - start;

    uint32_t freeHeap =
        ESP.getFreeHeap();

    publishMetrics(
        stackTime,
        heapTime,
        freeHeap);

    Serial.print("Stack(us): ");
    Serial.print(stackTime);

    Serial.print(" | Heap(us): ");
    Serial.print(heapTime);

    Serial.print(" | FreeHeap: ");
    Serial.println(freeHeap);

    delay(1000);
}