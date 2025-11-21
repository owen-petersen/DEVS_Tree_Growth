Requirements:

Make sure you have the following installed:

Ubuntu / Debian
sudo apt update
sudo apt install g++ cmake git

Cadmium V2

This repo already bundles all needed Cadmium V2 headers inside main/cadmium/core.
No external library installation is required.

How to Build & Run
1. Clone the repository
git clone https://github.com/vaanuthanesh2003/DEVS_Tree_Growth.git
cd DEVS_Tree_Growth

2. Run the build script
./build.sh

This script:

- creates a build/ directory

- compiles all sources with CMake

- produces the executable:

build/main/tree_sim

Run the simulation

From inside build/main:

./tree_sim

View JSON output

The logger writes packets into:

bin/tree_output.json


Open it:

cat bin/tree_output.json


Example snippet:

{
"packet": 1,
"entries": [
  { "t": 0, "height": 3.0, "crown": 1.0 },
  { "t": 1, "height": 6.0, "crown": 2.0 },
  { "t": 2, "height": 9.0, "crown": 3.0 },
  { "t": 3, "height": 12.0, "crown": 4.0 },
  { "t": 4, "height": 15.0, "crown": 5.0 },
  { "t": 5, "height": 18.0, "crown": 6.0 },
  { "t": 6, "height": 21.0, "crown": 7.0 },
  { "t": 7, "height": 24.0, "crown": 8.0 },
  { "t": 8, "height": 27.0, "crown": 9.0 },
  { "t": 9, "height": 30.0, "crown": 10.0 },
  { "t": 10, "height": 33.0, "crown": 11.0 },
  { "t": 11, "height": 36.0, "crown": 12.0 },
  { "t": 12, "height": 39.0, "crown": 13.0 },
  { "t": 13, "height": 42.0, "crown": 14.0 },
  { "t": 14, "height": 45.0, "crown": 15.0 },
  { "t": 15, "height": 48.0, "crown": 16.0 },
  { "t": 16, "height": 51.0, "crown": 17.0 },
  { "t": 17, "height": 54.0, "crown": 18.0 },
  { "t": 18, "height": 57.0, "crown": 19.0 },
  { "t": 19, "height": 60.0, "crown": 20.0 }
]}
