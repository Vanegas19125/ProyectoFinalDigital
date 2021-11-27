/*************************************************************************************************
  ESP32 Web Server
  Ejemplo de creación de Web server 
  Basándose en los ejemplos de: 
  https://lastminuteengineers.com/creating-esp32-web-server-arduino-ide/
  https://electropeak.com/learn
**************************************************************************************************/
//************************************************************************************************
// Librerías
//************************************************************************************************
#include <WiFi.h>
#include <WebServer.h>
//************************************************************************************************
// Variables globales
//************************************************************************************************
// SSID & Password
const char* ssid = "Jose´s Laptops";  // Enter your SSID here
const char* password = "12345678";  //Enter your Password here
String cadena = "";
String cadena2 = "";
int parqueos[] = {0,0,0,0}; // para guardar si el parqueo esta ocupado o no
int parqueos2[] = {0,0,0,0}; // para guardar si el parqueo esta ocupado o no
int contador =0;
int pines7seg[] = {12,13,15,2,0,14,27};
int segmentos7[][9]= {
    {1,1,1,1,1,1,0},
    {0,1,1,0,0,0,0},
    {1,1,0,1,1,0,1},
    {1,1,1,1,0,0,1},
    {0,1,1,0,0,1,1},
    {1,0,1,1,0,1,1},
    {1,0,1,1,1,1,1},
    {1,1,1,0,0,0,0},
    {1,1,1,1,1,1,1},
    {1,1,1,1,0,1,1}
  };


WebServer server(80);  // Object of WebServer(HTTP port, 80 is defult)


uint8_t LED1pin = 2;
bool LED1status = LOW;

//************************************************************************************************
// Configuración
//************************************************************************************************
void setup() {
  Serial.begin(115200);
  Serial2.begin(115200);
  Serial.println("Try Connecting to ");
  Serial.println(ssid);

  pinMode(LED1pin, OUTPUT);

  for(int i=0; i<7;i++) pinMode(pines7seg[i],OUTPUT);
  // Connect to your wi-fi modem
  WiFi.begin(ssid, password);

  // Check wi-fi is connected to wi-fi network
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected successfully");
  Serial.print("Got IP: ");
  Serial.println(WiFi.localIP());  //Show ESP32 IP on serial

  server.on("/", conexion); // Directamente desde e.g. 192.168.0.8

  
  server.onNotFound(handle_NotFound);

  server.begin();
  Serial.println("HTTP server started");
  delay(100);
}
//************************************************************************************************
// loop principal
//************************************************************************************************
void loop() {
  server.handleClient();
  if(Serial2.available()){
    char a= Serial2.read();
    //Serial.print(a);
    if (a == '\n'){
           parqueos[0] = cadena.substring(0,1).toInt();
           parqueos[1] = cadena.substring(2,3).toInt();
           parqueos[2] = cadena.substring(4,5).toInt();
           parqueos[3] = cadena.substring(6,7).toInt();
           cadena = "";
     }else cadena += a;
  }

  if(Serial.available()){
    char a= Serial.read();
    Serial.print(a);
    if (a == '\n'){
           parqueos2[0] = cadena2.substring(0,1).toInt();
           parqueos2[1] = cadena2.substring(2,3).toInt();
           parqueos2[2] = cadena2.substring(4,5).toInt();
           parqueos2[3] = cadena2.substring(6,7).toInt();
           cadena2 = "";
     }else cadena2 += a;
  }

  for(int i=0; i<4;i++){
    contador += parqueos[i]+ (1-parqueos2[i]);
  }

  for(int i=0; i<7; i++) digitalWrite(pines7seg[i], segmentos7[contador][i]);
  contador = 0;
}
//************************************************************************************************
// Handler de Inicio página
//************************************************************************************************
void conexion(){  
  String text = "<!DOCTYPE html> <html>\n" ;
  text +="<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\"> <meta http-equiv=\"refresh\" content=\"1\" >\n";
  text +="<title>SISTEMA DE PARQUEOS</title>\n";
  text += "<h1 style=\"color: #5e9ca0; text-align: center;\">PARQUEO MATIC</h1>\n";
  text += "<h1 style=\"color: #5e9ca0; text-align: center;\">sistema de parqueos automaticos</h1>";
  if (parqueos[0]) text += "<h3 style=\"text-align: center;\">&nbsp;PARQUEO 1: disponible &#128664</h3> ";
  else text += "<h3 style=\"text-align: center;\">&nbsp;PARQUEO 1: ocupado</h3> ";
  
  if (parqueos[1]) text += "<h3 style=\"text-align: center;\">&nbsp;PARQUEO 2: disponible &#128664</h3> ";
  else text += "<h3 style=\"text-align: center;\">&nbsp;PARQUEO 2: ocupado</h3  > ";
  
  if (parqueos[2]) text += "<h3 style=\"text-align: center;\">&nbsp;PARQUEO 3: disponible &#128664</h3> ";
  else text += "<h3 style=\"text-align: center;\">&nbsp;PARQUEO 3: ocupado</h3> ";
  
  if (parqueos[3]) text += "<h3 style=\"text-align: center;\">&nbsp;PARQUEO 4: disponible &#128664</h3> ";
  else text += "<h3 style=\"text-align: center;\">&nbsp;PARQUEO 4: ocupado</h3> ";

  if (parqueos2[0]==0) text += "<h3 style=\"text-align: center;\">&nbsp;PARQUEO 5: disponible &#128664</h3> ";
  else text += "<h3 style=\"text-align: center;\">&nbsp;PARQUEO 5: ocupado</h3> ";
  
  if (parqueos2[1]==0) text += "<h3 style=\"text-align: center;\">&nbsp;PARQUEO 6: disponible &#128664</h3> ";
  else text += "<h3 style=\"text-align: center;\">&nbsp;PARQUEO 6: ocupado</h3  > ";
  
  if (parqueos2[2]==0) text += "<h3 style=\"text-align: center;\">&nbsp;PARQUEO 7: disponible &#128664</h3> ";
  else text += "<h3 style=\"text-align: center;\">&nbsp;PARQUEO 7: ocupado</h3> ";
  
  if (parqueos2[3]==0) text += "<h3 style=\"text-align: center;\">&nbsp;PARQUEO 8: disponible &#128664</h3> ";
  else text += "<h3 style=\"text-align: center;\">&nbsp;PARQUEO 8: ocupado</h3> ";


  
  text += "</html>";
    
  server.send(200, "text/html", text);
}

void handle_NotFound() {
  server.send(404, "text/plain", "Not found");
}
