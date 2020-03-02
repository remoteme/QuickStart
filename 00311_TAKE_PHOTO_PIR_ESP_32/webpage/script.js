var cameraDeviceId=$$esp$$;

function takePhotoNow(){
	RemoteMe.getInstance().sendUserMessage(cameraDeviceId ,[1]);
	$("#progress").css("display","block");
}


$(document).ready(function () {
	let remoteme=RemoteMe.getInstance();//connect to RemoteMe and keeps conenction live

	let fileName = "photos/photo.jpg";
	let deviceId = thisDeviceId;

	let image = $('#imageForPhoto');

	image[0].src=`/wp/device_${deviceId}/${fileName}?r=${Math.floor(Math.random() * 10000)}`;

	image[0].onload = function() {
		$("#progress").css("display","none");
	};

	remoteme.addFileChangeListener((rdeviceId,rfileName)=>{
		if ((deviceId==rdeviceId)&&(rfileName==fileName)){
			image[0].src=`/wp/device_${deviceId}/${fileName}?r=${Math.floor(Math.random() * 10000)}`;

		}
	});

	remoteme.subscribeEvent(EventSubscriberTypeEnum.FILE_CHANGE);
});
