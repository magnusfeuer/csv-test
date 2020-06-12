// (C) 2020 - Magnus Feuer
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, version 3.
//
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.
//

// Some simple support functions
#include <cstdint>
#include <string>
#include <vector>

namespace csv {

    // Convert a string line (with no \n at the end) to
    // a vector of string tokens.
    // 'separator' is the character used to delineate fields.
    // `escape` is the character used to escape the next character.
    //
    // SLOW!
    uint32_t tokenize_line(const std::string& line,
                           uint8_t separator,
                           uint8_t escape,
                           std::vector<std::string>& result)
    {
        bool escape_mode(false);
        std::string token("");
        int res(0);

        // Check for nil lines.
        if (!line.length())
            return 0;

        for(uint8_t ch: line) {
            // Was the previous character the escape character?

            // Is this an escape character?
            // If so, switch mode and continue.
            if (ch == escape) {
                escape_mode = true;
                continue;
            }

            // We are not in escape mode, and we found a separator.
            // Wrap up the current token.
            if (!escape_mode && ch == separator) {
                // Push back the complete token on the result vector.
                result.push_back(token);

                token.resize(0); // Reset the token.
                res++;
                continue;
            }

            // We are either in escape mode, accepting any character,
            // and/or we have found a non-separator character.
            // Add it to the current token.
            token.push_back(ch);
        }

        // Push back the final token
        result.push_back(token);

        // Return nu,ber of tokens.
        return res + 1;
    }
}
