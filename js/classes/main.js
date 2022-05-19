import {Rectangle} from './rectangle.js';

// note : modules are automatically deferred

const my_rectangle = new Rectangle(10, 20);
document.getElementById("rectangle_width").textContent = my_rectangle.width;
document.getElementById("rectangle_height").textContent = my_rectangle.height;
document.getElementById("rectangle_area").textContent = my_rectangle.area;
document.getElementById("rectangle_is_square").textContent = my_rectangle.is_square();
document.getElementById("rectangle_adjective").textContent = my_rectangle.adjective;

const my_square = Rectangle.create_square(30, 30);
document.getElementById("square_width").textContent = my_square.width;
document.getElementById("square_height").textContent = my_square.height;
document.getElementById("square_area").textContent = my_square.area;
document.getElementById("square_is_square").textContent = my_square.is_square();
document.getElementById("square_adjective").textContent = my_square.adjective;
