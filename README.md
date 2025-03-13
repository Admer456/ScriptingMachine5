
# Scripting Machine V

*Scripting Machine V / ScriptingMachine5 / sm5 / SMV* is a simple low-level library that lets you load .NET assemblies (read: C# DLLs) from a C++ app. It's ideal for game engines, and is currently being used in Sharp-Life, a port of Half-Life SDK to C#.

Stuff is very very early right now, so there's no documentation or build instructions! The build process for the 2 existing examples isn't exactly straightforward, because you need to manually copy `example.runtimeconfig.json` to your working directory, as well as configure the working dir when debugging the examples, for example `Example1_BasicUsage/ManagedLibrary/bin/Debug/net8.0/`.

Stay tuned for further developments!

*If you want a fully functional, higher-level library, then try out [Coral](https://github.com/StudioCherno/Coral)! SMV mostly exists because I couldn't get Coral to work in x86, and I didn't need most of its features.*

## Licence

Licenced under MIT. Contains code from the .NET Foundation (hostfxr.h and coreclr_delegates.h) and StudioCherno's Coral library (code to locate hostfxr.dll).
