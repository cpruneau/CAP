#include "Exceptions.hpp"

using std::cout;
using std::endl;

namespace CAP
{


Exception::Exception(const String & _message, const String & _source)
:
source(_source),
message(_message)
{   }

void Exception::print()
{
  cout << "========================================================" << endl;
  cout << "Exception" << endl;
  cout << "========================================================" << endl;
  cout << "source.......: " << source << endl;
  cout << "message......: " << message << endl;
  cout << "========================================================" << endl;
}

FactoryException::FactoryException(long _currentSize,
                                   long _requestedSize,
                                   const String & _message,
                                   const String & _source)
:
Exception(_message,_source),
currentSize(_currentSize),
requestedSize(_requestedSize)
{   }

void FactoryException::print()
{
  cout << "========================================================" << endl;
  cout << "FactoryException" << endl;
  cout << "========================================================" << endl;
  cout << "source.............: " << source << endl;
  cout << "message............: " << message << endl;
  cout << "currentSize........: " << currentSize << endl;
  cout << "requestedSize......: " << requestedSize << endl;
  cout << "========================================================" << endl;
}

TaskException::TaskException(const String & _message, const String & _source)
:
Exception(_message,_source)
{   }

void TaskException::print()
{
  cout << "========================================================" << endl;
  cout << "TaskException" << endl;
  cout << "========================================================" << endl;
  cout << "source.......: " << source << endl;
  cout << "message......: " << message << endl;
  cout << "========================================================" << endl;
}

MemoryException::MemoryException(const String & _message, const String & _source)
:
Exception(_message,_source)
{   }

void MemoryException::print()
{
  cout << "========================================================" << endl;
  cout << "MemoryException" << endl;
  cout << "========================================================" << endl;
  cout << "source.......: " << source << endl;
  cout << "message......: " << message << endl;
  cout << "========================================================" << endl;
}

MathException::MathException(const String & _message, const String & _source)
:
Exception(_message,_source)
{   }

void MathException::print()
{
  cout << "========================================================" << endl;
  cout << "MathException" << endl;
  cout << "========================================================" << endl;
  cout << "source.......: " << source << endl;
  cout << "message......: " << message << endl;
  cout << "========================================================" << endl;
}

FileException::FileException(const String & _fileName, const String & _message, const String & _source)
:
Exception(_message,_source),
pathName("nil"),
fileName(_fileName)
{   }

FileException::FileException(const String & _pathName, const String & _fileName, const String & _message, const String & _source)
:
Exception(_message,_source),
pathName(_pathName),
fileName(_fileName)
{   }



void FileException::print()
{
  cout << "========================================================" << endl;
  cout << "FileException" << endl;
  cout << "========================================================" << endl;
  cout << "source.......: " << source << endl;
  cout << "message......: " << message << endl;
  cout << "path name....: " << pathName << endl;
  cout << "file name....: " << fileName << endl;
  cout << "========================================================" << endl;
}



HistogramException::HistogramException(const String & _histogramName, const String & _message, const String & _source)
:
Exception(_message,_source),
histogramName(_histogramName)
{   }

void HistogramException::print()
{
  cout << "========================================================" << endl;
  cout << "HistogramException" << endl;
  cout << "========================================================" << endl;
  cout << "source............: " << source << endl;
  cout << "message...........: " << message << endl;
  cout << "histogram name....: " << histogramName << endl;
  cout << "========================================================" << endl;
}





} // namespace CAP

