#include <iomanip>
#include <climits>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <tuple>
#include <regex>
#include <filesystem>
#include "string.hpp"
using namespace std;
using namespace std::filesystem;

void usage(string program)
{
	cerr << "pdb/pdbqt residue renumbering tool by Maozi Chen" << endl;
	cerr << "version 1.0.1" << endl;
	cerr << endl;
	cerr << "usage:" << endl;
	cerr << "  " << program << " input.pdb [from1].[to1]:[offset1] [from2].[to2]:[offset2] ..." << endl;
	cerr << "  where" << endl;
	cerr << "    (optional) [from] is the lower bound of residue number for the rule, " << endl;
	cerr << "    (optional) [to] is the upper bound of residue number for the rule," << endl;
	cerr << "               [offset] is a positive/negative number or x (means delete) to be applied to residues in the inclusive interval [from, to]." << endl;
	cerr << endl;
	cerr << "example:" << endl;
	cerr << "  [add 200 to all residue numbers]     \t" << program << " input.pdb +200" << endl;
	cerr << "  [add 200 to residue numbered 412]    \t" << program << " input.pdb 412:+200" << endl;
	cerr << "  [subtract 100 from those <= 200]     \t" << program << " input.pdb .200:-100" << endl;
	cerr << "  [subtract 100 from those >= 200]     \t" << program << " input.pdb 200.:-100" << endl;
	cerr << "  [add 100 to those >=100 and <=200]   \t" << program << " input.pdb 100.200:+100" << endl;
	cerr << "  [delete residues with number >= 100] \t" << program << " input.pdb 100.:x" << endl;
	cerr << "  [combining multiple rules]           \t" << program << " input.pdb .100:-100 101.200:x -200" << endl;
	cerr << endl;
	cerr << "note:" << endl;
	cerr << "  when no bounds defined, both dot (.) and colon (:) can be ignored." << endl;
	cerr << "  rules are executed sequentially, so an unbounded rule should only appear in the last." << endl;
}

int main(int argc, char* argv[])
{
	if (argc < 3) // program input rule1
	{
		usage(argv[0]);
		return 1;
	}

	path p(argv[1]);

	vector<tuple<int, int, int>> rules;
	for (int i = 2; i < argc; i++)
	{
		string rule(argv[i]);
		smatch ms;

		if (!regex_match(rule, ms, regex("((([+-]?\\d+)|([+-]?\\d+)?\\.([+-]?\\d+)?):)?(x|[+-]\\d+)")))
		{
			cerr << "*****[ERROR] unrecognized rule '" << rule << "'" << endl << endl;
			usage(argv[0]);
			return 2;
		}

		int low = INT_MIN, high = INT_MAX, delta = INT_MIN;
		if (ms[3].str().size())
			low = high = stoi(ms[3].str());
		else
		{
			if (ms[4].str().size())
				low = stoi(ms[4].str());
			if (ms[5].str().size())
				high = stoi(ms[5].str());
		}
		if (ms[6].str() != "x")
			delta = stoi(ms[6].str());

		rules.emplace_back(low, high, delta);
	}

	try
	{
		string line;
		ofstream out(path("renumbered_") += p.filename());
		int renumbered = 0, deleted = 0, untouched = 0, errors = 0, ln = 1;
		bool first = true;

		for (ifstream f(p); safe_getline(f, line); ln++)
		{
			auto sig = line.substr(0, 6);
			bool hit = false;

			if (line.length() >= 26 && (sig == "ATOM  " || sig == "HETATM" || sig == "ANISOU" || sig == "TER   "))
			{
				int seq = stoi(line.substr(22, 4));

				bool del = false;
				for (auto& [low, high, delta] : rules)
				{
					if (seq >= low && seq <= high)
					{
						hit = true;
						if (delta == INT_MIN)
							del = true;
						else
						{
							seq += delta;
							if (seq > 9999 || seq < -999)
							{
								errors++;
								cerr << "*****[ERROR] residue number overflow: " << seq << " on line " << ln << endl;
							}
						}
						break;
					}
				}

				if (hit)
				{
					if (del)
						deleted++;
					else
					{
						if (first)
							first = false;
						else
							out << endl;
						renumbered++;
						out << line.substr(0, 22) << setw(4) << seq << line.substr(26);
					}
					continue;
				}
			}

			untouched++;
			if (first)
				first = false;
			else
				out << endl;
			out << line;
		}

		cout << "*****[DONE] renumbered " << renumbered << " lines, deleted " << deleted << " lines, untouched " << untouched << " lines, error " << errors << " lines" << endl;
		return 0;
	}
	catch (exception & ex)
	{
		cerr << ex.what() << endl;
		return 2;
	}
}
