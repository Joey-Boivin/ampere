## Build

```shell
mkdir -p build
cd build
cmake -D CMAKE_BUILD_TYPE=Debug ..
make
```

## Tools
Make sure you are located at the project root in the terminal before executing the following commands
### Run in Xephyr session

```shell
chmod u+x tools/xephyr-session.sh
tools/xephyr-session.sh
```

### Format code


```shell
python tools/format.py
```

### Run valgrind
```shell
chmod u+x tools/run-valgrind.sh
tools/run-valgrind.sh
```
