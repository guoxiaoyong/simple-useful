#include <ql/quantlib.hpp>
//#include <ql/userconfig.hpp>
#include <iostream>
#include <vector>
#include <boost/foreach.hpp>

using namespace std;
using namespace QuantLib;

int main(void) {

    Calendar mycal = China();

        vector<Date> hl = mycal.holidayList(mycal, Date(11,Mar,2016), Date(20,Apr,2016), true);

        BOOST_FOREACH(Date& d, hl) {

            cout << d << endl;
        }
/*

    int n;
    for (n = Jan; n <= Dec; n++) {

        Date d(15, (Month)n, 2016);
        //cout << d << endl;
        cout << mycal.adjust(d, Preceding) << endl;
    }
*/

    return 0;
}

