#ifndef CAP_Exceptions
#define CAP_Exceptions
#include <iostream>
#include "Aliases.hpp"
namespace CAP
{

class Exception
{
public:
  Exception(const String & _message, const String & _source);
  virtual ~Exception() {}
  virtual void print();

protected:
  const String source;
  const String message;
};

class MemoryException : public Exception
{
public:
  MemoryException(const String & _message, const String & _source);
  virtual ~MemoryException() {}
  virtual void print();
};

class FactoryException : public Exception
{
public:
  FactoryException(long _currentSize,
                   long _requestedSize,
                   const String & _message,
                   const String & _source);
  virtual ~FactoryException() {}
  virtual void print();
protected:
  long currentSize;
  long requestedSize;
};

class TaskException : public Exception
{
public:
  TaskException(const String & _message, const String & _source);
  virtual ~TaskException() {}
  virtual void print();
};

class MathException : public Exception
{
public:
  MathException(const String & _message, const String & _source);
  virtual ~MathException() {}
  virtual void print();
};

class FileException : public Exception
{
public:
  FileException(const String & _fileName, const String & _message, const String & _source);
  FileException(const String & _pathName, const String & _fileName, const String & _message, const String & _source);
  virtual ~FileException() {}
  virtual void print();
protected:
  const String pathName;
  const String fileName;
};


class HistogramException : public Exception
{
public:
  HistogramException(const String & _histogramName, const String & _message, const String & _source);
  virtual ~HistogramException() {}
  virtual void print();
protected:
  const String histogramName;
};




} // namespace CAP

#endif
