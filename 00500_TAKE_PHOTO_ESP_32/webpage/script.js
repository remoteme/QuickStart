function takePhotoNow(){
	RemoteMe.getInstance().sendUserMessage(1 ,[1]);
	$("#progress").css("display","block");
}


$(document).ready(function () {
	let remoteme=RemoteMe.getInstance();//connect to RemoteMe and keeps conenction live

	let fileName = "photos/pam.jpg";
	let deviceId = thisDeviceId;

	let image = $('#imageForPhoto');

	image[0].src=`/wp/device_${deviceId}/${fileName}?r=${Math.floor(Math.random() * 10000)}`;

	image[0].onload = function() {
		$("#progress").css("display","none");
	};

	remoteme.remoteMeConfig.deviceFileChange.push((rdeviceId,rfileName)=>{
		if ((deviceId==rdeviceId)&&(rfileName==fileName)){
			image[0].src=`/wp/device_${deviceId}/${fileName}?r=${Math.floor(Math.random() * 10000)}`;

		}
	});


	remoteme.subscribeEvent(EventSubscriberTypeEnum.FILE_CHANGE);

});
