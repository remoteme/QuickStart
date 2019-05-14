var counter=0;
function onButtonClick(){
	console.log(`Button click counter set to ${counter}`);
	RemoteMe.getInstance().getVariables().setInteger('someIntegerVariable',counter);
	setButtonText();
}
function setButtonText(){
	$('button').html(`click to set counter to ${++counter} and send to ESP`);
}

function onJustBooleanVariableChange(value){
	console.log('JustBooleanVariable changed into'+value);
	if (value){
		$('#divBoolean').addClass('on');
	}else{
		$('#divBoolean').removeClass('on');
	}
}

$(document).ready(function () {
	setButtonText();
	let remoteMe = RemoteMe.getInstance();
	remoteMe.getVariables().observeBoolean('justBooleanVariable',onJustBooleanVariableChange);

	remoteMe.getVariables().observeSmallInteger3('just3IntegerVariable',(x,y,z)=>{
		console.log(`3 value integer changed into x:${x} y:${y} z:${z}`);
		$('#divX').html(x);
		$('#divY').html(y);
		$('#divZ').html(z);
	});
});

