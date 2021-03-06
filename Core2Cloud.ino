/*
   HTTP Client for Pushing data form Core HW into Sensors2cloud to build scalable IoT apps
   Author: Sensors2Cloud Inc.,
   THIS SOFTWARE IS PROVIDED TO YOU "AS IS," AND WE MAKE NO EXPRESS OR IMPLIED WARRANTIES 
   WHATSOEVER WITH RESPECT TO ITS FUNCTIONALITY,   OPERABILITY, OR USE, INCLUDING, WITHOUT LIMITATION, 
   ANY IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, OR INFRINGEMENT. 
   WE EXPRESSLY DISCLAIM ANY LIABILITY WHATSOEVER FOR ANY DIRECT, INDIRECT, CONSEQUENTIAL, INCIDENTAL 
   OR SPECIAL DAMAGES, INCLUDING, WITHOUT LIMITATION, LOST REVENUES, LOST PROFITS, LOSSES RESULTING 
   FROM BUSINESS INTERRUPTION OR LOSS OF DATA, REGARDLESS OF THE FORM OF ACTION OR LEGAL THEORY 
   UNDER WHICH THE LIABILITY MAY BE ASSERTED, EVEN IF ADVISED OF THE POSSIBILITY OR LIKELIHOOD OF SUCH DAMAGES.
*/

#include "HttpClient/HttpClient.h"

unsigned int nextTime = 0;    // Next time to contact the server
int incomingByte = 0;   // for incoming serial data
int totalCount = 0; 
int AIN0;   // holds value of analog A0
int AIN1;   // holds value of analog A1
int AIN2;   // holds value of analog A2
int AIN3;   // holds value of analog A3
int AIN4;   // holds value of analog A4
int AIN5;   // holds value of analog A5
int AIN6;   // holds value of analog A6
int AIN7;   // holds value of analog A7
HttpClient http;
char params[512];             // holds data
char chanBuff[65];            // holds channel data
char securekey[] = "SENSORS2CLOUD-KEY-HERE";  // Obtain from Device Manager -> Edit Gateway -> Copy Secure Key then paste here
char macaddr[]   = "CORE-DEVICE-ID";       // DEVICE ID of your Core Device

// Headers currently need to be set at init, useful for API keys etc.
http_header_t headers[] = {
    { "Content-Type", "application/json" },
    { "securekey" , securekey },
    { "macaddr" ,  macaddr},
   // { "Content-Type", "text/xml" },
    { "Accept" , "*/*"},
    { NULL, NULL } // NOTE: Always terminate headers will NULL
};

http_request_t request;
http_response_t response;

void setup() 
{
    pinMode(A0,INPUT);  // initialize A0 as input
    pinMode(A1,INPUT);  // initialize A1 as input
    pinMode(A2,INPUT);  // initialize A2 as input
    pinMode(A3,INPUT);  // initialize A3 as input
    pinMode(A4,INPUT);  // initialize A4 as input
    pinMode(A5,INPUT);  // initialize A5 as input
    pinMode(A6,INPUT);  // initialize A6 as input
    pinMode(A7,INPUT);  // initialize A7 as input
    Serial.begin(9600);
}

void loop() 
{
    AIN0 = analogRead(A0);
    AIN1 = analogRead(A1);
    AIN2 = analogRead(A2);
    AIN3 = analogRead(A3);
    AIN4 = analogRead(A4);
    AIN5 = analogRead(A5);
    AIN6 = analogRead(A6);
    AIN7 = analogRead(A7);
    if (nextTime > millis()) 
    {
        return;
    }
    // send data only when you receive data:
    if (Serial.available() > 0) 
    {
        // read the incoming byte:
        incomingByte = Serial.read();
        // say what you got:
        Serial.print("I received: ");
        Serial.println(incomingByte, DEC);
    }
    Serial.println();
    Serial.println("Application>\tStart of Loop.");
    // Request path and body can be set at runtime or at setup.
    request.hostname = "developer.sensors2cloud.com";
    request.port = 80;
    request.path = "/arduino";

    // The library also supports sending a body with your request:
      // JSON starts here
      sprintf(params,"%s","{"); 
      sprintf(params,"%s\"macaddr\":\"%s\"",params,macaddr);
      sprintf(params,"%s,\"uptime\":%i",params,55); 
      sprintf(params,"%s,\"sensorList\":%s",params,"[{");
      sprintf(params,"%s\"name\":%s",params,"\"Core0\"");
      sprintf(params,"%s,\"type\":%s",params,"\"Core\""); 
      sprintf(params,"%s,\"channels\":%s",params,"[");
      // AIN0 channel
      sprintf(chanBuff,"{\"name\":\"%s\",\"value\":\"%i\",\"unit\":\"%s\",\"time\":\"%i\"}","A0",AIN0,"V",totalCount);
      sprintf(params,"%s%s,",params,chanBuff);
      // AIN1 channel
      sprintf(chanBuff,"{\"name\":\"%s\",\"value\":\"%i\",\"unit\":\"%s\",\"time\":\"%i\"}","A1",AIN1,"V",totalCount);
      sprintf(params,"%s%s,",params,chanBuff);
      // AIN2 channel
      sprintf(chanBuff,"{\"name\":\"%s\",\"value\":\"%i\",\"unit\":\"%s\",\"time\":\"%i\"}","A2",AIN2,"V",totalCount);
      sprintf(params,"%s%s,",params,chanBuff);
      // AIN3 channel
      sprintf(chanBuff,"{\"name\":\"%s\",\"value\":\"%i\",\"unit\":\"%s\",\"time\":\"%i\"}","A3",AIN3,"V",totalCount);
      sprintf(params,"%s%s,",params,chanBuff);
      // AIN4 channel
      sprintf(chanBuff,"{\"name\":\"%s\",\"value\":\"%i\",\"unit\":\"%s\",\"time\":\"%i\"}","A4",AIN4,"V",totalCount);
      sprintf(params,"%s%s,",params,chanBuff); 
      // AIN5 channel
      sprintf(chanBuff,"{\"name\":\"%s\",\"value\":\"%i\",\"unit\":\"%s\",\"time\":\"%i\"}","A5",AIN5,"V",totalCount);
      sprintf(params,"%s%s,",params,chanBuff); 
      // AIN6 channel
      sprintf(chanBuff,"{\"name\":\"%s\",\"value\":\"%i\",\"unit\":\"%s\",\"time\":\"%i\"}","A6",AIN6,"V",totalCount);
      sprintf(params,"%s%s,",params,chanBuff); 
       // AIN7 channel
      sprintf(chanBuff,"{\"name\":\"%s\",\"value\":\"%i\",\"unit\":\"%s\",\"time\":\"%i\"}","A7",AIN7,"V",totalCount);
      sprintf(params,"%s%s",params,chanBuff);     // Notice in the last channel, the %s%s without the comma at the end
      // Complete JSON
      sprintf(params,"%s%s",params,"]}]}"); 
    request.body = params;

    // POST request
    http.post(request, response, headers);
    Serial.print("Application>\tResponse status: ");
    Serial.println(response.status);

    Serial.print("Application>\tHTTP Response Body: ");
    Serial.println(response.body);

    nextTime = millis() + 10000;
    totalCount++;
}
