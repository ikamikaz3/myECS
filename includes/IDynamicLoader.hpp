#ifndef				__IDYNAMIC_LOADER_HPP__
# define			__IDYNAMIC_LOADER_HPP__

# include			<map>
# include			<string>
# include			<iostream>

class				IDynamicLoader
{
public:
// #if defined(__UNIX__)
  typedef void (*ptrToF)(EntityManager &, SystemManager&, MessageBus &);
// #elif defined (_WIN32) || defined(WIN32)
    //typedef void (__cdecl *ptrToF)(EntityManager &, SystemManager&, MessageBus &);
// #endif
  typedef ptrToF	(*f)();

  virtual ~IDynamicLoader() {}
  virtual ptrToF        load(const std::string &libName, const std::string &symName) = 0;
  virtual void			close(const std::string &symName) = 0;
};

#if defined(__GNUC__)
# include			<dlfcn.h>
class				unixDynamicLoader : public IDynamicLoader
{
public:
  unixDynamicLoader() {}
  virtual ~unixDynamicLoader() {
    for (auto it : _hashHandler)
      close(it.first);
  }

  virtual ptrToF                load(const std::string &libName, const std::string &symName)
  {
    f                           fPtr;
    std::string			newLibName = libName;
    void			*handler = dlopen(newLibName.c_str(), RTLD_LAZY);

    if (!handler)
      {
	std::cerr << "[DynamicLoader::Load] failure : " << dlerror() << std::endl;
      }
    _hashHandler[symName] = handler;
	std::cout << "trying to open: " << libName << std::endl;
	std::cout << "newLibName: " << newLibName << std::endl;
	std::cout << "symname: " << symName << std::endl;
	std::cout << "libhandle: " << handler << std::endl;
	std::cout << "pointer: " << dlsym(handler, symName.c_str()) << std::endl;
	if (!(fPtr = reinterpret_cast<f>(dlsym(handler, symName.c_str()))))
      {
	std::cerr << "[DynamicLoader::Load] failure : " << dlerror() << std::endl;
      }
    return fPtr();
  }

  virtual void			close(const std::string &symName)
  {
    dlclose(_hashHandler[symName]);
    _hashHandler.erase(symName);
  }

private:
  std::map<std::string, void *> _hashHandler;

};
# elif		defined(_WIN32) || defined(WIN32)
# include			<windows.h>
class				windowsDynamicLoader : public IDynamicLoader
{
public:
  windowsDynamicLoader() {}
  virtual ~windowsDynamicLoader() {
    for (auto it : _hashHandler)
      close(it.first);
  }
#include <string>
  virtual ptrToF        	load(const std::string &libName, const std::string &symName)
  {
	f	           			fPtr;
    std::string				newLibName = libName;
	HINSTANCE			    handler = LoadLibrary((newLibName.c_str()));

    //_hashHandler[symName] = handler;
	std::cout << "trying to open: " << libName << std::endl;
	std::cout << "newLibName: " << newLibName << std::endl;
	std::cout << "symname: " << symName << std::endl;
	std::cout << "libhandle: " << handler << std::endl;
	//if (!(fPtr = reinterpret_cast<f>(GetProcAddress(GetModuleHandle(TEXT("client_rtype.dll")), "returnLoader"))))
	if (!(fPtr = reinterpret_cast<f>(GetProcAddress(handler, symName.c_str()))))
	{
        std::cerr << "[DynamicLoader::Load] failure : " << symName.c_str() << " libname = " << libName << std::endl;
      }
    return fPtr();
	  //HINSTANCE hGetProcIDDLL = LoadLibrary(libName.c_str());

	  //if (!hGetProcIDDLL) {
		 // std::cout << "could not load the dynamic library" << std::endl;
		 // //return EXIT_FAILURE;
	  //}

		 // f_funci funci = (f_funci)GetProcAddress(hGetProcIDDLL, symName.c_str());
	  //if (!funci) {
		 // std::cout << "could not locate the function" << std::endl;
		 // //return EXIT_FAILURE;
	  //}

	  //std::cout << "funci() returned " << funci() << std::endl;
	  //return (funci());
  }

  virtual void			close(const std::string &symName)
  {
    //FreeLibrary((HINSTANCE)_hashHandler[symName]);
    //_hashHandler.erase(symName);
  }

private:
  std::map<std::string, void *> _hashHandler;

};

#endif

#endif				// !__IDYNAMIC_LOADER_HPP__
