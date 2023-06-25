#include <iostream>
#include <boost/log/trivial.hpp>
#include <boost/program_options.hpp>

constexpr int default_age = 42;

int main(int argc, const char *argv[])
{
    try
    {
        boost::program_options::options_description spec{"Options"};
        spec.add_options()("age", boost::program_options::value<int>()->default_value(default_age), "age du capitaine");

        boost::program_options::variables_map vm;
        boost::program_options::store(parse_command_line(argc, argv, spec), vm);

        if (vm.count("age"))
        {
            auto age = vm["age"].as<int>();
            BOOST_LOG_TRIVIAL(info) << "Quel est donc l'age du capitaine ? " << age;
        }
        else
        {
            throw std::runtime_error("on ne devrait pas finir ici, vu qu'on a un 'default_age' !");
        }
    }
    catch (const boost::program_options::error &ex)
    {
        std::cerr << ex.what() << '\n';
    }
}
