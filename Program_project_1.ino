#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <WiFi.h>
#include <PubSubClient.h>

/* Inisialisasi pin yang digunakan pada sensor input evaporator*/
#define I2C_SDA_1 23
#define I2C_SCL_1 19
/* inisialisasi pin yang digunakan pada sensor output evaporator*/
#define I2C_SDA_2 33
#define I2C_SCL_2 32
//#define ledPin 22

/* Membuat objek untuk membuat pin dapat berkomunikasi dengan 12C */
TwoWire I2CBME1 = TwoWire(0);
TwoWire I2CBME2 = TwoWire(1);
/* Membuat objek untuk dapat memanggil fungsi pada library */
Adafruit_BME280 bme1;
Adafruit_BME280 bme2;
/* Menginisialisasikan username dan password wifi/hotspot untuk
menyambungkan ESP32 dengan Internet */
const char* ssid = "NAMA_WIFI";
const char* password = "Password";
unsigned long delayTime;
/* Menginisialisasikan alamat dari broker MQTT untuk
menyambungkan ESP32 dengan broker MQTT */
const char* mqtt_server = "test.mosquitto.org";

// Membuat array untuk menampung nilai pembacaan sensor 1/in_Eva
char temp1_inEva [8];
char hum1_inEva [8];
// Membuat array untuk menampung nilai pembacaan sensor 2/out_Eva
char temp2_outEva[8];
char hum2_outEva [8];
// Mendfinisikan konstanta untuk perhitungan model matematika
float temp1,hum1,temp2,hum2;
float x1,x2,z;
float e =2.72;
double p;
WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;
void setup() {
// put your setup code here, to run once:
Serial.begin(115200);
setup_wifi(); // Memanggil fungsi setup_wifi
/* Memulai komunikasi I2C pada pin yang digunakan */
Serial.println(F("BME280 1 test"));
I2CBME1.begin(I2C_SDA_1, I2C_SCL_1, 100000); //Komunikasi pada pin sensor 1/sensor
in_Eva
Serial.println(F("BME280 2 test"));
I2CBME2.begin(I2C_SDA_2, I2C_SCL_2, 100000);//Komunikasi pada pin sensor 2/sensor
out_Eva

if (!status1) {
Serial.println("Could not find a valid BME280 1 sensor, check wiring!");
while (1);
}
status2 = bme2.begin(0x76, &I2CBME2); //Pengecekan senor2/out_Eva
if (!status2) {
Serial.println("Could not find a valid BME280 2 sensor, check wiring!");
while (1);
}
Serial.println("-- Test OKE --");
/* Memulai menghubungkan ESP32 dengan broker MQTT */
client.setServer(mqtt_server, 1883);
delayTime = 1000;
Serial.println();
}
/* Fungsi untuk menyambungkan ESP32 dengan internet */
void setup_wifi() {
delay(10);
/* Menyambungkan ESP32 dengan Internet */
Serial.println();
Serial.print("Connecting to ");
Serial.println(ssid);
WiFi.begin(ssid, password);
while (WiFi.status() != WL_CONNECTED) {
delay(500);
Serial.print(".");
}
randomSeed(micros());
Serial.println("");
Serial.println("WiFi connected");
Serial.println("IP address: ");
Serial.println(WiFi.localIP());
}
bool status1;// Objek untuk mengecek komunikasi pada sensor in_Eva
bool status2;// Objek untuk mengecek komunikasi pada sensor out_Eva
/* Mengecek komunikasi I2C pada sensor dengan ESP32 */
status1 = bme1.begin(0x76, &I2CBME1); // Pengecekan sensor 1/in_Eva
if (!status1) {
Serial.println("Could not find a valid BME280 1 sensor, check wiring!");
while (1);
}
status2 = bme2.begin(0x76, &I2CBME2); //Pengecekan senor2/out_Eva
if (!status2) {
Serial.println("Could not find a valid BME280 2 sensor, check wiring!");
while (1);
}
Serial.println("-- Test OKE --");
/* Memulai menghubungkan ESP32 dengan broker MQTT */
client.setServer(mqtt_server, 1883);
delayTime = 1000;
Serial.println();
}
/* Fungsi untuk menyambungkan ESP32 dengan internet */
void setup_wifi() {
delay(10);
/* Menyambungkan ESP32 dengan Internet */
Serial.println();
Serial.print("Connecting to ");
Serial.println(ssid);
WiFi.begin(ssid, password);
while (WiFi.status() != WL_CONNECTED) {
delay(500);
Serial.print(".");
}
randomSeed(micros());
Serial.println("");
Serial.println("WiFi connected");
Serial.println("IP address: ");
Serial.println(WiFi.localIP());
}
/* Fungsi untuk membaca humidity dan temperature dari sensor
* dan mengirimkan kepada server MQTT */
void printValues() {
// Membaca nilai temperature dan humidity pada sensor 1/in_Eva
temp1 = bme1.readTemperature();
hum1 = bme1.readHumidity();
// Membaca nilai temperature dan humidity pada sensor 2/out_Eva
temp2 = bme2.readTemperature();
hum2 = bme2.readHumidity();
// Memasukan nilai ke dalam array
dtostrf (temp1,1,2,temp1_inEva);
dtostrf (hum1,1,2,hum1_inEva);
dtostrf (temp2,1,2,temp2_outEva);
dtostrf (hum2,1,2,hum2_outEva);
// Perhitungan untuk mengetahui kondisi filter udara AC
x1 = temp1-temp2;
x2 = hum2-hum1;
z = -45.12 + (x1*7.43)+(x2 *0.75);
p = pow(e,z)/(1 + pow(e,z));
Serial.println ("-------SENSOR 1--------");
client.publish("inEva/temp", temp1_inEva); // Mengirimkan array hasil pembacaan sensor ke
sever MQTT dengan topic "inEva/temp" untuk pembacaan temperature sensor 1/in_Eva
client.publish("inEva/hum", hum1_inEva); // Mengirimkan array hasil pembacaan sensor ke sever
MQTT dengan topic "inEva/hum" untuk pembacaan humidity sensor 1/in_Eva
Serial.print("Temperature = ");
Serial.print(temp1);
Serial.println(" *C");
Serial.print("Humidity = ");
Serial.print(hum1);
Serial.println(" %");
Serial.println();
Serial.println ("-------SENSOR 2--------");
client.publish("outEva/temp",temp2_outEva);// Mengirimkan array hasil pembacaan sensor ke
sever MQTT dengan topic "outEva/temp" untuk pembacaan temperature sensor 2/out_Eva
client.publish("outEva/hum",hum2_outEva); // Mengirimkan array hasil pembacaan sensor ke
sever MQTT dengan topic "outEva/hum" untuk pembacaan humidity sensor 2/out_Eva
Serial.print("Temperature = ");
Serial.print(temp2);
Serial.println(" *C");
Serial.print("Humidity = ");
Serial.print(hum2);
Serial.println(" %");
// /* Peringatan evaporator AC rusak*/

char OKE [50]= "evaporator AC dalam keadaan baik"; // Pesan peringatan evaporator AC baik-
baik saja

char EROR[50] = "evaporator AC perlu dilakukan pembersihan";// Pesan peringatan evaporator
AC sudah tidak baik-baik saja
char SALAH [50] = "pengukuran salah";
if(p>=0.51 && p<=1){
Serial.println(OKE);
Serial.println (p);
client.publish("Eva/AC",OKE); //Megirimkan pesan Evaisi evaporator AC baik-baik saja
}
else if (p < 0.51 && p >= 0) {
Serial.println(EROR);
Serial.println (p);
client.publish("Eva/AC",EROR); // Mengirimkan pesan Evaisi evaporator AC sudah rusak.
}
else {
client.publish("Eva/AC",SALAH);
Serial.println (SALAH);
}
Serial.println();
delay(30000);
}
/* Fungsi reconnect apabila ESP32 tidak terhubung dengan broker MQTT */
void reconnect() {
// Berulang sampai terhubung
while (!client.connected()) {
Serial.print("Attempting MQTT connection...");
String clientId = "ESP32Client-";
clientId += String(random(0xffff), HEX);
//Program untuk menghubungkan MQTT
if (client.connect(clientId.c_str())) {
Serial.println("connected");
}
else {
Serial.print("failed, reconnect=");
Serial.print(client.state());
Serial.println(" try again in 5 seconds");
// Delay lima detik sebelum berulang
delay(5000);
}
}
}
void loop() {
if (!client.connected()) {
reconnect();
}
printValues();
client.loop();
long now = millis();
if (now - lastMsg > 5000) {
lastMsg = now;
}
delay(delayTime);
}