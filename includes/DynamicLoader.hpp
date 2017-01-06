#ifndef		__DYNAMIC_LOADER_HPP__
# define	__DYNAMIC_LOADER_HPP__

#if defined(_MSC_VER)
# include <windows.h>
#elif defined(__GNUC__)
# include <dlfcn.h>
#else
#error define your copiler
#endif

# include	<map>
# include	<string>
# include	<iostream>

class		DynamicLoader
{
private:
  std::map<std::string , void *> _hashHandler;

public:
  DynamicLoader() {}

  ~DynamicLoader()
  {
#if defined(__GNUC__)
    for (auto it : _hashHandler)
      dlclose(it.second);
#elif defined(_MSC_VER_)
    for (auto it : _hashHandler)
      FreeLibrary((HINSTANCE)it.second);
#endif
    _hashHandler.erase(_hashHandler.begin(), _hashHandler.end());
  }

#if defined(__GNUC__)
  template<typename T>
  std::unique_ptr<T>		_unixLoad(const std::string &libName, const std::string &symName)
  {
    T				*(*fPtr)();
    std::string			newLibName = libName + ".so";
    void			*handler = dlopen(newLibName.c_str(), RTLD_LAZY);

    _hashHandler[symName] = handler;
    if (!(fPtr = reinterpret_cast<T* (*)()>(dlsym(handler, symName.c_str()))))
      {
        std::cerr << "[DynamicLoader::Load] failure : " << dlerror() << std::endl;
        return (NULL);
      }
    return std::unique_ptr<T>(fPtr());
  }

#elif defined(_MSC_VER_)
  template<typename T>
  std::unique_ptr<T>		_windowsLoad(const std::string &libName, const std::string &symName)
  {
    T				*(*fPtr)();
    std::string			newLibName = libName + ".dll";
    void			*handler = (void*)LoadLibrary(newLibName);

    _hashHandler[symName] = handler;
    if (!(fPtr = reinterpret_cast<T* (*)()>(GetProcAddress((HINSTANCE)handler, symName))))
      {
        std::cerr << "[DynamicLoader::Load] failure : " << std::endl;
        return (NULL);
      }
    return std::unique_ptr<T>(fPtr());
  }

#endif
  template<typename T>
  std::unique_ptr<T>		load(const std::string &libName, const std::string &symName)
  {
    if (_hashHandler[symName])
      return (NULL);
    else
      {
#if defined(__GNUC__)
	return (_unixLoad<T>(libName, symName));
#elif defined(_MSC_VER_)
	return(_windowsLoader<T>(libName, symName));
#endif
      }
  }

  void				close(const std::string &symName) {
#if defined(__GNUC__)
    dlclose(_hashHandler[symName]);
#elif defined(_MSC_VER_)
    FreeLibrary((HINSTANCE)_hashHandler[symName]);
#endif
    _hashHandler.erase(symName);
  }
};

#endif		// !__DYNAMIC_LOADER_HPP__
