#include <WiFi.h>
#include <ESPAsyncWebSrv.h>
#include <Wire.h>
#include <SD.h>
const char *ssid = "RFTEST";
const char *password = "12345678";
const char *fileNameR="Raw Data.csv";
const char *fileName="Resdata.csv";
String resString;
String sttString;
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
  
   server.on("/resData",HTTP_GET,[](AsyncWebServerRequest *request)
    {
      String resData="";
      resData +=resString;
    request->send(200,"text/plain",resData);
    });
    server.on("/resStat",HTTP_GET,[](AsyncWebServerRequest *request)
    {
      String resStat="";
      resStat +=sttString;
    request->send(200,"text/plain",resStat);
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
            border-radius: 20px;
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
        .readcount{
            padding: 12px 24px;
            border: none;
            cursor: pointer;
            font-size: 16px;
            background-color: #d2de2d;
            color: #fff;
            border-radius: 4px;
            box-shadow: 0 4px 6px rgba(76, 175, 80, 0.3);
            transition: background-color 0.3s ease-in-out, transform 0.1s ease-in-out, box-shadow 0.3s ease-in-out;
        }
       .readcount:disabled{
        padding: 12px 24px;
            border: none;
            cursor: pointer;
            font-size: 16px;
            background-color: #cb5119;
            color: #fff;
            border-radius: 4px;
            box-shadow: 0 4px 6px rgba(76, 175, 80, 0.3);
            transition: background-color 0.3s ease-in-out, transform 0.1s ease-in-out, box-shadow 0.3s ease-in-out;  
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
                background-color: #333;
        }
        .scrollable-table {
            max-height: 15000px; /* Adjust the height as needed */
            overflow: scroll; /* Set overflow to auto or scroll */
            background-color: #289e9a;
            border-radius: 25px;
            scrollbar-color: #763535;
        }

        table {
            border-collapse: collapse;
            width: 100%;
        }
        th, td {
            border: 1px solid black;
            padding: 8px;
            text-align: center;
        }
          /* Fixed header styles */
        thead {
            position: sticky;
            top: 0;
            background-color: white;
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
            <h2>Overall Result: <span id="statusMessage">NULL</span></h2>
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

        <div class="view-window" id="rawDataView">
            <h2>Raw Data.csv</h2>
            <pre id="rawDataContent"></pre>
        </div>

        <div class="controls">
            <div class="center-button" id="ReadCount">
                <button id="ReadCount"disabled>Read Count</button>
                <p id="ReadCountInfo"></p>
            </div>
            <div class="center-button" id="ConnectionRes">
                <button id="ConnectionRes"disabled>Connect/Disconnect</button>
                <p id="ConnectionInfo"></p>
            </div>
            <div class="center-button" id="ResResult">
                <button id="ResResult"disabled>Res Test Result</button>
                <p id="ResResultInfo"></p>
            </div>         
        </div>

        <div class="additional-output">
            <div class="additional-output-box" id="Res1Read">
                <h3>Reading 1(ohm)</h3>
                <p id="Res1Info"></p>
            </div>
            <div class="additional-output-box" id="Res2Read">
                <h3>Reading 2(ohm)</h3>
                <p id="Res2Info"></p>
            </div>
            <div class="additional-output-box" id="Res3Read">
                <h3>Reading 3(ohm)</h3>
                <p id="Res3Info"></p>
            </div>         
        </div>

        <div class="scrollable-table">
            <table id="data-table">
                <thead>
                    <tr>
                      <th>Res1</th>
                      <th>Res2</th>
                      <th>Res3</th>
                    </tr>
                  </thead>
                  <tbody>
                <td id="res1">-</td>
                <td id="res2">-</td>
                <td id="res3">-</td>
                  </tbody>
            </table>
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
            const ReadCountInd=document.getElementById('ReadCount');
            const DisconnetInd=document.getElementById('ConnectionRes');
            const ResTestInd=document.getElementById('ResResult');
            const R1d=document.getElementById('Res1Info');
            const R2d=document.getElementById('Res2Info');
            const R3d=document.getElementById('Res3Info');
            function updateResData(resData){
                var resValues=resData.split(',');
                for(var i=0;i<resValues.length; i++){
                    var resValue=resValues[i].trim();
                    document.getElementById("ResResult"+(i+1)).innerText=resValue;
                }
            }
            function updateResStat(resStat){
                var resStats=resStat.split(',');
                var StatusCD=resStat[0].trim;
                var Rcount=resStat[1].trim;
            }
            function updateTable(res1, res2, res3) {
                // Create a new row with the received values
                var newRow = "<tr><td>" + res1 + "</td><td>" + res2 + "</td><td>" + res3 + "</td></tr>";

                // Get the table body
                var tableBody = document.getElementById("data-table").getElementsByTagName('tbody')[0];

                // Add the new row to the beginning of the table body
                tableBody.insertAdjacentHTML('afterbegin', newRow);

                // If the table has more than 30 rows, remove the last row
                if (tableBody.rows.length > 30) {
                tableBody.deleteRow(-1);
                }
            }
            function fetchRes(){
                fetch("/resData")
                .then(response=>response.text())
                .then(data=>updateResData(data));
            }
            function fetchResStt(){
                fetch("/resStat")
                .then(response=>response.text())
                .then(data=>updateResStat(data));
            }
           ///setInterval(fetchRes,100);
            function startTest() {
                var xhr = new XMLHttpRequest();
                var message = CablePNInput.value; // Use the selected value as the message
                xhr.open("GET", "/triggerFunction?message=" + message, true);
                xhr.send();
            }
            function receiveDataFromESP32() {
            // Replace this part with actual code to receive data from ESP32
            // For example, you might use AJAX or WebSocket to get data from ESP32
            // Here, I'm just generating random values as an example
            var res1 = Math.random() * 100;
            var res2 = Math.random() * 100;
            var res3 = Math.random() * 100;
    
                // Update the table with received values
                updateTable(res1.toFixed(2), res2.toFixed(2), res3.toFixed(2));
            }
            setInterval(receiveDataFromESP32, 1000);
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
               setTimeout(()=>{
                startTest();
               },3000);
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
                }, 3000); // Simulate a 3-second test
            });
            startResButton.addEventListener('click',()=>{
                startRes();
                statusMessageElement.textContent="Res Test Ongoing..";
                disconnectionInfoElement.textContent="";
                unexpectedConnectionInfoElement.textContent="";
                const resValues=["Value1","Value1","Value1"];5
                setTimeout(() => {
                    statusMessageElement.textContent="Ongoing";
                    downloadReportButton.disabled=false;
                    
                    //RValue
                    R1d.textContent=resValues[0];
                    R2d.textContent=resValues[1];
                    R3d.textContent=resValues[2];
                    //RStatus
                    if(StatusCD=0){
                    DisconnetInd.disabled=false;
                    DisconnetInd.textContent="Discconect";
                    }
                    else if(StatusCD=1){
                    DisconnectInd.disabled=true;
                    DisconnectInd.textContent="Connect";    
                    }
                    ReadCountInd.disabled=false;
                    ReadCountInd.textContent=Rcount;
                    if(Rcount>=30){
                    ResTestInd.disabled=false;    
                    }

                  //  ReadCountInd=
                    disconnectionInfoElement.textContent="NA";
                    disconnectionInfoElement.textContent="NA";
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

            async function fetchContinuity(url,l1,l2,l3){
                const response=await fetch(url);
                const data=await response.text();
                const dataArray=data.split(',')
                

            }
        </script>
    </div>
</body>
</html>
      )=====";
  }
