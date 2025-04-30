#include "data_struct.hpp"
#include "stream_guard.hpp"

#include <iostream>
#include <iomanip>
#include <cstring>
#include <cctype>

std::ostream& trojan::output::operator<<(std::ostream& stream, const DoubleLiteral& value)
{
    std::ostream::sentry sentry(stream);
    if (!sentry) {
        return stream;
    }
    StreamGuard guard(stream);
    return stream << std::fixed << value.value << 'd';
}
std::ostream& trojan::output::operator<<(std::ostream& stream, const UllLiteral& value)
{
    std::ostream::sentry sentry(stream);
    if (!sentry) {
        return stream;
    }
    return stream << value.value << "ull";
}
std::ostream& trojan::output::operator<<(std::ostream& stream, const StringLiteral& value)
{
    std::ostream::sentry sentry(stream);
    if (!sentry) {
        return stream;
    }
    return stream << '"' << value.value << '"';
}
std::ostream& trojan::operator<<(std::ostream& stream,const DataStruct& value)
{
    std::ostream::sentry sentry(stream);
    if (!sentry) {
        return stream;
    }
    stream << "(:key1 " << output::DoubleLiteral{value.key1};
    stream << ":key2 " << output::UllLiteral{value.key2};
    return stream << ":key3 " << output::StringLiteral{value.key3} << ":)";
}
std::istream& trojan::input::operator>>(std::istream& stream, Delimiter&& value)
{
    std::istream::sentry sentry(stream);
    if (!sentry) {
        return stream;
    }
    char c = '\0';
    c = stream.get();
    if (c != value.expected) {
        stream.setstate(std::ios::failbit);
    }
    return stream;
}
std::istream& trojan::input::operator>>(std::istream& stream, DoubleLiteral&& value)
{
    std::istream::sentry sentry(stream);
    if (!sentry) {
        return stream;
    }
    StreamGuard guard(stream);
    double temp = 0.0;
    char c = '\0';
    if (!(stream >> temp >> std::noskipws >> c) && ((c != 'd') && (c != 'D'))) {
        stream.setstate(std::ios::failbit);
        return stream;
    }
    value.link = temp;
    return stream;
}
std::istream& trojan::input::operator>>(std::istream& stream, UllLiteral&& value)
{
    std::istream::sentry sentry(stream);
    if (!sentry) {
        return stream;
    }
    StreamGuard guard(stream);
    unsigned long long temp = 1;
    char ullTag[4] = "\0";
    if (std::isdigit(stream.peek()) && (stream >> temp) && (stream >> std::setw(4) >> ullTag)
            && (std::strcmp(ullTag, "ull") || std::strcmp(ullTag, "ULL")))
    {
        value.link = temp;
        return stream;
    }
    stream.setstate(std::ios::failbit);
    return stream;
}
std::istream& trojan::input::operator>>(std::istream& stream, StringLiteral&& value)
{
    std::istream::sentry sentry(stream);
    if (!sentry) {
        return stream;
    }
    std::string temp;
    if (std::getline(stream >> Delimiter{ '"' }, temp, '"')) {
        value.link = temp;
    }
    return stream;
}
std::istream& trojan::operator>>(std::istream& stream, DataStruct& value)
{
    std::istream::sentry sentry(stream);
    if (!sentry) {
        return stream;
    }
    StreamGuard guard(stream);
    char keyTag[5] = "\0";
    bool received[3] = {false, false, false};
    char key = 0;
    DataStruct temp;
    bool isGood = true;
    stream >> input::Delimiter{'('};
    while ((stream >> std::setw(5) >> keyTag) && (std::strcmp(keyTag, ":key") == 0) && isGood) {
        key = stream.get();
        if (stream && (key >= '1') && (key <= '3') && !received[key - '1']) {
            if (key == '1') {
                stream >> input::DoubleLiteral{temp.key1};
            }
            else if (key == '2') {
                stream >> input::UllLiteral{temp.key2};
            }
            else if (key == '3') {
                stream >> input::StringLiteral{temp.key3};
            }
            received[key - '1'] = true;
            if (received[0] && received[1] && received[2]) {
                if (stream >> input::Delimiter{':'} >> input::Delimiter{')'}) {
                    value = temp;
                }
                return stream;
            }
        }
        else {
            stream.setstate(std::ios::failbit);
            return stream;
        }
        isGood = received[key - '1'];
    }
    stream.setstate(std::ios::failbit);
    return stream;
}
