//
// main.cpp for  in /home/frasse_l/Projets/autocompletion
// 
// Made by loic frasse-mathon
// Login   <frasse_l@epitech.net>
// 
// Started on  Thu Jun 30 09:16:46 2016 loic frasse-mathon
// Last update Thu Jun 30 13:45:23 2016 loic frasse-mathon
//

#include "autocompletion.hh"

static bool	isNumber(const std::string &string)
{
  size_t	i;

  i = 0;
  while (i < string.length())
    {
      if (string[i] < '0' || string[i] > '9')
	return false;
      i++;
    }
  return true;
}

static bool	isStreetType(const std::string &arg)
{
  std::string	string = arg;
  std::transform(string.begin(), string.end(), string.begin(), ::tolower);
  return string == "allée" || string == "avenue" || string == "boulevard" || string == "chemin"
    || string == "impasse" || string == "place" || string == "quai" || string == "rue" || string == "square";
}

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
  std::ifstream	stream;
  std::string	line;

  stream.open(path);
  if (!stream)
    bad_arg();

  while (std::getline(stream, line))
    {
      bool		before = true;
      bool		ok = true;
      size_t		i = 0;
      std::string	name;
      std::string       address;
      while (i < line.length() && ok)
	{
	  if (line[i] == ',' && before)
	    before = false;
	  else if (line[i] == ',')
	    ok = false;
	  else if (before)
	    name.push_back(line[i]);
	  else
	    address.push_back(line[i]);
	  i++;
	}
      std::vector<std::string>	split;
      std::istringstream	iss(address);
      std::string		tmp;
      while (std::getline(iss, tmp, ' '))
	split.push_back(tmp);
      if (!ok || name.empty() || address.empty() || address[0] != ' ' || split.size() < 4 || !isNumber(split[1])
	  || !isStreetType(split[2]))
	std::cerr << line << std::endl;
      else
	autoCompletion.addAddress(name, address.substr(1, address.length() - 1));
    }
}

static void	remove(std::vector<ac::City *> &choices, const std::string &total)
{
  size_t	i;
  size_t	j = 0;
  if (choices.size() > 1)
    {
      while (j < choices.size())
	{
	  std::string tmp = choices[j]->getName();
	  std::transform(tmp.begin(), tmp.end(), tmp.begin(), ::tolower);
	  bool			ok = false;
	  std::istringstream	iss(tmp);
	  std::string		line;
	  while (!ok && std::getline(iss, line, ' '))
	    {
	      i = 0;
	      while (i < total.length() && total[i] == line[i])
		i++;
	      if (i == total.length())
		ok = true;
	    }
	  if (!ok)
	    {
	      std::vector<ac::City *>::iterator it = choices.begin();
	      std::vector<ac::City *>::iterator it_end = choices.end();
	      size_t	k = 0;
	      while (k < j)
		{
		  it++;
		  k++;
		}
	      choices.erase(it);
	      j--;
	    }
	  j++;
	}
    }
  if (choices.size() == 1)
    {
      /* TODO Check addresses */
    }
  else if (choices.empty())
    {
      std::cerr << "Unknown address" << std::endl;
      exit(84);
    }
  i++;
}

static void	run(ac::AutoCompletion &autoCompletion)
{
  std::vector<ac::City *>	choices = autoCompletion.getCities();
  std::string			total;
  std::string			line;
  while (true)
    {
      /* TODO : Print possible values */
      if (!std::getline(std::cin, line))
	return ;
      std::transform(line.begin(), line.end(), line.begin(), ::tolower);
      if (line == "abort")
	exit(0);
      total.append(line);
      remove(choices, total);
    }
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
  run(autoCompletion);
  return (0);
}
