// Proyecto 4
// Mario Estrada - 18123
/***********
  ESP32 Web Server
  Ejemplo de creación de Web server 
  Basándose en los ejemplos de: 
  https://lastminuteengineers.com/creating-esp32-web-server-arduino-ide/
  https://electropeak.com/learn
************/
//************
// Librerías
//************
#include <WiFi.h>
#include <WebServer.h>
//************
// Variables globales
//************
// SSID & Password
const char* ssid = "TURBONETT_4BF538";  // Enter your SSID here
const char* password = "859B594874";  //Enter your Password here
WebServer server(80);  // Object of WebServer(HTTP port, 80 is defult)
uint8_t Push[4];
uint8_t Dispon = 0;
uint8_t LED1pin = 2;
bool LED1status = LOW;


void setup() {
  Serial.begin(115200);
  Serial2.begin(115200);
  Serial.println("Try Connecting to ");
  Serial.println(ssid);
  pinMode(LED1pin, OUTPUT);
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
  server.on("/", handle_OnConnect); 
  server.on("/datos", handle_Data); // handler para enviar datos
  server.onNotFound(handle_NotFound);
  server.begin();
  Serial.println("HTTP server started");
  delay(100);
}

void loop() {
  server.handleClient();          
  if (Serial2.available())        
    {   
      digitalWrite(2, 1);         
        for (int i = 0; i<=3; i++)
        {
          Push[i] = Serial2.read(); 
        }
        Dispon = (((4 - Push[0])- Push[1])- Push[2])- Push[3];  
    }
    digitalWrite(2, 0);        
}


void handle_OnConnect() {
  server.send(200, "text/html", SendHTML());
}


void handle_Data() {                          
  server.send(200, "application/json", data_json());
}


