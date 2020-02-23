var espId=$$esp$$;



function send(number,targetDeviceId=espId){

	stringToSend=getArray($("#textInput").val());//convert string to utf-8 arrays of bytes

	dataToSend = new RemoteMeData(2 );//size of int 16
	dataToSend.putInt16(number);
	RemoteMe.getInstance().sendUserMessage(targetDeviceId,dataToSend.getArray());
}


$(document).ready(function () {
	RemoteMe.getInstance();//connect to RemoteMe and keeps conenction live


	RemoteMe.getInstance();//connect to RemoteMe and keeps conenction live


	RemoteMe.getInstance().addVariableBooleanChangeListener("button1Active",(value)=>{
		$("#button1")[0].disabled=value;
	});
	RemoteMe.getInstance().addVariableBooleanChangeListener("button2Active",(value)=>{
		$("#button2")[0].disabled=value;
	});

	RemoteMe.getInstance().addVariableTextChangeListener("button1Text",(value)=>{
		$("#button1")[0].innerHTML=value;
	});

	RemoteMe.getInstance().addVariableTextChangeListener("button2Text",(value)=>{
		$("#button2")[0].innerHTML=value;
	});

	RemoteMe.getInstance().addVariableTextChangeListener("label",(value)=>{
		$("#label")[0].innerHTML=value;
	});

$$IF rental == 1 $$
	RemoteMe.getInstance().addGuestInfoChangeListener((credit,time)=>{
		$("#button3")[0].disabled=(credit<=10);
	});
$$/IF$$
});


