document.onreadystatechange = function () {
	switch (document.readyState){
		case "loading":
			document.getElementById('loading').style.visibility="visible";
			break;
		case "complete":
			document.getElementById('loading').style.visibility="hidden";
			break;
	}
}

function request_file() {
	var filename = document.getElementById("filename-meta-data").getAttribute("filename");
	var fileRequest = new XMLHttpRequest();
	fileRequest.open("GET", "/file/"+filename, false);
	fileRequest.onreadystatechange = function () {
		parse_file(fileRequest.responseText);
	}
	fileRequest.send();
}

function parse_file(file_text) {
	var data = split_text(file_text);		// turn csv into matrix of values	
	data = parse_floats(data);				// turn strings into floats
	data = turn_rows_into_objects(data);	// parse the rows
	data = truncate_zero_rows(data);		// remove rows where there is not lat/lon data (lat and lon = 0)
	data = interpolate_data(data);			// fill in gaps between position of data
	plot_data(data);
}

// Helper functions for parse_file

function split_text(text) {
	// returns matrix that represents the data
	var rows = text.split('\n');			// split text by newline
	rows.pop();								// there is an extra new line at the end (since the last line is empty)
	return rows.map(x => x.split(','));		// split each row by commas
}

function parse_floats(data) {
	// for each element of the matrix, parse it into a string
	return data.map(row => row.map(s => parseFloat(s)));
}

function turn_rows_into_objects(matrix) {
	// turn each row into an object for more abstraction
	return matrix.map(row => new PositionalData(row));
}

const LAT_INDEX = 10;
const LON_INDEX = 11;

class PositionalData {
	constructor(data) {
		this.lat = convertLatLonFormats(data[LAT_INDEX]);
		this.lon = convertLatLonFormats(data[LON_INDEX]);
		this.data = data.slice(0, LAT_INDEX);
	}
}

function convertLatLonFormats(angle) {
	// converts lat/lon from DDDMM.MMMMMM format (what NMEA uses)
	// to Decimal degrees (more linear scale)
	var sign_multiplier = (angle < 0) ? -1 : 1;
	angle = Math.abs(angle);
	var degrees = Math.floor(angle / 100);
	var minutes = angle % 100;
	return (degrees + minutes/60) * sign_multiplier;
}

function truncate_zero_rows(data) {
	// returns data to remove rows where (lat,lon) = (0,0)
	return data.filter(obj => obj.lat!=0 || obj.lon!=0);
}

function interpolate_data(data) {
	// evenly space out the datapoints on segments where lat and lon are constant

	// step 1: group points if they have the same coordinates
	var segments = [[data[0]]]
	var prev = data[0];
	for(var i=1; i<data.length; i++) {
		var curr = data[i];
		if(curr.lon!=prev.lon || curr.lat!=prev.lat) {
			segments.push([]);
		} 
		segments[segments.length-1].push(curr);
		prev = curr;
	}

	// step 2: calculate intermediary points
	var output = []
	for(var i=0; i<segments.length; i++) {
		if (i ==  segments.length - 1) {
			// if its the last segment, just add all since theres nothing to interpolate with
			Array.prototype.push.apply(output, segments[i])	// join output and last segment
		} else {
			var next_point = segments[i+1][0];
			var segment = segments[i];
			var first_point = segment[0];
			var num_points = segment.length;

			var lat_increment = (next_point.lat - first_point.lat) / num_points;
			var lon_increment = (next_point.lon - first_point.lon) / num_points;

			for(var j=0; j<num_points; j++) {
				var point = segment[j];
				point.lat += lat_increment * j;
				point.lon += lon_increment * j;
				output.push(point);
			}
		}
	}

	// return the interpolated data
	return output;
}

function convert_data_to_plot_data(data) {
	function convert(data) {
		return {
			x: data.lon,
			y: data.lat,
			d: data.data
		};
	}
	return data.map(convert);
}

function plot_data(data) {
	var lat_list = data.map(obj => obj.lat);
	var avg_lat = lat_list.reduce((a,b)=>(a+b)) / lat_list.length;

	var lon_list = data.map(obj => obj.lon);
	var avg_lon = lon_list.reduce((a,b)=>(a+b)) / lon_list.length;

	// construct map
	var myMap = L.map('mapid').setView([avg_lat, avg_lon], 13);

	// get tile data using api (please dont misuse my api token)
	// and add it to the map
	var accessToken = "pk.eyJ1Ijoid2NzdW4iLCJhIjoiY2ttMzRqMjFlMG16dDJ4anZveW13YzVxYyJ9.dVGn6WMRq5T555GYt7Q8ew"
	L.tileLayer('https://api.mapbox.com/styles/v1/{id}/tiles/{z}/{x}/{y}?access_token={accessToken}', {
		attribution: 'Map data &copy; <a href="https://www.openstreetmap.org/copyright">OpenStreetMap</a> contributors, Imagery © <a href="https://www.mapbox.com/">Mapbox</a>',
		maxZoom: 18,
		id: 'mapbox/streets-v11',
		tileSize: 512,
		zoomOffset: -1,
		accessToken: accessToken
	}).addTo(myMap);

	// plot the points
	for (const point of data) {
		var marker = L.marker([point.lat, point.lon]).addTo(myMap);
		marker.bindPopup(get_tooltip(point))
	}
}

function get_tooltip(obj) {
	var d = obj.data;
	var x = obj.lon;
	var y = obj.lat;
	return `<b>Accel-X: </b>${d[0]} <br/> \
			<b>Accel-Y: </b>${d[1]} <br/> \
			<b>Accel-Z: </b>${d[2]} <br/> \
			<b>Gyro-X: </b>${d[3]} °<br/> \
			<b>Gyro-Y: </b>${d[4]} °<br/> \
			<b>Gyro-Z: </b>${d[5]} °<br/> \
			<b>Temp-1: </b>${d[6]} °f<br/> \
			<b>Temp-2: </b>${d[7]} °f<br/> \
			<b>Temp-3: </b>${d[8]} °f<br/> \
			<b>Wheelspeed: </b>${d[9]} mph<br/> \
			<b>Long: </b>${x}<br/> \
			<b>Lat: </b>${y} °`;
}

	
request_file();
