#include <CLI/CLI.hpp>
#include <filesystem>
#include <iostream>
#include <string>
#include <tuple>
#include <variant>
#include <vector>

using std::cerr;
using std::cout;
using std::endl;

std::tuple<std::string, std::string, std::string> parse_url(std::string url_input) {
    const std::string DEFAULT_METHOD = "https";
    std::vector<std::string> tokens;
    size_t pos = 0;

    // normalement, il faudrait parser METHOD://HOST/PATH, mais pour simplifier la POC, je parse METHOD/HOST/PATH
    std::string delimiter = "/";
    while ((pos = url_input.find(delimiter)) != std::string::npos) {
        tokens.push_back(url_input.substr(0, pos));
        url_input.erase(0, pos + delimiter.length());
    }
    tokens.push_back(url_input);

    if (tokens.size() == 2) {
        return {DEFAULT_METHOD, tokens[0], tokens[1]};
    } else if (tokens.size() == 3) {
        return {tokens[0], tokens[1], tokens[2]};
    } else {
        throw CLI::ParseError{"Invalid url : valid inputs are 'METHOD/HOST/PATH' or 'HOST/PATH'",
                              CLI::ExitCodes::ValidationError};
    }
}

struct Url {
    std::string method;
    std::string host;
    std::string path;

    Url() = default;
    Url(std::string const& input) { std::tie(method, host, path) = parse_url(input); }

    std::string as_string() const { return method + "+" + host + "+" + path; }
};

using ContentSource = std::variant<std::filesystem::path, Url>;

struct CliArgs {
    ContentSource content_source;

    int other_option;

    std::ostream& to_stream(std::ostream& out) const {
        out << "CliArgs(";
        out << "data-provider=";
        out << visit(
            [&](auto&& the_variant) {
                using T = std::decay_t<decltype(the_variant)>;
                if constexpr (std::is_same_v<T, std::filesystem::path>) {
                    return std::string(the_variant);
                } else if constexpr (std::is_same_v<T, Url>) {
                    return the_variant.as_string();
                }
            },
            content_source);
        out << ", other_option=" << other_option;
        out << ")";
        return out;
    }

    std::string to_string() const {
        std::ostringstream oss;
        to_stream(oss);
        return oss.str();
    }
};

int process(CliArgs args) {
    // On "utilise" (en vrai : on se contente d'afficher) la source du contenu :
    std::visit(
        [&](auto&& the_variant) {
            using T = std::decay_t<decltype(the_variant)>;
            if constexpr (std::is_same_v<T, std::filesystem::path>) {
                cout << "Le contenu est lu depuis un local-file : " << std::string(the_variant) << endl;
            } else if constexpr (std::is_same_v<T, Url>) {
                cout << "Le contenu est lu depuis une URL : " << the_variant.as_string() << endl;
            }
        },
        args.content_source);

    cout << "other_option = " << args.other_option << endl;
    return 0;
}

int main([[maybe_unused]] int argc, [[maybe_unused]] char** argv) {
    CLI::App app{std::string("Ce main est une POC d'utilisation de CLI11.")};
    CliArgs args;

    // On groupe les deux options dans un groupe. Puis on force l'utilisateur à passer exactement l'une des options du
    // groupe.
    auto* content_source = app.add_option_group("content_source", "source of the content");
    content_source
        ->add_option<ContentSource, std::filesystem::path>(
            "-f,--from-file", args.content_source, "Local file containing the content.")
        ->check(CLI::ExistingFile);
    content_source->add_option<ContentSource, Url>("-u,--from-url", args.content_source, "URL containing the content.");
    content_source->require_option(1);

    app.add_option("-o,--other-option", args.other_option, "other illustratory option")->required();

    CLI11_PARSE(app, argc, argv);

    cerr << "ARGS = " << args.to_string() << endl;
    ;

    return process(args);
}
