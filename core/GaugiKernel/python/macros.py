

__all__ = [
           "MSG_VERBOSE",
           "MSG_DEBUG",
           "MSG_INFO",
           "MSG_WARNING",
           "MSG_ERROR",
           "MSG_FATAL",
           ]

def MSG_VERBOSE( self, msg, *args):
  self._logger.verbose(msg,*args)

def MSG_DEBUG( self, msg, *args):
  self._logger.debug(msg,*args)

def MSG_INFO( self, msg, *args):
  self._logger.info(msg,*args)

def MSG_WARNING( self, msg, *args):
  self._logger.warning(msg,*args)

def MSG_ERROR( self, msg, *args):
  self._logger.error(msg,*args)

def MSG_FATAL( self, msg, *args):
  self._logger.fatal(msg,*args)


