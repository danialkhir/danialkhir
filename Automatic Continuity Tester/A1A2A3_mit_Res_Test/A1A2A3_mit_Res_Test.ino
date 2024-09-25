#include <WiFi.h>
#include <ESPAsyncWebSrv.h>
#include <Wire.h>
#include <SD.h>
const char *ssid = "RFTEST";
const char *password = "12345678";
const char *fileNameR="Raw Data.csv";
const char *fileName="Resdata.csv";
File dataFileR=SD.open(fileNameR,FILE_WRITE);
File dataFile=SD.open(fileName,FILE_WRITE);
AsyncWebServer server(80);
#include "ACT.h"
void triggerFunction(AsyncWebServerRequest *request) {
    if (request->hasParam("message")) {
        String message = request->getParam("message")->value();
        if (message.equals("startTest")) {
            startContinuityTest(); // Call the function to start the continuity test
            request->send(200, "text/plain", "Test started");
            Serial.println("UWC Tester");
        }
        if(message.equals("M62497-003")) {
          performContinuityTestM62497_003();
          Serial.println("97");
        }
        if(message.equals("M62498-003")) {
          performContinuityTestM62498_003();
          Serial.println("98");
        }
        if (message.equals("startResistance")) {
            Serial.println("UWC Res Testing");
            ResTest(); // Call the function to start the continuity test
            request->send(200, "text/plain", "ResTest started");
            
        }
    }
  }

void setup() {
  Serial.begin(115200);
  Wire.begin();
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
 
  // server.on("/startTest", HTTP_GET, [](AsyncWebServerRequest *request){
  //   startContinuityTest(); // Call the function to start the continuity test
  //   request->send(200, "text/plain", "Test started");
  // });

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(200, "text/html", getHTMLContent());
  });
   server.on("/triggerFunction", HTTP_GET, [](AsyncWebServerRequest *request) {
        triggerFunction(request);
    });
  server.begin();
  Serial.println("HTTP server started");
   if (SD.begin()) {
    Serial.println("SD card initialized.");
    WriteHead();  // Write CSV header
  }
   else {
    Serial.println("SD card initialization failed.");
  }
  Serial.println(WiFi.localIP());
  }
void loop() {
  // entahle
  
  }
