var espId=$$esp$$;
var pythonId=$$python$$;

var sendTime;

function sendToEsp(){
	send(espId)
}
function sendToPython(){
	send(pythonId)
}

function send(targetDeviceId){

	stringToSend=getArray($("#textInput").val());//convert string to utf-8 arrays of bytes

	dataToSend = new RemoteMeData(stringToSend.length // string length
		+1// each string is neded by 0 so we need one byte for it
		+8 );//size of double
	dataToSend.putString(stringToSend);
	dataToSend.putDouble($("#floatInput").val());
	RemoteMe.getInstance().sendUserMessage(targetDeviceId,dataToSend.getArray());
	sendTime= new Date().getTime();
}
function onUserMessage(senderDeviceId,data){
	console.info(`got message from ${senderDeviceId}`);
	data = new RemoteMeData(data);//lets just replace it
	let stringVale= data.popString();
	let doubleValue=data.popDouble();

	console.info(`message string ${stringVale} double from message: ${doubleValue}`);

	$("#output").html(`got message from ESP string : '${stringVale}' double ${doubleValue} <br/> time : ${ new Date().getTime()-sendTime} ms`)
}

$(document).ready(function () {
	RemoteMe.getInstance();//connect to RemoteMe and keeps conenction live
	RemoteMe.getInstance().remoteMeConfig.onUserMessage=onUserMessage;
});


