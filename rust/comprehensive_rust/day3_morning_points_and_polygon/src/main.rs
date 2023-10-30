// TODO: remove this when you're done with your implementation.
#![allow(unused_variables, dead_code)]

#[derive(Debug, PartialEq, Clone, Copy)]
pub struct Point {
    x: i32,
    y: i32,
}

impl Point {
    fn new(x: i32, y: i32) -> Self {
        Point{x, y}
    }

    fn magnitude(&self) -> f64 {
        let x = self.x as f64;
        let y = self.y as f64;
        (x*x + y*y).sqrt()
    }

    fn dist(&self, other: Point) -> f64 {
        let dx = self.x as f64 - other.x as f64;
        let dy = self.y as f64 - other.y as f64;
        (dx*dx + dy*dy).sqrt()
    }
}

impl std::ops::Add for Point {
    type Output = Self;

    fn add(self, other: Self) -> Self {
        Self {x: self.x + other.x, y: self.y + other.y}
    }
}

pub struct Polygon {
    points: Vec<Point>,
    current_point_index: usize,
}

impl Polygon {
    fn new() -> Self {
        Polygon { points: vec![], current_point_index: 0 }
    }

    fn add_point(self: &mut Self, point: Point) {
        self.points.push(point);
        self.current_point_index = 0;
    }

    fn left_most_point(&self) -> Option<Point> {
        self.points.iter().min_by_key(|p| p.x).cloned()
    }

    // EDIT : j'ai refait à la main, mais il y a plus simple, cf. juste après
    fn iter<'a>(&'a self) -> PolygonIte<'a> {
        PolygonIte{ polygon: &self, current_point_index: 0}
    }

    // EDIT : il me semblait avoir testé ceci sans réussir à le faire marcher.
    //        (sans doute qu'il m'a manqué le fait de renvoyer un impl Iterator du bon item)
    //        c'est un mal pour un bien : implémenter l'itérateur moi-même m'a appris des choses
    // fn iter(&self) -> impl Iterator<Item = &Point> {
    //     self.points.iter()
    // }
}

pub struct PolygonIte<'a> {
    polygon: &'a Polygon,
    current_point_index: usize,
}

impl<'a> Iterator for PolygonIte<'a> {
    type Item = &'a Point;
    fn next(&mut self) -> Option<Self::Item> {
        if self.current_point_index >= self.polygon.points.len() {
            None
        } else {
            self.current_point_index += 1;
            Some(&self.polygon.points[self.current_point_index - 1])
        }
    }
}

pub struct Circle {
    center: Point,
    radius: i32,
}

impl Circle {
    fn new(center: Point, radius: i32) -> Self {
        Circle { center, radius }
    }
}

pub enum Shape {
    Polygon(Polygon),
    Circle(Circle),
}

impl Shape {
    fn perimeter(&self) -> f64 {
        match self {
            Shape::Polygon(p) => {
                let mut total_distance: f64 = 0.;
                let points = p.iter();
                let points_shifted_by_1 = Iterator::chain(p.iter().skip(1), p.iter().take(1));

                for (p1, p2) in std::iter::zip(points, points_shifted_by_1) {
                    total_distance += p1.dist(p2.clone());
                }
                total_distance
            },
            Shape::Circle(c) => { 2. * std::f64::consts::PI * c.radius as f64 },
        }
    }
}

impl From<Polygon> for Shape {
    fn from(poly: Polygon) -> Self {
        Shape::Polygon(poly)
    }
}

impl From<Circle> for Shape {
    fn from(circ: Circle) -> Self {
        Shape::Circle(circ)
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    fn round_two_digits(x: f64) -> f64 {
        (x * 100.0).round() / 100.0
    }

    #[test]
    fn test_point_magnitude() {
        let p1 = Point::new(12, 13);
        assert_eq!(round_two_digits(p1.magnitude()), 17.69);
    }

    #[test]
    fn test_point_dist() {
        let p1 = Point::new(10, 10);
        let p2 = Point::new(14, 13);
        assert_eq!(round_two_digits(p1.dist(p2)), 5.00);
    }

    #[test]
    fn test_point_add() {
        let p1 = Point::new(16, 16);
        let p2 = p1 + Point::new(-4, 3);
        assert_eq!(p2, Point::new(12, 19));
    }

    #[test]
    fn test_polygon_left_most_point() {
        let p1 = Point::new(12, 13);
        let p2 = Point::new(16, 16);

        let mut poly = Polygon::new();
        poly.add_point(p1);
        poly.add_point(p2);
        assert_eq!(poly.left_most_point(), Some(p1));
    }

    #[test]
    fn test_polygon_iter() {
        let p1 = Point::new(12, 13);
        let p2 = Point::new(16, 16);

        let mut poly = Polygon::new();
        poly.add_point(p1);
        poly.add_point(p2);

        let points = poly.iter().cloned().collect::<Vec<_>>();
        assert_eq!(points, vec![Point::new(12, 13), Point::new(16, 16)]);
    }

    #[test]
    fn test_shape_perimeters() {
        let mut poly = Polygon::new();
        poly.add_point(Point::new(12, 13));
        poly.add_point(Point::new(17, 11));
        poly.add_point(Point::new(16, 16));
        let shapes = vec![
            Shape::from(poly),
            Shape::from(Circle::new(Point::new(10, 20), 5)),
        ];
        let perimeters = shapes
            .iter()
            .map(Shape::perimeter)
            .map(round_two_digits)
            .collect::<Vec<_>>();
        assert_eq!(perimeters, vec![15.48, 31.42]);
    }
}

#[allow(dead_code)]
fn main() {}