String getHTMLContent() {
  // letak sini web
    return R"=====( 
 <!DOCTYPE html>
  <html lang="en">
  <head>
      <meta charset="UTF-8">
      <meta name="viewport" content="width=device-width, initial-scale=1.0">
      <title>UWC Automatic Continuity Tester</title>
      <style>
          /* Global Styles */
          body {
              font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif;
              margin: 0;
              padding: 0;
              background-color: #F4FF4B;
              color: #333;
          }

          .container {
              margin: 0 auto;
              width: 80%;
              padding: 20px;
              border: 1px solid #0cdbf2;
              background-color: #0b54e7;
              box-shadow: 0 2px 4px rgba(0, 0, 0, 0.1);
          }
          h1 {
              text-align: center;
              font-size: 24px;
              margin-bottom: 20px;
              border-radius: 20px;
              background-color: #deba29;
              box-shadow: #333;
          }

          /* Form Styles */
          .inputs {
              display: flex;
              flex-direction: column;
              margin-bottom: 20px;
          }

          label {
              display: block;
              margin-bottom: 5px;
              font-size: 16px;
          }

          input {
              padding: 8px;
              border: 1px solid #ccc;
              width: 100%;
              font-size: 14px;
              margin-bottom: 10px;
              margin-right: 40px;
          }

          /* Controls Styles */
          .controls {
                      display: flex;
              justify-content: space-between;
              align-items: center;
              margin-bottom: 20px;
              margin-right: 10px;
          }

          .center-button {
              margin-top: 10px;
          }

          button {
              padding: 12px 24px;
              border: none;
              cursor: pointer;
              font-size: 16px;
              background-color: #4CAF50;
              color: #fff;
              border-radius: 4px;
              box-shadow: 0 4px 6px rgba(76, 175, 80, 0.3);
              transition: background-color 0.3s ease-in-out, transform 0.1s ease-in-out, box-shadow 0.3s ease-in-out;
          }
          button:hover {
          background-color: #388E3C;
          transform: translateY(-5px);
          box-shadow: 0 6px 8px rgba(76, 175, 80, 0.5);
      }
          
          button:disabled {
              background-color: #ddd;
              color: #666;
              cursor: not-allowed;
              box-shadow: none;
          }

          /* Status Styles */
          .status {
              text-align: center;
              margin-bottom: 20px;
          }

          h2 {
              font-size: 18px;
              margin-bottom: 10px;
          }

          /* Output Styles */
          .output {
              margin-bottom: 20px;
          }

          h3 {
              font-size: 16px;
              margin-bottom: 10px;
          }
          .powered-by {
              text-align: center;
              padding: 10px 0;
              font-size: 14px;
              color: #888;
          }
          p {
              font-size: 14px;
          }

          p.note {
              font-size: 12px;
              color: #999;
          }

          /* Additional Output Boxes */
          .additional-output {
              display: flex;
              justify-content: space-between;
          }

          .additional-output-box {
              border-radius: 20px;
              flex-basis: 48%;
              padding: 10px;
              border: 1px solid #bcbf22;
              background-color: #ed842e;
              margin-bottom: 10px;
              box-shadow: #bcbf22;
              margin-right: 5px;
          }
          .view-window {
                  overflow-y: scroll;
                  height: 300px; /* Adjust height as needed */
                  border: 1px solid #ccc;
                  border-radius: 15px;
                  padding: 10px;
                  margin-bottom: 20px;
          }
          /*Dropdown Selection*/
      </style>
  </head>
  <body>
      <div class="powered-by">
          Powered By RF EE Team
      </div>
      <div class="container">
          <h1>UWC Automatic Continuity Tester</h1>

          <div class="inputs">
              <label for="project">Project:</label>
              <input type="text" id="project" name="project" placeholder="Enter project name">

              <label for="model">Model:</label>
              <input type="text" id="model" name="model" placeholder="Enter model number">
          </div>

          <label for="CablePN">CABLE PN:</label>
          <select id="CablePN" name="CablePN">
              <option value="M62497-003">M62497-003</option>
              <option value="M62498-003">M62498-003</option>
          </select>
          
      
          <div class="inputs">
              <label for="testedBy">Tested By:</label>
              <input type="text" id="testedBy" name="testedBy" placeholder="Enter tester's name">
          </div>

          <div class="controls">
              <button id="startTest">Start Test</button>
              <div class="center-button">
                  <button id="startRes">Start Res</button>
              </div>
              <div class="center-button">
                  <button id="downloadReport" disabled>Download Report</button>
              </div>
          </div>

          <div class="status">
              <h2>Test Status: <span id="statusMessage">NULL</span></h2>
          </div>

          <div class="output">
              <div class="project">
                  <h3>Project: <span id="projectValue"></span></h3>
              </div>
              <div class="model">
                  <h3>Model: <span id="modelValue"></span></h3>
              </div>
              <div class="CablePN">
                  <h3>Cable PN: <span id="CablePNValue"></span></h3>
              </div>
              <div class="testedBy">
                  <h3>Tested By: <span id="testedByValue"></span></h3>
              </div>
          </div>

          <!-- Additional Output Boxes -->
          <div class="additional-output">
              <div class="additional-output-box" id="disconnectionBox">
                  <h3>Disconnection</h3>
                  <p id="disconnectionInfo"></p>
              </div>
              <div class="additional-output-box" id="unexpectedConnectionBox">
                  <h3>Unexpected Connection</h3>
                  <p id="unexpectedConnectionInfo"></p>
              </div>
          </div>

          <div class="controls">
              <div class="center-button" id="ReadCount">
                  <button id="ReadCount">Read Count</button>
                  <p id="ReadCountInfo"></p>
              </div>
              <div class="center-button" id="ConnectionRes">
                  <button id="ConnectionRes">Connect/Disconnect</button>
                  <p id="ConnectionInfo"></p>
              </div>
              <div class="center-button" id="ResResult">
                  <button id="ResResult">Res Test Result</button>
                  <p id="ResResultInfo"></p>
              </div>         
          </div>

          <div class="additional-output">
              <div class="additional-output-box" id="Res1Read">
                  <h3>Res1</h3>
                  <p id="Res1Info"></p>
              </div>
              <div class="additional-output-box" id="Res2Read">
                  <h3>Res2</h3>
                  <p id="Res2Info"></p>
              </div>
              <div class="additional-output-box" id="Res3Read">
                  <h3>Res3</h3>
                  <p id="Res3Info"></p>
              </div>         
          </div>
          <!-- RF EE TM -->
        
          <script>
              const startTestButton = document.getElementById('startTest');
              const downloadReportButton = document.getElementById('downloadReport');
              const startResButton = document.getElementById('startRes');
              const statusMessageElement = document.getElementById('statusMessage');
              const projectValueElement = document.getElementById('projectValue');
              const modelValueElement = document.getElementById('modelValue');
              const CableValueElement=document.getElementById('CablePNValue');
              const testedByValueElement = document.getElementById('testedByValue');
              const disconnectionInfoElement = document.getElementById('disconnectionInfo');
              const unexpectedConnectionInfoElement = document.getElementById('unexpectedConnectionInfo');
              const projectInput = document.getElementById('project');
              const modelInput = document.getElementById('model');
              //const CablePNInput = document.getElementById('CablePN');
              const testedByInput = document.getElementById('testedBy');
              const CablePNInput = document.getElementById('CablePN');

              function startTest() {
                  var xhr = new XMLHttpRequest();
                  var message = CablePNInput.value; // Use the selected value as the message
                  xhr.open("GET", "/triggerFunction?message=" + message, true);
                  xhr.send();
              }

              function startRes(){
                var xhr= new XMLHttpRequest();
                var message="startResistance";
                xhr.open("GET","triggerFunction?message="+ message, true);
                xhr.send();
              }
              projectInput.addEventListener('input', () => {
                  projectValueElement.textContent = projectInput.value;
              });

              modelInput.addEventListener('input', () => {
                  modelValueElement.textContent = modelInput.value;
              });

              CablePNInput.addEventListener('input',()=>{
                  CableValueElement.textContent=CablePNInput.value;
              });

              testedByInput.addEventListener('input', () => {
                  testedByValueElement.textContent = testedByInput.value;
              });

              startTestButton.addEventListener('click', () => {
                  // Simulate the test and update the test status
                
                startTest();
              
                  statusMessageElement.textContent = "Test in progress...";
                  disconnectionInfoElement.textContent = ""; // Clear previous disconnection info
                  unexpectedConnectionInfoElement.textContent = ""; // Clear previous unexpected connection info
                  setTimeout(() => {
                      // Simulate test completion
                      statusMessageElement.textContent = "Test completed successfully";
                      downloadReportButton.disabled = false;

                      // Simulate disconnection and unexpected connection
                      disconnectionInfoElement.textContent = "Refer to Raw Test Result(TBD)";
                      unexpectedConnectionInfoElement.textContent = "Refer to Raw Test Result(TBD).";
                  }, 500); // Simulate a 3-second test
              });
              startResButton.addEventListener('click',()=>{
                  startRes();
                  statusMessageElement.textContent="Res Test Ongoing..";
                  disconnectionInfoElement.textContent="";
                  unexpectedConnectionInfoElement.textContent="";
                  setTimeout(() => {
                      statusMessageElement.textContent="HuiHui";
                      downloadReportButton.disabled=false;

                      disconnectionInfoElement.textContent="Yaposh";
                      disconnectionInfoElement.textContent="Kutush";
                  }, 300);
              });
              downloadReportButton.addEventListener('click', () => {
                  // Get the current timestamp
                  const now = new Date();
                  const year = now.getFullYear().toString().slice(-2);
                  const month = String(now.getMonth() + 1).padStart(2, '0');
                  const day = String(now.getDate()).padStart(2, '0');
                  const hours = String(now.getHours()).padStart(2, '0');
                  const minutes = String(now.getMinutes()).padStart(2, '0');
                  const timestamp = year + month + day + '_' + hours + minutes;

                  // Create the CSV data
                  const data = [
                      ["Timestamp", "Test Status", "Project", "Model","Cable PN", "Tested By"],
                      [timestamp, statusMessageElement.textContent, projectInput.value, modelInput.value,CablePNInput.value, testedByInput.value],
                  ];

                  const fileName = modelInput.value + CablePNInput.value+testedByInput.value+ '_' + timestamp + ".csv";
                  const csvContent = "data:text/csv;charset=utf-8," + data.map(row => row.join(",")).join("\n");
                  const encodedUri = encodeURI(csvContent);
                  const link = document.createElement("a");
                  link.setAttribute("href", encodedUri);
                  link.setAttribute("download", fileName);
                  document.body.appendChild(link);
                  link.click();
                // Add this function to your JavaScript code in the HTML file
              });
          </script>
      </div>
  </body>
  </html>
      )=====";
    }
