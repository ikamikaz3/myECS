//
// MessageBus.cpp for ECS in /home/mikaz3/Tek3/B5/AdvancedCpp/cpp_rtype/ECS
// 
// Made by Thomas Billot
// Login   <mikaz3@epitech.net>
// 
// Started on  Wed Dec 21 18:25:12 2016 Thomas Billot
// Last update Sat Dec 31 03:33:44 2016 Thomas Billot
//

#include	<iostream>
#include	"MessageBus.hpp"

MessageBus::Message::Message(int typeId, void *data)
  : _typeId(typeId),
    _data(data)
{}

MessageBus::Message::~Message()
{}

int		MessageBus::Message::getTypeId()
{
  return _typeId;
}

void		*MessageBus::Message::getData()
{
  return _data;
}

MessageBus::MessageBus()
  : _validMessages{ENTITY_CREATED, ENTITY_DESTROYED, ENTITY_COMPOSITION_CHANGED},
    _subscribedList(),
    _messages(),
    _systems()
{}

MessageBus::~MessageBus()
{}

void		MessageBus::registerSystem(std::shared_ptr<ASystem> systemToManage)
{
  _systems.push_back(systemToManage);
}

void		MessageBus::registerValidMessageId(int messageId)
{
  _validMessages.push_back(messageId);
}

void		MessageBus::subscribeToMessage(const std::string &system,
					       const int message)
{
  bool		isValid = false;
  bool		isSystemValid = false;
  
  for (auto itM : _validMessages)
    if (message == itM)
      isValid = true;
  
  for (std::vector<std::shared_ptr<ASystem> >::const_iterator itS = _systems.begin();
       itS != _systems.end();
       ++itS)
    {
      std::shared_ptr<ASystem> pS = *itS;
      if (pS->name() == system)
	{
	  isSystemValid = true;
	  _subscribedList.emplace(message, system);
	}
    }
  if (!isSystemValid || !isValid)
    throw std::runtime_error("Error: MessageBus::subscribeToMessage: couldn't add " + system);
}

void		MessageBus::post(const int messageTypeId,
				 void *messageData)
{
  _messages.push_back(Message(messageTypeId, messageData));
}

bool		MessageBus::isSubscribed(const std::string &name,
					 const int messageTypeId)
{
  for (std::vector<int>::const_iterator itM = _validMessages.begin();
       itM != _validMessages.end();
       ++itM)
    {
      if (*itM == messageTypeId)
	{
	  std::pair<std::multimap<int, std::string>::iterator,
		    std::multimap<int, std::string>::iterator> ret;
	  ret = _subscribedList.equal_range(*itM);
	  for (std::multimap<int, std::string>::iterator itR = ret.first;
	       itR != ret.second;
	       ++itR)
	    {
	      if (itR->second == name)
		return true;
	    }
	}
    }
  return false;
}

void		MessageBus::broadcast()
{
  for (auto message : _messages)
    {
      for (auto it = _systems.begin(); it != _systems.end(); ++it)
	{
	  if (isSubscribed((*it)->name(), message.getTypeId()))
	    (*it)->handleMessage(message.getTypeId(), message.getData());
	}
    }
  _messages.clear();
}

void		MessageBus::removeSystem(const std::string &name)
{
  for (std::vector<std::shared_ptr<ASystem> >::const_iterator it = _systems.begin();
       it != _systems.end();
       ++it)
    {
      std::shared_ptr<ASystem> pS = *it;
      if (pS->name() == name)
	_systems.erase(it);
    }
}
