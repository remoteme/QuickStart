var previousYoutube = undefined;

$(document).ready(function () {

	showInfoModal("Checking RasbperryPi connection", "fab fa-raspberry-pi");
	let notConnected = setTimeout(() => {
		showInfoModal("RasbperryPi Not Connected - You cannot interact ", "fas fa-unlink", "#FF0000");
	}, 2000);
	let remoteme = RemoteMe.getInstance();
	remoteme.remoteMeConfig.deviceConnectionChange.push((deviceId, connected) => {
		if (deviceId == $$python$$) {//rpi device
			clearTimeout(notConnected);
			if (!connected) {
				showInfoModal("RasbperryPi Not Connected - You cannot interact ", "fas fa-unlink", "#FF0000");
			} else {
				showInfoModal("RasbperryPi connected", "fab fa-raspberry-pi", "#00c900", 2);
				connectYoutubeVariable();
			}

		}

	});

	remoteme.subscribeEvent(EventSubscriberTypeEnum.DEVICE_CONNECTION);


});

function connectYoutubeVariable() {

	remoteme = RemoteMe.getInstance();
	remoteme.getVariables().observeInteger("$$youtube_state$$", (state) => {
		let htmlYoutubeState = "";
		if (state == 0) {
			htmlYoutubeState=`<i class="fab fa-youtube" style='color:black'></i> not connected`;
		} else if (state == 1) {
			htmlYoutubeState=`<i class="fab fa-youtube" style='color:#00c900'></i>connected`;
			if (previousYoutube != 1) {
				if (previousYoutube != undefined) {
					showInfoModal("Youtube actived. Page will be reload in 10s", "fab fa-youtube", "#00c900");
					setTimeout(() => {
						location.reload(true);
					}, 10000);

				} else {
					$("#youtubeContainer").html(`<iframe width="100%" height="100%" src="https://www.youtube.com/embed/live_stream?channel=UCVYdTbKJgYtWV3IbeaWCr6w&autoplay=1" frameborder="0" allowfullscreen></iframe>`);

					showInfoModal("Youtube actived.", "fab fa-youtube", "#00c900", 2);
				}
			}

		} else if (state == -1) {
			showInfoModal("Youtube connect fail", "fab fa-youtube", "#00c900");
			htmlYoutubeState=`<i class="fab fa-youtube" style='color:#00c900'></i>connected`;
		} else if (state == 2) {
			showInfoModal("Youtube starting", "fab fa-youtube", "#0000FF");
			htmlYoutubeState=`<i class="fab fa-youtube" style='color:#0000FF'></i>connecting`;
		} else if (state == 3) {
			showInfoModal("Youtube closing", "fab fa-youtube", "orange");
			htmlYoutubeState=`<i class="fab fa-youtube" style='color:orange'></i>closing`;
		}
		previousYoutube = state;
		$('#youtubeStatediv').html(htmlYoutubeState);
	});
}