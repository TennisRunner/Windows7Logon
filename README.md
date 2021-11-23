# Windows7Logon
Logs into the Windows 7 Lock Screen so you can programatically unlock the windows 7 lock screen

# How it works

Once injected, it reads the password to type from the file `passwordFileName`. 
You should change this file name or use another method to pass the password to the dll such as using 
RPM, WPM, dll exports, shared memory, sockets, or enviorment variables.
I used a text file since its a fast and simple solution.

It finds the "Logon Window" using `FindWindow`. 
Only the LogonUI.exe process and proceses started by it will have access to the window handles.

From there, it sets focus to it and uses `SendInput` to simulate typing in the password and the return key.
The dll has no error reporting so if anything goes wrong, you'll need to debug it yourself.

After its attempt, it unloads the dll so it can be retried if nessesary. 

# How to use it

Inject the dll into the LogonUI.exe when the lock screen is shown.
You can use `MemoryMapping`, `SetWindowsHook`, or `CreateRemoteThread` passing the file name address as the lparam, after writing the string 
to an allocated page of process's memory. 
