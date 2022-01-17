#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <vector>

#include <boost/multiprecision/cpp_int.hpp>

typedef boost::multiprecision::int1024_t integer;

std::map<integer, bool> db;

integer hashing(const std::vector<int> &sequence) {
    integer hash{0};
    for (auto i{1}; i < sequence.size(); i++) {
        hash += sequence[i] << i;
    }
    return hash;
}

bool next_orbit(std::vector<int> &sequence) {
    integer key;
    for (auto i{1}; i < sequence.size(); i++) {
        key = hashing(sequence);
        if (!db[key]) {
            db[key] = true;
            goto finally;
        } else {
            sequence[i] = !sequence[i];
        }
    }
    return false;
    finally:
    return true;
}

integer ssp_oracle(std::vector<int> &sequence, const std::vector<integer> &universe) {
    integer local{universe.front()};
    for (auto i{1}; i < sequence.size(); i++) {
        if (sequence[i]) {
            local += universe[i];
            if (local >= 0) {
                return local;
            }
        }
    }
    return -local;
}

std::vector<int> hess(std::vector<integer> &universe, const int &number_of_variables) {
    std::sort(universe.begin(), universe.end());
    std::reverse(universe.begin() + 1, universe.end());
    integer limit{std::numeric_limits<integer>::max()};
    auto n = static_cast<int>(number_of_variables);
    std::vector<int> sequence(n, 0);
    sequence.front() = 1;
    std::cout.precision(std::numeric_limits<int>::max_digits10 + 1);
    integer cursor{limit};
    while (next_orbit(sequence)) {
        integer local{0}, global{limit};
        for (auto i{1}; i < n; i++) {
            sequence[i] = !sequence[i];
            local = ssp_oracle(sequence, universe);
            if (local < global) {
                global = local;
                if (global < cursor) {
                    cursor = global;
                    std::cout << "c " << cursor << std::endl;
                    if (global == 0) {
                        goto finally;
                    }
                }
            } else if (local > global) {
                sequence[i] = !sequence[i];
            }
        }
    }
    sequence.clear();
    finally:
    return sequence;
}


int main(int argc, char *argv[]) {
    int number_of_variables;
    std::string temporal;
    std::vector<integer> universe;
    std::ifstream file(argv[1]);
    file.seekg(0, std::ifstream::end);
    int length = file.tellg();
    file.seekg(0, std::ifstream::beg);
    char *buffer = new char[length];
    std::cout << "c reading " << length << " characters from " << argv[1] << std::endl;
    file.read(buffer, length);
    if (file) {
        std::stringstream buffer_stream(buffer);
        for (std::string line; std::getline(buffer_stream, line);) {
            if (line.front() == 'c') {
                continue;
            }
            std::stringstream line_stream(line);
            if (line.front() == 'p') {
                line_stream >> temporal; // p
                line_stream >> temporal; // ssp
                line_stream >> number_of_variables;
                continue;
            }
            while (line_stream.good()) {
                line_stream >> temporal;
                universe.emplace_back(integer(temporal.c_str()));
            }
        }
    } else {
        throw std::runtime_error("FILE READING ERROR");
    }
    file.close();
    delete[] buffer;
    temporal.clear();

    auto solution = hess(universe, number_of_variables);

    if (!solution.empty()) {
        std::cout << "SAT" << std::endl;
        std::cout << "[";
        integer local{};
        for (auto i{0}; i < solution.size(); i++) {
            if (solution[i]) {
                local += universe[i];
                std::cout << universe[i] << ", ";
                if (local >= 0) {
                    break;
                }
            }
        }
        std::cout << "]" << std::endl;
    } else {
        std::cout << "UNSAT" << std::endl;
    }

    return EXIT_SUCCESS;
}
// 299143602211297741204568619614507931500043196174228961815838679604385362082070060023736643021
// 170940884877719966997141193892121109
// 2282989190483180967303957107017620
// 850540759120787036555324586910735
// 35667904713522356584935352659340
// 167907321433991906607695400224