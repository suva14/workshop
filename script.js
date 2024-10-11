// // Fonction pour envoyer une commande pour contrôler la LED
// document.getElementById('toggleLedButton').onclick = function() {
//     console.log("Commande envoyée : toggleLED");
//     connection.send('toggleLED');
// };

// Fonction pour envoyer la valeur du potentiomètre (luminosité de la LED)
document.getElementById('ledRange').oninput = function() {
    let ledValue = this.value;
    console.log("Commande envoyée : ledBrightness, Valeur : " + ledValue);
    connection.send('ledBrightness:' + ledValue);
};

// Fonction pour envoyer une commande pour contrôler le moteur
document.getElementById("toggleMotorButton").onclick = function() {
    console.log("Commande envoyée : toggleMotor");
    connection.send('toggleMotor');
};

connection.onmessage = function(event) {
    console.log("Message reçu : " + event.data);
    
    // Si le message est "buttonPressed", allumer la lampe
    if (event.data === "buttonPressed") {
        // Alterner l'état de la lampe
        var lamp = document.getElementById("lamp");
        if (lamp.style.backgroundColor === "yellow") {
            lamp.style.backgroundColor = "grey"; // Éteindre la lampe
        } else {
            lamp.style.backgroundColor = "yellow"; // Allumer la lampe
        }
    }

    // Afficher directement la distance dans l'élément HTML
    else if (event.data[0]=="d"){
        document.getElementById("distanceDisplay").innerText = event.data.substr(1);
    }
};



// const test = 1;
// let test2= 2;
// var test3= 3;

// //to create an element in JS
// const div = document.createElement('div');

// //to select an element in js with a css selector
// const div= document.querySelector('div');
// //or by id
// const div = document.getElementById('div');

// //to modify an element
// div.style.backgroundColor="red";
// div.style.color='white';
// div.style.fontSize='20px';
// div.innerHTML="<h1>Hello World</h1>";

// //function in js
// function myFunction(){
//     console.log('HEllo World');
// }
// myFunction();

// //to add an event listener
// div.addEventListener(type, function);

// //my data
// let array = ['one','two','three','four'];