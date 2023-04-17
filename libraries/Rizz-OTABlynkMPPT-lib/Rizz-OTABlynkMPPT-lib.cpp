#include "Arduino.h"
#include "Rizz-OTABlynkMPPT-lib.h"

AsyncWebServer server(80);
WebSocketsServer webSocket = WebSocketsServer(81);
             
// akses               192.168.4.1


char _webpage[] PROGMEM = R"=====(
<html>
<head>
<meta charset="utf-8">
<title>Login</title>
<meta http-equiv="pragma" content="no-cache" />
<meta http-equiv="expires" content="-1" />	
<meta name="viewport" content="width=device-width; initial-scale=1.0; maximum-scale=1.0; user-scalable=0" />
<link rel="stylesheet" media="screen">

<script>
var connection = new WebSocket('ws://'+location.hostname+':81/');

connection.onopen = function () 
{  
connection.send('Connect ' + new Date()); };
 connection.onerror = function (error) 
 {   
  console.log('WebSocket Error ', error);
 };
 connection.onmessage = function (e) 
  {  
    console.log('Server: ', e.data);
  };
</script>

</head>
<body class='login'>
<!--
$(if chap-id)
	<form name="sendin" action="$(link-login-only)" method="post">
		<input type="hidden" name="username" />
		<input type="hidden" name="password" />
		<input type="hidden" name="token_blynk" />
		<input type="hidden" name="dst" value="$(link-orig)" />
		<input type="hidden" name="popup" value="true" />
	</form>
	<script type="text/javascript" src="md5.js"></script>-->
	<script type="text/javascript">
	<!--
	    function credentials_rec()
{  var ssid = document.getElementById('ssid_cred').value;
  var pass = document.getElementById('pass_cred').value;
  var auth = document.getElementById('token_blynk').value;
  var full_command = '#{"ssid":"'+ ssid +'", "pass":"' +pass +'","auth":"'+ auth +'"}';
  console.log(full_command);
  connection.send(full_command);
  location.replace('http://'+location.hostname+'/submit');
  return false
}
	//--></script>

<!--$(endif)-->

<style>
body {
padding:0;margin:0 auto;
    background:#fff;
	min-width: 300px;
}
#head{
    display: block;
    background: #607D8B;
    width: 100%;
    position:relative;
    top:-27px;
    box-shadow: 0px 3px 3px #9a9a9a;
}
#logo{
position: relative;
    top:0px;
}

legend{
    position:relative;
    top:-27px;
    color:#0baac0;
    font-size: 25px;
    font-family:sans-serif;
    
}
#box{
    position:relative;
    top:-27px;
	width:98%;
	max-width:514px;
	height:230px;	
	background-color:#0bb3c7;
	border-radius:5px;
	padding:40px 0 0 0;
	margin:0px auto;
    box-shadow: 2px 3px 3px #9a9a9a;


}


/**------------USER------------**/
#ssid_cred{
border:1px #ccc solid;color:#6D6D6D;background:#fff;opacity: 1;
height:40px;font-size:16px;
display:block;width:230px;
text-align:center;border-radius:5px;
font-family:inherit
}
/**--------------------PASSWORD----------**/
#pass_cred{
border:1px #ccc solid;color:#6D6D6D;background:#fff;opacity: 1;height:40px;font-size:16px;
display:block;margin:5px;width:230px;text-align:center;border-radius:5px;font-family:inherit
}
/**--------------------TokenBlynk----------**/
#token_blynk{
border:1px #ccc solid;color:#6D6D6D;background:#fff;opacity: 1;height:40px;font-size:16px;
display:block;margin:5px;width:230px;text-align:center;border-radius:5px;font-family:inherit
}
/*---------------Boton---------*/
#boton{
	-webkit-appearance:none;color:#fff;
	background:#0da2b4; background-image:url(data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAABgAAAAYCAQAAABKfvVzAAAAj0lEQVR4AcXPPU6FQBiGUXB5JGQUdmXiopTNzI8xFJpbHqkonNyBGIzvU32EU0x3wcwWt603T7qfVR+8gHULno/AjHeDfmv0gdAGC4b9esRrG9ys+v168OmrAs0il4FJcW9ZqEHRWqoBdHfCf4NJkYXzoIB0HmQQz4MgicbrH/3HIGst1iBIjd/HGhz3O/ANxFCpjJsKZlQAAAAASUVORK5CYII=);
	background-repeat: no-repeat; background-position:center; opacity: 1;
    border:none;border-radius:50%;height:70px;width:70px;font-size:16px;font-weight:400;outline:0;text-transform:uppercase;
    letter-spacing:1.5px;cursor:pointer
    }
