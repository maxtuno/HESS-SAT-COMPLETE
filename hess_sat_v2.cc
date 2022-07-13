/*
MIT License
Copyright (c) 2012-2022 Oscar Riveros (https://twitter.com/maxtuno, Chile).
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
#include <iostream>
#include <map>
#include <vector>
#include <chrono>
#include <cmath>
#include <thread>
#include <mutex>
#include <random>

#include "picosha2.h"

std::mutex mutex;

int base = 127;

typedef std::size_t integer;
std::map<integer, bool> db;

std::size_t hashing(const std::vector<unsigned char> &sequence) {
    std::size_t hash{0};
    for (auto &&k: sequence) {
        hash ^= k + 0x9e3779b9 + (hash << 6) + (hash >> 2);
    }
    return hash;
}

void step(int i, int j, int k, std::vector<unsigned char> &bit) {
    std::swap(bit[i], bit[j]);
    bit[k] = 32 + (bit[k] + 1) % (base - 32);
}

bool next_orbit(std::vector<unsigned char> &bit) {
    integer key;
    for (auto i{0}; i < bit.size(); i++) {
        for (auto j{0}; j < bit.size(); j++) {
            for (auto k{0}; k < bit.size(); k++) {
                key = hashing(bit);
                mutex.lock();
                if (db.find(key) == db.end()) {
                    db[key] = true;
                    mutex.unlock();
                    return true;
                } else {
                    mutex.unlock();
                    step(i, j, k, bit);
                }
            }
        }
    }
    return false;
}

float sha256_oracle(std::vector<unsigned char> &bit, const std::string &hash, std::string &hash_hex_str, const float &n, const float &cursor) {
    hash_hex_str.clear();
    picosha2::hash256_hex_string(bit, hash_hex_str);
    float local = 0;
    for (auto i{0}; i < hash.size(); i += std::log(hash.size())) { // PCP
        local += std::pow((hash[i] - hash_hex_str[i]) / static_cast<float>(base), 2);
        if (local > cursor) {
            break;
        }
    }
    return std::sqrt(local);
}

void hess(std::string &hash, const int &n, const int id) {
    std::string hash_hex_str;
    auto start = std::chrono::steady_clock::now();
    std::vector<unsigned char> bit(n, ' '), aux;
    auto cursor{std::numeric_limits<float>::max()};
    while (next_orbit(bit)) {
        closure:
        for (auto i{0}; i < n; i++) {
            for (auto j{0}; j < n; j++) {
                for (auto k{0}; k < n; k++) {
                    aux.assign(bit.begin(), bit.end());
                    step(i, j, k, bit);
                    auto local = sha256_oracle(bit, hash, hash_hex_str, n, cursor);
                    if (local < cursor) {
                        cursor = local;
                        auto end = std::chrono::steady_clock::now();
                        mutex.lock();
                        std::cout << "c (" << id << ") " << cursor << " | " << std::chrono::duration_cast<std::chrono::seconds>(end - start).count() << " (s)";
                        if (cursor == 0) {
                            std::cout << " | ";
                            for (auto &item: bit) {
                                std::cout << item;
                            }
                            std::cout << " | " << hash_hex_str << std::endl;
                            mutex.unlock();
                            std::exit(EXIT_SUCCESS);
                        }
                        std::cout << std::endl;
                        mutex.unlock();
                        goto closure;
                    } else if (local > cursor) {
                        bit.assign(aux.begin(), aux.end());
                    }
                }
            }
        }
    }
}

int main(int argc, char *argv[]) {

    std::string hash{argv[1]};
    std::transform(hash.begin(), hash.end(), hash.begin(), ::tolower);

    auto nt = std::atoi(argv[3]);

    std::vector<std::thread> th;

    for (auto i{0}; i < nt; i++) {
        th.emplace_back([&]() { hess(hash, std::atoi(argv[2]), i); });
    }

    for (auto i{0}; i < nt; i++) {
        th[i].join();
    }

    return EXIT_SUCCESS;
}
