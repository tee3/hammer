#include <boost/date_time/gregorian/gregorian.hpp>
#include <iostream>

int
main()
{
  std::cout << boost::gregorian::day_clock::local_day() << std::endl;

  return 1;
}
