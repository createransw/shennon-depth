// recursively generate formulas of functions of 3 variables
// depth induction

#include <algorithm>
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <utility>
#include <vector>

using namespace std;

void Enumeration(vector<int32_t>& depthsList) {
    depthsList.resize(0x100, -1);

    // set functions with 0 depth
    depthsList[0x0F] = 0;
    depthsList[0x33] = 0;
    depthsList[0x55] = 0;

    depthsList[0xF0] = 0;
    depthsList[0xCC] = 0;
    depthsList[0xAA] = 0;

    bool allFlg = true;
    for (int32_t depth = 1; allFlg; ++depth) {
        allFlg = false;

        for (uint32_t firstArg = 0; firstArg < depthsList.size(); ++firstArg) {
            if (depthsList[firstArg] != depth - 1) {
                continue;
            }

            allFlg = true;

            // try conjunction and disjunction
            for (uint32_t secondArg = 0; secondArg < depthsList.size(); ++secondArg) {
                if ((depthsList[secondArg] < 0) || (depthsList[secondArg] >= depth)) {
                    continue;
                }

                int32_t current = depthsList[firstArg & secondArg];
                if ((current < 0) || (current > depth)) {
                    depthsList[firstArg & secondArg] = depth;
                }

                current = depthsList[~(firstArg & secondArg) & 0xFF];
                if ((current < 0) || (current > depth)) {
                    depthsList[~(firstArg & secondArg) & 0xFF] = depth;
                }

                current = depthsList[firstArg | secondArg];
                if ((current < 0) || (current > depth)) {
                    depthsList[firstArg | secondArg] = depth;
                }

                current = depthsList[~(firstArg | secondArg) & 0xFF];
                if ((current < 0) || (current > depth)) {
                    depthsList[~(firstArg | secondArg) & 0xFF] = depth;
                }
            }
        }
    }
}

void Print(vector<int32_t> &v) {
    pair<int32_t, size_t> maxDepth(v[0], 0);

    for (size_t i = 0; i < v.size(); ++i) {
        if (v[i] >= maxDepth.first) {
            maxDepth.first = v[i];
            maxDepth.second = i;
        }
    }
    cout << "D(3) = " << maxDepth.first << endl;
    cout << "number of function with max depth: ";
    //cout << "0x" << setw(2) << setfill('0') << hex << maxDepth.second << endl;
    cout << maxDepth.second << endl;
}

void PrintTable(vector<int32_t> &values, vector<bool> &mask) {
    if (values.size() != mask.size()) {
        return;
    }

    for (int32_t i = 0; i <= *max_element(values.begin(), values.end()); ++i) {
        cout << i << "\t|\t";

        size_t first = 0;
        size_t last = 0;

        while (first < values.size()) {
            while ((first < values.size()) && ((values[first] != i) || (!mask[first]))) {
                first++;
            }

            last = first + 1;

            while ((last <= values.size()) && (values[last] == i) && (mask[last])) {
                last++;
            }

            if (first == values.size()) {
                break;
            }

            if (last == first + 1) {
                cout << first << ' ';
            } else {
                cout << first << '-' << last - 1 << ' ';
            }

            first = last;
        }

        cout << endl << "_____________________________________________________" << endl;
    }
}

void ThreeVariablesMask(vector<bool> &mask) {
    mask.resize(256, true);

    for (int32_t i = 0; i < mask.size(); ++i) {
        if ((i & 0b1111) == ((i>>4) & 0b1111)) {
            mask[i] = false;
            continue;
        }

        if ((i & 0b11) == ((i>>2) & 0b11)) {
            if (((i>>4) & 0b11) == ((i>>6) & 0b11)) {
                mask[i] = false;
                continue;
            }
        }

        if ((i & 0b1) == ((i>>1) & 0b1)) {
            if (((i>>2) & 0b1) == ((i>>3) & 0b1)) {
                if ((i>>4 & 0b1) == ((i>>5) & 0b1)) {
                    if (((i>>6) & 0b1) == ((i>>7) & 0b1)) {
                        mask[i] = false;
                    }
                }
            }
        }
    }
}

int main() {
    vector<int32_t> v;
    Enumeration(v);

    /*vector<bool> mask_all = vector<bool>(v.size(), true);
    PrintTable(v, mask_all);

    Print(v);

    cout << endl << endl << endl;*/

    vector<bool> mask_variables;
    for (auto i : mask_variables) {
        cout << i;
    }
    ThreeVariablesMask(mask_variables);

    PrintTable(v, mask_variables);

    return 0;
}
