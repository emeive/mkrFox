#include <SigFox.h>
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

#define SEALEVELPRESSURE_HPA (1013.25)
 int8_t temperatura, temperaturaInterna, humedad;
 int altura;
 float presion;
 int16_t presion1;

 
Adafruit_BME280 bme; // I2C
unsigned status;

void setup() {
  // Configuración del monitor serie
  Serial.begin(9600);
  delay(500);
  status = bme.begin(); 
  // Iniciamos módulo SigFox
  // Esto lo hacemos para comprobar que podemos conectarnos a SigFox
  // En caso contrario no tiene sentido continuar
  if (!SigFox.begin()) {
    Serial.println("Error al iniciar módulo SigFox");
    return;

  }
  // Apagamos el módulo
  SigFox.end();
  // Activamos el modo debug en el módulo SigFox
  SigFox.debug();
}
 
void loop() {
  // Iniciamos el módulo SigFox
  SigFox.begin();
  // Esperamos 100 ms hasta que se conecte
  delay(100);
 
  // Leemos sensores
  temperaturaInterna = (int)SigFox.internalTemperature();
  temperatura = bme.readTemperature();
  presion = (bme.readPressure()/100.0F)*10;
  humedad = bme.readHumidity();
  altura = bme.readAltitude(SEALEVELPRESSURE_HPA);

  presion1 = presion/10;
 
  // Limpieza de todas las interrupciones pendientes
  SigFox.status();
  delay(1);
 
  // Comenzamos el proceso para transmitir el mensaje
  SigFox.beginPacket();
 
  //Escribimos el mensaje
  SigFox.write(presion1);
  SigFox.write(temperatura);
  SigFox.write(humedad);
  SigFox.write(temperaturaInterna);
  SigFox.write(altura);
  
  
  // Terminamos el proceso de enviar el mensaje
  int resultado = SigFox.endPacket();
 
  // Comprobamos el resultado
  if(resultado == 0){
    Serial.println("Mensaje enviado !!!");
  }else{
    Serial.println("Error al enviar el mensaje");
  }

  // Apagamos el módulo
  SigFox.end();
 
  // Esperamos 15 minutos hasta enviar el siguiente mensaje
  printValues(temperatura, temperaturaInterna, presion1, humedad, altura);
  delay(600000);
}

void printValues(int8_t temperatura, int8_t temperaturaInterna,  int16_t presion1, int8_t humedad, int altura ) {
    Serial.print("Temperatura = ");
    Serial.print(temperatura);
    Serial.println(" °C");

    Serial.print("Temperatura Sigfox = ");
    Serial.print(temperaturaInterna);
     
    Serial.println(" °C");

    Serial.print("Presion = ");
    Serial.print(presion1);
    Serial.println(" hPa");

    Serial.print("Approx. Altitud = ");
    Serial.print(altura);
    Serial.println(" m");


    Serial.print("Humedad = ");
    Serial.print(humedad);
    Serial.println(" %");
   

    Serial.println();
}
