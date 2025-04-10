#include "task.h"
#include <vector>
#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <iterator>
#include "ScopeGuard.h"
// (:key1 1.0000000000e+01:key2 'c':key3 "D a t a'":)
// (:key1 -1.23:key3 "doomsday":key2 ' ':)

// (:logagooga1 57.069351:key2 'q':key3 "grKSqShULB":)

namespace doomsday
{
    bool parseDataStruct(const std::string& input, DataStruct& data) {
        bool flag1 = false;
        bool flag2 = false;
        bool flag3 = false;

        if (input.empty())
            return false;

        std::string tmp;
        auto it = input.begin();

        while (it != input.end()) {
            if (*it == '(' || *it == ')' || *it == ':') {
                ++it;
                continue;
            }


            tmp.clear();
            while (it != input.end() && *it != '(' && *it != ')' && *it != ':' && *it != '\'' && *it != '"' && *it != ' ') {
                tmp += *it++;
            }
            // std::cout << tmp << std::endl;


            if (tmp == "key1") {
                if (flag1) return false;
                it += 1;
                tmp.clear();
                while (it != input.end() && *it != ':') {
                    tmp += *it++;
                }
                try {
                    data.key1 = std::stod(tmp);
                } catch (...) {
                    return false;
                }
                flag1 = true;
            } else if (tmp == "key2") {
                if (flag2) return false;
                if (*(++it) != '\'') return false;
                it++;

                tmp.clear();
                while (it != input.end() && *it != '\'') {
                    tmp += *it++;
                }
                if (tmp.size() != 1) return false;
                data.key2 = static_cast<char>(tmp[0]);
                flag2 = true;
                ++it;
            } else if (tmp == "key3") {
                if (flag3) return false;
                if (*(++it) != '"') return false;
                it++;
                tmp.clear();

                while (it != input.end() && *it != '"') {
                    tmp += *it++;
                }

                data.key3 = tmp;
                flag3 = true;
                ++it;
            } else break;
        }

        return (flag1 && flag2 && flag3);
    }


    bool compareDataStruct(const DataStruct& a, const DataStruct& b) {
        if (a.key1 != b.key1) {
            return a.key1 < b.key1;
        }
        if (a.key2 != b.key2) {
            return a.key2 < b.key2;
        }
        return a.key3.length() < b.key3.length();
    }


    std::ostream & operator<<(std::ostream& os, const DataStruct& data) {
        std::ostream::sentry sentry(os);
        if (sentry) {
            StreamGuard guard(os);
            os << "(:key1 " << std::scientific << std::setprecision(10) << data.key1 << ":"
                << "key2 '" << data.key2 << "':"
                << "key3 \"" << data.key3 << "\":)";
        }
        return os;
    }

    std::istream & operator>>(std::istream& is, DataStruct& data) {
        std::string input;
        char ch;

    //    std::copy(std::istream_iterator<char>(is), std::istream_iterator<char>(),
    //      std::back_inserter(input));
    //    It drove me crazy
    //    for (auto it = start; it != end || *it != '\n'; it++)
    //    {
    //        std::cout << "FUCK: " << *it << std::endl;
    //    }

        // std::getline(is, input); // I cannot understand how not to use getline here
        // i understtod. This segment is literally the getline guts
        while (is.get(ch))
        {
            if (ch == '\n')
                break;
            input += ch;
        }

        if (is.eof())
            return is;

        if (!parseDataStruct(input, data)) {
            std::cout << "Looks like there is no supported record. Cannot determine input. Test skipped" << "\n";
            is.setstate(std::ios::failbit);
        }

        return is;
    }
}
