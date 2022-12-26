import moment from 'moment';

// let message: string = `If you see this message, today is ${moment().format('dddd')}, and it worked !`;
let message = `If you see this message, today is ${moment().format('dddd')}, and it worked !`;

// create a new heading 1 element
let heading = document.createElement('h1');
heading.textContent = message;

// add the heading the document
document.body.appendChild(heading);
