/*
Copyright (c) 2012-2022 Oscar Riveros (@maxtuno, Chile).

Permission is hereby granted, free of charge, to any person obtaining
a copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including
without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to
permit persons to whom the Software is furnished to do so, subject to
the following conditions:

The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <vector>

#include <boost/multiprecision/cpp_int.hpp>

typedef boost::multiprecision::cpp_int integer;

std::map<integer, bool> db;

integer hashing(const std::vector<int> &sequence) {
    integer hash{0};
    for (auto i{1}; i < sequence.size(); i++) {
        if (sequence[i]) {
            hash += integer(1) << i;
        }
    }
    return hash;
}

bool next_orbit(std::vector<int> &sequence) {
    integer key;
    for (auto i{0}; i < sequence.size(); i++) {
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

int maxsat_oracle(std::vector<int> &sequence, const std::vector<std::vector<int>> &clauses) {
    int local{static_cast<int>(clauses.size())};
    for (auto &clause: clauses) {
        for (auto i{0}; i < clause.size(); i++) {
            if (sequence[abs(clause[i]) - 1] == clause[i] > 0) {
                local--;
                break;
            }
        }
    }
    return local;
}

std::vector<int> hess(const std::vector<std::vector<int>> &clauses, const int &number_of_variables, const int &number_of_clauses) {
    auto n = static_cast<int>(number_of_variables);
    std::vector<int> sequence(n, 0);
    std::cout.precision(std::numeric_limits<int>::max_digits10 + 1);
    auto cursor{std::numeric_limits<int>::max()};
    while (next_orbit(sequence)) {
        auto local{0}, global{std::numeric_limits<int>::max()};
        for (auto i{0}; i < n; i++) {
            sequence[i] = !sequence[i];
            local = maxsat_oracle(sequence, clauses);
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
    int number_of_variables, number_of_clauses;
    std::string temporal;
    std::vector<std::vector<int>> clauses;
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
                line_stream >> temporal; // cnf
                line_stream >> number_of_variables;
                line_stream >> number_of_clauses;
                continue;
            }
            std::vector<int> literals;
            while (line_stream.good()) {
                line_stream >> temporal;
                if (temporal.front() == '0') {
                    continue;
                }
                literals.emplace_back(std::atoi(temporal.c_str()));
            }
            if (!literals.empty()) {
                clauses.emplace_back(literals);
            }
        }
    } else {
        throw std::runtime_error("FILE READING ERROR");
    }
    file.close();
    delete[] buffer;
    temporal.clear();

    auto solution = hess(clauses, number_of_variables, number_of_clauses);

    if (!solution.empty()) {
        std::cout << "SAT" << std::endl;
        for (auto i{0}; i < solution.size(); i++) {
            std::cout << (solution[i] ? +(i + 1) : -(i + 1)) << " ";
        }
        std::cout << 0 << std::endl;
    } else {
        std::cout << "UNSAT" << std::endl;
    }

    return EXIT_SUCCESS;
}
