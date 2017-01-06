#include        <iostream>
#include        "GameEngine.hpp"

int			main(int argc, char **argv)
{
  try
    {
      if (argc != 2)
	throw std::runtime_error("Usage: ./ecs path_to_libs");

      std::string		path(argv[1]);
      GameEngine		g(path);
      g.run();
    }
  catch (const std::exception &e)
    {
      std::cout << e.what() << std::endl;
    }
  return 0;
}
