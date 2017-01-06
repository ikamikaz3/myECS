//
// ISystem.hpp for ECS in /home/mikaz3/Tek3/B5/AdvancedCpp/ECS
// 
// Made by Thomas Billot
// Login   <mikaz3@epitech.net>
// 
// Started on  Fri Nov 25 14:42:17 2016 Thomas Billot
// Last update Tue Dec 27 10:28:20 2016 Thomas Billot
//

#ifndef			_ASYSTEM_HPP_
# define		_ASYSTEM_HPP_

# include "EntityManager.hpp"
# include "MessageBus.hpp"

class MessageBus;
class EntityManager;

class					ASystem
{

protected:
  typedef void (ASystem::*message_handler)(void *messageData);
  
  int					_mask;
  std::string				_name;
  EntityManager				&_entityManager;
  MessageBus				&_messageBus;
  std::vector<int>			_affectedEntities;
  std::map<int, message_handler>	_messageHandlers;
  
public:

  ASystem(EntityManager &entityManager, MessageBus &messageBus);
  virtual		~ASystem() {}

  void			addName(const std::string &name);
  void			addAffectedComponents(const std::vector<std::string> &affectedComponents);
  void			loadMessageHandler(int messageTypeId,
					   message_handler handler);
  void			handleMessage(int messageTypeId,
				      void *messageData);
  void			handleEntityCreated(void *messageData);
  void			handleEntityDeleted(void *messageData);
  void			update();
  const std::string	&name() const;
  virtual void		preRoutine() = 0;
  virtual void		updateEntity(int entityId) = 0;
  virtual void		postRoutine() = 0;
};


#endif			/* _ASYSTEM_HPP_ */
