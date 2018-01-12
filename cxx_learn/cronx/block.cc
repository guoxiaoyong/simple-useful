#include <unistd.h>
#include <fcntl.h>
#include <string>
#include <cstdlib>
#include <cstdio>

using namespace std;

void closeall() {

    for (int n = 0; n < 1024; n++) {

        close(n);
    }
};

string file_get_contents(string filename) {

    FILE* fp = fopen(filename.c_str(), "rb");
    if (fp == NULL) return "";

    string tmp;
    int c;
    while ( (c = fgetc(fp)) != EOF ) tmp.push_back((char)c);

    fclose(fp);
    return tmp;
}

void file_put_contents(string filename, string contents) {


    FILE* fp = fopen(filename.c_str(), "wb");
    fwrite(contents.c_str(), contents.size(), 1, fp);
    fclose(fp);
}


int main(int argc, char* argv[]) {

    closeall();
    daemon(0, 1);

    string script_file("/tmp/block/config");
    string input("/tmp/block/n0");
    string output("/tmp/block/n1");
    string errput("/tmp/block/n2");
    string cmd("/usr/bin/bash");
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

