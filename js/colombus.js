// Colombus landing page scripts


$(document).ready( function(){

	

	// Background randomization
	var bg = Math.round(Math.random()*10);
	$("body").css("background", "url('content/bg/"+bg+".jpg')");



	// Center the stuff
	var win_h = $(document).innerHeight();
	var c_h = $("#container").innerHeight();
	$("#container").css("margin-top", ((win_h)/2)-(c_h/2)+"px");

	window.onresize = function(e){
		win_h = $(document).innerHeight();
		c_h = $("#container").innerHeight();
		$("#container").css("margin-top", ((win_h)/2)-(c_h/2)+"px");
	};



	// Gives focus
	$("#rs").focus();


});