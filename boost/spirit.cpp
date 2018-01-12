#include <iostream>
#include <iterator>
#include <string>
#include <vector>

#include <boost/spirit/core.hpp>
#include <boost/spirit/iterator/file_iterator.hpp>

using namespace std;
using namespace boost::spirit;

template <typename IteratorT> struct LineInfo {
  IteratorT lineIter;
  size_t lineNum;

  LineInfo(IteratorT beg) : lineIter(beg), lineNum(1) {}
};

template <typename IteratorT> struct NewLine {
  LineInfo<IteratorT> &lineInfo;
  NewLine(LineInfo<IteratorT> &info) : lineInfo(info) {}

  void operator()(IteratorT first, IteratorT last) const {
    lineInfo.lineIter = last;
    lineInfo.lineNum++;
  }
};

struct AddVal {

  vector<string> &values;
  AddVal(vector<string> &vec) : values(vec) {}

  template <class IteratorT>
  void operator()(IteratorT first, IteratorT last) const {
#ifdef _MSC_VER
    string s;
    s.resize(distance(first, last));
    for (size_t i = 0; first != last; ++i) {
      s[i] = *first++;
    }
#else
    string s(first, last);
#endif
    string::size_type pos = 0;
    while (1) {
      pos = s.find("\"\"", pos);
      if (pos == string::npos)
        break;
      ++pos;
      s.erase(pos, 1);
    }
    values.push_back(s);
  }
};

template <class IteratorT>
struct CSVParser : public grammar<CSVParser<IteratorT>> {
  vector<string> &v;
  LineInfo<IteratorT> &lineInfo;

  CSVParser(vector<string> &vec, LineInfo<IteratorT> &info)
      : v(vec), lineInfo(info) {}

  template <typename ScannerT> struct definition {
    rule<ScannerT> csv, val, quoted_val, naked_val;

    definition(const CSVParser<IteratorT> &self) {
      csv = val >> *(',' >> val) >>
            (eol_p[NewLine<IteratorT>(self.lineInfo)] | end_p);

      val = *blank_p >> ch_p('\"') >> quoted_val[AddVal(self.v)] >>
                ch_p('\"') >> *blank_p |
            naked_val[AddVal(self.v)];

      quoted_val = *(eol_p[NewLine<IteratorT>(self.lineInfo)] | ~ch_p('"') |
                     str_p("\"\""));

      naked_val = *(~ch_p(',') & ~ch_p('\"') & ~ch_p('\n'));
    }

    const rule<ScannerT> &start() const { return csv; }
  };
};

template <typename IteratorT>
parse_info<IteratorT> parse_csv(const IteratorT &first, const IteratorT &last,
                                vector<string> &vec,
                                LineInfo<IteratorT> &info) {
  CSVParser<IteratorT> csv(vec, info);

  return parse(first, last, csv);
}

int main(int argc, char **argv) {
  if (argc != 2)
    return 1;

  typedef file_iterator<char> iterator_t;

  iterator_t begin(argv[1]);
  if (!begin) {
    cout << "Unable to open file: " << argv[1] << endl;
    return -1;
  }

  iterator_t first = begin;

  iterator_t last = first.make_end();

  LineInfo<iterator_t> line_info(begin);
  while (first != last) {
    vector<string> v;
    parse_info<iterator_t> info = parse_csv(first, last, v, line_info);

    if (!info.hit) {
      cout << "Error!!  Line: " << line_info.lineNum
           << ", Column: " << distance(line_info.lineIter, info.stop) + 1
           << endl;
      break;
    }

    cout << "Parses OK:" << endl;
    for (vector<string>::size_type i = 0; i < v.size(); ++i)
      cout << i + 1 << ": " << v[i] << endl;

    cout << "-------------------------\n";

    first = info.stop;
  }

  return 0;
}