#boton:hover{
	background-color:#607D8B;-webkit-transition:all 0.3s ease;-moz-transition:all 0.3s ease;-o-transition:all 0.3s ease;transition:all 0.3s ease
	}
    .vertical-form .footer{
        position: relative;
        top:-37px;
        text-align:center;
color:#ccc;}
.vertical-form .footer a{text-decoration:none;color:#4285F4;}
    @media (max-width:450px){

       }
</style>

<form class="vertical-form" name="login" action="$(link-login-only)" method="post" background="#A03472"
	$(if chap-id) onSubmit="return credentials_rec()" $(endif)>
<input type="hidden" name="dst" value="$(link-orig)" />
<input type="hidden" name="popup" value="true" />
<div style="margin:0;padding:50;display:inline"></div>
<center>
    <div id="head">
    <img id="logo" src="data:image/png;base64,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"/> </div>
    <legend>
    <p>Mau dihubungkan kemana?</p>
    </legend>
<div id="box">
<div><input id="ssid_cred" autocomplete="on" name="ssid_cred" type="text" placeholder="Nama Hotspot"/></div>
<div><input id="pass_cred" autocomplete="off" name="pass_cred" type="password" label="false" placeholder="Passwordnya"/></div>
<div><input id="token_blynk" autocomplete="off" name="token_blynk" type="text" label="false" placeholder="Token Blynk"/></div>
<div><input id="boton" name="submit" type="submit" value=" " /></div>
</div>
</center>
</form>
</body>
</html>


)=====";



char _loginpage[] PROGMEM = R"=====(
<html>
<head>
<meta charset="utf-8">
<title>Login</title>
<meta http-equiv="pragma" content="no-cache" />
<meta http-equiv="expires" content="-1" />	
<meta name="viewport" content="width=device-width; initial-scale=1.0; maximum-scale=1.0; user-scalable=0" />
<link rel="stylesheet" media="screen">
</head>

<body>
<p>Tunggu sampai anda berhasil Login</p>
</body>

</html>

)=====";

void _webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length)
{

  switch (type) {
    case WStype_DISCONNECTED:
      Serial.printf("[%u] Disconnected!\n", num);
      break;
    case WStype_CONNECTED: {
        IPAddress ip = webSocket.remoteIP(num);
        Serial.printf("[%u] Connected from %d.%d.%d.%d url: %s\n", num, ip[0], ip[1], ip[2], ip[3], payload);

        // send message to client
        webSocket.sendTXT(num, "Connected");
      }
      break;
    case WStype_TEXT:
      Serial.printf("[%u] get Text: %s\n", num, payload);

      if (payload[0] == '#') 
      {
        String message = String((char*)( payload));
        message = message.substring(1);
        Serial.println(message);

        //JSON part
        DynamicJsonDocument doc(1024);
        DeserializationError error = deserializeJson(doc, message);

        String ssid = doc["ssid"];
        String pass = doc["pass"];
        String auth = doc["auth"];
        Serial.println(ssid); Serial.println(pass);


        // Clearing EEPROM
        if (ssid.length() > 0 && pass.length() > 0) {
          Serial.println("clearing eeprom");
          for (int i = 0; i < 100; ++i) {
            EEPROM.write(i, 0);
          }


          // Storing in EEPROM
          Serial.println("writing eeprom ssid:");
          for (int i = 0; i < ssid.length(); ++i)
          {
            EEPROM.write(i, ssid[i]);
            Serial.print("Wrote: ");
            Serial.println(ssid[i]);
          }
          Serial.println("writing eeprom pass:");
          for (int i = 0; i < pass.length(); ++i)
          {
            EEPROM.write(32 + i, pass[i]);
            Serial.print("Wrote: ");
            Serial.println(pass[i]);
          }
          Serial.println("writing eeprom auth token:");
          for (int i = 0; i < auth.length(); ++i)
          {
            EEPROM.write(64 + i, auth[i]);
            Serial.print("Wrote: ");
            Serial.println(auth[i]);
          }
          EEPROM.commit();
          delay(2000);

          //Restarting ESP board
          ESP.restart();
          break;
        }
     }
  }
}



