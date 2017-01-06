//
// InputSystem.hpp for ECS in /home/mikaz3/Tek3/B5/AdvancedCpp/cpp_rtype/ECS
// 
// Made by Thomas Billot
// Login   <mikaz3@epitech.net>
// 
// Started on  Tue Dec 27 16:15:19 2016 Thomas Billot
// Last update Tue Dec 27 17:29:02 2016 Thomas Billot
//

#pragma once

#include        <map>
#include	<SFML/Window.hpp>
#include	"IGui.hpp"
#include        "ASystem.hpp"

class			InputSystem : public ASystem
{
public:
  
  enum Action
    {
      MoveLeft,
      MoveRight,
      MoveUp,
      MoveDown,
      Fire
    };

  explicit InputSystem();
  virtual ~InputSystem();

  void			setGUI(IGui &gui);
  void			assignKey(Action action,
			  sf::Keyboard::Key key);
  sf::Keyboard::Key     getAssignedKey(Action action);
  void			CommandUp(void);
  virtual void			updateEntity(int entityId);
  

  InputSystem(const InputSystem &) = delete;
  InputSystem		&operator=(const InputSystem &) = delete;
  
private:

  typedef void (*Command)(void);
  
  std::map<sf::Keyboard::Key, Action>	_keyBindings;
  std::map<Action, Command>		_actionBindings;
  std::shared_ptr<IGui>		        _gui;

};
