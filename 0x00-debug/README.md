This project is to test that GDB (g++ debugger) works in VSCode, following tutorial from `https://code.visualstudio.com/docs/cpp/config-mingw`

1. Make sure `tasks.json` includes the correct values, e.g.:

```json
{
  "version": "2.0.0",
  "tasks": [
    {
      "type": "shell",
      "label": "WY make", // name must match in launch.json
      "command": "C:\\MinGW\\bin\\mingw32-make.exe",
      "args": [],
      "options": {
        "cwd": "${fileDirname}"
      },
      "problemMatcher": ["$gcc"],
      "group": {
        "kind": "build",
        "isDefault": true
      }
    },
    {
      "type": "shell",
      "label": "WY g++",
      "command": "C:\\MinGW\\bin\\g++.exe",
      "args": [
        "-g", // adds debugger info to the binary, important!
        "${file}",
        "-o",
        "${fileDirname}\\bin\\${fileBasenameNoExtension}.exe"
      ],
      "options": {
        "cwd": "C:\\MinGW\\bin"
      },
      "problemMatcher": ["$gcc"],
      "group": {
        "kind": "build",
        "isDefault": true
      }
    }
  ]
}
```

2. Configure `launch.json`, e.g.:

```json
{
  "version": "0.2.0",
  "configurations": [
    {
      "name": "g++.exe build and debug active file",
      "type": "cppdbg",
      "request": "launch",
      "program": "${fileDirname}\\bin\\${fileBasenameNoExtension}.exe", // make sure path is correct in order to target the built binary
      "args": [],
      "stopAtEntry": false,
      "cwd": "${workspaceFolder}",
      "environment": [],
      "externalConsole": false,
      "MIMode": "gdb",
      "miDebuggerPath": "C:\\MinGW\\bin\\gdb.exe", // required for debugging
      "setupCommands": [
        {
          "description": "Enable pretty-printing for gdb",
          "text": "-enable-pretty-printing",
          "ignoreFailures": true
        }
      ],
      "preLaunchTask": "WY make" // must match `label` in `tasks.json`
    }
  ]
}
```

3. Add breakpoints in the code

4. Press `F5` within the entry file, e.g. `main.cpp`, to run the `launch.json` (which will run the `preLaunchTask` to build the binary before launching)

5. Alternatively, press `CTRL + SHIFT + B` to just run `tasks.json`, to make sure the build works.