String SendHTML(void) {
  String ptr = "<!DOCTYPE html>\n";
  ptr = "<html>\n";
  ptr += "<body>\n";
 // Formato del servidor
  ptr +="<body>\n";
  ptr +="<body style=\"background-color:#F0FFF0;\">\n";
  ptr +="</body>\n";

  ptr +="<style>\n";
  ptr +="h1 {\n";
  ptr +="  text-align: center;\n";
  ptr +="  font-size: 300%;\n";
  ptr +="}\n";
  ptr +="h3 {\n";
  ptr +="  text-align: center;\n";
  ptr +="  font-size: 100%;\n";
  ptr +="}\n";
  ptr += "table {\n";
  ptr += "  width: 50%;\n";
  ptr += "  height: 200px;\n";
  ptr += "}\n";
  ptr +="</style>\n";
  ptr += "<h1>Control de estacionamientos</h1>\n";

//Tabla de control de parqueos

  ptr += "<table border=\"1\" style=\"margin: 0 auto\"; \n";
  ptr += "  <tr>\n";
  ptr += "    <th># de parqueo</th>\n";
  ptr += "    <th>Parqueo 1</th>\n";
  ptr += "    <th>Parqueo 2</th>\n";
  ptr += "    <th>Parqueo 3</th>\n";
  ptr += "    <th>Parqueo 4</th>\n";
  ptr += "  </tr>\n";
  ptr += "  <tr>\n";
  ptr += "    <td>Disponibilidad</td>\n";
  ptr += "    <td><canvas id=\"Parqueo 1\" width=\"200\" height=\"100\" style=\"border:0px solid #000000;\">\n";
  ptr += "</canvas>\n";
  ptr += "</td>\n";
  ptr += "    <td><canvas id=\"Parqueo 2\" width=\"200\" height=\"100\" style=\"border:0px solid #000000;\">\n";
  ptr += "</canvas>\n";
  ptr += "</td>\n";
  ptr += "    <td><canvas id=\"Parqueo 3\" width=\"200\" height=\"100\" style=\"border:0px solid #000000;\">\n";
  ptr += "</canvas>\n";
  ptr += "</td>\n";
  ptr += "    <td><canvas id=\"Parqueo 4\" width=\"200\" height=\"100\" style=\"border:0px solid #000000;\">\n";
  ptr += "</canvas>\n";
  ptr += "</td>\n";
  ptr += "  </tr>\n";
  ptr += "</table>";
  ptr += "<h3>Verde = Disponible</h3>\n";
  ptr += "<h3>Rojo = Ocupado</h3>\n";
  ptr += "<canvas id=\"Cantidad\" width=\"900\" height=\"60\" style=\"border:0px solid #000000;\">\n";
  ptr += "</canvas>\n";
  ptr += "<script>\n";
  
  //Funcion de control de botones
  ptr += "function boton(n_parqueo, valor){\n";
  ptr += "var canvas = document.getElementById(n_parqueo);\n";
  ptr += "var ctx = canvas.getContext(\"2d\");\n";
  ptr += "if (valor == 0){\n";
  ptr += "ctx.fillStyle = \"#7CFC00\";\n";
  ptr += "};\n";
  ptr += "if (valor == 1){\n";
  ptr += "ctx.fillStyle = \"#fc0303\";\n";
  ptr += "};\n";
  ptr += "ctx.fillRect(0,0,200,100);\n";
  ptr += "ctx.fillStyle = \"#000000\";\n";
  ptr += "};\n";

 //Funcion de lugares disponibles
  ptr += "function Estacionamiento(cantidad){\n";
  ptr += "var canvas = document.getElementById(\"Cantidad\");\n";
  ptr += "var ctx = canvas.getContext(\"2d\");\n";
  ptr += "ctx.fillStyle = \"#F0FFF0\";\n";
  ptr += "ctx.fillRect(0,0,825,40);\n";
  ptr += "ctx.fillStyle = \"#000000\";\n";
  ptr += "};\n";

//Funcion de envio al WebServer
  ptr += "Estacionamiento(4);\n";
  ptr += "boton(\"Parqueo 1\", 0);\n";
  ptr += "boton(\"Parqueo 2\", 0);\n";
  ptr += "boton(\"Parqueo 3\", 0);\n";
  ptr += "boton(\"Parqueo 4\", 0);\n";
  ptr += "var sendHttpRequest = function(){\n";
  ptr += "var xhr = new XMLHttpRequest();\n";
  ptr += "xhr.open(\"GET\", \"http://192.168.1.2/datos\");\n";
  ptr += "xhr.responseType = \'json\';\n";

//Asignacion de botones a cuadros de parqueo
  ptr += "xhr.onload = function() {\n";
  ptr += "  console.log(xhr.response);\n";
  ptr += "Estacionamiento(xhr.response.Espacios);\n";
  ptr += "boton(\"Parqueo 1\", xhr.response.lugar1);\n";
  ptr += "boton(\"Parqueo 2\", xhr.response.lugar2);\n";
  ptr += "boton(\"Parqueo 3\", xhr.response.lugar3);\n";
  ptr += "boton(\"Parqueo 4\", xhr.response.lugar4);\n"; 
  ptr += "};\n";
  ptr += "xhr.send();\n"; 
  ptr += "return xhr.response;\n";
  ptr += "};\n"; 
  ptr += "setInterval(function(){\n";
  ptr += "sendHttpRequest();\n";
  ptr += "},1);\n";
  ptr += "</script>\n";
  ptr += "</body>\n";
  ptr += "</html>\n";
  return ptr;
}

void handle_NotFound() {
  server.send(404, "text/plain", "Not found");
}
String data_json(void){
  String dato_json = "{\"lugar1\":";
          dato_json += Push[0];
          dato_json += ",";
          dato_json += "\"lugar2\":";
          dato_json += Push[1];
          dato_json += ",";
          dato_json += "\"lugar3\":";
          dato_json += Push[2];
          dato_json += ",";
          dato_json += "\"lugar4\":";
          dato_json += Push[3];
          dato_json += ",";
          dato_json += "\"Espacios\":";
          dato_json += Dispon;
          dato_json += "}";
  return dato_json;
}
