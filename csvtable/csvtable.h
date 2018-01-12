#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include <map>
#include <string>
#include <cassert>
#include <sstream>
#include <iomanip>

#include <boost/algorithm/string.hpp>
#include <boost/bind.hpp>
#include <boost/any.hpp>
#include <boost/crc.hpp>
#include <boost/lambda/lambda.hpp>


inline uint16_t byte2uint16(std::string s) {

    return *(const uint16_t*)s.c_str();
}


inline uint16_t crc16(const char* data, const int len) {

    boost::crc_16_type crc;
    crc.process_bytes(data, len);
    uint16_t chksum = crc.checksum();
    return chksum;
}


inline uint16_t crc16(std::string s) {

    uint16_t chksum = crc16(s.c_str(), s.size());
    return chksum;
}


inline std::string tohex(const std::string s) {

    using namespace boost::lambda;

    std::stringstream ss;
    ss.fill('0');

#if 0
    struct T {

        static void op(std::stringstream* ss, unsigned char c) {

            (*ss).width(2);
            (*ss).fill('0');
            (*ss) << std::hex << (unsigned int)c << " ";
        }
    };

    std::for_each(s.begin(), s.end(), boost::bind(&T::op, &ss, _1));
#endif


    unsigned char x;
    unsigned int  y;
    auto fun = ( var(x) = boost::lambda::_1,
                 var(y) = var(x),
                 ss << constant(std::setw(2))
                 << std::hex << var(y) );

    std::for_each(s.begin(), s.end(), fun);

    return ss.str();
}


/* ===========================================
 *
 * if file does not exist or file is empty,
 * return empty string
 *
 * =========================================== */
std::string file_get_contents(const std::string filename) {

    FILE* fp;
    int c;
    std::string contents;

    fp = fopen(filename.c_str(), "rb");

    if (fp == NULL) return "";

    while ( (c = fgetc(fp)) != EOF ) {

        contents.push_back((char)c);
    }

    fclose(fp);

    return contents;
}


int file_put_contents( const std::string filename,
                       const std::string contents) {

    FILE* fp;

    fp = fopen(filename.c_str(), "wb");

    if (fp == NULL) return 0;

    ssize_t sz = fwrite(contents.c_str(), 1, contents.size(), fp);
    fclose(fp);
    return sz;
}


std::vector<std::string> file(const std::string filename) {

    using namespace boost;
    using namespace std;

    string contents = file_get_contents(filename);
    vector<string> lines;
    split(lines, contents, is_any_of("\r\n"), token_compress_on);
    return lines;
}

enum {

    csv_without_header,
    csv_with_header,
};

template<typename E>
void ReadCsv( const std::string filename,
              std::vector<E>& table,
              const int header = csv_without_header) {

    using namespace std;

    struct T {

        static void op(vector<E>* tbl, string& s) {

            boost::trim(s);
            vector<string> vec;

            if (!s.empty()) {

                boost::split(vec, s, boost::is_any_of(","));
                tbl->push_back(E(vec));
            }
        }
    };

    vector<string> lines = file(filename);
    int offset = (header == csv_without_header) ? 0 : 1;
    for_each( lines.begin()+offset, lines.end(),
              boost::bind(&T::op, &table, _1));
}


template<typename E>
std::vector<E> ReadCsv( const std::string filename,
                        int header = csv_without_header) {

    using namespace std;

    vector<E> tbl;
    ReadCsv(filename, tbl, header);
    return tbl;
}


template<typename E>
int WriteCsv(const std::string filename, const std::vector<E>& tbl) {

    using namespace std;

    struct T {

        static void op(string* s1, const E& e) {

            string s2 = e.toString();

            if (s1->empty()) {

                s1[0] = s2;

            } else {

                s1[0] += "\n";
                s1[0] += s2;
            }
        }
    };

    string contents;
    for_each(tbl.begin(), tbl.end(), boost::bind(&T::op, &contents, _1));

    int wsz = file_put_contents(filename, contents);

    if (wsz != (int)contents.size()) {

        throw std::exception();
    }

    return wsz;
}


template<typename P>
bool ReadConfig(const std::string filename, P& param) {

    using namespace std;

    struct T {

        static bool empty(string& s) {

            boost::trim(s);
            return s.empty();
        }

        static bool comment(string& s) {

            boost::trim(s);
            return (s[0] == '#');
        }

        static void op(const string& s, P* param) {

            string::size_type pos = s.find('=');
            if (pos != string::npos) {

                string name  = s.substr(0, pos);
                string value = s.substr(pos+1);
                boost::trim(name);
                boost::trim(value);
                param->fromString(name, value);
            }
        }
    };

    vector<string> conf = file(filename);

    vector<string>::iterator n;

    n = remove_if(conf.begin(), conf.end(), T::empty);
    conf.erase(n, conf.end());

    n = remove_if(conf.begin(), conf.end(), T::comment);
    conf.erase(n, conf.end());

    for_each( conf.begin(), conf.end(),
              boost::bind(&T::op, _1, &param)); // question

    return param.check();
}




template<typename P>
P ReadConfig(const std::string filename) {

    P param;

    if ( ReadConfig(filename, param) ) {

        return param;

    } else {

        throw std::exception();
    }
}


#ifdef DOTEST
using namespace std;

struct table_entry {


    int id;
    double price;

    table_entry(vector<string> s) {

        id    = stoi(s[0]);
        price = stod(s[1]);
    }

    string toString() const {

        stringstream ss;
        ss << id << "," << price ;
        return ss.str();
    }

};


struct Parameters {

    double speed;
    double mpg;
    int    num;
    string name;

    map<string, boost::any> nv;

    Parameters() {

        nv["speed"] =  &speed;
        nv["mpg"]   =  &mpg;
        nv["num"]   =  &num;
        nv["name"]  =  &name;

    }

    void fromString(std::string& key, std::string& value) {

#define METHOD_ONE

#ifdef METHOD_ONE
        if (key == "speed") speed = std::stod(value);
        if (key == "mpg")   mpg   = std::stod(value);
        if (key == "num")   num   = std::stoi(value);
        if (key == "name")  name  = value;
#else
        nv[key].first(nv[key].second, value);
#endif

    }

    string toString() {

        stringstream ss;

        ss << "speed = " << speed  << endl;
        ss << "mpg = "   << mpg    << endl;
        ss << "num = "   << num    << endl;
        ss << "name = "  << name   << endl;

        return ss.str();
    }

    bool check() {

        return true;
    };
};


int main(void) {

    using namespace boost::lambda;

#if 1
    vector<table_entry> table = ReadCsv<table_entry>("table.csv");
    WriteCsv("t.csv", table);

    Parameters param;
    param = ReadConfig<Parameters>("param.conf");
    cout << param.toString() << std::endl;

    string s = file_get_contents("a.txt");
    file_put_contents("b.txt", "hello world");
#endif


    string ss(256,0);
    char i = 0;
    for_each(ss.begin(), ss.end(), boost::lambda::_1 = var(i)++);
    cout << tohex(ss) << endl;

    return 0;
}

#endif


