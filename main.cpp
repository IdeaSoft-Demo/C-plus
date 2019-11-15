#include <stdio.h>
#include <vector>
#include <stdexcept>
#include <iostream>

#define CATCH_CONFIG_MAIN
#include "catch.hpp"

using namespace std;

/* Task 1
 * As i got no answer on which exactly bit operator I should use
 * I felt free to use any I cosidered to be needed. */

int bit_add(int a, int b) {
    //initializing helper variables
    int carry, result(a), arg2(b);

    //performing loop while second argument has value
    while (arg2 != 0) {
        //calculating carry bit
        carry = result & arg2;
        //calculating arguments sum
        result = result ^ arg2;
        //switching carry variable
        arg2 = carry << 1;
    }

    //only in case initial arguments had the same sign and it has changed after the sum
    //we can be sure we got an integer overflow
    if (((~(a ^ b)) & (result ^ a)) < 0 ){
            throw overflow_error("Integer overflowed.");
    }

    return result;
}

/*Task 2
 * The task does not contain a lot of important information upon the initial set,
 * the purpose and restrictions on solution. Depending on different restrictions
 * (like memory usage, or computing speed) the algorithm may change, and different
 * additional heuristics can be applied. But as far as it was mentioned, that
 * dynamic programming is preffered - the solution contains exactly dynamic
 * programming approach for solving "subset sum problem".*/

void find_largest_sum(uint32_t T, const vector<uint32_t>& v, vector<uint32_t>& result, uint32_t& S) {
    // checking the initial set and the target sum not to be 0
    if (v.size() == 0 || T == 0)
        return;
    // preparing table for storing values
    vector<vector<bool>> table(v.size() + 1, vector<bool>(T + 1));
    size_t n = v.size();

    // filling the zero column
    for (size_t i = 0; i <= n; i++)
          table[i][0] = true;

    // filling the subset table in bottom up manner
    for (size_t i = 1; i <= n; i++) {
        for (size_t j = 1; j <= T; j++) {
            if(j<v[i-1])
                table[i][j] = table[i-1][j];
            if (j >= v[i-1])
                table[i][j] = table[i-1][j] || table[i - 1][j - v[i-1]];
        }
    }

    // difining the result subset
    size_t j = T;
    for (size_t i = n; i > 0; i--) {
        while (j > 0) {
            if (table[i][j]) {
                if (table[i-1][j]) {
                    break;
                } else {
                    result.push_back(v[i-1]);
                    S += v[i-1];
                    j -= v[i-1];
                    break;
                }
            }
            j--;
        }
        if (S > T) {
            // there's no exact subset, returning the closest one
            S -= result.back();
            result.pop_back();
            return;
        } else if (S == T) {
            // found the exact answer
            return;
        }
    }

    return;
}

TEST_CASE( "Bitwise additions are computed", "[addition]" ) {
    REQUIRE( bit_add(0, 0) == 0 );
    REQUIRE( bit_add(-1, 1) == 0 );
    REQUIRE( bit_add(1, 2) == 3 );
    REQUIRE( bit_add(2, 5) == 7 );
    REQUIRE( bit_add(489, 251) ==  740);
    REQUIRE( bit_add(135464, 396456) == 531920 );
    REQUIRE( bit_add(-3215, 543122) == 539907 );
    REQUIRE( bit_add(12354, -9237841) == -9225487 );
    REQUIRE_THROWS( bit_add(2147483647, 2));
    REQUIRE_THROWS( bit_add(-2147483648, -2));
}

TEST_CASE( "Subset sum search", "[subset sum]" ) {
    SECTION( "standard array" ) {
        vector<uint32_t> v{ 2, 5, 9, 3, 13, 5, 8, 2, 1, 9, 9, 6, 1, 10, 2, 5, 21, 4, 6, 15, 8, 4, 9, 2, 13, 4, 5, 6 };
        vector<uint32_t> cor{ 2, 5, 13, 3, 9, 5, 2 };
        vector<uint32_t> res;
        uint32_t T = 39;
        uint32_t S = 39;

        find_largest_sum(T, v, res, S);

        REQUIRE( res == cor );
        REQUIRE( S == 0 );
    }

}
