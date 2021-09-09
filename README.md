# Database project 1 - Database management system

## Build project

``` bash
cmake -H. -Bbuild
cd build && make
```

For release:
```bash
cd build && cmake -DCMAKE_BUILD_TYPE=Release .. && make
```
## Run project
```
./build/bin/project 
```

Run test:
```
./build/bin/project_test
```
## Components

### Storage
- disk storage organised & accessed a block as a unit

### Index
- B+ tree

## Logic
1. init disk storage
2. parse data & insert into b+ tree
3. print out stats
4. all the other additional stuff
