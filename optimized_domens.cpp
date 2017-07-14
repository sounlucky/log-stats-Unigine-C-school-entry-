//#include "stdafx.h"

#include <sstream>
#include <iostream>
#include <string>
#include <fstream>
#include <algorithm>
#include <map>
#include <set>
#include <unordered_map>

void exit_with(std::string er)
{
	std::cout << "Something went wrong. Probably " << er << '.';
	exit(-1);
}

bool can_be_in_domain(char c)
{
	return ('A' <= c && c <= 'Z') || ('a' <= c && c <= 'z') || ('0' <= c && c <= '9')
		|| c == '.' || c == '-';
}

bool can_be_in_path(char c)
{
	return  ('A' <= c && c <= 'Z') || ('a' <= c && c <= 'z') || ('0' <= c && c <= '9')
		|| c == '.' || c == ',' || c == '/' || c == '+' || c == '_';
}

struct Container
{
	char domain_buffer[1 << 10];
	char path_buffer[1 << 10];

	struct cmp_ints
	{
		bool operator()(const int a, const int b) const
		{
			return a > b;
		}
	};

	std::unordered_map<std::string, size_t> domains, paths;

	void parse_add_items(char* line, size_t& pos)
	{
		int domain_pos = 0, path_pos = 0;
		while (can_be_in_domain(line[pos]))
			domain_buffer[domain_pos++] = line[pos++];
		while (can_be_in_path(line[pos]))
			path_buffer[path_pos++] = line[pos++];
		std::string path_str(path_buffer , path_pos), domain_str(domain_buffer , domain_pos);
		if (path_pos = 0 || path_str == "")
			paths["/"]++;
		else
			paths[path_str]++; 
		domains[domain_str]++;
	}

	void out_result(std::ofstream& stream, size_t n)
	{
		size_t written_values = 0;//iterating to check if its > n
		std::map<size_t, std::set<std::string>, cmp_ints> domains_reversed, paths_reversed;
		stream << ", domains " << domains.size() << ", paths " << paths.size() << '\n';
		stream << '\n' << "top domains" << '\n';
		for (auto& i : domains)
			domains_reversed[i.second].insert(std::move(i.first));
		for (auto i = domains_reversed.cbegin(); i != domains_reversed.cend() && written_values < n; i++)
			for (auto j = i->second.cbegin(); j != i->second.cend() && written_values < n; j++) {
				written_values++;
				stream << i->first << ' ' << *j << '\n';
			}
		written_values = 0;
		stream << '\n';
		stream << "top paths" << '\n';
		for (auto& i : paths)
			paths_reversed[i.second].insert(std::move(i.first));
		for (auto i = paths_reversed.cbegin(); i != paths_reversed.cend() && written_values < n; i++)
			for (auto j = i->second.cbegin(); j != i->second.cend() && written_values < n; j++) {
				written_values++;
				stream << i->first << ' ' << *j << '\n';
			}
	}
};

bool seek_for_domain(char*& in_str, size_t& pos) {
	while (in_str[pos] != 0) {
		if (in_str[pos++] == 'h')
			if (in_str[pos++] == 't')
				if (in_str[pos++] == 't')
					if (in_str[pos++] == 'p')
						switch (in_str[pos++]) {
						case 's':
							if (in_str[pos++] == ':')
								if (in_str[pos++] == '/')
									if (in_str[pos++] == '/')
										return true;
							break;
						case ':':
							if (in_str[pos++] == '/')
								if (in_str[pos++] == '/')
									return true;
							break;
						default:
							break;
						}
	}
	return false;
}

int main(int argc, char* argv[])
{
	size_t n = 0xFFFFFFFF;
	size_t num_of_urls = 0;

	if (argc < 2) {
		exit_with("not enough parameters passed");
	}
	std::ifstream in_file;
	std::ofstream ou_file;
	if (std::string(argv[1]) == std::string("-n"))
	{
		n = std::atoi(argv[2]);
		in_file.open(argv[3], std::ios::binary);
		ou_file.open(argv[4], std::ios::binary);
	}
	else
	{
		in_file.open(argv[1], std::ios::binary);
		ou_file.open(argv[2], std::ios::binary);
	}

	Container container;

	in_file.seekg(0, in_file.end);

	size_t data_size = in_file.tellg();
	char* data = new char[data_size + 1]; data[data_size] = 0;
	in_file.seekg(0, in_file.beg);
	in_file.read(data, data_size);
	size_t register pos = 0;
	while (seek_for_domain(data, pos))
	{
		num_of_urls++;
		container.parse_add_items(data, pos);;
	}

	ou_file << "total urls " << num_of_urls;
	container.out_result(ou_file, n);
	//system("pause");
	return 0;
}