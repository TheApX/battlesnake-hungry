# Build, test and run

Configure:
```
cmake --config Release -H./src -B./build
```

Build:
```
cmake --build ./build --config Release -j 8 \
    --target battlesnake_hungry \
    --target battlesnake_hungry_test \
    --target battlesnake_hungry_test_data
```

Test:
```
cd /build/ && /build/battlesnake_hungry_test
```
Run:
```
./build/battlesnake_hungry
```

Or just build and run it in a Docker container:

```
docker-compose build
docker-compose run
```

Once started, the web-server listens on port `12388`. You can change it in [source code](../main.cpp).

[Back to contents](../README.md#contents)
