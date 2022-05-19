class Rectangle {
    constructor(height, width) {
        this.height = height;
        this.width = width;

        // attributes :
        this.adjective = width > 25 ? "big" : "small";
    }

    // equivalents of python properties :
    get area() {
        return this.height * this.width;
    }

    // methods :
    is_square() {
        return this.height === this.width;
    }

    // static methods :
    static create_square(length) {
        return new Rectangle(length, length);
    }

}

export {
    Rectangle,
};
