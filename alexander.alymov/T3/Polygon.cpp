#include "Polygon.h"

namespace alymov
{

    std::istream& operator>>(std::istream& in, DelimiterIO&& dest)
    {
        std::istream::sentry sentry(in);
        if (!sentry)
        {
            return in;
        }
        char c = '0';
        in >> c;
        if (in && (c != dest.exp))
        {
            in.setstate(std::ios::failbit);
        }
        return in;
    }

    std::istream& operator>>(std::istream& in, Point& dest)
    {
        std::istream::sentry sentry(in);
        if (!sentry)
        {
            return in;
        }
        return in >> DelimiterIO{ '(' } >> dest.x >> DelimiterIO{ ';' }
        >> dest.y >> DelimiterIO{ ')' };
    }

    std::istream& operator>>(std::istream& in, Polygon& dest)
    {
        std::istream::sentry sentry(in);
        if (!sentry)
        {
            return in;
        }
        dest.points.clear();
        size_t size = 0;
        in >> size;
        if (in.peek() != ' ')
        {
            in.setstate(std::ios::failbit);
            return in;
        }
        std::string str;
        std::getline(in, str, '\n');
        if (str.empty() || str[0] != ' ')
        {
            in.setstate(std::ios::failbit);
            return in;
        }
        for (size_t i = 1; i < str.size(); ++i)
        {
            if (str[i] == '(' && str[i - 1] != ' ')
            {
                in.setstate(std::ios::failbit);
                return in;
            }
        }
        std::istringstream input(str);
        if (!input || size < 3)
        {
            in.setstate(std::ios::failbit);
        }

        std::vector < Point > temp{};
        std::copy(std::istream_iterator< Point >(input), std::istream_iterator< Point >(),
            std::back_inserter(temp));
        if (temp.size() == size && temp.size() >= 3)
        {
            dest.points = temp;
        }
        else
        {
            in.setstate(std::ios::failbit);
        }
        return in;
    }

}
