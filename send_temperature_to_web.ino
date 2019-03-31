#include <ESP8266WiFi.h>           // Use this for WiFi instead of Ethernet.h
#include <OneWire.h>
#include <DallasTemperature.h>
#include <MySQL_Connection.h>
#include <MySQL_Cursor.h>
#include <Sqlinfo.h> // sql infofile header
#define myPeriodic 15 //in sec | Thingspeak pub is 15sec
#define ONE_WIRE_BUS 2  // DS18B20 on arduino pin2 corresponds to D4 on physical board

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature DS18B20(&oneWire);
IPAddress server_addr(18,221,17,28);  // IP of the MySQL *server* here
char user[] = USER_ID              // MySQL user login username
char password[] = SQL_PASSWORD        // MySQL user login password

// Sample query
// WiFi card example
char ssid[] = SS_ID         // your SSID
char pass[] = WIFI_PASSWORD     // your SSID Password

WiFiClient client;                 // Use this for WiFi instead of EthernetClient
MySQL_Connection conn(&client);
MySQL_Cursor* cursor;

void setup()
{
  Serial.begin(115200);
  while (!Serial); // wait for serial port to connect. Needed for Leonardo only

  // Begin WiFi section
  Serial.printf("\nConnecting to %s", ssid);
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  // print out info about the connection:
  Serial.println("\nConnected to network");
  Serial.print("My IP address is: ");
  Serial.println(WiFi.localIP());

  Serial.print("Connecting to SQL...  ");
  if (conn.connect(server_addr, 3306, user, password))
    Serial.println("OK.");
  else
    Serial.println("FAILED.");
  
  // create MySQL cursor object
  cursor = new MySQL_Cursor(&conn);
  if (conn.connected())
    cursor->execute("USE mydb");
  
}

void loop()
{
  String INSERT = "INSERT INTO temperature (temperature) VALUES ('";
  char INSERT_SQL[60] = {0};
  DS18B20.requestTemperatures(); 
  float temp = DS18B20.getTempCByIndex(0);
  Serial.print(String(temp)+" Temperature: ");
  Serial.println(temp);
  INSERT += String(temp);
  INSERT += "')";
  INSERT.toCharArray(INSERT_SQL,INSERT.length()+1);
  Serial.println(INSERT_SQL);
  if (conn.connected())
    cursor->execute(INSERT_SQL);
  delay(120000);
}
