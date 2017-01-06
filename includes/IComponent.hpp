//
// IComponent.hpp for ECS in /home/mikaz3/Tek3/B5/AdvancedCpp/ECS
// 
// Made by Thomas Billot
// Login   <mikaz3@epitech.net>
// 
// Started on  Fri Nov 25 16:03:44 2016 Thomas Billot
// Last update Mon Dec 19 15:07:52 2016 Thomas Billot
//

#ifndef			_ICOMPONENT_HPP_
# define		_ICOMPONENT_HPP_

#include <string>

class				IComponent
{

public :
  
  virtual			~IComponent() {}
  virtual const std::string     getName() = 0;
};


#endif			/* _ICOMPONENT_HPP_ */
