//
// main.cpp for  in /home/frasse_l/Projets/autocompletion
// 
// Made by loic frasse-mathon
// Login   <frasse_l@epitech.net>
// 
// Started on  Thu Jun 30 09:16:46 2016 loic frasse-mathon
// Last update Thu Jun 30 09:54:58 2016 loic frasse-mathon
//

#include "autocompletion.hh"

static void	usage()
{
  std::cout << "USAGE" << std::endl;
  std::cout << "\t./autocompletion dictionnary" << std::endl << std::endl;
  std::cout << "DESCRIPTION" << std::endl;
  std::cout << "\tdictionnary file, containing one address per line, serving as knowledge base" << std::endl;
  exit(0);
}

static void	bad_arg()
{
  std::cerr << "Invalid argument" << std::endl;
  exit(84);
}

static void	add_dictionary(ac::AutoCompletion &autoCompletion, char *path)
{
  (void)autoCompletion;
  (void)path;
  /* TODO */
}

int			main(int ac, char **av)
{
  ac::AutoCompletion	autoCompletion;
  int			i;

  i = 1;
  if (ac == 1)
    bad_arg();
  while (i < ac)
    {
      if (std::string("-h") == av[i])
	usage();
      if (i != 1)
	bad_arg();
      add_dictionary(autoCompletion, av[i]);
      i++;
    }
  return (0);
}
