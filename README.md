# HOT RELOADING in `C` and `raylib`

> [!WARNING]
> This repo is in development and will change in future.

Step 1. Compile `main.c` with raylib as dll or so. I am using cmake here.

```bash
$ cmake --build build
```

Step 2. Write your own `game.c` (update logic) and implement the interface in `game.h`.

Step 3. Compile `game.c` with `game.h`, `raylib.h` as a dll or so, where raylib will be a dll/so.

```bash
$ gcc -shared -o build\game.dll game.c .\build\libraylib.dll  -I.\build\vendored\raylib\raylib\include
```

> In above, I already built raylib as dll with cmake (`.\build\libraylib.dll`). The path to the `raylib.h` is in `.\build\vendored\raylib\raylib\include` this dir.
> Finally, I am saving the `game.dll` in the `build` dir along with the raylib dll for the dll to resolve.

Step 4. Finally run your dll with `main.exe`.

```bash
$ .\build\main.exe .\build\game.dll
```

Now when you want to change something you can just make the changes in `game.c` and then compile it as dll. The `main.exe` will reload the animation without you manually stoping and restarting the animation.

> [!CAUTION]
> BUG: If you try to reload the animation while it is still reloading, the window flashes infinitely and the program will go into infinite loop.
