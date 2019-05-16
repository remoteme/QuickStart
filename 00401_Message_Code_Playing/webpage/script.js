var targetDeviceId=$$esp$$;


function onButtonClick(){

	stringToSend=getArray($("#textInput").val());//convert string to utf-8 arrays of bytes

	dataToSend = new RemoteMeData(stringToSend.length // string length
		+1// each string is neded by 0 so we need one byte for it
		+8 );//size of double
	dataToSend.putString(stringToSend);
	dataToSend.putDouble($("#floatInput").val());
	RemoteMe.getInstance().sendUserMessage(targetDeviceId,dataToSend.getArray());
}

$(document).ready(function () {
	RemoteMe.getInstance();//connect to RemoteMe and keeps conenction live

});

