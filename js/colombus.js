// Colombus landing page scripts


$(document).ready( function(){

	// Background randomization
	var bg = Math.round(Math.random()*10);
	$("body").css("background", "url('content/bg/"+bg+".jpg')");

	// Gives focus
	$("#rs").focus();


});