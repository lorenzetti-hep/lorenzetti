__all__ = [ "Pool","Slot", "chunks"]


from GaugiKernel import LoggingLevel, Logger
from GaugiKernel.macros import *
from pprint import pprint
import argparse
import os, time
import subprocess

def chunks(lst, n):
    """Yield successive n-sized chunks from lst."""
    for i in range(0, len(lst), n):
        yield lst[i:i + n]



class Slot(object):

  def __init__( self ):
    self.__proc = None
    self.__lock = False

  def lock(self):
    self.__lock=True
  
  def unlock(self):
    self.__lock=False

  def update(self):
    if self.__proc and not self.__proc.poll():
      self.unlock()

  def run(self, command, dry_run=False):
    pprint(command)
    time.sleep(2)
    if not dry_run:
      self.__proc = subprocess.Popen(command, shell=True, env=os.environ)
      self.lock()

  def isAvailable(self):
    if self.__proc:
      if not self.__proc.poll() is None:
        self.unlock()
    return not self.__lock





class Pool( Logger ):

  def __init__(self, func, inputs,maxJobs, output, dry_run):
    
    Logger.__init__(self)
    self.__inputs = inputs
    self.__func = func
    self.__slots = [Slot() for _ in range(maxJobs)]
    self.__output = output
    self.__outputs = []
    self.dry_run = dry_run

  def getAvailable(self):
    for slot in self.__slots:
      if slot.isAvailable():
        return slot
    return None

  
  def busy(self):
    for slot in self.__slots:
      if not slot.isAvailable():
        return True
    return False


  def generate(self):
    # prepare the command job
    inputs = self.__inputs.pop()
    idx = len(self.__inputs) # output label number
    output = self.__output + '.' + str(idx)
    command = self.__func(inputs, output + '.tmp')
    # remove tmp name when complete the job
    command += f' && mv {output}.tmp {output}'
    self.__outputs.append(output)
    print(command)
    return command, output


  #
  # Run jobs
  #
  def run( self ):

    while len(self.__inputs) > 0:
      slot = self.getAvailable()
      if slot:
        command, output = self.generate()
        if os.path.exists(output):
          MSG_WARNING(self, f"File {output} exist. Skip.")
          continue
        slot.run( command , self.dry_run)
    
    while self.busy():
      continue

  def merge(self):
    command = "hadd -f "+self.__output
    for fname in self.__outputs:
      command += ' '+fname
    os.system(command)
    for fname in self.__outputs:
      os.system( 'rm -rf '+fname)




class Process:

    def __init__(
        self, 
        func, 
        args, 
        kwargs, 
        workarea     : str=tempfile.mktemp(),
        logname      : str="output.log"
    ):
        
        self.func           = func
        self.args           = args
        self.kwargs         = kwargs
        self.workarea       = workarea
        self.__proc         = None
        self.__mon_thread   = None
        self.__proc_stat    = None
        self.resultval      = None
        self.__broken       = False
        self.__killed       = False
        self.__pending      = True
        os.makedirs(workarea, exist_ok=True)
        self.log_path       = workarea+'/'+logname
        self.__manager      = multiprocessing.Manager()
        self.__returnval    = self.__manager.dict()

    @property
    def exitcode(self):
        if self.__proc:
            return self.__proc.exitcode
        return None
    



    def run_async(self) -> int:

        self.__pending=False
        def run_proc( returnval, logpath, func, args, kwargs):
          try:
            val = func(*args, **kwargs)
            returnval["result"]=val
            sys.exit(0)
          except Exception:
            with open(logpath, "w") as f:
              f.write(traceback.format_exc())
            sys.exit(1)
      
        args            = (self.__returnval, self.log_path, self.func, self.args, self.kwargs)
        self.__proc     = multiprocessing.Process(target=run_proc, args=args)
        self.__proc.start()     
        self.__mon_thread = Monitor(self.__proc)
        self.__mon_thread.start()
        self.__proc_stat = psutil.Process(self.__proc.pid)
        logger.debug("starting process")
        broken = self.status() == "failed"
        self.__broken = broken
        return not broken # Lets considering the first seconds as broken

       
    def join(self):
        while self.is_alive():
            sleep(1)


    def result(self):
        if not self.resultval:
          self.resultval = self.__returnval["result"] if "result" in self.__returnval.keys() else None
        return self.resultval


    def is_alive(self):
        return True if (self.__proc and self.__proc.is_alive()) else False


    def kill(self):
        if self.is_alive() and self.__proc:
            children = self.__proc_stat.children(recursive=True)
            for child in children:
                p=psutil.Process(child.pid)
                p.kill()
            self.__proc.kill()
            self.__killed=True
        else:
            self.__killed=True


    def status(self):
        if self.is_alive():
            return "running"
        elif self.__pending:
            return "pending"
        elif self.__killed:
            return "killed"
        elif self.__broken:
            return "broken"
        elif (self.exitcode and  self.exitcode>0):
            return "failed"
        else:
            return "completed"

    def traceback_print_exc(self):
       with open(self.log_path,'r') as f:
          print(f.read())