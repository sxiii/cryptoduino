#define DB7 32
#define DB6 33
#define DB5 34
#define DB4 35
#define DB3 36
#define DB2 37
#define DB1 38
#define DB0 39
#define EN1 40
#define RW  41
#define RS  42
#define CON 43
#define GND 44
#define VCC 45
#define EN2 46

#include <SPI.h>
#include <Ethernet.h>
#include <ArduinoJson.h>
#include <LiquidCrystal.h>

LiquidCrystal lcd1(42, EN1, 35, 34, 33, 32);
LiquidCrystal lcd2(42, EN2, 35, 34, 33, 32);

static void printToDisplay(LiquidCrystal& lcd, const char* data, byte column, byte row, boolean moveCursor) {
  if (moveCursor) {
    lcd.setCursor(20 * column, row);
  }

  lcd.print(data);
}

static void connect(Client& client, const char* server, const uint16_t& port) {
  if (!client.connected()) {
    if (!client.connect(server, port)) {
      lcd1.clear();
      lcd2.clear();
      printToDisplay(lcd1, "Connection to the ", 0, 0, true);
      printToDisplay(lcd1, server, 0, 0, false);
      printToDisplay(lcd1, " server is not established.", 0, 0, false);
    }
  }
}

static void request(Client& client, const char* host, const char* method, const char* resource) {
  client.print(method);
  client.print(" ");
  client.print(resource);
  client.println(" HTTP/1.1");
  client.print("Host: ");
  client.println(host);
  client.println("Connection: close");
  client.println();
}

static boolean fetch(Client& client, char* buffer, size_t bufferSize, size_t requestStripCount) {
  delay(2000);

  if (!client.find("\r\n\r\n")) {
    return false;
  }

  size_t i = 0;

  while (client.available()) {
    char data = client.read();

    if (requestStripCount > 0) {
      requestStripCount--;
    } else {
      if (i < bufferSize) {
        if (buffer[i - 1] == '\r' && data == '\n') {
          buffer[i - 1] = '\0';
        } else {
          buffer[i++] = data;
        }
      }
    }
  }

  client.stop();

  return true;
}

static void decode(const char* buffer, size_t bufferSize, LiquidCrystal& lcd, byte row) {
  Serial.println(buffer);

  StaticJsonBuffer<512> jsonBuffer;
 
  JsonObject& root = jsonBuffer.parseObject(buffer);

  if (root.success()) {
    boolean success = root["success"];

    if (success) {
      const char* marketName = root["result"][0]["MarketName"];

      printToDisplay(lcd, marketName, 0, row, true);
      printToDisplay(lcd, " ", 0, row, false);

      const char* low = root["result"][0]["Low"];

      printToDisplay(lcd, "low: ", 0, row, false);
      printToDisplay(lcd, low, 0, row, false);

      const char* high = root["result"][0]["High"];

      printToDisplay(lcd, " high: ", 0, row, false);
      printToDisplay(lcd, high, 0, row, false);
    }
  } else {
    Serial.println("parseObject() failed");
  }
}

void setup() {
  Serial.begin(115200);
  while (!Serial);

  pinMode(RW, OUTPUT);
  pinMode(A13, OUTPUT);
  digitalWrite(A13, HIGH);

  lcd1.begin(40, 2);
  lcd2.begin(40, 2);

  byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };

  if (Ethernet.begin(mac) == 0) {
    printToDisplay(lcd1, "Failed to configure Ethernet using DHCP.", 0, 0, true);
  }

  delay(1000);
}

void loop()
{
  EthernetClient client;
  const char bittrexServer[] = "bittrex.com";
  const uint16_t bittrexPort = 80;
  const size_t bufferSize = 512;
  char buffer[bufferSize];

  connect(client, bittrexServer, bittrexPort);

  request(client, bittrexServer, "GET", "/api/v1.1/public/getmarketsummary?market=btc-ltc");

  boolean bittrexBTCLTCStatus = fetch(client, buffer, bufferSize, 4);

  if (bittrexBTCLTCStatus) {
    decode(buffer, bufferSize, lcd1, 0);
  }

  connect(client, bittrexServer, bittrexPort);

  request(client, bittrexServer, "GET", "/api/v1.1/public/getmarketsummary?market=btc-eth");

  boolean bittrexBTCETHStatus = fetch(client, buffer, bufferSize, 4);

  if (bittrexBTCETHStatus) {
    decode(buffer, bufferSize, lcd1, 1);
  }

  connect(client, bittrexServer, bittrexPort);

  request(client, bittrexServer, "GET", "/api/v1.1/public/getmarketsummary?market=btc-zec");

  boolean bittrexBTCZECStatus = fetch(client, buffer, bufferSize, 4);

  if (bittrexBTCZECStatus) {
    decode(buffer, bufferSize, lcd2, 0);
  }

  connect(client, bittrexServer, bittrexPort);

  request(client, bittrexServer, "GET", "/api/v1.1/public/getmarketsummary?market=btc-etc");

  boolean bittrexBTCETCStatus = fetch(client, buffer, bufferSize, 4);

  if (bittrexBTCETCStatus) {
    decode(buffer, bufferSize, lcd2, 1);
  }
}
