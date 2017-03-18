#include <iostream>
#include <libs/shared_lib.h>
#include <libs/static_lib.h>

int
main()
{
  std::cout << from_static_lib() << std::endl;
  std::cout << from_shared_lib() << std::endl;

  return 0;
}