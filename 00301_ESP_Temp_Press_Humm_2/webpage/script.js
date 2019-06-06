function onWeather_Now(temp,press,date,icon){
    onWeather("#weather_now",temp,press,date,icon);
}

function onWeather_Tomorrow(temp,press,date,icon){
    onWeather("#weather_tomorrow",temp,press,date,icon);
}

function onWeather(parentEl,temp,press,date,icon){
    console.info(`i ${temp} i1 : ${press} s1 : ${date} s2: ${icon}`);
    let parent=$(parentEl);

    parent.find(".temp").html(temp+" °C");
    parent.find(".press").html(press+" hPa");
    parent.find(".date").html(date);

  
    parent.find(".iconBox").html(`<div class="icon ${icon}"></div>`);

}


function onLocal(children,value){
    let parent=$("#weather_local");
    parent.find(children).html(value);
   
}


$(document).ready(function () {
 
   RemoteMe.getInstance().getVariables().observeSmallInteger2Text2("Weather_Now",onWeather_Now);
   RemoteMe.getInstance().getVariables().observeSmallInteger2Text2("Weather_Tomorrow",onWeather_Tomorrow); 

   RemoteMe.getInstance().getVariables().observeInteger("humm",value=>onLocal('.humm',value+" %")); 
    RemoteMe.getInstance().getVariables().observeInteger("press",value=>onLocal('.press',value+" hPa")); 
    RemoteMe.getInstance().getVariables().observeInteger("temp",value=>onLocal('.temp',value+" °C")); 
});
