function on_select_change() {
	var filename = document.getElementById("filename-meta-data").getAttribute("filename");
	var selectedSensor = document.getElementById("sensor-select").value;
	var selectedFunc = document.getElementById("function-select").value;
	window.location.assign("/file/"+filename+'/'+selectedSensor+"/"+selectedFunc);
}
