import moment from 'moment';

// montre comment importer un autre module :
import {log_hoho} from './youpi/hoho';

let message: string = `If you see this message, today is ${moment().format('dddd')}, and it worked !`;

// create a new heading 1 element
let heading = document.createElement('h1');
heading.textContent = message;

// add the heading the document
document.body.appendChild(heading);

log_hoho("SANTA");

// invalid_call, will properly be detected by typescript plugin :
log_hoho(42);
