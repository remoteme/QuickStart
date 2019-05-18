var espId=$$esp$$;
var pythonId=$$python$$;

function sendToEsp(){
	send(espId)
}
function sendToPython(){
	send(pythonId)
}

function send(targetDeviceId){


	dataToSend = new RemoteMeData(8+8 );//size of double is 8 so we have 16 sinve sending two
	dataToSend.putDouble($("#double1Input").val());
	dataToSend.putDouble($("#double2Input").val());
	let sendTime= new Date().getTime();
	RemoteMe.getInstance().sendUserSyncMessage(targetDeviceId,dataToSend.getArray(),data=>{
		data = new RemoteMeData(data);//lets just replace it
		let doubleValue=data.popDouble();
		alert(`result from device: ${doubleValue} got in ${new Date().getTime() - sendTime }ms`);
	});

}


$(document).ready(function () {
	RemoteMe.getInstance();//connect to RemoteMe and keeps conenction live
});
