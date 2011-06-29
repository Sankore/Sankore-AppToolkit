
var word = "horse";
var img = "template/images/horse.png";

function startEditing()
{
	sankore.returnStatus("startEditing()", "Ok!");
}


function stopEditing()
{
	sankore.returnStatus("stopEditing()", "Ok!");
}


function initialize()
{
	sankore.returnStatus("initialize()", "Ok!");
	letters = shuffle(createWordLetters( word ));
	modeEdit();
}

function createWordLetters( word )
{
	var ch, el;
	var letters = [];
	for( var i = 0; i < word.length; i++ )
	{
		ch = word.charAt( i );
		el = document.createElement( "div" );
		$(el).addClass( "letter" ).text( ch );
		letters.push( el );
	}
	return letters;
}

function checkResponse()
{
	var str = "";
	$( "#mp_word .letter" ).each( function(){
		str += $(this).text();
	});
	if( str == word ){
		message( "Right!" );
		sankore.returnStatus("checkResponse()", "Right!");
	}
	else{
		sankore.returnStatus("checkResponse()", "Wrong!");
	}
}

function shuffle( arr )
{
	var pos, tmp;
	
	for( var i = 0; i < arr.length; i++ )
	{
		pos = Math.round( Math.random() * ( arr.length - 1 ) );
		tmp = arr[pos];
		arr[pos] = arr[i];
		arr[i] = tmp;
	}
	return arr;
}

var letters = shuffle( createWordLetters( word ) );

function modeView()
{
	$( "#mp_img").empty().append('<img src="'+img+'">');
	$( "#mp_word" ).empty();
	for( var i in letters ){
		$("#mp_word").append( letters[i] );
	}
	$( "#mp_word" ).sortable( { update: checkResponse } );
}

function modeEdit()
{
	//$( "#mp_img").empty().append('Image: <input value="'+img+'"><input type="button" value="Load to see..."><br><img>');
	$( "#mp_word").empty().append('Name: <input value="'+word+'"><input type="button" value="OK">');
	$( "#mp_img input:button" ).click( function()
	{
		img = $( "#mp_img input:text" ).attr( "value" );
		$( "#mp_img img" ).attr( "src", img );
	});
	$( "#mp_word input:button" ).click( function()
	{
		word = $( "#mp_word input:text" ).attr( "value" );
		modeView();
	});
}

function message( msg )
{
	$( "#msg" ).text( msg ).animate( {opacity : 1}, "fast" ).delay( 1000 ).animate( {opacity: 0}, "slow" );
}

$(document).ready(function()
{
	$("#ub-widget").append('<div id="mp_img"></div><div id="mp_word"></div><div id="msg"></div>');
	$( "#msg" ).css( {opacity: 0} );
	modeView();
	//message( "ok" );
	//modeEdit();
});
