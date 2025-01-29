# Project
 
README.txt

" This project contains two main directories:
" 1. client: This directory contains the source code and implementation files.
" 2. clientApplication: This directory acts as the run environment for executing the server-client setup.

" ---------------------------------------------------------------
" How to Build the Executable
" ---------------------------------------------------------------

" There are two ways to build the executable:

" 1. Using the `build.sh` script in the `clientApplication` directory:
"    - Navigate to the `clientApplication` directory.
"    - Run the `build.sh` script. This script will:
"      - Start the build process.
"      - Copy the built binary and the script into the `clientApplication` directory.
  
" 2. Manual Build:
"    - Navigate to the `client` directory.
"    - Create a `build` directory inside the `client` directory.
"    - Inside the `build` directory, run the following commands:
"      cmake ..
"      make
"    - After this, the built binary will be placed in the `client/bin` directory.

" ---------------------------------------------------------------
" How to Run the Executable
" ---------------------------------------------------------------

" Once you have built the binary and placed it in the `clientApplication` directory,
" alongside the `runclientApplication.sh` script (which is located in the `client/scripts` directory),
" you can start the client-server process:

" 1. Running the client-server:
"    - In the `clientApplication` directory, execute the `runclientApplication.sh` script.
"    - This will start the client-server process and begin processing the data.
  
" 2. Logs and Output:
"    - During execution, run-time logs will be generated and stored in the `logs` directory.
"    - After processing, a `packets.json` file will be generated as the output.

" 3. Handling Previous Outputs:
"    - When you run the binary again, the previous `packets.json` file will be moved
"      to a new directory named `archiveOutputFile` for archiving purposes.

" ---------------------------------------------------------------
" Directory Structure
" ---------------------------------------------------------------

" client/:
"    - Contains the source code and implementation files.
"    - bin/: Directory where the final built binary will be placed.
"    - scripts/: Contains the script files such as `runclientApplication.sh`.
  
" clientApplication/:
"    - The run environment where the built binary and scripts are placed.
"    - logs/: Contains runtime logs generated during execution.
"    - archiveOutputFile/: Stores the previous `packets.json` files.
