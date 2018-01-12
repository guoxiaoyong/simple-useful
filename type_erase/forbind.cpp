#include <algorithm>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <iostream>
#include <vector>

struct status {
  std::string name_;
  bool ok_;

  status(const std::string &name) : name_(name), ok_(true) {}

  void break_it() { ok_ = false; }

  bool is_broken() const { return ok_; }

  void report() const {
    std::cout << name_ << " is "
              << (ok_ ? "working nominally" : "terribly broken") << '\n';
  }
};

int main(void) {
  std::vector<status> statuses;
  statuses.push_back(status("status 1"));
  statuses.push_back(status("status 2"));
  statuses.push_back(status("status 3"));
  statuses.push_back(status("status 4"));

  statuses[1].break_it();
  statuses[2].break_it();

  for (std::vector<status>::iterator it = statuses.begin();
       it != statuses.end(); ++it) {
    it->report();
  }

  std::cout << std::endl;
  std::for_each(statuses.begin(), statuses.end(), std::mem_fn(&status::report));
  std::for_each(statuses.begin(), statuses.end(),
                boost::bind(&status::report, _1));
  std::vector<boost::shared_ptr<status>> s_statuses;

  s_statuses.push_back(boost::shared_ptr<status>(new status("status 1")));
  s_statuses.push_back(boost::shared_ptr<status>(new status("status 2")));
  s_statuses.push_back(boost::shared_ptr<status>(new status("status 3")));
  s_statuses.push_back(boost::shared_ptr<status>(new status("status 4")));

  std::for_each(s_statuses.begin(), s_statuses.end(),
                boost::bind(&status::report, _1));
  std::for_each(s_statuses.begin(), s_statuses.end(),
                std::mem_fn(&status::report));
  std::vector<status *> statuses2;

  statuses2.push_back(new status("status 1"));
  statuses2.push_back(new status("status 2"));
  statuses2.push_back(new status("status 3"));
  statuses2.push_back(new status("status 4"));

  std::for_each(statuses2.begin(), statuses2.end(),
                std::mem_fn(&status::report));
  std::for_each(statuses2.begin(), statuses2.end(),
                std::mem_fun(&status::report));

  return 0;
}
