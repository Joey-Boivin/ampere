### Build

```shell
mkdir -p build
cd build
cmake -D CMAKE_BUILD_TYPE=Debug ..
make
```

### Run in Xephyr session

Make sure you are located at the project root in the terminal before executing the following command
```shell
chmod u+x tools/xephyr-session.sh
tools/xephyr-session.sh
```

### Format code
Make sure you are located at the project root in the terminal before executing the following command

```shell
python tools/format.py
```
