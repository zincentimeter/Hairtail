var picker = new Pikaday({
	field: document.getElementById('datepicker'),
	onSelect: function(date) {
		website = document.getElementById("Website").value;

		document.write("在",date.toString(),"您查询的网站",website,"的访问量为：",23333);

		getInfo(website, date.getTime());
		}
	});