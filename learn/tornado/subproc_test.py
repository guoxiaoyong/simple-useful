import subprocess, signal

#signal.signal(signal.SIGCHLD, signal.SIG_IGN)
p = subprocess.Popen(['echo','foo'])
while p.poll() is None:
   pass # wait for the process to exit
