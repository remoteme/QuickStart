$( document ).ready(function() {
	addChart();
});

function addChart(){
// for more into and configuration examples etc check: https://plot.ly/javascript/reference/

	yestardayMoment=moment().subtract(1, 'days').format("DD.MM.YYYY");
	todayMoment=moment().format("DD.MM.YYYY");

	addDatePickerForChart('chartRange9797',yestardayMoment,todayMoment,(from,to)=>{
		var variables="";


		variables+="$$humm$$";


		variables+=",";

		variables+="$$temp$$";


		variables+=",";

		variables+="$$press$$";


		var url = `/api/rest/v1/variable/get/timeseries/dd.MM.yyyy%20HH:mm/${from}/${to}/${variables}/`;

		$.get(url, function (data, status) {


			var trace1 = {
				x: data["$$temp$$"].datas,
				y:  data["$$temp$$"].value,
				name: "temperature",
				type: "area"
			};
			var trace2 = {
				x: data["$$press$$"].datas,
				y:  data["$$press$$"].value,
				name: "pressure",
				yaxis: "y2",
				type: "scatter"
			};
			var trace3 = {
				x: data["$$humm$$"].datas,
				y:  data["$$humm$$"].value,
				name: "humidity",
				yaxis: "y3",
				type: "scatter"
			};

			var data = [trace1, trace2, trace3];
			var layout = {
				title: "temperature humidity pressure",
				xaxis:{
					categoryorder: "category ascending"
				},
				yaxis: {

					title: "C",
					titlefont: {color: "#1f77b4"},
					tickfont: {color: "#1f77b4"}
				},
				yaxis2: {

					title: "Psi",
					titlefont: {color: "#ff7f0e"},
					tickfont: {color: "#ff7f0e"},
					anchor: "free",
					overlaying: "y",
					side: "left",
					position: 0.05
				},
				yaxis3: {

					title: "%Hum",
					titlefont: {color: "#d62728"},
					tickfont: {color: "#d62728"},
					anchor: "x",
					overlaying: "y",
					side: "right"
				}
			};

			Plotly.newPlot('chartDiv9797', data, layout);
		});
	});
}
