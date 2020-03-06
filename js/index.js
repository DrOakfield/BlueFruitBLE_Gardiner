// Based on an example:
//https://github.com/don/cordova-plugin-ble-central


// ASCII only
//function bytesToString(buffer) {
//    return String.fromCharCode.apply(null, new Uint8Array(buffer));
//}

// ASCII only
function stringToBytes(string) {
    var array = new Uint8Array(string.length);
    for (var i = 0, l = string.length; i < l; i++) {
        array[i] = string.charCodeAt(i);
    }
    return array.buffer;
}



// this is ble hm-10 UART service
/*var blue= {
    serviceUUID: "0000FFE0-0000-1000-8000-00805F9B34FB",
    characteristicUUID: "0000FFE1-0000-1000-8000-00805F9B34FB"
};*/

//the bluefruit UIID Service
var blue ={
	serviceUUID: '6e400001-b5a3-f393-e0a9-e50e24dcca9e',
    txCharacteristic: '6e400002-b5a3-f393-e0a9-e50e24dcca9e', // transmit is from the phone's perspective
    rxCharacteristic: '6e400003-b5a3-f393-e0a9-e50e24dcca9e'  // receive is from the phone's perspective
}

var ConnDeviceId;
var deviceList =[];
 
function onLoad(){
	document.addEventListener('deviceready', onDeviceReady, false);
    bleDeviceList.addEventListener('touchstart', conn, false); // assume not scrolling
}

function onDeviceReady(){
	refreshDeviceList();
}

function refreshDeviceList(){
	//deviceList =[];
	//document.getElementById("bleDeviceList").innerHTML = ''; // empties the list
	if (cordova.platformId === 'android') { // Android filtering is broken
		ble.scan([], 5, onDiscoverDevice, onError);
	} else {
		//alert("Disconnected");
		ble.scan([blue.serviceUUID], 5, onDiscoverDevice, onError);
	}
}

function onDiscoverDevice(device){
	//Make a list in html and show devises
	if(device.name == "CURTAINCONTROLLER") {
		ConnDeviceId = device.id;

		
	}
		
}

function buttonConnect() {
	ble.connect(ConnDeviceId, onConnect, onConnError);
}

/*function conn(){
	var  deviceTouch= event.srcElement.innerHTML;
	//document.getElementById("debugDiv").innerHTML =""; // empty debugDiv
	var deviceTouchArr = deviceTouch.split(",");
	ConnDeviceId = deviceTouchArr[1];
	//document.getElementById("debugDiv").innerHTML += "<br>"+deviceTouchArr[0]+"<br>"+deviceTouchArr[1]; //for debug:
	ble.connect(ConnDeviceId, onConnect, onConnError);
 }*/
 
 //succes
function onConnect(){
	//document.getElementById("statusDiv").innerHTML = " Status: Connected";
	//document.getElementById("bleId").innerHTML = ConnDeviceId;
	ble.startNotification(ConnDeviceId, blue.serviceUUID, blue.rxCharacteristic, onData, onError);
	
}

//failure
function onConnError(){
	alert("Problem connecting");
	//document.getElementById("statusDiv").innerHTML = " Status: Disonnected";
}

function onData(data){ // data received from Arduino
	//document.getElementById("receiveDiv").innerHTML =  "Received: " + bytesToString(data) + "<br/>";
}

function currentTime(){
	var d = new Date();
	
	var H = d.getHours().toString;
	var M = d.getMinutes().toString;
	var S = d.getSeconds().toString;
	
	var currentTime = H + M + S;
	return currentTime;
}

function sendCurrentTime(){
	var currentTimeTemp = currentTime(); 
	var currentTimeBytes = stringToBytes(currentTimeTemp);

	ble.writeWithoutResponse(ConnDeviceId, blue.serviceUUID, blue.txCharacteristic, currentTimeBytes, onSend, onError);
}

function sendSave(){

	var sunrise = stringToBytes(sunriseBox.value);
	var sunset = stringToBytes(sunsetBox.value);

	ble.writeWithoutResponse(ConnDeviceId, blue.serviceUUID, blue.txCharacteristic, sunrise, onSendSave, onError);
	ble.writeWithoutResponse(ConnDeviceId, blue.serviceUUID, blue.txCharacteristic, sunset, onSendSave, onError);

	
}

function onSendSave() {
	document.getElementById("sendDiv1").innerHTML = "Sent: " + sunriseBox.value + "<br/>";
	document.getElementById("sendDiv2").innerHTML = "Sent: " + sunsetBox.value + "<br/>";
}


function data(txt){
	GemtInput.value = txt;
	sendData();
}	

function sendData() { // send data to Arduino
	var data = stringToBytes(GemtInput.value)
	ble.writeWithoutResponse(ConnDeviceId, blue.serviceUUID, blue.txCharacteristic, data, onSend, onError);
}
	
function onSend(){
	//document.getElementById("manualDiv").innerHTML = "Sent: " + GemtInput.value + "<br/>";
}



function onError(reason)  {
	alert("ERROR: " + reason); // real apps should use notification.alert
}
