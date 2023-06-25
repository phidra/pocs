#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <functional>
#include <cassert>

using namespace std;

// Cette POC montre comment utiliser getline pour parser les lignes d'un fichier.
//
// NOTE : c'est la suite de la POC sur le parsing de ligne.
// NOTE : ce code n'est PAS robuste à un input-file dont le dernier caractère n'est pas un saut de ligne.

vector<string> split_line(string const& line)
{
    // pour cette POC, le délimiteur est fixé :
    const char COMMA = ',';

	vector<string> tokens;
    stringstream ss(line);

    // NOTE : une raison pour laquelle ce code se comporte comme expected est que si getline ne trouve rien, il renvoie une string vide.
    while(ss.good() )
    {
        string substr;
        getline(ss, substr, COMMA);
        tokens.push_back(substr);
    }
    return tokens;
}

vector<vector<string>> parse_file(string const& filename)
{
    vector<vector<string>> to_return;

    ifstream file(filename);

    while(file.good())
    {
        string line;
        getline(file, line);
        // pour tenir compte du saut de ligne final du fichier (en effet, un fichier correctement
        // formaté est supposé se terminer par un saut de ligne), on n'ajoute le dernier token que
        // si le stream est encore valide après avoir utilisé getline (i.e. on n'a pas encore atteint
        // la fin du fichier).
        //
        // Dit autrement, la séquence de fin de fichiers est supposée être :
        //      line\nline\nEOF
        // Lorsque getline s'arrête à cause du dernier '\n', il faut ajouter un nouveau token.
        // Par contre, lorsque getline s'arrête à cause du EOF, il ne faut pas ajouter de nouveau token vide.
        if (file.good())
        {
            vector<string> line_tokens = split_line(line);
            to_return.push_back(line_tokens);
        }
    }
    return to_return;
}

bool check_file(string filename, vector<vector<string>> const& expected_file_tokens)
{
    cout << boolalpha;
    vector<vector<string>> file_tokens = parse_file(filename);
    cout << (file_tokens == expected_file_tokens ? "[OK]" : "BOUM") << "      " << filename << endl;
}

int main(void)
{
    // Ces tests indiquent le comportement souhaité dans chaque cas.
    // Notamment, dans plusieurs cas, on VEUT récupérer des tokens vides.
	check_file("test01_nominal.txt",               {{"luke","35","AAA"},{"padme","62","BBB"}});
	check_file("test02_emptyline_end.txt",         {{"luke","35","AAA"},{"padme","62","BBB"},{""}});
	check_file("test03_emptyline_middle.txt",      {{"luke","35","AAA"},{""},{"padme","62","BBB"}});
	check_file("test04_finalcomma_end.txt",        {{"luke","35","AAA"},{"padme","62","BBB", ""}});
	check_file("test05_finalcomma_middle.txt",     {{"luke","35","AAA"},{"padme","62","BBB", ""},{"anakin","55","CCC"}});
	check_file("test06_commathenempty_end.txt",    {{"luke","35","AAA"},{"padme","62","BBB"},{"anakin","55","CCC", ""}, {""}});
	check_file("test07_commathenempty_middle.txt", {{"luke","35","AAA"},{"padme","62","BBB", ""},{""}, {"anakin","55","CCC"}});
	check_file("test08_emptyfile.txt",             {});  // aucun caractère dans le fichier -> parsé comme une seule ligne vide
	check_file("test09_emptyline.txt",             {{""}}); // un seul caractère dans le fichier (\n) -> parsé comme deux lignes vides
	check_file("test10_singlecomma.txt",           {{"",""}});
	check_file("test11_onefield.txt",              {{"luke"}});
	check_file("test12_onefieldcomma.txt",         {{"luke",""}});
	check_file("test13_commaonefield.txt",         {{"","luke"}});
}
