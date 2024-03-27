#include <httplib.h>
#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>

#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

using namespace std;

void handle_echo(httplib::Request const& req, httplib::Response& res) {
    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);

    rapidjson::Document doc(rapidjson::kObjectType);
    rapidjson::Document::AllocatorType& a = doc.GetAllocator();
    doc.AddMember("method", rapidjson::Value().SetString(req.method.c_str(), a), a);
    doc.AddMember("version", rapidjson::Value().SetString(req.version.c_str(), a), a);
    doc.AddMember("path", rapidjson::Value().SetString(req.path.c_str(), a), a);

    // reading params :
    unordered_map<string, vector<string>> all_the_params;
    for (auto [key, value] : req.params) {
        // first time we encounter the param :
        if (all_the_params.find(key) == all_the_params.end()) {
            all_the_params.insert({key, {}});
        }

        // the order of the various params is lost
        // but for a given repeated param, the order of its repeated items is kept
        all_the_params[key].emplace_back(value);
    }

    // serializing params :
    rapidjson::Value params(rapidjson::kObjectType);
    for (auto [key, values] : all_the_params) {
        rapidjson::Value this_param_values(rapidjson::kArrayType);
        for (auto value : values) {
            this_param_values.PushBack(rapidjson::Value().SetString(value.c_str(), a), a);
        }
        params.AddMember(rapidjson::Value().SetString(key.c_str(), a), this_param_values, a);
    }
    doc.AddMember("params", params, a);

    // headers :
    rapidjson::Value headers(rapidjson::kObjectType);
    for (auto it = req.headers.begin(); it != req.headers.end(); ++it) {
        headers.AddMember(
            rapidjson::Value().SetString(it->first.c_str(), a), rapidjson::Value().SetString(it->second.c_str(), a), a);
    }
    doc.AddMember("headers", headers, a);

    doc.Accept(writer);

    res.set_content(buffer.GetString(), "application/json");
}

int main([[maybe_unused]] int argc, char** argv) {
    int port = stoi(argv[1]);

    // run server :
    httplib::Server svr;
    svr.Get("/echo", handle_echo);

    cout << "Server is about to blockingly listen, you can shut it down with Ctrl+C, and request it with :" << endl;
    cout << endl;
    cout << "    http://localhost:" << port << "/echo?jedis=luke&youpi=hoho&jedis=yoda&jedis=obi-wan" << endl;
    cout << endl;
    svr.listen("0.0.0.0", port);

    cerr << "Exiting" << endl;

    return 0;
}
