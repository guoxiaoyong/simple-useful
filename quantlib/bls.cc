#include <ql/quantlib.hpp>
//#include <ql/userconfig.hpp>
#include <iostream>
#include <vector>
#include <boost/foreach.hpp>

using namespace std;
using namespace QuantLib;

void testingBlackScholesCalculator() {

    Real S0  = 100.0;
    Real K   = 105.0;
    Real rd  = 0.034;
    Real rf  = 0.021;
    Real tau = 0.5;
    Real vol = 0.177;

    Real domDisc = std::exp(-rd*tau);
    Real forDisc = std::exp(-rf*tau);
    Real stdDev = vol*std::sqrt(tau);

    boost::shared_ptr<PlainVanillaPayoff> vanillaPayoffPut(new PlainVanillaPayoff(Option::Put, K));
    boost::shared_ptr<AssetOrNothingPayoff> aonPayoffCall(new AssetOrNothingPayoff(Option::Call, K));

    BlackScholesCalculator vanillaPutPricer(vanillaPayoffPut, S0, forDisc, stdDev, domDisc);
    BlackScholesCalculator aonCallPricer(aonPayoffCall, S0, forDisc, stdDev, domDisc);

    cout << "------------- Vanilla VAlues -----------------" << endl;
    cout << "Value: " << vanillaPutPricer.value() << endl;


}



int main(void) {

    testingBlackScholesCalculator();
    return 0;
}

