# Custom_Shell

This program is a basic shell created in C. It gives you a command-line interface where you can run commands, including built-in ones like `cd`, `pwd`, `history`, and `exit`. You can also add extra paths as arguments when you start the shell. These paths will be added to the `PATH` environment variable, letting you run more commands directly from the shell.

## Installation

1. Clone this repository to your local machine and enter to "Custom_Shell" folder:

    ```bash
    cd Chess_Simulator_Bash
    ```
2. Make the script executable:

    ```bash
    chmod +x ./myshell.c
    ```    
3. Compile the code:

   ```bash
    gcc -o myshell myshell.c
    ```
   
## Features
- Runs external commands using `execvp`.
- Includes built-in commands like `cd`, `pwd`, `history`, and `exit`.
- Keeps a history of commands, with a limit on how many are saved.
- Lets you set the `PATH` environment variable dynamically.

## Usage
1. **Compile**: Use a C compiler like GCC to compile the program.
   ```bash
   gcc -o myshell myshell.c
   ```
2. **Run**: Start the shell by running the compiled file.
   ```bash
   ./myshell
   ```
3. **Commands**:
   - External commands: Type any external command available on your system.
   - Built-in commands:
     - `cd [directory]`: Change the current directory.
     - `pwd`: Show the current working directory.
     - `history`: Show the command history.
     - `exit`: Close the shell.

4. **Additional Arguments**: You can add extra paths as arguments when you start the shell to include them in the `PATH` environment variable. For example:
   ```bash
   ./myshell /usr/local/bin /opt/bin
   ```
This lets you run commands found in `/usr/local/bin` and `/opt/bin` directly from the shell.
