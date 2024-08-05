#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ArduinoJson.h>

Adafruit_MPU6050 mpu;
ESP8266WebServer server(80);

const char* ssid = "iitk";
const char* password = "";

void handleRoot();
void handleData();
void handleNotFound();

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  server.on("/", HTTP_GET, handleRoot);
  server.on("/data", HTTP_GET, handleData);
  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");

  if (!mpu.begin()) {
    Serial.println("MPU6050 connection failed");
    while (1);
  }
}

void loop() {
  server.handleClient();
}

void handleRoot() {
  String html = R"=====(
<!DOCTYPE HTML>
<html>
<head>
  <title>Gyro</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <link rel="icon" href="data:,">
  <link rel="stylesheet" href="https://use.fontawesome.com/releases/v5.7.2/css/all.css" integrity="sha384-fnmOCqbTlWIlj8LyTjo7mOUStjsKC4pOpQbqyi7RrhN7udi9RwhKkMHpvLbHG9Sr" crossorigin="anonymous">
  <script src="https://cdnjs.cloudflare.com/ajax/libs/three.js/107/three.min.js"></script>
  <style>
    html {
      font-family: Arial;
      display: inline-block;
      text-align: center;
    }
    p {
      font-size: 1.2rem;
    }
    body {
      margin: 0;
    }
    .topnav {
      overflow: hidden;
      background-color: #de9733;
      color: #1c1a14;
      font-size: 1rem;
    }
    .content {
      padding: 20px;
    }
    .card {
      background-color: white;
      box-shadow: 2px 2px 12px 1px rgba(140,140,140,.5);
    }
    .card-title {
      color: #de9733;
      font-weight: bold;
    }
    .cards {
      max-width: 800px;
      margin: 0 auto;
      display: grid;
      grid-gap: 2rem;
      grid-template-columns: repeat(auto-fit, minmax(200px, 1fr));
    }
    .reading {
      font-size: 1.2rem;
    }
    .cube-content {
      width: 100%;
      background-color: white;
      height: 300px;
      margin: auto;
      padding-top: 2%;
    }
    #reset, #resetX, #resetY, #resetZ {
      border: none;
      color: #FEFCFB;
      background-color: #de9733;
      padding: 10px;
      text-align: center;
      display: inline-block;
      font-size: 14px;
      border-radius: 4px;
    }
    #reset {
      width: 150px;
    }
    #resetX, #resetY, #resetZ {
      width: 20px;
      padding-top: 10px;
      padding-bottom: 10px;
    }
  </style>
