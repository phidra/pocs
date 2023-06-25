#include <vector>
#include <iostream>

#include "boost/geometry.hpp"
#include "boost/geometry/geometries/point.hpp"
#include "boost/geometry/index/rtree.hpp"

namespace bg = boost::geometry;
namespace bgi = boost::geometry::index;


int main(int argc, char** argv) {
    using Point = bg::model::point<long double, 2, bg::cs::spherical_equatorial<bg::degree> >;
    using Value = std::pair<Point, size_t>;
    bgi::rtree<Value, bgi::quadratic<16> > rtree;

    std::cout << R"POUET(
Cette POC montre l'utilisation de boost::geometry et de son R-Tree pour faire des requêtes spatiales efficaces.

On remplit le R-Tree avec une petite centaine de long;lat, puis on requête les 5 points les plus proches d'un
point de référence.
)POUET" << std::endl;

	constexpr const int nb_points = 85;
	std::array<Point, nb_points> values = {
		Point{2.314682, 48.878715}, Point{2.331161, 48.896775}, Point{2.342834, 48.875554}, Point{2.384719, 48.899032},
		Point{2.384719, 48.883231}, Point{2.393646, 48.867876}, Point{2.414932, 48.855679}, Point{2.421798, 48.837153},
		Point{2.278289, 48.922499}, Point{2.248764, 48.912572}, Point{2.230911, 48.887294}, Point{2.210311, 48.866521},
		Point{2.207565, 48.852065}, Point{2.239151, 48.836249}, Point{2.257690, 48.822236}, Point{2.284469, 48.820880},
		Point{2.344207, 48.823592}, Point{2.355194, 48.822236}, Point{2.392959, 48.828113}, Point{2.422485, 48.831277},
		Point{2.425231, 48.863811}, Point{2.404632, 48.891809}, Point{2.365493, 48.901740}, Point{2.337341, 48.901289},
		Point{2.298889, 48.898580}, Point{2.277603, 48.876006}, Point{2.261810, 48.855679}, Point{2.288589, 48.843028},
		Point{2.311248, 48.836701}, Point{2.345581, 48.838509}, Point{2.360687, 48.841672}, Point{2.393646, 48.839864},
		Point{2.403259, 48.845287}, Point{2.405319, 48.854324}, Point{2.390899, 48.859745}, Point{2.379226, 48.865618},
		Point{2.378540, 48.874651}, Point{2.373046, 48.882328}, Point{2.354507, 48.893163}, Point{2.299575, 48.879167},
		Point{2.287902, 48.856131}, Point{2.304382, 48.851161}, Point{2.323608, 48.846643}, Point{2.344894, 48.846643},
		Point{2.362747, 48.846643}, Point{2.375793, 48.850258}, Point{2.383346, 48.852065}, Point{2.364807, 48.866069},
		Point{2.364807, 48.878715}, Point{2.350387, 48.879167}, Point{2.334594, 48.879167}, Point{2.321548, 48.877360},
		Point{2.305068, 48.866973}, Point{2.305068, 48.859745}, Point{2.322235, 48.855228}, Point{2.345581, 48.853872},
		Point{2.361373, 48.853872}, Point{2.355194, 48.863359}, Point{2.348327, 48.869683}, Point{2.329788, 48.871038},
		Point{2.338027, 48.861101}, Point{2.206878, 48.881876}, Point{2.223358, 48.913926}, Point{2.244644, 48.927913},
		Point{2.269363, 48.940092}, Point{2.327728, 48.954522}, Point{2.383346, 48.950915}, Point{2.408752, 48.950915},
		Point{2.462997, 48.940543}, Point{2.489776, 48.912572}, Point{2.498703, 48.882779}, Point{2.498703, 48.841220},
		Point{2.486343, 48.819072}, Point{2.454757, 48.802793}, Point{2.381286, 48.792842}, Point{2.316055, 48.789223},
		Point{2.227478, 48.796008}, Point{2.176666, 48.815454}, Point{2.156753, 48.852517}, Point{2.166366, 48.891809},

        // Ces points sont les 5 points les plus proches du point recherché :
		Point{2.357597, 48.875723},
		Point{2.357940, 48.872788},
		Point{2.353134, 48.872506},
		Point{2.351331, 48.874086},
		Point{2.353391, 48.876006}
	};

    for ( size_t i = 0 ; i < nb_points ; ++i )
    {
        rtree.insert({values[i], i});
    }

    std::cout << "Le rtree contient maintenant " << rtree.size() << " valeurs" << std::endl;

	// find 5 nearest values to a reference point
	std::vector<Value> closest_points;
	Point ref{2.354593, 48.874086};
	rtree.query(bgi::nearest(ref, 5), std::back_inserter(closest_points));


	std::cout << "REFERENCE : " << bg::wkt<Point>(ref) << std::endl;
	std::cout << "5 points les plus proches de la référence et leur indice dans l'array :" << std::endl;
    for (auto close_point: closest_points)
    {
		std::cout << "\t" << bg::wkt<Point>(close_point.first) << " - " << close_point.second << std::endl;
    }
    return 0;
}
