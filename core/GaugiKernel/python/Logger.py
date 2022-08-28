
__all__ = ["LoggingLevel", "Logger"]

import logging
from GaugiKernel import EnumStringification


# This won't handle print and sys.stdout, but most of the cases are handled.
_nl = True

def nlStatus():
  global _nl
  return _nl

def resetNlStatus():
  global _nl
  _nl = True

class LoggingLevel(EnumStringification):
  """
    A wrapper for logging levels, which allows stringification of known log
    levels.
  """
  VERBOSE  = logging.DEBUG - 1
  DEBUG    = logging.DEBUG
  INFO     = logging.INFO
  WARNING  = logging.WARNING
  ERROR    = logging.ERROR
  CRITICAL = logging.CRITICAL
  FATAL    = logging.CRITICAL
  MUTE     = logging.CRITICAL # MUTE Still displays fatal messages.


logging.addLevelName(LoggingLevel.VERBOSE, "VERBOSE")
logging.addLevelName(LoggingLevel.FATAL,    "FATAL" )

def verbose(self, message, *args, **kws):
  """
    Attempt to emit verbose message
  """
  if self.isEnabledFor(LoggingLevel.VERBOSE):
    self._log(LoggingLevel.VERBOSE, message, args, **kws)

class FatalError(RuntimeError):
  pass

def _getAnyException(args):
  exceptionType = [issubclass(arg,BaseException) if type(arg) is type else False for arg in args]
  Exc = None
  if any(exceptionType):
    # Check if any args message is the exception type that should be raised
    args = list(args)
    Exc = args.pop( exceptionType.index( True ) )
    args = tuple(args)
  return Exc, args

def warning(self, message, *args, **kws):
  Exc, args = _getAnyException(args)
  if self.isEnabledFor(LoggingLevel.WARNING):
    self._log(LoggingLevel.WARNING, message, args, **kws)
  if Exc is not None:
    if args:
      raise Exc(message % (args if len(args) > 1 else args[0]))
    else:
      raise Exc(message)

def error(self, message, *args, **kws):
  Exc, args = _getAnyException(args)
  if self.isEnabledFor(LoggingLevel.ERROR):
    self._log(LoggingLevel.ERROR, message, args, **kws)
  if Exc is not None:
    if args:
      raise Exc(message % (args if len(args) > 1 else args[0]))
    else:
      raise Exc(message)

def fatal(self, message, *args, **kws):
  """
    Attempt to emit fatal message
  """
  Exc, args = _getAnyException(args)
  if Exc is None: Exc = FatalError
  if self.isEnabledFor(LoggingLevel.FATAL):
    self._log(LoggingLevel.FATAL, message, args, **kws)
  if args:
    raise Exc(message % (args if len(args) > 1 else args[0]))
  else:
    raise Exc(message)

logging.Logger.verbose = verbose
logging.Logger.warning = warning
logging.Logger.error = error
logging.Logger.fatal = fatal
logging.Logger.critical = fatal


def _getFormatter():
  class Formatter(logging.Formatter):
    import numpy as np
    gray, red, green, yellow, blue, magenta, cyan, white = ['0;%d' % int(d) for d in (30 + np.arange(8))]
    bold_black, bold_red, bold_green, bold_yellow, bold_blue, bold_magenta, bold_cyan, bold_white = ['1;%d' % d for d in 90 + np.arange(8)]
    gray = '1;30'
    reset_seq = "\033[0m"
    color_seq = "\033[%(color)sm"
    colors = {
               'VERBOSE':  gray,
               'DEBUG':    cyan,
               'INFO':     green,
               'WARNING':  bold_yellow,
               'ERROR':    red,
               'CRITICAL': bold_red,
               'FATAL':    bold_red,
             }

    # It's possible to overwrite the message color by doing:
    # logger.info('MSG IN MAGENTA', extra={'color' : Logger._formatter.bold_magenta})

    def __init__(self, msg, use_color = False):
      if use_color:
        logging.Formatter.__init__(self, self.color_seq + msg + self.reset_seq )
      else:
        logging.Formatter.__init__(self, msg)
      self.use_color = use_color

    def format(self, record):
      if not(hasattr(record,'nl')):
        record.nl = True
      levelname = record.levelname
      if not 'color' in record.__dict__ and self.use_color and levelname in self.colors:
        record.color = self.colors[levelname]
      return logging.Formatter.format(self, record)
  import os, sys

  formatter = Formatter(
                       "%(asctime)s | Py.%(name)-33.33s %(levelname)7.7s %(message)s",
                       not(int(os.environ.get('RCM_NO_COLOR',1)) or not(sys.stdout.isatty()))
                       )
  return formatter

# create console handler and set level to notset
def _getConsoleHandler():
  import sys
  ch = logging.StreamHandler( sys.__stdout__ )
  ch.setLevel( logging.NOTSET ) #  Minimal level in which the ch will print
  ch.setFormatter(_getFormatter())
  return ch

console = _getConsoleHandler()


class Logger( object ):
  """
    Simple class for giving inherited classes logging capability as well as the
    possibility for being serialized by pickle.
    Logger will keep its logging level even after unpickled.
  """

  def __init__(self, d={}, **kw ):
    """
      Retrieve from args the logger, or create it using default configuration.
    """

    
    #if 'level' in d:
    #  if d['level'] not in None:
    #    self.level = retrieve_kw(d, 'level', LoggingLevel.INFO)
    #  else:
    #    d.pop('level')
    #else:
    self.level = LoggingLevel.INFO
    self._logger = self.getModuleLogger( self.__class__.__name__, self.getLevel() )
    self._logger.verbose('Initialiazing %s', self.__class__.__name__)

 
  def __getattr__(self, attr):
    if attr.startswith('_') and  attr.lstrip('_') in ( 'verbose', 'debug', 'info'
                                                     , 'warning', 'error', 'critical'
                                                     , 'fatal'):
      return getattr( self._logger, attr.lstrip('_') )
    raise AttributeError( 'AttributeError was raised inside an instance of Logger class while attempting to get: %s' % attr )

  
  @classmethod
  def getModuleLogger(cls, logName, logDefaultLevel = LoggingLevel.INFO):
    """
      Retrieve logging stream handler using logName and add a handler
      to stdout if it does not have any handlers yet.
      Format logging stream handler to output in the same format used by Athena
      messages.
    """
    # Retrieve the logger
    logger = logging.getLogger( logName )
    # add ch to logger
    logger.addHandler(console)
    logger.setLevel( logDefaultLevel )
    return logger

 
  def getLevel(self):
    return self.level

  def setLevel(self, value):
    if value not in (None):
      self.level = value
      if self._logger.level != self.level:
        self._logger.setLevel(self._level)



