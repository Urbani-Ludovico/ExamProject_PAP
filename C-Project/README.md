# Raytracing

This code implements a simple Raytracing for an environment including only sphere objects.

## Run syntax

```bash
build/main scene_path out_path image_width image_height
```
Where:
- `scene_path`: Path to file.txt with scene parameters
- `out_path`: Path used to save image
- `image_width` and `image_height`: Size of image

Example as in test `t1a`
```bash
build/main tests/t1.txt tests/t1_1920x1080.ppm 1920 1080
```

### Makefile commands

| Target | Description     |
|--------|-----------------|
| main   | Compile project |
| test   | Run all tests   |

## Implementation notes

If available the program uses AVX2 vectorialiation.

## Errors

| Error code | Type                 | Description                        |
|------------|----------------------|------------------------------------|
| 1          |                      | Unknown error                      |
| 2          | Missing arguments    | Wrong arguments number             |
| 3          | Value Error          | Incorrect arguments                |
| 4          | IO Error             | Error while opening file           |
| 5          | Malformed scene file |                                    |
| 6          | Allocation error     | Malloc return NULL                 |
| 7          | Trucate Error        | Error with ftruncate function      |
| 8          | Write Error          | Error while writing data to file   |
| 8          | Memory Map Failed    | Function mmap returns a MAP_FAILED |