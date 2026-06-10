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
};

void connectWiFi()
{
    Serial.print("Connecting WiFi");

    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }

    Serial.println();
    Serial.println("WiFi Connected");
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());
}

void reconnectMQTT()
{
    while (!client.connected())
    {
        Serial.print("Connecting MQTT...");

        if (client.connect("ESP32_GPS"))
        {
            Serial.println("Connected");
        }
        else
        {
            Serial.print("Failed rc=");
            Serial.print(client.state());
            Serial.println(" retrying...");
            delay(2000);
        }
    }
}

void publishGPS()
{
    static float lat = 12.9123;
    static float lon = 77.6123;

    lat += random(-20, 20) / 10000.0;
    lon += random(-20, 20) / 10000.0;

    char payload[100];

    sprintf(
        payload,
        "{lat:%.5f,lon:%.5f}",
        lat,
        lon
    );

    client.publish("gps/stack", payload);

    Serial.print("Published: ");
    Serial.println(payload);
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

    publishGPS();

    delay(1000);
}