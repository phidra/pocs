#include <iostream>

#include "ad/cppgtfs/Parser.h"

using namespace std;

int main(int argc, char* argv[]) {
    if (argc < 2) {
        cout << "ERROR : missing gtfs folder" << endl;
        exit(1);
    }

    string gtfs_folder = argv[1];
    cout << "Parsing GTFS from folder : " << gtfs_folder << endl;

    ad::cppgtfs::Parser parser;
    ad::cppgtfs::gtfs::Feed feed;
    parser.parse(&feed, gtfs_folder);

    cout << "GTFS feed contains  " << feed.getRoutes().size() << " routes" << endl;
    cout << "GTFS feed contains  " << feed.getStops().size() << " stops" << endl;
    cout << "GTFS feed contains  " << feed.getTrips().size() << " trips" << endl;
    cout << endl;
    cout << endl;
    return 0;
}
