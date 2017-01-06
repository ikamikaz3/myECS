//
// EntityManager.cpp for ECS in /home/mikaz3/Tek3/B5/AdvancedCpp/ECS
// 
// Made by Thomas Billot
// Login   <mikaz3@epitech.net>
// 
// Started on  Fri Nov 25 17:06:52 2016 Thomas Billot
// Last update Wed Dec 28 21:53:54 2016 Thomas Billot
//

#include <exception>
#include <iostream>
#include <type_traits>
#include <stdexcept>
#include "EntityManager.hpp"

EntityManager::EntityManager(MessageBus &messageBus)
  : _messageBus(messageBus),
    _entities(),
    _componentMasks(),
    _components()
{
  for (unsigned int id = 0; id < EntityManager::_maxEntities; id++)
    _entities[id] = eState::NONE;
}

EntityManager::~EntityManager()
{
  std::cout << "really destroying entity Manager!!!!!!!!" << std::endl;
}

void		EntityManager::addEntityType(const std::string &typeName,
					     const std::vector<std::string> &components,
					     EntityInitialiser entityInitialiser)
{
  int		mask = 0;
  
  for (auto it : components)
    {
      mask |= getComponentMask(it);
    }
  std::cout << "new entity type mask: " << mask << std::endl;
  _entityTypes[typeName] = mask;
  _entityInitialisers[typeName] = entityInitialiser;
}

int				EntityManager::createEntity(const std::string &typeName,
							    ...)
{
  int typeMask = 0;

  try {
    typeMask = _entityTypes.at(typeName);
  } catch (std::exception &e) {
    
    std::cout << "exception: " << e.what() << std::endl;
  }

  if (typeMask == -1)
    return (-1);
  for (unsigned int id = 0; id < EntityManager::_maxEntities; id++)
    {
      if (_entities[id] == eState::NONE)
	{
	  std::cout << "[new entity] id = " << id << std::endl;
	  _entities[id] = eState::USED;
	  _typeOfEntities[id] = typeMask;
	  _messageBus.post(MessageBus::ENTITY_CREATED, new int(id));
	  va_list args;
	  va_start(args, typeName);
	  _entityInitialisers.at(typeName)(*this, id, args);
	  va_end(args);
	  return id;
	}
    }
  return (NONE);
}

void				EntityManager::deleteEntity(int id)
{
  _entities[id] = eState::NONE;
  _messageBus.post(MessageBus::ENTITY_DESTROYED, new int(id));
}

int				EntityManager::getEntityType(int entityId)
{
  return _typeOfEntities[entityId];
}

int				EntityManager::createComponentMask(const std::string &name)
{
  static int i = 0;
  int	newComponentMask = 1 << i++;
  std::cout << "new Component Mask: " << newComponentMask << std::endl;
  _componentMasks.emplace(name, newComponentMask);
  return 1 << i;
}

int				EntityManager::getComponentMask(const std::string &name)
{
  auto it = _componentMasks.find(name);

  if (it != _componentMasks.end())
    return it->second;
  return -1;
}

IComponent			*EntityManager::getComponent(int id,
							    const std::string &componentType)
{
  return (_components[componentType]->at(id)).get();
}

std::shared_ptr<EntityManager::component_pool> EntityManager::getArrayByName(const std::string &name)
{
  std::map<std::string, std::shared_ptr<component_pool >>::const_iterator it = _components.find(name);

  if (it != _components.end())
    {
      return it->second;
    }
  return NULL;
}

//const std::array<int, EntityManager::_maxEntities>	&EntityManager::getEntities() const
//{
//}

std::shared_ptr<std::vector<IComponent*>> EntityManager::getComponentsById(int entity) const
{
  // std::shared_ptr<std::vector<IComponent*> > pV(new std::vector<IComponent*>);
  
  // for (unsigned int id = 0; id < EntityManager::_maxEntities; id++)
  //   {
  //     if (_entities[id] == eSystem::USED)
  // 	{
	  
  // 	  std::pair<std::multimap<int, std::unique_ptr<IComponent>>::const_iterator,
  // 		    std::multimap<int, std::unique_ptr<IComponent>>::const_iterator> ret;
  // 	  ret = _map.equal_range(id);
  // 	  for (std::multimap<int, std::unique_ptr<IComponent> >::const_iterator it = ret.first;
  // 	       it != ret.second;
  // 	       ++it)
  // 	    {
  // 	      pV->push_back(&(*it->second));
  // 	    }
  // 	}
  //   }
  // return pV;
  (void)entity;
  return NULL;
}
