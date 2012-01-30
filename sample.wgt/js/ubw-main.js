/*
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

function init(){
	
    var ubwidget = $("#ubwidget");
	
    var text = "";
    var winwidth;
    var winheight;
    var textField = $('<textarea id="txt" cols="20" rows="10"></textarea>');
		
    $("#ubwidget")
    .append(textField)
    .mousemove(function(){        
        if(window.sankore){
            window.sankore.setPreference("noteText", textField.val());
        }
    });
	
    if(window.sankore){
        //Defining the Open-Sankore language
        var lang = "";
        lang = sankore.locale().substr(0,2);
   
        //loading translation file for the current language
        $("<script type='text/javascript' src='js/translation/" + lang + ".js'>").appendTo("head");
        //verify that the translation exists
        if((typeof sankoreLang) == "undefined")
            //loading the default translation
            $("<script type='text/javascript' src='js/translation/en.js'>").appendTo("head"); 
        
        text = window.sankore.preference('noteText', text);
        
        $('.ubw-container').css({
            width:window.innerWidth - 2,
            height:window.innerHeight - 20
        });

        if(text) 
            textField.val(text);            
        else 
            textField.text(sankoreLang.welcome);       
    }		
				
    window.onresize = function(){
        winwidth = window.innerWidth;
        winheight = window.innerHeight;
		  	
        if(winwidth <= 290)
        {
            window.resizeTo(290,winheight);
        }
        if(winheight <= 100)
        {
            window.resizeTo(winwidth,100);
        }
        if(winheight > 600)
        {
            window.resizeTo(winwidth,600);
        }      		

        $('.ubw-container').width(winwidth-2);
    }
}