void credentials::Erase_eeprom()
{
  EEPROM.begin(512); //Initialasing EEPROM
  Serial.println("Erasing...");
  Serial.println("clearing eeprom");
    for (int i = 0; i < 100; ++i) 
    {
      EEPROM.write(i, 0);
    }
   EEPROM.commit();
}



String credentials::EEPROM_Config()
{
 EEPROM.begin(512); //Initialasing EEPROM
  Serial.println();
  Serial.println();

  //---------------------------------------- Read eeprom for ssid and pass
  Serial.println("Reading EEPROM");


  for (int i = 0; i < 32; ++i)
  {
    ssid += char(EEPROM.read(i));
  }
  Serial.print("SSID: ");
  Serial.println(ssid);

  for (int i = 32; i < 64; ++i)
  {
    pass += char(EEPROM.read(i));
  }

  Serial.print("Password: ");
  Serial.println(pass);
  String token_blynk = "";
  for (int i = 64; i < 100; ++i)
  {
    token_blynk += char(EEPROM.read(i));
  }
  Serial.print("Auth Token: ");
  Serial.println(token_blynk);
  return token_blynk;
}



bool credentials::credentials_get()
{
  if (_testWifi())
  {
    Serial.println("Succesfully Connected!!!");
    return true;
  }
  else
  {
    Serial.println("Turning the HotSpot On");
    return false;
  }
}



void credentials::setupAP(char* softap_ssid, char* softap_pass)
{
  
  WiFi.disconnect();
  delay(100);
  WiFi.softAP(softap_ssid,softap_pass);
  Serial.println("softap");
  _launchWeb();
  Serial.println("Server Started");
   webSocket.begin();
    webSocket.onEvent(_webSocketEvent);
}



bool credentials::_testWifi()
{
  int c = 0;
  Serial.println("Waiting for Wifi to connect");
  char* my_ssid = &ssid[0];
  char* my_pass = &pass[0];

  WiFi.begin(my_ssid, my_pass);
  while ( c < 20 ) {
    if (WiFi.status() == WL_CONNECTED)
    {
      return true;
    }
    delay(500);
    Serial.print("*");
    c++;
  }
  Serial.println("");
  Serial.println("Connect timed out, opening AP");
  return false;
}



// This is the function which will be called when an invalid page is called from client
void notFound(AsyncWebServerRequest *request)
{
  request->send(404, "text/plain", "Not found");
}


void credentials::_createWebServer()
{
server.on("/", [](AsyncWebServerRequest * request) {
  request->send_P(200, "text/html", _webpage);
});

// Send a GET request to <IP>/get?message=<message>
server.on("/submit", HTTP_GET, [] (AsyncWebServerRequest * request) {
   request->send(200, "text/html", _loginpage);
});

server.onNotFound(notFound);
server.begin();
  
}

void credentials::_launchWeb()
{
  Serial.println("");
  Serial.print("SoftAP IP: ");
  Serial.println(WiFi.softAPIP());
  _createWebServer();
  // Start the server

}

void credentials::server_loops()
{ 
     webSocket.loop();
}
