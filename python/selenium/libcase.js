// This library provides case-related services.

function invert_case(str)
{
    var output = '';
    var code = '';
    
    for(var i=0,len=str.length-1; i<=len; i++){
        code = str.charCodeAt(i);
        if(code >= 65 && code <= 90){
            output+=str.charAt(i).toLowerCase();
        }else if(code >=97 && code <= 122){
            output+=str.charAt(i).toUpperCase();
        }else{
            output+=str.charAt(i);
        }
    }
    return output;
}

function lower_case(str)
{
    return str.toLowerCase();
}

function upper_case(str)
{
    return str.toUpperCase();
}