</head>
<body>
  <div class="topnav">
    <h1> Gyroscope 3D Virtualization </h1>
  </div>
  <div class="content">
    <div>
      <label><input type="radio" name="dataType" value="gyro" checked onclick="toggleSection('gyro')"> Gyroscope</label>
      <label><input type="radio" name="dataType" value="accel" onclick="toggleSection('accel')"> Accelerometer</label>
      <label><input type="radio" name="dataType" value="temp" onclick="toggleSection('temp')"> Temperature</label>
      <label><input type="radio" name="dataType" value="all" onclick="toggleSection('all')"> All</label>
    </div>
    <div class="cards">
      <div class="card" id="gyroCard">
        <p class="card-title">GYROSCOPE</p>
        <p><span class="reading">X: <span id="gyroX"></span> rad</span></p>
        <p><span class="reading">Y: <span id="gyroY"></span> rad</span></p>
        <p><span class="reading">Z: <span id="gyroZ"></span> rad</span></p>
      </div>
      <div class="card" id="accelCard">
        <p class="card-title">ACCELEROMETER</p>
        <p><span class="reading">X: <span id="accX"></span> ms<sup>2</sup></span></p>
        <p><span class="reading">Y: <span id="accY"></span> ms<sup>2</sup></span></p>
        <p><span class="reading">Z: <span id="accZ"></span> ms<sup>2</sup></span></p>
      </div>
      <div class="card" id="tempCard">
        <p class="card-title">TEMPERATURE</p>
        <p><span class="reading"><span id="temp"></span> &deg;C</span></p>
        <p class="card-title">3D</p>
        <button id="reset" onclick="resetPosition(this)">RESET POSITION</button>
        <button id="resetX" onclick="resetPosition(this)">X</button>
        <button id="resetY" onclick="resetPosition(this)">Y</button>
        <button id="resetZ" onclick="resetPosition(this)">Z</button>
      </div>
    </div>
    <div class="cube-content">
      <div id="3Dcube"></div>
    </div>
  </div>
  <script>
    let scene, camera, renderer, cube;

    function parentWidth(elem) {
      return elem.parentElement.clientWidth;
    }

    function parentHeight(elem) {
      return elem.parentElement.clientHeight;
    }

    function init3D() {
      scene = new THREE.Scene();
      scene.background = new THREE.Color(0xffffff);

      camera = new THREE.PerspectiveCamera(75, parentWidth(document.getElementById("3Dcube")) / parentHeight(document.getElementById("3Dcube")), 0.1, 1000);

      renderer = new THREE.WebGLRenderer({ antialias: true });
      renderer.setSize(parentWidth(document.getElementById("3Dcube")), parentHeight(document.getElementById("3Dcube")));

      document.getElementById('3Dcube').appendChild(renderer.domElement);

      const geometry = new THREE.BoxGeometry(5, 1, 4);
      var cubeMaterials = [
        new THREE.MeshBasicMaterial({ color: 0x03045e }),
        new THREE.MeshBasicMaterial({ color: 0x023e8a }),
        new THREE.MeshBasicMaterial({ color: 0x0077b6 }),
        new THREE.MeshBasicMaterial({ color: 0x03045e }),
        new THREE.MeshBasicMaterial({ color: 0x023e8a }),
        new THREE.MeshBasicMaterial({ color: 0x0077b6 }),
      ];
      const material = new THREE.MeshFaceMaterial(cubeMaterials);
      cube = new THREE.Mesh(geometry, material);
      scene.add(cube);
      camera.position.z = 5;
      renderer.render(scene, camera);
    }

    function onWindowResize() {
      camera.aspect = parentWidth(document.getElementById("3Dcube")) / parentHeight(document.getElementById("3Dcube"));
      camera.updateProjectionMatrix();
      renderer.setSize(parentWidth(document.getElementById("3Dcube")), parentHeight(document.getElementById("3Dcube")));
    }

    window.addEventListener('resize', onWindowResize, false);

    function fetchData() {
      fetch('/data')
        .then(response => response.json())
        .then(data => {
          document.getElementById("gyroX").innerHTML = data.gyroX.toFixed(2);
          document.getElementById("gyroY").innerHTML = data.gyroY.toFixed(2);
          document.getElementById("gyroZ").innerHTML = data.gyroZ.toFixed(2);
          document.getElementById("accX").innerHTML = data.accelX.toFixed(2);
          document.getElementById("accY").innerHTML = data.accelY.toFixed(2);
          document.getElementById("accZ").innerHTML = data.accelZ.toFixed(2);
          document.getElementById("temp").innerHTML = data.temp.toFixed(2);
          cube.rotation.x = data.gyroY;
          cube.rotation.z = data.gyroX;
          cube.rotation.y = data.gyroZ;
          renderer.render(scene, camera);
        })
        .catch(error => console.log(error));
    }

    setInterval(fetchData, 1000);

    function resetPosition(element) {
      var xhr = new XMLHttpRequest();
      xhr.open("GET", "/reset", true);
      xhr.send();
    }

    function toggleSection(section) {
      document.getElementById('gyroCard').style.display = section === 'gyro' || section === 'all' ? 'block' : 'none';
      document.getElementById('accelCard').style.display = section === 'accel' || section === 'all' ? 'block' : 'none';
      document.getElementById('tempCard').style.display = section === 'temp' || section === 'all' ? 'block' : 'none';
    }

    document.addEventListener('DOMContentLoaded', function() {
      init3D();
    });
  </script>
</body>
</html>
  )=====";
  server.send(200, "text/html", html);
}

void handleData() {
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  StaticJsonDocument<200> jsonDoc;
  jsonDoc["accelX"] = a.acceleration.x;
  jsonDoc["accelY"] = a.acceleration.y;
  jsonDoc["accelZ"] = a.acceleration.z;
  jsonDoc["gyroX"] = g.gyro.x; // Yaw
  jsonDoc["gyroY"] = g.gyro.y; // Pitch
  jsonDoc["gyroZ"] = g.gyro.z; // Roll
  jsonDoc["temp"] = temp.temperature;

  String jsonString;
  serializeJson(jsonDoc, jsonString);

  server.send(200, "application/json", jsonString);
}

void handleNotFound() {
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";

  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }

  server.send(404, "text/plain", message);
}
