#include <cstdio>
#include <glog/logging.h>


int main(int argc, char *argv[]) {
  int orig_flag = FLAGS_log_prefix;
  // Initialize Google's logging library.
  google::InitGoogleLogging(argv[0]);
  FLAGS_logtostderr = 1;
  for (int n = 0; n < argc; n++) {
    if (n == 2) {
      FLAGS_log_prefix = 0;
      LOG(INFO) << "Found " << argv[n] << " cookies";
      FLAGS_log_prefix = 1;
    } else {
      LOG(INFO) << "Found " << argv[n] << " cookies";
    }
  }
  return 0;
}
