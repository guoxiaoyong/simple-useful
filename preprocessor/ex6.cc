#define UL unsigned long
#define znew  ((z=36969*(z&65535)+(z>>16))<<16)
#define wnew  ((w=18000*(w&65535)+(w>>16))&65535)
#define MWC   (znew+wnew)
#define SHR3  (jsr=(jsr=(jsr=jsr^(jsr<<17))^(jsr>>13))^(jsr<<5))
#define CONG  (jcong=69069*jcong+1234567)
#define KISS  ((MWC^CONG)+SHR3)

/*  Global static variables:
    (the seed changes on every minute) */

static UL z=362436069*(int)__TIMESTAMP__, w=521288629*(int)__TIMESTAMP__, \
   jsr=123456789*(int)__TIMESTAMP__, jcong=380116160*(int)__TIMESTAMP__;

#include <iostream>

using namespace std;

int main(int argc, char* argv[]){
    cout<<KISS<<endl;
    cout<<KISS<<endl;
    cout<<KISS<<endl;
    return 0;
}
