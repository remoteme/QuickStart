$( document ).ready(function() {
   addChart();
});

function addChart(){
// for more into and configuration examples etc check: https://plot.ly/javascript/reference/

	yestardayMoment=moment().subtract(1, 'days').format("DD.MM.YYYY");
	todayMoment=moment().format("DD.MM.YYYY");

	addDatePickerForChart('chartRange9797',yestardayMoment,todayMoment,(from,to)=>{
		var variables="";
		
			
			variables+="humm";
		
			
		variables+=",";
			
			variables+="temp";
		
			
		variables+=",";
			
			variables+="press";
		
		
		var url = `/api/rest/v1/variable/get/timeseries/dd.MM.yyyy%20HH:mm/${from}/${to}/${variables}/`;
		
		$.get(url, function (data, status) {
		
			if (data["humm"]==undefined){
				alert("cannot read data for variable humm");
				return ;
			}
			var trace0 = {
				x: data["humm"].datas,
				y: data["humm"].value,
				name: "hummidity hPa",
				fill: "none",
				type: "area",
				stackgroup: 'one',
				marker: {
					color: '#FF0000',
					line: {
						color: '#FF0000'
					}
				}
			};
		
			if (data["temp"]==undefined){
				alert("cannot read data for variable temp");
				return ;
			}
			var trace1 = {
				x: data["temp"].datas,
				y: data["temp"].value,
				name: "temperature ",
				fill: "none",
				type: "area",
				stackgroup: 'one',
				marker: {
					color: '#00ffb7',
					line: {
						color: '#00ffb7'
					}
				}
			};
		
			if (data["press"]==undefined){
				alert("cannot read data for variable press");
				return ;
			}
			var trace2 = {
				x: data["press"].datas,
				y: data["press"].value,
				name: "pressure",
				fill: "none",
				type: "bar",
				stackgroup: 'one',
				marker: {
					color: '#bb00ff',
					line: {
						color: '#bb00ff'
					}
				}
			};
		
			var data = [trace0,trace1,trace2,];
			var layout = {
				title: "Temprature, Pressure and Hummidity",
				xaxis: {
					categoryorder:  "X ax"
				},
		
			yaxis: {
				title: "%",
				titlefont: {color: "#1f77b4"},
				tickfont: {color: "#1f77b4"},
				anchor: "free",
				overlaying: "y",
				side: "left"
			},
		
			yaxis2: {
				title: "C",
				titlefont: {color: "#1f77b4"},
				tickfont: {color: "#1f77b4"},
				anchor: "free",
				overlaying: "y",
				side: "left"
			},
		
			yaxis3: {
				title: "hPa",
				titlefont: {color: "#1f77b4"},
				tickfont: {color: "#1f77b4"},
				anchor: "free",
				overlaying: "y",
				side: "left"
			},
		
			};
			Plotly.newPlot('chartDiv9797', data, layout);
		});
	});
}