const test = 1;
let test2= 2;
var test3= 3;

//to create an element in JS
const div = document.createElement('div');

//to select an element in js with a css selector
const div= document.querySelector('div');
//or by id
const div = document.getElementById('div');

//to modify an element
div.style.backgroundColor="red";
div.style.color='white';
div.style.fontSize='20px';
div.innerHTML="<h1>Hello World</h1>";

//function in js
function myFunction(){
    console.log('HEllo World');
}
myFunction();

//to add an event listener
div.addEventListener(type, function);

//my data
let array = ['one','two','three','four'];